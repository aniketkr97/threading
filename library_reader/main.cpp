#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

const int SEATS = 3;

class LibraryCleaner {

    private:
        int active_readers;
        int waiting_readers;
        int waiting_cleaner;
        int active_cleaner;
        int seatsOccupied;
        mutex m;
        condition_variable cv;

    public:
        LibraryCleaner() : active_readers(0) , seatsOccupied(0) , waiting_cleaner(0) , waiting_readers(0) , active_cleaner(0) {};

        void Reader(int id) {

            {
                unique_lock <mutex> lock(m);
                waiting_readers++;
                cv.wait(lock , [this]{
                    return (active_cleaner == 0 &&   seatsOccupied <SEATS);
                });

                // READ OPERATION
                active_readers++;
                seatsOccupied++;
                waiting_readers--;
                cout << "Reader " << id << " is reading" << endl;
            }
        

            // READER LEAVES

            {
                unique_lock <mutex> lock(m);
                active_readers--;
                seatsOccupied--;
                cout << "Reader " << id << " is leaving" << endl;
                if(active_readers == 0 && waiting_cleaner > 0) 
                    cv.notify_one();
                else    
                    cv.notify_all();
                
            }

        }

        void cleaner(int id){
            
            {
                unique_lock <mutex> lock(m);
                waiting_cleaner++;
                cv.wait(lock, [this] {
                    return active_readers == 0 &&  seatsOccupied == 0;
                });

                cout << "Cleaner " << id << " is cleaning" << endl;
                // CLEANER CLEANS
                active_cleaner++;
                waiting_cleaner--;
            }

            {
                unique_lock <mutex> lock(m);
                
                active_cleaner--;
                cout << "Cleaner  " << id << " is leaving " << endl;  
                if(active_cleaner == 0 )
                    cv.notify_all();
            }
        }

};


int main() {
    LibraryCleaner library;

    vector<thread> readers;
    vector<thread> cleaners;

    // Spawn reader threads
    for (int i = 1; i <= 20; ++i) {
        readers.emplace_back(&LibraryCleaner::Reader, &library, i);
        // this_thread::sleep_for(chrono::milliseconds(200)); // Simulate staggered arrivals
    }

    // Spawn cleaner threads
    for (int i = 1; i <= 2; ++i) {
        cleaners.emplace_back(&LibraryCleaner::cleaner, &library, i);
        // this_thread::sleep_for(chrono::seconds(10)); // Simulate cleaner arrival intervals
    }

    // Join threads
    for (auto &reader : readers) {
        reader.join();
    }

    for (auto &cleaner : cleaners) {
        cleaner.join();
    }

    return 0;
}