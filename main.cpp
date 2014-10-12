#include <iostream>
#include "messaging.hpp"
#include <string>

using namespace std;

class context : public messaging::IMessageContext<int>,
        public messaging::IMessageContext<std::string>
{
public:
    void go(const int &i)
    {
        messaging::send<int>(this, i);
    }
    void go(const string &s)
    {
        messaging::send<std::string>(this, s);
    }
};

class listener : public messaging::IMessageListener<int>,
        public messaging::IMessageListener<string>
{
    virtual void handleMessage(const int &i){
        std::cout << i << std::endl;
    }
    virtual void handleMessage(const string &i) {
        std::cout << i << std::endl;
    }

public:
    virtual ~listener(){}
};


int main()
{

    context c;
    listener l;

    messaging::attach<int>(&c,&l);
    messaging::attach<std::string>(&c,&l);

    c.go("Hello, world!");
    c.go(1234);

    return 0;
}


