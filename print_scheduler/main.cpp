#include <iostream>
#include <thread>
#include <condition_variable>
using namespace std;

condition_variable cv;
mutex mtx;
int current_turn = 0; // 0 for A, 1 for B, 2 for C
const int iterations = 5;

void print_char(char c, int turn) {
    for(int i = 0 ; i < iterations ; i++) {
        unique_lock <mutex> lock(mtx);
        cv.wait(lock, [&] { return current_turn % 3 == turn; });
        current_turn++;
        cout << c;
        cv.notify_all();
    }
}

int main() {
    thread t1(print_char, 'A', 0);
    thread t2(print_char, 'B', 1);
    thread t3(print_char, 'C', 2);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
