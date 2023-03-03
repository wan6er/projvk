#include "thread_pool.h"
// #include "sync_queue.h"
#include "sync_stack.h"

#include <iostream>

template<typename _Ty>
void queue_add_num(_Ty& deque, int start, int end)
{
    for (int i = start; i < end; ++i) {
        deque.push(i);
    }
}
template<typename _Ty>
void queue_del_num(_Ty& deque, int start, int end)
{
    for (int i = start; i < end; ++i) {
        deque.pop();
    }
}

void test_queue()
{
    constexpr int COUNT = 5000;
    utils::LockFreeStack<int> deque1;

    auto start = std::chrono::system_clock::now();
    {

        std::thread thr1([&] { queue_add_num(deque1, 0, COUNT); });
        std::thread thr2([&] { queue_add_num(deque1, COUNT, 2 * COUNT); });

        thr1.join();
        thr2.join();

        
        std::thread dthr1([&] { queue_del_num(deque1, 0, COUNT); });
        std::thread thr3([&] { queue_add_num(deque1, 0, COUNT); });
        dthr1.join();
        thr3.join();
    }
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

    auto head = deque1.get_head().value();
    for (size_t i = 0; i < deque1.size() - 1; ++i) {
        head = head->next;
    }

    start = std::chrono::system_clock::now();
    
    {
        utils::LockFreeStack<int> deque2;
        std::thread thr0([&] { queue_add_num(deque2, 0, COUNT * 3); });
        thr0.join();
        std::thread dthr0([&] { queue_del_num(deque2, 0, COUNT); });
        dthr0.join();
    }

    auto cmp_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

    std::cout << "process time:" << time.count() << " " << cmp_time.count() << " ";

}

void test_pool()
{
    int a = 0;
    {
        utils::LockFreeThreadPool tasks(4);
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
        // std::cout << "process time:" << time.count() << " ";

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

}

int main()
{
    for (int i = 0; i < 10000; ++i) {
        test_queue();
        test_pool();
        std::cout << "\n";
    }
    // test_lockfree();
}