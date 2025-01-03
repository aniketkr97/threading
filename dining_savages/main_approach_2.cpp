#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <mutex>
#include <chrono>

using namespace std;

// Shared resource
int potServings = 0;
const int MAX_SERVINGS = 5;  // Capacity of the pot when refilled

// Synchronization primitives (C++20 semaphores)
binary_semaphore cookSemaphore(0);  // Signaled by a savage when the pot is empty
binary_semaphore potSemaphore(0);   // Signaled by the cook when the pot is refilled
mutex potMutex;                     // Protects the shared resource (potServings)

// Savage thread function
void savage(int id) {
    while (true) {
        // Lock the pot to check if there are servings
        potMutex.lock();

        // If the pot is empty, wake the cook and wait for a refill
        if (potServings == 0) {
            cout << "Savage " << id << " sees the pot is empty and wakes the cook.\n";

            // Signal the cook that the pot is empty
            cookSemaphore.release();

            // Release the lock before waiting on potSemaphore (avoid deadlock)
            potMutex.unlock();

            // Wait until the cook refills the pot
            potSemaphore.acquire();

            // Lock again to consume after pot is refilled
            potMutex.lock();
        }

        // Consume one serving
        potServings--;
        cout << "Savage " << id << " is eating. Servings left: " << potServings << "\n";

        // Unlock the pot so other savages can access it
        potMutex.unlock();

        // Simulate time taken to eat
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

// Cook thread function
void cook() {
    while (true) {
        // Wait for a savage to signal that the pot is empty
        cookSemaphore.acquire();

        // Refill the pot
        potMutex.lock();
        potServings = MAX_SERVINGS;
        cout << "Cook refills the pot with " << MAX_SERVINGS << " servings.\n";
        potMutex.unlock();

        // Notify all waiting savages (or the next savage) that the pot is refilled
        potSemaphore.release();
    }
}

int main() {
    const int NUM_SAVAGES = 3;  // Number of savages

    // Create the cook thread
    thread cookThread(cook);

    // Create savage threads
    vector<thread> savageThreads;
    for (int i = 1; i <= NUM_SAVAGES; i++) {
        savageThreads.emplace_back(savage, i);
    }

    // Join threads (infinite loops in the functions, so program runs indefinitely)
    cookThread.join();
    for (auto &t : savageThreads) {
        t.join();
    }

    return 0;
}
