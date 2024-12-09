#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class FizzBuzz {
private:
    int n;
    int counter = 1;
    mutex mtx;
    condition_variable condition;

public:
    FizzBuzz(int n) {
        this->n = n;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            condition.wait(lock, [this] {
                return (counter > n) || (counter % 3 == 0 && counter % 5 != 0);
            });
            if (counter > n)
                break;
            printFizz();
            counter++;
            condition.notify_all();
        }
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            condition.wait(lock, [this] {
                return (counter > n) || (counter % 3 != 0 && counter % 5 == 0);
            });
            if (counter > n)
                break;
            printBuzz();
            counter++;
            condition.notify_all();
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
    void fizzbuzz(function<void()> printFizzBuzz) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            condition.wait(lock, [this] {
                return (counter > n) || (counter % 3 == 0 && counter % 5 == 0);
            });
            if (counter > n)
                break;
            printFizzBuzz();
            counter++;
            condition.notify_all();
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            condition.wait(lock, [this] {
                return (counter > n) || (counter % 3 != 0 && counter % 5 != 0);
            });
            if (counter > n)
                break;
            printNumber(counter);
            counter++;
            condition.notify_all();
        }
    }
};

int main() {
    int n = 15; // Define the range for FizzBuzz
    FizzBuzz fizzBuzz(n);

    // Threads for each task
    thread fizzThread(&FizzBuzz::fizz, &fizzBuzz, []() {
        cout << "fizz ";
    });

    thread buzzThread(&FizzBuzz::buzz, &fizzBuzz, []() {
        cout << "buzz ";
    });

    thread fizzBuzzThread(&FizzBuzz::fizzbuzz, &fizzBuzz, []() {
        cout << "fizzbuzz ";
    });

    thread numberThread(&FizzBuzz::number, &fizzBuzz, [](int x) {
        cout << x << " ";
    });

    // Join threads to ensure proper execution
    fizzThread.join();
    buzzThread.join();
    fizzBuzzThread.join();
    numberThread.join();

    return 0;
}

