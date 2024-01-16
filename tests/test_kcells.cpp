#include <iostream>
#include <tuple>

#include "kcell.hpp"
#include "kcells.hpp"
#include "interval.hpp"
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

    std::cout << "Testing KCells..." << std::endl;

    std::cout << "cell = " << cell << std::endl;

    auto cn = cell.next<2>();
    std::cout << "cn = " << cn << std::endl;

    KCells cells(cell, KCell<false>{}, KCell<true, 1>{});
    std::cout << "cells = " << cells << std::endl;

    using std::get;
    std::cout << get<1>(cells) << std::endl;

    std::cout << (cells + cells) << std::endl;

    std::cout << cells.next() << std::endl;

    std::cout << std::endl;
    std::cout << (cn.next() + cn).next() << std::endl;

    std::cout << std::endl;

    std::cout << "cells + cell = " << (cells + cell) << std::endl;

    std::cout << "cells.has(KCell<false>) = " << cells.has(KCell<false>{}) << std::endl;
    std::cout << "cells.has(KCell<false, 1>) = " << cells.has(KCell<false, 1>{}) << std::endl;
    std::cout << std::endl;

    std::cout << "Testing empty KCells..." << std::endl;
    auto empty = KCells{};
    std::cout << "empty = " << empty << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    empty.shift(fn, 5, i, 2);
    std::cout << "empty.next() = " << empty.next() << std::endl;
    std::cout << "empty.prev() = " << empty.prev() << std::endl;
    std::cout << "empty.incident() = " << empty.incident() << std::endl;
    std::cout << "empty.lowerIncident() = " << empty.lowerIncident() << std::endl;
    std::cout << "empty.upperIncident() = " << empty.upperIncident() << std::endl;
    std::cout << "empty.properNeighborhood() = " << empty.properNeighborhood() << std::endl;
    std::cout << "empty.neighborhood() = " << empty.neighborhood() << std::endl;
    std::cout << "empty.up() = " << empty.up() << std::endl;
    std::cout << "empty.down() = " << empty.down() << std::endl;

    return 0;
}