#pragma once

#include <cstddef>
#include <ostream>
#include <type_traits>

#include "interval.hpp"

namespace details
{
    /// Bitwise shift with signed shift
    template <
        typename T,
        typename std::enable_if_t<std::is_integral_v<T>, bool> = true
    >
    constexpr T bitwise_shift(T i, std::ptrdiff_t shift) noexcept
    {
        if (shift >= 0)
            return i * (1 << shift);
        else
            return i / (1 << -shift);       
    }

    /// Shift level and position of one index
    template <
        typename T,
        typename std::enable_if_t<std::is_integral_v<T>, bool> = true
    >
    constexpr T apply(T i, std::ptrdiff_t index_shift, std::ptrdiff_t level_shift) noexcept
    {
        return bitwise_shift(i, level_shift) + index_shift;
    }

} // namespace details

/**
 * Khalimsky cell in 1D
 * 
 * @tparam Open         true if the cell is open, false if closed
 * @tparam IndexShift   Index shift relatively to the original cell
 * @tparam LevelShift   Level shift relatively to the original cell
*/
template <
    bool Open,
    std::ptrdiff_t IndexShift,
    std::ptrdiff_t LevelShift
>
struct KCell
{
    /// Topology of the cell ({0, 1} in 1D, 0...3 in 2D, 0...7 in 3D, ...) => can be used to index a dedicated storage
    using topology_type = unsigned int;

    /// Topology of the cell as an unsigned integer (0 <=> open, 1 <=> close)
    static constexpr topology_type topology() noexcept { return static_cast<topology_type>(Open); }

    /// Khalimsky coordinate of the cell
    static constexpr std::ptrdiff_t khalimsky() noexcept { return 2 * IndexShift + Open; }

    /// Forward move while keeping the same topology
    template <std::ptrdiff_t Steps = 1>
    static constexpr KCell<Open, IndexShift + Steps, LevelShift> next() noexcept { return {}; }

    /// Backward move while keeping the same topology
    template <std::ptrdiff_t Steps = 1>
    static constexpr KCell<Open, IndexShift - Steps, LevelShift> prev() noexcept { return {}; }

    /// Forward move including adjacent cells (change topology for odd steps)
    template <std::ptrdiff_t Steps = 1>
    static constexpr KCell<(khalimsky() + Steps) % 2, (khalimsky() + Steps)  / 2, LevelShift> nextIncident() noexcept { return {}; }


    /// Changing level while keeping the same topology
    // FIXME: should returns 2 cells
    template <std::ptrdiff_t Levels = 1>
    static constexpr KCell<Open, details::apply(IndexShift, 0, Levels), LevelShift + Levels> up() noexcept { return {}; }

    /// Changing level while keeping the same topology
    // FIXME: should returns 2 cells
    template <std::ptrdiff_t Levels = 1>
    static constexpr KCell<Open, details::apply(IndexShift, 0, -Levels), LevelShift - Levels> down() noexcept { return {}; }

    /// Apply the level/index shifts of the current cell to a given index
    template <typename T>
    static constexpr auto apply(T i) noexcept -> decltype(details::apply(i, IndexShift, LevelShift))
    {
        return details::apply(i, IndexShift, LevelShift);
    }

    /// Apply the level/index shifts of the current cell to a given interval
    static constexpr Interval apply(Interval const& i)
    {
        auto step = details::bitwise_shift(i.step, LevelShift);
        return {
            apply(i.a),
            apply(i.b),
            step > 0 ? step : 1
        };
    }

    template <typename Function>
    static constexpr void apply(Function && fn, Interval const& i) { std::forward<Function>(fn)(apply(i)); }
};

template <
    bool Open,
    std::ptrdiff_t IndexShift,
    std::ptrdiff_t LevelShift
>
std::ostream & operator<< ( std::ostream & out, KCell<Open, IndexShift, LevelShift> const& cell )
{
    out << "KCell<topology=" << cell.topology()
        << ",index_shift=" << IndexShift
        << ",level_shift=" << LevelShift
        << ">";
    return out;
}

