#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <atomic>
#include <memory>
#include <vector>

class DelayedScheduler {
private:
    struct Task {
        std::function<void()> function;
        std::chrono::steady_clock::time_point executeAt;
        
        Task(std::function<void()> func, std::chrono::steady_clock::time_point time)
            : function(std::move(func)), executeAt(time) {}
        
        bool operator>(const Task& other) const {
            return executeAt > other.executeAt;
        }
    };

    using TaskPtr = std::shared_ptr<Task>;
    
    std::priority_queue<TaskPtr, std::vector<TaskPtr>, 
        std::greater<TaskPtr>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> running{true};
    std::vector<std::thread> workerThreads;
    
    void workerFunction() {
        while (running) {
            TaskPtr task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                
                auto waitUntil = [this]() {
                    return !running || 
                           (!taskQueue.empty() && 
                            taskQueue.top()->executeAt <= std::chrono::steady_clock::now());
                };
                
                condition.wait(lock, waitUntil);
                
                if (!running) break;
                if (taskQueue.empty()) continue;
                
                task = taskQueue.top();
                taskQueue.pop();
            }
            
            if (task) {
                task->function();
            }
        }
    }

public:
    explicit DelayedScheduler(size_t threadCount = 1) {
        for (size_t i = 0; i < threadCount; ++i) {
            workerThreads.emplace_back(&DelayedScheduler::workerFunction, this);
        }
    }
    
    ~DelayedScheduler() {
        shutdown();
    }
    
    // Schedule a task to run after a delay
    void scheduleAfter(std::function<void()> task, 
                      std::chrono::milliseconds delay) {
        auto executeTime = std::chrono::steady_clock::now() + delay;
        auto taskPtr = std::make_shared<Task>(std::move(task), executeTime);
        
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(taskPtr);
        }
        condition.notify_one();
    }
    
    // Schedule a task to run at a specific time point
    template<typename Clock, typename Duration>
    void scheduleAt(std::function<void()> task, 
                   std::chrono::time_point<Clock, Duration> timePoint) {
        auto executeTime = std::chrono::time_point_cast<std::chrono::steady_clock::duration>(
            timePoint - Clock::now() + std::chrono::steady_clock::now());
        
        auto taskPtr = std::make_shared<Task>(std::move(task), executeTime);
        
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(taskPtr);
        }
        condition.notify_one();
    }
    
    void shutdown() {
        running = false;
        condition.notify_all();
        
        for (auto& thread : workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        workerThreads.clear();
    }
};

// Example usage
int main() {
    DelayedScheduler scheduler(2); // Create scheduler with 2 worker threads
    
    // Schedule some tasks
    scheduler.scheduleAfter(
        []() { std::cout << "Task 1 executed after 2 seconds\n"; },
        std::chrono::seconds(2)
    );
    
    scheduler.scheduleAfter(
        []() { std::cout << "Task 2 executed after 1 second\n"; },
        std::chrono::seconds(1)
    );
    
    // Schedule a task at a specific time
    auto futureTime = std::chrono::system_clock::now() + std::chrono::seconds(3);
    scheduler.scheduleAt(
        []() { std::cout << "Task 3 executed at specific time\n"; },
        futureTime
    );
    
    // Wait for a while to let tasks execute
    std::this_thread::sleep_for(std::chrono::seconds(4));
    
    // Shutdown the scheduler
    scheduler.shutdown();
    
    return 0;
}