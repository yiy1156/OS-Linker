# OS-Linker
OS linker program 
This projects test out several scheduling algorithms.
Input File
• The first line in the file is the total number of
processes.
• Each process will be represented by 4 numbers:
A B C D:
– A: process ID
– B: CPU time
– C: I/O time
– Arrival time

Scheduling Algorithms
0: First-Come-First-Served (nonpreemptive)
– Queue of ready processes
– Newly arriving processes are added to the end of
the queue.
– When a process is blocked, due to I/O, and then
becomes ready, it is added to the end of the
queue.
– If two processes happen to be ready at the same
time, give preference to the one with lower ID.

1: Round-Robin with quantum 2
– Another process scheduled if one of the following
occurs:
• Current running process terminates
• Current running process is blocked on I/O
• Current running process ran for 2 cycles
– You can think of RR as a queue of ready processes.
When a process goes from running to ready, it moves
to the back of the queue.
– If two processes become Ready at the same time, give
preference to the one with smaller ID

2: Shortest remaining job first (preemptive)
– At each cycle, you calculate the remaining CPU
time for all ready/running processes and run the
one with shortest reaming time
– If several processes have the same remaining CPU
time, give preference to the process with lower ID.

Output
• Timing snapshot: at every line show:
– Cycle time
– State of each process (running, ready, or blocked)
• example: 1:blocked (i.e. process 1 is in blocked state)
• Print processes ordered by their process ID
– Be careful: do not show processes that have not yet arrived, or
those that have terminated.
• Statistics:
– Finishing time (i.e. last cycle)
– CPU utilization ( #cycles CPU was doing work / total number of
cycles)
• When there is a cycle where none of the processes is running, then
the CPU is considered idle.
– For each process:
• Turnaround time (i.e. cycle this process finished – cycle it started + 1)
