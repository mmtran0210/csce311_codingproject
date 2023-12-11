# Concurrent Map Data Structure

## Compile Instruction

Step 1: 

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

