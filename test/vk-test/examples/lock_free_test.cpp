#include "thread_pool/thread_pool.h"
// #include "lock_free/sync_queue.h"
#include "lock_free/sync_stack.h"
#include "lock_free_ptr/lock_free_ptrs.h"
#include "lock_free_ptr/shared_ptrs.h"
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
    typename _Ty::SharedNodePtr ptr;
    for (int i = start; i < end; ++i) {
        ptr = deque.pop();
        if (!ptr.empty()) {
            // std::cout << ptr->val << ";\t";
        }
    }
    // std::cout << "\n";
}

template<typename _SyncTy>
void test_sync(cperf::PerfNodes& perf)
{

    constexpr int COUNT = 2000;
    _SyncTy deque1;
    {

        std::thread thr1([&] { queue_add_num(deque1, 0, COUNT); });
        std::thread thr2([&] { queue_add_num(deque1, COUNT, 2 * COUNT); });
        thr2.join();
        thr1.join();
        // std::cout << "push finished\t";

        auto ptr = deque1.top();
        for (size_t i = 0; i < deque1.size(); ++i) {
            CPERF_ASSERT(!ptr.empty());
            ptr = ptr->next;
        }

        std::thread dthr1([&] { queue_del_num(deque1, 0, COUNT); });
        std::thread dthr2([&] { queue_del_num(deque1, 0, COUNT); });
        dthr1.join();
        dthr2.join();
        std::cout << "clean finished\t";
        CPERF_ASSERT(deque1.size() == 0);

        std::thread dthr3([&] { queue_del_num(deque1, 0, COUNT); });
        std::thread thr3([&] { queue_add_num(deque1, 0, COUNT); });
        std::thread dthr4([&] { queue_del_num(deque1, 0, COUNT); });
        std::thread thr4([&] { queue_add_num(deque1, 0, COUNT); });
        dthr4.join();
        thr4.join();
        dthr3.join();
        thr3.join();
        std::cout << "push & pop finished\t";

        ptr = deque1.top();
        for (size_t i = 0; i < deque1.size(); ++i) {
            CPERF_ASSERT(!ptr.empty());
            ptr = ptr->next;
        }
 
    }
}

void test_pool()
{
    std::atomic<int> a = 0;
    int const COUNT = 5000;
    {
        utils::ThreadPool tasks(4);
        // tasks.pause();

        for (int i = 0; i < COUNT; ++i) {
            tasks.push([&]() {
                // std::this_thread::yield();
                a++;
            });
        }
        tasks.wait_done();
        CPERF_ASSERT(a == COUNT);

        tasks.push([&]() {
            for (int i = 0; i < COUNT; ++i) {
                // std::this_thread::yield();
                a++;
            }
        });
        tasks.wait_done();        
        // std::cout << "one task finished\t";
        CPERF_ASSERT(a == COUNT * 2);
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

        // assert(tp1 != tp1->ptr);

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

        // assert(tp1 != tp1->ptr);

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
        auto n = utils::make_shared<node>(d);
        node_ptr expect;
        head.load(expect);
        while (!head.compare_exchange_weak(expect, n));
        n->next = expect;
    };

    atomic_node_ptr head;

    auto push_test = [&]() {
        for (int i = 0; i < 1000; ++i) {
            push(head, i);
        }
    };
    std::thread th1(push_test);
    std::thread th2(push_test);
    th1.join();
    th2.join();


}

#include <map>
int main()
{
    for (int i = 0; i < 10000; ++i) {
        // test_ring_ref();
        // test_lockfree_ring_ref();
        // test_thread_ref();
        cperf::PerfNodes perf;
        // perf.begin("atomic_stack");
        test_sync<utils::LockFreeStack<int>>(perf);
        // perf.end("atomic_stack");
        // perf.begin("atomic_queue");
        test_sync<utils::LockFreeQueue<int>>(perf);
        // perf.end("atomic_queue");
        // perf.begin("pool");
        // test_pool();
        // perf.end("pool");

        // cperf::read_nodes(perf.dump(), [](cperf::_Key name, cperf::_ListNode<cperf::PerfNode> const* head) {
        //     auto time = head->val.end - head->val.start;
        //     std::cout << name << " " << cperf::time_cast<std::chrono::milliseconds>(time) << ", ";
        // });
        std::cout << "tested " << i << " \n";

    }
    std::cout << "finished\n";
}