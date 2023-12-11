#ifndef OPERATION_QUEUE_H
#define OPERATION_QUEUE_H

#include <queue>
#include <pthread.h>
#include <string>

// Define a structure for map operations
struct MapOperation {
    enum Type { INSERT, DELETE, LOOKUP, STOP } type;
    int key;
    std::string value; // Only used for insert operations

    // Default constructor
    MapOperation() : type(STOP), key(0), value("") {}

    // Parameterized constructor
    MapOperation(Type t, int k, const std::string& v = "") : type(t), key(k), value(v) {}
};


// Define a thread-safe queue
class OperationQueue {
private:
    std::queue<MapOperation> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

public:
    OperationQueue();
    ~OperationQueue();

    void enqueue(const MapOperation& op);
    bool dequeue(MapOperation& op);
};

#endif // OPERATION_QUEUE_H
