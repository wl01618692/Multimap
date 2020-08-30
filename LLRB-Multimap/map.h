#ifndef MAP_H_
#define MAP_H_

#include <iostream>
#include <memory>
#include <string>
#include <utility>

template <typename K, typename V>
class Map {
 public:
  // Return size of tree
  unsigned int Size();
  // Return value associated to @key
  const V& Get(const K& key);
  // Return whether @key is found in tree
  bool Contains(const K& key);
  // Return max key in tree
  const K& Max();
  // Return min key in tree
  const K& Min();
  // Insert @key in tree
  void Insert(const K &key, const V &value);
  // Remove @key from tree
  void Remove(const K &key);
  // Print tree in-order
  void Print();

 private:
  enum Color { RED, BLACK };
  struct Node{
    K key;
    V value;
    bool color;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };
  std::unique_ptr<Node> root;
  unsigned int cur_size = 0;

  // Iterative helper methods
  Node* Get(Node *n, const K &key);

  // Recursive helper methods
  Node* Min(Node *n);
  void Insert(std::unique_ptr<Node> &n, const K &key, const V &value);
  void Remove(std::unique_ptr<Node> &n, const K &key);
  void Print(Node *n);

  // Helper methods for the self-balancing
  bool IsRed(Node *n);
  void FlipColors(Node *n);
  void RotateRight(std::unique_ptr<Node> &prt);
  void RotateLeft(std::unique_ptr<Node> &prt);
  void FixUp(std::unique_ptr<Node> &n);
  void MoveRedRight(std::unique_ptr<Node> &n);
  void MoveRedLeft(std::unique_ptr<Node> &n);
  void DeleteMin(std::unique_ptr<Node> &n);
};

template <typename K, typename V>
unsigned int Map<K, V>::Size() {
  return cur_size;
}

template <typename K, typename V>
typename Map<K, V>::Node* Map<K, V>::Get(Node *n, const K &key) {
  while (n) {
    if (key == n->key)
      return n;

    if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }
  return nullptr;
}

template <typename K, typename V>
const V& Map<K, V>::Get(const K &key) {
  Node *n = Get(root.get(), key);
  if (!n)
    throw std::runtime_error("Error: cannot find key");
  return n->value;
}

template <typename K, typename V>
bool Map<K, V>::Contains(const K &key) {
  return Get(root.get(), key) != nullptr;
}

template <typename K, typename V>
const K& Map<K, V>::Max(void) {
  Node *n = root.get();
  while (n->right) n = n->right.get();
  return n->key;
}

template <typename K, typename V>
const K& Map<K, V>::Min(void) {
  return Min(root.get())->key;
}

template <typename K, typename V>
typename Map<K, V>::Node* Map<K, V>::Min(Node *n) {
  if (n->left)
    return Min(n->left.get());
  else
    return n;
}

template <typename K, typename V>
bool Map<K, V>::IsRed(Node *n) {
  if (!n) return false;
  return (n->color == RED);
}

template <typename K, typename V>
void Map<K, V>::FlipColors(Node *n) {
  n->color = !n->color;
  n->left->color = !n->left->color;
  n->right->color = !n->right->color;
}

template <typename K, typename V>
void Map<K, V>::RotateRight(std::unique_ptr<Node> &prt) {
  std::unique_ptr<Node> chd = std::move(prt->left);
  prt->left = std::move(chd->right);
  chd->color = prt->color;
  prt->color = RED;
  chd->right = std::move(prt);
  prt = std::move(chd);
}

template <typename K, typename V>
void Map<K, V>::RotateLeft(std::unique_ptr<Node> &prt) {
  std::unique_ptr<Node> chd = std::move(prt->right);
  prt->right = std::move(chd->left);
  chd->color = prt->color;
  prt->color = RED;
  chd->left = std::move(prt);
  prt = std::move(chd);
}

template <typename K, typename V>
void Map<K, V>::FixUp(std::unique_ptr<Node> &n) {
  // Rotate left if there is a right-leaning red node
  if (IsRed(n->right.get()) && !IsRed(n->left.get()))
    RotateLeft(n);
  // Rotate right if red-red pair of nodes on left
  if (IsRed(n->left.get()) && IsRed(n->left->left.get()))
    RotateRight(n);
  // Recoloring if both children are red
  if (IsRed(n->left.get()) && IsRed(n->right.get()))
    FlipColors(n.get());
}

template <typename K, typename V>
void Map<K, V>::MoveRedRight(std::unique_ptr<Node> &n) {
  FlipColors(n.get());
  if (IsRed(n->left->left.get())) {
    RotateRight(n);
    FlipColors(n.get());
  }
}

template <typename K, typename V>
void Map<K, V>::MoveRedLeft(std::unique_ptr<Node> &n) {
  FlipColors(n.get());
  if (IsRed(n->right->left.get())) {
    RotateRight(n->right);
    RotateLeft(n);
    FlipColors(n.get());
  }
}

template <typename K, typename V>
void Map<K, V>::DeleteMin(std::unique_ptr<Node> &n) {
  // No left child, min is 'n'
  if (!n->left) {
    // Remove n
    n = nullptr;
    return;
  }

  if (!IsRed(n->left.get()) && !IsRed(n->left->left.get()))
    MoveRedLeft(n);

  DeleteMin(n->left);

  FixUp(n);
}

template <typename K, typename V>
void Map<K, V>::Remove(const K &key) {
  if (!Contains(key))
    return;
  Remove(root, key);
  cur_size--;
  if (root)
    root->color = BLACK;
}

template <typename K, typename V>
void Map<K, V>::Remove(std::unique_ptr<Node> &n, const K &key) {
  // Key not found
  if (!n) return;

  if (key < n->key) {
    if (!IsRed(n->left.get()) && !IsRed(n->left->left.get()))
      MoveRedLeft(n);
    Remove(n->left, key);
  } else {
    if (IsRed(n->left.get()))
      RotateRight(n);

    if (key == n->key && !n->right) {
      // Remove n
      n = nullptr;
      return;
    }

    if (!IsRed(n->right.get()) && !IsRed(n->right->left.get()))
      MoveRedRight(n);

    if (key == n->key) {
      // Find min node in the right subtree
      Node *n_min = Min(n->right.get());
      // Copy content from min node
      n->key = n_min->key;
      n->value = n_min->value;
      // Delete min node recursively
      DeleteMin(n->right);
    } else {
      Remove(n->right, key);
    }
  }

  FixUp(n);
}

template <typename K, typename V>
void Map<K, V>::Insert(const K &key, const V &value) {
  Insert(root, key, value);
  cur_size++;
  root->color = BLACK;
}

template <typename K, typename V>
void Map<K, V>::Insert(std::unique_ptr<Node> &n,
                       const K &key, const V &value) {
  if (!n)
    n = std::unique_ptr<Node>(new Node{key, value, RED, nullptr , nullptr});
  else if (key < n->key)
    Insert(n->left, key, value);
  else if (key > n->key)
    Insert(n->right, key, value);
  else
    throw std::runtime_error("Key already inserted");

  FixUp(n);
}

template <typename K, typename V>
void Map<K, V>::Print() {
  Print(root.get());
  std::cout << std::endl;
}

template <typename K, typename V>
void Map<K, V>::Print(Node *n) {
  if (!n) return;
  Print(n->left.get());
  std::cout << "<" << n->key << "," << n->value << "> ";
  Print(n->right.get());
}

#endif  // MAP_H_
