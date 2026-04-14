#!/usr/bin/env python3

"""
Local testing tool for Treedir

Disclaimer: This is NOT the same code used to test your solution when it is submitted.
This tool is provided as-is. Its purpose is to help with debugging the interactive
problem and it has no ambitions to extensively test all possibilities that
are allowed by the problem statement. While the tool tries to yield the same
results as the real judging system, this is not guaranteed and the result
may differ if the tested program does not use the correct formatting or
exhibits other incorrect behavior. The tool also *does not* enforce time and
memory limits that are applied to submitted solutions.

Feel free to make whatever alterations or augmentations you like.

The behavior is controlled by an input data file.
The first line of the file contains two integers: T (number of testcases) and seed.
For each testcase:
-   The first line contains two integers n and s, the size of the tree and the integer to pass.
-   For the next n-1 lines, each line contains two integers u and v, representing an edge between nodes u and v in the tree.
 
Assigning seed to 0 will disable shuffling of testcases and edges in the trees.

Here is an example file:
2 20260412
7 5
1 6
3 5
2 4
3 4
1 2
6 7
9 9
1 2
1 4
3 4
3 5
3 8
5 6
6 7
8 9

The testing tool is run like this on Windows:
    python treedir_testing_tool.py <data_file> <program> <arguments>
or on Linux:
    python3 treedir_testing_tool.py <data_file> <program> <arguments>

where `arguments` are optional arguments to the program to run. The following
show examples for different languages:

    python3 treedir_testing_tool.py 1.in ./myprogram
    python3 treedir_testing_tool.py 1.in java -cp . MyProgram
    python3 treedir_testing_tool.py 1.in python3 myprogram.py

The tool logs the complete interaction.
If you do not want that, pass `--quiet` (before the data file name).

Templated from the local testing tool of ICPC EC-final 2025. 
"""

from __future__ import annotations

import argparse
import random
import subprocess
import sys
from typing import List, Tuple


verbose = True
process = None
run_id = 0


class WrongAnswer(RuntimeError):
    pass


class EndOfFile(RuntimeError):
    pass


def vprint(*args, **kwargs) -> None:
    if verbose:
        print("< ", end="")
        print(*args, **kwargs)
        sys.stdout.flush()
    print(*args, file=process.stdin, flush=True, **kwargs)


def vreadline(optional: bool = False) -> str:
    line = process.stdout.readline()
    if verbose and line:
        print(">", line.rstrip("\n"))
    if not line and not optional:
        raise EndOfFile()
    return line

def read_case(path: str) -> Tuple[int, List[Tuple[int, int, List[Tuple[int, int]]]], int]:
    with open(path, "r", encoding="utf-8") as f:
        tokens = f.read().strip().split()
    if not tokens:
        raise ValueError("Input file is empty.")
    T = int(tokens[0])
    seed = int(tokens[1])
    sum_n = 0
    tests = []
    for i in range(T):
        if len(tokens) < 4 + 2 * sum_n:
            raise ValueError("Input file is too short.")
        n = int(tokens[2 + 2*sum_n])
        s = int(tokens[3 + 2*sum_n])
        
        edges = []
        if len(tokens) < 2 + 2 * (sum_n + n):
            raise ValueError("Input file is too short.")
        
        for j in range(n - 1):
            u = int(tokens[4 + 2*sum_n + 2*j])
            v = int(tokens[5 + 2*sum_n + 2*j])
            edges.append((u, v))
            
        sum_n += n
        tests.append((n, s, edges))
    
    return T, tests, seed


def start_process(program: List[str], phase: str, bufsize: int | None = None) -> subprocess.Popen:
    global run_id, process
    run_id += 1
    if verbose:
        print(f"[run {run_id}] {phase}: {' '.join(program)}")
    kwargs = {
        "shell": True,
        "stdin": subprocess.PIPE,
        "stdout": subprocess.PIPE,
        "text": True,
    }
    if bufsize is not None:
        kwargs["bufsize"] = bufsize
    process = subprocess.Popen(" ".join(program), **kwargs)
    if process.stdin is None or process.stdout is None:
        raise RuntimeError("Failed to open pipes.")
    return process

def run_encode(program: List[str], T: int, tests: List[int, int, List[Tuple[int, int]]]) -> List[Tuple[int, List[Tuple[int, int]]]]:
    raw_input_header = str(T) + " 1"
    

    start_process(program, "Orienting", bufsize=1)
    vprint(raw_input_header)
    pass_data = []
    counter = 0
    for n, s, edges in tests:
        counter += 1
        if verbose:
            print(f"Case {counter}: n = {n}, s = {s}")
        vprint(str(n) + " " + str(s))
        oriented_edges = []
        for u, v in edges:
            vprint(f"{u} {v}")
            while True:
                read_str = vreadline().strip()
                if read_str != "": 
                    ori_u, ori_v = map(int, read_str.split())
                    if (ori_u, ori_v) == (u, v) or (ori_v, ori_u) == (u, v):
                        oriented_edges.append((ori_u, ori_v))
                        break
                    else:
                        raise WrongAnswer(f"{read_str} is not a valid orientation for edge {u} {v}.")
        pass_data.append((int(s), oriented_edges))
    if process.stdin:
        process.stdin.close()
    process.wait()
    if process.returncode != 0:
        raise WrongAnswer(f"Orienting phase exit code {process.returncode}.")
    if len(pass_data) != T:
        raise WrongAnswer(f"Orienting output length {len(pass_data)} != {T}.")
    return pass_data


def run_decode(
    program: List[str],
    T: int,
    passed_data: List[Tuple[int, List[Tuple[int, int]]]],
    seed: int,
):
    start_process(program, "Seeking", bufsize=1)

    vprint(str(T) + " 2")
    
    rnd = random.Random(seed)
    tests = passed_data[:]
    if seed:
        rnd.shuffle(tests)
    counter = 0
    for s, edge_list in tests:
        counter += 1
        n = len(edge_list) + 1
        if verbose:
            print(f"Case {counter}: n = {n}, s = {s}")
        vprint(f"{n}")
        if seed:
            rnd.shuffle(edge_list)
        for u, v in edge_list:
            vprint(f"{u} {v}")
        ans = int(vreadline().strip())
        if ans != s:
            raise WrongAnswer(f"Incorrect pass: {s} -> {ans}")
    try:
        process.stdin.close()
    except Exception:
        pass
    process.wait(timeout=5)
    if process.returncode != 0:
        raise WrongAnswer(f"Seeking phase exit code {process.returncode}.")



def main() -> int:
    parser = argparse.ArgumentParser(
        usage="%(prog)s [--quiet] data.in program [args...]"
    )
    parser.add_argument(
        "--quiet", "-q", action="store_true", help="Do not show interactions"
    )
    parser.add_argument("data", help="Input file that controls the behavior of the tool")
    parser.add_argument("program", nargs=argparse.REMAINDER, help="Program to run")
    args = parser.parse_args()

    global verbose
    verbose = not args.quiet

    if not args.program:
        parser.error("Must specify program to run")

    T, tests, seed = read_case(args.data)

    passed_data = run_encode(args.program, T, tests)
    run_decode(args.program, T, passed_data, seed)

    print(f"Program finished correctly.", file=sys.stderr)

    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except WrongAnswer as exc:
        print(f"Error: {exc}", file=sys.stderr)
        sys.exit(1)
