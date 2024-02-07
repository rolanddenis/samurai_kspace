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

    std::cout << "Testing KCell in 1D..." << std::endl;
    KCellND c1d(cell);
    std::cout << "c1d = " << c1d << std::endl;
    std::cout << "topology = " << c1d.topology() << std::endl;
    std::cout << "dimension = " << c1d.dimension() << std::endl;
    std::cout << "fn(level=5, i):" << std::endl;
    c1d.shift(fn, 5, i);
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

    auto c3dpn2 = c3d.properNeighborhood<2>();
    std::cout << "c3d.properNeighborhood<2>() = " << c3dpn2 << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dpn2.shift(fn, 5, i, 2, 3);
    std::cout << "c3d.properNeighborhood<2>().indexShift() = " << c3dpn2.indexShift() << std::endl;
    std::cout << std::endl;

    auto c3dnn = c3d.neighborhood();
    std::cout << "c3d.neighborhood() = " << c3dnn << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    c3dnn.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    std::cout << "Examples from stencil.hpp:" << std::endl;

    std::cout
        << "Star shaped 0-neighborhood of c3d = "
        << (c3d + c3d.dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood<0>(); }
        )).indexShift()
        << std::endl;

    std::cout
        << "Star shaped 1-neighborhood of c3d = "
        << (c3d + c3d.dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood(); }
        )).indexShift()
        << std::endl;

    std::cout
        << "Star shaped 2-neighborhood of c3d = "
        << (c3d + c3d.dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood<2>(); }
        )).indexShift()
        << std::endl;

    std::cout
        << "Star shaped 3-neighborhood of c3d = "
        << (c3d + c3d.dimension_concatenate(
            [] (auto, auto cell) { return cell.template properNeighborhood<3>(); }
        )).indexShift()
        << std::endl;

    std::cout << "Cartesian directions of c3d = " << c3d.properNeighborhood().indexShift() << std::endl;
    std::cout
        << "Positive Cartesian directions of c3d = "
        << c3d.dimension_concatenate(
            [] (auto, auto cell) { return cell.next(); }
        ).indexShift()
        << std::endl;

    std::cout << std::endl;

    std::cout
        << "Corners of c1d = "
        << (c1d.enumerate_cartesian(
            [] (auto, auto cell) { return cell.properNeighborhood(); }
        ) - c1d.properNeighborhood<0>()).indexShift()
        << std::endl;

    std::cout
        << "Corners of c2d = "
        << (c2d.enumerate_cartesian(
            [] (auto, auto cell) { return cell.properNeighborhood(); }
        ) - c2d.properNeighborhood<1>()).indexShift()
        << std::endl;

    std::cout
        << "Corners of c3d = "
        << (c3d.enumerate_cartesian(
            [] (auto, auto cell) { return cell.properNeighborhood(); }
        ) - c3d.properNeighborhood<2>()).indexShift()
        << std::endl;
    std::cout << std::endl;

    std::cout << "Checking constexpr of indexShift:" << std::endl;
    constexpr auto index_shift = (c3d.enumerate_cartesian(
            [] (auto, auto cell) { return cell.properNeighborhood(); }
        ) - c3d.properNeighborhood<2>()).indexShift();
    std::cout << "First value of corners of c3d = " << std::integral_constant<std::ptrdiff_t, index_shift[0][0]>{} << std::endl;
    std::cout << std::endl; 

    std::cout << "Testing factories:" << std::endl;
    std::cout << "make_KCellND<3>() = " << make_KCellND<3>() << std::endl;
    std::cout << "make_KCellND<3, 5>() = " << make_KCellND<3, 5>() << std::endl;
    std::cout << std::endl;

    std::cout << "Testing faces and neighborhood:" << std::endl;
    auto f3d = c3d.incident<1, -1>();
    std::cout << "f3d = c3d.incident<1, -1>() = " << f3d << std::endl;
    auto f3d_ui = f3d.upperIncident();
    std::cout << "f3d.upperIncident().indexShift() = " << f3d_ui.indexShift() << std::endl;
    std::cout << "fn(level=5, i, 2, 3):" << std::endl;
    f3d_ui.shift(fn, 5, i, 2, 3);
    std::cout << std::endl;

    // FIXME: implement a foreach on spanned directions (2^n for a spell)
    std::cout << "Testing directional stencils:" << std::endl;
    constexpr std::size_t distance = 0;
    std::size_t id = 0;
    auto helper = [&c3d, &id] (auto i, auto step)
    {
        auto cell = c3d.next<decltype(i)::value, decltype(step)::value>();
        std::cout << "dir_stencils[" << id << "].direction = " << cell.indexShift() << std::endl;
        auto stencil = c3d + c3d.dimension_concatenate(
            [] (auto j, auto cell)
            {
                return cell.template properNeighborhood<(decltype(i)::value == decltype(j)::value) ? distance : 0, decltype(step)::value>();
            }
        );
        std::cout << "dir_stencils[" << id << "].stencil = " << stencil.indexShift() << std::endl;
        ++id;
    };
    c3d.enumerate(
        [&helper] (auto i, auto)
        {
            helper(i, std::integral_constant<std::ptrdiff_t, -1>{});
            helper(i, std::integral_constant<std::ptrdiff_t, 1>{});
        }
    );
    std::cout << std::endl;

    std::cout << "Testing reference returned by shift:" << std::endl;
    std::array<double, 5> a{1., 2., 3., 4., 5.};
    auto afn = [&a] (auto, auto i, auto...) -> decltype(auto) { return a[i]; };
    std::cout << "a = " << a << std::endl;
    std::cout << "c1d.shift(a, 0, 0) = " << c1d.shift(afn, 0, 0) << std::endl;
    c1d.shift(afn, 0, 0) = -1;
    std::cout << "after `c1d.shift(afn, 0, 0) = -1`, a = " << a << std::endl;
    auto && v0 = c1d.shift(afn, 0, 0);
    v0 = -2;
    std::cout << "a = " << a << std::endl;
    auto && [v1, v2, v3] = c1d.neighborhood().shift(afn, 0, 2);
    //auto && [v1, v2, v3] = c1d.next<2>().neighborhood().shift(afn, 0, 0);
    std::cout << "v1 = " << v1 << " ; v2 = " << v2 << " ; v3 = " << v3 << std::endl;
    v1 = 3.14;
    v3 = -8;
    std::cout << "v1 = " << v1 << " ; v2 = " << v2 << " ; v3 = " << v3 << std::endl;
    //std::cout << v1.toto() << std::endl;
    std::cout << "a = " << a << std::endl;
    auto && v4 = c1d.neighborhood().shift(afn, 0, 2);
    std::get<0>(v4) = 99.;
    //v4.toto();
    std::cout << "a = " << a << std::endl;

    return 0;
}