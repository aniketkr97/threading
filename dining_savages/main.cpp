#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

// Shared resource
int potServings = 0;               
const int MAX_SERVINGS = 5;        // Capacity of the pot when refilled

// Synchronization primitives
mutex potMutex;                    // Protects access to potServings
condition_variable potEmptyCV;     // Signals the cook that the pot is empty
condition_variable potRefilledCV;  // Signals the savages that the pot is refilled

// Savage thread function
void savage(int id) {
    while (true) {
        unique_lock<mutex> lock(potMutex);

        // If the pot is empty, signal the cook and wait
        while (potServings == 0) {
            cout << "Savage " << id << ": Pot is empty, waking the cook.\n";
            potEmptyCV.notify_one();             // Notify cook
            potRefilledCV.wait(lock, [] { 
                return potServings > 0;          // Wait until pot is refilled
            });
        }

        // Consume one serving
        potServings--;
        cout << "Savage " << id << ": Eats a serving. Servings left: " 
             << potServings << endl;

        // Release the mutex
        lock.unlock();

        // Simulate eating time
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

// Cook thread function
void cook() {
    while (true) {
        // Wait for a savage to signal that the pot is empty
        unique_lock<mutex> lock(potMutex);
        potEmptyCV.wait(lock, [] {
            return potServings == 0;
        });

        // Refill the pot
        potServings = MAX_SERVINGS;
        cout << "Cook: Refills the pot with " 
             << MAX_SERVINGS << " servings.\n";

        // Notify all waiting savages that the pot is refilled
        potRefilledCV.notify_all();
    }
}

int main() {
    const int NUM_SAVAGES = 3;  // Number of savage threads

    // Create the cook thread
    thread cookThread(cook);

    // Create savage threads
    vector<thread> savageThreads;
    for (int i = 1; i <= NUM_SAVAGES; i++) {
        savageThreads.emplace_back(savage, i);
    }

    // Join all threads (infinite loop, so realistically won't return)
    cookThread.join();
    for (auto &t : savageThreads) {
        t.join();
    }

    return 0;
}
