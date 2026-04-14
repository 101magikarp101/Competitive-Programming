#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
import os
import re
import resource
import shlex
import signal
import subprocess
import sys
import tempfile
import textwrap
import time
import warnings
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Sequence, Tuple

EXIT_AC = 42
EXIT_WA = 43

warnings.filterwarnings(
    "ignore",
    category=DeprecationWarning,
    message=r".*use of fork\(\) may lead to deadlocks in the child.*",
)

ASCII_WHITESPACE_RE = re.compile(br"[ \t\n\r\f\v]+")
FLOAT_TOKEN_RE = re.compile(
    rb"^[+-]?(?:(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?)$"
)
ASCII_UPPER = bytes(range(65, 91))
ASCII_LOWER = bytes(range(97, 123))
ASCII_CASEMAP = bytes.maketrans(ASCII_UPPER, ASCII_LOWER)


class Colors:
    GREEN = "\033[92m"
    RED = "\033[91m"
    YELLOW = "\033[93m"
    BLUE = "\033[94m"
    BOLD = "\033[1m"
    RESET = "\033[0m"


def want_color(no_color: bool) -> bool:
    return (not no_color) and sys.stdout.isatty()


def colorize(s: str, color: str, use_color: bool) -> str:
    if not use_color:
        return s
    return f"{color}{s}{Colors.RESET}"


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def natural_key(s: str):
    return [int(tok) if tok.isdigit() else tok.lower() for tok in re.split(r"(\d+)", s)]


@dataclass
class DefaultValidatorConfig:
    case_sensitive: bool = False
    space_change_sensitive: bool = False
    float_relative_tolerance: Optional[float] = None
    float_absolute_tolerance: Optional[float] = None

    @property
    def has_float_tolerance(self) -> bool:
        return self.float_relative_tolerance is not None or self.float_absolute_tolerance is not None


@dataclass
class ProblemConfig:
    problem_dir: Path
    name: str
    time_limit_seconds: float
    memory_limit_mb: int
    validation: str
    validator_flags_text: str
    default_validator: DefaultValidatorConfig
    has_custom_validator: bool

    @property
    def interactive(self) -> bool:
        return "interactive" in self.validation.lower()


@dataclass
class TestCase:
    suite: str
    input_path: Path
    answer_path: Path

    @property
    def label(self) -> str:
        return f"{self.suite}/{self.input_path.name}"


@dataclass
class RunResult:
    exit_code: Optional[int]
    signal_num: Optional[int]
    timed_out: bool
    time_seconds: float
    memory_kb: int
    stderr_text: str


@dataclass
class InteractiveRunBundle:
    contestant: RunResult
    validator_exit_code: Optional[int]
    validator_signal_num: Optional[int]
    validator_timed_out: bool
    validator_stderr_text: str
    validator_detail: str


@dataclass
class JudgeResult:
    verdict: str
    short_status: str
    detail: str


class RunnerError(Exception):
    pass


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def read_bytes(path: Path) -> bytes:
    return path.read_bytes()


def parse_float_from_token(token: bytes) -> Optional[float]:
    if not FLOAT_TOKEN_RE.fullmatch(token):
        return None
    try:
        value = float(token.decode("ascii"))
    except (UnicodeDecodeError, ValueError, OverflowError):
        return None
    if not math.isfinite(value):
        return None
    return value


def parse_default_validator_flags(flag_text: str) -> DefaultValidatorConfig:
    cfg = DefaultValidatorConfig()
    if not flag_text.strip():
        return cfg

    tokens = shlex.split(flag_text)
    i = 0
    saw_float_tolerance = False
    saw_rel = False
    saw_abs = False
    while i < len(tokens):
        tok = tokens[i]
        if tok == "case_sensitive":
            cfg.case_sensitive = True
            i += 1
            continue
        if tok == "space_change_sensitive":
            cfg.space_change_sensitive = True
            i += 1
            continue
        if tok in {"float_tolerance", "float_relative_tolerance", "float_absolute_tolerance"}:
            if i + 1 >= len(tokens):
                raise RunnerError(f"Missing value after validator flag {tok!r}")
            try:
                eps = float(tokens[i + 1])
            except ValueError as exc:
                raise RunnerError(f"Could not parse tolerance value for validator flag {tok!r}") from exc
            if eps < 0 or not math.isfinite(eps):
                raise RunnerError(f"Invalid tolerance {tokens[i + 1]!r} for validator flag {tok!r}")
            if tok == "float_tolerance":
                if saw_float_tolerance or saw_rel or saw_abs:
                    raise RunnerError(
                        "Invalid validator_flags: float_tolerance cannot be repeated or combined with "
                        "float_relative_tolerance/float_absolute_tolerance"
                    )
                saw_float_tolerance = True
                cfg.float_relative_tolerance = eps
                cfg.float_absolute_tolerance = eps
            elif tok == "float_relative_tolerance":
                if saw_float_tolerance or saw_rel:
                    raise RunnerError(
                        "Invalid validator_flags: float_relative_tolerance cannot be repeated or combined with float_tolerance"
                    )
                saw_rel = True
                cfg.float_relative_tolerance = eps
            else:
                if saw_float_tolerance or saw_abs:
                    raise RunnerError(
                        "Invalid validator_flags: float_absolute_tolerance cannot be repeated or combined with float_tolerance"
                    )
                saw_abs = True
                cfg.float_absolute_tolerance = eps
            i += 2
            continue
        raise RunnerError(f"Unsupported validator flag {tok!r}")
    return cfg


def parse_problem_config(problem_dir: Path) -> ProblemConfig:
    yaml_path = problem_dir / "problem.yaml"
    if not yaml_path.exists():
        raise RunnerError(f"{problem_dir} does not contain problem.yaml")

    yaml_text = read_text(yaml_path)
    name_match = re.search(r"(?m)^name:\s*(.+?)\s*$", yaml_text)
    validation_match = re.search(r"(?m)^validation:\s*(.+?)\s*$", yaml_text)
    validator_flags_match = re.search(r"(?m)^validator_flags:\s*(.*?)\s*$", yaml_text)
    memory_match = re.search(r"(?m)^\s*memory:\s*(\d+)\s*$", yaml_text)
    multiplier_match = re.search(r"(?m)^\s*time_multiplier:\s*([0-9]+(?:\.[0-9]+)?)\s*$", yaml_text)

    timelimit_path = problem_dir / ".timelimit"
    if not timelimit_path.exists():
        raise RunnerError(f"{problem_dir} does not contain .timelimit")

    try:
        base_tl = float(read_text(timelimit_path).strip())
    except ValueError as exc:
        raise RunnerError(f"Could not parse time limit in {timelimit_path}") from exc

    multiplier = float(multiplier_match.group(1)) if multiplier_match else 1.0
    memory_mb = int(memory_match.group(1)) if memory_match else 1024
    validation = validation_match.group(1).strip() if validation_match else "default"
    validator_flags_text = validator_flags_match.group(1).strip() if validator_flags_match else ""
    name = name_match.group(1).strip() if name_match else problem_dir.name
    has_custom_validator = (problem_dir / "output_validators" / "validator").is_dir()
    default_validator = parse_default_validator_flags(validator_flags_text)

    return ProblemConfig(
        problem_dir=problem_dir,
        name=name,
        time_limit_seconds=base_tl * multiplier,
        memory_limit_mb=memory_mb,
        validation=validation,
        validator_flags_text=validator_flags_text,
        default_validator=default_validator,
        has_custom_validator=has_custom_validator,
    )


def is_problem_dir(path: Path) -> bool:
    return (path / "problem.yaml").exists() and (path / "data").is_dir()


def discover_problem_dir(solution_path: Path, args) -> Path:
    if args.problem_dir:
        p = Path(args.problem_dir).resolve()
        if not is_problem_dir(p):
            raise RunnerError(f"--problem-dir is not a valid problem package: {p}")
        return p

    if args.root and args.problem:
        p = (Path(args.root).resolve() / args.problem)
        if not is_problem_dir(p):
            raise RunnerError(f"Could not find problem '{args.problem}' under {Path(args.root).resolve()}")
        return p

    cwd = Path.cwd().resolve()
    if is_problem_dir(cwd):
        return cwd

    for ancestor in [solution_path.resolve(), *solution_path.resolve().parents]:
        if ancestor.is_dir() and is_problem_dir(ancestor):
            return ancestor

    if args.root and not args.problem:
        raise RunnerError("You provided --root without --problem")

    raise RunnerError(
        "Could not determine the problem folder automatically. Run the script inside a problem folder, "
        "or pass --problem-dir DIR, or pass --root ROOT --problem NAME."
    )


def collect_tests(problem_dir: Path, suite: str) -> List[TestCase]:
    data_dir = problem_dir / "data"
    suites: List[str]
    if suite == "all":
        suites = [s for s in ["sample", "secret"] if (data_dir / s).is_dir()]
    else:
        suites = [suite]

    tests: List[TestCase] = []
    saw_only_interaction = False
    for s in suites:
        suite_dir = data_dir / s
        if not suite_dir.is_dir():
            continue

        inputs = sorted(suite_dir.glob("*.in"), key=lambda p: natural_key(p.name))
        if inputs:
            for input_path in inputs:
                answer_path = input_path.with_suffix(".ans")
                if not answer_path.exists():
                    raise RunnerError(f"Missing answer file for {input_path}")
                tests.append(TestCase(suite=s, input_path=input_path, answer_path=answer_path))
            continue

        interaction_files = sorted(suite_dir.glob("*.interaction"), key=lambda p: natural_key(p.name))
        if interaction_files:
            saw_only_interaction = True

    if tests:
        return tests

    if saw_only_interaction:
        raise RunnerError(
            f"Found only .interaction transcript files under {problem_dir / 'data'} for suite={suite}. "
            f"Those transcripts are not directly runnable test inputs. Use a suite with packaged .in/.ans files "
            f"(typically secret), or provide a package that includes runnable sample inputs."
        )

    raise RunnerError(f"No tests found for suite={suite} under {problem_dir / 'data'}")



def compile_cpp(source: Path, output_bin: Path, gxx: str, extra_flags: Sequence[str]) -> None:
    cmd = [gxx, str(source), "-O2", "-pipe", "-std=c++20", "-o", str(output_bin), *extra_flags]
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if proc.returncode != 0:
        msg = proc.stderr.strip() or proc.stdout.strip() or "unknown compiler error"
        raise RunnerError("Compilation failed:\n" + msg)


def compile_custom_validator(problem: ProblemConfig, build_dir: Path, gxx: str) -> Optional[Path]:
    validator_dir = problem.problem_dir / "output_validators" / "validator"
    if not validator_dir.is_dir():
        return None

    sources = sorted(
        [p for p in validator_dir.rglob("*") if p.suffix in {".cc", ".cpp", ".cxx", ".c"}],
        key=lambda p: str(p),
    )
    if not sources:
        raise RunnerError(f"No validator source files found under {validator_dir}")

    output_bin = build_dir / "output_validator"
    cmd = [gxx, "-O2", "-pipe", "-std=c++20", "-I", str(validator_dir)] + [str(p) for p in sources] + ["-o", str(output_bin)]
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if proc.returncode != 0:
        msg = proc.stderr.strip() or proc.stdout.strip() or "unknown validator compiler error"
        raise RunnerError("Failed to compile custom validator:\n" + msg)
    return output_bin


def decode_wait_status(status: int) -> Tuple[Optional[int], Optional[int]]:
    if os.WIFEXITED(status):
        return os.WEXITSTATUS(status), None
    if os.WIFSIGNALED(status):
        return None, os.WTERMSIG(status)
    return None, None


def normalize_ru_maxrss_to_kib(raw_ru_maxrss: int) -> int:
    if sys.platform == "darwin":
        return int((raw_ru_maxrss + 1023) // 1024)
    return int(raw_ru_maxrss)


def run_executable(
    exe_path: Path,
    input_path: Path,
    output_path: Path,
    timeout_seconds: float,
    memory_limit_mb: int,
) -> RunResult:
    err_r, err_w = os.pipe()
    stdin_f = open(input_path, "rb")
    stdout_f = open(output_path, "wb")

    pid = os.fork()
    if pid == 0:
        try:
            os.setsid()
            os.dup2(stdin_f.fileno(), 0)
            os.dup2(stdout_f.fileno(), 1)
            os.dup2(err_w, 2)
            stdin_f.close()
            stdout_f.close()
            os.close(err_r)
            os.close(err_w)

            mem_bytes = int(memory_limit_mb * 1024 * 1024)
            try:
                resource.setrlimit(resource.RLIMIT_AS, (mem_bytes, mem_bytes))
            except Exception:
                pass
            try:
                resource.setrlimit(resource.RLIMIT_DATA, (mem_bytes, mem_bytes))
            except Exception:
                pass
            try:
                resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            except Exception:
                pass

            os.execv(str(exe_path), [str(exe_path)])
        except BaseException:
            os._exit(127)

    stdin_f.close()
    stdout_f.close()
    os.close(err_w)

    stderr_chunks: List[bytes] = []
    start = time.perf_counter()
    timed_out = False
    status: Optional[int] = None
    rusage = None

    try:
        os.set_blocking(err_r, False)
    except AttributeError:
        pass

    while True:
        try:
            chunk = os.read(err_r, 65536)
            if chunk:
                stderr_chunks.append(chunk)
        except BlockingIOError:
            pass
        except OSError:
            pass

        waited_pid, waited_status, waited_rusage = os.wait4(pid, os.WNOHANG)
        if waited_pid == pid:
            status = waited_status
            rusage = waited_rusage
            break

        if time.perf_counter() - start > timeout_seconds:
            timed_out = True
            try:
                os.killpg(pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            waited_pid, waited_status, waited_rusage = os.wait4(pid, 0)
            status = waited_status
            rusage = waited_rusage
            break

        time.sleep(0.002)

    while True:
        try:
            chunk = os.read(err_r, 65536)
            if not chunk:
                break
            stderr_chunks.append(chunk)
        except BlockingIOError:
            continue
        except OSError:
            break

    os.close(err_r)

    elapsed = time.perf_counter() - start
    exit_code, signal_num = decode_wait_status(status if status is not None else 0)
    raw_ru_maxrss = int(getattr(rusage, "ru_maxrss", 0) or 0)
    memory_kb = normalize_ru_maxrss_to_kib(raw_ru_maxrss)
    stderr_text = b"".join(stderr_chunks).decode("utf-8", errors="replace")
    return RunResult(
        exit_code=exit_code,
        signal_num=signal_num,
        timed_out=timed_out,
        time_seconds=elapsed,
        memory_kb=memory_kb,
        stderr_text=stderr_text,
    )


def read_feedback_detail(feedback_dir: Path) -> str:
    judge_msg_path = feedback_dir / "judgemessage.txt"
    judge_error_path = feedback_dir / "judgeerror.txt"
    team_msg_path = feedback_dir / "teammessage.txt"
    if judge_msg_path.exists():
        return read_text(judge_msg_path).strip()
    if judge_error_path.exists():
        return read_text(judge_error_path).strip()
    if team_msg_path.exists():
        return read_text(team_msg_path).strip()
    return ""


def run_interactive_case(
    contestant_exe: Path,
    validator_bin: Path,
    test: TestCase,
    timeout_seconds: float,
    memory_limit_mb: int,
    scratch_dir: Path,
) -> InteractiveRunBundle:
    feedback_dir = scratch_dir / "feedback"
    feedback_dir.mkdir(parents=True, exist_ok=True)

    to_contestant_r, to_contestant_w = os.pipe()
    from_contestant_r, from_contestant_w = os.pipe()
    contestant_err_r, contestant_err_w = os.pipe()
    validator_err_r, validator_err_w = os.pipe()

    contestant_pid = os.fork()
    if contestant_pid == 0:
        try:
            os.setsid()
            os.dup2(to_contestant_r, 0)
            os.dup2(from_contestant_w, 1)
            os.dup2(contestant_err_w, 2)

            for fd in [
                to_contestant_r, to_contestant_w, from_contestant_r, from_contestant_w,
                contestant_err_r, contestant_err_w, validator_err_r, validator_err_w,
            ]:
                try:
                    os.close(fd)
                except OSError:
                    pass

            mem_bytes = int(memory_limit_mb * 1024 * 1024)
            try:
                resource.setrlimit(resource.RLIMIT_AS, (mem_bytes, mem_bytes))
            except Exception:
                pass
            try:
                resource.setrlimit(resource.RLIMIT_DATA, (mem_bytes, mem_bytes))
            except Exception:
                pass
            try:
                resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            except Exception:
                pass

            os.execv(str(contestant_exe), [str(contestant_exe)])
        except BaseException:
            os._exit(127)

    validator_pid = os.fork()
    if validator_pid == 0:
        try:
            os.setsid()
            os.dup2(from_contestant_r, 0)
            os.dup2(to_contestant_w, 1)
            os.dup2(validator_err_w, 2)

            for fd in [
                to_contestant_r, to_contestant_w, from_contestant_r, from_contestant_w,
                contestant_err_r, contestant_err_w, validator_err_r, validator_err_w,
            ]:
                try:
                    os.close(fd)
                except OSError:
                    pass

            try:
                resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            except Exception:
                pass

            os.execv(
                str(validator_bin),
                [
                    str(validator_bin),
                    str(test.input_path),
                    str(test.answer_path),
                    str(feedback_dir),
                ],
            )
        except BaseException:
            os._exit(127)

    for fd in [to_contestant_r, to_contestant_w, from_contestant_r, from_contestant_w, contestant_err_w, validator_err_w]:
        try:
            os.close(fd)
        except OSError:
            pass

    for fd in [contestant_err_r, validator_err_r]:
        try:
            os.set_blocking(fd, False)
        except AttributeError:
            pass

    contestant_stderr_chunks: List[bytes] = []
    validator_stderr_chunks: List[bytes] = []
    start = time.perf_counter()

    contestant_status = None
    contestant_rusage = None
    validator_status = None
    validator_timed_out = False
    contestant_timed_out = False

    def drain_fd(fd: int, dest: List[bytes]) -> None:
        while True:
            try:
                chunk = os.read(fd, 65536)
                if not chunk:
                    break
                dest.append(chunk)
            except BlockingIOError:
                break
            except OSError:
                break

    def kill_pid_and_group(pid: int) -> None:
        for target in (pid, -pid):
            try:
                os.kill(target, signal.SIGKILL)
            except ProcessLookupError:
                pass
            except PermissionError:
                pass
            except OSError:
                pass

    while True:
        drain_fd(contestant_err_r, contestant_stderr_chunks)
        drain_fd(validator_err_r, validator_stderr_chunks)

        if contestant_status is None:
            waited_pid, waited_status, waited_rusage = os.wait4(contestant_pid, os.WNOHANG)
            if waited_pid == contestant_pid:
                contestant_status = waited_status
                contestant_rusage = waited_rusage

        if validator_status is None:
            waited_pid, waited_status = os.waitpid(validator_pid, os.WNOHANG)
            if waited_pid == validator_pid:
                validator_status = waited_status

        if contestant_status is not None and validator_status is not None:
            break

        if time.perf_counter() - start > timeout_seconds:
            contestant_timed_out = contestant_status is None
            validator_timed_out = validator_status is None
            if contestant_status is None:
                kill_pid_and_group(contestant_pid)
            if validator_status is None:
                kill_pid_and_group(validator_pid)
            if contestant_status is None:
                waited_pid, waited_status, waited_rusage = os.wait4(contestant_pid, 0)
                if waited_pid == contestant_pid:
                    contestant_status = waited_status
                    contestant_rusage = waited_rusage
            if validator_status is None:
                waited_pid, waited_status = os.waitpid(validator_pid, 0)
                if waited_pid == validator_pid:
                    validator_status = waited_status
            break

        time.sleep(0.002)

    drain_fd(contestant_err_r, contestant_stderr_chunks)
    drain_fd(validator_err_r, validator_stderr_chunks)

    for fd in [contestant_err_r, validator_err_r]:
        try:
            os.close(fd)
        except OSError:
            pass

    elapsed = time.perf_counter() - start
    contestant_exit_code, contestant_signal_num = decode_wait_status(contestant_status if contestant_status is not None else 0)
    raw_ru_maxrss = int(getattr(contestant_rusage, "ru_maxrss", 0) or 0)
    contestant_memory_kb = normalize_ru_maxrss_to_kib(raw_ru_maxrss)

    contestant_run = RunResult(
        exit_code=contestant_exit_code,
        signal_num=contestant_signal_num,
        timed_out=contestant_timed_out,
        time_seconds=elapsed,
        memory_kb=contestant_memory_kb,
        stderr_text=b"".join(contestant_stderr_chunks).decode("utf-8", errors="replace"),
    )
    validator_exit_code, validator_signal_num = decode_wait_status(validator_status if validator_status is not None else 0)

    return InteractiveRunBundle(
        contestant=contestant_run,
        validator_exit_code=validator_exit_code,
        validator_signal_num=validator_signal_num,
        validator_timed_out=validator_timed_out,
        validator_stderr_text=b"".join(validator_stderr_chunks).decode("utf-8", errors="replace"),
        validator_detail=read_feedback_detail(feedback_dir),
    )


def maybe_hide_detail(detail: str, reveal_detail: bool, fallback: str) -> str:
    if not detail:
        return fallback if not reveal_detail else ""
    return detail if reveal_detail else fallback


def judge_interactive_bundle(
    bundle: InteractiveRunBundle,
    memory_limit_mb: int,
    *,
    reveal_detail: bool = True,
) -> JudgeResult:
    if bundle.validator_timed_out and bundle.contestant.timed_out:
        return JudgeResult("TLE", "BUG", f"Time limit exceeded (> {bundle.contestant.time_seconds:.3f}s wall clock)")

    if bundle.validator_exit_code == EXIT_WA:
        return JudgeResult("WA", "WA", maybe_hide_detail(bundle.validator_detail, reveal_detail, "Wrong answer"))
    if bundle.validator_signal_num is not None:
        detail = bundle.validator_detail or bundle.validator_stderr_text.strip() or f"Interactive validator terminated by signal {bundle.validator_signal_num}"
        return JudgeResult("JUDGE", "BUG", maybe_hide_detail(detail, reveal_detail, "Judge error"))
    if bundle.validator_exit_code not in {None, EXIT_AC}:
        detail = bundle.validator_detail or bundle.validator_stderr_text.strip() or f"Interactive validator exited with code {bundle.validator_exit_code}"
        return JudgeResult("JUDGE", "BUG", maybe_hide_detail(detail, reveal_detail, "Judge error"))

    prelim = classify_run_failure(bundle.contestant, memory_limit_mb)
    if prelim.verdict != "OK":
        return prelim

    if bundle.validator_exit_code == EXIT_AC:
        return JudgeResult("AC", "AC", maybe_hide_detail(bundle.validator_detail, reveal_detail, ""))

    detail = bundle.validator_detail or bundle.validator_stderr_text.strip() or "Interactive judging did not finish cleanly"
    return JudgeResult("JUDGE", "BUG", maybe_hide_detail(detail, reveal_detail, "Judge error"))


def classify_run_failure(run: RunResult, memory_limit_mb: int) -> JudgeResult:
    if run.timed_out:
        return JudgeResult("TLE", "BUG", f"Time limit exceeded (> {run.time_seconds:.3f}s wall clock)")

    mem_limit_kb = memory_limit_mb * 1024
    stderr_lower = run.stderr_text.lower()
    if run.exit_code == 0 and run.signal_num is None:
        return JudgeResult("OK", "AC", "")

    if run.signal_num in {signal.SIGSEGV, signal.SIGABRT, signal.SIGKILL, signal.SIGXFSZ} or run.exit_code in {134, 137, 139}:
        if run.memory_kb >= int(mem_limit_kb * 0.95) or "bad_alloc" in stderr_lower or "memory" in stderr_lower:
            return JudgeResult("MLE", "BUG", "Memory limit exceeded or allocation failure")

    if run.memory_kb >= int(mem_limit_kb * 0.98) and run.signal_num is not None:
        return JudgeResult("MLE", "BUG", "Likely memory limit exceeded")

    if run.signal_num is not None:
        return JudgeResult("RTE", "BUG", f"Terminated by signal {run.signal_num}")
    return JudgeResult("RTE", "BUG", f"Exited with code {run.exit_code}")


def split_tokens_and_whitespace(data: bytes) -> Tuple[List[bytes], List[bytes]]:
    tokens: List[bytes] = []
    whitespaces: List[bytes] = []
    pos = 0
    first = True
    while True:
        m = ASCII_WHITESPACE_RE.match(data, pos)
        if m:
            ws = data[pos:m.end()]
            pos = m.end()
        else:
            ws = b""
        whitespaces.append(ws)
        if pos >= len(data):
            if first:
                tokens = []
            break
        m = ASCII_WHITESPACE_RE.search(data, pos)
        end = m.start() if m else len(data)
        tokens.append(data[pos:end])
        pos = end
        first = False
        if pos >= len(data) and len(whitespaces) == len(tokens):
            whitespaces.append(b"")
            break
    if len(whitespaces) != len(tokens) + 1:
        whitespaces.append(b"")
    return tokens, whitespaces


def ascii_casefold(token: bytes) -> bytes:
    return token.translate(ASCII_CASEMAP)


def tokens_match(expected: bytes, actual: bytes, cfg: DefaultValidatorConfig) -> bool:
    ans_float = parse_float_from_token(expected) if cfg.has_float_tolerance else None
    if ans_float is not None:
        out_float = parse_float_from_token(actual)
        if out_float is None:
            return False
        diff = abs(out_float - ans_float)
        ok = False
        if cfg.float_absolute_tolerance is not None and diff <= cfg.float_absolute_tolerance:
            ok = True
        if cfg.float_relative_tolerance is not None and diff <= cfg.float_relative_tolerance * abs(ans_float):
            ok = True
        return ok

    if cfg.case_sensitive:
        return actual == expected
    return ascii_casefold(actual) == ascii_casefold(expected)


def format_token_for_message(token: bytes) -> str:
    try:
        return token.decode("utf-8")
    except UnicodeDecodeError:
        return repr(token)


def judge_by_answer_file(
    output_path: Path,
    answer_path: Path,
    cfg: DefaultValidatorConfig,
    *,
    reveal_diff: bool = True,
) -> JudgeResult:
    actual = read_bytes(output_path)
    expected = read_bytes(answer_path)
    a_tokens, a_ws = split_tokens_and_whitespace(actual)
    e_tokens, e_ws = split_tokens_and_whitespace(expected)

    if cfg.space_change_sensitive and a_ws != e_ws:
        if reveal_diff:
            upto = min(len(a_ws), len(e_ws))
            mismatch = next((i for i in range(upto) if a_ws[i] != e_ws[i]), None)
            if mismatch is not None:
                if mismatch == 0:
                    detail = "Leading whitespace differs from answer"
                elif mismatch == len(e_tokens):
                    detail = "Trailing whitespace differs from answer"
                else:
                    detail = f"Whitespace differs before token index {mismatch}"
            elif len(a_ws) != len(e_ws):
                detail = "Whitespace structure differs from answer"
            else:
                detail = "Whitespace differs from answer"
        else:
            detail = "Wrong answer"
        return JudgeResult("WA", "WA", detail)

    if len(a_tokens) != len(e_tokens):
        if reveal_diff:
            if len(a_tokens) < len(e_tokens):
                detail = f"Output ended early: expected {len(e_tokens)} tokens, got {len(a_tokens)}"
            else:
                detail = f"Too many tokens: expected {len(e_tokens)} tokens, got {len(a_tokens)}"
        else:
            detail = "Wrong answer"
        return JudgeResult("WA", "WA", detail)

    for i, (a_tok, e_tok) in enumerate(zip(a_tokens, e_tokens)):
        if tokens_match(e_tok, a_tok, cfg):
            continue
        if reveal_diff:
            if cfg.has_float_tolerance and parse_float_from_token(e_tok) is not None:
                detail = (
                    f"First differing token at index {i}: expected numeric token {format_token_for_message(e_tok)!r}, "
                    f"got {format_token_for_message(a_tok)!r}"
                )
            else:
                detail = (
                    f"First differing token at index {i}: expected {format_token_for_message(e_tok)!r}, "
                    f"got {format_token_for_message(a_tok)!r}"
                )
        else:
            detail = "Wrong answer"
        return JudgeResult("WA", "WA", detail)

    return JudgeResult("AC", "AC", "")


def judge_with_validator(
    validator_bin: Path,
    test: TestCase,
    output_path: Path,
    scratch_dir: Path,
    *,
    reveal_detail: bool = True,
) -> JudgeResult:
    feedback_dir = scratch_dir / "feedback"
    feedback_dir.mkdir(parents=True, exist_ok=True)
    with open(output_path, "rb") as contestant_out:
        proc = subprocess.run(
            [str(validator_bin), str(test.input_path), str(test.answer_path), str(feedback_dir)],
            stdin=contestant_out,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )

    detail = read_feedback_detail(feedback_dir)
    if not detail and proc.stderr.strip():
        detail = proc.stderr.strip()

    if proc.returncode == EXIT_AC:
        return JudgeResult("AC", "AC", maybe_hide_detail(detail, reveal_detail, ""))
    if proc.returncode == EXIT_WA:
        return JudgeResult("WA", "WA", maybe_hide_detail(detail or "Wrong answer", reveal_detail, "Wrong answer"))
    return JudgeResult(
        "JUDGE",
        "BUG",
        maybe_hide_detail(detail or f"Validator exited with code {proc.returncode}", reveal_detail, "Judge error"),
    )


def print_header(problem: ProblemConfig, tests: Sequence[TestCase], use_color: bool) -> None:
    print(colorize(problem.name, Colors.BOLD + Colors.BLUE, use_color))
    print(f"Problem dir : {problem.problem_dir}")
    print(f"Validation  : {problem.validation}")
    if problem.validator_flags_text:
        print(f"Val. flags  : {problem.validator_flags_text}")
    print(f"Time limit  : {problem.time_limit_seconds:.3f}s")
    print(f"Memory limit: {problem.memory_limit_mb} MB")
    print(f"Tests       : {len(tests)}")
    print()


def status_color(verdict: str) -> str:
    if verdict == "AC":
        return Colors.GREEN
    if verdict == "WA":
        return Colors.RED
    return Colors.YELLOW


def format_status(verdict: str, use_color: bool) -> str:
    return colorize(f"[{verdict}]", status_color(verdict), use_color)


def choose_suite(problem_dir: Path, requested_suite: Optional[str]) -> str:
    if requested_suite:
        return requested_suite
    if (problem_dir / "data" / "secret").is_dir():
        return "secret"
    return "sample"


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Compile a C++ solution and run it against an ICPC/Kattis-style local problem package."
    )
    parser.add_argument("solution", help="Path to the candidate C++ source file")
    parser.add_argument("--problem-dir", help="Path to the specific problem package directory")
    parser.add_argument("--root", help="Root directory containing many problem folders")
    parser.add_argument("--problem", help="Problem folder name under --root")
    parser.add_argument("--suite", choices=["sample", "secret", "all"], help="Which tests to run (default: secret if present, otherwise sample)")
    parser.add_argument("--keep-going", action="store_true", help="Do not stop on the first non-AC verdict")
    parser.add_argument("--g++", dest="gxx", default="g++", help="C++ compiler command to use")
    parser.add_argument("--extra-cxxflags", default="", help="Extra flags appended to the C++ compile command")
    parser.add_argument("--no-color", action="store_true", help="Disable ANSI colors")
    parser.add_argument("--show-secret-diff", action="store_true", help="Show mismatch details even on secret tests")

    args = parser.parse_args()
    use_color = want_color(args.no_color)

    solution_path = Path(args.solution).resolve()
    if not solution_path.exists():
        eprint(f"Solution file not found: {solution_path}")
        return 2
    if solution_path.suffix not in {".cpp", ".cc", ".cxx", ".C"}:
        eprint(f"Expected a C++ source file, got: {solution_path.name}")
        return 2

    try:
        problem_dir = discover_problem_dir(solution_path, args)
        problem = parse_problem_config(problem_dir)
        suite = choose_suite(problem_dir, args.suite)
        tests = collect_tests(problem_dir, suite)
    except RunnerError as exc:
        eprint(str(exc))
        return 2

    print_header(problem, tests, use_color)

    with tempfile.TemporaryDirectory(prefix=f"local_judge_{problem.problem_dir.name}_") as tmp:
        tmpdir = Path(tmp)
        exe_path = tmpdir / "submission"
        extra_flags = shlex.split(args.extra_cxxflags)

        try:
            print("Compiling submission...")
            compile_cpp(solution_path, exe_path, args.gxx, extra_flags)
            validator_bin = None
            if problem.has_custom_validator:
                print("Compiling custom validator...")
                validator_bin = compile_custom_validator(problem, tmpdir, args.gxx)
            print()
        except RunnerError as exc:
            eprint(str(exc))
            return 1

        ac_count = 0
        for idx, test in enumerate(tests, start=1):
            reveal_detail = args.show_secret_diff or test.suite != "secret"

            if problem.interactive:
                if validator_bin is None:
                    raise RunnerError("Interactive problem requires a compiled custom validator/interactor")
                bundle = run_interactive_case(
                    contestant_exe=exe_path,
                    validator_bin=validator_bin,
                    test=test,
                    timeout_seconds=problem.time_limit_seconds,
                    memory_limit_mb=problem.memory_limit_mb,
                    scratch_dir=tmpdir / f"judge_{idx:04d}",
                )
                run = bundle.contestant
                judge = judge_interactive_bundle(
                    bundle,
                    problem.memory_limit_mb,
                    reveal_detail=reveal_detail,
                )
                extra_stderr = bundle.validator_stderr_text.strip()
            else:
                out_path = tmpdir / f"out_{idx:04d}.txt"
                run = run_executable(
                    exe_path=exe_path,
                    input_path=test.input_path,
                    output_path=out_path,
                    timeout_seconds=problem.time_limit_seconds,
                    memory_limit_mb=problem.memory_limit_mb,
                )

                prelim = classify_run_failure(run, problem.memory_limit_mb)
                if prelim.verdict == "OK":
                    if validator_bin is not None:
                        judge = judge_with_validator(
                            validator_bin,
                            test,
                            out_path,
                            tmpdir / f"judge_{idx:04d}",
                            reveal_detail=reveal_detail,
                        )
                    else:
                        judge = judge_by_answer_file(
                            out_path,
                            test.answer_path,
                            problem.default_validator,
                            reveal_diff=reveal_detail,
                        )
                else:
                    judge = prelim
                extra_stderr = ""

            line = (
                f"{format_status(judge.verdict, use_color)} "
                f"{test.label:<28} "
                f"time={run.time_seconds:7.3f}s "
                f"mem={run.memory_kb:8d} KiB"
            )
            print(line)
            if judge.detail:
                wrapped = textwrap.fill(judge.detail, width=100, initial_indent="    ", subsequent_indent="    ")
                print(wrapped)
            if run.stderr_text.strip() and judge.verdict != "AC":
                stderr_preview = run.stderr_text.strip().splitlines()
                preview = " | ".join(stderr_preview[:2])
                wrapped = textwrap.fill(f"stderr: {preview}", width=100, initial_indent="    ", subsequent_indent="    ")
                print(wrapped)
            if extra_stderr and judge.verdict != "AC":
                validator_preview = extra_stderr.splitlines()
                preview = " | ".join(validator_preview[:2])
                wrapped = textwrap.fill(
                    f"interactor stderr: {preview}",
                    width=100,
                    initial_indent="    ",
                    subsequent_indent="    ",
                )
                print(wrapped)

            if judge.verdict == "AC":
                ac_count += 1
            elif not args.keep_going:
                print()
                print(f"Stopped after first non-AC verdict on {test.label}.")
                return 1

        print()
        print(colorize(f"Finished: {ac_count}/{len(tests)} tests accepted.", Colors.BOLD + Colors.GREEN, use_color))
        return 0 if ac_count == len(tests) else 1


if __name__ == "__main__":
    raise SystemExit(main())
