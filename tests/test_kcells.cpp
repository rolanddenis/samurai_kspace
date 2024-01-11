#include <iostream>
#include <tuple>

#include "kcell.hpp"
#include "kcells.hpp"
#include "interval.hpp"
#include "tools.hpp"

int main()
{
    KCell cell;

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

    return 0;
}