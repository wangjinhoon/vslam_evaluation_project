#include "module1/module1.hpp"
#include <iostream>

void foo_func()
{
    std::cout << "header1::foo(): " << header1::foo() << std::endl;
    std::cout << ("success!") <<std::endl;
}

namespace header1{
    int foo() {return 0;}
    void bar() {};
}