#include <iostream>


enum class Vehicle_State{
    INIT,
    RUNNING,
    ERROR,
    SHUTDOWN
};

class Vehicle_Contorller{
public:
    Vehicle_Contorller() = default;

    Vehicle_Contorller(Vehicle_State state):m_current_state(state){

    }

    void changestate(Vehicle_State state){
        m_current_state = state;
    }

    Vehicle_State get_state() const{
        return m_current_state;
    }

    
private:
    Vehicle_State m_current_state{Vehicle_State::INIT};

};


std::ostream& operator<<(std::ostream& os,
                         Vehicle_State state)
    {
        switch(state)
        {
        case Vehicle_State::INIT:
            return os << "INIT";

        case Vehicle_State::RUNNING:
            return os << "RUNNING";

        case Vehicle_State::ERROR:
            return os << "ERROR";

        case Vehicle_State::SHUTDOWN:
            return os << "SHUTDOWN";
        }

        return os << "UNKNOWN";
    }

int main()
{
    Vehicle_Contorller controller(Vehicle_State::INIT);
    std::cout << controller.get_state() << '\n';

    controller.changestate(Vehicle_State::RUNNING);
    std::cout<<controller.get_state() << '\n';

    controller.changestate(Vehicle_State::ERROR);
   std::cout<<controller.get_state() << '\n';

    controller.changestate(Vehicle_State::SHUTDOWN);
    std::cout<<controller.get_state() << '\n';
}