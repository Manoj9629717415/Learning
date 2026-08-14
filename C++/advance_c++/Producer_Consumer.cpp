
template<typename T>
class Observer{
    Observer(const Observer& other) = delete;
    Observer& operator=(const Observer& other) = delete;
    Observer(Observer&& other) = delete;
    Observer& operator=(Observer&& other)=delete;
    ~Observer() = default;

    virtual void register_observer() = 0;

    virtual void notify(T data) = 0;

};

struct SpeedData{
    int speed;
    int rpm;
};

template<typename T>
class Producer{
public:
    void registry(Observer<T> observer_obj)
    {
        m_consumer_list.push_back(observer_obj);
    }

    void update(T data)
    {
        if(!m_consumer_list.empty())
        {
            for(const auto& listners : m_consumer_list)
            {
                listners.notify(data);
            }
       }
    }

private:

    std::vector<Observer<T>> m_consumer_list;

};

enum class Dataclassifier{
    SPEED,
    GEAR
};

class ProducerFactory{
    public:
        static ProducerFactory* getInstance(){
            if(instance == nullptr)
            {
                std::lock_guard<std::mutex> lock(mtx);
                if(instance == nullptr)
                {
                    instance = new ProducerFactory();
                }
            }
            return instance;
        }

        template<typename T>
        Producer<T> getProducerInstance(Dataclassifier classifier)
        {
            switch(classifier)
            {
                case Dataclassifier::SPEED:
                {
                    if(speeddata_producer == nullptr)
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        speeddata_producer = new Producer<SpeedData>();
                    }
                    return speeddata_producer;
                }
            }
            return nullptr;
        }

    private:
        ProducerFactory() = default;
        ~ProducerFactory() = default;
        static ProducerFactory* instance;
        static std::mutex mtx;
        Producer<SpeedData>* speeddata_producer;
};
ProducerFactory* ProducerFactory::instance = nullptr;
std::mutex ProducerFactory::mtx;

template<typename SpeedData>
class Cluster_Consumer : public Observer{

       void register_observer() override{
            ProducerFactory::getInstance()->getProducerInstance()->registry(this);
       }

    void notify(T data) override{
        std::cout<<"data received for Cluster speed "<<data.speed;
    }

};

template<typename SpeedData>
class HU_Consumer : public Observer{

       void register_observer() override{
            ProducerFactory::getInstance()->getProducerInstance()->registry(this);
       }

    void notify(T data) override{
        std::cout<<"data received for HU speed "<<data.speed;
    }

};

