#include "./util/common.hpp"
#include "./util/data_bridge.hpp"
#include "trueform/intersects.hpp"
#include "trueform/plane.hpp"
#include "trueform/polygon_range.hpp"
#include "trueform/ray_config.hpp"
#include "trueform/ray_hit.hpp"
#include "trueform/search.hpp"
#include "trueform/tick_tock.hpp"
#include "trueform/transformation.hpp"
#include "trueform/transformed.hpp"
#include "trueform/tree.hpp"

#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkMatrix4x4.h"
#include "vtkOpenGLActor.h"
#include "vtkOpenGLPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkXRenderWindowInteractor.h"
#include <set>
#include <string>

template <class> struct print_t;

class ray_caster {
public:
  auto push_back(vtkActor *actor, vtkPolyData *poly) -> void {
    map[actor] = actors.size();
    actors.push_back(actor);
    polys.push_back(poly);
    inv_matrix.push_back(vtk_make_unique<vtkMatrix4x4>());
    trees.emplace_back();
    trees.back().build(
        tf::make_polygon_range(get_triangle_faces(poly), get_points(poly)),
        tf::config_tree(4, 4));
  }

  auto get_actors() const -> const auto & { return actors; }

  auto ray_hit(tf::ray<float, 3> ray)
      -> std::pair<vtkActor *, tf::vector<float, 3>> {
    tf::tree_ray_info<int, tf::ray_cast_info<float>> result;
    tf::ray_config<float> config{};
    vtkActor *picked = nullptr;

    for (std::size_t i = 0; i < polys.size(); ++i) {
      auto polygons = tf::make_polygon_range(get_triangle_faces(polys[i]),
                                             get_points(polys[i]));
      auto tr = inv_transform(i);
      auto l_ray = tf::transformed(ray, tr);
      auto res = tf::ray_cast(
          l_ray, trees[i],
          [&polygons](const auto &ray, auto id) {
            return tf::ray_cast(ray, polygons[id]);
          },
          config);
      if (res) {
        result = res;
        config.max_t = result.info.t;
        picked = actors[i];
      }
    }
    return std::make_pair(picked, ray.origin + result.info.t * ray.direction);
  }

  auto collision(vtkActor *actor, std::set<vtkActor *> &colliding) {
    std::size_t id = map[actor];
    auto polygons0 = tf::make_polygon_range(get_triangle_faces(polys[id]),
                                            get_points(polys[id]));
    auto tr0 = transform(id);
    const auto &tree0 = trees[id];
    for (std::size_t i = 0; i < polys.size(); ++i) {
      if (i == id)
        continue;
      auto polygons1 = tf::make_polygon_range(get_triangle_faces(polys[i]),
                                              get_points(polys[i]));
      const auto &tree1 = trees[i];
      auto tr1 = transform(i);
      auto collision = tf::search(
          tree0, tree1,
          [&](const auto &aabb0, const auto &aabb1) {
            return tf::intersects(tf::transformed(aabb0, tr0),
                                  tf::transformed(aabb1, tr1));
          },
          [&](auto i0, auto i1) {
            return tf::intersects(tf::transformed(polygons0[i0], tr0),
                                  tf::transformed(polygons1[i1], tr1));
          });
      if (collision)
        colliding.insert(actors[i]);
      else
        colliding.erase(actors[i]);
    }
  }

private:
  auto invert(int i) -> void {
    /*if (actors[i]->GetUserMatrix()->GetMTime() < inv_matrix[i]->GetMTime())
     * {*/
    vtkMatrix4x4::Invert(actors[i]->GetUserMatrix(), inv_matrix[i].get());
    inv_matrix[i]->Modified();
    /*}*/
  }

  auto inv_transform(int id) -> tf::transformation<float, 3> {
    invert(id);
    tf::transformation<float, 3> out;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 4; ++j) {
        out(i, j) = inv_matrix[id]->Element[i][j];
      }
    }
    return out;
  }

  auto transform(int id) -> tf::transformation<float, 3> {
    tf::transformation<float, 3> out;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 4; ++j) {
        out(i, j) = actors[id]->GetUserMatrix()->Element[i][j];
      }
    }
    return out;
  }

  std::map<vtkActor *, int> map;
  std::vector<vtkPolyData *> polys;
  std::vector<vtkActor *> actors;
  std::vector<vtk_unique_ptr<vtkMatrix4x4>> inv_matrix;
  std::vector<tf::tree<int, float, 3>> trees;
};

class cursor_interactor : public vtkInteractorStyleTrackballCamera {
public:
  ray_caster rc;
  std::vector<float> pick_times;
  std::vector<float> collide_times;
  int time_index = 0;
  vtkTextActor *pick_text;
  vtkTextActor *collide_text;
  std::array<double, 3> normal_mesh_color{0.8, 0.8, 0.8};
  std::array<double, 3> coliding_mesh_color{0.8, 1, 1};
  std::array<double, 3> selected_mesh_color{1, 0.9, 1};

  tf::plane<float, 3> moving_plane;
  tf::vector<float, 3> last_point;
  tf::vector<float, 3> dx;
  vtkActor *selected_actor = nullptr;
  bool selected_mode = false;
  bool camera_mode = false;
  std::set<vtkActor *> colliding;

  auto add_time(std::vector<float> &times, float t) {
    if (times.size() < 100) {
      times.push_back(t);
    } else {
      times[time_index] = t;
    }
    time_index = (time_index + 1) % 100;
    float sum = 0;
    for (auto time : times)
      sum += time;
    return sum / times.size();
  }

  auto add_pick_time(float t) {
    auto pick_time = add_time(pick_times, t);
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Picking time per frame  : %.1f mcs",
                  pick_time * 1000);
    pick_text->SetInput(buffer);
  }

  auto add_collide_time(float t) {
    auto collide_time = add_time(collide_times, t);
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Collision time per frame: %.1f mcs",
                  collide_time * 1000);
    collide_text->SetInput(buffer);
  }

  auto get_ray_and_renderer() {
    auto x = this->Interactor->GetEventPosition()[0];
    auto y = this->Interactor->GetEventPosition()[1];
    auto renderer = this->Interactor->FindPokedRenderer(x, y);
    auto ray = get_world_point_ray(renderer, x, y);
    tf::normalize(ray.direction);
    tf::ray<float, 3> _ray{ray.origin, ray.direction};
    return std::make_pair(_ray, renderer);
  }

  auto make_moving_plane(tf::vector<float, 3> origin, vtkRenderer *renderer) {
    vtkCamera *camera = renderer->GetActiveCamera();
    // Get the camera's position and focal point
    double cameraPosition[3];
    double focalPoint[3];

    camera->GetPosition(cameraPosition);
    camera->GetFocalPoint(focalPoint);
    auto normal = tf::make_unit_vector(
        tf::make_vector_view<3>(&focalPoint[0]).as<float>() -
        tf::make_vector_view<3>(&cameraPosition[0]).as<float>());
    moving_plane = tf::make_plane(normal, origin);
  }

  auto reset_active_color(vtkActor *selected_actor) {
    if (!selected_actor)
      return;
    selected_actor->GetProperty()->SetColor(
        normal_mesh_color[0], normal_mesh_color[1], normal_mesh_color[2]);
  }

  auto set_active_color(vtkActor *selected_actor) {
    selected_actor->GetProperty()->SetColor(
        selected_mesh_color[0], selected_mesh_color[1], selected_mesh_color[2]);
  }

  auto set_colliding_color(vtkActor *selected_actor) {
    selected_actor->GetProperty()->SetColor(
        coliding_mesh_color[0], coliding_mesh_color[1], coliding_mesh_color[2]);
  }

  auto move_selected(vtkActor *selected_actor) {
    for (int i = 0; i < 3; ++i)
      selected_actor->GetUserMatrix()->Element[i][3] += dx[i];
    selected_actor->GetUserMatrix()->Modified();
  }

  auto reset_colliding_colors() {
    colliding.clear();
    for (auto actor : rc.get_actors()) {
      if (actor != selected_actor)
        reset_active_color(actor);
    }
  }

  auto handle_collisions() {
    tf::tick();
    rc.collision(selected_actor, colliding);
    add_collide_time(tf::tock());
    for (auto actor : rc.get_actors()) {
      if (actor == selected_actor)
        continue;
      if (colliding.find(actor) == colliding.end()) {
        reset_active_color(actor);
      } else {
        set_colliding_color(actor);
      }
    }
  }

  auto OnLeftButtonDown() -> void override {
    if (selected_actor) {
      selected_mode = true;
      /*this->Interactor->GetRenderWindow()->HideCursor();*/
      this->Interactor->Render();
    } else {
      camera_mode = true;
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
  }

  auto OnLeftButtonUp() -> void override {
    if (selected_mode) {
      selected_mode = false;
      reset_colliding_colors();
      /*this->Interactor->GetRenderWindow()->ShowCursor();*/
      this->Interactor->Render();
    } else if (camera_mode) {
      camera_mode = false;
      vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
    }
  }

  auto OnMouseMove() -> void override {
    tf::tick();
    auto [ray, renderer] = get_ray_and_renderer();
    add_pick_time(tf::tock());
    if (!selected_mode && !camera_mode) {
      auto [actor, point] = rc.ray_hit(ray);
      if (actor) {
        make_moving_plane(point, renderer);
        if (selected_actor != actor) {
          reset_active_color(selected_actor);
          set_active_color(actor);
        }
        last_point = point;
      } else {
        reset_active_color(selected_actor);
      }
      selected_actor = actor;
      this->Interactor->Render();
    } else if (selected_mode) {
      auto next_point = tf::ray_hit(ray, moving_plane).point;
      dx = next_point - last_point;
      last_point = next_point;
      move_selected(selected_actor);
      handle_collisions();
      this->Interactor->Render();
    } else if (camera_mode) {
      vtkInteractorStyleTrackballCamera::OnMouseMove();
      this->Interactor->Render();
    }
  }

  static cursor_interactor *New();
  vtkTypeMacro(cursor_interactor, vtkInteractorStyleTrackballCamera)
};

vtkStandardNewMacro(cursor_interactor);

auto center_and_scale(vtkPolyData *poly) -> void {
  auto pts = get_points(poly);
  auto aabb = tf::aabb_from(tf::make_polygon(pts));
  auto center = aabb.center();
  auto r = aabb.diagonal().length() / 2;
  for (auto pt : pts) {
    pt -= center;
    pt *= 10 / r;
  }
}

auto set_at(vtkMatrix4x4 *mat, tf::vector<float, 3> at) -> void {
  for (int i = 0; i < 3; ++i)
    mat->Element[i][3] = at[i];
  mat->Modified();
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: program <input.stl>\n";
    return 1;
  }
  auto poly = readSTL(argv[1]);
  center_and_scale(poly.get());

  auto inter = vtk_make_unique<cursor_interactor>();
  auto renderer = vtk_make_unique<vtkRenderer>();
  auto render_window = vtk_make_unique<vtkRenderWindow>();
  auto interactor = vtk_make_unique<vtkXRenderWindowInteractor>();
  interactor->SetInteractorStyle(inter.get());
  int n_actors_in_dim = 5;
  std::size_t total_polygons = 0;
  for (int i = 0; i < n_actors_in_dim; ++i) {
    for (int j = 0; j < n_actors_in_dim; ++j) {
      total_polygons += poly->GetNumberOfPolys();
      auto mapper = vtk_make_unique<vtkOpenGLPolyDataMapper>();
      auto actor = vtk_make_unique<vtkOpenGLActor>();
      actor->SetMapper(mapper.get());
      mapper->SetInputData(poly.get());
      auto matrix = vtk_make_unique<vtkMatrix4x4>();
      matrix->Identity();
      set_at(matrix.get(), {{i * 15.f, j * 15.f, 0.f}});
      actor->SetUserMatrix(matrix.get());
      inter->rc.push_back(actor.get(), poly.get());
      renderer->AddActor(actor.get());
      inter->reset_active_color(actor.get());
    }
  }

  renderer->SetBackground(27. / 255, 43. / 255, 52. / 255);
  render_window->SetInteractor(interactor.get());
  render_window->AddRenderer(renderer.get());

  renderer->SetViewport(0.0, 0.12, 1.0, 1.0); // top 80%
  auto renderer_text = vtk_make_unique<vtkRenderer>();
  renderer_text->SetViewport(0.0, 0.0, 1.0, 0.12); // bottom 20%
  renderer_text->InteractiveOff();

  auto text0 = vtk_make_unique<vtkTextActor>();
  text0->SetInput(
      ("Total polygons in scene : " + std::to_string(total_polygons)).c_str());
  auto textprop0 = text0->GetTextProperty();
  textprop0->SetFontSize(40);
  textprop0->SetColor(1.0, 1.0, 1.0);
  textprop0->SetJustificationToLeft();
  textprop0->SetVerticalJustificationToCentered();
  text0->SetDisplayPosition(40, 55);
  renderer_text->AddActor2D(text0.get());

  auto text1 = vtk_make_unique<vtkTextActor>();
  text1->SetInput("Picking time per frame  : 0 mcs");
  auto textprop1 = text1->GetTextProperty();
  textprop1->SetFontSize(40);
  textprop1->SetColor(1.0, 1.0, 1.0);
  textprop1->SetJustificationToLeft();
  textprop1->SetVerticalJustificationToCentered();
  text1->SetDisplayPosition(40, 115);
  renderer_text->AddActor2D(text1.get());

  auto text2 = vtk_make_unique<vtkTextActor>();
  text2->SetInput("Collision time per frame: 0 mcs");
  auto textprop2 = text2->GetTextProperty();
  textprop2->SetFontSize(40);
  textprop2->SetColor(1.0, 1.0, 1.0);
  textprop2->SetJustificationToLeft();
  textprop2->SetVerticalJustificationToCentered();
  text2->SetDisplayPosition(40, 175);
  renderer_text->AddActor2D(text2.get());

  inter->pick_text = text1.get();
  inter->collide_text = text2.get();

  renderer_text->SetBackground(0.090, 0.143, 0.173); // darker tone
  render_window->AddRenderer(renderer_text.get());

  render_window->Render();
  interactor->Start();

  return 0;
}
