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

## Testing

### Performance
Performance testing was carried out to measure the scalability and efficiency of the map:

- **Single-threaded Performance**: With one thread, the map's output was nearly identical to the expected output, confirming its functional correctness.

- **Multi-threaded Performance**: When using multiple threads, I observed some discrepancies in the output. This indicates potential issues with thread synchronization or the ordering of operations.

### Error Analysis

To address the errors in multi-threaded performance, we:
- Reviewed the locking mechanisms to prevent race conditions.
- Ensured that the operation queue maintains the correct order of operations.
- Checked for proper handling of the `STOP` operation by all threads.

### Performance Categorization

The performance can be categorized based on the number of threads used:
- **Single-threaded**: Excellent correctness with a baseline for performance comparison.
- **Multi-threaded (2-4 threads)**: Good performance with occasional synchronization issues.
- **Multi-threaded (5+ threads)**: Performance improvements plateau, indicating the need for further optimization.