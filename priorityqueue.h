#ifndef _PRIORITYQUEUE_H_
#define _PRIORITYQUEUE_H_

#include <vector>
#include <utility>
#include "json.hpp"

typedef double                  Key;
typedef std::pair<int, int>     Value;
typedef std::pair<Key, Value>   KeyValuePair; 

class PriorityQueue {
  public:
    PriorityQueue(); // default constructor
    PriorityQueue(std::size_t max_nodes); // copy constructor
    void insert(Key k);
    void insert(KeyValuePair kv);
    KeyValuePair min();
    KeyValuePair removeMin();
    void removeNode(size_t i);
    bool isEmpty() const;
    size_t size() const;
    nlohmann::json JSON() const;
    Key getKey(size_t i);
    Value getValue(size_t i);

  private:
    void heapifyUp(size_t i);
    void heapifyDown(size_t i);
    size_t PARENT(size_t i);
    size_t LEFT(size_t i);
    size_t RIGHT(size_t i);

    std::vector<KeyValuePair>   nodes_;
    size_t                      max_size_;
    size_t                      size_;

    const static size_t         ROOT = 1;
};  // class PriorityQueue

#endif  // _PRIORITYQUEUE_H_
