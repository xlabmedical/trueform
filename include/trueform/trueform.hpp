/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

/**
 * @mainpage trueform
 *
 * @section intro_sec High-Performance Geometry Processing
 *
 * **trueform** is a modular, production-grade C++ library for general-purpose geometric computing.
 * It offers efficient data structures, parallel algorithms, and expressive range-based APIs 
 * for solving real-world geometry problems at scale.
 *
 */

/** @defgroup geometry Geometry
 *  @brief Core geometric types and operations.
 *
 *  This module includes basic math primitives such as vectors and axis-aligned bounding boxes (AABBs),
 *  along with common geometric functions like dot products, intersections, and distance calculations.
 *
 *  @{
 */
#include "./aabb.hpp"
#include "./aabb_from.hpp"
#include "./aabb_metrics.hpp"
#include "./aabb_union.hpp"
#include "./closest_point.hpp"
#include "./closest_point_on_triangle.hpp"
#include "./closest_point_pair.hpp"
#include "./distance.hpp"
#include "./dot.hpp"
#include "./intersects.hpp"
#include "./minimal_maximal_distance.hpp"
#include "./normalize.hpp"
#include "./normalized.hpp"
#include "./transformation.hpp"
#include "./transformed.hpp"
#include "./vector.hpp"
#include "./vector_view.hpp"
/** @} */


/** @defgroup ranges Ranges
 *  @brief Composable range and iterator adapters.
 *
 *  These views allow for zero-copy transformation and traversal of structured data,
 *  making it possible to operate on existing layouts (e.g. triangles, ids, point clouds)
 *  without manual slicing or allocation.
 *
 *  The provided range factories are tailored specifically to geometric processing,
 *  with support for block-based traversal (e.g. triangle lists), indirect access patterns,
 *  and offset-based grouping. Each range adapter preserves or propagates static size information
 *  where available, enabling efficient compile-time dispatch and structured bindings on subranges.
 *
 *  This makes them ideal for composing high-performance geometry pipelines without sacrificing flexibility.
 *
 *  @{
 */

#include "./block_indirect_range.hpp"
#include "./blocked_range.hpp"
#include "./indirect_range.hpp"
#include "./mapped_range.hpp"
#include "./offset_block_range.hpp"
#include "./range.hpp"
#include "./sequence_range.hpp"
/** @} */

/** @defgroup spatial_structures Spatial Structures
 *  @brief Hierarchical spatial data structures.
 *
 *  Includes fast bounding-volume hierarchies (`tree`, `mod_tree`) for static and dynamic
 *  spatial indexing, along with customizable partitioning strategies.
 *
 *  @{
 */
#include "./tree.hpp"
#include "./mod_tree.hpp"
#include "./tree_config.hpp"
#include "./tree_node.hpp"
#include "./partitioning.hpp"
/** @} */

/** @defgroup spatial_queries Spatial Queries
 *  @brief Search, proximity, and spatial relationships.
 *
 *  Provides efficient algorithms for nearest-neighbor search, self, and pairwise intersection aggregtion or collision detection,
 *  and spatial joins using BVH trees. Includes broad-phase and narrow-phase utilities.
 *
 *  @{
 */
#include "./nearness_search.hpp"
#include "./search.hpp"
#include "./search_broad.hpp"
#include "./search_self.hpp"
#include "./search_self_broad.hpp"
#include "./tree_closest_point.hpp"
#include "./tree_closest_point_pair.hpp"
#include "./tree_knn.hpp"
/** @} */

/** @defgroup random Random Utilities
 *  @brief Random generation for testing and sampling.
 *
 *  Includes random scalar/vector generation, as well as
 *  uniform random rigid body transformations for stochastic setups.
 *
 *  @{
 */
#include "./random.hpp"
#include "./random_transformation.hpp"
#include "./random_vector.hpp"
/** @} */

/** @defgroup utilities Utilities
 *  @brief Infrastructure utilities for memory, mapping, and profiling.
 *
 *  Includes buffers, sparse ID mapping, and lightweight timing helpers.
 *
 *  @{
 */
#include "./mapping.hpp"
#include "./buffer.hpp"
#include "./small_buffer.hpp"
#include "./static_size.hpp"
#include "./tick_tock.hpp"
/** @} */

/** @defgroup algorithms Algorithms
 *  @brief Parallel and functional primitives.
 *
 *  Includes high-performance utilities for applying, copying, filtering, and
 *  partitioning data ranges in parallel — using Intel TBB where applicable.
 *
 *  @{
 */
#include "./invert_map_with_nones.hpp"
#include "./mask_to_map.hpp"
#include "./mask_to_mapping.hpp"
#include "./parallel_apply.hpp"
#include "./parallel_copy.hpp"
#include "./parallel_for.hpp"
#include "./parallel_iota.hpp"
#include "./partition_range_into_parts.hpp"
/** @} */
