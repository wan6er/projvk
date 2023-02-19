#include "test_header.h"

#include "thread_executor.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>
#include <iostream>


#include "sync_queue.h"

template<typename _Ty>
void queue_add_num(_Ty& deque, int start, int end)
{
    for (int i = start; i < end; ++i) {
        deque.push_back(i);
    }
}
void queue_del_num(utils::SyncQueue<int>& deque, int start, int end)
{
    for (int i = start; i < end; ++i) {
        deque.pop_front();
    }
}

TEST_FUNC_BEGIN("sync deque")
    constexpr int COUNT = 10000;

    auto start = std::chrono::system_clock::now();
    
    utils::SyncQueue<int> deque1;
    {

        std::thread thr1([&] { queue_add_num(deque1, 0, COUNT); });
        std::thread thr2([&] { queue_add_num(deque1, 0, COUNT); });

        thr1.join();
        thr2.join();
        
        std::thread dthr1([&] { queue_del_num(deque1, 0, COUNT); });
        std::thread thr3([&] { queue_add_num(deque1, 0, COUNT); });
        dthr1.join();
        thr3.join();
    }
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    
    start = std::chrono::system_clock::now();
    
    {
        utils::SyncQueue<int> deque2;
        std::thread thr0([&] { queue_add_num(deque2, 0, COUNT * 3); });
        thr0.join();
        std::thread dthr0([&] { queue_del_num(deque2, 0, COUNT); });
        dthr0.join();
    }

    auto cmp_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

    // start = std::chrono::system_clock::now();
    
    // {
    //     std::deque<int> stddeque;
    //     std::thread stdthr([&] { queue_add_num(stddeque, 0, COUNT * 2); });
    //     stdthr.join();
    // }

    // auto std_cmp_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);


    CHECK(deque1.flag().size == COUNT * 2);
    std::cout << "process time:" << time.count() << " " << cmp_time.count() << " ";


TEST_FUNC_END

void test_add(int& a, int& b)
{
    a = a + b;
}

#include "thread_pool.h"
TEST_FUNC_BEGIN("thread pool")
    
    int a = 0;
    {
        utils::ThreadPool tasks(8);
        tasks.pause();

        for (int i = 0; i < 10000; ++i) {
            tasks.push([&]() {
                // std::this_thread::yield();
                a++;
            });
        }
        auto start = std::chrono::system_clock::now();
        tasks.start();
        tasks.wait_done();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        std::cout << "process time:" << time.count() << " ";

        tasks.pause();
        tasks.push([&]() {
            for (int i = 0; i < 10000; ++i) {
                // std::this_thread::yield();
                a++;
            }
        });
        start = std::chrono::system_clock::now();
        tasks.start();
        tasks.wait_done();        
        auto cmp_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        std::cout << "process time:" << time.count() << " " << cmp_time.count() << " ";
    }

    // CHECK(a > 0);

TEST_FUNC_END

#include "perf_node.h"
TEST_FUNC_BEGIN("cperf")

    cperf::PerfNode<std::string> head("head");
    cperf::PerfNode<std::string> test1("test1");
    cperf::PerfNode<std::string> test2("test2");
    cperf::PerfNode<std::string> test3("test3");
    cperf::append_perf_node(head, test1);
    cperf::append_perf_node(head, test2);
    cperf::append_perf_node(head, test3);
    CHECK(head.child->child->child->key == test3.key);

TEST_FUNC_END