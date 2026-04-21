// Exercise 4: Integration — Road Segment Parser
// No scaffold. Write this program from scratch.
//
// Data file: data/road_segments.csv
// Header: segment_id,start_lat,start_lon,end_lat,end_lon,road_class,speed_limit
//
// Implement:
//   struct RoadSegment — all seven fields
//   class RoadGraph:
//     - owns std::vector<RoadSegment> segments_
//     - void load(const std::string& path)          — parse CSV, skip header, skip malformed
//     - void print_all() const                      — std::for_each + lambda
//     - int count_by_class(const std::string&) const — std::count_if + lambda
//     - const RoadSegment* fastest() const          — highest speed_limit; nullptr if empty
//     - void normalize_speeds()                     — std::transform + captured max; no-op if empty
//     - float average_speed() const                 — average of all speed_limits
//
// In main:
//   1. load("data/road_segments.csv") and print all segments
//   2. print count of "motorway" and "primary" segments
//   3. find and print the fastest segment
//   4. print average speed before normalization
//   5. normalize speeds, print all — verify all in [0,1]
