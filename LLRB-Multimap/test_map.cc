#include <gtest/gtest.h>
#include <vector>

#include "map.h"
#include <algorithm>

// Test one key
TEST(Map, OneKey) {
  Map<int, int> map;
  std::vector<int> keys{2};

  for (auto i : keys) {
    map.Insert(i, i);
  }

  EXPECT_EQ(map.Contains(2), true);
  EXPECT_EQ(map.Get(2), keys[0]);
}

// Test multiple keys
TEST(Map, MultipleKeys) {
  Map<int, int> map;
  std::vector<int> keys{2, 18, 42, 43};

  // Insert a bunch of keys
  for (auto i : keys) {
    map.Insert(i, i);
  }

  // Check that every key is contained
  std::random_shuffle(keys.begin(), keys.end());
  for (auto i : keys) {
    EXPECT_EQ(map.Contains(i), true);
    EXPECT_EQ(map.Get(i), i);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
