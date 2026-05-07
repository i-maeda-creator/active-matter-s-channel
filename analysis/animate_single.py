#!/usr/bin/env python3
"""Create a GIF from one particle trajectory file."""

from __future__ import annotations

import argparse
import math
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


def read_params(path: Path) -> dict[str, float | str]:
    params: dict[str, float | str] = {}
    if not path.exists():
        return params

    for line in path.read_text(encoding="utf-8").splitlines():
        parts = line.split(maxsplit=1)
        if len(parts) != 2:
            continue
        key, value = parts
        try:
            params[key] = float(value)
        except ValueError:
            params[key] = value
    return params


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("trajectory", type=Path)
    parser.add_argument("--output", type=Path, default=Path("analysis/run.gif"))
    parser.add_argument("--params", type=Path, default=None)
    parser.add_argument("--frames", type=int, default=120)
    parser.add_argument("--stride", type=int, default=1)
    parser.add_argument("--fps", type=int, default=12)
    parser.add_argument("--xlim", default=None, help="x axis range as min,max")
    parser.add_argument("--ylim", default=None, help="y axis range as min,max")
    parser.add_argument("--ring", action="store_true", help="draw ring road boundaries from params")
    args = parser.parse_args()

    frames = read_frames(args.trajectory)
    frame_ids = list(range(0, len(frames), args.stride))[: args.frames]
    params = read_params(args.params) if args.params else {}

    xlim = parse_range(args.xlim)
    ylim = parse_range(args.ylim)
    if xlim is None:
        xlim = (0.0, float(params.get("LX", 80.0)))
    if ylim is None:
        ylim = (0.0, float(params.get("LY", 80.0)))

    fig, ax = plt.subplots(figsize=(6.2, 6.2), constrained_layout=True)
    ax.set_aspect("equal", adjustable="box")
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.grid(True, linewidth=0.4, alpha=0.35)

    if args.ring:
        lx = float(params.get("LX", xlim[1]))
        ly = float(params.get("LY", ylim[1]))
        rc = float(params.get("RC", 25.0))
        rw = float(params.get("RW", 8.0))
        cx = 0.5*lx
        cy = 0.5*ly
        for rr in (rc - 0.5*rw, rc + 0.5*rw):
            ax.add_patch(Circle((cx, cy), rr, fill=False, edgecolor="#555555", linewidth=1.2, alpha=0.55))

    patches: list[Circle] = []
    label = ax.text(0.5, 1.02, "", transform=ax.transAxes, ha="center")

    def reset_patches() -> None:
        for patch in patches:
            patch.remove()
        patches.clear()

    def update(frame_index: int):
        reset_patches()
        source_frame = frame_ids[frame_index]
        for x, y, r in frames[source_frame]:
            patch = Circle((x, y), r, facecolor="#27ae60", edgecolor="#145a32", alpha=0.6)
            ax.add_patch(patch)
            patches.append(patch)
        label.set_text(f"frame {source_frame}")
        return patches + [label]

    anim = FuncAnimation(fig, update, frames=len(frame_ids), interval=1000 / args.fps, blit=True)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    anim.save(args.output, writer=PillowWriter(fps=args.fps))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
