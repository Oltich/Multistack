#include "Multistack.h"
#include <iostream>

int main() {
  try {
    std::cout << "=== Testing MultiStack ===\n";

    MultiStack<int> ms(15, 3);

    std::cout << "Initial state:\n" << ms << std::endl;

    std::cout << "Pushing elements...\n";
    for (int i = 0; i < 3; ++i) {
      ms.Push(0, i * 10 + 1);
      ms.Push(1, i * 10 + 2);
      ms.Push(2, i * 10 + 3);
    }

    std::cout << ms << std::endl;

    std::cout << "operator[]:\n";
    ms[0].push(100);
    ms[1].push(200);
    std::cout << "Top of stack 0: " << ms[0].peek() << std::endl;
    std::cout << "Size of stack 1: " << ms[1].size() << std::endl;
    std::cout << "Capacity of stack 2: " << ms[2].capacity() << std::endl;

    std::cout << "\nPopping elements:\n";
    for (int i = 0; i < 2; ++i) {
      std::cout << "Pop from stack 0: " << ms.Pop(0) << std::endl;
      std::cout << "Pop from stack 1: " << ms[1].pop() << std::endl;
    }

    std::cout<< ms << std::endl;

    std::cout << "== Testing repack ==\n";

    for (int i = 0; i < 8; ++i) {
      if (!ms.IsFull(0)) {
        ms.Push(0, 100 + i);
      }
    }

    std::cout << "Before repack:\n" << ms << std::endl;
    ms.Repack();
    std::cout << "After repack:\n" << ms << std::endl;

    std::cout << "Using iterator:\n";
    for (auto it = ms.begin(); it != ms.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << std::endl;

 
    MultiStack<int> ms2 = ms;
    std::cout << "ms == ms2: " << (ms == ms2) << std::endl;

    ms2.Push(0, 9999);
    std::cout << "ms == ms2 after modification: " << (ms == ms2) << std::endl;

    MultiStack<int> ms3(5, 2);
    std::cout << "Empty multistack:\n" << ms3 << std::endl;

    std::cout << "Is stack 0 empty? " << ms3.IsEmpty(0) << std::endl;
    std::cout << "Is stack 1 empty? " << ms3[1].empty() << std::endl;

  }
  catch (const char* msg) {
    std::cerr << "Error: " << msg << std::endl;
  }

  return 0;
}