#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class Bridge{

    enum Direction {
        EMPTY = 0,
        NORTH = 1,
        SOUTH = 2,
    };

    private:
        condition_variable cv_north;
        condition_variable cv_south;
        mutex m;
        int currentDirection;    // 0 = empty, 1 = north, 2 = south
        int lastServed;          // 1 for North last served and 2 for south last served
        int northWaiting;
        int northCrossing;
        int southWaiting;
        int southCrossing;
        void crossBridge(int carId, int direction) {
            string dirStr = (direction == NORTH) ? "NORTH" : "SOUTH";
            std::cout << "Car " << carId << " from " << dirStr << " is crossing the bridge...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "Car " << carId << " from " << dirStr << " has crossed.\n";
    }

    public:
        Bridge() : currentDirection(EMPTY),
                    lastServed(SOUTH) ,   // start by giving priority to north (so lastServed=2 => next prefer north)
                    northWaiting(0),   
                    northCrossing(0),
                    southWaiting(0),
                    southCrossing(0) {};

        void carNorthArrives(int carId) {
            unique_lock <mutex> lock(m);
            northWaiting++;
            cv_north.wait(lock, [this] {
                return currentDirection == EMPTY || 
                    (lastServed == SOUTH && southCrossing == 0);
            });


            // Process to Cross
            northWaiting--;
            northCrossing++;
            lastServed = NORTH;
            currentDirection = NORTH;
            lock.unlock();

            crossBridge(carId, NORTH);

            lock.lock();
            northCrossing--;
            if(northCrossing == 0) {
                currentDirection = EMPTY;
                cv_south.notify_all();
            }        
        }

        void carSouthArrives(int carId) {
            unique_lock <mutex> lock(m);
            southWaiting++;
            cv_south.wait(lock, [this] {
                return currentDirection == EMPTY || 
                       (lastServed == NORTH && northCrossing == 0);
            });

            // Process to Cross
            southWaiting--;
            southCrossing++;
            lastServed = SOUTH;
            currentDirection = SOUTH;
            lock.unlock();

            crossBridge(carId , SOUTH);

            lock.lock();
            southCrossing--;
            if(southCrossing == 0) {
                currentDirection = EMPTY;
                cv_north.notify_all();
            }

        }

};

int main() {
    Bridge bridge;

    std::vector<std::thread> cars;

    // Create a mix of north and south cars
    // For demonstration: first 5 from north, next 5 from south, then mixed.
    for (int i = 1; i <= 5; ++i) {
        cars.emplace_back(&Bridge::carNorthArrives, &bridge, i);
    }
    for (int i = 6; i <= 10; ++i) {
        cars.emplace_back(&Bridge::carSouthArrives, &bridge, i);
    }
    for (int i = 11; i <= 15; ++i) {
        if (i % 2 == 0)
            cars.emplace_back(&Bridge::carSouthArrives, &bridge, i);
        else
            cars.emplace_back(&Bridge::carNorthArrives, &bridge, i);
    }

    for (auto &c : cars) {
        c.join();
    }

    return 0;
}
