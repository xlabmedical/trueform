#include "trueform/inject_plane.hpp"
#include "trueform/plane.hpp"
#include "trueform/polygon.hpp"

using A = std::array<tf::vector<float, 3>, 3>;

struct B : tf::inject_plane_t<float, 3, A> {};
struct C : tf::inject_normal_t<float, 3, A> {};

template <class> struct print_t;

int main() {
  tf::vector<float, 3> x;
  auto plane = tf::make_plane(x, x, x);
  A a;
  auto b = B{tf::inject_plane(plane, a)};
  auto c = C{tf::inject_normal(plane.normal, a)};

  auto poly_b = tf::make_polygon(b);
  auto poly_c = tf::make_polygon(c);
  auto poly_a = tf::make_polygon(a);

  auto inj_plane_b = tf::inject_plane(poly_b);
  auto inj_plane_c = tf::inject_plane(poly_c);
  auto inj_plane_a = tf::inject_plane(poly_a);
  static_assert(!tf::has_injected_plane<A>);
  static_assert(tf::has_injected_plane<decltype(poly_b)>);

  static_assert(
      std::is_same_v<
          decltype(inj_plane_a),
          tf::polygon<tf::dynamic_size, tf::inject_plane_t<float, 3, A>>>);
  static_assert(
      std::is_same_v<
          decltype(inj_plane_c),
          tf::polygon<tf::dynamic_size, tf::inject_plane_t<float, 3, C>>>);
  static_assert(
      std::is_same_v<decltype(inj_plane_b), tf::polygon<tf::dynamic_size, B>>);

  auto inj_normal_b = tf::inject_normal(poly_b);
  auto inj_normal_c = tf::inject_normal(poly_c);
  auto inj_normal_a = tf::inject_normal(poly_a);
  static_assert(!tf::has_injected_normal<A>);
  static_assert(tf::has_injected_normal<decltype(poly_c)>);

  static_assert(
      std::is_same_v<
          decltype(inj_normal_a),
          tf::polygon<tf::dynamic_size, tf::inject_normal_t<float, 3, A>>>);
  static_assert(
      std::is_same_v<decltype(inj_normal_c), tf::polygon<tf::dynamic_size, C>>);
  static_assert(
      std::is_same_v<decltype(inj_normal_b), tf::polygon<tf::dynamic_size, B>>);
}
