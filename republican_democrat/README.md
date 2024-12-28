# Republican-Democrat Bathroom Problem

## Problem Description
The Republican-Democrat bathroom problem is a classic synchronization problem that illustrates concurrent access control to a shared resource. In this scenario, both Republicans and Democrats need to use a shared bathroom facility, but certain constraints must be enforced to maintain order and prevent conflicts.

### Problem Constraints
1. Multiple people from the same party can use the bathroom simultaneously
2. People from different parties cannot use the bathroom at the same time
3. There is a maximum occupancy limit for the bathroom
4. The solution must be fair (prevent starvation)
5. The solution must be deadlock-free

## Solution Overview
The implementation provides a thread-safe bathroom management system using C++ synchronization primitives. It ensures all constraints are met while maintaining fairness and preventing deadlocks.

### Key Features
- Thread-safe access control
- Configurable maximum occupancy
- Fair scheduling between parties
- Deadlock prevention
- Starvation prevention
- Real-time status monitoring

### Implementation Details
The solution uses the following C++ features:
- `std::mutex` for thread synchronization
- `std::condition_variable` for thread waiting/notification
- `std::thread` for simulating multiple users
- RAII principles for resource management

## Code Structure

### Main Components
- `Bathroom` class: Core synchronization logic
- `Party` enum: Represents political affiliations
- Simulation functions: For testing the implementation

### Key Methods
```cpp
void enterRepublican(int id);
void exitRepublican(int id);
void enterDemocrat(int id);
void exitDemocrat(int id);
```

### Synchronization Mechanisms
The implementation uses:
1. Mutex for protecting shared state
2. Condition variables for party coordination
3. Wait predicates for enforcing access rules
4. Atomic operations for thread-safe counters

