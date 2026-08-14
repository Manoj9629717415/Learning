#include <iostream>
#include <future>
#include <chrono>
#include <thread>


void worker1(int a, int b,std::promise<int> p)
{
    try{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (b == 0)
        {
            throw std::runtime_error("runtime error ocured ");
        }
        int c = a * b;
        p.set_value(c);
    }
    catch(...){
        p.set_exception(std::current_exception());
    }
}


int worker2(int a, int b)
{
    std::cout<<"worker2 execution Started"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (b == 0)
    {
        throw std::runtime_error("runtime error ocured ");
    }
    int c = a * b;
    return c;
    
}

int calculate(int a,int b)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    if(b == 0)
    {
        throw std::runtime_error("invlaid value");
    }

    return a / b;

}


int main()
{
    // std::promise<int> p;
    // std::future<int> f = p.get_future();
    int a{25},b{4};

    // std::thread w1(worker1,a,b,std::move(p));

    // int result = f.get();

    // std::cout<<" result "<<result<<std::endl;

    // w1.join();

    // b = 0;
    // std::promise<int> p1;
    // std::future<int> f1 = p1.get_future();

    // std::thread w2(worker1,a,b,std::move(p1));

    // try
    // {
    //     std::cout<<" result "<<f1.get();
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }
    
    // w2.join();

    // auto f = std::async(std::launch::deferred,worker2,a,b);

    // std::cout<< "Main continues "<<std::endl;

    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout<< "After sleep "<<std::endl;

    // int result = f.get();

    // std::cout<< "result "<< result<<std::endl;

    // auto f1 = std::async(std::launch::async,worker2,a,0);
    // std::cout<< "Main continues "<<std::endl;

    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout<< "After sleep "<<std::endl;


    // try{
    //     int result = f1.get();

    //     std::cout<< "result "<< result<<std::endl;
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr<<e.what()<<'\n';
    // }

    std::packaged_task<int(int,int)> task(calculate);
    std::future<int> f2 = task.get_future();

    std::thread t(std::move(task),20,10);

    int result = f2.get();

    std::cout<<" result "<<result<<std::endl;

    t.join();

    std::packaged_task<int(int,int)> task1(calculate);

    std::future<int> f3 = task1.get_future();

    std::thread t1(std::move(task1),20,0);

    try
    {
        int result1 = f3.get();

        std::cout<<" result "<<result1<<std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
    }
    t1.join();

    std::packaged_task task3(calculate);
    std::future<int> f4 = task3.get_future();

    task3(10,2);
    int result3 = f4.get();

    std::cout<<" result 3 = "<<result3<<std::endl;


}
