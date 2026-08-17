#include <iostream>
#include <concept>

struct Car
{
    bool start()
    {
        std::cout << "Car started\n";
        return true;
    }
};

struct Engine
{
    bool start()
    {
        std::cout << "Engine started\n";
        return true;
    }
};

struct BadComponent{
    int start()
    {
        return 1;
    }
};
struct Wheel
{
    void rotate()
    {
        std::cout << "Wheel rotating\n";
    }
};

template<typename T>
concept Startable = requires(T obj)
{
    {obj.start()} -> std::same_as<bool>;
};

template<Startable T>
void StartComponent(T& obj)
{
    obj.start();
}

int main()
{
    Car C;
    StartComponent(C);

    Engine E;
    StartComponent(E);

    // Wheel W;
    // StartComponent(W);

    BadComponent B;
    StartComponent(B);

    return 0;
}