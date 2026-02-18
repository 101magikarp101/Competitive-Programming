#!/usr/bin/env python3
"""
3D word-search solver (straight-line words in a 3D letter cube), ranked by wordfreq,
and FILTERED to exclude the 36 grid words (and optionally other excludes).

Requires:
  pip install wordfreq
"""

from __future__ import annotations

import argparse
import csv
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Set, Tuple, Optional

from wordfreq import zipf_frequency

# ---------------------------------------------------------------------
# 1) Put your 6x6 grid of 6-letter words here (row-major).
# ---------------------------------------------------------------------
GRID_WORDS: List[List[str]] = [
    ["SPONGE", "CANCER", "KIMONO", "MIRROR", "POLAND", "GARLIC"],
    ["DESERT", "GRAPES", "CHURCH", "BIKINI", "SHRIMP", "ORANGE"],
    ["BELIZE", "BEAVER", "WINDOW", "DRAGON", "TOILET", "COFFIN"],
    ["WAFFLE", "GOBLIN", "COOKIE", "BANANA", "SWEDEN", "SCHOOL"],
    ["VIOLIN", "TSHIRT", "RUSSIA", "ABACUS", "CACTUS", "SCROLL"],
    ["BEETLE", "ROCKET", "MONKEY", "GUITAR", "CARROT", "LAPTOP"],
]

# 26 directions in 3D (dx, dy, dz) excluding (0,0,0)
DIRS: List[Tuple[int, int, int]] = [
    (dx, dy, dz)
    for dx in (-1, 0, 1)
    for dy in (-1, 0, 1)
    for dz in (-1, 0, 1)
    if not (dx == 0 and dy == 0 and dz == 0)
]


@dataclass(frozen=True)
class Occurrence:
    word: str
    zipf: float
    start: Tuple[int, int, int]          # (x, y, z), 0-indexed
    direction: Tuple[int, int, int]
    coords: Tuple[Tuple[int, int, int], ...]  # full path coords


def _clean_word(s: str) -> str:
    # Keep only letters; uppercase for consistent cube matching.
    return re.sub(r"[^A-Za-z]", "", s).upper()


def load_wordlist(path: Path, min_len: int) -> Set[str]:
    words: Set[str] = set()
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        w = _clean_word(line.strip())
        if len(w) >= min_len:
            words.add(w)
    return words


def load_exclude(path: Path) -> Set[str]:
    excludes: Set[str] = set()
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        w = _clean_word(line.strip())
        if w:
            excludes.add(w)
    return excludes


def build_cube(grid_words: List[List[str]]) -> List[List[List[str]]]:
    """
    Returns cube[z][y][x] as single-character strings.
    Depth is word length; assumes all words have the same length.
    """
    h = len(grid_words)
    w = len(grid_words[0]) if h else 0
    if any(len(row) != w for row in grid_words):
        raise ValueError("Grid must be rectangular (all rows same length).")

    depth = len(grid_words[0][0])
    for r in grid_words:
        for s in r:
            if len(s) != depth:
                raise ValueError("All grid words must have the same length (cube depth).")

    cube = [[[""] * w for _ in range(h)] for _ in range(depth)]
    for y in range(h):
        for x in range(w):
            s = _clean_word(grid_words[y][x])
            if len(s) != depth:
                raise ValueError(f"Word at row {y}, col {x} is not length {depth}: {grid_words[y][x]!r}")
            for z in range(depth):
                cube[z][y][x] = s[z]
    return cube


def in_bounds(x: int, y: int, z: int, w: int, h: int, d: int) -> bool:
    return 0 <= x < w and 0 <= y < h and 0 <= z < d


def dir_label(dx: int, dy: int, dz: int) -> str:
    parts: List[str] = []
    if dy == -1: parts.append("N")
    if dy == 1:  parts.append("S")
    if dx == 1:  parts.append("E")
    if dx == -1: parts.append("W")
    if dz == 1:  parts.append("IN")
    if dz == -1: parts.append("OUT")
    return "+".join(parts) if parts else "—"


def coord_label(x: int, y: int, z: int) -> str:
    # Column A.., Row 1.., Layer 1..
    return f"{chr(ord('A') + x)}{y + 1}-L{z + 1}"


def score_zipf(word_upper: str, lang: str) -> float:
    # wordfreq expects lowercase; returns ~0.0 for unknown, higher is more common.
    return float(zipf_frequency(word_upper.lower(), lang))


def grid_word_excludes(grid_words: List[List[str]]) -> Set[str]:
    """All words used to make the grid (uppercased/cleaned)."""
    return {_clean_word(w) for row in grid_words for w in row if _clean_word(w)}


def find_words(
    cube: List[List[List[str]]],
    min_len: int,
    max_len: Optional[int],
    exclude: Set[str],
    # validation mode:
    wordlist: Optional[Set[str]],
    lang: str,
    min_zipf: float,
) -> List[Occurrence]:
    d = len(cube)
    h = len(cube[0])
    w = len(cube[0][0])

    results: List[Occurrence] = []

    for z0 in range(d):
        for y0 in range(h):
            for x0 in range(w):
                for dx, dy, dz in DIRS:
                    x, y, z = x0, y0, z0
                    chars: List[str] = []
                    coords: List[Tuple[int, int, int]] = []

                    steps = 0
                    while in_bounds(x, y, z, w, h, d):
                        steps += 1
                        if max_len is not None and steps > max_len:
                            break

                        chars.append(cube[z][y][x])
                        coords.append((x, y, z))

                        if len(chars) >= min_len:
                            s = "".join(chars)  # uppercase
                            if s in exclude:
                                pass
                            else:
                                # Validate
                                if wordlist is not None:
                                    ok = (s in wordlist)
                                    zf = score_zipf(s, lang) if ok else 0.0
                                else:
                                    zf = score_zipf(s, lang)
                                    ok = (zf >= min_zipf)

                                if ok:
                                    results.append(
                                        Occurrence(
                                            word=s,
                                            zipf=zf,
                                            start=(x0, y0, z0),
                                            direction=(dx, dy, dz),
                                            coords=tuple(coords),
                                        )
                                    )

                        x += dx
                        y += dy
                        z += dz

    return results


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--min-len", type=int, default=4, help="Minimum word length to report (default: 4)")
    ap.add_argument("--max-len", type=int, default=None, help="Maximum word length (default: unlimited)")
    ap.add_argument("--exclude", type=Path, default=None, help="Optional extra exclude list (one word per line)")
    ap.add_argument(
        "--exclude-grid-words",
        action="store_true",
        default=True,
        help="Exclude the 36 words used to construct the grid (default: on).",
    )
    ap.add_argument(
        "--include-grid-words",
        action="store_true",
        help="Override and allow grid-construction words to appear in results.",
    )

    # Validation options
    ap.add_argument(
        "--wordlist",
        type=Path,
        default=None,
        help="Optional wordlist for validation (one word per line). "
             "If omitted, validation uses wordfreq >= --min-zipf."
    )
    ap.add_argument("--lang", type=str, default="en", help="wordfreq language code (default: en)")
    ap.add_argument(
        "--min-zipf",
        type=float,
        default=0.0,
        help="When --wordlist is omitted: accept words with zipf >= this threshold (default: 0.0). "
             "Practical values: 2.0–3.5 to filter obscure words."
    )

    # Output options
    ap.add_argument("--csv", type=Path, default=None, help="Optional CSV output path")
    ap.add_argument("--unique", action="store_true", help="Print each word once (still counts occurrences)")
    args = ap.parse_args()

    # Build excludes
    exclude: Set[str] = set()
    if args.exclude is not None:
        exclude |= load_exclude(args.exclude)

    # Exclude the words used to create the grid (unless overridden)
    if args.exclude_grid_words and not args.include_grid_words:
        exclude |= grid_word_excludes(GRID_WORDS)

    # Load optional validator wordlist
    wordlist: Optional[Set[str]] = None
    if args.wordlist is not None:
        wordlist = load_wordlist(args.wordlist, args.min_len)
        # Also remove excluded words from the validator set (optional but useful)
        wordlist -= exclude

    cube = build_cube(GRID_WORDS)
    hits = find_words(
        cube=cube,
        min_len=args.min_len,
        max_len=args.max_len,
        exclude=exclude,
        wordlist=wordlist,
        lang=args.lang,
        min_zipf=args.min_zipf,
    )

    # Aggregate occurrences by word
    by_word: Dict[str, List[Occurrence]] = {}
    for occ in hits:
        by_word.setdefault(occ.word, []).append(occ)

    # Determine each word’s score for sorting (use max zipf among occurrences)
    word_score: Dict[str, float] = {w: max(o.zipf for o in occs) for w, occs in by_word.items()}

    # Sort by zipf desc, then length desc, then alphabetically
    words_sorted = sorted(
        by_word.keys(),
        key=lambda w: (-word_score[w], -len(w), w),
    )

    print(f"Found {len(hits)} occurrences across {len(words_sorted)} unique words (min_len={args.min_len}).")
    if args.wordlist is None:
        print(f"Validation: wordfreq zipf >= {args.min_zipf} (lang={args.lang})")
    else:
        print(f"Validation: wordlist={args.wordlist} (ranking via wordfreq; lang={args.lang})")
    if args.exclude_grid_words and not args.include_grid_words:
        print("Excluded: grid-construction words")
    if args.exclude is not None:
        print(f"Excluded: {args.exclude}")
    print()

    if args.unique:
        for w in words_sorted:
            occ0 = by_word[w][0]
            dx, dy, dz = occ0.direction
            path = " -> ".join(coord_label(x, y, z) for (x, y, z) in occ0.coords)
            print(
                f"{w:>10s}  zipf={word_score[w]:.2f}  (len {len(w)})  occ={len(by_word[w])}  "
                f"example: {coord_label(*occ0.start)}  {dir_label(dx,dy,dz)}  {path}"
            )
    else:
        for w in words_sorted:
            for occ in by_word[w]:
                dx, dy, dz = occ.direction
                path = " -> ".join(coord_label(x, y, z) for (x, y, z) in occ.coords)
                print(
                    f"{w:>10s}  zipf={occ.zipf:.2f}  (len {len(w)})  "
                    f"start={coord_label(*occ.start)}  dir={dir_label(dx,dy,dz)}  {path}"
                )

    # Optional CSV output (one row per occurrence)
    if args.csv is not None:
        with args.csv.open("w", newline="", encoding="utf-8") as f:
            wr = csv.writer(f)
            wr.writerow(["word", "zipf", "length", "start", "direction", "path"])
            for w in words_sorted:
                for occ in by_word[w]:
                    dx, dy, dz = occ.direction
                    wr.writerow([
                        w,
                        f"{occ.zipf:.2f}",
                        len(w),
                        coord_label(*occ.start),
                        dir_label(dx, dy, dz),
                        " -> ".join(coord_label(x, y, z) for (x, y, z) in occ.coords),
                    ])
        print()
        print(f"Wrote CSV: {args.csv}")


if __name__ == "__main__":
    main()
