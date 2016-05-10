#ifndef RT_SYNC_QUEUE
#define RT_SYNC_QUEUE

#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

using std::move;
using std::queue;
using std::mutex;
using std::condition_variable;
using std::unique_lock;

template <typename T>
class SyncQueue {
    queue<T> q;
    mutex m;
    condition_variable cond;
    bool finished;

public:
    SyncQueue() {
        finished = false;
    }

    void push(T &&element) {
        unique_lock<mutex> lock(m);
        q.push(move(element));
        cond.notify_all();
    }

    bool pop(T &result) {
        unique_lock<mutex> lock(m);
        cond.wait(lock, [this] {return !this->q.empty() || finished;});
        if (q.empty()) {
            return false;
        }
        result = move(q.front());
        q.pop();
        return true;
    }

    void finish() {
        unique_lock<mutex> lock(m);
        finished = true;
        cond.notify_all();
    }

    bool empty() {
        unique_lock<mutex> lock(m);
        return q.empty();
    }
};

#endif
