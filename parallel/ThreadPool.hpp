#ifndef RT_THREAD_POOL
#define RT_THREAD_POOL

#include "SyncQueue.hpp"
#include <vector>
#include <thread>
#include <memory>
#include <future>
#include <numeric>
#include <functional>

using std::function;
using std::thread;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::packaged_task;

void worker(shared_ptr<SyncQueue<packaged_task<void()> > > q) {
    packaged_task<void()> task;
    while (q->pop(task)) {
        task();
    }
}

class ThreadPool {
public:
    shared_ptr<SyncQueue<packaged_task<void()> > > q;
    vector <thread> workers;
    ThreadPool(size_t workersNumber) {
        q = shared_ptr<SyncQueue<packaged_task<void()> > >(
            new SyncQueue<packaged_task<void()> >());
        for (size_t i = 0; i < workersNumber; ++i) {
            workers.push_back(thread(worker, q));
        }
    }

    void submit(std::function<void()> f) {
        q->push(packaged_task<void()>(f));
    }

    ~ThreadPool() {
        q->finish();
        for (auto &workerItem: workers) {
            workerItem.join();
        }
    }
};

#endif
