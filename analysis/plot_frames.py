#!/usr/bin/env python3
"""Plot particle frames from one or two gnuplot-indexed trajectory files."""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.patches import Circle

from compare_runs import read_frames


def draw_frame(ax, frame, title: str, xlim=None, ylim=None) -> None:
    ax.set_aspect("equal", adjustable="box")
    ax.set_title(title)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.grid(True, linewidth=0.4, alpha=0.35)

    xs = [p[0] for p in frame]
    ys = [p[1] for p in frame]
    rs = [p[2] for p in frame]

    for x, y, r in frame:
        ax.add_patch(Circle((x, y), r, facecolor="#2f80ed", edgecolor="#12355b", alpha=0.55))

    ax.scatter(xs, ys, s=8, color="#12355b", zorder=3)

    if xlim:
        ax.set_xlim(xlim)
    else:
        pad = max(rs) * 4.0
        ax.set_xlim(min(xs) - pad, max(xs) + pad)

    if ylim:
        ax.set_ylim(ylim)
    else:
        pad = max(rs) * 4.0
        ax.set_ylim(min(ys) - pad, max(ys) + pad)


def parse_range(value: str | None):
    if value is None:
        return None
    lo, hi = value.split(",", 1)
    return (float(lo), float(hi))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("old", type=Path)
    parser.add_argument("new", type=Path)
    parser.add_argument("--frame", type=int, default=0)
    parser.add_argument("--output", type=Path, default=Path("analysis/frame_compare.png"))
    parser.add_argument("--xlim", default=None, help="x axis range as min,max")
    parser.add_argument("--ylim", default=None, help="y axis range as min,max")
    args = parser.parse_args()

    old_frames = read_frames(args.old)
    new_frames = read_frames(args.new)
    old_frame = old_frames[args.frame]
    new_frame = new_frames[args.frame]

    xlim = parse_range(args.xlim)
    ylim = parse_range(args.ylim)

    fig, axes = plt.subplots(1, 2, figsize=(12, 5), constrained_layout=True)
    draw_frame(axes[0], old_frame, f"old out.dat frame {args.frame}", xlim=xlim, ylim=ylim)
    draw_frame(axes[1], new_frame, f"recovered frame {args.frame}", xlim=xlim, ylim=ylim)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(args.output, dpi=180)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
