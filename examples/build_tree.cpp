#include "./util/read_mesh.hpp"
#include "trueform/tree.hpp"
#include "trueform/tree_config.hpp"
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

  using triangle_t = std::array<int, 3>;
  tf::tree<int, float, 3> mesh_tree;
  mesh_tree.build(
      /*tf::strategy::floyd_rivest (or some other strategy),*/
      triangles, tf::config_tree(4, 4, [&points = points](const triangle_t &t) {
        return tf::aabb_union(
            tf::aabb_union(tf::make_aabb(points[t[0]], points[t[0]]),
                           points[t[1]]),
            points[t[2]]);
      }));
  std::cout << "---------------------------------" << std::endl;
  std::cout << "Build triangle tree." << std::endl;

  tf::tree<int, float, 3> point_tree;
  point_tree.build(
      /*tf::strategy::floyd_rivest (or some other strategy),*/
      points, tf::config_tree(4, 4, [](const tf::vector<float, 3> &pt) {
        return tf::aabb_from(pt);
      }));
  std::cout << "---------------------------------" << std::endl;
  std::cout << "Build point tree." << std::endl;

  return mesh_tree.nodes().size() + point_tree.nodes().size();
}
