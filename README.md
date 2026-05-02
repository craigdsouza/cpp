# C++ Systems Programming - Learning Repo

A structured, daily curriculum building toward production-grade C++ for the autonomous vehicle stack. Started April 2026. 6 hours/day, 5 days/week.

**Goal:** Build the systems programming foundation needed to work on AV mapping and infrastructure - the kind of production grade C++ that runs at 30Hz on a vehicle.

---

## What's in this repo

Daily exercises and quizzes organized by topic. Each day has:
- **Exercises** - easy (Claude assisted) to challenging (implemented from scratch, no scaffolding)
- **Quiz** - reasoning through edge cases to build judgement crucial for system design.

---

## Curriculum progress

| Days | Topics | Key concepts |
|------|--------|--------------|
| 1–4 | Foundations | Value types, references, pointers, stack vs heap, RAII |
| 5–7 | OOP | Classes, constructors, destructors, inheritance, virtual dispatch |
| 8–9 | Modern C++ | Templates, function/class specialization, lambdas, `std::algorithm` |
| 10–11 | File I/O & tooling | `ifstream`, `stringstream`, CSV parsing, `try/catch`, CMake, multi-file projects |
| 12–14 | Graph algorithms | Adjacency list, BFS, Dijkstra's shortest path, GeoJSON road parsing with `nlohmann/json`, Haversine distance |
| 15–16 | Pathfinding + visualization | A\* with Haversine heuristic, early termination, benchmarking with `std::chrono`, Python Folium route visualization |
| 17–18 | Project 1 polish | README, edge case handling, coordinate transform docs, A\* vs Dijkstra benchmarks, clean CMake build |
| 19+ | Concurrency | `std::thread`, `std::mutex`, `std::condition_variable`, producer-consumer |
| 20+ | Networking | gRPC basics, `.proto` authoring, client-server pattern |
| 21+ | Protocol Buffers | Protobuf schema design, serialization, nested message types |

---

## Portfolio projects (separate repo)

The curriculum feeds directly into applied systems projects in [`autonomous-stack-projects`](https://github.com/craigdsouza/autonomous-stack-projects):

| Project | Description | Stack | Status |
|---------|-------------|-------|--------|
| P1: GeoJSON Road Graph Engine | OSM GeoJSON → adjacency list → Dijkstra's + A* shortest path + Python map visualization | C++ + Python | 🔄 In progress |
| P2: Point Cloud Spatial Index | k-d tree for 3D point cloud data - k-NN and radius queries, benchmarked vs brute force on 1M+ points | C++ | 🔄 In progress |
| P3: HD Map Tile Server | Tile-based map server with protobuf schema + gRPC service streaming tiles by bounding box | C++ + gRPC + protobuf | 🔜 Planned |

---

## Background

I have 10+ years of Python and JavaScript experience, including production geospatial software ([Jaltol](https://jaltol.app)) and a co-founded IoT startup ([Connected Farms](https://www.connectedfarms.in/)). I'm adding C++ because the problems I want to work on - AV mapping, spatial computing infrastructure, real-time pipelines - require it.

---

## Contact

[craigdsouza.in](http://craigdsouza.in) · [LinkedIn](https://linkedin.com/in/craig-dsouza) · craigds022@gmail.com
