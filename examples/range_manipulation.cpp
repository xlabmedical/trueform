#include "trueform/block_indirect_range.hpp"
#include "trueform/blocked_range.hpp"
#include "trueform/indirect_range.hpp"
#include "trueform/mapped_range.hpp"
#include "trueform/offset_block_range.hpp"
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
  auto [c_id, d_id, e_id] = static_indirect.ids();
  std::cout << c_id << ": " << c << ", " << d_id << ": " << d << ", " << e_id
            << ": " << e << std::endl;
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

  std::vector<int> ds{{0, 1, 2, 3, 4, 5}};
  // we can go static
  for (auto [x0, x1, x2] : tf::make_blocked_range<3>(ds))
    std::cout << x0 << ", " << x1 << ", " << x2 << std::endl;
  // or dynamic
  for (auto r : tf::make_blocked_range(ds, 3))
    std::cout << r[0] << ", " << r[1] << ", " << r[2] << std::endl;

  // we can have indirect blocks, usefull for indexing points
  // using face ids
  for (auto r :
       tf::make_block_indirect_range(tf::make_blocked_range<3>(ds), data)) {
    auto [c0, c1, c2] = r;
    auto [id0, id1, id2] = r.ids();
    std::cout << id0 << ": " << c0 << ", " << id1 << ": " << c1 << ", " << id2
              << ": " << c2 << std::endl;
  }

  // size of offsets is one more than the number of blocks
  std::vector<int> offsets{{0, 2, 5, 6}};
  for(auto r:tf::make_offset_block_range(offsets, data)) {
    for(auto c:r)
      std::cout << c << ", ";
    std::cout << std::endl;
  }
}
