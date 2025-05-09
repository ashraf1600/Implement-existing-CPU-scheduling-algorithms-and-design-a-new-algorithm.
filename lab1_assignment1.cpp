#include<bits/stdc++.h>
using namespace std;

// Process structure to store all process details
struct Process {
    int id;                 // Process ID
    int burstTime;          // Burst time
    int arrivalTime;        // Arrival time
    int priority;           // Priority (lower value means higher priority)
    int remainingTime;      // Remaining burst time
    int startTime;          // First time CPU is allocated
    int completionTime;     // Time when process completes
    int responseTime;       // Time from arrival to first CPU allocation
    int waitingTime;        // Time spent waiting
    int turnaroundTime;     // Time from arrival to completion
    bool started;           // Flag to check if process has started execution

};

// Function to display the Gantt chart

void displayGanttChart(const vector<pair<int, int>>& ganttChart, const vector<Process>& processes) {
    cout << "\nGantt Chart: ";
    int totalTime = 0;
    for (const auto& entry : ganttChart) {
        totalTime += entry.second;
        if (entry.first == -1) {
            cout << "| IDLE(" << entry.second << ") ";
        } else {
            cout << "| P" << entry.first + 1 << "(" << totalTime << ") ";
        }
    }
    cout << "|\n\n";
}

// Function to display process details and metrics
void displayProcessDetails(const vector<Process>& processes) {
    cout << "\n+----+------------+-------------+---------------+-----------------+------------------+--------------------+\n";
    cout << "| PID  | Burst Time | Arrival Time| Response Time |  Waiting Time   | Turnaround Time| Completion Time    |\n";
    cout << "+------+------------+-------------+---------------+-----------------+------------------+--------------------+\n";

    for (const auto& p : processes) {
        cout << "| P" << p.id + 1 << "   | " << setw(10) << p.burstTime << " | "
             << setw(11) << p.arrivalTime << " | " << setw(12) << p.responseTime << " | "
             << setw(15) << p.waitingTime << " | " << setw(16) << p.turnaroundTime << " | "
             << setw(18) << p.completionTime << " |\n";
    }

    cout << "+------+------------+-------------+--------------+-----------------+------------------+--------------------+\n";
}

// Function to calculate and display average metrics
tuple<double, double, double> displayAverageMetrics(const vector<Process>& processes) {
    double avgResponseTime = 0, avgWaitingTime = 0, avgTurnaroundTime = 0;

    for (const auto& p : processes) {
        avgResponseTime += p.responseTime;
        avgWaitingTime += p.waitingTime;
        avgTurnaroundTime += p.turnaroundTime;
    }

    int n = processes.size();
    if (n > 0) {
        avgResponseTime /= n;
        avgWaitingTime /= n;

        avgTurnaroundTime /= n;
    }

    cout << "\nAverage Response Time: " << avgResponseTime << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << avgTurnaroundTime << endl;

    return make_tuple(avgResponseTime, avgWaitingTime, avgTurnaroundTime);
}

// Function to get process input from user
vector<Process> getProcessInput(bool needsPriority) {
    int n;
    cout << "Number of processes, n = : ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i;

        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> processes[i].burstTime;

        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> processes[i].arrivalTime;

        if (needsPriority) {
            cout << "Enter the priority of P" << i + 1 << " (lower value means higher priority): ";
            cin >> processes[i].priority;
        } else {
            processes[i].priority = 0; // Default priority
        }

        processes[i].remainingTime = processes[i].burstTime;
        processes[i].started = false;
    }

    return processes;
}

// First Come First Serve (FCFS) Scheduling Algorithm
//---------------------------------------------------------------

vector<Process> fcfs(vector<Process> processes) {
    int n = processes.size();

    // Sort processes based on arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    vector<pair<int, int>> ganttChart;
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        // If there's a gap between processes, add idle time to Gantt chart
        if (currentTime < processes[i].arrivalTime) {
            ganttChart.push_back({-1, processes[i].arrivalTime - currentTime});
            currentTime = processes[i].arrivalTime;
        }

        // Set start time if process hasn't started yet
        if (!processes[i].started) {
            processes[i].startTime = currentTime;
            processes[i].started = true;
        }

        // Add process to Gantt chart
        ganttChart.push_back({processes[i].id, processes[i].burstTime});

        // Update current time
        currentTime += processes[i].burstTime;

        // Calculate completion time
        processes[i].completionTime = currentTime;

        // Calculate response time
        processes[i].responseTime = processes[i].startTime - processes[i].arrivalTime;

        // Calculate turnaround time
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;

        // Calculate waiting time
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}

// Non-Preemptive Shortest Job First (SJF) Scheduling Algorithm
vector<Process> nonPreemptiveSJF(vector<Process> processes) {
    int n = processes.size();
    vector<pair<int, int>> ganttChart;

    // Initialize remaining time
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    int completed = 0;
    int currentTime = 0;

    while (completed < n) {
        int shortestJobIdx = -1;
        int shortestBurst = numeric_limits<int>::max();

        // Find the process with the shortest burst time that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].burstTime < shortestBurst) {
                    shortestBurst = processes[i].burstTime;
                    shortestJobIdx = i;
                }
            }
        }

        // If no process is found, advance time
        if (shortestJobIdx == -1) {
            // Find the next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime > currentTime && processes[i].remainingTime > 0) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }

            // Add idle time to Gantt chart
            if (nextArrival != numeric_limits<int>::max()) {
                ganttChart.push_back({-1, nextArrival - currentTime});
                currentTime = nextArrival;
            } else {
                break; // No more processes to execute
            }
        } else {
            // Set start time if process hasn't started yet
            if (!processes[shortestJobIdx].started) {
                processes[shortestJobIdx].startTime = currentTime;
                processes[shortestJobIdx].started = true;
            }

            // Add process to Gantt chart
            ganttChart.push_back({processes[shortestJobIdx].id, processes[shortestJobIdx].burstTime});

            // Update current time
            currentTime += processes[shortestJobIdx].burstTime;

            // Mark process as completed
            processes[shortestJobIdx].remainingTime = 0;
            processes[shortestJobIdx].completionTime = currentTime;

            // Calculate response time
            processes[shortestJobIdx].responseTime = processes[shortestJobIdx].startTime - processes[shortestJobIdx].arrivalTime;

            // Calculate turnaround time
            processes[shortestJobIdx].turnaroundTime = processes[shortestJobIdx].completionTime - processes[shortestJobIdx].arrivalTime;

            // Calculate waiting time
            processes[shortestJobIdx].waitingTime = processes[shortestJobIdx].turnaroundTime - processes[shortestJobIdx].burstTime;

            completed++;
        }
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}

// Preemptive Shortest Job First (SJF) Scheduling Algorithm
vector<Process> preemptiveSJF(vector<Process> processes) {
    int n = processes.size();
    vector<pair<int, int>> ganttChart;

    // Initialize remaining time
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    int completed = 0;
    int currentTime = 0;
    int prevProcess = -1;
    int timeQuantum = 1; // For Gantt chart visualization

    while (completed < n) {
        int shortestJobIdx = -1;
        int shortestRemaining = numeric_limits<int>::max();

        // Find the process with the shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].remainingTime < shortestRemaining) {
                    shortestRemaining = processes[i].remainingTime;
                    shortestJobIdx = i;
                }
            }
        }

        // If no process is found, advance time
        if (shortestJobIdx == -1) {
            // Find the next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime > currentTime && processes[i].remainingTime > 0) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }

            // Add idle time to Gantt chart
            if (nextArrival != numeric_limits<int>::max()) {
                if (prevProcess != -1) {
                    ganttChart.push_back({prevProcess, timeQuantum});
                    prevProcess = -1;
                    timeQuantum = 0;
                }
                ganttChart.push_back({-1, nextArrival - currentTime});
                currentTime = nextArrival;
            } else {
                break; // No more processes to execute
            }
        } else {
            // Set start time if process hasn't started yet
            if (!processes[shortestJobIdx].started) {
                processes[shortestJobIdx].startTime = currentTime;
                processes[shortestJobIdx].started = true;
            }

            // Update Gantt chart
            if (prevProcess != shortestJobIdx) {
                if (prevProcess != -1) {
                    ganttChart.push_back({prevProcess, timeQuantum});
                }
                prevProcess = shortestJobIdx;
                timeQuantum = 1;
            } else {
                timeQuantum++;
            }

            // Decrement remaining time
            processes[shortestJobIdx].remainingTime--;
            currentTime++;

            // Check if process is completed
            if (processes[shortestJobIdx].remainingTime == 0) {
                processes[shortestJobIdx].completionTime = currentTime;

                // Calculate response time
                processes[shortestJobIdx].responseTime = processes[shortestJobIdx].startTime - processes[shortestJobIdx].arrivalTime;

                // Calculate turnaround time
                processes[shortestJobIdx].turnaroundTime = processes[shortestJobIdx].completionTime - processes[shortestJobIdx].arrivalTime;

                // Calculate waiting time
                processes[shortestJobIdx].waitingTime = processes[shortestJobIdx].turnaroundTime - processes[shortestJobIdx].burstTime;

                completed++;
            }
        }
    }

    // Add the last process to Gantt chart
    if (prevProcess != -1) {
        ganttChart.push_back({prevProcess, timeQuantum});
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}

// Non-Preemptive Priority Scheduling Algorithm
vector<Process> nonPreemptivePriority(vector<Process> processes) {
    int n = processes.size();
    vector<pair<int, int>> ganttChart;

    // Initialize remaining time
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    int completed = 0;
    int currentTime = 0;

    while (completed < n) {
        int highestPriorityIdx = -1;
        int highestPriority = numeric_limits<int>::max();

        // Find the process with the highest priority that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    highestPriorityIdx = i;
                }
            }
        }

        // If no process is found, advance time
        if (highestPriorityIdx == -1) {
            // Find the next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime > currentTime && processes[i].remainingTime > 0) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }

            // Add idle time to Gantt chart
            if (nextArrival != numeric_limits<int>::max()) {
                ganttChart.push_back({-1, nextArrival - currentTime});
                currentTime = nextArrival;
            } else {
                break; // No more processes to execute
            }
        } else {
            // Set start time if process hasn't started yet
            if (!processes[highestPriorityIdx].started) {
                processes[highestPriorityIdx].startTime = currentTime;
                processes[highestPriorityIdx].started = true;
            }

            // Add process to Gantt chart
            ganttChart.push_back({processes[highestPriorityIdx].id, processes[highestPriorityIdx].burstTime});

            // Update current time
            currentTime += processes[highestPriorityIdx].burstTime;

            // Mark process as completed
            processes[highestPriorityIdx].remainingTime = 0;
            processes[highestPriorityIdx].completionTime = currentTime;

            // Calculate response time
            processes[highestPriorityIdx].responseTime = processes[highestPriorityIdx].startTime - processes[highestPriorityIdx].arrivalTime;

            // Calculate turnaround time
            processes[highestPriorityIdx].turnaroundTime = processes[highestPriorityIdx].completionTime - processes[highestPriorityIdx].arrivalTime;

            // Calculate waiting time
            processes[highestPriorityIdx].waitingTime = processes[highestPriorityIdx].turnaroundTime - processes[highestPriorityIdx].burstTime;

            completed++;
        }
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}

// Preemptive Priority Scheduling Algorithm
vector<Process> preemptivePriority(vector<Process> processes) {
    int n = processes.size();
    vector<pair<int, int>> ganttChart;

    // Initialize remaining time
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    int completed = 0;
    int currentTime = 0;
    int prevProcess = -1;
    int timeQuantum = 1; // For Gantt chart visualization

    while (completed < n) {
        int highestPriorityIdx = -1;
        int highestPriority = numeric_limits<int>::max();

        // Find the process with the highest priority that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    highestPriorityIdx = i;
                }
            }
        }

        // If no process is found, advance time
        if (highestPriorityIdx == -1) {
            // Find the next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime > currentTime && processes[i].remainingTime > 0) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }

            // Add idle time to Gantt chart
            if (nextArrival != numeric_limits<int>::max()) {
                if (prevProcess != -1) {
                    ganttChart.push_back({prevProcess, timeQuantum});
                    prevProcess = -1;
                    timeQuantum = 0;
                }
                ganttChart.push_back({-1, nextArrival - currentTime});
                currentTime = nextArrival;
            } else {
                break; // No more processes to execute
            }
        } else {
            // Set start time if process hasn't started yet
            if (!processes[highestPriorityIdx].started) {
                processes[highestPriorityIdx].startTime = currentTime;
                processes[highestPriorityIdx].started = true;
            }

            // Update Gantt chart
            if (prevProcess != highestPriorityIdx) {
                if (prevProcess != -1) {
                    ganttChart.push_back({prevProcess, timeQuantum});
                }
                prevProcess = highestPriorityIdx;
                timeQuantum = 1;
            } else {
                timeQuantum++;
            }

            // Decrement remaining time
            processes[highestPriorityIdx].remainingTime--;
            currentTime++;

            // Check if process is completed
            if (processes[highestPriorityIdx].remainingTime == 0) {
                processes[highestPriorityIdx].completionTime = currentTime;

                // Calculate response time
                processes[highestPriorityIdx].responseTime = processes[highestPriorityIdx].startTime - processes[highestPriorityIdx].arrivalTime;

                // Calculate turnaround time
                processes[highestPriorityIdx].turnaroundTime = processes[highestPriorityIdx].completionTime - processes[highestPriorityIdx].arrivalTime;

                // Calculate waiting time
                processes[highestPriorityIdx].waitingTime = processes[highestPriorityIdx].turnaroundTime - processes[highestPriorityIdx].burstTime;

                completed++;
            }
        }
    }

    // Add the last process to Gantt chart
    if (prevProcess != -1) {
        ganttChart.push_back({prevProcess, timeQuantum});
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}

// Round Robin Scheduling Algorithm
vector<Process> roundRobin(vector<Process> processes) {
    int n = processes.size();
    vector<pair<int, int>> ganttChart;

    // Get time quantum
    int timeQuantum;
    cout << "Time Quantum: ";
    cin >> timeQuantum;

    // Initialize remaining time
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    // Sort processes based on arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int completed = 0;
    int currentTime = 0;
    queue<int> readyQueue;
    int idx = 0;

    // Add first process to ready queue
    while (idx < n && processes[idx].arrivalTime <= currentTime) {
        readyQueue.push(idx);
        idx++;
    }

    while (completed < n) {
        // If ready queue is empty, advance time
        if (readyQueue.empty()) {
            // Find the next arriving process
            if (idx < n) {
                ganttChart.push_back({-1, processes[idx].arrivalTime - currentTime});
                currentTime = processes[idx].arrivalTime;

                // Add all processes that have arrived
                while (idx < n && processes[idx].arrivalTime <= currentTime) {
                    readyQueue.push(idx);
                    idx++;
                }
            } else {
                break; // No more processes to execute
            }
        } else {
            // Get the next process from ready queue
            int processIdx = readyQueue.front();
            readyQueue.pop();

            // Set start time if process hasn't started yet
            if (!processes[processIdx].started) {
                processes[processIdx].startTime = currentTime;
                processes[processIdx].started = true;
            }

            // Calculate execution time for this time slice
            int executionTime = min(timeQuantum, processes[processIdx].remainingTime);

            // Add process to Gantt chart
            ganttChart.push_back({processes[processIdx].id, executionTime});

            // Update current time
            currentTime += executionTime;

            // Decrement remaining time
            processes[processIdx].remainingTime -= executionTime;

            // Add newly arrived processes to ready queue
            while (idx < n && processes[idx].arrivalTime <= currentTime) {
                readyQueue.push(idx);
                idx++;
            }

            // If process is not completed, add it back to ready queue
            if (processes[processIdx].remainingTime > 0) {
                readyQueue.push(processIdx);
            } else {
                // Mark process as completed
                processes[processIdx].completionTime = currentTime;

                // Calculate response time
                processes[processIdx].responseTime = processes[processIdx].startTime - processes[processIdx].arrivalTime;

                // Calculate turnaround time
                processes[processIdx].turnaroundTime = processes[processIdx].completionTime - processes[processIdx].arrivalTime;

                // Calculate waiting time
                processes[processIdx].waitingTime = processes[processIdx].turnaroundTime - processes[processIdx].burstTime;

                completed++;
            }
        }
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}

// we know in priority scheduling after checking priority we use fcfs ( first come first service method) method to find answer.
//  but now you have to implement considering shortest burst time is scheduled to execute first which is 
//  Longest Remaining Job First method after checking priority. In a word , you should consider two things in 
//  this algorithm priority at first and then based on priority longest burst time is scheduled to execute first.

// NEW ALGO.

vector<Process> Own_algo(vector<Process> processes) {
    int n = processes.size();
    vector<pair<int, int>> ganttChart;

    // Initialize remaining time
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    int completed = 0;
    int currentTime = 0;

    while (completed < n) {
        int bestProcessIdx = -1;
        int maxRemainingTime = -1;

        // Find the process with the highest priority and longest remaining burst time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                // Check if this process has higher priority or same priority but shortest burst time
                if (bestProcessIdx == -1 || processes[i].priority < processes[bestProcessIdx].priority ||
                    (processes[i].priority == processes[bestProcessIdx].priority && processes[i].remainingTime < maxRemainingTime)) {
                    maxRemainingTime = processes[i].remainingTime;
                    bestProcessIdx = i;
                }
            }
        }

        // If no process is found, advance time
        if (bestProcessIdx == -1) {
            // Find the next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime > currentTime && processes[i].remainingTime > 0) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }

            // Add idle time to Gantt chart
            if (nextArrival != numeric_limits<int>::max()) {
                ganttChart.push_back({-1, nextArrival - currentTime});
                currentTime = nextArrival;
            } else {
                break; // No more processes to execute
            }
        } else {
            // Set start time if process hasn't started yet
            if (!processes[bestProcessIdx].started) {
                processes[bestProcessIdx].startTime = currentTime;
                processes[bestProcessIdx].started = true;
            }

            // Add process to Gantt chart
            ganttChart.push_back({processes[bestProcessIdx].id, processes[bestProcessIdx].burstTime});

            // Update current time
            currentTime += processes[bestProcessIdx].remainingTime;

            // Mark process as completed
            processes[bestProcessIdx].remainingTime = 0;
            processes[bestProcessIdx].completionTime = currentTime;

            // Calculate response time
            processes[bestProcessIdx].responseTime = processes[bestProcessIdx].startTime - processes[bestProcessIdx].arrivalTime;

            // Calculate turnaround time
            processes[bestProcessIdx].turnaroundTime = processes[bestProcessIdx].completionTime - processes[bestProcessIdx].arrivalTime;

            // Calculate waiting time
            processes[bestProcessIdx].waitingTime = processes[bestProcessIdx].turnaroundTime - processes[bestProcessIdx].burstTime;

            completed++;
        }
    }

    // Display Gantt chart
    displayGanttChart(ganttChart, processes);

    // Sort processes by ID for display
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    return processes;
}



// Function to compare all algorithms
void compareAllAlgorithms() {
    // Get process input
    vector<Process> processes = getProcessInput(true);

    // Make copies of the processes for each algorithm
    vector<Process> fcfsProcesses = processes;
    vector<Process> nonPreemptiveSJFProcesses = processes;
    vector<Process> preemptiveSJFProcesses = processes;
    vector<Process> nonPreemptivePriorityProcesses = processes;
    vector<Process> preemptivePriorityProcesses = processes;
    vector<Process> roundRobinProcesses = processes;
    vector<Process> own_algo = processes;

    // Get time quantum for Round Robin
    int timeQuantum;
    cout << "Time Quantum for Round Robin: ";
    cin >> timeQuantum;

    // Run all algorithms
    cout << "\n\n========== FCFS Scheduling ==========\n";
    fcfsProcesses = fcfs(fcfsProcesses);
    double fcfsResponseTime = 0, fcfsWaitingTime = 0, fcfsTurnaroundTime = 0;
    for (const auto& p : fcfsProcesses) {
        fcfsResponseTime += p.responseTime;
        fcfsWaitingTime += p.waitingTime;
        fcfsTurnaroundTime += p.turnaroundTime;
    }
    fcfsResponseTime /= fcfsProcesses.size();
    fcfsWaitingTime /= fcfsProcesses.size();
    fcfsTurnaroundTime /= fcfsProcesses.size();

    cout << "\n\n========== Non-Preemptive SJF Scheduling ==========\n";
    nonPreemptiveSJFProcesses = nonPreemptiveSJF(nonPreemptiveSJFProcesses);
    double npsjfResponseTime = 0, npsjfWaitingTime = 0, npsjfTurnaroundTime = 0;
    for (const auto& p : nonPreemptiveSJFProcesses) {
        npsjfResponseTime += p.responseTime;
        npsjfWaitingTime += p.waitingTime;
        npsjfTurnaroundTime += p.turnaroundTime;
    }
    npsjfResponseTime /= nonPreemptiveSJFProcesses.size();
    npsjfWaitingTime /= nonPreemptiveSJFProcesses.size();
    npsjfTurnaroundTime /= nonPreemptiveSJFProcesses.size();

    cout << "\n\n========== Preemptive SJF Scheduling ==========\n";
    preemptiveSJFProcesses = preemptiveSJF(preemptiveSJFProcesses);
    double psjfResponseTime = 0, psjfWaitingTime = 0, psjfTurnaroundTime = 0;
    for (const auto& p : preemptiveSJFProcesses) {
        psjfResponseTime += p.responseTime;
        psjfWaitingTime += p.waitingTime;
        psjfTurnaroundTime += p.turnaroundTime;
    }
    psjfResponseTime /= preemptiveSJFProcesses.size();
    psjfWaitingTime /= preemptiveSJFProcesses.size();
    psjfTurnaroundTime /= preemptiveSJFProcesses.size();

    cout << "\n\n========== Non-Preemptive Priority Scheduling ==========\n";
    nonPreemptivePriorityProcesses = nonPreemptivePriority(nonPreemptivePriorityProcesses);
    double nppResponseTime = 0, nppWaitingTime = 0, nppTurnaroundTime = 0;
    for (const auto& p : nonPreemptivePriorityProcesses) {
        nppResponseTime += p.responseTime;
        nppWaitingTime += p.waitingTime;
        nppTurnaroundTime += p.turnaroundTime;
    }
    nppResponseTime /= nonPreemptivePriorityProcesses.size();
    nppWaitingTime /= nonPreemptivePriorityProcesses.size();
    nppTurnaroundTime /= nonPreemptivePriorityProcesses.size();

    cout << "\n\n========== Preemptive Priority Scheduling ==========\n";
    preemptivePriorityProcesses = preemptivePriority(preemptivePriorityProcesses);
    double ppResponseTime = 0, ppWaitingTime = 0, ppTurnaroundTime = 0;
    for (const auto& p : preemptivePriorityProcesses) {
        ppResponseTime += p.responseTime;
        ppWaitingTime += p.waitingTime;
        ppTurnaroundTime += p.turnaroundTime;
    }
    ppResponseTime /= preemptivePriorityProcesses.size();
    ppWaitingTime /= preemptivePriorityProcesses.size();
    ppTurnaroundTime /= preemptivePriorityProcesses.size();

    cout << "\n\n========== Round Robin Scheduling ==========\n";
    // Simulate Round Robin with the given time quantum
    int completed = 0;
    int currentTime = 0;
    queue<int> readyQueue;
    int idx = 0;

    // Sort processes based on arrival time
    sort(roundRobinProcesses.begin(), roundRobinProcesses.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    // Initialize remaining time
    for (auto& p : roundRobinProcesses) {
        p.remainingTime = p.burstTime;
        p.started = false;
    }

    // Add first process to ready queue
    while (idx < roundRobinProcesses.size() && roundRobinProcesses[idx].arrivalTime <= currentTime) {
        readyQueue.push(idx);
        idx++;
    }

    vector<pair<int, int>> rrGanttChart;

    while (completed < roundRobinProcesses.size()) {
        // If ready queue is empty, advance time
        if (readyQueue.empty()) {
            // Find the next arriving process
            if (idx < roundRobinProcesses.size()) {
                rrGanttChart.push_back({-1, roundRobinProcesses[idx].arrivalTime - currentTime});
                currentTime = roundRobinProcesses[idx].arrivalTime;

                // Add all processes that have arrived
                while (idx < roundRobinProcesses.size() && roundRobinProcesses[idx].arrivalTime <= currentTime) {
                    readyQueue.push(idx);
                    idx++;
                }
            } else {
                break; // No more processes to execute
            }
        } else {
            // Get the next process from ready queue
            int processIdx = readyQueue.front();
            readyQueue.pop();

            // Set start time if process hasn't started yet
            if (!roundRobinProcesses[processIdx].started) {
                roundRobinProcesses[processIdx].startTime = currentTime;
                roundRobinProcesses[processIdx].started = true;
            }

            // Calculate execution time for this time slice
            int executionTime = min(timeQuantum, roundRobinProcesses[processIdx].remainingTime);

            // Add process to Gantt chart
            rrGanttChart.push_back({roundRobinProcesses[processIdx].id, executionTime});

            // Update current time
            currentTime += executionTime;

            // Decrement remaining time
            roundRobinProcesses[processIdx].remainingTime -= executionTime;

            // Add newly arrived processes to ready queue
            while (idx < roundRobinProcesses.size() && roundRobinProcesses[idx].arrivalTime <= currentTime) {
                readyQueue.push(idx);
                idx++;
            }

            // If process is not completed, add it back to ready queue
            if (roundRobinProcesses[processIdx].remainingTime > 0) {
                readyQueue.push(processIdx);
            } else {
                // Mark process as completed
                roundRobinProcesses[processIdx].completionTime = currentTime;

                // Calculate response time
                roundRobinProcesses[processIdx].responseTime = roundRobinProcesses[processIdx].startTime - roundRobinProcesses[processIdx].arrivalTime;

                // Calculate turnaround time
                roundRobinProcesses[processIdx].turnaroundTime = roundRobinProcesses[processIdx].completionTime - roundRobinProcesses[processIdx].arrivalTime;

                // Calculate waiting time
                roundRobinProcesses[processIdx].waitingTime = roundRobinProcesses[processIdx].turnaroundTime - roundRobinProcesses[processIdx].burstTime;

                completed++;
            }
        }
    }

    // Display Gantt chart
    displayGanttChart(rrGanttChart, roundRobinProcesses);

    // Sort processes by ID for display
    sort(roundRobinProcesses.begin(), roundRobinProcesses.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    double rrResponseTime = 0, rrWaitingTime = 0, rrTurnaroundTime = 0;
    for (const auto& p : roundRobinProcesses) {
        rrResponseTime += p.responseTime;
        rrWaitingTime += p.waitingTime;
        rrTurnaroundTime += p.turnaroundTime;
    }
    rrResponseTime /= roundRobinProcesses.size();
    rrWaitingTime /= roundRobinProcesses.size();
    rrTurnaroundTime /= roundRobinProcesses.size();

    cout << "\n\n========== Own_Algorithm ==========\n";
    own_algo= Own_algo(own_algo);
    double ownResponseTime = 0, ownWaitingTime = 0, ownTurnaroundTime = 0;
    for (const auto& p : own_algo) {
        ownResponseTime += p.responseTime;
        ownWaitingTime += p.waitingTime;
        ownTurnaroundTime += p.turnaroundTime;
    }
    ownResponseTime /= own_algo.size();
    ownWaitingTime /= own_algo.size();
    ownTurnaroundTime /= own_algo.size();


    

    // Display comparison results
    cout << "\n\n========== Comparison of All Algorithms ==========\n";
    cout << "+-------------------------+------------------+------------------+---------------------+\n";
    cout << "| Algorithm               | Avg Response T.  | Avg Waiting T.   | Avg Turnaround T.   |\n";
    cout << "+-------------------------+------------------+------------------+---------------------+\n";
    cout << "| FCFS                    | " << setw(16) << fcfsResponseTime << " | " << setw(16) << fcfsWaitingTime << " | " << setw(19) << fcfsTurnaroundTime << " |\n";
    cout << "| Non-Preemptive-SJF      | " << setw(16) << npsjfResponseTime << " | " << setw(16) << npsjfWaitingTime << " | " << setw(19) << npsjfTurnaroundTime << " |\n";
    cout << "| Preemptive-SJF          | " << setw(16) << psjfResponseTime << " | " << setw(16) << psjfWaitingTime << " | " << setw(19) << psjfTurnaroundTime << " |\n";
    cout << "| Non-Preemptive-Priority | " << setw(16) << nppResponseTime << " | " << setw(16) << nppWaitingTime << " | " << setw(19) << nppTurnaroundTime << " |\n";
    cout << "| Preemptive-Priority     | " << setw(16) << ppResponseTime << " | " << setw(16) << ppWaitingTime << " | " << setw(19) << ppTurnaroundTime << " |\n";
    cout << "| Round-Robin             | " << setw(16) << rrResponseTime << " | " << setw(16) << rrWaitingTime << " | " << setw(19) << rrTurnaroundTime << " |\n";
    cout << "| Own_Algo  | " << setw(16) << ownResponseTime << " | " << setw(16) << ownWaitingTime<< " | " << setw(19) << ownTurnaroundTime<< " |\n";
    cout << "+-------------------------+------------------+------------------+---------------------+\n";
}





//--------------------------------------------------------------------
// Main function to run the CPU scheduling algorithms and compare them
//--------------------------------------------------------------------




















int main() {
    int choice;

    while (true) {
        cout << "\n\n========== CPU Scheduling Algorithms ==========\n";
        cout << "1: FCFS\n";
        cout << "2: Non-Preemptive-SJF\n";
        cout << "3: Preemptive-SJF\n";
        cout << "4: Non-Preemptive-Priority\n";
        cout << "5: Preemptive-Priority\n";
        cout << "6: Round-Robin\n";
        cout << "7: Own_Algo\n";
        cout << "8: Compare-All\n";
        cout << "9: Exit\n";
        cout << "Enter your Choice: ";
        cin >> choice;

        vector<Process> processes;

        switch (choice) {
            case 1:
                processes = getProcessInput(false);
                processes = fcfs(processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 2:
                processes = getProcessInput(false);
                processes = nonPreemptiveSJF(processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 3:
                processes = getProcessInput(false);
                processes = preemptiveSJF(processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 4:
                processes = getProcessInput(true);
                processes = nonPreemptivePriority(processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 5:
                processes = getProcessInput(true);
                processes = preemptivePriority(processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 6:
                processes = getProcessInput(false);
                processes = roundRobin(processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 7:
                processes = getProcessInput(true);
                processes = Own_algo (processes);
                displayProcessDetails(processes);
                displayAverageMetrics(processes);
                break;
            case 8:
                compareAllAlgorithms();
                break;
            case 9:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
