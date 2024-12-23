# Priority-Based Print Scheduler

## Problem Statement

The **Priority-Based Print Scheduler** is a multithreaded program designed to execute print tasks based on their assigned priorities. The goal is to process tasks in **descending order of priority** (highest priority first) using efficient multithreading and synchronization mechanisms.

This scheduler allows multiple threads to add print tasks to a shared queue, while a separate thread processes these tasks according to their priority. The tasks are represented by a message and an associated priority value, where larger values indicate higher priority.

---

## Key Features

1. **Priority Queue Implementation:**
   - Tasks are stored in a `std::priority_queue` configured as a **max-heap**, ensuring the highest-priority task is always processed first.

2. **Thread-Safe Scheduling:**
   - Multiple threads can safely add tasks to the queue using a mutex (`std::mutex`) to prevent race conditions.

3. **Efficient Task Processing:**
   - A dedicated executor thread retrieves tasks from the queue, ensuring that tasks are processed in priority order.

4. **Clean Shutdown:**
   - The scheduler supports a graceful termination mechanism using a stop flag and a condition variable (`std::condition_variable`).

---

## Code Overview

### 1. Task Representation
Each task is represented as a pair:
- **`priority (int):`** Determines the task's priority. Larger values indicate higher priority.
- **`message (string):`** The content of the task to be printed.

### 2. Classes and Functions
- **`PrintScheduler`**:
  - `schedule_print(int priority, const string& message)`:
    Adds a task to the queue with the given priority and message.
  - `execute_print()`:
    A long-running function that retrieves and processes tasks from the queue.
  - `stop_scheduler()`:
    Stops the scheduler gracefully by setting a stop flag and notifying all waiting threads.

---

## How It Works

### 1. Task Scheduling:
- Multiple threads can call `schedule_print()` to add tasks to the queue.
- Tasks are pushed into a priority queue, which maintains their order based on priority.

### 2. Task Execution:
- A separate executor thread runs `execute_print()` to retrieve and process tasks.
- The thread waits until tasks are available or the scheduler is stopped.

### 3. Graceful Shutdown:
- The scheduler can be stopped using `stop_scheduler()`, which ensures all waiting threads wake up and terminate properly after processing remaining tasks.

---

## Usage

### 1. Compilation
To compile the code, use a C++ compiler that supports C++11 or later:
```bash
g++ -std=c++11 -pthread print_scheduler.cpp -o print_scheduler

