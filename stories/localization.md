

Parent : [Planning]

Inputs: [GPS, IMU, LIDAR, HD Map]

- GPS is the starting point, with 3-5 m error, but upto 10m in urban canyons.
- An IMU provides acceleration and rotation measurements, which are used to calibrate the last GPS reading.
By itself IMU is very error prone since it drifts over time, but with constant GPS signals as reference.
- An HD map + lidar cloud is a self calibrating loop, using these two alone cm level precision can be obtained, and GPS becomes a fallback.
- Ultimately an HD map is necessary , GPS+ Google maps doesn't suffice because you need to know which lane you're in, it doesn't know the geometry of the road so precise steering angles are impossible to guess, 

Open question - is it possible to use LIDAR itself to infer everything GPS+Google maps can't give you , e.g. upcoming geometry of road, lane boundaries, stop lines, zebra crossings. 

Tesla-approach
- constructs a 3D map (vector representation of the world) in real time using camera feed only
- reasons based on this about driving decisions
- Hydranet model used.

Boundary conditions
  ┌─────────────────────────────┬─────────────────────────┬───────────┐
  │                             │         Camera          │   LiDAR   │
  ├─────────────────────────────┼─────────────────────────┼───────────┤
  ├─────────────────────────────┼─────────────────────────┼───────────┤
  ├─────────────────────────────┼─────────────────────────┼───────────┤
  │ Works in low light          │ Poor                    │ Good      │
  ├─────────────────────────────┼─────────────────────────┼───────────┤
  │ Works in rain/fog           │ Moderate                │ Degrades  │
  ├─────────────────────────────┼─────────────────────────┼───────────┤
  └─────────────────────────────┴─────────────────────────┴───────────┘


Terms: Kalman Filter, CARLA, Neural radiance/ occupancy prediction, Hydranet