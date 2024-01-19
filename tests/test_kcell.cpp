#include <iostream>
#include <tuple>

#include "kcell.hpp"
#include "kcells.hpp"
#include "interval.hpp"
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
    std::cout << "cell.dimension() = " << cell.dimension() << std::endl;
    std::cout << "cell.topology() = " << cell.topology() << std::endl;
    std::cout << "cell.size() = " << cell.size() << std::endl;
    std::cout << "cell.shift(i) = " << cell.shift(i) << std::endl;
    std::cout << "fn(level=10, i=10):" << std::endl;
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

    auto cuu = cu.up();
    std::cout << "cuu = " << cuu << std::endl;
    std::cout << cuu.shift(i) << std::endl;
    cuu.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cu2 = cell.up<2>();
    std::cout << "cu2 = " << cu2 << std::endl;
    std::cout << cu2.shift(i) << std::endl;
    cu2.shift(fn, 10, 10);
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
    auto kcells = KCellND{}.up();
    u(level, i, j) = (1. / kcells.size()) * kcells.apply(
        [] (auto... cell) { return (cell.shift(u, level, i, j) + ...); }
    );

    
    */



    auto cd = cell.down();
    std::cout << "cd = " << cd << std::endl;
    std::cout << cd.shift(i) << std::endl;
    cd.shift(fn, 10, 10);
    std::cout << std::endl;

    auto ci = cell.incident();
    std::cout << "ci = " << ci << std::endl;
    std::cout << ci.shift(i) << std::endl;
    ci.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cii = ci.incident();
    std::cout << "cii = " << cii << std::endl;
    std::cout << cii.shift(i) << std::endl;
    cii.shift(fn, 10, 10);
    std::cout << std::endl;

    auto c = cii.incident<-2>();
    std::cout << "c = " << c << std::endl;
    std::cout << c.shift(i) << std::endl;
    c.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cli = cell.lowerIncident();
    std::cout << "cli = " << cli << std::endl;
    std::cout << cli.shift(i) << std::endl;
    cli.shift(fn, 10, 10);
    std::cout << std::endl;
    
    auto ciu = ci.up();
    std::cout << "ciu = " << ciu << std::endl;
    std::cout << ciu.shift(i) << std::endl;
    ciu.shift(fn, 10, 10);
    std::cout << std::endl; 
    
    auto c2 = ciu.down().incident<-1>().unique(); // Should contains only `cell`
    std::cout << "c2 = " << c2 << std::endl;
    std::cout << c2.shift(i) << std::endl;
    c2.shift(fn, 10, 10);
    std::cout << std::endl; 

    auto cn1 = cell.neighborhood();
    std::cout << "cell.neighborhood().indexShift() = " << cn1.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cn1.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cn2 = cell.neighborhood<2>();
    std::cout << "cell.neighborhood<2>().indexShift() = " << cn2.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cn2.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cn3 = cell.neighborhood<3>();
    std::cout << "cell.neighborhood<3>().indexShift() = " << cn3.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cn3.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cpn1 = cell.properNeighborhood();
    std::cout << "cell.properNeighborhood().indexShift() = " << cpn1.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cpn1.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cpn2 = cell.properNeighborhood<2>();
    std::cout << "cell.properNeighborhood<2>().indexShift() = " << cpn2.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cpn2.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cpn3 = cell.properNeighborhood<3>();
    std::cout << "cell.properNeighborhood<3>().indexShift() = " << cpn3.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cpn3.shift(fn, 10, 10);
    std::cout << std::endl;

    auto cn3m2 = cell.neighborhood<3>() - cell.properNeighborhood<2>();
    std::cout << "(cell.neighborhood<3>() - cell.properNeighborhood<2>()).indexShift() = " << cn3m2.indexShift() << std::endl;
    std::cout << "fn(level=10, 10):" << std::endl;
    cn3m2.shift(fn, 10, 10);
    std::cout << std::endl;

    return 0;
}