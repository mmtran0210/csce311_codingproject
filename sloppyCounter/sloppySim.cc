/**
 * @file sloppySim.cc
 * 
 * Minh My Tran - CSCE 311 002 - Sloppy Counter Simulator
 * Program 1
 */

#include <iostream>
#include <vector>
#include <pthread.h>
#include <random>
#include <unistd.h> // for usleep
#include <thread>
#include <mutex>

std::mutex mtx; // global mutex

typedef struct __shared {
    int global_counter = 0;
    std::vector<int> local_counters;
    int sloppiness = 10;
    int work_time = 10;
    int work_iterations= 100;
    bool cpu_bound = false;
    bool do_logging = false;
} shared;

typedef struct __thread_data {
    shared* sh;
    int thread_index;
} thread_data;

void* work(void* arg) {
    thread_data* data = (thread_data*) arg;
    shared* sh = data->sh;
    int thread_index = data->thread_index;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(sh->work_time*0.5, sh->work_time*1.5);

    for (int i = 0; i < sh->work_iterations; ++i) {
        int random_work_time = distribution(generator);
        if (sh->cpu_bound) {
            long increments = sh->work_time * 1e6; // Total number of increments
            for (long j = 0; j < increments; ++j) {
                // Perform some work here
            }
        } else {
            usleep(random_work_time * 1000); // usleep works with microseconds
        }

        // Increment the local and global counters regardless of whether cpu_bound is true or false
        if (++sh->local_counters[thread_index] == sh->sloppiness) {
            mtx.lock();
            sh->global_counter += sh->sloppiness;
            mtx.unlock();
            sh->local_counters[thread_index] = 0;
        }
    }

    if (sh->local_counters[thread_index] > 0) {
        mtx.lock();
        sh->global_counter += sh->local_counters[thread_index];
        mtx.unlock();
        sh->local_counters[thread_index] = 0;
    }

    return NULL;
}


void createThreads(std::vector<pthread_t>& threads, std::vector<thread_data>& thread_data_vec) {
    for (int i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], NULL, work, (void*)(&thread_data_vec[i]));
    }
}

void joinThreads(std::vector<pthread_t>& threads) {
    for (auto& t : threads) {
        pthread_join(t, NULL);
    }
}

void doLogging(shared& sh, int n_threads) {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sh.work_time * sh.work_iterations / 10));
        std::cout << "Global Ct = " << sh.global_counter << " Locals [";
        for (int j = 0; j < sh.local_counters.size(); ++j) {
            std::cout << sh.local_counters[j];
            if (j != sh.local_counters.size() - 1) std::cout << ",";
        }
        std::cout << "]" << std::endl;
    }

    
}

// Minh My Tran - CSCE 311 002 - Sloppy Counter Simulator
int main(int argc, char* argv[]) {
    shared sh;

    int n_threads = 2; // Default number of threads
    if (argc > 1) n_threads = std::stoi(argv[1]); //number of threads
    if (argc > 2) sh.sloppiness = std::stoi(argv[2]); // sloppiness
    if (argc > 3) {
        sh.work_time = std::stoi(argv[3]); // work time
        if (argc > 4) {
            sh.work_iterations = std::stoi(argv[4]); // work iterations
            if (argc > 5) {
                sh.cpu_bound = std::string(argv[5]) == "true"; // CPU bound
                if (argc > 6) sh.do_logging = std::string(argv[6]) == "true"; // logging
            }
        }
    }

    // Print settings
    std::cout << "with simulation parameters:\n";
    std::cout << "\t# of Threads = " << n_threads << "\n";
    std::cout << "\tSloppiness = " << sh.sloppiness << "\n";
    std::cout << "\tWork Time = " << sh.work_time << "\n";
    std::cout << "\tWork Iterations = " << sh.work_iterations << "\n";
    std::cout << "\tCPU Bound = " << (sh.cpu_bound ? 1 : 0) << "\n";
    std::cout << "\tDo Logging = " << (sh.do_logging ? 1 : 0) << "\n";

    sh.local_counters.resize(n_threads, 0);

    std::vector<pthread_t> threads(n_threads);
    std::vector<thread_data> thread_data_vec(n_threads);
    for (int i = 0; i < n_threads; ++i) {
        thread_data_vec[i].sh = &sh;
        thread_data_vec[i].thread_index = i;
    }

    createThreads(threads, thread_data_vec);

    if (sh.do_logging) {
        doLogging(sh, n_threads);
    }

    joinThreads(threads);

    // Print the final global count
    std::cout << "Final Global Count:" << sh.global_counter << "\n";

    return 0;
}
