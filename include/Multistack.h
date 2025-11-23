#include <iostream>
#include <cstring>

template<typename T>
class MultiStack {
private:
  T* data;
  int* tops;
  int* capacities;
  int* starts;
  int total_capacity;
  int num_stacks;

public:
  MultiStack() : data(nullptr), tops(nullptr), capacities(nullptr),
    starts(nullptr), total_capacity(0), num_stacks(0) {
  }

  MultiStack(int total_size, int k) : total_capacity(total_size), num_stacks(k) {
    if (k <= 0 || total_size <= 0)
      throw "Invalid parameters";

    data = new T[total_capacity];
    tops = new int[num_stacks];
    capacities = new int[num_stacks];
    starts = new int[num_stacks];

    int base_capacity = total_capacity / num_stacks;
    int remainder = total_capacity % num_stacks;

    for (int i = 0; i < num_stacks; ++i) {
      capacities[i] = base_capacity + (i < remainder ? 1 : 0);
      starts[i] = (i == 0) ? 0 : starts[i - 1] + capacities[i - 1];
      tops[i] = starts[i] - 1;
    }
  }

  MultiStack(const MultiStack& other) {
    copyFrom(other);
  }

  MultiStack(MultiStack&& other) noexcept {
    moveFrom(std::move(other));
  }

  ~MultiStack() {
    clear();
  }

  MultiStack& operator=(const MultiStack& other) {
    if (this != &other) {
      clear();
      copyFrom(other);
    }
    return *this;
  }

  MultiStack& operator=(MultiStack&& other) noexcept {
    if (this != &other) {
      clear();
      moveFrom(std::move(other));
    }
    return *this;
  }

  bool operator==(const MultiStack& other) const {
    if (num_stacks != other.num_stacks || total_capacity != other.total_capacity)
      return false;

    for (int i = 0; i < num_stacks; ++i) {
      if (Size(i) != other.Size(i))
        return false;

      for (int j = 0; j < Size(i); ++j) {
        if (data[starts[i] + j] != other.data[other.starts[i] + j])
          return false;
      }
    }
    return true;
  }

  bool operator!=(const MultiStack& other) const {
    return !(*this == other);
  }

  class StackProxy;
  StackProxy operator[](int index) {
    if (index < 0 || index >= num_stacks)
      throw "Invalid stack index";
    return StackProxy(*this, index);
  }

  void Push(int stack_num, const T& value) {
    if (stack_num < 0 || stack_num >= num_stacks)
      throw "Invalid stack number";

    if (IsFull(stack_num)) {
      Repack();
      if (IsFull(stack_num))
        throw "Stack overflow after repack";
    }

    if (IsEmpty(stack_num)) {
      tops[stack_num] = starts[stack_num];
    }
    else {
      tops[stack_num]++;
    }
    data[tops[stack_num]] = value;
  }

  T Pop(int stack_num) {
    if (stack_num < 0 || stack_num >= num_stacks)
      throw "Invalid stack number";
    if (IsEmpty(stack_num))
      throw "Stack is empty";

    T value = data[tops[stack_num]];
    if (tops[stack_num] == starts[stack_num]) {
      tops[stack_num] = starts[stack_num] - 1;
    }
    else {
      tops[stack_num]--;
    }
    return value;
  }

  bool IsEmpty(int stack_num) const {
    return tops[stack_num] < starts[stack_num];
  }

  bool IsFull(int stack_num) const {
    return tops[stack_num] >= starts[stack_num] + capacities[stack_num] - 1;
  }

  void Repack() {
    int total_elements = 0;
    int* stack_sizes = new int[num_stacks];

    for (int i = 0; i < num_stacks; ++i) {
      stack_sizes[i] = Size(i);
      total_elements += stack_sizes[i];
    }

    if (total_elements > total_capacity) {
      delete[] stack_sizes;
      throw "Total capacity exceeded - cannot repack";
    }

    T* temp_data = new T[total_elements];
    int temp_index = 0;

    for (int i = 0; i < num_stacks; ++i) {
      for (int j = 0; j < stack_sizes[i]; ++j) {
        temp_data[temp_index++] = data[starts[i] + j];
      }
    }

    int* new_capacities = new int[num_stacks];
    int* new_starts = new int[num_stacks];

    int remaining_capacity = total_capacity;

    for (int i = 0; i < num_stacks; ++i) {
      new_capacities[i] = stack_sizes[i] + 1;
      remaining_capacity -= new_capacities[i];
    }

    if (remaining_capacity > 0) {
      int extra_per_stack = remaining_capacity / num_stacks;
      for (int i = 0; i < num_stacks; ++i) {
        new_capacities[i] += extra_per_stack;
      }
      remaining_capacity -= extra_per_stack * num_stacks;

      for (int i = 0; i < remaining_capacity; ++i) {
        new_capacities[i]++;
      }
    }

    new_starts[0] = 0;
    for (int i = 1; i < num_stacks; ++i) {
      new_starts[i] = new_starts[i - 1] + new_capacities[i - 1];
    }

    temp_index = 0;
    for (int i = 0; i < num_stacks; ++i) {
      capacities[i] = new_capacities[i];
      starts[i] = new_starts[i];

      if (stack_sizes[i] > 0) {
        tops[i] = starts[i] + stack_sizes[i] - 1;
        for (int j = 0; j < stack_sizes[i]; ++j) {
          data[starts[i] + j] = temp_data[temp_index++];
        }
      }
      else {
        tops[i] = starts[i] - 1;
      }
    }

    delete[] temp_data;
    delete[] stack_sizes;
    delete[] new_capacities;
    delete[] new_starts;
  }

  int Size(int stack_num) const {
    if (IsEmpty(stack_num)) return 0;
    return tops[stack_num] - starts[stack_num] + 1;
  }

  T Peek(int stack_num) const {
    if (IsEmpty(stack_num))
      throw "Stack is empty";
    return data[tops[stack_num]];
  }

  int GetTotalCapacity() const { return total_capacity; }
  int GetNumStacks() const { return num_stacks; }
  int GetStackCapacity(int stack_num) const {
    return capacities[stack_num];
  }

  friend std::ostream& operator<<(std::ostream& os, const MultiStack& ms) {
    os << "MultiStack with " << ms.num_stacks << " stacks (total capacity: "
      << ms.total_capacity << "):\n";
    for (int i = 0; i < ms.num_stacks; ++i) {
      os << "Stack " << i << " (size: " << ms.Size(i)
        << "/capacity: " << ms.capacities[i] << "): ";
      if (ms.IsEmpty(i)) {
        os << "empty";
      }
      else {
        for (int j = 0; j < ms.Size(i); ++j) {
          os << ms.data[ms.starts[i] + j] << " ";
        }
      }
      os << "\n";
    }
    return os;
  }

  class StackProxy {
  private:
    MultiStack& parent;
    int stack_num;

  public:
    StackProxy(MultiStack& p, int num) : parent(p), stack_num(num) {}

    void push(const T& value) { parent.Push(stack_num, value); }
    T pop() { return parent.Pop(stack_num); }
    bool empty() const { return parent.IsEmpty(stack_num); }
    bool full() const { return parent.IsFull(stack_num); }
    T peek() const { return parent.Peek(stack_num); }
    int size() const { return parent.Size(stack_num); }
    int capacity() const { return parent.GetStackCapacity(stack_num); }
  };

  class Iterator {
  private:
    MultiStack& stack;
    int current_stack;
    int current_index;

  public:
    Iterator(MultiStack& s, int stack_num, int index)
      : stack(s), current_stack(stack_num), current_index(index) {
    }

    bool operator!=(const Iterator& other) const {
      return current_stack != other.current_stack || current_index != other.current_index;
    }

    Iterator& operator++() {
      current_index++;

      if (current_stack < stack.num_stacks &&
        current_index >= stack.Size(current_stack)) {
        current_stack++;
        current_index = 0;

        while (current_stack < stack.num_stacks && stack.Size(current_stack) == 0) {
          current_stack++;
        }
      }

      return *this;
    }

    T& operator*() {
      return stack.data[stack.starts[current_stack] + current_index];
    }
  };

  Iterator begin() {
    int first_stack = 0;
    while (first_stack < num_stacks && Size(first_stack) == 0) {
      first_stack++;
    }

    if (first_stack >= num_stacks) {
      return end();
    }

    return Iterator(*this, first_stack, 0);
  }

  Iterator end() {
    return Iterator(*this, num_stacks, 0);
  }

private:
  void copyFrom(const MultiStack& other) {
    total_capacity = other.total_capacity;
    num_stacks = other.num_stacks;

    data = new T[total_capacity];
    tops = new int[num_stacks];
    capacities = new int[num_stacks];
    starts = new int[num_stacks];

    for (int i = 0; i < total_capacity; ++i) {
      data[i] = other.data[i];
    }

    for (int i = 0; i < num_stacks; ++i) {
      tops[i] = other.tops[i];
      capacities[i] = other.capacities[i];
      starts[i] = other.starts[i];
    }
  }

  void moveFrom(MultiStack&& other) {
    data = other.data;
    tops = other.tops;
    capacities = other.capacities;
    starts = other.starts;
    total_capacity = other.total_capacity;
    num_stacks = other.num_stacks;

    other.data = nullptr;
    other.tops = nullptr;
    other.capacities = nullptr;
    other.starts = nullptr;
    other.total_capacity = 0;
    other.num_stacks = 0;
  }

  void clear() {
    delete[] data;
    delete[] tops;
    delete[] capacities;
    delete[] starts;

    data = nullptr;
    tops = nullptr;
    capacities = nullptr;
    starts = nullptr;
    total_capacity = 0;
    num_stacks = 0;
  }
};
