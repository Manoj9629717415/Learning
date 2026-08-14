#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>

std::mutex worker3_mu;

void worker1(int a)
{
    std::cout<<"worker1 A = "<<a<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
}

void worker2(int a,int b,int& result)
{
    result = a+b;
    std::cout<<"worker2 result = "<<result<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
}

void worker3(int& counter)
{
    std::cout<<"worker3 before lock = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
    std::lock_guard<std::mutex> lock(worker3_mu);
    ++counter;
    std::cout<<"worker3 counter = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"worker3  after lock = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
}

void worker4(int& counter)
{
    std::cout<<"worker4 before lock = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
    std::unique_lock<std::mutex> lock(worker3_mu);
    ++counter;
    std::cout<<"worker4 counter = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    lock.lock();
    std::cout<<"worker4  after lock = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
}

void worker5(std::atomic<int>& counter)
{
    int count = 0;
    while(count < 1000)
    {
        ++counter;
        // std::cout<<"worker5 counter = "<<counter<<" thread_id = "<<std::this_thread::get_id()<<std::endl;
        ++count;
    }
}

void woker6(std::atomic<bool>& running)
{
    std::cout<<"before exchange running = "<<running<<std::endl;
    bool expected = false;
    bool result = running.compare_exchange_strong(expected,true);
    std::cout<<"After exchange running result = "<<result<< " running = "<<running<<" expected = "<<expected<<std::endl;
}

void producer(int& x,std::atomic<bool>& flag)
{
    x = 100;
    // flag.store(true,std::memory_order_relaxed);
    flag.store(true,std::memory_order_release);
}

void consumer1(int& x,std::atomic<bool>& flag)
{
    // while(!flag.load(std::memory_order_relaxed))
    // {

    // }
    while(!flag.load(std::memory_order_acquire))
    {

    }
    std::cout<<"consumer 1 x= "<<x<<std::endl;
}

void consumer2(int& x,std::atomic<bool>& flag)
{
    // while(!flag.load(std::memory_order_relaxed))
    // {

    // }
    while(!flag.load(std::memory_order_acquire))
    {

    }
    std::cout<<"consumer 2 x= "<<x<<std::endl;
}

bool increment_if_less_than_100(std::atomic<int>& counter)
{
    int expected = counter.load();

    while(expected < 100)
    {
        if(counter.compare_exchange_weak(expected,expected+1))
        {
            return true;
        }
    }

    return false;
}

void worker7(std::atomic<int>& counter,int& ind_countrer)
{
    for(int i = 0;i <100 ; i++)
    {
        if (increment_if_less_than_100(counter))
        {
            ++ind_countrer;
        }
    }
}

int main()
{
    int a{10},b{20},counter{0};
    // std::thread w1(worker1,a);
    std::atomic<int> atmoic_counter{0};

    // std::thread w2(worker2,a,b,std::ref(result));


    std::vector<std::thread> counterthread;

    // for(int i=0;i<10;i++)
    // {
    //     counterthread.emplace_back(worker3,std::ref(counter));
    // }
    
    // for(int i=0;i<10;i++)
    // {
    //     counterthread.emplace_back(worker4,std::ref(counter));
    // }

    // for(int i=0;i<10;i++)
    // {
    //     counterthread.emplace_back(worker5,std::ref(atmoic_counter));
    // }

    // std::atomic<bool> running{false};
    // std::vector<std::thread> atomic_threads;
    // for(int i = 0;i<10;i++)
    // {
    //     atomic_threads.emplace_back(woker6,std::ref(running));
    // }

    // int x = 0;
    // std::atomic<bool> ready{false};

    // std::thread p(producer,std::ref(x),std::ref(ready));
    // std::thread con1(consumer1,std::ref(x),std::ref(ready));
    // std::thread con2(consumer2,std::ref(x),std::ref(ready));

    std::atomic<int> g{150};

    int expected = 100;

    bool result = g.compare_exchange_weak(expected, 200);

    std::cout<<"compare_exchange_weak result = "<<result<<" expected = "<<expected<< " g = "<<g<<std::endl;

    std::atomic<int> cas_loop_counter{0};
    std::vector<int> indidualcounter(20,0);

    for(int i=0;i<20;i++)
    {
        counterthread.emplace_back(worker7,std::ref(cas_loop_counter),std::ref(indidualcounter[i]));
    }



    // p.join();
    // con1.join();
    // con2.join();

    // w1.join();
    // w2.join();

    for(auto& th : counterthread)
    {
        th.join();
    }

    //  for(auto& th : atomic_threads)
    // {
    //     th.join();
    // }

     std::cout<<"cas_loop_counter "<<cas_loop_counter<<std::endl;
    for(int i = 0;i <20;i++)
    {
        std::cout<<"thread "<<i<< "incremeneted "<<indidualcounter[i]<<" times"<<std::endl;
    }

}