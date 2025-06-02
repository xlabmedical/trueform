#include "./util/read_mesh.hpp"
#include "trueform/intersects.hpp"
#include "trueform/random.hpp"
#include "trueform/random_transformation.hpp"
#include "trueform/search.hpp"
#include "trueform/transformation.hpp"
#include "trueform/transformed.hpp"
#include "trueform/tree.hpp"
#include <atomic>
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

  // pick random point
  auto id0 = tf::random<int>(0, points.size() - 1);
  auto pt0 = points[id0];
  auto id1 = tf::random<int>(0, points.size() - 1);
  auto pt1 = points[id1];

  tf::tree<int, float, 3> tree;
  tree.build(
      /*tf::strategy::floyd_rivest (or some other strategy),*/
      points, [](const tf::vector<float, 3> &pt) { return tf::aabb_from(pt); },
      tf::tree_config{4, 4});
  std::cout << "Build point tree." << std::endl;
  std::cout << "---------------------------------" << std::endl;

  std::cout << "We will use the points of the dataset. We will place spheres "
               "with radius epsilon on each point and "
               "transform one copy of the point-cloud. Then we will find if "
               "the clouds are in collision."
            << std::endl;
  std::cout << "Selected points with ids: " << id0 << ", " << id1
            << " to align under random rotation." << std::endl;
  std::cout << "---------------------------------" << std::endl;


  // create transform that:
  // 1. makes pt1 the origin
  // 2. applies a random rotation
  // 3. aligns pt1 to pt0
  auto transformation =
      tf::transformed(tf::make_transformation_from_translation(-pt1),
                      tf::random_transformation<float, 3>(pt0));

  std::atomic<bool> are_colliding{false};
  // we may use the same tree, as we will simply
  // apply the transformation to the aabbs and primitives.
  tf::search(
      tree, tree,
      [&](const auto &aabb0, const auto &aabb1) { // transform the aabb
        return tf::intersects(aabb0, tf::transformed(aabb1, transformation),
                              std::numeric_limits<float>::epsilon());
      },
      [&points = points, &transformation, &are_colliding](auto id0, auto id1) {
        if ((points[id0] - transformation.transform_point(points[id1]))
                .length2() < std::numeric_limits<float>::epsilon()) {
          are_colliding.store(true);
          return true;
        }
        return false;
      },
      [&are_colliding] { return are_colliding.load(); });

  std::cout << "Are clouds colliding: " << (are_colliding.load() ? "yes" : "no")
            << std::endl;
}
