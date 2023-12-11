#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <pthread.h>
#include <cstring>

// Define the structure for the (key, value) pair
struct KeyValuePair {
    int key;
    std::string value;
    KeyValuePair* next;

    KeyValuePair(int k, const std::string& v) : key(k), value(v), next(nullptr) {}
};

// Define the hash table class
class HashTable {
private:
    std::vector<KeyValuePair*> table;
    size_t tableSize;
    pthread_mutex_t* locks;

    // Hash function to calculate the index for a given key
    size_t hashFunction(int key);

public:
    // Constructor
    HashTable(size_t size);
    // Destructor
    ~HashTable();

    // Insertion method
    bool insert(int key, const std::string& value);
    // Deletion method
    bool remove(int key);
    // Lookup method
    std::string lookup(int key);
};

#endif // HASH_TABLE_H
