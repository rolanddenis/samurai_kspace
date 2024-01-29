#pragma once

#include <cstddef>
#include <ostream>
#include <type_traits>

#include "kcells.hpp"

// Pre-declaration
template <
    bool Open,
    std::ptrdiff_t IndexShift,
    std::ptrdiff_t LevelShift
>
struct KCell;
namespace details
{
    /// Bitwise shift with signed shift (<< for positive shift, >> for negative shift)
    template <typename T>
    constexpr auto bitwise_shift(T && i, std::ptrdiff_t shift) noexcept
    {
        if (shift >= 0)
            return std::forward<T>(i) << shift;
        else
            return std::forward<T>(i) >> -shift;     
    }

    /// Shift level and position of one index or interval
    template <typename T>
    constexpr auto shift(T && i, std::ptrdiff_t index_shift, std::ptrdiff_t level_shift) noexcept
    {
        return bitwise_shift(std::forward<T>(i), level_shift) + index_shift;
    }

    /// Alternative version with shift as template parameters (premature optimization to check if we can avoid useless operations on custom type)
    template <
        std::ptrdiff_t IndexShift = 0,
        std::ptrdiff_t LevelShift = 0,
        typename T
    >
    constexpr auto shift(T i) noexcept
    {
        if constexpr (LevelShift > 0)
            i <<= LevelShift;
        else if constexpr (LevelShift < 0)
            i >>= -LevelShift;
        if constexpr (IndexShift != 0)
            i += IndexShift;
        return i;
    }

    /// Allows KCell to be stored in a KCells
    template <
        bool Open,
        std::ptrdiff_t IndexShift,
        std::ptrdiff_t LevelShift
    >
    struct is_kcell<KCell<Open, IndexShift, LevelShift>> : std::true_type {};

} // namespace details

/**
 * Khalimsky cell in 1D
 * 
 * @tparam Open         true if the cell is open, false if closed
 * @tparam IndexShift   Index shift relatively to the original cell
 * @tparam LevelShift   Level shift relatively to the original cell
*/
template <
    bool Open = true,
    std::ptrdiff_t IndexShift = 0,
    std::ptrdiff_t LevelShift = 0
>
struct KCell
{
    /// Topology of the cell ({0, 1} in 1D, 0...3 in 2D, 0...7 in 3D, ...) => can be used to index a dedicated storage
    using topology_type = unsigned int;
    using dimension_type = unsigned int;

    /// Topology of the cell as an unsigned integer (0 <=> open, 1 <=> close)
    static constexpr topology_type topology() noexcept { return static_cast<topology_type>(Open); }

    /// Dimension of the cell
    static constexpr dimension_type dimension() noexcept { return static_cast<dimension_type>(Open); }

    /// Dimension of the space
    static constexpr dimension_type size() noexcept { return 1; }

    // Convenient template parameter access
    static constexpr bool isOpen() noexcept { return Open; }
    static constexpr std::ptrdiff_t indexShift() noexcept { return IndexShift; }
    static constexpr std::ptrdiff_t levelShift() noexcept { return LevelShift; }

    /// Khalimsky coordinate of the cell
    static constexpr std::ptrdiff_t khalimsky() noexcept { return 2 * IndexShift + Open; }

    /// Forward move while keeping the same topology
    template <std::ptrdiff_t Steps = 1>
    static constexpr KCells<KCell<Open, IndexShift + Steps, LevelShift>> next() noexcept { return {}; }

    /// Backward move while keeping the same topology
    template <std::ptrdiff_t Steps = 1>
    static constexpr KCells<KCell<Open, IndexShift - Steps, LevelShift>> prev() noexcept { return {}; }

    /// Forward/backward move including incident cells (change topology for odd steps)
    template <std::ptrdiff_t Steps = 1>
    static constexpr KCells<KCell<(khalimsky() + Steps) % 2 != 0, ((khalimsky() + Steps) >> 1), LevelShift>> incident() noexcept { return {}; }

    /// Neighborhood of incident cells of dimension dim-1 (eg for a face in 2D, it returns it's edges)
    static constexpr auto lowerIncident() noexcept
    {
        if constexpr (dimension() == 0)
            return KCells{};
        else
            return incident<-1>() + incident<1>();
    }
    
    /// Neighborhood of incident cells of dimension dim+1 (eg for an edge in 2D, it returns it's two faces)
    static constexpr auto upperIncident() noexcept
    {
        if constexpr (dimension() == size())
            return KCells{};
        else
            return incident<-1>() + incident<1>();
    }

    /// Proper neighborhood of proper (current cell not included) adjacent cells (same topology)
    template <
        std::size_t Distance = 1,
        std::ptrdiff_t Step = 1
    >
    static constexpr auto properNeighborhood() noexcept
    {
        if constexpr (Distance == 0)
            return KCells{};
        else
            return prev<Step * static_cast<std::ptrdiff_t>(Distance)>() + properNeighborhood<Distance - 1, Step>() + next<Step * static_cast<std::ptrdiff_t>(Distance)>();
    }

    /// Neighborhood adjacent cells (same topology), including current cell
    template <
        std::size_t Distance = 1,
        std::ptrdiff_t Step = 1
    >
    static constexpr auto neighborhood() noexcept
    {
        if constexpr (Distance == 0)
            return KCells(KCell{});
        else
            return prev<Step * static_cast<std::ptrdiff_t>(Distance)>() + neighborhood<Distance - 1, Step>() + next<Step * static_cast<std::ptrdiff_t>(Distance)>();
    }

    /// Changing level while keeping the same topology
    template <std::ptrdiff_t Levels = 1>
    static constexpr
    auto up() noexcept
    {
        if constexpr (Levels < 0)
            return down<-Levels>();
        if constexpr (Levels == 0)
            return KCells<KCell>{};
        else if constexpr (Levels == 1)
            return KCells<
                KCell<Open, details::shift<0, Levels>(IndexShift), LevelShift + Levels>,
                KCell<Open, details::shift<1, Levels>(IndexShift), LevelShift + Levels>
            >{};
        else
            return up().template up<Levels - 1>();
    }

    /// Changing level while keeping the same topology
    template <std::ptrdiff_t Levels = 1>
    static constexpr
    auto down() noexcept
    {
        if constexpr (Levels < 0)
            return up<-Levels>();
        else
            return KCells<KCell<Open, details::shift<0, -Levels>(IndexShift), LevelShift - Levels>>{};
    }

    /// Apply the level/index shifts of the current cell to a given index (or interval)
    template <typename T>
    static constexpr auto shift(T && i) noexcept
    {
        return details::shift<IndexShift, LevelShift>(std::forward<T>(i));
    }

    template <typename Function, typename Index>
    static constexpr auto shift(Function && fn, std::size_t level, Index && i)
    {
        return std::forward<Function>(fn)(level + LevelShift, shift(std::forward<Index>(i)));
    }
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

