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
./active-matter runs/legacy-shape-r04-ly34 1 1000 shape 0.4 10 68 34
```

Next checks:

- Run the current best guess for more output frames and compare trajectories against the old `out.dat`.
- Add an analysis script that compares frame-level center of mass, bounding box, mean velocity, and flow through the S-channel.
- Recover whether old `LX/LY` were exactly `68/34` or whether the output only occupied part of a larger domain.
