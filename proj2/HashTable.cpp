#include "HashTable.h"
#include <string.h>

size_t HashTable::hashFunction(int key) {
    // modulo-based hash function
    return key % tableSize;
}
HashTable::HashTable(size_t size) : table(size, nullptr), tableSize(size) {
    locks = new pthread_mutex_t[tableSize];
    for (size_t i = 0; i < tableSize; ++i) {
        pthread_mutex_init(&locks[i], nullptr);
    }
}

HashTable::~HashTable() {
    for (size_t i = 0; i < tableSize; ++i) {
        // Destroy the mutex
        pthread_mutex_destroy(&locks[i]);
        // Delete all KeyValuePair in the linked list
        KeyValuePair* entry = table[i];
        while (entry != nullptr) {
            KeyValuePair* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
    delete[] locks;
}

bool HashTable::insert(int key, const std::string& value) {
    size_t index = hashFunction(key);
    pthread_mutex_lock(&locks[index]);

    KeyValuePair* prev = nullptr;
    KeyValuePair* entry = table[index];
    while (entry != nullptr && entry->key != key) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr) {
        // Create a copy of the value string to ensure it's stored correctly.
        char* valueCopy = new char[value.length() + 1];
        std::strcpy(valueCopy, value.c_str());

        entry = new KeyValuePair(key, valueCopy); // Assuming KeyValuePair takes a char* for value.
        if (prev == nullptr) {
            table[index] = entry;
        } else {
            prev->next = entry;
        }
        pthread_mutex_unlock(&locks[index]);
        delete[] valueCopy; // Clean up the temporary copy.
        return true;
    } else {
        pthread_mutex_unlock(&locks[index]);
        return false;
    }
}


bool HashTable::remove(int key) {
    size_t index = hashFunction(key);
    pthread_mutex_lock(&locks[index]);

    KeyValuePair* prev = nullptr;
    KeyValuePair* entry = table[index];
    while (entry != nullptr && entry->key != key) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr) {
        pthread_mutex_unlock(&locks[index]);
        return false;
    } else {
        if (prev == nullptr) {
            table[index] = entry->next;
        } else {
            prev->next = entry->next;
        }
        delete entry;
        pthread_mutex_unlock(&locks[index]);
        return true;
    }
}

std::string HashTable::lookup(int key) {
    size_t index = hashFunction(key);
    pthread_mutex_lock(&locks[index]);

    KeyValuePair* entry = table[index];
    while (entry != nullptr && entry->key != key) {
        entry = entry->next;
    }

    std::string result;
    if (entry != nullptr) {
        // Assuming entry->value is a null-terminated C-style string.
        result = std::string(entry->value);
    } else {
        result = "No " + std::to_string(key);
    }

    pthread_mutex_unlock(&locks[index]);
    return result;
}

