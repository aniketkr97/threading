#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <random>
#include <chrono>
#include <string>

class UnisexBathroom {
private:
    static const int MAX_CAPACITY = 3;
    
    std::mutex mtx;
    std::condition_variable male_cv;
    std::condition_variable female_cv;
    
    int males_inside;
    int females_inside;
    int males_waiting;
    int females_waiting;
    
    // For maintaining fairness
    int consecutive_male_entries;
    int consecutive_female_entries;
    static const int MAX_CONSECUTIVE = 3;
    
    // For console output
    std::mutex console_mutex;
    
    void print_state(const std::string& message) {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << message << " [M_in:" << males_inside 
                 << " F_in:" << females_inside 
                 << " M_wait:" << males_waiting 
                 << " F_wait:" << females_waiting << "]" << std::endl;
    }

    bool can_male_enter() {
        return (females_inside == 0) && 
               (males_inside < MAX_CAPACITY) && 
               (consecutive_female_entries >= MAX_CONSECUTIVE || females_waiting == 0);
    }

    bool can_female_enter() {
        return (males_inside == 0) && 
               (females_inside < MAX_CAPACITY) && 
               (consecutive_male_entries >= MAX_CONSECUTIVE || males_waiting == 0);
    }

public:
    UnisexBathroom() : 
        males_inside(0), females_inside(0),
        males_waiting(0), females_waiting(0),
        consecutive_male_entries(0), consecutive_female_entries(0) {}

    void male_enter() {
        std::unique_lock<std::mutex> lock(mtx);
        
        males_waiting++;
        print_state("Male waiting to enter");
        
        male_cv.wait(lock, [this] { return can_male_enter(); });
        
        males_waiting--;
        males_inside++;
        consecutive_male_entries++;
        consecutive_female_entries = 0;
        
        print_state("Male entered bathroom");
    }

    void male_leave() {
        std::unique_lock<std::mutex> lock(mtx);
        
        males_inside--;
        print_state("Male left bathroom");
        
        if (males_inside == 0) {
            // Notify all waiting females
            female_cv.notify_all();
        } else {
            // Notify waiting males if there's still capacity
            male_cv.notify_one();
        }
    }

    void female_enter() {
        std::unique_lock<std::mutex> lock(mtx);
        
        females_waiting++;
        print_state("Female waiting to enter");
        
        female_cv.wait(lock, [this] { return can_female_enter(); });
        
        females_waiting--;
        females_inside++;
        consecutive_female_entries++;
        consecutive_male_entries = 0;
        
        print_state("Female entered bathroom");
    }

    void female_leave() {
        std::unique_lock<std::mutex> lock(mtx);
        
        females_inside--;
        print_state("Female left bathroom");
        
        if (females_inside == 0) {
            // Notify all waiting males
            male_cv.notify_all();
        } else {
            // Notify waiting females if there's still capacity
            female_cv.notify_one();
        }
    }
};

// Simulate person behavior
void person_routine(UnisexBathroom& bathroom, bool is_male, int id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> time_in_bathroom(1, 3);
    std::uniform_real_distribution<> time_between_visits(2, 5);

    for (int i = 0; i < 3; ++i) {  // Each person visits 3 times
        // Try to enter bathroom
        if (is_male) {
            bathroom.male_enter();
        } else {
            bathroom.female_enter();
        }

        // Use bathroom for random duration
        std::this_thread::sleep_for(
            std::chrono::seconds(static_cast<int>(time_in_bathroom(gen))));

        // Leave bathroom
        if (is_male) {
            bathroom.male_leave();
        } else {
            bathroom.female_leave();
        }

        // Wait before next visit
        std::this_thread::sleep_for(
            std::chrono::seconds(static_cast<int>(time_between_visits(gen))));
    }
}

int main() {
    UnisexBathroom bathroom;
    std::vector<std::thread> people;
    const int num_each_gender = 5;

    // Create male and female threads
    for (int i = 0; i < num_each_gender; ++i) {
        people.emplace_back(person_routine, std::ref(bathroom), true, i);   // Males
        people.emplace_back(person_routine, std::ref(bathroom), false, i);  // Females
    }

    // Wait for all threads to complete
    for (auto& thread : people) {
        thread.join();
    }

    std::cout << "Simulation completed." << std::endl;
    return 0;
}
