# trueform

**High-Performance Geometry Processing**

This repository is a growing collection of fast, production-grade tools for general-purpose geometric computing.

For usage examples and benchmarks, see the [Features](#features) section.

---

## 🔧 How to Use

`trueform` is a modern C++17 header-only library and is designed to be simple to integrate.

### 📦 Requirements

- C++17 or later
- CMake ≥ 3.14
- TBB (Threading Building Blocks)


### 🛠️ CMake Integration

#### Using FetchContent

Add this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  trueform
  GIT_REPOSITORY https://github.com/xlabmedical/trueform.git
  GIT_TAG        main
)

FetchContent_MakeAvailable(trueform)

target_link_libraries(my_target PRIVATE tf::trueform)
```

---

# Features

## 📦 tf::tree

**A General-Purpose Spatial Hierarchy for Real-Time Geometry Queries.**


<p align="center">
  <img src="papers/tree/figures/libraries-cons.png" width="45%" alt="Libraries Comparison">
  <img src="papers/tree/figures/self_search_matrix.png" width="45%" alt="Self Search Matrix">
</p>

**Left:** Construction time comparison between <code>tf::tree</code>, CGAL, and libigl.  
**Right:** Performance of <code>tf::tree</code> on ε-self-intersection detection. Each curve shows query time over varying rates of duplicated points perturbed by ε.

<p align="center">
  <img src="papers/tree/figures/closest-kde.png" width="45%" alt="Closest KDE">
  <img src="papers/tree/figures/intersect-kde.png" width="45%" alt="Intersect KDE">
</p>

Distributions generated from 10,000 randomized relative placements of model pairs.  
**Left:** Closest-point queries with non-intersecting configurations.  
**Right:** Intersection queries with intersecting configurations.

For a comprehensive discussion of methodology, benchmarking, and implementation, refer to our [technical paper (PDF)](papers/tree.pdf).

---
### 🌲 Building the Tree

#### Triangle Mesh

Construct a tree over a list of triangles (defined by vertex indices) by supplying a function that returns the AABB for each triangle.

```c++
using triangle_t = std::array<int, 3>;
std::vector<tf::vector<float, 3>> points;
std::vector<triangle_t> triangles;

tf::tree<int, float, 3> tree;
tree.build(
    //tf::strategy::nth_element,
    triangles,
    [&points](const triangle_t &t) {
      return tf::aabb_union(
        tf::aabb_union(tf::make_aabb(points[t[0]], points[t[0]]), points[t[1]]),
        points[t[2]]);
    },
    tf::tree_config{4, 4});
```

#### Point Cloud


Construct a tree over a list of points, each represented by a single AABB.

```c++
tf::tree<int, float, 3> point_tree;
point_tree.build(
    //tf::strategy::nth_element,
    points,
    [](const tf::vector<float, 3> &pt) { return tf::aabb_from(pt); },
    tf::tree_config{4, 4});
```

### 🔍 Search Queries

#### Primitive-to-Tree: find all nearby points


Query the tree with a single point and collect all points within a certain tolerance.

```c++
auto query_pt = tf::random_vector<float, 3>();
float tolerance = 0.001f;
std::vector<int> ids_in_tolerance;

tf::search(
    point_tree,
    [&query_pt, tolerance](const tf::aabb<float, 3> &aabb) {
        return tf::intersects(query_pt, aabb, tolerance);
    },
    [&points, &ids_in_tolerance, query_pt, tolerance2 = tolerance * tolerance](int id) {
        if ((query_pt - points[id]).length2() < tolerance2)
            ids_in_tolerance.push_back(id);
        return false; // continue
    });
```


#### Tree-to-Tree: collect all close pairs


Find all pairs of points across two trees that are within a given tolerance of each other.


```c++

tbb::concurrent_vector<std::pair<int, int>> id_pairs_in_tolerance;

tf::search(
    point_tree0, point_tree1,
    [tolerance](const tf::aabb<float, 3> &aabb0, const tf::aabb<float, 3> &aabb1) {
        return tf::intersects(aabb0, aabb1, tolerance);
    },
    [&points0, &points1, &id_pairs_in_tolerance, tolerance2 = tolerance * tolerance](int id0, int id1) {
        if ((points0[id0] - points1[id1]).length2() < tolerance2)
            id_pairs_in_tolerance.emplace_back(id0, id1);
        return false; // continue
    },
    [] { return false; }); // never abort
```

#### Tree-to-Tree: Early exit collision query

Stop traversal as soon as a single close pair is found. Useful for collision checks.

```c++
std::atomic<bool> found_collision{false};

tf::search(
    point_tree0, point_tree1,
    [tolerance](const tf::aabb<float, 3> &aabb0, const tf::aabb<float, 3> &aabb1) {
        return tf::intersects(aabb0, aabb1, tolerance);
    },
    [&points0, &points1, &found_collision, tolerance2 = tolerance * tolerance](int id0, int id1) {
        if ((points0[id0] - points1[id1]).length2() < tolerance2) {
            found_collision = true;
            return true; // early exit
        }
        return false;
    },
    [&found_collision] {
        return found_collision.load();
    });
```

#### Self-Search: Find self-intersecting primitive pairs

Detects all pairs of primitives within a single tree that are closer than a given tolerance—useful for self-collision detection or ε-proximity merging.

```c++
tbb::concurrent_vector<std::pair<int, int>> id_pairs_in_tolerance;

tf::search_self(
    point_tree,
    [tolerance](const tf::aabb<float, 3> &aabb0,
                const tf::aabb<float, 3> &aabb1) {
        return tf::intersects(aabb0, aabb1, tolerance);
    },
    [&points0, &points1, &id_pairs_in_tolerance,
        tolearance2 = tolerance * tolerance](int id0, int id1) {
        if ((points0[id0] - points1[id1]).length2() < tolearance2)
            id_pairs_in_tolerance.emplace_back(id0, id1);
        return false;
    },
    [] { /*never abort*/
            return false;
    });
```

### 📏 Nearness Queries

`tf::tree` provides efficient closest-point queries using a Top-K sorted stack strategy, which is faster than classic priority queues for many real-world use cases. These queries can be used to compute the nearest point to a primitive or the closest pair between two trees.

---

#### Tree-to-Primitive: Closest Point

Given a query point, find the closest point stored in a spatial tree. This is useful for snapping, picking, and nearest-neighbor tasks.

```c++
auto query_pt = tf::random_vector<float, 3>();

auto [point_id, closest_point] = tf::nearness_search(
    point_tree,
    // Metric: squared distance from query to an AABB
    [&query_pt](const tf::aabb<float, 3> &aabb) {
        return tf::distance2(aabb, query_pt);
    },
    // Closest point candidate from a primitive
    [&query_pt, &points](int id) {
        return tf::make_closest_point(
            (query_pt - points[id]).length2(), points[id]);
    });

auto [metric, point] = closest_point;
```

### Tree-to-Tree: Closest Point Pair

Compute the closest pair of points between two spatial trees. This is used for contact detection, soft collision avoidance, and geometric alignment.

```c++
auto [point_id, closest_point_pair] = tf::nearness_search(
    point_tree0, point_tree1,
    // Metric: min and min-max distances between AABBs
    [](const tf::aabb<float, 3> &aabb0, const tf::aabb<float, 3> &aabb1) {
        return tf::make_aabb_metrics(aabb0, aabb1);
    },
    // Closest point pair from two primitives
    [points0, &points1](int id0, int id1) {
        return tf::make_closest_point_pair(
            (points0[id0] - points1[id1]).length2(),
            points0[id0],
            points1[id1]);
    });

auto [metric, point0, point1] = closest_point_pair;
```
