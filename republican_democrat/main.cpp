#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <vector>
using namespace std;

enum class Party { NONE, REP, DEM };

class Bathroom {
public:
    // You can adjust this value as desired:
    static const int MAX_OCCUPANCY = 3;

    Bathroom()
        : currentParty(Party::NONE),
          countInBathroom(0),
          waitingReps(0),
          waitingDems(0),
          maxOccupancy(MAX_OCCUPANCY)
    {}

    // Alternatively, you can allow a custom occupancy limit:
    // Bathroom(int maxOcc)
    //     : currentParty(Party::NONE),
    //       countInBathroom(0),
    //       waitingReps(0),
    //       waitingDems(0),
    //       maxOccupancy(maxOcc)
    // {}

    void enterRepublican(int id) {
        unique_lock <mutex> lock(mtx);
        waitingReps++;
        // Wait while
        // 1. The bathroom is being used by Democrats
        // 2. The bathroom is full
        // 3. The bathroom is empty but Democrats are waiting (fairness), OR
        cv.wait(lock , [this] {
            return ! ( (currentParty == Party :: DEM) || (currentParty == Party :: NONE && waitingDems > 0) || (countInBathroom >= MAX_OCCUPANCY) ) ;
        });
        waitingReps--;
        countInBathroom++;
        currentParty = Party :: REP;
        std::cout << "Republican " << id << " entered the bathroom. "
                  << "(countInBathroom=" << countInBathroom << ")\n";
        
    }

    void exitRepublican(int id) {
        unique_lock <mutex> lock(mtx);
        countInBathroom--;
         if (countInBathroom == 0) {
            currentParty = Party::NONE;
        }
        
        cout << "Republican " << id << " exiting the bathroom. \n";
        cv.notify_all();
    }

    void enterDemocrat(int id) {
        unique_lock <mutex> lock(mtx);
        waitingDems++;
        // Wait while
        // 1. Republicans are using the Bathroom
        // 2. Fairness if the current party is None and republicans are wating 
        // 3. The bathroom is full
        cv.wait(lock , [this] {
            return !( (currentParty == Party::REP) || (currentParty == Party::NONE && waitingReps > 0) || (countInBathroom >= MAX_OCCUPANCY) );
        });
        waitingDems--;
        countInBathroom++;
        currentParty = Party::DEM;
        std::cout << "Democrat " << id << " entered the bathroom. "
                  << "(countInBathroom=" << countInBathroom << ")\n";
        
    }

    void exitDemocrat(int id) {
        unique_lock <mutex> lock(mtx);
        countInBathroom--;
        if(countInBathroom == 0 )
            currentParty = Party::NONE;
        cout << "Democrat " << id << " exiting the bathroom. \n";
        cv.notify_all();
    }

private:
    std::mutex mtx;
    std::condition_variable cv;

    // Shared state
    Party currentParty;
    int countInBathroom;
    int waitingReps;
    int waitingDems;

    // Maximum number of people allowed in the bathroom at once
    const int maxOccupancy;
};

// Simulate some variable usage time for each person
void simulateUsage(const std::string& who, int id) {
    // Sleep for some random time between 0.5 and 1.5 seconds
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(500, 1500);
    int ms = dist(rng);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Republican thread function
void republicanThread(Bathroom& bathroom, int id) {
    // Random delay before attempt
    {
        static std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(100, 2000);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
    }

    std::cout << "Republican " << id << " attempting to enter.\n";
    bathroom.enterRepublican(id);

    simulateUsage("Republican", id);

    bathroom.exitRepublican(id);
}

// Democrat thread function
void democratThread(Bathroom& bathroom, int id) {
    // Random delay before attempt
    {
        static std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(100, 2000);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
    }

    std::cout << "Democrat " << id << " attempting to enter.\n";
    bathroom.enterDemocrat(id);

    simulateUsage("Democrat", id);

    bathroom.exitDemocrat(id);
}

int main() {
    // Create a shared Bathroom object (with default maxOccupancy = 3)
    Bathroom bathroom;

    // We will create some Republicans and Democrats
    const int NUM_PEOPLE = 5;

    std::vector<std::thread> threads;
    threads.reserve(NUM_PEOPLE * 2);

    for (int i = 0; i < NUM_PEOPLE; ++i) {
        threads.emplace_back(republicanThread, std::ref(bathroom), i);
        threads.emplace_back(democratThread, std::ref(bathroom), i);
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished.\n";
    return 0;
}
