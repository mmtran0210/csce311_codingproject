#include "OpQueue.h"

OperationQueue::OperationQueue() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

OperationQueue::~OperationQueue() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void OperationQueue::enqueue(const MapOperation& op) {
    pthread_mutex_lock(&mutex);
    queue.push(op);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

bool OperationQueue::dequeue(MapOperation& op) {
    pthread_mutex_lock(&mutex);
    while (queue.empty()) {
        pthread_cond_wait(&cond, &mutex);
    }
    op = queue.front();
    queue.pop();
    pthread_mutex_unlock(&mutex);
    return true;
}
