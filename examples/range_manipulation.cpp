#include "trueform/indirect_range.hpp"
#include "trueform/mapped_range.hpp"
#include "trueform/range.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::array<int, 3> static_ids{2, 3, 4};
  std::vector<int> ids{{2, 3, 4}};
  std::string data = "abcdef";

  auto static_indirect = tf::make_indirect_range(static_ids, data);
  // retains the static size
  auto [c, d, e] = static_indirect;
  std::cout << c << ", " << d << ", " << e << std::endl;
  auto indirect = tf::make_indirect_range(ids, data);
  std::cout << std::equal(static_indirect.begin(), static_indirect.end(),
                          indirect.begin())
            << std::endl;

  auto static_mapped = tf::make_mapped_range(
      static_indirect, [](char c) -> char { return c + ('A' - 'a'); });
  // we still retain static size
  auto [C, D, E] = static_mapped;
  std::cout << C << ", " << D << ", " << E << std::endl;

  auto mapped = tf::make_mapped_range(
      indirect, [](char c) -> char { return c + ('A' - 'a'); });

  std::cout << std::equal(static_mapped.begin(), static_mapped.end(),
                          mapped.begin())
            << std::endl;
}
