# Active-Matter City Map

## Thesis

この研究では、自動運転を「個々の車が賢くなる技術」だけとして扱わない。

都市全体を、自己駆動する多数のエージェントが局所相互作用によって流れ、詰まり、同期し、秩序を作るアクティブマターとして扱う。

## Core Correspondence

| Active matter model | Traffic / city interpretation |
| --- | --- |
| Particle | Vehicle, pedestrian, robot, drone, or transit unit |
| Position `(x, y)` | Location in road, lane, crossing, station, or city layer |
| Radius `R` | Safety buffer, personal space, vehicle footprint |
| Active force `FA` | Desired speed or propulsion intent |
| Direction `th` | Heading, route preference, local navigation intent |
| WCA repulsion | Collision avoidance and minimum distance keeping |
| Wall force `WE` | Road boundary, lane edge, curb, building wall, restricted area |
| S-channel | Curve, bottleneck, merge, detour, narrow street, intersection fragment |
| Noise `DD`, `DR` | Driver variation, sensing error, uncertainty, human randomness |
| Kuramoto coupling `KR` | Platooning, cooperative driving, alignment, social following |
| Boundary condition | Road network topology, loop roads, ramps, lane connections |

## What The Current 2D Model Can Mean

The current S-channel simulation can be reinterpreted as a minimal traffic experiment.

- The channel is a constrained road geometry.
- Particles are simple autonomous agents.
- Repulsion is collision avoidance.
- Direction coupling is cooperative alignment.
- Noise is uncertainty or individuality.
- Flow through the channel is transport efficiency.

This makes the current code useful even before full 3D city modeling.

## First City Primitive: Circular Road

The first urban primitive should be a closed circular road.

Why this comes first:

- It removes entrances and exits.
- It lets us measure congestion without boundary injection artifacts.
- Density is controlled by particle count and road length.
- Platooning and stop-and-go waves can circulate repeatedly.
- It is the simplest bridge between active matter and traffic flow.

In the code this is the `ring` mode:

```sh
./active-matter runs/ring-road-smoke 120 100 ring 0.4 80 80 80 25 8
python analysis/animate_single.py runs/ring-road-smoke/out.dat --params runs/ring-road-smoke/params.txt --output analysis/ring_road_smoke.gif --frames 120 --stride 1 --fps 12 --xlim 0,80 --ylim 0,80 --ring
```

Interpretation:

- `RC`: road centerline radius.
- `RW`: lane width.
- `NP`: number of vehicles or agents on the loop.
- `FA`: desired driving speed.
- `KR`: cooperative alignment or platooning strength.
- `DR`: heading uncertainty.

## Research Questions

### 1. Activity vs Congestion

Does increasing `FA` always improve throughput, or does high activity create more collisions and jams?

Traffic interpretation:

- More aggressive desired speed may increase flow in free space.
- In bottlenecks, it may amplify congestion.

### 2. Synchronization vs Flexibility

Does increasing `KR` help agents move as a coherent stream, or does it create rigid platoons that jam at curves?

Traffic interpretation:

- Cooperative driving can stabilize flow.
- Too much alignment may reduce local adaptability.

### 3. Noise as a Jam Breaker

Does increasing `DR` destroy order, or can it help agents escape local jams?

Traffic interpretation:

- Small randomness may break deadlocks.
- Large randomness may reduce safety and coordination.

### 4. Boundary Design

How does road geometry shape flow?

Traffic interpretation:

- Some bottlenecks are not only capacity problems.
- Boundary curvature, merge angle, and lane width can determine whether active agents self-organize or jam.

## Minimum Observable Set

To make this a research program, each simulation should output:

- Throughput: number of agents crossing a gate per unit time.
- Mean speed: average displacement per unit time.
- Density: local crowding or vehicle concentration.
- Polar order: alignment of agent headings.
- Jam time: time spent below a motion threshold.
- Wall pressure: how strongly agents push against boundaries.

## Path To 3D

3D is not the immediate first step. The useful sequence is:

1. Make 2D traffic observables robust.
2. Add lanes and gates to the 2D geometry.
3. Add multiple connected 2D layers.
4. Treat layer transitions as ramps, stations, elevators, or drone altitude changes.
5. Generalize positions and directions to full 3D.

This avoids jumping to a complex 3D simulation before the science question is measurable.

## Working Research Statement

Self-driving urban mobility can be modeled as an active-matter system in which local collision avoidance, cooperative alignment, boundary geometry, and controlled randomness jointly determine macroscopic traffic flow.

The near-term target is to identify when local autonomous rules produce efficient flow, and when the same rules produce congestion.
