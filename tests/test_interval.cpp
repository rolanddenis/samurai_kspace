#include <iostream>
#include "interval.hpp"

int main()
{
    {
        Interval i{0, 9};
        std::cout << "i = " << i << std::endl;
        std::cout << "i >> 1  = " << (i >> 1) << std::endl;
        std::cout << "i >> 2  = " << (i >> 2) << std::endl;
        std::cout << "i >> 20 = " << (i >> 20) << std::endl;
        std::cout << "i << 1  = " << (i << 1) << std::endl;
        std::cout << "i << 2  = " << (i << 2) << std::endl;
        std::cout << "(i << 2) >> 2  = " << ((i << 2) >> 2) << std::endl;
        std::cout << "(i >> 2) << 2  = " << ((i >> 2) << 2) << std::endl;
    }

    {
        Interval i{-9, 2};
        std::cout << "i = " << i << std::endl;
        std::cout << "i >> 1  = " << (i >> 1) << std::endl;
        std::cout << "i >> 2  = " << (i >> 2) << std::endl;
        std::cout << "i >> 10 = " << (i >> 10) << std::endl;
        std::cout << "i << 1  = " << (i << 1) << std::endl;
        std::cout << "i << 2  = " << (i << 2) << std::endl;
        std::cout << "(i << 2) >> 2  = " << ((i << 2) >> 2) << std::endl;
        std::cout << "(i >> 2) << 2  = " << ((i >> 2) << 2) << std::endl;
    }

    {
        Interval i{-9, 0};
        std::cout << "i = " << i << std::endl;
        std::cout << "i >> 1  = " << (i >> 1) << std::endl;
        std::cout << "i >> 2  = " << (i >> 2) << std::endl;
        std::cout << "i >> 10 = " << (i >> 10) << std::endl;
        std::cout << "i << 1  = " << (i << 1) << std::endl;
        std::cout << "i << 2  = " << (i << 2) << std::endl;
        std::cout << "(i << 2) >> 2  = " << ((i << 2) >> 2) << std::endl;
        std::cout << "(i >> 2) << 2  = " << ((i >> 2) << 2) << std::endl;
    }

    return 0;
}