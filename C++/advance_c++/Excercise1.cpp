#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <map>
#include <algorithm>
#include <atomic>
#include <cstring>
#include <future>


class Print {
   public:
    Print(int m) : max{m}, counter{0} {}

    void print_even() {
        while (true) {
            std::unique_lock<std::mutex> ul(m);
            cv.wait(ul, [&]() { return (counter % 2 == 0 || counter > max); });
            if (counter > max) {
                cv.notify_one();
                break;
            }
            std::cout << counter << std::endl;
            ++counter;
            cv.notify_one();
        }
    }

    void print_odd() {
        while (true) {
            std::unique_lock<std::mutex> ul(m);
            cv.wait(ul, [&]() { return (counter % 2 != 0 || counter > max); });
            if (counter > max) {
                cv.notify_one();
                break;
            }
            std::cout << counter << std::endl;
            ++counter;
            cv.notify_one();
        }
    }

   private:
    std::mutex m;
    std::condition_variable cv;
    int counter;
    int max{100};
};

class PC {
   public:
    PC(int c) : count{c} {}

    void producer(const std::string_view& thread_name) {
        while (true) {
            std::unique_lock<std::mutex> ul(_m);
            cv.wait(ul, [&]() {
                return (q.empty() || (!q.empty() && q.front() > max));
            });
            if (!q.empty() && q.front() > max) {
                cv.notify_all();
                break;
            }
            q.push(count);
            std::cout << thread_name << "produces " << count << std::endl;
            ++count;
            cv.notify_all();
        }
    }

    void consumer(const std::string_view& thread_name) {
        while (true) {
            std::unique_lock<std::mutex> ul(_m);
            cv.wait(ul, [&]() {
                return (!q.empty() || (!q.empty() && q.front() > max));
            });
            if (q.front() > max) {
                cv.notify_all();
                break;
            }
            int value = q.front();
            std::cout << thread_name << "consumed " << value << std::endl;
            q.pop();
            cv.notify_all();
        }
    }

   private:
    std::queue<int> q;
    std::mutex _m;
    std::condition_variable cv;
    int max{300};
    int count{100};
};

class RW {
   public:
    RW(int c) : count{c} {}

    void write(std::string thread_name) {
        while (true) {
            std::unique_lock<std::shared_mutex> ul(_m);
            if (v.size() > max) {
                break;
            }
            v.push_back(count);
            std::cout << thread_name << " produces " << count << std::endl;
            ++count;
            ul.unlock();
            std::this_thread::yield();
        }
    }

    void reader(std::string thread_name) {
        while (true) {
            std::shared_lock<std::shared_mutex> ul(_m);
            if (v.size() > max) {
                break;
            }
            if (!v.empty()) {
                int value = *(v.end() - 1);
                std::cout << thread_name << " consumed " << value << std::endl;
            }
            ul.unlock();
            std::this_thread::yield();
        }
    }

   private:
    std::vector<int> v;
    std::shared_mutex _m;
    int max{100};
    int count{100};
};

// diner's

struct Forks {
    std::mutex m_fork;
};

void philospher(int id, Forks& left, Forks& right) {
    for (int i = 0; i < 1; i++) {
        std::cout << "" << std::endl;
        std::cout << id << " Thinking ........" << std::endl;
        std::scoped_lock lock(left.m_fork, right.m_fork);
        std::cout << id << " Eating ........." << std::endl;
        std::cout << "" << std::endl;
    }
    std::cout << id << " Done ........." << std::endl;
}

// meyers singleton , lazy initalization

class Meyers_singleton {
   public:
    Meyers_singleton(const Meyers_singleton&) = delete;
    Meyers_singleton& operator=(const Meyers_singleton&) = delete;

    static Meyers_singleton& getInstance() {
        static Meyers_singleton instance;
        return instance;
    }

    void print() {
        ++data;
        std::cout << "I am meyers singleton " << data << std::endl;
    }

   private:
    Meyers_singleton() = default;
    ~Meyers_singleton() = default;
    int data{10};
};

class Smart_Singleton {
   public:
    static Smart_Singleton& getInstance() {
        std::call_once(flag, [&]() { instance.reset(new Smart_Singleton()); });
        return *instance;
    }

   private:
    static std::unique_ptr<Smart_Singleton> instance;
    static std::once_flag flag;
    Smart_Singleton() = default;
};

std::unique_ptr<Smart_Singleton> Smart_Singleton::instance = nullptr;
std::once_flag Smart_Singleton::flag;

class RawSingleton {
   private:
    static RawSingleton* instance;
    static std::mutex mtx;
    RawSingleton() = default;
    ~RawSingleton(){
        delete instance;
    }

   public:
    static RawSingleton* getInstance() {
        if (instance == nullptr) {  // First check
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) {  // Second check
                instance = new RawSingleton();
            }
        }
        return instance;
    }
};
RawSingleton* RawSingleton::instance = nullptr;
std::mutex RawSingleton::mtx;


class ThreadPool{

public:

    explicit ThreadPool(int num_workers = std::thread::hardware_concurrency()){
        for(int i=0 ; i< num_workers;i++)
        {   
            m_workers.emplace_back([this,i](){
                worker_loop(i);
            });
        }
    }

    void push(std::function<void()> task){
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_tasks.push(std::move(task));
        }
        m_cv.notify_one(); 
    }

    ~ThreadPool(){
        shutdown_call();

        for(std::thread& worker : m_workers){
            worker.join();
        }
    }

    void shutdown_call(){
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stop_flag = true;
        }
        m_cv.notify_all();

    }

private:

    void worker_loop(int i_th_worker){
        while(!m_stop_flag){
            std::function<void()> item;
            {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            m_cv.wait(lock,[this](){
                return !m_tasks.empty() || m_stop_flag;
            });
            if(m_stop_flag){
                m_cv.notify_all();
                break;
            }
            item = m_tasks.front();
            std::cout <<" Executed by worker " << i_th_worker << std::endl;
            m_tasks.pop();
            m_cv.notify_all();
            }
            item();
        }
    }

private:
    std::mutex m_mutex{};
    std::condition_variable m_cv{};
    std::queue<std::function<void()>> m_tasks{};
    std::vector<std::thread> m_workers{};
    std::atomic<int> m_stop_flag{};
};

#include <stdexcept>

class Pool {
public:
    Pool(int num_workers) : m_thread_stop(false) {
        for (int i = 0; i < num_workers; ++i) {
            m_workers.emplace_back([this] { worker_loop(); });
        }
    }

    void enqueue(std::function<void()> function) {
        {
            std::unique_lock<std::mutex> ul(m_mutex);
            if (m_thread_stop) {
                throw std::runtime_error("enqueue on stopped pool");
            }
            m_tasks.push(std::move(function));
        }
        m_cv.notify_one();
    }

    ~Pool() {
        {
            std::unique_lock<std::mutex> ul(m_mutex);
            m_thread_stop = true;
        }

        m_cv.notify_all();

        for (std::thread& task : m_workers) {
            if (task.joinable()) {
                task.join();
            }
        }
    }

private:
    void worker_loop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> ul(m_mutex);
                m_cv.wait(ul, [this] {
                    return m_thread_stop || !m_tasks.empty();
                });

                if (m_thread_stop && m_tasks.empty()) {
                    return;
                }

                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
            task();
        }
    }

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    bool m_thread_stop;
};

class FizzBuzz
{
private:
    int m_max;
    int m_current;
    std::mutex m_mutex;
    std::condition_variable m_cv;

public:
    FizzBuzz(int max) : m_max{max}, m_current{1} {}

    void run(std::function<bool(int)> validator, std::function<void(int)> action)
    {
        while(true)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock,[&](){
                return m_current > m_max || validator(m_current);
            });
            if(m_current > m_max)
            {
                m_cv.notify_all();
                break;
            }
            action(m_current);
            ++m_current;
            m_cv.notify_all();
        }
    }
};

enum class VehicleType{
    Bike,
    Car,
    Truck
};

class Vehicle {
    int num_seats{};
    int num_mirror{};
public:
    Vehicle(int seats,int mirrors) : num_seats{seats}, num_mirror{mirrors}{

    }
    virtual void make_people_sit(int num_people) = 0;
    virtual void adjust_mirrors(bool is_left) = 0;
    virtual ~Vehicle() = default;
};

class Truck : public Vehicle{

public:

    Truck(int seats, int mirrors) : Vehicle(seats,mirrors){}
    void make_people_sit(int num_people) override{
        if(num_people > 10)
        {
            std::cout << "Full cannot sit anymore" << std::endl;
        }
    }
    void adjust_mirrors(bool is_left) override{
        if(is_left)
        {
            std::cout << "Left mirror adjusted" << std::endl;
        }
        else{
            std::cout << "Right mirror adjusted" << std::endl;
        }
    }
};

class Bike : public Vehicle{
public:

    Bike(int seats, int mirrors) : Vehicle(seats,mirrors){}
    void make_people_sit(int num_people) override{
        if(num_people > 2)
        {
            std::cout << "Full cannot sit anymore" << std::endl;
        }
    }
    void adjust_mirrors(bool is_left) override{
        if(is_left)
        {
            std::cout << "Left mirror adjusted" << std::endl;
        }
        else{
            std::cout << "Right mirror adjusted" << std::endl;
        }
    }
};

class Car : public Vehicle{

public:

    Car(int seats, int mirrors) : Vehicle(seats,mirrors){}
    void make_people_sit(int num_people) override{
        if(num_people > 4)
        {
            std::cout << "Full cannot sit anymore" << std::endl;
        }
    }
    void adjust_mirrors(bool is_left) override{
        if(is_left)
        {
            std::cout << "Left mirror adjusted" << std::endl;
        }
        else{
            std::cout << "Right mirror adjusted" << std::endl;
        }       
    }
};

class Factory{

public:
    static std::unique_ptr<Vehicle> create(VehicleType vehicle,int num_people, int num_mirrors){
        switch(vehicle){
            case VehicleType::Bike : return std::make_unique<Bike>(num_people,num_mirrors);
            case VehicleType::Car : return std::make_unique<Car>(num_people,num_mirrors);
            case VehicleType::Truck : return std::make_unique<Truck>(num_people,num_mirrors);
            default: return nullptr;
        }
        return nullptr;
    }

};

//Abstract factory 
class AbstractFactory{
public:
    virtual std::unique_ptr<Vehicle> create( int num_people, int num_mirrors ) = 0;
    virtual ~AbstractFactory() = default;
};

class BikeFactory : public AbstractFactory{

public:
    std::unique_ptr<Vehicle> create(int num_people, int num_mirrors) override{
        return std::make_unique<Bike>(num_people,num_mirrors);
    }

};

class CarFactory : public AbstractFactory{

public:
    std::unique_ptr<Vehicle> create(int num_people, int num_mirrors) override{
        return std::make_unique<Car>(num_people,num_mirrors);
    }

};

class TruckFactory : public AbstractFactory{

public:
    std::unique_ptr<Vehicle> create( int num_people, int num_mirrors) override{
        return std::make_unique<Truck>(num_people,num_mirrors);
    }

};

class VehicleCreator{
    private:
    std::map<VehicleType,std::unique_ptr<AbstractFactory>> m_vehicle_f_map;
    public:
    VehicleCreator() {
        m_vehicle_f_map[VehicleType::Bike] = std::make_unique<BikeFactory>();
        m_vehicle_f_map[VehicleType::Car] = std::make_unique<CarFactory>();
        m_vehicle_f_map[VehicleType::Truck] = std::make_unique<TruckFactory>();
    }

    std::unique_ptr<Vehicle> create(VehicleType type, int num_people, int num_mirrors) {
        return m_vehicle_f_map[type]->create(num_people, num_mirrors);
    }
};

class Observer{

public:
    virtual void update(int data) = 0;
    virtual ~Observer() = default;
};

class Subject{
public:
    virtual void register_observer(Observer* ob) = 0;
    virtual void remove_observer(Observer* ob) = 0;
    virtual void notify_observer() = 0;
    virtual ~Subject() = default;
    
};

class Weclass : public Subject{
private:
    std::vector<Observer*> observers;
    int data;
public:
    void register_observer(Observer* ob) override{
        observers.push_back(ob);
    }
    void remove_observer(Observer* ob) override{
        observers.erase(std::remove(observers.begin(), observers.end(), ob), observers.end());
    }
    void notify_observer() override{
        for(auto obser : observers ){
            obser->update(data);
        }
    }

    void set_values(int data){
        this->data = data;
        notify_observer();
    }

};

class Display : public Observer{
public:
    void update(int data) override{
        std::cout << data << std::endl;
    }

};

// use of std::forward 
void result(int& arg){
    std::cout << "Lvalue result" << std::endl;
}
void result(int&& arg){
    std::cout << "Rvalue result" << std::endl;
}

template <typename T>
void process(T&& arg){
    result(std::forward<T>(arg));
}

template <typename T>
class Unique_ptr
{
    T* ptr;
public:
    Unique_ptr(const Unique_ptr& ptr) = delete;
    Unique_ptr& operator=(const Unique_ptr& ptr) = delete;

    Unique_ptr(T* temp_ptr= nullptr) : ptr{temp_ptr}{

    }
    Unique_ptr(Unique_ptr&& temp_ptr) : ptr{temp_ptr.release()}{}

    Unique_ptr& operator =(Unique_ptr&& temp_ptr){
        if(this != &temp_ptr){
            reset(temp_ptr.release());
        }
        return *this;
    }

    ~Unique_ptr(){
        delete ptr;
    }

    T* release(){
        T* copy = ptr;
        ptr = nullptr;
        return copy;
    }

    void reset(T* temp= nullptr){
        delete ptr;
        ptr = temp;
    }

    T* get()const{
        return ptr;
    }

    T& operator *() const {
        return *ptr;
    }

    T* operator ->() const{
        return ptr;
    }
    explicit operator bool() const{
        return (ptr != nullptr);
    }
};

template <typename T>
class Shared_ptr
{
    T* ptr;
    std::atomic<int>* count;

    void cleanup(){
        if(count && count->fetch_sub(1) == 1){
            delete ptr;
            delete count;
        }
    }
public:
    Shared_ptr(const Shared_ptr& temp_ptr): ptr{temp_ptr.ptr}, count{temp_ptr.count}{
        if(count){
            count->fetch_add(1);
        }
    }
    Shared_ptr& operator=(const Shared_ptr& temp_ptr){
        if(this != &temp_ptr){
            cleanup();
            ptr = temp_ptr.ptr;
            count = temp_ptr.count;
            if(count){
                count->fetch_add(1);
            }
            return *this;
        }
        return *this;
    }

    explicit Shared_ptr(T* temp_ptr= nullptr) : ptr{temp_ptr},count{temp_ptr ? std::atomic<int>(1) : nullptr}{

    }
    Shared_ptr(Shared_ptr&& temp_ptr) : ptr{temp_ptr.ptr}, count {temp_ptr.count}{
        temp_ptr.ptr = nullptr;
        temp_ptr.count = nullptr;
    }

    ~Shared_ptr(){
        cleanup();
    }

    int use_count() const { return count ? count->load(): 0; } 
    

    T* get()const{
        return ptr;
    }

    T& operator *() const {
        return *ptr;
    }

    T* operator ->() const{
        return ptr;
    }
    explicit operator bool() const{
        return (ptr != nullptr);
    }
};

class ThatClass{
public:
    ThatClass(const char* n, int a, int u_value, char s_value){
        name = new char[strlen(n) + 1];
        strcpy(name,n);
        age = new int(a);
        u_ptr = std::make_unique<int>(u_value);
        s_ptr = std::make_shared<char>(s_value);
    }

    ThatClass(const ThatClass& other){
        name = new char[strlen(other.name) + 1];
        strcpy(name,other.name);
        age = new int(*other.age);
        u_ptr = std::make_unique<int>(*other.u_ptr);
        s_ptr = other.s_ptr;
    }

    ThatClass& operator =(const ThatClass& other){
        if(this == &other){
            return *this;
        }
        delete[] name;
        delete age;
        name = new char[strlen(other.name) + 1];
        strcpy(name,other.name);
        age = new int(*other.age);
        u_ptr = std::make_unique<int>(*other.u_ptr);
        s_ptr = other.s_ptr;
        return *this;
    }

    ThatClass(ThatClass&& other): name{other.name}, age{other.age}, u_ptr{std::move(other.u_ptr)}, s_ptr{std::move(other.s_ptr)}{
        other.name = nullptr;
        other.age = nullptr;
    }

    ThatClass& operator =(ThatClass&& other){
        if(this == &other){
            return *this;
        }
        delete[] name;
        delete age;
        name = other.name;
        age = other.age;
        u_ptr = std::move(other.u_ptr);
        s_ptr = std::move(other.s_ptr);
        return *this;
    }


private:
    char *name;
    int* age;
    std::unique_ptr<int> u_ptr;
    std::shared_ptr<char> s_ptr;
};

void producer(std::promise<int> p){
    try{
        int result = 42;
        p.set_value(result);
    }
    catch(...)
    {
        p.set_exception(std::current_exception());
    }
}

int compute(int x)
{
    return x*x;
}


int main() {

    std::mutex m_mutex;
    std::unique_lock<std::mutex> lock(m_mutex,std::defer_lock);

    lock.lock();
    lock.unlock();
    if(lock.try_lock())
    {

    }
    lock.unlock();

    std::promise<int> p,p1;

    std::future<int> f = p.get_future();
    //shared future
    std::shared_future<int> f3 = p1.get_future().share();   

    std::future<int> f1 = std::async(std::launch::async,compute,10);

    f1.wait();

    std::cout << "My my " << std::endl;
    int another = f1.get();

    std::thread t1(producer,std::move(p));

    int value = f.get();
    t1.join();



/*
   
    std::vector<int> v{1,2,1,2,5,3,7,5};

    std::for_each(v.begin(),v.end(),[](int& n){
        n = n*2;
    });

    std::for_each(v.begin(),v.end(),[](int n){
        std::cout << "N value" << n << std::endl;
    });

    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());

    std::for_each(v.begin(),v.end(),[](int n){
        std::cout << "unique value " << n << std::endl;
    });

  
    int a = 10;

    process(10);
    process(a);


    // Observer
    Weclass sub;
    Display disp1;
    Display disp2;

    sub.register_observer(&disp1);
    sub.register_observer(&disp2);

    sub.set_values(1);
    sub.set_values(33);

    sub.remove_observer(&disp1);
    sub.set_values(323);

    //Abstract Factory
    //provides an interface for creating families of related or dependent objects without specifying their concrete classes
    std::unique_ptr<VehicleCreator> creator = std::make_unique<VehicleCreator>();
    std::unique_ptr<Vehicle> v = creator->create(VehicleType::Bike,10,2);
    v->adjust_mirrors(false);
    v->make_people_sit(78);

    //Factory Method
    // provides an interface for creating objects of a superclass, but allows subclasses to alter the type of objects that will be created
    std::unique_ptr<Vehicle> v=Factory::create(VehicleType::Bike,2,2);
    v->make_people_sit(10);
    v->adjust_mirrors(true);


    FizzBuzz f(15);

    std::thread t1(&FizzBuzz::run,&f,
    [](int i){ return i%3 == 0 && i%5 != 0; },
    [](int i){ std::cout << "Fizz" << std::endl;});

    std::thread t2(&FizzBuzz::run,&f,
    [](int i){ return i%5 == 0 && i%3 != 0; },
    [](int i){ std::cout << "Buzz" << std::endl;});

    std::thread t3(&FizzBuzz::run,&f,
    [](int i){ return i%3 == 0 && i%5 == 0; },
    [](int i){ std::cout << "FizzBuzz" << std::endl;});

    std::thread t4(&FizzBuzz::run,&f,
    [](int i){ return i%3 != 0 && i%5 != 0; },
    [](int i){ std::cout << i << std::endl;});

    t1.join();
    t2.join();
    t3.join();
    t4.join();


    Pool pool(10);

    for (int i = 0; i < 8; i++) {
        pool.enqueue([i]() {
            std::cout << i << " Worker is here "
                      << std::this_thread::get_id() << std::endl;
        });
    }


 

        RawSingleton* instance = RawSingleton::getInstance();

        Meyers_singleton& instance = Meyers_singleton::getInstance();
        instance.print();
        Meyers_singleton& instance1 = Meyers_singleton::getInstance();
        instance1.print();

        constexpr int num_p{5};
        std::vector<Forks> forks(num_p);
        std::vector<std::thread> philosphers;

        for(int i =0 ; i < num_p ; ++i)
        {
            philosphers.emplace_back(philospher,i,std::ref(forks[i]),
       std::ref(forks[(i+1) % num_p]));
        }

        for(auto& i : philosphers)
        {
            i.join();
        }

        Print p(200);
        std::thread t1(&Print::print_even,&p);
        std::thread t2(&Print::print_odd,&p);

        t1.join();
        t2.join();

        std::vector<std::thread> v_t;
        RW r_w(200);

        PC p1(200);
        std::thread t3(&PC::consumer,&p1,"t3");
        std::thread t9(&PC::consumer,&p1,"t9");
        std::thread t12(&PC::consumer,&p1,"t12");
        std::thread t13(&PC::consumer,&p1,"t13");
        std::thread t14(&PC::consumer,&p1,"t14");
        std::thread t15(&PC::consumer,&p1,"t15");
        std::thread t4(&PC::producer,&p1,"t4");
        std::thread t5(&PC::producer,&p1,"t5");
        std::thread t6(&PC::producer,&p1,"t6");
        std::thread t7(&PC::producer,&p1,"t7");
        std::thread t8(&PC::producer,&p1,"t8");
        std::thread t10(&PC::producer,&p1,"t10");


        t3.join();
        t4.join();
        t9.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t10.join();
        t12.join();
        t13.join();
        t14.join();
        t15.join();

        for(int i=0;i< 10;i++)
        {
            v_t.emplace_back(&RW::write,&r_w,std::to_string(i));
            v_t.emplace_back(&RW::reader,&r_w,std::to_string(i));
        }

        for(auto &i : v_t)
        {
            i.join();
        }*/
        return 0;
}
