# Single-Lane Bridge Concurrency Problem

## Overview

This problem involves coordinating access to a single-lane bridge that cars approach from two opposite directions: **north** and **south**. Because the bridge is narrow, cars traveling in opposite directions cannot cross at the same time. The goal is to design a concurrency control mechanism that ensures safe, fair, and efficient passage of cars.

## Requirements

1. **Mutual Exclusion**:  
   Only cars from one direction should be on the bridge at any given time. If cars traveling north are currently on the bridge, no southbound car may enter, and vice versa.

2. **Fairness**:  
   Neither direction should be starved of access. When cars are waiting on both sides, the system should allow them to take turns entering the bridge, preventing one side from monopolizing it indefinitely.

3. **No Deadlocks or Unnecessary Delays**:  
   The system must avoid situations where cars become stuck waiting even though passage is possible. As soon as it’s safe for a car (or group of cars) to proceed, it should be allowed to do so.

## Constraints

- Each car is represented by a separate thread.
- Cars arrive from either the north or south side of the bridge.
- Multiple cars may be waiting at each side of the bridge at any time.
- Synchronization primitives such as mutexes, locks, and condition variables must be used to ensure thread-safe operations.

## Objectives

- Implement a mechanism that allows cars from only one direction on the bridge at a time.
- Ensure that when a direction finishes crossing, waiting cars from the other direction get a chance to cross.
- Prevent starvation: if cars are continuously arriving from one side, waiting cars on the other side should still be given opportunities to cross eventually.

## Example

In the provided code example:
- Two separate condition variables (`_north_cd` and `_south_cd`) manage waiting cars.
- Shared state variables (`north_waiting`, `south_waiting`, `north_travelling`, `south_travelling`, `road`, and `last_served`) determine who gets to enter next.
- Cars are represented by threads that attempt to enter, travel, and leave the bridge under the synchronization scheme.

By following these guidelines and correctly using synchronization mechanisms, you can create a system that efficiently and fairly manages traffic on a single-lane bridge, ensuring that all cars pass safely and without indefinite delays.

