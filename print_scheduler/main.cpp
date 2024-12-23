#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
using namespace std;

// Define a type for the task: (priority, message)
using Task = pair<int, string>;

class PrintScheduler {
private:
    priority_queue<Task> task_queue; // Max-heap by default in C++ for priority_queue
    mutex mtx; // Mutex for synchronizing access to task_queue
    condition_variable cv; // Condition variable for thread notification
    bool stop = false; // Flag to stop the scheduler

public:
    // Function to schedule a print task
    void schedule_print(int priority, const string& message) {
        unique_lock<mutex> lock(mtx);
        task_queue.push({priority, message});
        cout << "Scheduled: [" << priority << "] " << message << endl;
        cv.notify_one(); // Notify a waiting thread
    }

    // Function to execute and print tasks
    void execute_print() {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this] { // Wait until there is a task or stop is requested
                return !task_queue.empty() || stop;
            });

            if (stop && task_queue.empty()) {
                break;
            }

            // Process the highest-priority task
            cout << "Running task: " << task_queue.top().second << endl;
            task_queue.pop();
        }
    }

    void stop_scheduler() {
        unique_lock<mutex> lock(mtx);
        stop = true;
        cv.notify_all(); // Notify all waiting threads so that they can exit
    }
};

int main() {
    PrintScheduler scheduler;

    // Start the executor thread
    thread executor(&PrintScheduler::execute_print, &scheduler);

    // Schedule tasks
    vector<thread> threads;
    for (int i = 1; i <= 10; ++i) {
        threads.emplace_back(&PrintScheduler::schedule_print, &scheduler, i, "Task " + to_string(i));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Stop the scheduler
    this_thread::sleep_for(chrono::seconds(1)); // Simulate delay to process tasks
    scheduler.stop_scheduler();

    executor.join(); // Wait for the executor thread to finish
    return 0;
}
