// forward list

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node* next;
} Node;

Node* allocNode(int value) {
  Node* ptr = (Node*) malloc(sizeof(*ptr)); // NOLINT
  ptr->value = value;
  return ptr;
}

void freeNode(Node* node) {
  free(node);
}

void insertNode(Node** h, int value) {
  Node* node = allocNode(value);
  node->next = (*h);
  (*h) = node;
}

void printNode(const Node* ptr) {
  printf("[");
  for (; ptr; ptr = ptr->next) {
    if (ptr->next) {
      printf("%d, ", ptr->value);
    } else {
      printf("%d", ptr->value);
    }
  }
  printf("]\n");
}

void removeNode(Node** h, int value) {
  Node root;
  root.next = *h;
  for (Node* ptr = &root; ptr->next;) {
    Node* node = ptr->next;
    if (node->value == value) {
      ptr->next = ptr->next->next;
      freeNode(node);
    } else {
      ptr = ptr->next;
    }
  }
  *h = root.next;
}

int main() {
  Node* h = NULL;
  printNode(h);

  insertNode(&h, 1);
  printNode(h);

  insertNode(&h, 2);
  printNode(h);

  insertNode(&h, 2);
  printNode(h);

  insertNode(&h, 3);
  printNode(h);

  removeNode(&h, 3);
  printNode(h);

  removeNode(&h, 2);
  removeNode(&h, 2);
  printNode(h);
  return 0;
}
