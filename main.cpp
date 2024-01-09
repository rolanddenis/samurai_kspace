#include <iostream>
#include <tuple>

#include "kcell.hpp"
#include "kcells.hpp"
#include "interval.hpp"

template <typename... T, std::size_t... Index>
void disp_tuple_impl(std::ostream& out, std::tuple<T...> const& t, std::index_sequence<Index...>)
{
    ((out << std::get<Index>(t) << ", "), ...);
}

template <typename... T>
std::ostream & operator<< (std::ostream& out, std::tuple<T...> const& t)
{
    out << "tuple(";
    disp_tuple_impl(out, t, std::make_index_sequence<sizeof...(T)>{});
    out << ")";
    return out;
}

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

    auto cn = cell.next<2>();
    std::cout << "cn = " << cn << std::endl;
    std::cout << cn.shift(i) << std::endl;
    cn.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cu = cell.up();
    std::cout << "cu = " << cu << std::endl;
    std::cout << cu.shift(i) << std::endl;
    cu.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cun = cu.next();
    std::cout << "cun = " << cun << std::endl;
    std::cout << cun.shift(i) << std::endl;
    cun.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cnun = cell.next().up().next();
    std::cout << "cnun = " << cnun << std::endl;
    std::cout << cnun.shift(i) << std::endl;
    cnun.shift(fn, 10, 10);
    std::cout << "sum(cnun.index) = " << cnun.apply([] (auto... cell) { return (cell.shift([] (std::size_t, std::size_t i) { return i; }, 10, 10) + ...); }) << std::endl;
    std::cout << std::endl;

    /*
    Instead of:

    u(level, i, j) = 0.25*(u(level+1, 2*i, 2*j)
                        + u(level+1, 2*i+1, 2*j)
                        + u(level+1, 2*i, 2*j+1)
                        + u(level+1, 2*i+1, 2*j+1));

    We can write:

    auto kcells = KCell{}.up();
    u(level, i, j) = (1. / kcells.size()) * kcells.apply(
        [] (auto... cell) { return (cell.shift(u, level, i, j) + ...); }
    );
    */



    auto cd = cell.up<-2>();
    std::cout << "cd = " << cd << std::endl;
    std::cout << cd.shift(i) << std::endl;
    cd.shift(fn, 10, 10);
    std::cout << std::endl;

    auto ci = cell.nextIncident();
    std::cout << "ci = " << ci << std::endl;
    std::cout << ci.shift(i) << std::endl;
    ci.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cii = ci.nextIncident();
    std::cout << "cii = " << cii << std::endl;
    std::cout << cii.shift(i) << std::endl;
    cii.shift(fn, 10, 10);
    std::cout << std::endl;


    std::cout << "Testing KCells..." << std::endl;
    KCells cells(cell, cu, cun);
    std::cout << cells << std::endl;

    using std::get;
    std::cout << get<1>(cells) << std::endl;

    std::cout << (cells + cells) << std::endl;

    std::cout << cells.next() << std::endl;

    std::cout << std::endl;
    std::cout << (cn.next() + cn).next() << std::endl;

    std::cout << std::endl;

    return 0;
}