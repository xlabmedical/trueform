#pragma once
#include "./data_bridge.hpp"
#include "trueform/blocked_range.hpp"
#include "trueform/normalize.hpp"
#include "trueform/normalized.hpp"
#include "trueform/ray.hpp"
#include "trueform/vector_view.hpp"
#include "vtkCamera.h"
#include "vtkCellData.h"
#include "vtkCommand.h"
#include "vtkFloatArray.h"
#include "vtkOBJReader.h"
#include "vtkOpenGLActor.h"
#include "vtkOpenGLSphereMapper.h"
#include "vtkOpenGLStickMapper.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include "vtkTextActor.h"
#include <memory>

template <typename T> struct vtk_deleter {
  void operator()(T *ptr) const {
    if (ptr) {
      ptr->Delete();
    }
  }
};

template <typename T> using vtk_unique_ptr = std::unique_ptr<T, vtk_deleter<T>>;

template <typename T, typename... Ts> auto vtk_make_unique(Ts &&...args) {
  return vtk_unique_ptr<T>{T::New(static_cast<Ts &&>(args)...)};
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

class RightAlignTextUpdater : public vtkCommand {
public:
  vtkRenderWindow *Window;
  vtkTextActor *Text;
  int x;
  int y;

  RightAlignTextUpdater(vtkRenderWindow *Window, vtkTextActor *Text, int x,
                        int y)
      : Window{Window}, Text{Text}, x{x}, y{y} {}

  static RightAlignTextUpdater *New(vtkRenderWindow *Window, vtkTextActor *Text,
                                    int x, int y) {
    return new RightAlignTextUpdater{Window, Text, x, y};
  }

  void Execute(vtkObject *, unsigned long, void *) override {
    int *size = Window->GetSize();
    Text->SetDisplayPosition(size[0] - x, y);
  }
};

class segments_actor {
public:
  segments_actor(float radius = 0.2f)
      : radius{radius}, sphere_data(vtk_make_unique<vtkPolyData>()),
        stick_data(vtk_make_unique<vtkPolyData>()),
        sphere_mapper(vtk_make_unique<vtkOpenGLSphereMapper>()),
        stick_mapper(vtk_make_unique<vtkOpenGLStickMapper>()),
        sphere_actor(vtk_make_unique<vtkOpenGLActor>()),
        stick_actor(vtk_make_unique<vtkOpenGLActor>()) {

    sphere_data->Initialize();
    stick_data->Initialize();
    // Configure sphere mapper
    sphere_mapper->SetInputData(sphere_data.get());
    sphere_mapper->SetRadius(radius * 2);

    sphere_actor->SetMapper(sphere_mapper.get());
    sphere_actor->GetProperty()->SetColor(0.8, 0.2, 0.2);
    sphere_actor->SetPickable(false);

    // Configure stick mapper
    stick_mapper->SetInputData(stick_data.get());
    stick_mapper->SetScaleArray("scales");
    stick_mapper->SetOrientationArray("orients");

    stick_actor->SetMapper(stick_mapper.get());
    stick_actor->GetProperty()->SetColor(0.2, 0.7, 1.0);
    stick_actor->SetPickable(false);
  }

  void reset() {
    sphere_data->Reset();
    stick_data->Reset();
  }

  void reset(const std::vector<std::array<tf::point<float, 3>, 2>> &segments) {
    auto all_points = vtk_make_unique<vtkPoints>();
    all_points->SetNumberOfPoints(segments.size() * 2);
    auto start_points = vtk_make_unique<vtkPoints>();
    start_points->SetNumberOfPoints(segments.size());
    auto scales = vtk_make_unique<vtkFloatArray>();
    scales->SetName("scales");
    scales->SetNumberOfComponents(3);
    scales->SetNumberOfTuples(segments.size());
    auto orients = vtk_make_unique<vtkFloatArray>();
    orients->SetName("orients");
    orients->SetNumberOfComponents(3);
    orients->SetNumberOfTuples(segments.size());

    auto orients_r = tf::make_blocked_range<3>(
        tf::make_range(orients->GetPointer(0), orients->GetNumberOfValues()));
    auto scales_r = tf::make_blocked_range<3>(
        tf::make_range(scales->GetPointer(0), scales->GetNumberOfValues()));
    auto all_pts_r = get_points(all_points.get());
    auto star_points_r = get_points(start_points.get());

    int index = 0;
    for (auto [pt0, pt1] : segments) {
      all_pts_r[2 * index] = pt0;
      all_pts_r[2 * index + 1] = pt1;
      star_points_r[index] = pt0;

      scales_r[index][0] = (pt0 - pt1).length();
      scales_r[index][1] = radius;
      scales_r[index][2] = 1;

      auto dir = tf::make_point_view<3>(&orients_r[index][0]);
      dir = tf::normalized(pt1 - pt0);
      index++;
    }

    sphere_data->Reset();
    stick_data->Reset();
    sphere_data->SetPoints(all_points.get());
    stick_data->SetPoints(start_points.get());
    stick_data->GetPointData()->AddArray(scales.get());
    stick_data->GetPointData()->AddArray(orients.get());
    sphere_mapper->Modified();
    stick_mapper->Modified();
  }

  void add_to_renderer(vtkRenderer *renderer) {
    renderer->AddActor(sphere_actor.get());
    /*renderer->AddActor(stick_actor.get());*/
  }

private:
  float radius;

  vtk_unique_ptr<vtkPolyData> sphere_data;
  vtk_unique_ptr<vtkPolyData> stick_data;

  vtk_unique_ptr<vtkOpenGLSphereMapper> sphere_mapper;
  vtk_unique_ptr<vtkOpenGLStickMapper> stick_mapper;

  vtk_unique_ptr<vtkOpenGLActor> sphere_actor;
  vtk_unique_ptr<vtkOpenGLActor> stick_actor;
};
