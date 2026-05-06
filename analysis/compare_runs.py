#!/usr/bin/env python3
"""Compare two gnuplot-indexed particle trajectory files.

Each frame is separated by one or more blank lines. Particle rows are:

    x y r
"""

from __future__ import annotations

import argparse
import math
from pathlib import Path


def read_frames(path: Path) -> list[list[tuple[float, float, float]]]:
    frames: list[list[tuple[float, float, float]]] = []
    frame: list[tuple[float, float, float]] = []

    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line:
            if frame:
                frames.append(frame)
                frame = []
            continue

        parts = line.split()
        if len(parts) < 3:
            raise ValueError(f"Bad row in {path}: {line!r}")
        frame.append((float(parts[0]), float(parts[1]), float(parts[2])))

    if frame:
        frames.append(frame)

    return frames


def metrics(frame: list[tuple[float, float, float]]) -> dict[str, float]:
    n = len(frame)
    xs = [p[0] for p in frame]
    ys = [p[1] for p in frame]
    rs = [p[2] for p in frame]

    return {
        "n": float(n),
        "cx": sum(xs) / n,
        "cy": sum(ys) / n,
        "min_x": min(xs),
        "max_x": max(xs),
        "min_y": min(ys),
        "max_y": max(ys),
        "mean_r": sum(rs) / n,
    }


def compare_frame(
    old_frame: list[tuple[float, float, float]],
    new_frame: list[tuple[float, float, float]],
) -> dict[str, float]:
    old = metrics(old_frame)
    new = metrics(new_frame)

    out = {
        "old_n": old["n"],
        "new_n": new["n"],
        "old_cx": old["cx"],
        "new_cx": new["cx"],
        "old_cy": old["cy"],
        "new_cy": new["cy"],
        "delta_cx": new["cx"] - old["cx"],
        "delta_cy": new["cy"] - old["cy"],
        "old_width": old["max_x"] - old["min_x"],
        "new_width": new["max_x"] - new["min_x"],
        "old_height": old["max_y"] - old["min_y"],
        "new_height": new["max_y"] - new["min_y"],
        "old_mean_r": old["mean_r"],
        "new_mean_r": new["mean_r"],
    }

    if len(old_frame) == len(new_frame):
        sq = 0.0
        for old_p, new_p in zip(old_frame, new_frame):
            sq += (new_p[0] - old_p[0]) ** 2 + (new_p[1] - old_p[1]) ** 2
        out["ordered_rmse"] = math.sqrt(sq / len(old_frame))

        sorted_sq = 0.0
        old_sorted = sorted(old_frame, key=lambda p: (p[0], p[1]))
        new_sorted = sorted(new_frame, key=lambda p: (p[0], p[1]))
        for old_p, new_p in zip(old_sorted, new_sorted):
            sorted_sq += (new_p[0] - old_p[0]) ** 2 + (new_p[1] - old_p[1]) ** 2
        out["sorted_xy_rmse"] = math.sqrt(sorted_sq / len(old_frame))
    else:
        out["ordered_rmse"] = float("nan")
        out["sorted_xy_rmse"] = float("nan")

    return out


def print_table(rows: list[dict[str, float]]) -> None:
    headers = [
        "frame",
        "old_n",
        "new_n",
        "old_cx",
        "new_cx",
        "old_cy",
        "new_cy",
        "delta_cx",
        "delta_cy",
        "old_width",
        "new_width",
        "old_height",
        "new_height",
        "ordered_rmse",
        "sorted_xy_rmse",
    ]
    print(",".join(headers))
    for row in rows:
        values = []
        for h in headers:
            value = row[h]
            if h in ("frame", "old_n", "new_n"):
                values.append(str(int(value)))
            else:
                values.append(f"{value:.6f}")
        print(",".join(values))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("old", type=Path)
    parser.add_argument("new", type=Path)
    parser.add_argument("--frames", type=int, default=1)
    args = parser.parse_args()

    old_frames = read_frames(args.old)
    new_frames = read_frames(args.new)
    nframes = min(args.frames, len(old_frames), len(new_frames))

    rows = []
    for i in range(nframes):
        row = compare_frame(old_frames[i], new_frames[i])
        row["frame"] = float(i)
        rows.append(row)

    print_table(rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
