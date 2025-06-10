#include "trueform/inject_plane.hpp"
#include "trueform/plane.hpp"
#include "trueform/polygon.hpp"

struct A : std::array<tf::vector<float, 3>, 3> {};
struct has_plane_t : tf::inject_plane_t<float, 3, A> {};
struct has_normal_t : tf::inject_normal_t<float, 3, A> {};

int main() {
  tf::vector<float, 3> x;
  auto plane = tf::make_plane(x, x, x);

  auto inj_plane0 = tf::inject_plane(
      tf::make_polygon(has_normal_t{tf::inject_normal(plane.normal, A{})}));
  static_assert(
      std::is_base_of_v<
          tf::implementation::inject_plane_to_normal_t<float, 3, has_normal_t>,
          decltype(inj_plane0)>);
  auto inj_plane1 = tf::inject_plane(
      tf::make_polygon(has_plane_t{tf::inject_plane(plane, A{})}));
  auto inj_plane2 = tf::inject_plane(tf::make_polygon(A{}));
  static_assert(
      std::is_base_of_v<tf::implementation::inject_full_plane_t<float, 3, A>,
                        decltype(inj_plane2)>);

  static_assert(
      std::is_same_v<decltype(inj_plane0), // injects plane, as it only has a
                                           // normal
                     tf::polygon<tf::dynamic_size,
                                 tf::inject_plane_t<float, 3, has_normal_t>>>);
  static_assert(
      std::is_same_v<decltype(inj_plane1), // does not inject the plane
                     tf::polygon<tf::dynamic_size, has_plane_t>>);
  static_assert(
      std::is_same_v<
          decltype(inj_plane2), // injects plane as it has nothing
          tf::polygon<tf::dynamic_size, tf::inject_plane_t<float, 3, A>>>);

  auto inj_normal0 = tf::inject_normal(
      tf::make_polygon(has_normal_t{tf::inject_normal(plane.normal, A{})}));
  auto inj_normal1 = tf::inject_normal(
      tf::make_polygon(has_plane_t{tf::inject_plane(plane, A{})}));
  auto inj_normal2 = tf::inject_normal(tf::make_polygon(A{}));

  static_assert(
      std::is_same_v<decltype(inj_normal0), // has normal so does not inject
                     tf::polygon<tf::dynamic_size, has_normal_t>>);
  static_assert(
      std::is_same_v<decltype(inj_normal1), // has plane so does not inject
                     tf::polygon<tf::dynamic_size, has_plane_t>>);
  static_assert(
      std::is_same_v<
          decltype(inj_normal2), // injects, as it has nothing
          tf::polygon<tf::dynamic_size, tf::inject_normal_t<float, 3, A>>>);
}
