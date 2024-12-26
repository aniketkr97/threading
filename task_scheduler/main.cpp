#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace std;

using Task = pair<int, string>;



class TaskScheduler {

    mutex mtx;
    condition_variable cv;
    priority_queue <Task> taskList;
    bool stop = false;

    

    public:
        void executeTask() {
            while(true) {
                unique_lock <mutex> lock(mtx);
                cv.wait(lock , [this] {
                    return !taskList.empty() || stop;
                });
                if(taskList.empty() && stop)
                    break;
                auto [priority , taskName] = taskList.top();
                taskList.pop();
                cout << "Running Task " << taskName << "\n";
            }
        }

        void scheduleTask(int priority , string taskName) {
            unique_lock <mutex> lock(mtx);
            cout << "Scheduling Task " << taskName << "\n";
            taskList.push({priority , taskName});
            cv.notify_one();
        }

        void stop_execution() {
            unique_lock <mutex> lock(mtx);
            stop = true;
            cv.notify_all();
        }

};

int main() {
    TaskScheduler scheduler;

    // Thread to execute tasks
    std::thread executor(&TaskScheduler::executeTask, &scheduler);

    // Simulate adding tasks from multiple threads
    std::vector<std::thread> taskGenerators;
    for (int i = 1; i <= 20; ++i) {
        taskGenerators.emplace_back([&scheduler, i]() {
            int priority = rand() % 4 + 1;  // Random priority between 1 and 4
            scheduler.scheduleTask(priority , "Task-" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate delay between tasks
        });
    }
    // Join all task generator threads
    for (auto& thread : taskGenerators) {
        thread.join();
    }

    // Stop the scheduler after all tasks have been added
    scheduler.stop_execution();

    // Join the executor thread
    executor.join();

    std::cout << "Scheduler has been stopped.\n";

    return 0;
}


