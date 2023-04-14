#include <vector>
#include "json.hpp"

#include "priorityqueue.h"
using namespace std;

PriorityQueue::PriorityQueue() :
    nodes_(1, KeyValuePair()),
    max_size_(1),
    size_(0) {

}

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
    // adds into the heap as the last one(size) +1. then heapify up.
    size_++;
    if (nodes_.max_size() < size_ + 1) {
        nodes_.push_back(kv);
    } else {
        nodes_[size_] = kv;
    }
    heapifyUp(size_);
}

KeyValuePair PriorityQueue::min() {
    //returns the first one because that is most likely the root.
    return nodes_[ROOT];
}

KeyValuePair PriorityQueue::removeMin() {
    //under the assumption root is the lowest value.
    KeyValuePair MinPair = nodes_[ROOT];
    removeNode(ROOT);

    return MinPair;
}

bool PriorityQueue::isEmpty() const {
    if (nodes_.empty()) {
        return true;
    }
    return false;
}

size_t PriorityQueue::size() const {
	return size_;
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
    size_t parent = PARENT(i);
	if (nodes_[i].first < nodes_[parent].first && parent != 0) {
        swap(nodes_[i], nodes_[parent]);
        heapifyUp(PARENT(i));
    }
}

void PriorityQueue::heapifyDown(size_t i) {
	size_t left = LEFT(i);
    size_t right = RIGHT(i);

    size_t smallest = i;

    if(nodes_[left].first < nodes_[smallest].first && left <= size_) { // compare left child
        smallest = left;
    }
    if(nodes_[right].first < nodes_[smallest].first && right <= size_) { // compare right child
        smallest = right;
    }
    // swap with smallest child and heapify down if there are smaller
    if (smallest != i) {
        // cout << "swap " << nodes_[i].first << " with " << nodes_[smallest].first << endl;
        swap(nodes_[i], nodes_[smallest]);
        heapifyDown(smallest);
    }
}

void PriorityQueue::removeNode(size_t i) {

    swap(nodes_[i], nodes_[size_]); // swap the node with last
    nodes_.erase(nodes_.begin() + size_); // set the swapped node to null
    size_--; // update size count

    heapifyDown(i);
}

Key PriorityQueue::getKey(size_t i) {
	return nodes_[i].first;
}

Value PriorityQueue::getValue(size_t i) {
    return nodes_[i].second;
}

size_t PriorityQueue::PARENT(size_t i) {
    return i/2; // find parent
}

size_t PriorityQueue::LEFT(size_t i) {
    return 2*i; // find left child
}
size_t PriorityQueue::RIGHT(size_t i) {
    return 2*i + 1; // find right child
}

