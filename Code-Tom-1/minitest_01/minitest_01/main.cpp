#include <iostream>

#define l(v) std::wcout << #v << " = " << (v) << "\n";

struct  Test
{       Test(){}
       ~Test(){ delete p; }

    int n{0};

    static inline Test* p = nullptr;

    static void init()
    {   if(p == nullptr) p = new Test;
        else return;
        p->n  = 2024;
    }
};

Test test;

int main()
{
    Test::init();

    std::cout << "Hello Start ..." << std::endl;
    std::cout << Test::p->n        << std::endl;
    std::cout << "Finished."       << std::endl;
}
