/**
 * @file sloppySim.cc
 * 
 * Minh My Tran - CSCE 311 002 - Sloppy Counter Simulator
 * Program 1
 */

#include <iostream>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <random>
#include <atomic>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using std::vector;

typedef struct __shared {
    std::atomic<int> global_counter;
    int sloppiness;
    int work_time;
    int work_iterations;
    bool cpu_bound;
    bool do_logging;
} shared;

void* work(void* arg) {
    shared* sh = (shared*) arg;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(sh->work_time*0.5, sh->work_time*1.5);

    int local_counter = 0;
    for (int i = 0; i < sh->work_iterations; ++i) {
        int random_work_time = distribution(generator);
        if (sh->cpu_bound) {
            long increments = sh->work_time * 1e6; // Total number of increments
            for (long j = 0; j < increments; ++j) {
                // Perform some work that takes approximately work_time milliseconds
            }
        } else {
            #ifdef _WIN32
        Sleep(random_work_time * 1000);
            #else
        usleep(random_work_time * 1000 * 1000); // usleep works with microseconds
    #endif

        }
        if (++local_counter % sh->sloppiness == 0) {
            sh->global_counter += sh->sloppiness;
            local_counter = 0;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    shared sh;
    sh.global_counter = 0;
    sh.sloppiness = 10;
    sh.work_time = 10;
    sh.work_iterations = 100;
    sh.cpu_bound = false;
    sh.do_logging = false;

    int n_threads = 2; // Default number of threads
    if (argc > 1) n_threads = std::stoi(argv[1]); // If provided, update the number of threads
    if (argc > 2) sh.sloppiness = std::stoi(argv[2]); // If provided, update the sloppiness
    if (argc > 3) {
        sh.work_time = std::stoi(argv[3]); // If provided, update the work time
        if (argc > 4) {
            sh.work_iterations = std::stoi(argv[4]); // If provided, update the work iterations
            if (argc > 5) {
                sh.cpu_bound = std::string(argv[5]) == "true"; // If provided, update the CPU bound
                if (argc > 6) sh.do_logging = std::string(argv[6]) == "true"; // If provided, update the logging
            }
        }
    }

    if (sh.do_logging) {
        std::cout << "N_Threads = " << n_threads << std::endl;
        std::cout << "Sloppiness = " << sh.sloppiness << std::endl;
        std::cout << "Work Time = " << sh.work_time << std::endl;
        std::cout << "Work Iterations =  " << sh.work_iterations << std::endl;
        std::cout << "CPU Bound =  " << (sh.cpu_bound ? "true" : "false") << std::endl;
        std::cout << "Do Logging =  " << (sh.do_logging ? "true" : "false") << std::endl;
    }

    vector<pthread_t> threads(n_threads);
    for (int i = 0; i < n_threads; ++i) {
        pthread_create(&threads[i], NULL, work, &sh);
    }

    for (auto& t : threads) {
        pthread_join(t, NULL);
    }

    if (sh.do_logging) {
        std::cout << "Final global counter: " << sh.global_counter << std::endl;
    }

    return 0;
}
