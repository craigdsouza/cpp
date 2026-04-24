# NVIDIA DRIVE AV — Mapping Team: Career Readiness Path

**Target Role:** Software Engineer, DRIVE Mapping Team (JR2014851)
**Your Current Level:** 4.0/10
**Date Created:** March 25, 2026
**Last Updated:** 2026-04-23

---

## Your Starting Profile

**Strengths (leverage these):**

- 10 years of programming experience across Python and JavaScript
- Geospatial development with raster and vector data (directly maps to the role)
- Built a geospatial SaaS product — you understand spatial data pipelines end-to-end
- Product management experience — you think in systems, not just code
- Moderate linear algebra / coordinate systems knowledge from GIS work
- Regular AI coding tool user (Claude, Cursor)

**Critical Gaps (close these):**

- No C++ experience (the role's primary language)
- No embedded or real-time systems background
- No distributed systems, gRPC, or protocol buffer experience
- AI/ML at hobbyist level only, need professional-grade transformer/LLM skills
- No hands-on autonomous vehicle or navigation map (NDS.Live, OSM routing) experience
- No CUDA / GPU programming

---

## Current Sprint

> This section stays current. Last 7 curriculum days = what was actually covered. Next 7 = what's planned. Beyond that, see the Level progression and Project Track below.

### Last 7 Curriculum Days


| Day | Date       | Topic                                                                 | Exercises    | Quiz      |
| --- | ---------- | --------------------------------------------------------------------- | ------------ | --------- |
| 5   | 2026-04-03 | RAII & Destructors — destructor timing, stack unwinding, TileBuffer   | 6/6 pass     | 4.0/6.0   |
| 6   | 2026-04-15 | Smart Pointers — unique_ptr, shared_ptr, move, factory functions      | 5/5 pass     | 6.75/7.0  |
| 7   | 2026-04-17 | Move Semantics — rvalue refs, move constructor, move assignment, NRVO | 5/5 pass     | 4.0/5.0   |
| 8   | 2026-04-20 | Templates — function/class templates, specialization, non-type params | 5/5 pass     | 3.75/6.0  |
| 9   | 2026-04-21 | Lambdas + std::algorithm — captures, sort, find_if, transform         | 4/4 pass     | 5.5/7.0   |
| 10  | 2026-04-22 | File Parsing + Real Data — ifstream, stringstream, CSV, try/catch     | 4/4 pass     | 4.25/6.0  |
| 11  | 2026-04-23 | CMake + Multi-File Projects — header/source split, linking, CMake     | 4/4 pass     | 5.5/7.0   |


**Trajectory:** 100% exercise pass rate across Days 6–11 (24/24 exercises). Quiz scores stabilizing in the 4–6.75 range. Mechanical fluency is strong and consistent — the student writes multi-file C++ with CMake without scaffolding. One persistent conceptual gap: type safety per instantiation (carried forward from Days 8, 9, and 11). "Why" reasoning has improved significantly since Day 7.

---

### Next 7 Curriculum Days


| Day | Target Date | Topic                                   | Key Concepts                                                                                  | Real Data?                    |
| --- | ----------- | --------------------------------------- | --------------------------------------------------------------------------------------------- | ----------------------------- |
| 12  | Fri Apr 24  | Road Graph Fundamentals                 | adjacency list, BFS, CSV edge loading, unordered_set visited tracking                         | Yes — road edge CSV           |
| 13  | Mon Apr 27  | Graph Algorithms                        | Dijkstra's shortest path, priority_queue, path reconstruction                                 | Yes                           |
| 14  | Tue Apr 28  | P1 Setup + GeoJSON Parsing              | create P1 repo, integrate nlohmann/json, parse real OSM GeoJSON into RoadGraph adjacency list | **Yes — real OSM data**       |
| 15  | Wed Apr 29  | P1 Algorithms                           | port Dijkstra's + implement A* in P1 codebase, test on real SF road data                      | Yes                           |
| 16  | Thu Apr 30   | P1 Python Visualization                 | Folium/Leaflet route visualization, WGS84 coordinate handling, export from C++ graph          | Yes                           |
| 17  | Fri May 1   | P1 Polish                               | README, edge case handling, coordinate transform docs, query benchmarks                       | Yes                           |
| 18  | Mon May 2   | P1 Complete — GitHub Push               | final review, clean CMake build, portfolio write-up, push to `autonomous-stack-projects/`     | **Yes — P1 done**             |


**Note on Project 1:** Days 12–13 build the C++ core in the learning repo (exercises). Days 14–17 apply that code to real OSM GeoJSON in the actual P1 repo. Day 18 is the finish line: clean build, README, GitHub push. Inheritance, concurrency, and networking (originally planned for Days 14–17) are postponed to after P1 is shipped — they feed P2 and P3, not P1.

---

## Portfolio Project Track

> **Strategy:** 9 projects across the full AV stack. Each one is a GitHub-ready artifact. The split across projects reflects how the modern AV stack actually works: Python trains models, C++ runs them safely at 30Hz on hardware. Projects 1–3 target the NVIDIA DRIVE Mapping role (June 1 application). Projects 4–9 broaden the portfolio to cover perception, localization, planning, and prediction — opening doors to other AV roles beyond mapping.
>
> **Repo:** `autonomous-stack-projects/` (separate from the C++ learning repo)

---

### Layer 1: Mapping + Infrastructure

#### Project 1: GeoJSON → Road Graph Engine

**Stack:** C++ | **AV Layer:** Mapping

**What:** Ingest OpenStreetMap GeoJSON exports, build a road network graph, run shortest-path queries (Dijkstra's, A*).

**Why it matters:** The DRIVE Mapping team builds "scalable map-building workflows." This is the exact pipeline shape — raw spatial data → structured graph. Your GIS background gives you a head start on coordinate handling that most C++ engineers won't have.

**Deliverables:**

- C++ library that parses GeoJSON road segments into an adjacency list
- Dijkstra's and A* on the road graph
- Python script that visualizes routes on a Folium/Leaflet map
- README documenting coordinate system handling

**C++ skills:** STL containers, file I/O, graph algorithms, smart pointers, CMake

---

#### Project 2: Point Cloud Spatial Index

**Stack:** C++ | **AV Layer:** Mapping / Localization primitive

**What:** k-d tree in C++ that indexes 3D point cloud data, supports nearest-neighbor and radius queries.

**Why it matters:** Map-based localization matches live lidar scans against stored map features. The k-d tree is the core data structure that makes this fast. Building it from scratch proves you understand spatial indexing at the low level.

**Deliverables:**

- C++ k-d tree for 3D points (insert, k-NN, radius search)
- Benchmark: k-d tree vs brute force on 1M+ points
- Python script generating synthetic point clouds and visualizing query results
- Optional: load a real lidar clip from KITTI

**C++ skills:** Pointers, recursion, memory layout, cache-friendly data structures, benchmarking

---

#### Project 3: HD Map Tile Server

**Stack:** C++ + protobuf + gRPC | **AV Layer:** Infrastructure

**What:** Tile-based map server — stores road geometry in a spatial grid, serves tiles on demand, handles UTM ↔ WGS84 coordinate transforms.

**Why it matters:** "Deliver SD maps to all in-car consumers" is literally the job description. A tile server is the exact architectural pattern. You've built tiled spatial data systems before from your SaaS work — this is that, in C++.

**Deliverables:**

- C++ tile index with spatial hashing (tile key = zoom/x/y)
- Protocol Buffers schema for map tiles
- gRPC service that accepts a bounding box query and streams back tiles
- Python client that requests tiles along a route and renders them
- Health monitoring endpoint

**C++ skills:** gRPC, protobuf, multithreading, serialization, network programming

---

### Layer 2: Perception

#### Project 4: Perception Pipeline

**Stack:** Python (training) + C++ (runtime) | **AV Layer:** Perception

**What:** Train an object detection model on a real driving dataset. Write the C++ inference runtime that runs it on the vehicle side.

**Python side:** Fine-tune YOLOv8 (or similar) on KITTI or nuScenes in PyTorch. Detect cars, pedestrians, cyclists from camera frames. Export to ONNX.

**C++ side:** Inference runtime wrapper using ONNX Runtime or TensorRT. Frame preprocessing pipeline, post-processing detections, safety validation layer that sanity-checks outputs before they leave the module.

**Why it matters:** This is the "Python trains it, C++runs it safely at 30Hz" pattern that shows up across the entire modern AV stack. Most candidates can train a model. Fewer can write the C++ wrapper that makes it production-reliable.

**Deliverables:**

- PyTorch training pipeline + ONNX export
- C++ inference runtime with preprocessing + postprocessing
- Safety validation layer (bounds checking, confidence thresholds, output schema enforcement)
- Benchmark: latency on a recorded driving clip

**Skills:** PyTorch, ONNX Runtime / TensorRT, C++ systems programming

---

### Layer 3: Localization + Sensor Fusion

#### Project 5: State Estimator

**Stack:** C++ only | **AV Layer:** Localization / Sensor Fusion

**What:** Extended Kalman Filter that fuses noisy GPS + IMU readings into a clean vehicle pose estimate (position, orientation, velocity).

**Why C++ only:** EKF is classical estimation — fixed-size matrix operations, deterministic execution, no DL needed. Strong systems C++ work. Every downstream module depends on knowing where the car is.

**Deliverables:**

- C++ EKF implementation fusing GPS + IMU
- Runs on a recorded sensor log (synthetic or KITTI IMU data)
- Python visualization of raw GPS vs filtered pose track
- Unit tests for filter correctness

**C++ skills:** Linear algebra in C++ (Eigen library), numerical stability, fixed-rate execution

---

#### Project 6: Map-Based Localization

**Stack:** C++ | **AV Layer:** Localization

**What:** Use the road graph (Project 1) + vehicle pose (Project 5) + detections (Project 4) to localize the vehicle on the map. Particle filter or scan-matching approach.

**Why it matters:** This is the first project that wires multiple prior projects together. It demonstrates you understand how the layers talk to each other — which is what a senior AV engineer actually evaluates in an interview.

**Deliverables:**

- C++ particle filter or scan matcher
- Runs on a recorded driving clip using outputs from Projects 1, 4, 5
- Python visualization of localization track vs ground truth

**C++ skills:** Probabilistic algorithms, inter-module interfaces, real-time data flow

---

### Layer 4: Planning

#### Project 7: Motion Planner

**Stack:** C++ | **AV Layer:** Planning

**What:** Real-time trajectory planner. A* or RRT for route selection + polynomial profiling for local trajectory. Takes map (Project 1), current pose (Project 5), detected obstacles (Project 4). Must run at 10Hz.

**Why it matters:** Planning is the highest-value module to demonstrate competence in for non-perception AV roles. Real-time constraint (10Hz, deterministic) is what separates a systems engineer from an algorithm prototyper.

**Deliverables:**

- C++ trajectory planner meeting 10Hz budget
- Obstacle avoidance using Project 4 detections
- Python visualization of planned trajectory overlaid on map
- Latency profiling: where does the compute go?

**C++ skills:** Real-time constraints, profiling, optimization algorithms, inter-module data contracts

---

### Layer 5: Prediction

#### Project 8: Trajectory Prediction

**Stack:** Python (training) + C++ (runtime) | **AV Layer:** Prediction

**What:** Train a transformer-based model to predict where surrounding agents will be in the next 3–5 seconds. Feed predictions into the motion planner's risk assessment.

**Python side:** Train on Argoverse or nuScenes using PyTorch. Transformer architecture operating on agent history + map context. Export to ONNX.

**C++ side:** Inference runtime feeding real-time predictions into Project 7's planner. Prediction uncertainty output shapes the planner's risk tolerance.

**Why it matters:** Prediction is one of the hardest unsolved problems in AV. Transformers operating on graph-structured map data is exactly what NVIDIA's job description referenced. This project sits at the frontier of what the role actually works on.

**Deliverables:**

- PyTorch transformer training pipeline on Argoverse/nuScenes
- ONNX export + C++ inference runtime
- Integration with Project 7: planner reacts to predicted agent trajectories
- Evaluation: ADE/FDE metrics on held-out scenarios

**Skills:** Transformer architecture, graph-structured inputs, PyTorch, ONNX Runtime in C++

---

### Capstone

#### Project 9: Mini AV Pipeline

**Stack:** C++ runtime + Python training pipelines | **AV Layer:** Full stack integration

**What:** Wire Projects 1, 4, 5, 7, 8 into a pipeline that runs on a recorded sensor clip (KITTI or nuScenes). Sensor data in → map-aware, prediction-informed trajectory out. Python script renders the result.

**Why it matters:** This is the project you demo in interviews. Most candidates have components. Almost nobody has a working pipeline. It proves you understand how the modules depend on each other, what the data contracts between them look like, and what breaks when one module is wrong.

**Deliverables:**

- C++ pipeline runner: sensor data → perception → state estimation → localization → prediction → planning
- Python evaluation script: trajectory quality, latency per module, failure mode analysis
- 2-minute demo video running on a real driving clip
- Architecture diagram + README explaining every design decision

---

### How the Projects Stack

> **Pace assumption:** 1 curriculum day per weekday (Mon–Fri). Project work runs concurrently in the same daily block. DL study on weekends. All dates calculated from Day 6 done on Wed Apr 15, 2026.


| Dates           | C++ Curriculum + DL                                                          | Project Milestone | Target Date | Status             |
| --------------- | ---------------------------------------------------------------------------- | ----------------- | ----------- | ------------------ |
| Mar 25 – Apr 15 | Days 1–6: types → smart pointers                                             | P1 start (Apr 15) | Apr 15      | **Done / Started** |
| Apr 17–25       | Days 7–13: move semantics, templates, lambdas, real data, CMake, graph algos | P1 active         | —           | **In Progress**    |
| Apr 28 – May 5  | Days 14–18: P1 GeoJSON parsing, Dijkstra's/A*, Python viz, polish, push      | **P1 complete**   | **May 5**   | Upcoming           |
| May 5–16        | Days 19–28: concurrency, networking, gRPC, protobuf                          | **P2 complete**   | **May 26**  | Upcoming           |
| May 19 – Jun 6  | Days 29–38: serialization, inference runtimes + DL: PyTorch basics, CNNs     | **P3 complete**   | **Jun 16**  | Upcoming           |
| Jun 9–27        | C++: ONNX/TensorRT + DL: object detection fine-tuning (KITTI/nuScenes)       | **P4 complete**   | **Jul 7**   | Upcoming           |
| Jun 30 – Jul 11 | C++: Eigen, EKF, linear algebra + DL: continued                              | **P5 complete**   | **Jul 21**  | Upcoming           |
| Jul 14–25       | C++: particle filters, inter-module interfaces                               | **P6 complete**   | **Aug 4**   | Upcoming           |
| Jul 28 – Aug 8  | C++: real-time optimization, profiling, lock-free structures                 | **P7 complete**   | **Aug 18**  | Upcoming           |
| Aug 11 – Sep 5  | DL: transformers, trajectory prediction + C++: inference integration         | **P8 complete**   | **Sep 15**  | Upcoming           |
| Sep 8 – Oct 3   | Integration, profiling, documentation, demo video                            | **P9 complete**   | **Oct 6**   | Upcoming           |


**Milestone summary:**


| Milestone   | Target Date | Significance                                                    |
| ----------- | ----------- | --------------------------------------------------------------- |
| P1 complete | **May 12**  | First GitHub artifact — apply to mapping roles                  |
| P2 complete | **May 26**  | Spatial indexing signal — apply to localization roles           |
| P3 complete | **Jun 16**  | Full mapping portfolio — strong signal for NVIDIA DRIVE Mapping |
| P4 complete | **Jul 7**   | Perception Python+C++ pipeline — broadens to perception roles   |
| P5 complete | **Jul 21**  | Sensor fusion — localization engineering roles                  |
| P6 complete | **Aug 4**   | End-to-end localization — connected multi-project portfolio     |
| P7 complete | **Aug 18**  | Motion planning — AV generalist signal                          |
| P8 complete | **Sep 15**  | Transformer-based prediction — frontier AV signal               |
| P9 complete | **Oct 6**   | Full pipeline demo — interview-ready capstone                   |


---

---

> **Note on Level sections vs Project Track:** The Level sections below describe conceptual depth targets and study resources. The Portfolio Project Track above is where the hands-on work happens. Each level checkpoint maps to specific projects: L3→4 = Projects 1–2, L4→5 = Project 3, L5→6 = Projects 4 + 8, L6→7 = Projects 5–6, L7→8 = Projects 7–9.

---

## Level 3 → 4: Foundations Reset (Weeks 1–6)

**Goal:** Establish the CS fundamentals this role assumes you already have as a "4+ years with BS CS" candidate.

**Status:** Largely complete — Days 1–6 cover C++ kickstart through smart pointers. Remaining: graph algorithms, CMake, GDB.

### C++ Kickstart

- Work through the first 15 chapters of *"A Tour of C++"* by Bjarne Stroustrup (the creator). This is a compact book that assumes you already program — it won't waste your time on "what is a variable."
- Key topics to nail: RAII, references vs pointers, const correctness, the STL containers (vector, map, unordered_map), iterators.
- **Hands-on:** Project 1 (Road Graph Engine) — parses real GeoJSON, builds a graph, runs shortest paths in C++. This replaces standalone exercises.

### Data Structures & Algorithms in C++

- Focus on graph algorithms — this role is literally about "transformer based models tailored for graphs." You need BFS, DFS, Dijkstra's, A* deeply internalized.
- Practice on LeetCode (medium difficulty) in C++ specifically. Aim for 3 problems per week. Tag: graphs, trees, priority queues.

### Linux & Build Systems

- Get comfortable with CMake, Make, GDB, and Valgrind. The NVIDIA stack is Linux-native.
- Set up a C++ development environment with VSCode + CMake + Clang. Build and run projects from the command line.

### Checkpoint

You're at Level 4 when you can: write a 500-line C++program from scratch that compiles without warnings under `-Wall -Wextra`, uses modern C++ idioms, and solves a graph traversal problem.

---

## Level 4 → 5: Modern C++ & Systems Thinking (Weeks 7–12)

**Goal:** Write C++ the way NVIDIA engineers write it — modern, efficient, safe.

### Modern C++14/17

- Study move semantics, smart pointers (unique_ptr, shared_ptr), lambdas, structured bindings, std::optional, std::variant.
- Read *"Effective Modern C++"* by Scott Meyers — this is the Bible for writing C++ that senior engineers respect.
- Understand template metaprogramming basics — you'll encounter it in NVIDIA's codebase.

### Operating Systems & Real-Time Concepts

- Read the first 30 chapters of *"Operating Systems: Three Easy Pieces"* (free online at ostep.org). Focus on: processes, threads, locks, condition variables, scheduling.
- Understand real-time scheduling: what "deterministic latency" means, why autonomous vehicles can't tolerate garbage collection pauses, priority inversion.
- **Project:** Write a multithreaded C++ producer-consumer pipeline. One thread reads sensor data from a file, another processes it, a third writes results. Use std::thread, std::mutex, std::condition_variable.

### Networking & Serialization Basics

- Learn what Protocol Buffers and FlatBuffers are and *why* they exist (hint: they're orders of magnitude faster than JSON for structured data, which matters when your car is parsing map tiles at 70 mph).
- Write a simple client-server pair using gRPC in C++. Define a .proto file, generate code, send and receive messages.
- Understand the difference between gRPC (request/response + streaming) and REST.

### Checkpoint

You're at Level 5 when you can: write multithreaded C++ with proper synchronization, explain why FlatBuffers are used instead of JSON in real-time systems, and build a working gRPC service.

---

## Level 5 → 6: Machine Learning Engineering (Running in parallel from Week 1)

**Goal:** Go from ML hobbyist to someone who can train, evaluate, and deploy models professionally.

> **Important:** This track runs in parallel with C++ study — weekends. Don't wait until Week 13 to start. The DL skills needed for Projects 4 and 8 will be ready when those projects are scheduled (Weeks 10–20) if you start now.

### Deep Learning Foundations

- Complete fast.ai's "Practical Deep Learning for Coders" — it's free and project-driven.
- Then go deeper with Stanford CS231n (computer vision) lectures on YouTube. Focus on: CNNs, object detection, semantic segmentation.
- Understand the training loop *mechanically*: forward pass, loss computation, backward pass, optimizer step. Be able to write one from scratch in PyTorch.

### Transformers (Critical for This Role)

- The job explicitly mentions "transformer based models tailored for graphs." This is cutting-edge.
- Study the original "Attention Is All You Need" paper. Then read about Graph Attention Networks (GAT) and Graph Transformers.
- Understand how transformers can operate on non-sequential data (graphs, point clouds, map topologies).
- **Project:** Fine-tune a pretrained model (e.g., a ViT or a GNN) on a small custom dataset. Document your prompt-engineering workflow using Claude to help debug training issues.

### LLM Evaluation Frameworks

- The role mentions "implement evaluation frameworks to measure performance of large scale LLMs." Study: perplexity, BLEU/ROUGE (for generation), task-specific benchmarks, human evaluation protocols.
- Look into frameworks like lm-evaluation-harness, HELM, and how companies build internal eval suites.
- **Project:** Build a simple evaluation pipeline in Python that scores a model on a custom benchmark. Measure accuracy, latency, and failure modes.

### Checkpoint

You're at Level 6 when you can: train a transformer model from scratch in PyTorch, explain self-attention mathematically, fine-tune a pretrained model on a domain-specific dataset, and build an evaluation framework.

---

## Level 6 → 7: Geospatial + AV Domain Expertise (Weeks 21–28)

**Goal:** Bridge your existing geospatial skills into the autonomous vehicle mapping domain.

### Navigation Maps Deep Dive

- Study the NDS.Live specification — this is NVIDIA's preferred map standard. Understand: map tiles, layers (road geometry, topology, attributes, lane models), update mechanisms.
- Contribute to OpenStreetMap. Seriously. Edit roads, add turn restrictions, understand the OSM data model (nodes, ways, relations). This gives you practical map-editing intuition.
- Understand HD Maps vs SD Maps vs navigation maps. The job says "deliver SD maps to all in-car consumers" — know what that means architecturally.

### Localization & Sensor Fusion

- Study how maps are used for localization: the car matches what it sees (lidar/camera) against what the map says should be there.
- Learn about: coordinate frames (vehicle, world, map), UTM projections (you likely know this from GIS), pose estimation, and how map-based localization provides redundancy for perception.
- Read papers from NVIDIA DRIVE Labs (available on their blog) — they publish accessible explanations of their tech.

### Map Data Pipelines

- Understand how maps are built: fleet vehicles collect data → cloud pipelines process it → map tiles are generated → updates are pushed to vehicles.
- Study how distributed map-building workflows operate (the job mentions "scalable and distributed map-building workflows"). Think: Spark/Dask for geospatial batch processing, message queues for real-time updates.
- **Hands-on:** Projects 1 and 3 cover this — OSM ingestion → road graph (P1) → tile server with gRPC (P3). No separate project needed here.

### 3D Geometry for AV

- Deepen your linear algebra: homogeneous coordinates, rotation matrices, quaternions, rigid body transformations.
- Study camera models: pintrhole model, intrinsic/extrinsic parameters, projection and unprojection.
- Understand point cloud processing basics — how lidar data becomes map features.

### Checkpoint

You're at Level 7 when you can: explain NDS.Live's tile structure, build a road-network graph from OSM data, perform coordinate transformations between vehicle and map frames, and describe how HD maps aid localization.

---

## Level 7 → 8: Embedded & Automotive Software (Weeks 29–36)

**Goal:** Understand the constraints of writing software that runs inside a car.

### Embedded Systems Fundamentals

- Study memory-constrained programming: no dynamic allocation in hot paths, fixed-size buffers, memory pools, cache-friendly data layouts.
- Understand AUTOSAR (the automotive software architecture standard) at a conceptual level — where the map provider sits in the software stack.
- Learn about functional safety (ISO 26262) — what "ASIL" ratings mean and why safety-critical software has strict coding standards.

### NVIDIA DRIVE Platform

- Study the NVIDIA DRIVE AGX Orin architecture: what compute is available (ARM CPUs, GPU, DLA), how software is partitioned across these.
- Understand NVIDIA's DriveWorks SDK — the middleware that sits between the OS and autonomous driving applications.
- Read NVIDIA's technical blog posts about their AV stack. Pay attention to how map services integrate with perception and planning.

### Real-Time C++ Patterns

- Study lock-free data structures (lock-free queues, atomic operations). Map data must be readable by multiple consumers without blocking.
- Understand zero-copy techniques — how FlatBuffers enable reading serialized data without deserialization (this is why the job lists FlatBuffers specifically).
- **Project:** Implement a lock-free ring buffer in C++ for passing map updates between threads. Benchmark it against a mutex-based queue.

### CUDA Fundamentals

- The "ways to stand out" section mentions CUDA. Start with NVIDIA's official CUDA programming guide.
- Understand: kernels, threads, blocks, grids, shared memory, memory coalescing.
- **Project:** Write a CUDA kernel that performs a parallel nearest-neighbor search on a set of 3D points (relevant to map matching).

### Checkpoint

You're at Level 8 when you can: write C++ that respects real-time constraints (no unbounded allocations, deterministic latency), explain the DRIVE AGX architecture, implement a lock-free data structure, and write a basic CUDA kernel.

---

## Level 8 → 9: Integration & System Design (Weeks 37–44)

**Goal:** Think like an architect — design systems, not just components.

### Map Provider Architecture

- Design a map provider service from scratch: how does it receive route information, select relevant map tiles, decode them, serve them to localization/planning/HMI consumers, and handle updates?
- Consider: health monitoring (the job mentions "health monitors"), graceful degradation when map data is stale or missing, data fusion with live perception.
- Draw architecture diagrams. Practice explaining your design decisions out loud — this is interview material.

### End-to-End AV System Understanding

- Study the full AV software stack: perception → prediction → planning → control. Understand where maps touch each module.
- Read Waymo's, Cruise's, and NVIDIA's published technical reports on their architectures.
- Understand "data-driven" vs "rule-based" approaches to driving, and where the industry is heading (hint: the role's focus on transformers and LLMs tells you NVIDIA's direction).

### Graph Neural Networks for Maps

- This is the cutting edge the role targets. Study: how road networks are naturally graphs, how GNNs can learn traffic patterns / routing / map quality signals.
- Read recent papers on map learning: "MapTR," "VectorMapNet," and related work on neural map representations.
- **Project:** Train a GNN that predicts road connectivity or traffic patterns from a real road network graph. Use PyTorch Geometric.

### Large-Scale Systems

- Study how to build scalable map-building pipelines: distributed processing (Apache Beam, Spark), data versioning, continuous integration for map data.
- Understand cloud-to-car deployment: how trained models and updated maps are pushed to vehicle fleets.

### Checkpoint

You're at Level 9 when you can: whiteboard a complete map provider architecture, explain how graph transformers apply to road networks, design a distributed map-building pipeline, and articulate where maps fit in the full AV stack.

---

## Level 9 → 10: Interview-Ready & Beyond (Weeks 45–52)

**Goal:** Be the candidate they can't say no to.

### Portfolio & Proof of Work

- Build a capstone project that combines everything: a C++ map provider that ingests NDS-format tiles, serves them via gRPC to simulated consumers, includes a Python-based map-building pipeline, and uses a trained GNN for map quality scoring. Open-source it on GitHub.
- Write technical blog posts about your journey. NVIDIA's culture values people who share knowledge.

### Interview Preparation

- Practice system design interviews focused on AV: "Design a map update system for a fleet of 100K vehicles." "Design a real-time map provider that handles tile loading, caching, and invalidation."
- Practice C++ coding interviews: focus on data structures, concurrency, and performance optimization.
- Prepare to discuss your geospatial background — frame your GIS experience as a *head start*, not a detour. You understand spatial indexing, projections, and tiled data formats at a level most candidates don't.

### AI-Assisted Development Fluency

- This role explicitly values "heavy AI user for day-to-day development." Build a portfolio of examples where you used Claude/Cursor to: architect a system, debug a tricky concurrency bug, generate test cases, review code for safety issues.
- Practice "prompt-crafting" — the job literally lists it as a requirement. Document your best prompts and workflows.

### Domain Credibility

- Follow NVIDIA DRIVE team publications, GTC talks, and technical blogs.
- Engage with the AV mapping community: attend or watch NDS Association events, contribute to OSM, participate in AV-related open source projects.
- Understand NVIDIA's competitive position vs Mobileye, Waymo, Tesla's approach to maps.

### Checkpoint

You're at Level 10 when you can: present a GitHub portfolio with AV-relevant C++and ML projects, whiteboard any system design question about map services, write modern C++ that would pass NVIDIA's code review, and articulate your unique value proposition as someone who bridges geospatial domain expertise with AV engineering.

---

## Recommended Resource Stack


| Category     | Resource                                      | Format        |
| ------------ | --------------------------------------------- | ------------- |
| C++          | *A Tour of C++* (Stroustrup)                  | Book          |
| C++          | *Effective Modern C++* (Meyers)               | Book          |
| OS/Systems   | *Operating Systems: Three Easy Pieces*        | Free online   |
| ML           | fast.ai Practical Deep Learning               | Free course   |
| CV           | Stanford CS231n                               | Free lectures |
| Transformers | *Attention Is All You Need* + survey papers   | Papers        |
| GNNs         | Stanford CS224W: Machine Learning with Graphs | Free lectures |
| AV           | NVIDIA DRIVE Labs blog                        | Blog/videos   |
| Maps         | NDS.Live specification                        | Spec docs     |
| Maps         | OpenStreetMap wiki + contributor guide        | Wiki          |
| CUDA         | NVIDIA CUDA Programming Guide                 | Free docs     |
| Embedded     | *Making Embedded Systems* (Elecia White)      | Book          |
| Interviews   | *System Design Interview* (Alex Xu)           | Book          |


---

## Weekly Time Commitment Estimate

- **Aggressive (job transition mode):** 20–25 hrs/week → ~12 months to Level 10
- **Steady (alongside current job):** 10–12 hrs/week → ~18–20 months to Level 10
- **Casual (evenings/weekends):** 5–7 hrs/week → ~24+ months to Level 10

---

## Your Unfair Advantages (Don't Forget These)

1. **Geospatial fluency** — You already think in coordinates, projections, tiled data, and spatial queries. Most SWEs applying to this role will be learning that from scratch.
2. **Product thinking** — You've led a product team. You understand why features exist, not just how to build them. This matters when designing map architectures that serve multiple consumers.
3. **Full-stack spatial pipeline experience** — You've built a geospatial SaaS end-to-end. The "scalable and distributed map-building workflows" part of this job is conceptually familiar territory for you.
4. **AI tool proficiency** — The job explicitly requires this. Many senior C++ engineers are *not* AI-native developers. You are.

The path from 3 to 10 is real work — roughly a year of focused study. But you're not starting from zero. You're starting from a different angle, and that angle has genuine value.