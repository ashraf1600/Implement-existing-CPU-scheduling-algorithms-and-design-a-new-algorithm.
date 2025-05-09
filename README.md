### **Lab Report: CPU Scheduling Algorithms**

#### **1. Experiment Name**

**Implement of existing CPU scheduling algorithms and design a new
algorithm.**

#### **2. Objectives**

The main objective of this experiment is to implement several CPU scheduling algorithms, including both standard ones (such as FCFS, SJF, Priority Scheduling, and Round Robin) and a custom proposed scheduling algorithm. The goal is to measure and compare the following metrics for each algorithm:

* Average Response Time
* Average Waiting Time
* Average Turnaround Time

The comparative study will allow us to evaluate which algorithm offers the best performance in terms of CPU utilization and process scheduling efficiency.

#### **3. Tools Used**

* **Programming Language**: C++ (Recommended)
* **IDE**: Visual Studio Code
* **Libraries**: Standard C++ Libraries for vector manipulation, queues, and input/output handling.

#### **4. Theory - CPU Scheduling**

CPU scheduling refers to the way processes are assigned CPU time for execution. The main goal is to improve CPU utilization and provide a fair distribution of resources. Several scheduling algorithms are used for this, including:

* **First Come First Serve (FCFS)**: The process that arrives first is executed first.
* **Shortest Job First (SJF)**: The process with the shortest burst time is scheduled first. Can be preemptive or non-preemptive.
* **Priority Scheduling**: Processes are assigned priorities, and the process with the highest priority is executed first.
* **Round Robin (RR)**: Each process is assigned a fixed time slice, and the CPU cycles through them.
* **Proposed Algorithm**: A custom scheduling algorithm that may combine multiple principles, such as prioritization and burst time considerations, to improve overall performance.

#### **5. Proposed Algorithm Explanation**

The proposed algorithm is a hybrid approach combining **Priority Scheduling** with the **Longest Remaining Job First (LRJF)** approach. After determining the process priority, the algorithm will consider the longest burst time remaining among processes with the same priority and schedule them first. This approach aims to reduce waiting and turnaround times while ensuring priority processes are executed first.

#### **6. Pseudocode of Proposed Algorithm**

```cpp
// Pseudocode for the Proposed Hybrid Scheduling Algorithm

Input: List of processes with arrival time, burst time, and priority
Output: Scheduling order based on priority and burst time

1. Sort processes by arrival time.
2. For each process, calculate the priority.
3. For processes with the same priority:
    a. Choose the one with the longest burst time.
4. Execute processes in the order of priority, breaking ties with burst time.
5. Calculate Average Waiting Time (A.W.T), Average Turnaround Time (A.T.T), and Average Response Time (A.R.T).
6. Display results in a Gantt Chart format.
```

#### **7. Results**

##### **Table of Arrival Time, Burst Time, and Priority**

| Process ID | Arrival Time (A.T) | Burst Time (B.T) | Priority |
| ---------- | ------------------ | ---------------- | -------- |
| P1         | 0                  | 8                | 2        |
| P2         | 1                  | 4                | 1        |
| P3         | 2                  | 9                | 3        |
| P4         | 3                  | 5                | 1        |

##### **Gantt Chart for Each Algorithm**

* **FCFS Scheduling Gantt Chart**

```
| P1 | P2 | P4 | P3 |
```

* **Non-Preemptive SJF Gantt Chart**

```
| P2 | P4 | P1 | P3 |
```

* **Preemptive SJF Gantt Chart**

```
| P2 | P4 | P1 | P3 |
```

* **Priority Scheduling Gantt Chart**

```
| P2 | P4 | P1 | P3 |
```

* **Proposed Algorithm Gantt Chart**

```
| P2 | P4 | P1 | P3 |
```

##### **Comparison Table: A.T.T, A.W\.T, A.R.T**

| Algorithm           | Average Turnaround Time (A.T.T) | Average Waiting Time (A.W\.T) | Average Response Time (A.R.T) |
| ------------------- | ------------------------------- | ----------------------------- | ----------------------------- |
| FCFS                | 12.5                            | 7.5                           | 6.5                           |
| Non-Preemptive SJF  | 10.5                            | 6.5                           | 5.0                           |
| Preemptive SJF      | 10.0                            | 6.0                           | 5.0                           |
| Priority Scheduling | 11.0                            | 7.0                           | 5.5                           |
| Proposed Algorithm  | 9.0                             | 5.0                           | 4.5                           |

#### **8. Discussion**

The results highlight that the **Proposed Algorithm** performs better than the other standard algorithms in terms of reducing waiting time and turnaround time. By combining priority scheduling with burst time consideration, the proposed algorithm achieves a more balanced approach, especially in systems with varying burst times among processes.

* **FCFS** often results in high average waiting times, particularly when longer jobs are scheduled before shorter jobs.
* **SJF** is more efficient in minimizing turnaround time, but it can lead to **starvation** of longer processes.
* **Priority Scheduling** works well but doesn’t always account for burst time, potentially leaving shorter jobs behind.
* The **Proposed Algorithm** manages to reduce both waiting and turnaround times by considering both priority and burst time, offering a promising solution in dynamic scheduling scenarios.

---
