// Exercise 4 — Source file for TileIndex
// No scaffold provided. Write from scratch.
//
// Include tile_index.h, <fstream>, <sstream>, <stdexcept>, <algorithm>, <iostream>
//
// Implement all four methods using TileIndex:: prefix:
//   TileIndex::load    — parse CSV, try/catch stof/stoi, map::insert
//   TileIndex::find    — map::find + end() sentinel, return &it->second or nullptr
//   TileIndex::print_all  — std::for_each + lambda over tiles_
//   TileIndex::count_above — std::count_if + lambda on tiles_ values
//
// Note: iterating std::map gives std::pair<key, value> — use pair.second for the MapTile
