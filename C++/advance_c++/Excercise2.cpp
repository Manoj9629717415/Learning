#include<vector>
#include<condition_variable>
#include<mutex>
#include<iostream>
#include<thread>


class ThreadRW {
public:
    void reader() {
        while (true) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]() {
                return !m_data.empty() || m_done;
            });

            while (!m_data.empty()) {
                int value = m_data.back();
                m_data.pop_back();
                std::cout << "Reader " << value << std::endl;
                if (value == 99) {
                    return;
                }
            }

            if (m_done && m_data.empty()) {
                return;
            }
        }
    }

    void writer() {
        for (int i = 0; i < max; i++) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_data.push_back(i);
                std::cout << "Writer: " << i << std::endl;
            }
            m_cv.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_done = true;
        }
        m_cv.notify_all();
    }

private:
    std::vector<int> m_data;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_done{false};
    int max = 100;
};

int main(){

    ThreadRW rw;
    std::thread reader(&ThreadRW::reader,&rw);
    std::thread writer(&ThreadRW::writer,&rw);

    if(reader.joinable()){
        reader.join();
    }

    if(writer.joinable()){
        writer.join();
    }

    return 0;
}
