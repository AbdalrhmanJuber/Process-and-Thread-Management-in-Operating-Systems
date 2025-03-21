# Process and Thread Management in Operating Systems

## Project Overview
This project demonstrates essential process and thread management concepts using C programming in Linux. It includes the implementation and comparison of multiprocessing and multithreading, using matrix multiplication as a computational workload to measure performance.

## Project Goals
- Create and manage child processes using `fork()`.
- Establish inter-process communication via pipes.
- Implement multithreading with shared data.
- Compare performance between multiprocessing and multithreading.
- Demonstrate differences between joinable and detached threads.

## Key Functionalities
### 1. Process Management
- Creation of child processes.
- Communication between parent and child processes using pipes.

### 2. Multithreading
- Implementation of threads sharing common data structures.
- Analysis of thread synchronization and management.

### 3. Performance Measurement
- Matrix multiplication as a workload for performance benchmarking.
- Comparison of execution times for processes and threads.
- Evaluation based on varying numbers of processes and threads.

### 4. Thread Management
- Comparison between joinable and detached threads.
- Analysis of execution behaviors and performance differences.

## Results and Observations
- Multiprocessing showed faster execution times in this particular task.
- Multithreading incurred overhead due to data sharing complexity.
- Increasing the number of processes improved performance consistent with Amdahl's Law.
- Detached threads executed faster but did not guarantee complete task execution compared to joinable threads.

## How to Run the Project
1. Clone this repository.
2. Compile the project using:
```bash
gcc main.c -o main -pthread
```
3. Execute the compiled program:
```bash
./main
```

## Technologies Used
- C programming language
- Linux system calls (`fork()`, `pipe()`, `pthread`)

## University
Birzeit University, Faculty of Engineering and Technology, Department of Electrical and Computer Engineering.

## Date
December 2, 2023

