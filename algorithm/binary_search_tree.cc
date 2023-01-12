#include <iostream>
#include <cassert>

#define USE_RECURSION 1

struct Node {
  int value;
  Node* parent;
  Node* left;
  Node* right;

  constexpr explicit Node(int v)
      : value(v), parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename OStream>
void Dump(OStream& os, const Node* root) {
  if (root != nullptr) {
    if (root->left)
      Dump(os, root->left);
    os << root->value << ' ';
    if (root->right)
      Dump(os, root->right);
  }
}

template <typename OStream>
OStream& operator<<(OStream& os, const Node& root) {
  os << '[';
  Dump(os, &root);
  os << ']';
  return os;
}

inline bool Empty(const Node* node) {
  return node == nullptr;
}

Node* Search(Node* node, int value) {
  assert(!Empty(node));
#if USE_RECURSION
  return node->value == value
      ? node
      : value < node->value
          ? Search(node->left, value)
          : Search(node->right, value);
#else
  while (!Empty(node) && node->value != value)
    node = value < node->value ? node->left : node->right;
  return node;
#endif
}

Node* Min(Node* node) {
  assert(!Empty(node));

  while (!Empty(node->left))
    node = node->left;
  return node;
}

Node* Max(Node* node) {
  assert(!Empty(node));

  while (!Empty(node->right))
    node = node->right;
  return node;
}

Node* Successor(Node* node) {
  assert(!Empty(node));

  if (!Empty(node->right))
    return Min(node->right);

  Node* p = node->parent;
  while (!Empty(p) && node == p->right) {
    node = p;
    p = p->parent;
  }
  return p;
}

Node* Predecessor(Node* node) {
  assert(!Empty(node));

  if (!Empty(node->left))
    return Max(node->left);

  Node* p = node->parent;
  while (!Empty(p) && node == p->left) {
    node = p;
    p = p->parent;
  }
  return p;
}

void Insert(Node** tree, int v) {
  Node* node = *tree;
  Node* new_node = new Node(v);
  Node* p = nullptr;

  while (!Empty(node)) {
    p = node;
    if (new_node->value < node->value)
      node = node->left;
    else
      node = node->right;
  }

  if (Empty(p)) {
    *tree = new_node;
  } else if (new_node->value < p->value) {
    p->left = new_node;
    new_node->parent = p;
  } else {
    p->right = new_node;
    new_node->parent = p;
  }
}

void Transplant(Node** tree, Node* u, Node* v) {
  assert(!Empty(*tree));

  if (u->parent == nullptr) {  // root
    *tree = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
}

void Delete(Node** tree, Node* r) {
  assert(!Empty(*tree) && !Empty(r));

  Node* node = *tree;

  if (Empty(r->left)) {
    Transplant(tree, r, r->right);
  } else if (Empty(r->right)) {
    Transplant(tree, r, r->left);
  } else {
    Node* p = Min(r->right);
    if (p->parent != r) {
      Transplant(tree, p, p->right);
      p->right = r->right;
      p->right->parent = p;
    }
    Transplant(tree, r, p);
    p->left = r->left;
    p->left->parent = p;
  }
}

int main() {
  Node* tree = nullptr;

  std::cout << "Tree: " << *tree << std::endl;

  Insert(&tree, 12);
  std::cout << "Tree: " << *tree << std::endl;

  Insert(&tree, 11);
  Insert(&tree, 8);
  Insert(&tree, 32);
  Insert(&tree, 9);
  std::cout << "Tree: " << *tree << std::endl;

  Node* p = Min(tree);
  std::cout << "Minimum : " << p->value << std::endl;
  p = Max(tree);
  std::cout << "Maximum : " << p->value << std::endl;

  p = Search(tree, 32);
  std::cout << "Search(32): " << p->value << std::endl;

  Delete(&tree, p);
  std::cout << "Delete(32): " << *tree << std::endl;

  return 0;
}
