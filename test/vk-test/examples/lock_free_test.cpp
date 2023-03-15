#include "thread_pool/thread_pool.h"
#include "lock_free/sync_queue.h"
#include "lock_free/sync_stack.h"
#include "perf_nodes.h"

#include <iostream>
#include <assert.h>

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

template<typename _SyncTy>
void test_sync(cperf::PerfNodes& perf)
{

    constexpr int COUNT = 5000;
    _SyncTy deque1;

    perf.begin("multi");
    // auto start = std::chrono::system_clock::now();
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
    // auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    perf.end("multi");

    auto head = deque1.get_head().value();
    for (size_t i = 0; i < deque1.size() - 1; ++i) {
        head = head->next;
    }

    perf.begin("single");
    // start = std::chrono::system_clock::now();
    {
        _SyncTy deque2;
        std::thread thr0([&] { queue_add_num(deque2, 0, COUNT * 3); });
        thr0.join();
        std::thread dthr0([&] { queue_del_num(deque2, 0, COUNT); });
        dthr0.join();
    }

    // auto cmp_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    perf.end("single");

    // std::cout << "process time:" << time.count() << " " << cmp_time.count() << " ";

}

void test_pool()
{
    int a = 0;
    {
        utils::ThreadPool tasks(4);
        tasks.pause();

        for (int i = 0; i < 10000; ++i) {
            tasks.push([&]() {
                // std::this_thread::yield();
                a++;
            });
        }
        tasks.start();
        tasks.wait_done();

        tasks.pause();
        tasks.push([&]() {
            for (int i = 0; i < 10000; ++i) {
                // std::this_thread::yield();
                a++;
            }
        });
        // start = std::chrono::system_clock::now();
        tasks.start();
        tasks.wait_done();        
    }

}

#include "lock_free_ptr/shared_ptr.h"
void test_ring_ref()
{
    static int test_c = 0;

    {

        struct test1;
        struct test2;

        using test1_SharedPtr = utils::SharedPtr<test1>;
        using test1_WeakPtr = utils::WeakPtr<test1>;
        using test2_SharedPtr = utils::SharedPtr<test2>;
        using test2_WeakPtr = utils::WeakPtr<test2>;

        struct test1
        {
            test1() {
                test_c++;
            }
            ~test1() {
                test_c--;
            }
            test2_WeakPtr ptr;
        };

        struct test2
        {
            test2() {
                test_c++;
            }
            ~test2() {
                test_c--;
            }
            test1_WeakPtr ptr;
        };

        test1_SharedPtr tp1 = utils::make_shared<test1>();
        test2_SharedPtr tp2 = utils::make_shared<test2>();

        tp1->ptr = tp2;
        tp2->ptr = tp1;

        assert(test_c == 2);
    }
    assert(test_c == 0);
}

#include "lock_free_ptr/lock_free_ptr.h"
void test_lockfree_ring_ref()
{
    static int test_c = 0;
    {

        struct test1;
        struct test2;

        using test1_SharedPtr = utils::LockFreePtr<test1>;
        using test1_WeakPtr = utils::LockFreeWeakPtr<test1>;
        using test2_SharedPtr = utils::LockFreePtr<test2>;
        using test2_WeakPtr = utils::LockFreeWeakPtr<test2>;

        struct test1
        {
            test1() {
                test_c++;
            }
            ~test1() {
                test_c--;
            }
            test2_WeakPtr ptr;
        };

        struct test2
        {
            test2() {
                test_c++;
            }
            ~test2() {
                test_c--;
            }
            test1_WeakPtr ptr;
        };

        test1_SharedPtr tp1 = utils::make_ptr<test1>();
        test2_SharedPtr tp2 = utils::make_ptr<test2>();

        tp1->ptr = tp2;
        tp2->ptr = tp1;

        assert(test_c == 2);

    }
    assert(test_c == 0);

}

void test_thread_ref()
{
    struct node;
    using node_ptr = utils::SharedPtr<node>;
    using atomic_node_ptr = utils::LockFreePtr<node>;

    struct node
    {
        int data = 0;
        node_ptr next = nullptr;

        node() = default;
        node(int a) : data(a) {}

        ~node() {
            // std::cout << "~node : " << data << ";\n";
        }
    };

    auto push = [](atomic_node_ptr& head, int d) {
        auto expect = head.load();
        auto n = utils::make_shared<node>(d);
        while (!head.compare_exchange_weak(expect, n));
        n->next = expect;
    };

    atomic_node_ptr head;

    auto push_test = [&]() {
        for (int i = 0; i < 10000; ++i) {
            push(head, i);
        }
    };
    std::thread th1(push_test);
    std::thread th2(push_test);
    th1.join();
    th2.join();

    node_ptr ptr = head.load();
    for (int i = 0; i < 20000 - 1; ++i) {
        ptr = ptr->next;
    }
    assert(!(ptr == nullptr));

}

int main()
{
    // test_ring_ref();
    // test_lockfree_ring_ref();
    test_thread_ref();

    // for (int i = 0; i < 10000; ++i) {
    //     cperf::PerfNodes perf;
    //     perf.begin("sync");
    //     test_sync<utils::LockFreeQueue<int>>(perf);
    //     perf.end("sync");
    //     perf.begin("pool");
    //     test_pool();
    //     perf.end("pool");

    //     cperf::read_nodes(perf.dump(), [](cperf::_Key name, cperf::_ListNode<cperf::PerfNode> const* head) {
    //         auto time = head->val.end - head->val.start;
    //         std::cout << name << " " << cperf::time_cast<std::chrono::milliseconds>(time) << ", ";
    //     });
    //     std::cout << "\n";

    // }
}