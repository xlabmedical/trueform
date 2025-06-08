#pragma once
#include "trueform/normalize.hpp"
#include "trueform/ray.hpp"
#include "trueform/vector_view.hpp"
#include "vtkCamera.h"
#include "vtkOBJReader.h"
#include "vtkPolyData.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include <memory>

template <typename T> struct vtk_deleter {
  void operator()(T *ptr) const {
    if (ptr) {
      ptr->Delete();
    }
  }
};

template <typename T> using vtk_unique_ptr = std::unique_ptr<T, vtk_deleter<T>>;

template <typename T> auto vtk_make_unique() {
  return vtk_unique_ptr<T>{T::New()};
}

inline auto readSTL(std::string name) {
  auto reader = vtk_make_unique<vtkSTLReader>();
  reader->SetFileName(name.c_str());
  reader->Update();
  auto out = vtk_make_unique<vtkPolyData>();
  out->ShallowCopy(reader->GetOutput());
  return out;
}

inline auto readOBJ(std::string name) {
  auto reader = vtk_make_unique<vtkOBJReader>();
  reader->SetFileName(name.c_str());
  reader->Update();
  auto out = vtk_make_unique<vtkPolyData>();
  out->ShallowCopy(reader->GetOutput());
  return out;
}

inline auto get_world_point_ray(vtkRenderer *renderer, int x, int y) {
  auto *camera = renderer->GetActiveCamera();
  tf::ray<double, 3> ray;
  if (!camera)
    return ray;

  auto origin = tf::make_vector_view<3>(camera->GetFocalPoint());
  auto ray_pos = tf::make_vector_view<3>(&ray.origin[0]);
  auto ray_dir = tf::make_vector_view<3>(&ray.direction[0]);
  if (camera->GetParallelProjection()) {
    renderer->SetDisplayPoint(x, y, 0.0);
    renderer->DisplayToWorld();
    ray_pos = tf::make_vector_view<3>(renderer->GetWorldPoint());
    ray_dir = tf::make_vector_view<3>(camera->GetDirectionOfProjection());
  } else {
    renderer->SetWorldPoint(origin[0], origin[1], origin[2], 1.0);
    renderer->WorldToDisplay();

    auto *display_coordinates = renderer->GetDisplayPoint();
    renderer->SetDisplayPoint(x, y, display_coordinates[2]);
    renderer->DisplayToWorld();

    auto *world_point = renderer->GetWorldPoint();
    auto *camera_position = camera->GetPosition();
    ray_pos = tf::make_vector_view<3>(world_point);
    ray_pos /= world_point[3];
    ray_dir = ray_pos - tf::make_vector_view<3>(camera_position);
    tf::normalize(ray_dir);
    ray_pos = tf::make_vector_view<3>(camera_position);
  }
  return ray;
}
