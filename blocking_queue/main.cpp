class BoundedBlockingQueue {
public:
    int capacity;
    queue <int> q;
    mutex m;
    condition_variable cv;
    BoundedBlockingQueue(int capacity) {
        this->capacity = capacity;
    }
    
    void enqueue(int element) {
        unique_lock <mutex> lock(m);
        cv.wait(lock , [this] {
            return q.size() < capacity;
        });
        q.push(element);
        cv.notify_one();
    }
    
    int dequeue() {
        unique_lock <mutex> lock(m);
        cv.wait(lock , [this] {
            return q.size() > 0;
        });
        int val = q.front();
        q.pop();
        cv.notify_one();
        return val;
    }
    
    int size() {
        unique_lock <mutex> lock(m);
        return q.size();
    }
};
