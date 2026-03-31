

Parent : [Localization]

Autonomous driving requires localization. The car must know exactly where it is in 3D space, it's speed and direction. 

Solution - `map-based-localization`
Map building helps here, the AI can compare what it sees with its cameras to the content of the map tiles. e.g. it sees a traffic light with its camera sensors and matches that to the traffic light in a map tile so it knows that it must be within a certain range of the given map tile. 

In more complex stacks, LIDAR sees thousands of points per second. These points are matched against 3D geometry stored in tiles, in a process called 'point cloud registration', which gives centimetre level precision. This isn't possible in simpler systems with only 2D geometry such as OSM maps, which show road centre lines, number of lanes, traffic signs, building footprints, but no surface geometry and not in real time. With 2D maps and GPS the level of localization that's possible is snapping the car to the centre of a road . This is sufficient for navigation in human driven cars, but not sufficient for control decisions.

In contrast, 3D HD maps contain surface geometry, in addition to everything 2D maps have. These are produced, typically using LIDAR and maintained in real time by the fleet itself. A car in the fleet matches its sensor data to its map tiles and if a significant deviation exists the map tile is updated. Map Tiles help cars see what's coming further ahead than its own visual/LIDAR sensors can see. "is there a junction ahead?" (2D&HDmaps), road-lines (for overtaking and speed limit rules - 2D&HD maps), road curvature (HD maps are better). The HD map provides **semantic and geometric context beyond the sensor horizon** — things the car can't see yet but needs to reason about now.

1. Open source 3D map formats                                                                                                     
                                                                                                                                    
  ┌────────────────┬────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
  │     Format     │                                                   Notes                                                    │
  ├────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────┤   
  │ OpenDRIVE      │ XML-based, describes road geometry in full 3D — lanes, elevation, curvature. The standard in simulation    │   
  │ (.xodr)        │ (CARLA uses it). Not tile-based but convertible.                                                           │   
  ├────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────┤   
  │ Lanelet2       │ Open format built for the ROS ecosystem. Represents lanes as "lanelets" with left/right boundaries in 3D.  │   
  │                │ Used in research stacks.                                                                                   │   
  ├────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────┤   
  │ Apollo         │ Baidu's extension of OpenDRIVE, open-sourced with their Apollo AV platform. Adds traffic light positions,  │   
  │ OpenDRIVE      │ crosswalks.                                                                                                │   
  └────────────────┴────────────────────────────────────────────────────────────────────────────────────────────────────────────┘   

  OpenDRIVE is the one worth knowing — it's what CARLA (the simulator in your brainstorm doc) uses natively.

2. Open 3D HD map datasets

  ┌────────────────────┬────────────────────────┬───────────────────────────────────────────────────────────┐
  │      Dataset       │        Provider        │                      What's included                      │
  ├────────────────────┼────────────────────────┼───────────────────────────────────────────────────────────┤
  │ nuScenes           │ Motional (ex-nuTonomy) │ Full HD map + LiDAR + camera, Boston + Singapore          │
  ├────────────────────┼────────────────────────┼───────────────────────────────────────────────────────────┤
  │ Argoverse 2        │ Argo AI (Ford)         │ HD map + sensor data, 6 US cities                         │
  ├────────────────────┼────────────────────────┼───────────────────────────────────────────────────────────┤

nuScenes and Argoverse 2 are the most useful for what you're building toward — both have HD maps with lane geometry you can       
actually query programmatically.

- Argoverse 2 paper — "Argoverse 2: Next Generation Acceleration of Self-Driving Research" — publicly on arXiv, describes their HD
   map format in detail.
- nuScenes paper — "nuScenes: A multimodal dataset for autonomous driving" — same, on arXiv.

The map builder pipeline
- build v1 of MapTiles
- service to make tiles available by range query - e.g. for map display use case
- service to make tiles available by lookup - e.g. for more precise localization
- fusion of updates from AV fleet > update MapTiles
- send updated MapTiles to all cars in fleet.
