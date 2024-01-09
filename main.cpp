#include <iostream>

#include "cell.hpp"
#include "interval.hpp"

int main()
{
    KCell<true, 0, 0> cell;
    std::cout << cell << std::endl;

    auto cn = cell.next<2>();
    std::cout << cn << std::endl;

    Interval i{10, 16};
    std::cout << cell.apply(i) << std::endl;
    std::cout << cn.apply(i) << std::endl;

    auto cu = cell.up();
    std::cout << cu << std::endl;
    std::cout << cu.apply(i) << std::endl;

    auto cun = cu.next();
    std::cout << cun << std::endl;
    std::cout << cun.apply(i) << std::endl;

    auto cnun = cell.next().up().next();
    std::cout << cnun << std::endl;
    std::cout << cnun.apply(i) << std::endl;

    auto cd = cell.up<-2>();
    std::cout << cd << std::endl;
    std::cout << cd.apply(i) << std::endl;

    auto ci = cell.nextIncident();
    std::cout << ci << std::endl;

    auto cii = ci.nextIncident();
    std::cout << cii << std::endl;
    
    return 0;
}