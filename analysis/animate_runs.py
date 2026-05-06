#!/usr/bin/env python3
"""Create a side-by-side GIF from two particle trajectory files."""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from matplotlib.patches import Circle

from compare_runs import read_frames


def parse_range(value: str | None):
    if value is None:
        return None
    lo, hi = value.split(",", 1)
    return (float(lo), float(hi))


def setup_axis(ax, title: str, xlim, ylim) -> None:
    ax.set_aspect("equal", adjustable="box")
    ax.set_title(title)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.grid(True, linewidth=0.4, alpha=0.35)
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("old", type=Path)
    parser.add_argument("new", type=Path)
    parser.add_argument("--output", type=Path, default=Path("analysis/legacy_compare.gif"))
    parser.add_argument("--frames", type=int, default=120)
    parser.add_argument("--stride", type=int, default=1)
    parser.add_argument("--fps", type=int, default=12)
    parser.add_argument("--xlim", default="0,68", help="x axis range as min,max")
    parser.add_argument("--ylim", default="0,35", help="y axis range as min,max")
    args = parser.parse_args()

    old_frames = read_frames(args.old)
    new_frames = read_frames(args.new)
    frame_ids = list(range(0, min(len(old_frames), len(new_frames)), args.stride))
    frame_ids = frame_ids[: args.frames]

    xlim = parse_range(args.xlim)
    ylim = parse_range(args.ylim)

    fig, axes = plt.subplots(1, 2, figsize=(11, 4.8), constrained_layout=True)
    setup_axis(axes[0], "old out.dat", xlim, ylim)
    setup_axis(axes[1], "recovered", xlim, ylim)

    old_patches: list[Circle] = []
    new_patches: list[Circle] = []

    def reset_patches() -> None:
        for patch in old_patches + new_patches:
            patch.remove()
        old_patches.clear()
        new_patches.clear()

    def add_particles(ax, frame, patches, color: str) -> None:
        for x, y, r in frame:
            patch = Circle((x, y), r, facecolor=color, edgecolor="#12355b", alpha=0.55)
            ax.add_patch(patch)
            patches.append(patch)

    frame_label = fig.text(0.5, 0.02, "", ha="center", va="bottom")

    def update(frame_index: int):
        reset_patches()
        source_frame = frame_ids[frame_index]
        add_particles(axes[0], old_frames[source_frame], old_patches, "#2f80ed")
        add_particles(axes[1], new_frames[source_frame], new_patches, "#27ae60")
        frame_label.set_text(f"frame {source_frame}")
        return old_patches + new_patches + [frame_label]

    anim = FuncAnimation(fig, update, frames=len(frame_ids), interval=1000 / args.fps, blit=True)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    anim.save(args.output, writer=PillowWriter(fps=args.fps))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
