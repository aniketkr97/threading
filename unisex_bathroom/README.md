Introduction

The Unisex Bathroom Problem is a classic synchronization challenge in operating systems and concurrent programming. It focuses on ensuring that multiple threads representing Males and Females can share a single resource (the bathroom), while preventing certain conflicts:

No opposite genders can occupy the bathroom at the same time.
Threads of the same gender can enter together to maximize concurrency.
Fairness should be maintained so no individual or group starves.
Problem Statement

There is one bathroom.
Multiple Males and Females arrive concurrently, each wanting to use the bathroom.
The bathroom can be in one of three states:
Empty
Occupied by Males
Occupied by Females
When occupied by one gender, any thread of the opposite gender must wait until the bathroom is empty.
The system should:
Prevent deadlock (no indefinite waiting with threads blocking each other).
Avoid starvation (every thread eventually gets to use the bathroom).
Ensure concurrency (if multiple people of the same gender arrive, they enter together).