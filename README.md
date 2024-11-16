
# Process Scheduling Algorithms

This project is an implementation of multiple process scheduling algorithms in C++. It provides a framework to simulate, analyze, and visualize how different scheduling techniques allocate CPU time to processes. The project supports algorithms like First Come First Serve (FCFS), Round Robin (RR), Shortest Process Next (SPN), Shortest Remaining Time (SRT), Highest Response Ratio Next (HRRN), and Feedback Queues (FB-1, FB-2i).




## Features

- Simulation of CPU Scheduling Algorithms
    - Includes major preemptive and non-preemptive scheduling techniques.
- Extensive Visualizations
    - Displays a timeline (Gantt chart-like) for each algorithm.
- Performance Statistics
    - Computes metrics such as:
        - Finish Time
        - Turnaround Time
        - Normalized Turnaround Time
        - Waiting Time
        - Averages of the above metrics for comparison.
- Dynamic Input Handling
    - Processes and algorithms can be dynamically configured through test cases or interactive input.
- Algorithms Supported
    - FCFS: First Come First Serve
    - RR: Round Robin (with configurable quantum)
    - SPN: Shortest Process Next
    - SRT: Shortest Remaining Time
    - HRRN: Highest Response Ratio Next
    - FB-1: Feedback Queue with fixed quantum
    - FB-2i: Feedback Queue with increasing quantum powers of 2.



## Algorithms


- First Come First Serve (FCFS)
    - First Come First Served (FCFS) is a scheduling algorithm in which the process that arrives first is executed first. It is a simple and easy-to-understand algorithm, but it can lead to poor performance if there are processes with long burst times. This algorithm does not have any mechanism for prioritizing processes, so it is considered a non-preemptive algorithm. In FCFS scheduling, the process that arrives first is executed first, regardless of its burst time or priority. This can lead to poor performance, as longer running processes will block shorter ones from being executed. It is commonly used in batch systems where the order of the processes is important.

- Round Robin with varying time quantum (RR)
    - Round Robin (RR) with variable quantum is a scheduling algorithm that uses a time-sharing approach to divide CPU time among processes. In this version of RR, the quantum (time slice) is not fixed and can be adjusted depending on the requirements of the processes. This allows processes with shorter burst times to be given smaller quanta and vice versa.

    - The algorithm works by maintaining a queue of processes, where each process is given a quantum of time to execute on the CPU. When a process's quantum expires, it is moved to the back of the queue, and the next process in the queue is given a quantum of time to execute.

    - The variable quantum allows the algorithm to be more efficient as it allows the CPU to spend more time on shorter processes and less time on longer ones. This can help to minimize the average waiting time for processes. Additionally, it also helps to avoid the issue of starvation, which occurs when a process with a long burst time prevents other processes from executing.

- Shortest Process Next (SPN)
    - Shortest Process Next (SPN) is a scheduling algorithm that prioritizes the execution of processes based on their burst time, or the amount of time they need to complete their task. It is a non-preemptive algorithm which means that once a process starts executing, it runs until completion or until it enters a waiting state.

    - The algorithm maintains a queue of processes, where each process is given a burst time when it arrives. The process with the shortest burst time is executed first, and as new processes arrive, they are added to the queue and sorted based on their burst time. The process with the shortest burst time will always be at the front of the queue, and thus will always be executed next.

    - This algorithm can be beneficial in situations where the objective is to minimize the average waiting time for processes, since shorter processes will be executed first, and thus will spend less time waiting in the queue. However, it can lead to longer running processes being blocked by shorter ones, which can negatively impact the overall performance of the system.

In summary, SPN is a scheduling algorithm that prioritizes the execution of processes based on their burst time, it's non-preemptive and it's commonly used in situations where the objective is to minimize the average waiting time for processes.

- Shortest Remaining Time (SRT)
    - Shortest Remaining Time Next (SRT) is a scheduling algorithm that is similar to the Shortest Process Next (SPN) algorithm, but it is a preemptive algorithm. This means that once a process starts executing, it can be interrupted by a new process with a shorter remaining time.

    - The algorithm maintains a queue of processes, where each process is given a burst time when it arrives. The process with the shortest remaining time is executed first, and as new processes arrive, they are added to the queue and sorted based on their remaining time. The process with the shortest remaining time will always be at the front of the queue, and thus will always be executed next.

    - This algorithm can be beneficial in situations where the objective is to minimize the average waiting time for processes, since shorter processes will be executed first, and thus will spend less time waiting in the queue. Additionally, it can be useful in situations where the burst time of processes is not known in advance, since the algorithm can adapt to changes in the remaining time as the process is executing.

In summary, SRT is a scheduling algorithm that prioritizes the execution of processes based on their remaining time, it's a preemptive algorithm, which means that it can interrupt a process that's already executing if a new process with a shorter remaining time arrives and it's commonly used in situations where the objective is to minimize the average waiting time for processes and burst time is not known in advance.

- Highest Response Ratio Next (HRRN)
    - Highest Response Ratio Next (HRRN) is a scheduling algorithm that prioritizes the execution of processes based on their response ratio. It is a non-preemptive algorithm which means that once a process starts executing, it runs until completion or until it enters a waiting state.

    - The response ratio is calculated by taking the ratio of the waiting time of a process and its burst time. The process with the highest response ratio is executed first, and as new processes arrive, they are added to the queue and sorted based on their response ratio. The process with the highest response ratio will always be at the front of the queue, and thus will always be executed next.

    - This algorithm can be beneficial in situations where the objective is to minimize the average waiting time for processes, since processes with longer waiting times will be executed first, and thus will spend less time waiting in the queue. Additionally, it can be useful in situations where the burst time of processes is not known in advance, since the algorithm can adapt to changes in the waiting time as the process is executing.

In summary, HRRN is a scheduling algorithm that prioritizes the execution of processes based on their response ratio, it's non-preemptive and it's commonly used in situations where the objective is to minimize the average waiting time for processes and burst time is not known in advance.

- Feedback (FB)
    - Feedback is a scheduling algorithm that allocates CPU time to different processes based on their priority level. It is a multi-level priority algorithm that uses multiple priority queues, each with a different priority level.

    - Processes with higher priority levels are executed first, and as new processes arrive, they are added to the appropriate priority queue based on their priority level. When a process completes execution, it is moved to the next lower priority queue.

    - This algorithm can be beneficial in situations where the system needs to handle a mix of short and long-running processes, as well as processes with varying priority levels. By having multiple priority queues, it ensures that processes with higher priority levels are executed first, while also allowing lower-priority processes to eventually be executed.

In summary, Feedback is a scheduling algorithm that allocates CPU time based on priority levels, it uses multiple priority queues with different levels of priority, processes with higher priority levels are executed first and when process completes execution, it is moved to the next lower priority queue, it's commonly used in situations where system needs to handle a mix of short and long-running processes, as well as processes with varying priority levels.

Feedback with varying time quantum (FBV)
Same as Feedback but with varying time quantum.
Feedback with varying time quantum also uses multiple priority queues and assigns a different time quantum for each priority level, it allows the algorithm to be more efficient by spending more time on higher-priority processes and less time on lower-priority processes.
## Installation

### Prerequisites
### C++ Compiler:
 Compatible with modern C++ standards (C++11 or later recommended).

### Terminal or IDE to compile and execute the program.


### Compilation and Execution
    
Step 1: Clone the Repository
```
https://github.com/itzpiyushjindal/Process-Scheduling-Algorithms.git
```

Step 2: Compile the Program

```
g++ -o scheduler src/main.cpp
```

Step 3: Run the Program

```
./scheduler
```




## Input Format

The program expects the following input:

### Operation Type
- trace:
    - For a detailed timeline visualization.
- stats: For statistical analysis.
### Algorithm List
Define algorithms with optional quantum (for RR or FB-2i).
Example: 1,2-4,3,5,6,7 (FCFS, RR with quantum 4, SPN, HRRN, FB-1, FB-2i).

### Processes
    - Provide a list of processes in the format:

    <Process Name>,<Arrival Time>,<Service Time>

### Example Input
```
stats
1,2-3,3
20
4
P1,0,4
P2,1,3
P3,2,1
P4,3,2
```

## Output Format

### Statistics
For statistical analysis, metrics are displayed in a tabular format:
```
FCFS
Process    | P1   | P2   | P3   | P4   |
Arrival    |  0   |  1   |  2   |  3   |
Service    |  4   |  3   |  1   |  2   |
Finish     |  4   |  7   |  8   | 10   |
Turnaround |  4   |  6   |  6   |  7   | 5.75|
NormTurn   | 1.00 | 2.00 | 6.00 | 3.50 | 3.13|
Waiting    |  0   |  3   |  5   |  5   | 3.25|
```

# Trace

A Gantt chart-like timeline is displayed for each algorithm:

```
FCFS    
0 1 2 3 4 5 6 7 
-----------------
P1      |****|    
P2           |***| 
P3              |*|
P4                |**|
-----------------

```

## Authors

- [@piyushjindal](https://github.com/itzpiyushjindal)


## 🔗 Links
[![linkedin](https://img.shields.io/badge/linkedin-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/piyush-jindal-175365259/)

## Leetcode
https://leetcode.com/u/Piyush_Jindal/




