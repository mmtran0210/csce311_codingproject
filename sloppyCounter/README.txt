MINH MY TRAN - CSCE 311 - 002

# Sloppy Counter Simulator

This program simulates a "sloppy counter" using multiple threads. Each thread increments a global counter after doing a certain amount of work. The amount of work each thread does before incrementing the global counter is controlled by a "sloppiness" parameter.

Structures: I have two structures, shared and thread_data. The shared structure contains all the shared data that the threads will use, including the global counter, local counters, and various parameters. The thread_data structure is used to pass data to each thread, including a pointer to the shared data and the index of the thread.

Work Function: This is the function that each thread will run. It simulates work by either performing a CPU-bound task (a loop of increments) or an I/O-bound task (sleeping for a certain amount of time). After each iteration, it checks if it’s time to update the global counter based on the Sloppiness parameter. If the local counter reaches the Sloppiness value, it updates the global counter and resets the local counter. At the end of the function, any remaining local counter value is added to the global counter.

Thread Creation and Joining: Create a vector of threads and a corresponding vector of thread_data structures. I then create the threads with the pthread_create function, passing the work function and a pointer to the thread_data structure for each thread. After all threads are created, I join them with the pthread_join function, which waits for all threads to finish their work.

Logging: If the Do_Logging parameter is true, the Do_Logging function is called, which prints out the current settings and periodically prints out the global counter and local counters. This allows you to monitor the progress of the simulation.

Command Line Arguments: In the main function, parse the command line arguments and set the parameters in the shared structure accordingly. If an argument is not provided, it defaults to a certain value and allow skipping after the third value

## Compilation

To compile the program, use the `make` command in the terminal:

```bash
make

This will create an executable file named sloppySim

You can run the program with the following command:

./sloppySim <N_Threads> <Sloppiness> <work_time> <work_iterations> <CPU_BOUND> <Do_Logging>


<N_Threads>: 	The number of threads to use. Defaults to 2.
<Sloppiness>: 	The number of events to do before updating the global counter. Defaults to 10.
<work_time>: 	The average work time in milliseconds. Defaults to 10ms. Skipping is allowed after this argument
<work_iterations>: 	The number of iterations per thread. Defaults to 100. 
<CPU_BOUND>: 	If “true”, each thread will be CPU-bound. If “false”, each thread will be I/O-bound. Defaults to “false”.
<Do_Logging>: 	If “true”, the program will print out logging information. Defaults to “false”.

Example

Here’s an example of how to run the program:

1>	Locate to the directory

2>	 make

3>	./sloppySim 4 5 11 100 true false


Sample Output: 

root@tramtrukemtrinh:/mnt/c/Users/mimin/Desktop/311test/csce311_codingproject/sloppyCounter# ./sloppySim 4 5 11 100 false true
with simulation parameters:
        #threads = 4
        sloppiness = 5
        work time = 11
        work iterations = 100
        CPU bound = 0
        Do logging = 1
Global Ct = 40 Locals [0,0,0,0]
Global Ct = 80 Locals [2,2,2,2]
Global Ct = 120 Locals [1,1,1,1]
Global Ct = 160 Locals [1,1,1,1]
Global Ct = 200 Locals [0,0,0,0]
Global Ct = 240 Locals [1,1,1,1]
Global Ct = 280 Locals [2,2,2,2]
Global Ct = 320 Locals [2,2,2,2]
Global Ct = 360 Locals [1,1,1,1]
Global Ct = 400 Locals [0,0,0,0]
Final Global Count:400
