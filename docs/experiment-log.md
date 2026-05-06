# Experiment Log

## 2026-05-06: Legacy 2D Condition Recovery

Goal: 古い `out.dat` を壊さず、当時の実行条件に近い設定を復元する。

Old root-level output:

- `out.dat`: 5000 nonblank particle rows.
- First frame: 10 particles.
- `ene.dat`: 500 lines, time from 1 to 500.
- Coordinate range across `out.dat`: approximately `x=0..68`, `y=0..34`.
- Particle radius in `out.dat`: `0.4`.

Findings:

- Current default `NP=250`, `R=0.5`, `LX=200`, `LY=80` makes `shape` initialization extremely slow because many particles are packed into the narrow S-channel entrance.
- `NP=10`, `R=0.4`, `LX=200`, `LY=80` runs, but its first-frame y positions remain near `78`, unlike the old output near `31`.
- `NP=10`, `R=0.4`, `LX=68`, `LY=35`, `inner_steps=1000` gives first-frame y positions near `32..34`.
- `NP=10`, `R=0.4`, `LX=68`, `LY=34`, `inner_steps=1000` gives first-frame y positions near `31..33`, closest so far to the old output.

Reproduction candidates:

```sh
./active-matter runs/legacy-shape-r04 1 1000 shape 0.4 10 68 35
./active-matter runs/legacy-shape-r04-ly34 1 1000 shape 0.4 10 68 34
```

Current best guess:

```sh
./active-matter runs/legacy-shape-r04-ly33-step7000 1 7000 shape 0.4 10 68 33
```

First-frame metric comparison for the current best guess:

```text
old center: (17.572446, 31.598673)
new center: (17.429625, 31.778469)
delta:      (-0.142821, 0.179796)
old span:   width=24.536137, height=3.099160
new span:   width=22.408645, height=2.093027
ordered RMSE:   11.940984
sorted xy RMSE: 4.740988
```

The center of mass is now close. Ordered particle RMSE is still large, so the particle identity/order or random initialization history is likely not identical.

Next checks:

- Add order-independent comparisons such as nearest-neighbor distance or sorted-by-x/y matching.
- Run the current best guess for more output frames and compare trajectories against the old `out.dat`.
- Recover whether old `LX/LY` were exactly `68/33` or whether the output only occupied part of a larger domain.

First-frame comparison command:

```sh
python analysis/compare_runs.py out.dat runs/legacy-shape-r04-ly34/out.dat --frames 1
python analysis/plot_frames.py out.dat runs/legacy-shape-r04-ly33-step7000/out.dat --frame 0 --output analysis/legacy_frame0_compare.png --xlim 0,34 --ylim 29,35
python analysis/animate_runs.py out.dat runs/legacy-best-500/out.dat --output analysis/legacy_compare.gif --frames 120 --stride 4 --fps 12 --xlim 0,68 --ylim 0,35
```
