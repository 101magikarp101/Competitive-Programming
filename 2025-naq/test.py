#!/usr/bin/env python3
import argparse
import difflib
import glob
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

# ---------- ANSI colors ----------
class C:
    R = "\033[31m"
    G = "\033[32m"
    Y = "\033[33m"
    B = "\033[34m"
    M = "\033[35m"
    C = "\033[36m"
    W = "\033[97m"
    DIM = "\033[2m"
    RST = "\033[0m"
    BOLD = "\033[1m"

def colorize(s, c): return f"{c}{s}{C.RST}"

# ---------- Helpers ----------
def detect_compiler(explicit: str | None):
    if explicit:
        return explicit
    candidates = [
        "/opt/homebrew/bin/g++-15",
        shutil.which("g++-15"),
        shutil.which("g++"),
    ]
    for c in candidates:
        if not c:
            continue
        if os.path.isabs(c):
            if os.path.exists(c):
                return c
        else:
            w = shutil.which(c)
            if w:
                return w
    return None

def compile_solution(compiler, cpp_path, out_path, extra_flags):
    cmd = [
        compiler,
        "-O3",
        "-std=c++17",
        "-Wl,-stack_size", "-Wl,0x20000000",
        "-o", str(out_path),
        str(cpp_path),
    ] + list(extra_flags)
    print(colorize("Compiling:", C.DIM), " ".join(cmd))
    try:
        res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    except FileNotFoundError:
        print(colorize("Compiler not found. Set --cxx to your compiler path.", C.R))
        sys.exit(2)
    if res.returncode != 0:
        print(colorize("Compilation failed.\n", C.R) + res.stderr)
        sys.exit(1)
    else:
        if res.stdout.strip():
            print(colorize(res.stdout, C.DIM))
        print(colorize("✅ Build succeeded.", C.G))

def read_file(p: Path) -> str:
    with open(p, "r", encoding="utf-8", errors="replace") as f:
        return f.read()

def normalize(s: str, mode: str) -> str:
    if mode == "strict":
        return s
    # default: trim trailing spaces per line and ignore trailing final newline(s)
    lines = s.splitlines()
    lines = [ln.rstrip() for ln in lines]
    return "\n".join(lines)

def diff_snippet(expected: str, got: str, name: str, max_lines: int):
    exp_lines = expected.splitlines(keepends=False)
    got_lines = got.splitlines(keepends=False)
    diff = list(difflib.unified_diff(exp_lines, got_lines, fromfile=f"{name}.ans", tofile=f"{name}.out", lineterm=""))
    if not diff:
        return ""
    if max_lines > 0 and len(diff) > max_lines:
        diff = diff[:max_lines] + [f"... ({len(diff)-max_lines} more lines)"]
    return "\n".join(diff)

def format_ms(seconds: float) -> str:
    if seconds < 1e-3:
        return f"{seconds*1e6:.0f}µs"
    if seconds < 1.0:
        return f"{seconds*1e3:.1f}ms"
    return f"{seconds:.3f}s"

def run_checker(checker_path: Path, input_path: Path, candidate_output: str, timeout: float):
    """
    Runs the checker. Convention:
      - If checker_path endswith .py, run: python3 checker_path input.in
      - Else assume it's directly executable: checker_path input.in
    The candidate solution's stdout is piped to the checker's stdin.
    Return: (exit_code, stdout, stderr)
    """
    if checker_path.suffix.lower() == ".py":
        cmd = ["python3", str(checker_path), str(input_path)]
    else:
        cmd = [str(checker_path), str(input_path)]
    try:
        proc = subprocess.run(
            cmd,
            input=candidate_output,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=timeout
        )
        return proc.returncode, proc.stdout, proc.stderr
    except subprocess.TimeoutExpired as e:
        # treat checker timeout as WA/TLE on checking stage
        msg = f"Checker timed out after {timeout:.2f}s"
        return 124, msg, ""

# ---------- Main ----------
def main():
    parser = argparse.ArgumentParser(description="Run C++ solution against data/*.in(.ans) with timing, colorful verdicts, and optional custom checker.")
    parser.add_argument("cpp", nargs="?", default="main.cpp", help="Path to C++ solution file (default: main.cpp)")
    parser.add_argument("--data", default="data", help="Folder containing .in/.ans (default: data)")
    parser.add_argument("--cxx", default=None, help="Compiler path (default: /opt/homebrew/bin/g++-15 fallback to g++-15/g++)")
    parser.add_argument("--timeout", type=float, default=2.0, help="Per-test timeout seconds for the solution (default: 2.0)")
    parser.add_argument("--check-timeout", type=float, default=2.0, help="Per-test timeout seconds for the checker (default: 2.0)")
    parser.add_argument("--strict", action="store_true", help="Exact byte-for-byte comparison (no whitespace normalization)")
    parser.add_argument("--show-diff", type=int, default=40, help="Show up to N lines of unified diff on failure (default: 40, 0 to suppress)")
    parser.add_argument("--exe", default=".build/solution.out", help="Output executable path (default: .build/solution.out)")
    parser.add_argument("--extra", nargs="+", default=[], help="Extra compiler flags (after defaults)")
    parser.add_argument("--checker", default=None, help="Path to a custom checker executable/script. If set, .ans is ignored.")
    parser.add_argument("--save-out", action="store_true", help="Save each test's stdout to .out/<name>.out")
    args = parser.parse_args()

    cpp_path = Path(args.cpp)
    if not cpp_path.exists():
        print(colorize(f"Missing C++ file: {cpp_path}", C.R))
        sys.exit(2)

    data_dir = Path(args.data)
    if not data_dir.exists():
        print(colorize(f"Missing data folder: {data_dir}", C.R))
        sys.exit(2)

    compiler = detect_compiler(args.cxx)
    if not compiler:
        print(colorize("Could not find a C++ compiler. Provide --cxx.", C.R))
        sys.exit(2)

    out_path = Path(args.exe)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    compile_solution(compiler, cpp_path, out_path, args.extra)

    in_files = sorted(Path(data_dir).glob("*.in"))
    if not in_files:
        print(colorize("No .in files found in data/. Nothing to run.", C.Y))
        sys.exit(0)

    checker_path = Path(args.checker) if args.checker else None
    if checker_path and not checker_path.exists():
        print(colorize(f"Checker not found: {checker_path}", C.R))
        sys.exit(2)

    # Header
    print()
    title = f" Running {cpp_path.name} on {len(in_files)} test(s) " + (f"[checker: {checker_path.name}] " if checker_path else "")
    bar = "═" * len(title)
    print(colorize("╔" + bar + "╗", C.B))
    print(colorize("║" + title + "║", C.B))
    print(colorize("╚" + bar + "╝", C.B))

    mode = "strict" if args.strict else "loose"
    stats = {"OK":0, "WA":0, "TLE":0, "RE":0, "SKIP":0}
    total_time = 0.0

    if args.save_out:
        Path(".out").mkdir(exist_ok=True)

    for in_path in in_files:
        name = in_path.stem
        ans_path = in_path.with_suffix(".ans")
        label = f"{name}"

        if not checker_path and not ans_path.exists():
            print(colorize(f"[{label}]", C.Y), colorize("⚠️  SKIP", C.Y), colorize("(.ans missing, no checker provided)", C.DIM))
            stats["SKIP"] += 1
            continue

        # Run solution
        start = time.perf_counter()
        try:
            with open(in_path, "rb") as fin:
                run = subprocess.run([str(out_path)],
                                     stdin=fin,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     timeout=args.timeout)
            elapsed = time.perf_counter() - start
        except subprocess.TimeoutExpired:
            elapsed = time.perf_counter() - start
            print(colorize(f"[{label}]", C.Y), colorize("⏰ TLE", C.Y), colorize(f"({format_ms(elapsed)})", C.DIM))
            stats["TLE"] += 1
            total_time += elapsed
            continue
        except Exception as e:
            elapsed = time.perf_counter() - start
            print(colorize(f"[{label}]", C.R), colorize("💥 RE (runner error)", C.R), colorize(f"({format_ms(elapsed)})", C.DIM))
            print(colorize(f"  {e}", C.DIM))
            stats["RE"] += 1
            total_time += elapsed
            continue

        total_time += elapsed
        out_text = run.stdout.decode("utf-8", errors="replace")
        err_text = run.stderr.decode("utf-8", errors="replace")

        if args.save_out:
            with open(Path(".out")/f"{name}.out", "w", encoding="utf-8") as f:
                f.write(out_text)

        if run.returncode != 0:
            print(colorize(f"[{label}]", C.R), colorize("💥 RE", C.R), colorize(f"({format_ms(elapsed)})", C.DIM))
            if err_text.strip():
                print(colorize("  stderr:", C.DIM))
                lines = err_text.strip().splitlines()
                print(colorize("  " + "\n  ".join(lines[:20]), C.DIM))
                if len(lines) > 20:
                    print(colorize("  ...", C.DIM))
            stats["RE"] += 1
            continue

        # --------- Verdict path: checker OR ans-compare ---------
        if checker_path:
            code, chk_out, chk_err = run_checker(checker_path, in_path, out_text, args.check_timeout)
            if code == 0:
                print(colorize(f"[{label}]", C.G), colorize("✅ OK", C.G), colorize(f"({format_ms(elapsed)})", C.DIM))
                if chk_out.strip():
                    print(colorize("  checker:", C.DIM))
                    print(colorize("  " + "\n  ".join(chk_out.strip().splitlines()[:20]), C.DIM))
                stats["OK"] += 1
            else:
                print(colorize(f"[{label}]", C.R), colorize("❌ WA", C.R), colorize(f"({format_ms(elapsed)})", C.DIM))
                if chk_out.strip():
                    print(colorize("  checker says:", C.DIM))
                    print(colorize("  " + "\n  ".join(chk_out.strip().splitlines()[:40]), C.W))
                if chk_err.strip():
                    print(colorize("  checker stderr:", C.DIM))
                    print(colorize("  " + "\n  ".join(chk_err.strip().splitlines()[:20]), C.DIM))
                stats["WA"] += 1
        else:
            exp_text = read_file(ans_path)
            exp_cmp = normalize(exp_text, mode)
            got_cmp = normalize(out_text, mode)

            if exp_cmp == got_cmp:
                print(colorize(f"[{label}]", C.G), colorize("✅ OK", C.G), colorize(f"({format_ms(elapsed)})", C.DIM))
                stats["OK"] += 1
            else:
                print(colorize(f"[{label}]", C.R), colorize("❌ WA", C.R), colorize(f"({format_ms(elapsed)})", C.DIM))
                if args.show_diff != 0:
                    print(colorize("  diff:", C.DIM))
                    snippet = diff_snippet(exp_cmp, got_cmp, name, args.show_diff)
                    if not snippet:
                        print(colorize("  (no unified diff; outputs differ in line endings or whitespace mode)", C.DIM))
                    else:
                        print(colorize("\n".join("  " + ln for ln in snippet.splitlines()), C.W))
                stats["WA"] += 1

    # Footer / summary
    print()
    total = sum(stats.values())
    pieces = [
        colorize(f"{stats['OK']} OK", C.G),
        colorize(f"{stats['WA']} WA", C.R),
        colorize(f"{stats['TLE']} TLE", C.Y),
        colorize(f"{stats['RE']} RE", C.M),
        colorize(f"{stats['SKIP']} SKIP", C.C),
    ]
    summary = f"Summary: {', '.join(pieces)}  |  total time {colorize(format_ms(total_time), C.B)}"
    print(colorize("─" * (len(summary) + 4), C.DIM))
    print("  " + summary)
    print(colorize("─" * (len(summary) + 4), C.DIM))

if __name__ == "__main__":
    main()
