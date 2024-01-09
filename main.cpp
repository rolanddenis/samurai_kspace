#include <iostream>

#include "cell.hpp"
#include "interval.hpp"

int main()
{
    auto fn = [] (std::size_t level, auto idx)
    {
        std::cout << "fn: level=" << level << ", idx=" << idx << std::endl;
    };

    KCell<true, 0, 0> cell;
    Interval i{10, 17};

    std::cout << cell << std::endl;
    std::cout << cell.apply(i) << std::endl;
    cell.apply(fn, 10, 10);
    std::cout << std::endl;

    auto cn = cell.next<2>();
    std::cout << cn << std::endl;
    std::cout << cn.apply(i) << std::endl;
    cn.apply(fn, 10, 10);
    std::cout << std::endl;

    auto cu = cell.up();
    std::cout << cu << std::endl;
    std::cout << cu.apply(i) << std::endl;
    cu.apply(fn, 10, 10);
    std::cout << std::endl;

    auto cun = cu.next();
    std::cout << cun << std::endl;
    std::cout << cun.apply(i) << std::endl;
    cun.apply(fn, 10, 10);
    std::cout << std::endl;

    auto cnun = cell.next().up().next();
    std::cout << cnun << std::endl;
    std::cout << cnun.apply(i) << std::endl;
    cnun.apply(fn, 10, 10);
    std::cout << std::endl;

    auto cd = cell.up<-2>();
    std::cout << cd << std::endl;
    std::cout << cd.apply(i) << std::endl;
    cd.apply(fn, 10, 10);
    std::cout << std::endl;

    auto ci = cell.nextIncident();
    std::cout << ci << std::endl;
    std::cout << ci.apply(i) << std::endl;
    ci.apply(fn, 10, 10);
    std::cout << std::endl;

    auto cii = ci.nextIncident();
    std::cout << cii << std::endl;
    std::cout << cii.apply(i) << std::endl;
    cii.apply(fn, 10, 10);
    std::cout << std::endl;

    return 0;
}