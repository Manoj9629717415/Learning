#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

struct VehicleConfig
{
    int speed;
    int rpm;
};



class VehicleConfigManager{
public:
    VehicleConfig getconfig() const
    {
        std::shared_lock<std::shared_mutex> lock(m_smx);
        return m_config;
    }

    void update_config(VehicleConfig config)
    {
        std::unique_lock<std::shared_mutex> lock(m_smx);
        m_config.rpm = config.rpm;
        m_config.speed = config.speed;
    }

private:
    mutable std::shared_mutex m_smx;
    VehicleConfig m_config{100, 6000};
};


int main()
{
    VehicleConfigManager manager;
    std::atomic<bool> shutdown{false};

    auto reader = [&manager,&shutdown](){
        while(!shutdown.load())                     
        {
            VehicleConfig config = manager.getconfig();
            std::cout<<" speed "<<config.speed<<" rpm "<<config.rpm<<" ID "<<std::this_thread::get_id()<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    std::vector<std::thread> reader_thread;

    for(int i=0;i<5;i++)
    {
        reader_thread.emplace_back(reader);
    }

    auto writer = [&manager]() mutable{
        VehicleConfig config{100,6000};
        for(int i = 0;i<10;i++)
        {
            config.speed += 20;
            config.rpm += 500;
            manager.update_config(config);
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    };

    std::thread writer_thread(writer);

    writer_thread.join();

    shutdown.store(true);

    for(auto& th : reader_thread)
    {
        th.join();
    }

    return 0;
}