#!/usr/bin/env python3
"""
Solve a black-wall, white-corridor maze using a tile grid (default 12×12),
then print the optimal route as a string of U/D/L/R moves.

Usage:
  python3 run.py path/to/maze.png [--tile 12] [--threshold 0.5]
                                  [--overlay out.png] [--verbose]

Assumptions:
  • Walls are pure black (#000000).
  • Start is green (#008000) and exit is red (#FF0000).
  • The path is found over tile centers (4-neighborhood BFS).
"""
import argparse
import sys
from collections import deque
from pathlib import Path
from typing import List, Tuple

import numpy as np
from PIL import Image, ImageDraw


# ----------------------------- helpers -----------------------------
def load_image(path: str):
    """Open image as RGB and return (PIL.Image, np.ndarray)."""
    try:
        img = Image.open(path).convert("RGB")
        return img, np.array(img)
    except Exception as e:
        raise SystemExit(f"Failed to open image: {e}")


def build_grid(arr: np.ndarray, tile: int, wall_frac: float):
    """
    Return: (cropped_arr, wall_grid, start_tile, end_tile, H, W)
      - cropped_arr: array cropped to a multiple of `tile`
      - wall_grid: boolean [rows, cols] where True = wall tile
      - start_tile, end_tile: (row, col) indices of start/end
      - H, W: cropped image height/width in pixels
    """
    h, w = arr.shape[:2]
    H = (h // tile) * tile
    W = (w // tile) * tile
    if H == 0 or W == 0:
        raise SystemExit("Image is smaller than one tile.")

    arr_c = arr[:H, :W]

    wall = (arr_c[:, :, 0] == 0) & (arr_c[:, :, 1] == 0) & (arr_c[:, :, 2] == 0)
    start_mask = (arr_c[:, :, 0] == 0) & (arr_c[:, :, 1] == 128) & (arr_c[:, :, 2] == 0)
    end_mask   = (arr_c[:, :, 0] == 255) & (arr_c[:, :, 1] == 0) & (arr_c[:, :, 2] == 0)

    rows, cols = H // tile, W // tile
    grid_wall = np.zeros((rows, cols), dtype=bool)
    for r in range(rows):
        y0 = r * tile
        for c in range(cols):
            x0 = c * tile
            block = wall[y0:y0 + tile, x0:x0 + tile]
            grid_wall[r, c] = (block.mean() >= wall_frac)

    if not start_mask.any() or not end_mask.any():
        raise SystemExit("Could not find green start and/or red end pixels.")

    sy, sx = np.argwhere(start_mask).mean(axis=0).astype(int)
    ty, tx = np.argwhere(end_mask).mean(axis=0).astype(int)

    start = (sy // tile, sx // tile)
    end   = (ty // tile, tx // tile)
    return arr_c, grid_wall, start, end, H, W


def bfs(grid_wall: np.ndarray, start: Tuple[int, int], end: Tuple[int, int]) -> List[Tuple[int, int]]:
    """Shortest path over tile centers using 4-neighborhood BFS."""
    rows, cols = grid_wall.shape
    prev = -np.ones((rows, cols, 2), dtype=int)
    q = deque([start])
    prev[start[0], start[1]] = (start[0], start[1])

    while q:
        r, c = q.popleft()
        if (r, c) == end:
            break
        for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nr, nc = r + dr, c + dc
            if 0 <= nr < rows and 0 <= nc < cols and not grid_wall[nr, nc] and prev[nr, nc, 0] == -1:
                prev[nr, nc] = (r, c)
                q.append((nr, nc))
    else:
        raise SystemExit("No path found on the tile grid.")

    path = []
    r, c = end
    while (r, c) != start:
        path.append((r, c))
        r, c = prev[r, c]
    path.append(start)
    path.reverse()
    return path


def path_to_moves(path: List[Tuple[int, int]]) -> str:
    """Convert tile path to U/D/L/R moves."""
    moves = []
    for (r1, c1), (r2, c2) in zip(path, path[1:]):
        dr, dc = r2 - r1, c2 - c1
        if dr == 1 and dc == 0:
            moves.append('D')
        elif dr == -1 and dc == 0:
            moves.append('U')
        elif dr == 0 and dc == 1:
            moves.append('R')
        elif dr == 0 and dc == -1:
            moves.append('L')
        else:
            raise RuntimeError(f"Non-orthogonal step: {(r1, c1)} -> {(r2, c2)}")
    return ''.join(moves)


def save_overlay(arr: np.ndarray, path: List[Tuple[int, int]], tile: int,
                 out_path: Path, line_width: int = 1):
    """Draw a red polyline through tile centers and save."""
    img = Image.fromarray(arr).copy()
    draw = ImageDraw.Draw(img)
    pts = [(c * tile + tile // 2, r * tile + tile // 2) for r, c in path]
    for i in range(1, len(pts)):
        draw.line([pts[i - 1], pts[i]], fill=(255, 0, 0), width=line_width)
    draw.ellipse([pts[0][0] - 2, pts[0][1] - 2, pts[0][0] + 2, pts[0][1] + 2], fill=(255, 0, 0))
    draw.ellipse([pts[-1][0] - 2, pts[-1][1] - 2, pts[-1][0] + 2, pts[-1][1] + 2], fill=(255, 0, 0))
    img.save(out_path)


# ----------------------------- main -----------------------------
def main():
    ap = argparse.ArgumentParser(description="Solve a maze image and print U/D/L/R moves.")
    ap.add_argument("image", help="Path to maze image")
    ap.add_argument("--tile", type=int, default=12, help="Tile size in pixels (default 12)")
    ap.add_argument("--threshold", type=float, default=0.5,
                    help="Fraction of black pixels in a tile to call it a wall (default 0.5)")
    ap.add_argument("--overlay", type=str, default=None, help="Optional path to save red-path overlay")
    ap.add_argument("--verbose", action="store_true", help="Print diagnostics to stderr")
    args = ap.parse_args()

    _, arr = load_image(args.image)
    arr_c, wall_grid, start, end, H, W = build_grid(arr, args.tile, args.threshold)

    if args.verbose:
        sys.stderr.write(f"[grid] {H // args.tile}x{W // args.tile} tiles | start={start} end={end}\n")

    path = bfs(wall_grid, start, end)
    moves = path_to_moves(path)

    # Print only the moves string
    print(moves)

    if args.overlay:
        save_overlay(arr_c, path, args.tile, Path(args.overlay))
        if args.verbose:
            sys.stderr.write(f"[overlay] saved to {args.overlay}\n")


if __name__ == "__main__":
    main()
