// https://github.com/mmtran0210/csce311_codingproject.git

// My Tran - CSCE 311 - Project 2 The bonus

#include "HashTable.h"
#include "OpQueue.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <pthread.h>

// Global operation queue
OperationQueue operationQueue;

// Global output vector to store results
std::vector<std::string> outputResults;

// Mutex for outputResults
pthread_mutex_t outputMutex;

// Thread function to process operations
void* processOperations(void* arg) {
    HashTable* map = static_cast<HashTable*>(arg);
    MapOperation op;

    while (operationQueue.dequeue(op)) {
        if (op.type == MapOperation::STOP) {
            break; // Exit the loop and terminate the thread
        }
        std::string result;
        switch (op.type) {
            case MapOperation::INSERT:
                result = map->insert(op.key, op.value) ? "OK" : "Fail";
                break;
            case MapOperation::DELETE:
                result = map->remove(op.key) ? "OK" : "Fail";
                break;
            case MapOperation::LOOKUP:
                result = map->lookup(op.key);
                // Debug statement to print the lookup result
                std::cout << "DEBUG: Lookup result for key " << op.key << ": '" << result << "'" << std::endl;
                break;
            case MapOperation::STOP:
                return nullptr;
        }
        pthread_mutex_lock(&outputMutex);
        outputResults.push_back(result);
        pthread_mutex_unlock(&outputMutex);
    }

    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    // Open the input file
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    // Read the number of threads from the first line
    std::string line;
    std::getline(inputFile, line);
    int numThreads;
    std::istringstream(line.substr(2)) >> numThreads;

    // Initialize the hash table
    HashTable map(1 << 20);

    // Initialize the mutex for outputResults
    pthread_mutex_init(&outputMutex, nullptr);

    // Create threads
    std::vector<pthread_t> threads(numThreads);
    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threads[i], nullptr, processOperations, &map);
    }

    // Read and enqueue commands from the input file
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        char command;
        int key;
        std::string value;
        iss >> command >> key;
        if (command == 'I') {
            iss.ignore(3); // Ignore the space and opening quote
            std::getline(iss, value, '"');
            operationQueue.enqueue(MapOperation(MapOperation::INSERT, key, value));
        } else if (command == 'D') {
            operationQueue.enqueue(MapOperation(MapOperation::DELETE, key));
        } else if (command == 'L') {
            operationQueue.enqueue(MapOperation(MapOperation::LOOKUP, key));
        }
    }

    // Enqueue a stop operation for each thread
    for (int i = 0; i < numThreads; ++i) {
        operationQueue.enqueue(MapOperation(MapOperation::STOP, 0));
    }

    // Wait for all threads to complete
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Destroy the mutex for outputResults
    pthread_mutex_destroy(&outputMutex);

 // Write the results to the output file
std::ofstream outputFile(outputFilePath);
if (!outputFile.is_open()) {
    std::cerr << "Error opening output file.\n";
    return 1;
}

// Write the number of threads to the output file
outputFile << "Using " << numThreads << " threads" << std::endl;

// Write the results to the output file, each on a new line
for (const auto& result : outputResults) {
    outputFile << result << std::endl; // Ensure each result is on a new line
}
    // Close the input and output files
    inputFile.close();
    outputFile.close();

    return 0;
}
