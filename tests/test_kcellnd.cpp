#include <iostream>
#include <tuple>

#include "kcell.hpp"
#include "kcells.hpp"
#include "interval.hpp"
#include "kcellnd.hpp"
#include "tools.hpp"


int main()
{
    auto fn = [] (std::size_t level, auto... idx)
    {
        std::cout << "fn: level=" << level << ", idx=";
        ((std::cout << idx << ","), ...);
        std::cout << std::endl;
    };

    KCell cell;
    Interval i{10, 17};

    std::cout << "cell = " << cell << std::endl;
    std::cout << cell.shift(i) << std::endl;
    cell.shift(fn, 10, 10);
    std::cout << std::endl;

    std::cout << "Testing KCell in 2D..." << std::endl;
    KCellND c2d(cell, cell);
    std::cout << "c2d = " << c2d << std::endl;
    std::cout << "topology = " << c2d.topology() << std::endl;
    std::cout << "dimension = " << c2d.dimension() << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    c2d.shift(fn, 5, i, 2);
    std::cout << std::endl;

    auto c2dn = c2d.next<0>();
    std::cout << "c2dn = " << c2dn << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    c2dn.shift(fn, 5, i, 2);
    std::cout << std::endl;

    auto c2du = c2d.up();
    std::cout << "c2du = " << c2du << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    c2du.shift(fn, 5, i, 2);
    std::cout << std::endl;

    auto c2d2u = c2d.up<2>();
    std::cout << "c2d2u = " << c2d2u << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    c2d2u.shift(fn, 5, i, 2);
    std::cout << std::endl;

    std::cout << "Testing KCell in 3D..." << std::endl;
    KCellND c3d(cell, cell, cell);
    std::cout << "c3d = " << c3d << std::endl;
    std::cout << "topology = " << c3d.topology() << std::endl;
    std::cout << "dimension = " << c3d.dimension() << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3d.shift(fn, 5, i, 2, 3);

    std::cout << std::endl;
    auto c3dn = c3d.next<0>();
    std::cout << "c3dn = " << c3dn << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dn.shift(fn, 5, i, 2, 3);

    std::cout << std::endl;

    auto c3du = c3d.up();
    std::cout << "c3du = " << c3du << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3du.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    return 0;
}