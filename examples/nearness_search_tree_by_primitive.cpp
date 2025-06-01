#include "./util/read_mesh.hpp"
#include "trueform/closest_point.hpp"
#include "trueform/closest_point_on_triangle.hpp"
#include "trueform/distance.hpp"
#include "trueform/nearness_search.hpp"
#include "trueform/normalized.hpp"
#include "trueform/random_vector.hpp"
#include "trueform/tree.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: program <input.obj>\n";
    return 1;
  }

  std::cout << "Reading file: " << argv[1] << std::endl;
  auto [points, triangles] = tf::examples::read_mesh(argv[1]);
  std::cout << "  number of triangles: " << triangles.size() << std::endl;
  std::cout << "  number of points   : " << points.size() << std::endl;
  std::cout << "---------------------------------" << std::endl;

  using triangle_t = std::array<int, 3>;
  tf::tree<int, float, 3> mesh_tree;
  mesh_tree.build(
      tf::strategy::floyd_rivest, triangles,
      [&points = points](const triangle_t &t) {
        return tf::aabb_union(
            tf::aabb_union(tf::make_aabb(points[t[0]], points[t[0]]),
                           points[t[1]]),
            points[t[2]]);
      },
      tf::tree_config{4, 4});
  std::cout << "Build triangle tree." << std::endl;
  std::cout << "---------------------------------" << std::endl;

  std::cout << "We will pick a random point on the sphere enclosing the mesh "
               "and compute the closest point on the mesh."
            << std::endl;
  std::cout << "---------------------------------" << std::endl;

  auto center = mesh_tree.nodes().front().aabb.center();
  auto radius = mesh_tree.nodes().front().aabb.diagonal().length() / 2;
  auto query_pt = tf::normalized(tf::random_vector<float, 3>()) * radius;

  auto [primitive_id, closest_point] = tf::nearness_search(
      mesh_tree,
      [query_pt](const tf::aabb<float, 3> &aabb) {
        return tf::distance2(aabb, query_pt);
      },
      [&query_pt, &points = points,
       &triangles = triangles](const auto &triangle_id) {
        auto cpt = tf::closest_point_on_triangle(triangles[triangle_id], points,
                                                 query_pt);
        return tf::make_closest_point((cpt - query_pt).length2(), cpt);
      });

  auto [metric, point] = closest_point;

  std::cout << "Closest point: " << point[0] << ", " << point[1] << ", "
            << point[2] << " on primitive: " << primitive_id << std::endl;
  std::cout << "At distance: " << std::sqrt(metric) << " from query_pt"
            << std::endl;
}
