#include <gtest/gtest.h>
#include <vector>
#include "multimap.h"
// Error testing with Get()
TEST(Multimap, GetErrorChecking) {
  Multimap<int, int> Multimap;
  for (int i = 0; i < 10; ++i) {
    EXPECT_THROW(Multimap.Get(i), std::runtime_error);
  }
}

// Test one key insertion
TEST(Multimap, OneKeyInsert) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());

  Multimap.Print();
  EXPECT_EQ(Multimap.Size(), 1);
  EXPECT_EQ(Multimap.Contains(2), true);
  EXPECT_EQ(Multimap.Get(2), keys[0]);
  EXPECT_EQ(Multimap.Min(), keys[0]);
  EXPECT_EQ(Multimap.Max(), keys[0]);
}

// Test one key remove
TEST(Multimap, OneKeyRemove) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  Multimap.Print();
  EXPECT_EQ(Multimap.Size(), 1);
  EXPECT_EQ(Multimap.Contains(2), true);
  // Remove the node because it has only one value
  Multimap.Remove(keys[0]);
  EXPECT_EQ(Multimap.Size(), 0);
  EXPECT_EQ(Multimap.Contains(2), false);
  EXPECT_THROW(Multimap.Get(2), std::runtime_error);
}

TEST(Multimap, SimpleMaxTesting) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(Multimap.Max(), 2);
  Multimap.Insert(10, 10);
  EXPECT_EQ(Multimap.Max(), 10);
}

TEST(Multimap, ComplexMaxTesting) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 3, 4, 5, 6, 7};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
    EXPECT_EQ(Multimap.Max(), i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  Multimap.Insert(10, 10);
  EXPECT_EQ(Multimap.Max(), 10);
}

TEST(Multimap, SimpleMinTesting) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(Multimap.Min(), 2);
  Multimap.Insert(10, 10);
  EXPECT_EQ(Multimap.Min(), 2);
  Multimap.Insert(1, 1);
  EXPECT_EQ(Multimap.Min(), 1);
}

TEST(Multimap, ComplexMinTesting) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 1, 0, -1, -2, -3};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
    EXPECT_EQ(Multimap.Min(), i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  Multimap.Insert(-10, -10);
  EXPECT_EQ(Multimap.Min(), -10);
}


TEST(Multimap, SimpleMaxMinTesting) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 3, 4};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(Multimap.Min(), 2);
  EXPECT_EQ(Multimap.Max(), 4);
  Multimap.Insert(10, 10);
  EXPECT_EQ(Multimap.Max(), 10);
  Multimap.Insert(1, 1);
  EXPECT_EQ(Multimap.Min(), 1);
}

TEST(Multimap, ComplexMaxMinTesting) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 3, 4};
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(Multimap.Min(), 2);
  EXPECT_EQ(Multimap.Max(), 4);
  Multimap.Insert(10, 10);
  EXPECT_EQ(Multimap.Max(), 10);
  Multimap.Insert(1, 1);
  EXPECT_EQ(Multimap.Min(), 1);
}

// Test multiple keys insertion with simple scenario
TEST(Multimap, SimpleMultipleKeysInsert) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 18, 42, 43, 16};

  // Insert a bunch of keys
  // Insertion should create new nodes
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());
  Multimap.Print();

// Check that every key is contained
  for (auto i : keys) {
    EXPECT_EQ(Multimap.Contains(i), true);
    EXPECT_EQ(Multimap.Get(i), i);
  }
  EXPECT_EQ(Multimap.Size(), 5);
  EXPECT_EQ(Multimap.Min(), 2);
  EXPECT_EQ(Multimap.Max(), 43);
}

// Test multiple keys removal with simple scenario
TEST(Multimap, SimpleMultipleKeysRemove) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 18, 42, 43, 16};

  // Insert a bunch of keys
  for (auto i : keys) {
    Multimap.Insert(i, i);
  }
  std::random_shuffle(keys.begin(), keys.end());

  // Check that every key is contained
//  std::random_shuffle(keys.begin(), keys.end());
  for (auto i : keys) {
    EXPECT_EQ(Multimap.Contains(i), true);
    EXPECT_EQ(Multimap.Get(i), i);
  }
  EXPECT_EQ(Multimap.Size(), 5);
  EXPECT_EQ(Multimap.Min(), 2);
  EXPECT_EQ(Multimap.Max(), 43);
  // Remove() should remove the nodes with key 2 and 43
  // since they have only 1 value
  Multimap.Remove(2);
  Multimap.Remove(43);
  EXPECT_EQ(Multimap.Min(), 16);
  EXPECT_EQ(Multimap.Max(), 42);
  EXPECT_EQ(Multimap.Size(), 3);
  EXPECT_EQ(Multimap.Contains(2), false);
  EXPECT_EQ(Multimap.Contains(43), false);
}

// Test multiple keys insertion with complex scenario
TEST(Multimap, ComplexMultipleKeys) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 5, 18, 42, 43, 16};
  std::vector<int> values{1, 2, 3};
  // Insertion should create new nodes first
  // Then push back the values into end of the vector
  for (auto i : keys) {
    for (auto j : values) {
      Multimap.Insert(i, j);
    }
  }
  std::random_shuffle(keys.begin(), keys.end());
  Multimap.Print();

  for (auto i : keys) {
    EXPECT_EQ(Multimap.Contains(i), true);
    EXPECT_EQ(Multimap.Get(i), 1);
  }
  EXPECT_EQ(Multimap.Size(), 18);
  EXPECT_EQ(Multimap.Min(), 2);
  EXPECT_EQ(Multimap.Max(), 43);
}

// Test multiple keys removal with complex scenario
TEST(Multimap, ComplexMultipleKeysRemove) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 5, 18, 42, 43, 16};
  std::vector<int> values{1, 2, 3};
  // Insertion all at once
  for (auto i : keys) {
    for (auto j : values) {
      Multimap.Insert(i, j);
    }
  }
  std::random_shuffle(keys.begin(), keys.end());
  // Remove value 2 from every keys
  for (auto i : keys) {
      Multimap.Remove(i);
  }
  Multimap.Print();

  for (auto i : keys) {
    EXPECT_EQ(Multimap.Contains(i), true);
    EXPECT_EQ(Multimap.Get(i), 2);
  }
  EXPECT_EQ(Multimap.Size(), 12);
  EXPECT_EQ(Multimap.Min(), 2);
  EXPECT_EQ(Multimap.Max(), 43);
}

TEST(Multimap, RemoveAllKeys) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 5, 18, 42, 43, 16};
  std::vector<int> values{1, 2, 3};
  // Insertion all at once
  for (auto i : keys) {
    for (auto j : values) {
      Multimap.Insert(i, j);
    }
  }
  std::random_shuffle(keys.begin(), keys.end());
  // Remove all values at once
  // We have an empty Multimap
  for (unsigned j = 0; j < values.size(); ++j) {
    for (auto i : keys) {
      Multimap.Remove(i);
    }
  }
  Multimap.Print();

  EXPECT_EQ(Multimap.Size(), 0);
  for (auto k : keys) {
    EXPECT_EQ(Multimap.Contains(k), false);
    EXPECT_THROW(Multimap.Get(k), std::runtime_error);
  }
  EXPECT_EQ(Multimap.Size(), 0);
}

TEST(Multimap, AlternateInsertRemoveKeys) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 5, 18, 42, 43, 16};
  std::vector<int> values{1, 2, 3};
  for (auto i : keys) {
    for (auto j : values) {
      // Create new nodes
      Multimap.Insert(i, j);
      EXPECT_EQ(Multimap.Size(), 1);
      EXPECT_EQ(Multimap.Contains(i), true);
      // Remove new nodes
      Multimap.Remove(i);
      EXPECT_EQ(Multimap.Size(), 0);
      EXPECT_EQ(Multimap.Contains(i), false);
    }
  }
  EXPECT_EQ(Multimap.Size(), 0);
}

// This unit test also checks get()
TEST(Multimap, ComplexInsertRemoveKeys) {
  Multimap<int, int> Multimap;
  std::vector<int> keys{2, 5, 18, 42, 43, 16};
  std::vector<int> values{1, 2, 3, 4};
  int count = 1;
  for (auto i : keys) {
    for (auto j : values) {
      // Create new nodes
      Multimap.Insert(i, j);
      ++count;
      // Remove the front value from each key
      if (count % 2 == 0) {
        Multimap.Remove(i);
      }
    }
  }

  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(Multimap.Size(), 12);
  for (auto i : keys) {
    EXPECT_EQ(Multimap.Get(i), 3);
    Multimap.Remove(i);
    EXPECT_EQ(Multimap.Get(i), 4);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
