# C++ Synchronization Problems Collection

This repository contains a comprehensive collection of concurrent programming and synchronization problems commonly used in interviews, assessments, and learning exercises. Each problem is designed to help you understand and implement various aspects of multi-threaded programming in C++.

## Table of Contents

1. [Classical Synchronization Problems](#classical-synchronization-problems)
2. [Resource Allocation Problems](#resource-allocation-problems)
3. [Producer-Consumer Variations](#producer-consumer-variations)
4. [Meeting/Coordination Problems](#meetingcoordination-problems)
5. [Vehicle/Traffic Related Problems](#vehicletraffic-related-problems)
6. [Service Provider Problems](#service-provider-problems)
7. [Consumer Service Problems](#consumer-service-problems)
8. [Modern Real-world Scenarios](#modern-real-world-scenarios)
9. [Data Structure Related Problems](#data-structure-related-problems)
10. [System Design Problems](#system-design-problems)
11. [Custom Synchronization Primitives](#custom-synchronization-primitives)
12. [Advanced Synchronization Problems](#advanced-synchronization-problems)

## Problem Categories

### Classical Synchronization Problems
- **Producer-Consumer (Bounded Buffer)**
  - Implement synchronized access to a fixed-size buffer between producer and consumer threads
- **Readers-Writers**
  - Multiple readers can read simultaneously, but only one writer can write at a time
- **Dining Philosophers**
  - Solve resource allocation without deadlock for philosophers sharing forks
- **Sleeping Barber**
  - Coordinate between a barber and customers with limited waiting seats
- **Cigarette Smokers**
  - Synchronize smokers with different resources to make cigarettes
- **H2O Water Molecule Formation**
  - Coordinate hydrogen and oxygen threads to form water molecules
- **Dining Savages**
  - Manage a shared pot of food between a cook and tribal members

### Resource Allocation Problems
- Bank Teller System
- Uber Ride Matching
- Theater Ticket Booking
- Parking Lot Management
- Resource Hierarchy
- ATM Transaction System
- Database Connection Pool

### Producer-Consumer Variations
- Multiple Producers Multiple Consumers
- Priority-based Producer Consumer
- Publisher-Subscriber System
- Message Queue Implementation
- Thread Pool Management
- Work Stealing Queue
- Event Processing System

### Meeting/Coordination Problems
- Building H2O Molecules
- Unisex Bathroom
- Bridge Crossing
- River Crossing
- Soccer Match Coordination
- Party Coordination
- Conference Room Booking

## Implementation Guidelines

### Required C++ Components
```cpp
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <future>
```

### Key Synchronization Primitives
- Mutex locks
- Condition variables
- Semaphores
- Critical sections
- Atomic operations
- Future/Promise

### Common Design Patterns
- RAII (Resource Acquisition Is Initialization)
- Double-checked locking
- Monitor pattern
- Active object pattern
- Thread pool pattern
- Publisher-subscriber pattern
- Guard pattern

## Problem-Solving Checklist

When implementing any synchronization problem, ensure you address:

1. **Basic Requirements**
   - Proper thread synchronization
   - Correct resource sharing
   - Sequential consistency

2. **Safety Properties**
   - Deadlock prevention
   - Race condition handling
   - Starvation prevention

3. **Performance Considerations**
   - Minimize critical sections
   - Optimize lock granularity
   - Reduce contention

4. **Error Handling**
   - Exception safety
   - Resource cleanup
   - Thread termination

5. **Testing Strategy**
   - Unit tests
   - Stress testing
   - Race condition testing
   - Performance benchmarking

## Contributing

Feel free to contribute additional problems, solutions, or improvements to existing implementations. Please ensure your contributions follow these guidelines:

1. Include detailed problem statements
2. Provide commented code implementations
3. Add test cases
4. Document synchronization mechanisms used
5. Explain potential pitfalls and solutions

## License

This collection is available under the MIT License. See the LICENSE file for details.

---

*Note: This is a living document and will be updated with new problems and solutions as they are added to the collection.*