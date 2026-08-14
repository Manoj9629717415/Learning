#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>


class ThreadPool{
public:
    explicit ThreadPool(size_t num_threads)
    {
        for(int i = 0;i<num_threads;i++)
        {
            m_workers.emplace_back([this]{
                worker();
            });
        }

    }

    void worker()
    {
        while(true)
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_cv.wait(lock,[this]{
                return !m_q.empty() || m_stop;
            });

            if(m_q.empty() && m_stop)
            {
                // return true;
                std::cout<<" break called"<<std::endl;
                break;
            }

            auto task = std::move(m_q.front());
            m_q.pop();
            lock.unlock();
            task();
        }
    }

    template<typename F,typename... Args>
    auto submit(F&& function,Args&&... args)
    {
        // auto callable = [function,args]()
        //                 {
        //                     return std::forward<F>function(std::forward<Args>args);
        //                 };

        auto callable = std::bind(
                        std::forward<F>(function),
                        std::forward<Args>(args)...
                    );
        
        using ReturnType  = std::invoke_result_t<F,Args...>;

        std::packaged_task<ReturnType()> task(std::move(callable));

         auto f = task.get_future();

        std::packaged_task<void()> wrapper([t = std::move(task)]() mutable {
            t();
        });
       
        {
            std::lock_guard<std::mutex> lock(m_queue_mutex);
            m_q.push(std::move(wrapper));
        }
        m_cv.notify_one();
        return f;
    }

    ~ThreadPool(){
        std::cout<<" destructor called"<<std::endl;
        {
            std::lock_guard<std::mutex> lock(m_queue_mutex);
            m_stop = true;
        }
        m_cv.notify_all();
        for (auto& th : m_workers)
        {
            th.join();
        }
    }

private:
    std::mutex m_queue_mutex;
    std::condition_variable m_cv;
    std::queue<std::packaged_task<void()>> m_q{};
    bool m_stop{false};
    std::vector<std::thread> m_workers;
};


int main()
{
    {
    ThreadPool pool(3);
    

    auto f1 = pool.submit([](int a ,int b)->int{
        std::cout<<"thread ID "<<std::this_thread::get_id()<<std::endl;
        return a * b;
    },10,5);

    auto f2 = pool.submit([](std::string oem = "Audi")-> std::string{
        std::cout<<"thread ID "<<std::this_thread::get_id()<<std::endl;
        return "welcome "+ oem;
    });

    auto f3 = pool.submit([]()-> int{
        std::cout<<"thread ID "<<std::this_thread::get_id()<<std::endl;
        throw std::runtime_error("no arguments");
        return 100;
    });

    try
    {
        auto result1 = f1.get();
        std::cout<<" result1 "<<result1<<std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
     try
    {
        auto result2 = f2.get();
        std::cout<<" result2 "<<result2<<std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
     try
    {
        auto result3 = f3.get();
        std::cout<<" result3 "<<result3<<std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    }
    std::cout<<" scope exit"<<std::endl;
    return 0;

}