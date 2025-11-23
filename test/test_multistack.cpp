#include "Multistack.h"
#include <gtest/gtest.h>

TEST(MultiStackTest, Constructor) {
  MultiStack<int> ms(10, 2);
  EXPECT_EQ(ms.GetNumStacks(), 2);
  EXPECT_EQ(ms.GetTotalCapacity(), 10);
}

TEST(MultiStackTest, IsEmptyInitially) {
  MultiStack<int> ms(10, 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(ms.IsEmpty(i));
  }
}

TEST(MultiStackTest, PushPopSingleElement) {
  MultiStack<int> ms(10, 2);

  ms.Push(0, 42);
  EXPECT_FALSE(ms.IsEmpty(0));
  EXPECT_EQ(ms.Size(0), 1);

  int value = ms.Pop(0);
  EXPECT_EQ(value, 42);
  EXPECT_TRUE(ms.IsEmpty(0));
}

TEST(MultiStackTest, PushPopMultipleElements) {
  MultiStack<int> ms(20, 3);

  for (int i = 0; i < 5; ++i) {
    ms.Push(0, i * 10);
    ms.Push(1, i * 20);
  }

  EXPECT_EQ(ms.Size(0), 5);
  EXPECT_EQ(ms.Size(1), 5);
  EXPECT_TRUE(ms.IsEmpty(2));

  for (int i = 4; i >= 0; --i) {
    EXPECT_EQ(ms.Pop(0), i * 10);
    EXPECT_EQ(ms.Pop(1), i * 20);
  }
}

TEST(MultiStackTest, PeekOperation) {
  MultiStack<int> ms(10, 2);

  ms.Push(0, 100);
  ms.Push(0, 200);

  EXPECT_EQ(ms.Peek(0), 200);
  EXPECT_EQ(ms.Size(0), 2);

  ms.Pop(0);
  EXPECT_EQ(ms.Peek(0), 100);
}

TEST(MultiStackTest, StackProxyOperator) {
  MultiStack<int> ms(15, 3);

  ms[0].push(10);
  ms[1].push(20);
  ms[2].push(30);

  EXPECT_EQ(ms[0].peek(), 10);
  EXPECT_EQ(ms[1].peek(), 20);
  EXPECT_EQ(ms[2].peek(), 30);

  EXPECT_EQ(ms[0].pop(), 10);
  EXPECT_TRUE(ms[0].empty());
}

TEST(MultiStackTest, CopyConstructor) {
  MultiStack<int> ms1(10, 2);
  ms1.Push(0, 1);
  ms1.Push(0, 2);
  ms1.Push(1, 3);

  MultiStack<int> ms2 = ms1;

  EXPECT_EQ(ms2.Size(0), 2);
  EXPECT_EQ(ms2.Size(1), 1);
  EXPECT_EQ(ms2.Pop(0), 2);
  EXPECT_EQ(ms2.Pop(0), 1);
  EXPECT_EQ(ms2.Pop(1), 3);

  EXPECT_EQ(ms1.Size(0), 2);
}

TEST(MultiStackTest, MoveConstructor) {
  MultiStack<int> ms1(10, 2);
  ms1.Push(0, 42);
  ms1.Push(1, 84);

  MultiStack<int> ms2 = std::move(ms1);

  EXPECT_EQ(ms2.Size(0), 1);
  EXPECT_EQ(ms2.Size(1), 1);
  EXPECT_EQ(ms2.Pop(0), 42);
  EXPECT_EQ(ms2.Pop(1), 84);

  EXPECT_EQ(ms1.GetNumStacks(), 0);
}

TEST(MultiStackTest, AssignmentOperator) {
  MultiStack<int> ms1(10, 2);
  ms1.Push(0, 100);
  ms1.Push(1, 200);

  MultiStack<int> ms2(5, 1);
  ms2 = ms1;

  EXPECT_EQ(ms2.GetNumStacks(), 2);
  EXPECT_EQ(ms2.Size(0), 1);
  EXPECT_EQ(ms2.Size(1), 1);
  EXPECT_EQ(ms2.Pop(0), 100);
  EXPECT_EQ(ms2.Pop(1), 200);
}

TEST(MultiStackTest, EqualityOperator) {
  MultiStack<int> ms1(10, 2);
  MultiStack<int> ms2(10, 2);
  MultiStack<int> ms3(15, 3);

  ms1.Push(0, 5);
  ms2.Push(0, 5);

  EXPECT_TRUE(ms1 == ms2);
  EXPECT_FALSE(ms1 == ms3);
  EXPECT_TRUE(ms1 != ms3);
}

TEST(MultiStackTest, RepackBasic) {
  MultiStack<int> ms(10, 3);

  for (int i = 0; i < 4; ++i) {
    ms.Push(0, i);
  }

  EXPECT_TRUE(ms.IsFull(0));

  ms.Repack();

  EXPECT_FALSE(ms.IsFull(0));
  EXPECT_EQ(ms.Size(0), 4);

  for (int i = 3; i >= 0; --i) {
    EXPECT_EQ(ms.Pop(0), i);
  }
}

TEST(MultiStackTest, Iterator) {
  MultiStack<int> ms(10, 2);

  ms.Push(0, 1);
  ms.Push(0, 2);
  ms.Push(1, 3);
  ms.Push(1, 4);

  std::vector<int> values;
  for (auto it = ms.begin(); it != ms.end(); ++it) {
    values.push_back(*it);
  }

  EXPECT_EQ(values.size(), 4);
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 2);
  EXPECT_EQ(values[2], 3);
  EXPECT_EQ(values[3], 4);
}

TEST(MultiStackTest, EmptyIterator) {
  MultiStack<int> ms(10, 2);

  int count = 0;
  for (auto it = ms.begin(); it != ms.end(); ++it) {
    count++;
  }

  EXPECT_EQ(count, 0);
}

TEST(MultiStackTest, ExceptionOnEmptyPop) {
  MultiStack<int> ms(10, 2);

  EXPECT_THROW(ms.Pop(0), const char*);
  EXPECT_THROW(ms[1].pop(), const char*);
}

TEST(MultiStackTest, ExceptionOnInvalidStack) {
  MultiStack<int> ms(10, 2);

  EXPECT_THROW(ms.Push(5, 42), const char*);
  EXPECT_THROW(ms.Pop(-1), const char*);
  EXPECT_THROW(ms[10], const char*);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}