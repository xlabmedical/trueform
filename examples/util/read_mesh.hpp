#pragma once
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include "./tiny_obj_loader.h"
#include "trueform/vector.hpp"
#include <string>
#include <vector>

namespace tf::examples {
inline auto read_mesh(const std::string &filename)
    -> std::pair<std::vector<tf::vector<float, 3>>,
                 std::vector<std::array<int, 3>>> {
  tinyobj::ObjReader rd;
  rd.ParseFromFile(filename);
  auto &&pts = rd.GetAttrib().GetVertices();
  auto &&tris = rd.GetShapes()[0].mesh.indices;

  std::vector<tf::vector<float, 3>> points;
  std::vector<std::array<int, 3>> triangles;

  points.resize(pts.size() / 3);
  auto *pptr = pts.data();
  for (int i = 0; i < int(points.size()); ++i) {
    for (int j = 0; j < 3; ++j)
      points[i][j] = *pptr++;
  }

  triangles.resize(tris.size() / 3);
  auto *tptr = tris.data();
  for (int i = 0; i < int(triangles.size()); ++i) {
    for (int j = 0; j < 3; ++j)
      triangles[i][j] = (tptr++)->vertex_index;
  }

  return std::make_pair(std::move(points), std::move(triangles));
}
} // namespace tf::examples
