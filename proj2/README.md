# Concurrent Map Data Structure

## Compile Instruction

Step 1: 
Assume that you have the input file, and you are already at the directory 

```
make
```

Step 2: 
```
./mapper <InputFile> <OutputFile>
```

## Approach

The implementation utilizes a hash table for storing (key, value) pairs and a thread-safe operation queue to manage map operations. The hash table is protected by an array of mutex locks to allow concurrent access by multiple threads.

### Hash Table

* The hash table can hold up to 2^22 items.
* Keys are integers, and values are C-style strings with no fixed maximum length.
* The hash function distributes keys evenly across the table to minimize collisions.

### Operation Queue 

* A separate operation queue is used to ensure the correct order of map operations.
* The queue is thread-safe, with mutex locks and condition variables to manage access.
* Careful synchronization is used to prevent race conditions and ensure the integrity of the map.

### Threading

* The program uses multiple threads to perform map operations concurrently.
* Each thread reads from the operation queue and executes the operation on the hash table.
* The number of threads is determined by the first line of the input file
>  (e.g., N 4 for 4 threads).

## Testing

### Performance
Performance testing was carried out to measure the scalability and efficiency of the map:

- **Single-threaded Performance**: With one thread, the map's output was nearly identical to the expected output, confirming its functional correctness.

- **Multi-threaded Performance**: When using multiple threads, I observed some discrepancies in the output. This indicates potential issues with thread synchronization or the ordering of operations.

### Performance Categorization

The performance can be categorized based on the number of threads used:
- **Single-threaded**: Excellent correctness with a baseline for performance comparison.
- **Multi-threaded (2-4 threads)**: Good performance with occasional synchronization issues.
- **Multi-threaded (5+ threads)**: Performance improvements plateau, indicating the need for further optimization.

### Error Analysis

To address the errors in multi-threaded performance, we:
- Reviewed the locking mechanisms to prevent race conditions.
- Ensured that the operation queue maintains the correct order of operations.
- Checked for proper handling of the `STOP` operation by all threads.

## Some Example Run and a little analysis:

### 1 THREAD RUN
for file 100k insert - 60k delete - 50k lookup, 1 thread, times take to output everything: 
real    0m35.809s
user    0m0.598s
sys     0m8.381s

for 10 - 6 - 1 - 1 thread:
real    0m0.146s
user    0m0.023s
sys     0m0.111s

### MULTIPLE THREAD RUN
for file 100k insert - 60k delete - 50k lookup, 4 thread
real    0m44.701s
user    0m1.971s
sys     0m14.579s
For this setup, starting to see error (command not working properly) at line 50.011 and all the way down
![image](https://github.com/mmtran0210/csce311_codingproject/assets/105460300/5d943798-7376-4947-9acc-36fdcea960ed)


for 10 - 6 - 1 - 5:
real    0m0.144s
user    0m0.031s
sys     0m0.103s
![image](https://github.com/mmtran0210/csce311_codingproject/assets/105460300/d64cb933-f76b-41f7-ab24-5e6fc267bff3)


