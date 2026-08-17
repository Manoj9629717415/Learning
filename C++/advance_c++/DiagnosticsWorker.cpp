#include <iostream>
#include <condition_variable>
#include <vector>
#include <thread>
#include <queue>
#include <chrono>



class DiagnosticsWorker{
public:
    DiagnosticsWorker():m_worker([this](std::stop_token token){
        start(token);
    }){}


    void start(std::stop_token token){
        while(!token.stop_requested())
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock,token,[this](){
                return !m_messages.empty();
            });
            if(token.stop_requested() && m_messages.empty())
            {
                break;
            }

            int message = std::move(m_messages.front());
            m_messages.pop();
            lock.unlock();
            std::cout<<" message "<<message<<std::endl;
        }
 
    }

    void submit_message(int message){
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_messages.push(message);
        }
        m_cv.notify_one();
    }

private:
    std::condition_variable_any m_cv;
    std::mutex m_mutex;
    std::queue<int> m_messages;
    std::jthread m_worker;

};

int main()
{
    DiagnosticsWorker worker;

    std::jthread t([&worker](std::stop_token token){
        int i = 0;
        while(!token.stop_requested())
        {
            worker.submit_message(i);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            i++;
        }    
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));

    t.request_stop();




}
