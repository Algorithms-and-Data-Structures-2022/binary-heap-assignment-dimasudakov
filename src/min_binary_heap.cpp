#include "assignment/min_binary_heap.hpp"

#include <algorithm>  // fill
#include <stdexcept>  // invalid_argument
#include <limits>     // numeric_limits
#include <queue>

using namespace std;

namespace assignment {

  MinBinaryHeap::MinBinaryHeap(int capacity) {
    if (capacity <= 0) {
      throw invalid_argument("capacity must be positive");
    }

    size_ = 0;
    capacity_ = capacity;
    data_ = new Node[capacity_];
    fill(data_, data_ + capacity_, Node{});
  }

  MinBinaryHeap::~MinBinaryHeap() {
    size_ = 0;
    capacity_ = 0;

    delete[] data_;
    data_ = nullptr;
  }

  bool MinBinaryHeap::Insert(int key, int value) {

    if (size_ == capacity_) {
      return false;
    }

    data_[size_].key = key;
    data_[size_].value = value;
    size_++;
    sift_up(size_ - 1);
    return true;
  }

  std::optional<int> MinBinaryHeap::Extract() {

    if (size_ == 0) {
      return std::nullopt;
    }

    int res = data_[0].value;
    size_--;
    if(size_ != 0) {
      data_[0] = data_[size_];
      heapify(0);
    }
    return res;
  }

  bool MinBinaryHeap::Remove(int key) {
    constexpr int min_key_value = std::numeric_limits<int>::min();

    optional<int> pos = search_index(key);
    if(pos == nullopt) return false;
    data_[pos.value()].key = min_key_value;
    sift_up(pos.value());
    Extract();

    return true;
  }

  void MinBinaryHeap::Clear() {
    size_ = 0;
  }

  std::optional<int> MinBinaryHeap::Search(int key) const {
    for(int i = 0; i < size_; i++) {
      if(data_[i].key == key) return data_[i].value;
    }
    return std::nullopt;
  }

  bool MinBinaryHeap::Contains(int key) const {
    return Search(key).has_value();
  }

  bool MinBinaryHeap::IsEmpty() const {
    return size_ == 0;
  }

  int MinBinaryHeap::capacity() const {
    return capacity_;
  }

  int MinBinaryHeap::size() const {
    return size_;
  }

  // вспомогательные функции

  void MinBinaryHeap::sift_up(int index) {
    while (index != 0 && data_[index].key < data_[parent_index(index)].key) {

      std::swap(data_[index], data_[parent_index(index)]);
      index = parent_index(index);
    }
  }

  void MinBinaryHeap::heapify(int index) {

    // индексы левого и правого потомков узла с индексом index
    const int left_index = left_child_index(index);
    const int right_index = right_child_index(index);

    // вышли за пределы массива, останавливаемся
    if (left_index >= size_) {
      return;
    }

    // индекс узла-потомка с наименьшим значением ключа
    int smallest_key_index = index;

    if (data_[left_index].key < data_[index].key) {
      smallest_key_index = left_index;
    }

    if (data_[right_index].key < data_[smallest_key_index].key) {
      smallest_key_index = right_index;
    }

    // если индекс наименьшего узла найден (не равен индексу самого узла)
    if (smallest_key_index != index) {

      // меняем местами родителя и потомка (swap)
      std::swap(data_[index], data_[smallest_key_index]);

      // рекурсивно спускаемся по куче, следуя индексу
      heapify(smallest_key_index);
    }
  }

  std::optional<int> MinBinaryHeap::search_index(int key) const {
    for(int i = 0; i < size_; i++) {
      if(data_[i].key == key) {
        return i;
      }
    }
    return std::nullopt;
  }

}  // namespace assignment
