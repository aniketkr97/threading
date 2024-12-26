#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>

using namespace std;

class BarberShop {
private:
    // Maximum number of waiting chairs
    const int MAX_WAITING_CUSTOMERS;

    // Synchronization primitives
    mutex shopMutex;
    condition_variable barberCV;  // For barber to wait/be notified
    condition_variable customerCV; // For customers to wait/be notified

    // Shop state
    bool barberSleeping;
    int waitingCustomers;
    queue<int> waitingRoom; // Store customer IDs
    bool stop; // Flag to stop the barber thread

    // Statistics
    int customersServed;
    int customersTurnedAway;

public:
    explicit BarberShop(int chairs)
        : MAX_WAITING_CUSTOMERS(chairs),
          barberSleeping(true),
          waitingCustomers(0),
          stop(false),
          customersServed(0),
          customersTurnedAway(0) {}

    // Barber's main loop
    void barberThread() {
        while (true) {
            unique_lock<mutex> lock(shopMutex);

            // Barber sleeps if no customers are waiting
            while (waitingRoom.empty() && !stop) {
                cout << "[Barber] No customers. Going to sleep.\n";
                barberSleeping = true;
                barberCV.wait(lock, [this] {
                    return !waitingRoom.empty() || stop;
                });
                barberSleeping = false;
            }

            // Exit if stopped and no more customers
            if (stop && waitingRoom.empty()) {
                cout << "[Barber] Exiting as shop is closed and no customers are left.\n";
                break;
            }

            // Process the next customer
            waitingCustomers--;
            int customerId = waitingRoom.front();
            waitingRoom.pop();

            lock.unlock(); // Release lock while cutting hair
            cutHair(customerId);

            lock.lock();
            customersServed++;
            cout << "[Barber] Finished haircut for customer " << customerId << "\n";

            // Notify next customer if there are others waiting
            customerCV.notify_one();
        }
    }

    void stopBarber() {
        unique_lock<mutex> lock(shopMutex);
        stop = true;
        // Wake up the barber thread so it can exit
        barberCV.notify_all();
    }

    // Customer arrival
    bool customerArrives(int customerId) {
        unique_lock<mutex> lock(shopMutex);

        cout << "[Customer " << customerId << "] Entering the shop.\n";

        if (waitingCustomers >= MAX_WAITING_CUSTOMERS) {
            // Shop full, customer turned away
            cout << "[Customer " << customerId << "] Turned away: Shop is full.\n";
            customersTurnedAway++;
            return false;
        }

        // Customer takes a seat in the waiting room
        waitingCustomers++;
        waitingRoom.push(customerId);

        // Wake up the barber if sleeping
        if (barberSleeping) {
            cout << "[Customer " << customerId << "] Waking up the barber.\n";
            barberCV.notify_one();
        }

        // Wait for the customer's turn
        customerCV.wait(lock, [this, customerId] {
            return !waitingRoom.empty() && waitingRoom.front() == customerId;
        });

        // Exit if the shop is stopped
        if (stop) {
            cout << "[Customer " << customerId << "] Leaving as shop is closed.\n";
            return false;
        }

        return true;
    }

    // Helper method: Simulate barber cutting hair
    void cutHair(int customerId) {
        cout << "[Barber] Cutting hair of customer " << customerId << "...\n";
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // Statistics methods
    int getCustomersServed() const { return customersServed; }
    int getCustomersTurnedAway() const { return customersTurnedAway; }
};

void createCustomers(BarberShop& shop, int numCustomers) {
    vector<thread> customers;

    for (int i = 1; i <= numCustomers; ++i) {
        customers.emplace_back([&shop, i]() {
            shop.customerArrives(i);
        });
        this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 100)); // Random delay
    }

    for (auto& customer : customers) {
        customer.join();
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Create barber shop with 5 waiting chairs
    BarberShop shop(5);

    // Start barber thread
    thread barber(&BarberShop::barberThread, &shop);

    // Generate customers
    createCustomers(shop, 20);

    // Stop barber thread
    shop.stopBarber();
    barber.join();

    // Print statistics
    cout << "\n=== Barber Shop Statistics ===\n";
    cout << "Customers served: " << shop.getCustomersServed() << "\n";
    cout << "Customers turned away: " << shop.getCustomersTurnedAway() << "\n";

    return 0;
}
