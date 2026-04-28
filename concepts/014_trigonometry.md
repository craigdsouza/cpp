# Trigonometry in C++ (`<cmath>`)

## Key functions

All `<cmath>` trig functions take angles in **radians**, not degrees.

| Formula symbol | C++ |
|---|---|
| sin(x) | `std::sin(x)` |
| cos(x) | `std::cos(x)` |
| sin²(x) | `std::sin(x) * std::sin(x)` |
| √x | `std::sqrt(x)` |
| atan2(y, x) | `std::atan2(y, x)` |

## Degrees to radians

```cpp
#include <cmath>
const double PI = M_PI;  // provided by <cmath>

double degrees_to_radians(double deg) {
    return deg * (PI / 180.0);
}
```

## Haversine distance — worked example

Computes the great-circle distance between two GPS coordinates, accounting for Earth's curvature. Used in `haversine_km()` in Project 1.

```cpp
double haversine_km(double lat1, double lon1, double lat2, double lon2) {
    const double PI = M_PI;
    double to_rad = PI / 180.0;

    double lat1r = lat1 * to_rad;
    double lat2r = lat2 * to_rad;
    double dlat  = (lat2 - lat1) * to_rad;
    double dlon  = (lon2 - lon1) * to_rad;

    double a = std::sin(dlat/2) * std::sin(dlat/2)
             + std::cos(lat1r) * std::cos(lat2r)
             * std::sin(dlon/2) * std::sin(dlon/2);

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return 6371.0 * c;  // kilometres
}
```

Why not just use Pythagoras on coordinates? One degree of longitude in SF (~38° latitude) is ~88 km, not ~111 km — longitude lines converge toward the poles. Treating coordinates as flat (x, y) introduces error that grows with distance.

## Why `atan2` instead of `acos`

The formula could also be written with `acos`, but `atan2(y, x)` is numerically more stable near 0 and π and handles the full range of angles without ambiguity. Prefer `atan2` for any inverse trig computation involving a ratio.
