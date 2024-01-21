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
    std::cout << "c2d.next<0>() = " << c2dn << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    c2dn.shift(fn, 5, i, 2);
    std::cout << std::endl;

    auto c2du = c2d.up();
    std::cout << "c2d.up() = " << c2du << std::endl;
    std::cout << "fn(level=5, i, 2):" << std::endl;
    c2du.shift(fn, 5, i, 2);
    std::cout << std::endl;

    auto c2d2u = c2d.up<2>();
    std::cout << "c2d.up<2>() = " << c2d2u << std::endl;
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
    std::cout << "c3d.next<0>() = " << c3dn << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dn.shift(fn, 5, i, 2, 3);

    std::cout << std::endl;

    auto c3du = c3d.up();
    std::cout << "c3d.up() = " << c3du << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3du.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    std::cout << "Testing incidence:" << std::endl;

    auto c3di1 = c3d.incident<1>();
    std::cout << "c3d.incident<1>() = " << c3di1 << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3di1.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    auto c3dli = c3d.lowerIncident();
    std::cout << "c3d.lowerIncident() = " << c3dli << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dli.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;
    
    auto c3di1li = c3di1.lowerIncident();
    std::cout << "c3d.incident<1>().lowerIncident() = " << c3di1li << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3di1li.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    auto c3di1i2 = c3di1.incident<2>();
    std::cout << "c3d.incident<1>().incident<2>() = " << c3di1i2 << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3di1i2.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    auto c3di1i2li = c3di1i2.lowerIncident();
    std::cout << "c3d.incident<1>().incident<2>().lowerIncident() = " << c3di1i2li << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3di1i2li.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    auto c3di1i2lili = c3di1i2li.lowerIncident();
    std::cout << "c3d.incident<1>().incident<2>().lowerIncident().lowerIncident() = " << c3di1i2lili << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3di1i2lili.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    auto face3d = KCellND{cell, cell, KCell<false>{}};
    std::cout << "face3d = " << face3d << std::endl;
    auto face3dui = face3d.upperIncident();
    std::cout << "face3d.upperIncident() = " << face3dui << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    face3dui.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    auto c3dpn = c3d.properNeighborhood();
    std::cout << "c3d.properNeighborhood() = " << c3dpn << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dpn.shift(fn, 5, i, 2, 3);
    std::cout << "c3d.properNeighborhood().indexShift() = " << c3dpn.indexShift() << std::endl;
    std::cout << std::endl;

    auto c3dnn = c3d.neighborhood();
    std::cout << "c3d.neighborhood() = " << c3dnn << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dnn.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    std::cout << "Examples from stencil.hpp:" << std::endl;

    std::cout
        << "Star shaped 1-neighborhood of c3d = "
        << (c3d + details::dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood(); },
            c3d
        )).indexShift()
        << std::endl;

    std::cout
        << "Star shaped 2-neighborhood of c3d = "
        << (c3d + details::dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood<2>(); },
            c3d
        )).indexShift()
        << std::endl;

    std::cout
        << "Star shaped 3-neighborhood of c3d = "
        << (c3d + details::dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood<3>(); },
            c3d
        )).indexShift()
        << std::endl;

    std::cout << "Cartesian directions of c3d = " << c3d.properNeighborhood().indexShift() << std::endl;
    std::cout
        << "Positive Cartesian directions of c3d = "
        << details::dimension_concatenate(
            [] (auto, auto cell) { return cell.next(); },
            c3d
        ).indexShift()
        << std::endl;

    std::cout << std::endl;

    std::cout << "Testing factories:" << std::endl;
    std::cout << "make_KCellND<3>() = " << make_KCellND<3>() << std::endl;
    std::cout << "make_KCellND<3, 5>() = " << make_KCellND<3, 5>() << std::endl;

    return 0;
}