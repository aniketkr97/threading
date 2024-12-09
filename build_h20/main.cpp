class H2O {
public:

    int h = 0;
    int o = 0;
    
    condition_variable cv;
    mutex m;
    H2O() {
        
    }

    // HHO
    // OHO
    // OHH
    // HOH
    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock <mutex> lock(m);
        cv.wait(lock , [this] {
            return h < 2 && (o == 0 || o == 1);
        });
        h++;
        if( h == 2 && o == 1) {
            h = 0;
            o = 0;
        }
        releaseHydrogen();
        cv.notify_one();
    }

    void oxygen(function<void()> releaseOxygen) {
        
        unique_lock <mutex> lock(m);
        cv.wait(lock , [this] {
            return h <= 2 && (o == 0);
        });
        o++;
        if( h == 2 && o == 1) {
            h = 0;
            o = 0;
        }
        releaseOxygen();
        cv.notify_one();// releaseOxygen() outputs "O". Do not change or remove this line.
        
    }
};
