#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

class ReadersWriters {
public:
    ReadersWriters(int numReaders, int numWriters)
        : readCount(0),
          num_readers(numReaders),
          num_writers(numWriters) 
    {
        // No active readers or writers yet
    }

    void run() {
        // Launch multiple reader threads
        // Each thread runs the same 'reader' function but acts as an independent reader
        for (int i = 1; i <= num_readers; ++i) {
            threads.emplace_back(&ReadersWriters::reader, this, i);
        }

        // Launch multiple writer threads
        // Each thread runs the same 'writer' function but acts as an independent writer
        for (int i = 1; i <= num_writers; ++i) {
            threads.emplace_back(&ReadersWriters::writer, this, i);
        }

        // Join all threads to ensure the main program waits for them to finish
        for (auto &t : threads) {
            t.join();
        }
    }

private:
    void reader(int id) {
        // In a loop, continuously read
        // In a real scenario, you might use a finite loop or condition
        while (true) {
            {
                // Lock the mutex to safely change readCount
                std::unique_lock<std::mutex> lock(mtx);
                
                // Wait until it's safe to read (i.e., no writer active)
                // Condition: It's safe to read if no writer is holding wrt_mtx
                // We rely on the logic that if no writer holds wrt_mtx, readers can proceed.
                readCount++;
                if (readCount == 1) {
                    // If this is the first reader, we lock the wrt_mtx to block writers
                    wrt_mtx.lock();
                }
            } // mutex 'mtx' unlocked here, readCount safely updated

            // --- Critical Section: Reading the resource ---
            std::cout << "Reader " << id << " is reading the data...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            {
                // After reading, lock mtx again to update readCount
                std::unique_lock<std::mutex> lock(mtx);
                readCount--;
                if (readCount == 0) {
                    // If this reader was the last reader, unlock wrt_mtx so writers can proceed
                    wrt_mtx.unlock();
                }
            }

            // Optional small pause to allow other threads to run
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void writer(int id) {
        // In a loop, continuously write
        while (true) {
            // Lock wrt_mtx for exclusive write access
            std::unique_lock<std::mutex> lock_wrt(wrt_mtx);

            // --- Critical Section: Writing the resource ---
            std::cout << "Writer " << id << " is writing data...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(150));

            // When unique_lock goes out of scope, wrt_mtx is unlocked automatically

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // Count of how many readers are currently accessing the resource
    int readCount;

    // Synchronization primitives
    std::mutex mtx;        // Protects readCount
    std::mutex wrt_mtx;    // Ensures mutual exclusion for writers

    int num_readers;
    int num_writers;

    std::vector<std::thread> threads;
};

int main() {
    // For example: 3 readers and 2 writers
    ReadersWriters rw(3, 2);
    rw.run();

    return 0;
}
