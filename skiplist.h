#include <stdlib.h>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <xmmintrin.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <bit>
#include <functional>
#include <mutex>
#include <vector>

#include <atomic>

typedef std::chrono::high_resolution_clock Clock;

// Key is an 8-byte integer
typedef uint64_t Key;

int compare_(const Key& a, const Key& b) {
    if (a < b) {
        return -1;
    } else if (a > b) {
        return +1;
    } else {
        return 0;
    }
}

template<typename Key>
class SkipList {
   private:
    struct Node;

   public:
    SkipList(int max_level = 16, float probability = 0.5);

    void Insert(const Key& key); // Insertion function (to be implemented by students)
    bool Contains(const Key& key) const; // Lookup function (to be implemented by students)
    std::vector<Key> Scan(const Key& key, const int scan_num); // Range query function (to be implemented by students)
    bool Delete(const Key& key) const; // Delete function (to be implemented by students)

   private:
    int RandomLevel(); // Generates a random level for new nodes (to be implemented by students)

    Node* head; // Head node (starting point of the SkipList)
    int max_level; // Maximum level in the SkipList
    float probability; // Probability factor for level increase
};

// SkipList Node structure
template<typename Key>
struct SkipList<Key>::Node {
    Key key;
    std::vector<Node*> next; // Pointer array for multiple levels

    // Constructor for Node
    Node(Key key, int level);
};

// Generate a random level for new nodes
template<typename Key>
int SkipList<Key>::RandomLevel() {
    // To be implemented by students
    int level = 1; // 기본 레벨 1로 설정
    while((float)rand()/RAND_MAX < 0.5 && level < max_level){level++;} // 레벨이 max_level보다 작다면 0.5의 확률로 레벨 1 증가
    return level; // Default return value (students should modify this)
}

// Constructor for SkipList
template<typename Key>
SkipList<Key>::SkipList(int max_level, float probability)
    : max_level(max_level), probability(probability) {
        // To be implemented by students
        head = new Node(Key(), max_level); // head 노드의 레벨을 max_level로 설정
        for(int i = 0; i < max_level; i++){ // 각 레벨의 포인터를 nullptr로 설정
            head->next[i] = nullptr;
        }
}

// Insert function (inserts a key into SkipList)
template<typename Key>
void SkipList<Key>::Insert(const Key& key) {
    std::vector<Node*> update(max_level);
    Node* current = head;

    for (int i = max_level - 1; i >= 0; i--){ // 높은 레벨부터 순회하면서 삽입할 위치를 찾음
        while (current->next[i] != nullptr && current->next[i]->key < key){
            current = current->next[i];
        }
        update[i] = current; // 
    }

    current = current->next[0];

    if(current != nullptr && current->key == key){
        return;
    }

    int level = RandomLevel();
    Node* new_node = new Node(key, level);

    for(int i = 0; i < level; i++){
        new_node->next[i] = update->next[i];
        update->next[i] = new_node[i];
    }

    // To be implemented by students
}

// Delete function (removes a key from SkipList)
template<typename Key>
bool SkipList<Key>::Delete(const Key& key) const {
    std::vector<Node*> update(max_level);
    Node* current = head;

    for (int i = max_level - 1; i >= 0; i--){ // 높은 레벨부터 순회하면서 Key값이 있을만한 자리를 찾음
        while (current->next[i] != nullptr && current->next[i]->key < key){
            current = current->next[i];
        }
        update[i] = current;
    }

    current = current->next[0];
    if (current != nullptr && current->key == key){
        for(int i = 0; i < current->next.size(); i++){
            if (update[i]->next[i] == current){
                update[i]->next[i] == current->next[i]; 
            }
        }
        delete current;
        return true;
    }
    // To be implemented by students
    return false;
}

// Lookup function (checks if a key exists in SkipList)
template<typename Key>
bool SkipList<Key>::Contains(const Key& key) const {
    Node* current = head;

    for (int i = max_level - 1; i >= 0; i--){ // 높은 레벨부터 순회하면서 Key값이 있을만한 자리를 찾음
        while (current->next[i] != nullptr && current->next[i]->key < key){
            current = current->next[i];
        }
    }
    current = current->next[0];
    return (current != nullptr && current->key == key);
}

// Range query function (retrieves scan_num keys starting from key)
template<typename Key>
std::vector<Key> SkipList<Key>::Scan(const Key& key, const int scan_num) {
    Node* current = head;
    std:: vector<Key> scan_result;

    for (int i = max_level - 1; i >= 0; i--){ // 높은 레벨부터 순회하면서 삽입할 위치를 찾음
        while (current->next[i] != nullptr && current->next[i]->key < key){
            current = current->next[i];
        }
    }
    for (int i = 0; i < scan_num; i++){
        current = current->next[0];
        scan_result.push_back(current->key)
    }

    // To be implemented by students
    return scan_result;
}