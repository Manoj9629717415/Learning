#include <iostream>
#include <utility>


class BigBuffer{
public:

    BigBuffer(size_t size){
        this->size = size;
        this->buffer = new int[size];
        std::cout<<"Construtor BigBuffer"<<std::endl;
    }

    ~BigBuffer()
    {
        delete[] buffer;
        std::cout<<"Desstrutor BigBuffer"<<std::endl;
    }

    //deep copy constructor
    BigBuffer(const BigBuffer& other){
        this->size = other.size;
        this->buffer = new int [other.size];
        std::copy(other.buffer,other.buffer+other.size,this->buffer);
        std::cout<<"Copy Construtor BigBuffer"<<std::endl;
    }

    BigBuffer& operator=(const BigBuffer& other){
        if(this != &other)
        {
            delete[] buffer;
            this->size = other.size;
            this->buffer = new int [other.size];
            std::copy(other.buffer,other.buffer+other.size,this->buffer);
            std::cout<<"copy assingment BigBuffer"<<std::endl;
        }
        return *this;
    }

    BigBuffer(BigBuffer&& other) noexcept{
        size = std::exchange(other.size,0);
        buffer = std::exchange(other.buffer,nullptr);
        std::cout<<"Move Construtor BigBuffer"<<std::endl;
    }

    BigBuffer& operator=(BigBuffer&& other) noexcept
    {
        if(this != &other){
            delete[] buffer;
            size = std::exchange(other.size,0);
            buffer = std::exchange(other.buffer,nullptr);
            std::cout<<"Move assignment Construtor BigBuffer"<<std::endl;
        }
        return *this;
    }


private:
    size_t size{};
    int* buffer{};

};

void process(std::string& s)
{
    std::cout<<"lvalue"<<std::endl;
}

void process(std::string&& s)
{
    std::cout<<"rvalue"<<std::endl;
}

template<typename T>
void wrapper1(T&& value)
{
    process(value);
}

template<typename T>
void wrapper2(T&& value)
{
    process(std::move(value));
}

template<typename T>
void wrapper3(T&& value)
{
    process(std::forward<T>(value));
}

int main()
{
   /* BigBuffer a(1000000);

    BigBuffer b(a);

    BigBuffer c = std::move(a);

    BigBuffer d(10);
    d = b;

    BigBuffer e(100);
    e = std::move(d);

    a= BigBuffer(50);

    std::string s = "BMW";

    std::string t = std::move(s);

    std::cout << s << std::endl;*/

    std::string s = "BMW";

    wrapper1(s);
    wrapper1("Audi");

    wrapper2(s);
    wrapper2("Audi");

    wrapper3(s);
    wrapper3("Audi");

    return 0;
}