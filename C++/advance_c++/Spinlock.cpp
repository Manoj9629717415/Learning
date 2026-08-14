#include <iostream>
#include <atomic>
#include <vector>
#include <thread>



class Spin_Lock{
public:
    Spin_Lock() = default;
    Spin_Lock(const Spin_Lock&) = delete;
    Spin_Lock& operator =(const Spin_Lock&) = delete;
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }

    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag{ATOMIC_FLAG_INIT};

};

class Spinlock_gurad{
public :
    explicit Spinlock_gurad(Spin_Lock& lock) : m_lock(lock){
        m_lock.lock();
    }

    ~Spinlock_gurad(){
        m_lock.unlock();
    }

    Spinlock_gurad(const Spinlock_gurad&) = delete;
    Spinlock_gurad& operator = (const Spinlock_gurad&) = delete;

private:
    Spin_Lock& m_lock;
};

void worker1(long& counter,Spin_Lock& spin_lock)
{
    for(int i = 0;i<10000;i++)
    {
        Spinlock_gurad lock(spin_lock);
        ++counter;
    }
}


int main()
{
    Spin_Lock lock;
    long counter{0};

    std::vector<std::thread> counter_threads;

    for(int i = 0;i<10;i++)
    {
        counter_threads.emplace_back(worker1,std::ref(counter),std::ref(lock));
    }

    for(auto& th : counter_threads)
    {
        th.join();
    }

    std::cout<<"counter "<<counter<<std::endl;

}