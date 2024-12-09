# Reading Room with a Cleaner Problem

## Problem Statement

Imagine a public reading room in a library that accommodates a certain number of readers simultaneously. Readers arrive at random times to sit and read. As long as there are free seats available, they can enter and begin reading immediately. If the reading room is full, new readers must wait until a seat frees up when another reader leaves.

However, the reading room also needs to be cleaned periodically by a cleaner. The cleaner must have exclusive access to the room while cleaning—meaning no readers can be inside at that time. The cleaner comes at certain intervals (or when triggered by some condition) to clean the room. If readers are currently occupying seats, the cleaner must wait until they all leave. Conversely, once the cleaner starts cleaning, no new readers may enter until the cleaning is finished and the cleaner leaves.

## Key Objectives and Constraints

### 1. **Limited Seating for Readers**
- There are `N` seats in the reading room.
- Multiple readers can read at the same time, but the total number of readers currently inside cannot exceed `N`.

### 2. **Mutual Exclusion for the Cleaner**
- The cleaner requires exclusive access to the room—no readers may be present during cleaning.

### 3. **Synchronization Goals**
- Readers should enter if seats are available and the cleaner is not cleaning.
- If the cleaner arrives when readers are inside, the cleaner must wait until all readers leave.
- Once the cleaner is inside, readers must wait until the cleaner finishes cleaning.

### 4. **Fairness and Starvation Prevention**
- Ensure that neither the cleaner nor the readers experience indefinite waiting.
  - Continuous arrival of new readers should not prevent the cleaner from ever getting a chance to clean.
  - Similarly, the cleaner should not block readers for excessive periods.

## Why Is This Challenging?
This scenario combines elements of:

- **Bounded Buffer Problem**: Limited seating capacity for readers.
- **Readers-Writers Problem Variant**: Multiple readers are allowed simultaneously, but the cleaner requires exclusive access.

Balancing these constraints requires careful use of synchronization primitives such as mutexes, semaphores, or condition variables to coordinate entries and exits, and to decide who gets priority when conflicts arise.

## Requirements

### Synchronization Goals
- **Readers**:
  - Readers should be able to enter and read if there are available seats and the cleaner is not cleaning.
  - Readers should leave when done, freeing up seats.
- **Cleaner**:
  - The cleaner must have exclusive access to the reading room.
  - The cleaner should be able to clean as soon as all readers leave.

### Fairness Goals
- Readers and the cleaner should not experience starvation.
- Ensure equitable access for both readers and the cleaner.

## Solution Design

### Synchronization Primitives
- **Mutex (Lock)**: To manage access to shared resources and ensure mutual exclusion.
- **Condition Variables**:
  - To signal readers when seats are available.
  - To signal the cleaner when the room is empty.
- **Semaphores**: To keep track of the number of available seats.

### Design Strategy
1. **For Readers**:
   - Check for available seats and enter if the cleaner is not cleaning.
   - Wait if the room is full or if cleaning is in progress.
   - Notify the cleaner if the room becomes empty after they leave.

2. **For the Cleaner**:
   - Wait until all readers leave the room.
   - Once the room is empty, lock access and begin cleaning.
   - Notify waiting readers once cleaning is complete.

## Implementation Goals
- Use thread-safe synchronization mechanisms to handle concurrent access.
- Prevent deadlocks and ensure fairness using condition variables and semaphores.
- Provide a modular solution that can be extended to support more complex behaviors if needed.

---

### Example Usage
1. Define the number of seats `N` in the reading room.
2. Simulate reader arrivals and departures.
3. Simulate cleaner triggers and ensure mutual exclusion during cleaning.

---

This problem is an exercise in designing and implementing efficient synchronization strategies to handle concurrent access with fairness and mutual exclusion in a multi-threaded environment.