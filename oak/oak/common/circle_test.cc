// Copyright 2022 The Oak Authors.

#include <vector>
#include "oak/common/circle.h"
#include "gtest/gtest.h"

namespace {

struct Data {
  int value;
  Circle node;
};

std::vector<int> CircleToVector(Circle* h) {
  std::vector<int> vec;
  Circle* n;
  OAK_CIRCLE_FOREACH(n, h) {
    Data* p = OAK_CIRCLE_DATA(n, Data, node);
    vec.push_back(p->value);
  }
  return vec;
}

void FreeCircle(Circle* h) {
  while (!OAK_CIRCLE_EMPTY(h)) {
    Circle* n = OAK_CIRCLE_NEXT(h);
    OAK_CIRCLE_REMOVE(n);
    Data* p = OAK_CIRCLE_DATA(n, Data, node);
    delete p;
  }
}

TEST(CircleTest, InsertHead) {
  Circle h;
  Data* p = nullptr;

  OAK_CIRCLE_INIT(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    OAK_CIRCLE_INIT(&p->node);
    OAK_CIRCLE_INSERT_HEAD(&h, &p->node);
  }

  Circle* c;
  int index = 10;
  OAK_CIRCLE_FOREACH(c, &h) {
    Data* p = OAK_CIRCLE_DATA(c, Data, node);
    EXPECT_EQ(p->value, --index);
  }

  FreeCircle(&h);
}

TEST(CircleTest, InsertTail) {
  Circle h;
  Data* p = nullptr;

  OAK_CIRCLE_INIT(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    OAK_CIRCLE_INIT(&p->node);
    OAK_CIRCLE_INSERT_TAIL(&h, &p->node);
  }

  Circle* c;
  int index = -1;
  OAK_CIRCLE_FOREACH(c, &h) {
    Data* p = OAK_CIRCLE_DATA(c, Data, node);
    EXPECT_EQ(p->value, ++index);
  }

  FreeCircle(&h);
}

TEST(CircleTest, ADD) {
  Circle h1, h2;
  Data* p = nullptr;
  std::vector<int> vec1, vec2, vec3;

  OAK_CIRCLE_INIT(&h1);
  OAK_CIRCLE_INIT(&h2);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    OAK_CIRCLE_INIT(&p->node);

    if (i % 2 == 0) {
      OAK_CIRCLE_INSERT_TAIL(&h1, &p->node);
      vec1.push_back(p->value);
    } else {
      OAK_CIRCLE_INSERT_TAIL(&h2, &p->node);
      vec2.push_back(p->value);
    }
  }

  for (const auto& v : vec2)
    vec1.push_back(v);

  OAK_CIRCLE_ADD(&h1, &h2);
  OAK_CIRCLE_INIT(&h2);
  std::vector<int> exp1{0, 2, 4, 1, 3};
  EXPECT_EQ(vec1, exp1);

  FreeCircle(&h1);
  FreeCircle(&h2);
}

TEST(CircleTest, SplitMove) {
  Circle h1, h2, h3, h4;
  Data* p = nullptr;

  OAK_CIRCLE_INIT(&h1);
  OAK_CIRCLE_INIT(&h2);
  OAK_CIRCLE_INIT(&h3);
  OAK_CIRCLE_INIT(&h4);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    OAK_CIRCLE_INIT(&p->node);
    if (i % 2 == 0) {
      OAK_CIRCLE_INSERT_TAIL(&h1, &p->node);
    } else {
      OAK_CIRCLE_INSERT_TAIL(&h2, &p->node);
    }
  }

  // test OAK_CIRCLE_SPLIT
  // h1 -> 0, 2, 4
  Circle* n = OAK_CIRCLE_NEXT(OAK_CIRCLE_NEXT(&h1));
  p = OAK_CIRCLE_DATA(n, Data, node);
  EXPECT_EQ(p->value, 2);
  OAK_CIRCLE_SPLIT(&h1, n, &h3);
  std::vector<int> exp1{0};
  std::vector<int> exp3{2, 4};
  EXPECT_EQ(CircleToVector(&h1), exp1);
  EXPECT_EQ(CircleToVector(&h3), exp3);

  // test OAK_CIRCLE_MOVE
  // h2 -> 1, 3
  OAK_CIRCLE_MOVE(&h2, &h4);
  EXPECT_TRUE(OAK_CIRCLE_EMPTY(&h2));
  std::vector<int> exp4{1, 3};
  EXPECT_EQ(CircleToVector(&h4), exp4);

  FreeCircle(&h1);
  FreeCircle(&h2);
  FreeCircle(&h3);
  FreeCircle(&h4);
}

}  // anonymous namespace
