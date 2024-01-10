#include <iostream>
#include <tuple>

#include "kcell.hpp"
#include "kcells.hpp"
#include "interval.hpp"
#include "kcellnd.hpp"
#include "tools.hpp"


int main()
{
    auto fn = [] (std::size_t level, auto idx)
    {
        std::cout << "fn: level=" << level << ", idx=" << idx << std::endl;
    };

    KCell cell;
    Interval i{10, 17};

    std::cout << "cell = " << cell << std::endl;
    std::cout << cell.shift(i) << std::endl;
    cell.shift(fn, 10, 10);
    std::cout << std::endl;

    std::cout << "Testing KCell in nD..." << std::endl;
    KCellND c3d(KCell<false>{}, cell, cell);
    std::cout << "c3d = " << c3d << std::endl;
    std::cout << "topology = " << c3d.topology() << std::endl;
    std::cout << "dimension = " << c3d.dimension() << std::endl;

    std::cout << std::endl;
    auto c3dn = c3d.next<0>();
    std::cout << "c3dn = " << c3dn << std::endl;
    return 0;
}