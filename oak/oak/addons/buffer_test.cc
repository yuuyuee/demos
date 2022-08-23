// Copyright 2022 The Oak Authors.

#include <string.h>
#include "oak/addons/buffer_internal.h"
#include "gtest/gtest.h"

namespace {
TEST(OAK_BUFFER_TEST, Initialize) {
  struct oak_buffer buffer;
  oak_buffer_init(&buffer, 100);
  EXPECT_EQ(buffer.capacity, 100U);
  EXPECT_EQ(buffer.size, 0U);
  oak_buffer_free(&buffer);

  struct oak_buffer buffer2;
  oak_buffer_init(&buffer2, 0);
  EXPECT_EQ(buffer2.capacity, 0U);
  EXPECT_EQ(buffer2.size, 0U);
  oak_buffer_free(&buffer2);

  struct oak_buffer* buffer3 = (struct oak_buffer*) malloc(sizeof(*buffer3));
  oak_buffer_init(buffer3, 100);
  EXPECT_EQ(buffer3->capacity, 100U);
  EXPECT_EQ(buffer3->size, 0U);
  oak_buffer_free(buffer3);
  free(buffer3);

  struct oak_buffer* buffer4 = (struct oak_buffer*) malloc(sizeof(*buffer4));
  oak_buffer_init(buffer4, 100);
  EXPECT_EQ(buffer4->capacity, 100U);
  EXPECT_EQ(buffer4->size, 0U);
  oak_buffer_free(buffer4);
  free(buffer4);
}

TEST(OAK_BUFFER_TEST, InitializeNotOwned) {
  const char* foo = "foo";
  struct oak_buffer buffer;
  oak_buffer_init2(&buffer, foo, strlen(foo));
  EXPECT_EQ(buffer.capacity, strlen(foo));
  EXPECT_EQ(buffer.size, strlen(foo));
  EXPECT_EQ(oak_buffer_compare_str(&buffer, foo, strlen(foo)), 0);
  oak_buffer_free(&buffer);
}

TEST(OAK_BUFFER_TEST, ClearEmpty) {
  struct oak_buffer b1, b2;
  oak_buffer_init(&b1, 100);
  oak_buffer_init(&b2, 100);

  EXPECT_EQ(b1.capacity, 100U);
  EXPECT_EQ(b1.size, 0U);
  EXPECT_TRUE(oak_buffer_empty(&b1));
  EXPECT_EQ(b2.capacity, 100U);
  EXPECT_EQ(b2.size, 0U);
  EXPECT_TRUE(oak_buffer_empty(&b2));

  const char* foo = "foo";
  oak_buffer_assign_str(&b1, foo, strlen(foo));
  oak_buffer_assign(&b2, &b1);

  EXPECT_EQ(b1.capacity, 100U);
  EXPECT_EQ(b1.size, strlen(foo));
  EXPECT_FALSE(oak_buffer_empty(&b1));
  EXPECT_EQ(b2.capacity, 100U);
  EXPECT_EQ(b2.size, strlen(foo));
  EXPECT_FALSE(oak_buffer_empty(&b2));

  oak_buffer_clear(&b1);
  oak_buffer_clear(&b2);

  EXPECT_EQ(b1.capacity, 100U);
  EXPECT_EQ(b1.size, 0U);
  EXPECT_TRUE(oak_buffer_empty(&b1));
  EXPECT_EQ(b2.capacity, 100U);
  EXPECT_EQ(b2.size, 0U);
  EXPECT_TRUE(oak_buffer_empty(&b2));

  oak_buffer_free(&b1);
  oak_buffer_free(&b2);
}

TEST(OAK_BUFFER_TEST, Move) {
  struct oak_buffer b1, b2, b3;
  oak_buffer_init(&b1, 0);
  oak_buffer_init(&b2, 0);
  oak_buffer_init(&b3, 0);

  const char* foo = "foo";
  const char* bar = "bar";
  oak_buffer_assign_str(&b1, foo, strlen(foo));
  oak_buffer_assign_str(&b2, bar, strlen(bar));
  oak_buffer_assign_str(&b3, bar, strlen(bar));
  EXPECT_EQ(b1.size, strlen(foo));
  EXPECT_EQ(b2.size, strlen(bar));
  EXPECT_EQ(b3.size, strlen(bar));

  oak_buffer_move(&b1, &b2);
  EXPECT_EQ(b1.size, 3U);
  EXPECT_EQ(b2.size, 0U);
  EXPECT_EQ(oak_buffer_compare(&b1, &b3), 0);

  oak_buffer_free(&b1);
  oak_buffer_free(&b2);
  oak_buffer_free(&b3);
}

TEST(OAK_BUFFER_TEST, Assign) {
  struct oak_buffer b1, b2;
  oak_buffer_init(&b1, 0);
  oak_buffer_init(&b2, 0);

  const char* foo = "foo";
  oak_buffer_assign_str(&b1, foo, strlen(foo));
  oak_buffer_assign(&b2, &b1);

  EXPECT_EQ(b1.capacity, strlen(foo));
  EXPECT_EQ(b1.size, strlen(foo));

  EXPECT_EQ(b2.capacity, strlen(foo));
  EXPECT_EQ(b2.size, strlen(foo));

  oak_buffer_free(&b1);
  oak_buffer_free(&b2);
}

TEST(OAK_BUFFER_TEST, Append) {
  struct oak_buffer b1, b2;
  oak_buffer_init(&b1, 0);
  oak_buffer_init(&b2, 0);

  const char* foo = "foo";
  const char* bar = "bar";
  oak_buffer_assign_str(&b1, foo, strlen(foo));

  oak_buffer_append(&b2, &b1);
  EXPECT_EQ(b2.capacity, (size_t) (strlen(foo) * 1.5));
  EXPECT_EQ(b2.size, strlen(foo));

  oak_buffer_append_str(&b2, bar, strlen(bar));
  EXPECT_EQ(b2.capacity, (size_t) ((strlen(foo) + strlen(bar)) * 1.5));
  EXPECT_EQ(b2.size, strlen(foo) + strlen(bar));

  oak_buffer_free(&b1);
  oak_buffer_free(&b2);
}

TEST(OAK_BUFFER_TEST, Compare) {
  struct oak_buffer b1, b2;
  oak_buffer_init(&b1, 0);
  oak_buffer_init(&b2, 0);

  const char* foo = "foo";
  const char* bar = "bar";
  oak_buffer_assign_str(&b1, foo, strlen(foo));
  oak_buffer_assign_str(&b2, foo, strlen(foo));
  EXPECT_EQ(oak_buffer_compare(&b1, &b2), 0);

  oak_buffer_append_str(&b2, bar, strlen(bar));
  EXPECT_NE(oak_buffer_compare(&b1, &b2), 0);

  oak_buffer_free(&b1);
  oak_buffer_free(&b2);
}

}  // anonymous namespace
