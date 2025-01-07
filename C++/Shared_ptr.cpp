#include <iostream>

class student
{
    public:
    int m_ID;
    
    student():m_ID(0)
    {
        
    }
    
    int getID()
    {
        return m_ID;
    }
};


template<typename T>
class shared_ptr {
    
    public:
        int *m_counter;
        T* m_ptr;
        
        shared_ptr(T* ptr)
        {
            std::cout<<"parameterized constructor called"<<std::endl;
            if(m_counter == nullptr)
            {
                m_counter = new int(0);
                (*m_counter)++;
            }
            m_ptr = ptr;
        }
        
        shared_ptr(shared_ptr<T>& copy)
        {
            std::cout<<"copy constructor called"<<std::endl;
            m_ptr = copy.get_ptr();
            m_counter = copy.getcounter();
            (*m_counter)++;
        }
        
        void operator= (shared_ptr sp)
        {
            std::cout<<"operator= overloading called"<<std::endl;
            m_ptr = sp.get_ptr();
            m_counter = sp.getcounter();
            (*m_counter)++;
        }
        
        int* getcounter() {
            return m_counter;
        }

        T* get_ptr() {
            return m_ptr;
        }
        
        ~shared_ptr()
        {
            (*m_counter)--;
            if((*m_counter) == 0)
            {
                delete m_counter;
                delete m_ptr;
            }
        }
    
};

int main()
{
    shared_ptr<student> ptr = new student();
    
    std::cout << ptr.get_ptr()->getID() << std::endl;
    std::cout << ptr.get_ptr() <<std::endl;
    std::cout << *(ptr.getcounter())<<std::endl;
    {
        std::cout<<"after pointer1 creation"<<std::endl;
        shared_ptr<student> ptr1(ptr);
        std::cout << ptr.get_ptr() <<std::endl;
        std::cout << *(ptr.getcounter())<<std::endl;
        std::cout << ptr1.get_ptr() <<std::endl;
        std::cout << *(ptr1.getcounter())<<std::endl;
    }
    
     std::cout<<"after pointer1 scope "<<std::endl;
     
    std::cout << ptr.get_ptr() <<std::endl;
    std::cout << *(ptr.getcounter())<<std::endl;
    
    return 0;
}