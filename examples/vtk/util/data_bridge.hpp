#pragma once
#include "trueform/blocked_range.hpp"
#include "trueform/point_range.hpp"
#include "trueform/polygon_range.hpp"
#include "vtkPolyData.h"

inline auto get_points(vtkPolyData *poly) {
  auto ptr =
      static_cast<float *>(poly->GetPoints()->GetData()->GetVoidPointer(0));
  return tf::make_point_range<3>(
      tf::make_range(ptr, 3 * poly->GetNumberOfPoints()));
}

#ifdef VTK_CELL_ARRAY_V2
#include "trueform/blocked_range.hpp"
inline auto get_triangle_faces(vtkCellArray *_cell_array) {
  return tf::make_blocked_range<3>(tf::make_range(
      static_cast<vtkIdType *>(
          _cell_array->GetConnectivityArray()->GetVoidPointer(0)),
      3 * _cell_array->GetNumberOfCells()));
}
#else
#include "trueform/tag_blocked_range.hpp"
inline auto get_triangle_faces(vtkCellArray *_cell_array) {
  return tf::make_tag_blocked_range<3>(
      tf::make_range(_cell_array->GetPointer(),
                     _cell_array->GetNumberOfConnectivityEntries()));
}
#endif

inline auto get_triangle_faces(vtkPolyData *poly) {
  return get_triangle_faces(poly->GetPolys());
}

inline auto get_triangles(vtkPolyData *poly) {
  return tf::make_polygon_range(get_triangle_faces(poly), get_points(poly));
}
