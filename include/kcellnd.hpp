#pragma once

#include <type_traits>
#include <array>
#include <cassert>
#include <utility>

#include "kcell.hpp"
#include "kcells.hpp"
#include "kcell_tuple.hpp"

// Pre-declaration
template <typename... T>
struct KCellND;

namespace samurai
{
    template <std::size_t dim, class TInterval, std::size_t Topology>
    struct CellInterval;

    template <std::size_t dim, class TInterval, std::size_t Topology>
    struct Cell;
}

namespace details
{

    template <
        std::size_t I,
        typename Function,
        typename... T
    >
    constexpr auto enumerate_cartesian_helper(Function && fn, KCellND<T...> const& kcell)
    {
        auto kcells = fn(std::integral_constant<std::size_t, I>{}, kcell.template get<I>());
        
        if constexpr (I == sizeof...(T) - 1)
            return kcells.foreach([] (auto kc) { return KCells(kc); });
        else
        {
            auto next_dim = enumerate_cartesian_helper<I + 1>(fn, kcell);

            // Outer loop
            auto tmp = kcells.foreach(
                [&next_dim](auto lhs)
                {
                    // Inner loop
                    return std::apply(
                        [&lhs](auto... rhs)
                        {
                            return std::make_tuple(KCells(lhs) + rhs...);
                        },
                        next_dim
                    );
                }
            );

            // Concatenate results
            return std::apply(
                [] (auto... t) { return std::tuple_cat(t...); },
                tmp
            );
        }
    }

    /// Cartesian product of enumerate result, converting inner KCells to KCellND
    template <
        typename Function,
        typename... T
    >
    constexpr auto enumerate_cartesian(Function && fn, KCellND<T...> const& kcell)
    {
        return std::apply(
            [] (auto... kcells) { return (KCells(KCellND(kcells)) + ... + KCells{}); },
            enumerate_cartesian_helper<0>(std::forward<Function>(fn), kcell)
        );
    }

    template <
        typename Function,
        typename... T,
        std::size_t... I
    >
    static constexpr auto dimension_concatenate_helper(Function && fn, KCellND<T...> const& kcell, std::index_sequence<I...>)
    {
        return (
            details::enumerate_cartesian(
                [&fn] (auto i, auto cell)
                {
                    if constexpr (decltype(i)::value == I)
                        return fn(i, cell);
                    else
                        return KCells(cell);
                },
                kcell
            ) + ... + KCells{}
        );
    }

    /// Concatenate the result of the function applied to each dimension separately (first only, then second only, then ...)
    template <
        typename Function,
        typename... T
    >
    constexpr auto dimension_concatenate(Function && fn, KCellND<T...> const& kcell)
    {
        return dimension_concatenate_helper(
            std::forward<Function>(fn),
            kcell,
            std::make_index_sequence<sizeof...(T)>{}
        );
    }

    /// Factory
    template <
        std::size_t Topology,
        std::size_t... I
    >
    constexpr auto make_KCellND_helper(std::index_sequence<I...>) noexcept
    {
        return KCellND<KCell<(Topology & (1ul << I)) != 0> ...>{};
    }

    /// Allows KCellND to be stored in a KCells
    template <typename... T>
    struct is_kcell<KCellND<T...>> : std::true_type {};

}

template <typename... T>
struct KCellND : KCellTuple<T...>
{
    static_assert(sizeof...(T) > 0, "KCellND cannot be of size 0 (space dimension)");
    static_assert(((T::levelShift() == KCellND::template kcell_type<0>::levelShift()) && ...), "All KCell must have the same level shift");

    constexpr KCellND(...) {}

    /// Cartesian product of enumerate result, converting inner KCells to KCellND
    template <
        typename Function
    >
    static constexpr auto enumerate_cartesian(Function && fn)
    {
        return details::enumerate_cartesian(std::forward<Function>(fn), KCellND{});
    }

    /// Concatenate the result of the function applied to each dimension separately (first only, then second only, then ...)
    template <
        typename Function
    >
    static constexpr auto dimension_concatenate(Function && fn)
    {
        return details::dimension_concatenate(std::forward<Function>(fn), KCellND{});
    }


    /// Topology defines the type of cell (eg in 2D: face, vertical edge, horizontal edge and vertex)
    static constexpr auto topology() noexcept
    {
        return std::apply(
            [](auto... t) { return (t + ...); },
            KCellND::enumerate(
                [](std::size_t i, auto kcell)
                {
                    return kcell.topology() << i;
                }
            )
        );
    }

    /// Dimension of the cell (eg in 2D: 2 for a face, 1 for a edge, 0 for a vertex)
    static constexpr auto dimension() noexcept
    {
        return KCellND::apply(
            [] (auto... cell) { return (cell.dimension() + ...); }
        );
    }

    /// Returns the index shift per dimension as a tuple
    static constexpr auto indexShift() noexcept
    {
        return KCellND::apply(
            [] (auto... cell) { return std::array<std::ptrdiff_t, sizeof...(T)>({cell.indexShift()...}); }
        );
    }

    /// Returns common shift of the representation level
    static constexpr auto levelShift() noexcept
    {
        return KCellND::template get<0>().levelShift();
    }

    /// Topology along a given direction
    template <
        std::size_t I
    >
    static constexpr bool isOpen() noexcept
    {
        return KCellND::template get<I>().isOpen();
    }

    /// Get the I-th spawn direction (the dimension's index)
    template <
        std::size_t I,
        std::size_t J = 0 // Internal index
    >
    static constexpr std::size_t direction() noexcept
    {
        static_assert(J < sizeof...(T), "Out of bound spawned direction");
        if constexpr (isOpen<J>())
        {
            if constexpr (I == 0)
                return J;
            else
                return direction<I - 1, J + 1>();
        }
        else
            return direction<I, J + 1>();
    }

    /// Get the I-th orthogonal direction (the dimension's index)
    template <
        std::size_t I,
        std::size_t J = 0 // Internal index
    >
    static constexpr std::size_t ortho_direction() noexcept
    {
        static_assert(J < sizeof...(T), "Out of bound orthogonal direction");
        if constexpr (not isOpen<J>())
        {
            if constexpr (I == 0)
                return J;
            else
                return ortho_direction<I - 1, J + 1>();
        }
        else
            return ortho_direction<I, J + 1>();
    }

    /// Get next cell (or the Steps-th next) of same topology in the direction I
    template <
        std::size_t I,
        std::ptrdiff_t Steps = 1
    >
    static constexpr auto next() noexcept
    {
        return enumerate_cartesian(
            [] (auto i, auto cell)
            {
                return cell.template next<(decltype(i)::value == I) ? Steps : 0>();
            }
        );
    }

    /// Get previous cell (or the Steps-th previous) of same topology in the direction I
    template <
        std::size_t I,
        std::ptrdiff_t Steps = 1
    >
    static constexpr auto prev() noexcept
    {
        return next<I, -Steps>();
    }

    /// Forward/backward move including incident cells in the direction I
    template <
        std::size_t I,
        std::ptrdiff_t Steps = 1
    >
    static constexpr auto incident() noexcept
    {
        return enumerate_cartesian(
            [] (auto i, auto cell)
            {
                return cell.template incident<(decltype(i)::value == I) ? Steps : 0>();
            }
        );
    }

    /// Neighborhood of incident cells of dimension dim-1 (eg for a face in 2D, it returns it's edges)
    static constexpr auto lowerIncident() noexcept
    {
        return dimension_concatenate(
            [] (auto, auto cell) { return cell.lowerIncident(); }
        );
    }

    /// Neighborhood of incident cells of dimension dim+1 (eg for a edge in 2D, it returns it's two neighbors faces)
    static constexpr auto upperIncident() noexcept
    {
        return dimension_concatenate(
            [] (auto, auto cell) { return cell.upperIncident(); }
        );
    }

    /// Neighborhood of proper (current cell not included) adjacent cells (same topology)
    template <
        std::size_t Distance = 1
    >
    static constexpr auto properNeighborhood() noexcept
    {
        if constexpr (Distance == 0)
            return KCells();
        else if constexpr (Distance == 1)
            return dimension_concatenate(
                [] (auto, auto cell) { return cell.properNeighborhood(); }
            );
        else
            return properNeighborhood<Distance - 1>().neighborhood().unique() - KCellND{};
    }

    /// Neighborhood (current cell not included) adjacent cells (same topology), including current cell
    template <
        std::size_t Distance = 1
    >
    static constexpr auto neighborhood() noexcept
    {
        return properNeighborhood<Distance>() + KCellND{};
    }

    /// Changing level while keeping the same topology
    template <std::ptrdiff_t Levels = 1>
    static constexpr auto up() noexcept
    {
        return enumerate_cartesian(
            [] (auto, auto cell) { return cell.template up<Levels>(); }
        );
    }

    /// Changing level while keeping the same topology
    template <std::ptrdiff_t Levels = 1>
    static constexpr auto down() noexcept
    {
        return up<-Levels>();
    }

    /// Apply the level/index shifts of the current kcell to a given index (or interval)
    template <
        typename... Index,
        typename = std::enable_if_t<sizeof...(Index) == sizeof...(T)>,
        typename = std::void_t<decltype(std::declval<Index>() + 1) ...> // Avoid conflict with the overload for Cell or CellInterval
    >
    static constexpr auto shift(Index &&... index) noexcept
    {
        auto indices = std::make_tuple(index...);

        return KCellND::enumerate(
            [&indices] (auto i, auto cell) { return cell.shift(std::get<decltype(i)::value>(indices)); }
        );
    }

    /// Apply the level/index shifts of the current kcell to a given indexable container (has operator[](std::size_t))
    template <
        typename Indices,
        typename = std::void_t<decltype(std::declval<Indices>()[0])>
    >
    static constexpr auto shift(Indices indices)
    {
        assert(indices.size() == KCellND::size() && "Invalid indices size");
        KCellND::enumerate(
            [&indices] (auto i, auto cell) { indices[i] = cell.shift(indices[i]); }
        );
        return indices;
    }

    /** Apply the level/index shifts of the current kcell to a given Samurai Cell
     * 
     * @pre It is up to the user to check if the given cell has an appropriate topology
     * @cond The returned Cell will have the same topology as the current KCell
     * @warning The returned CellInterval may have an invalid storage index, especially something (level or coordinates) other than the first coordinate is modified!
     *          This is up to the user to check if in the usage context, the index remains valid or not.
     */
    template <
        typename TInterval,
        std::size_t OtherTopology
    >
    static constexpr auto shift(samurai::Cell<KCellND::size(), TInterval, OtherTopology> const& cell) noexcept
    {
        return samurai::Cell<KCellND::size(), TInterval, topology()>(
            cell.level + levelShift(),
            shift(cell.indices),
            KCellND::template get<0>().shift(cell.index) // Warning: no guarantee that this shifted index is still valid!
        );
    }

    /** Apply the level/index shifts of the current kcell to a given Samurai CellInterval
     * 
     * @pre It is up to the user to check if the given cell interval has an appropriate topology
     * @cond The returned CellInterval will have the same topology as the current KCell
     * @warning The returned CellInterval may have an invalid storage index, especially something (level or coordinates) other than the first coordinate is modified!
     *          This is up to the user to check if in the usage context, the index remains valid or not.
     */
    template <
        typename TInterval,
        std::size_t OtherTopology
    >
    static constexpr auto shift(samurai::CellInterval<KCellND::size(), TInterval, OtherTopology> const& ci) noexcept
    {
        samurai::CellInterval<KCellND::size(), TInterval, topology()> new_ci(
            ci.level + levelShift(),
            KCellND::template get<0>().shift(ci.interval), // Warning: no guarantee that the storage index is still valid!
            ci.indices
        );
        KCellND::enumerate(
            [&new_ci] (auto i, auto cell)
            {
                if (i > 0)
                    new_ci.indices[i - 1] = cell.shift(new_ci.indices[i - 1]);
            }
        );
        return new_ci;     
    }

    template <
        typename Function,
        typename... Index,
        typename = std::enable_if_t<sizeof...(Index) == sizeof...(T)>,
        typename = std::enable_if_t<std::is_invocable_v<Function, std::size_t, Index...>>
    >
    static constexpr decltype(auto) shift(Function && fn, std::size_t level, Index && ... index)
    {
        auto indices = shift(std::forward<Index>(index)...);
        return std::apply(
            [&fn, level] (auto... idx) -> decltype(auto)
            {
                return std::forward<Function>(fn)(level + levelShift(), idx...);
            },
            indices
        );
    }

    template <
        typename Function,
        typename TInterval,
        std::size_t OtherTopology
    >
    static constexpr decltype(auto) shift(Function && fn, samurai::Cell<KCellND::size(), TInterval, OtherTopology> const& cell)
    {
        return std::forward<Function>(fn)(shift(cell));
    }

    template <
        typename Function,
        typename TInterval,
        std::size_t OtherTopology
    >
    static constexpr decltype(auto) shift(Function && fn, samurai::CellInterval<KCellND::size(), TInterval, OtherTopology> const& cell_interval)
    {
        return std::forward<Function>(fn)(shift(cell_interval));
    }

};

/// Deduction guide
template <typename... T>
KCellND(T...) -> KCellND<std::decay_t<T>...>;

template <typename... T>
KCellND(std::tuple<T...>) -> KCellND<T...>;

template <typename... T>
KCellND(KCells<T...>) -> KCellND<T...>;

/// Factories
template <
    std::size_t Dimension,
    std::size_t Topology
>
constexpr auto make_KCellND() noexcept
{
    return details::make_KCellND_helper<Topology>(std::make_index_sequence<Dimension>{});
}

template <
    std::size_t Dimension
>
constexpr auto make_KCellND() noexcept
{
    return make_KCellND<Dimension, (1u << Dimension) - 1>();
}

/// Fancy display
template <typename... T, typename = std::enable_if_t<(sizeof...(T) > 0)>>
std::ostream& operator<< (std::ostream& out, KCellND<T...> const& kcell)
{
    out << "KCellND(";
    out << "topology=";
    kcell.foreach([&out] (auto cell) { out << cell.topology(); });
    out << "b, IndexShift=(";
    kcell.enumerate([&out, &kcell] (auto i, auto cell) { out << cell.indexShift() << ((i < kcell.size() - 1) ? "," : ""); });
    out << "), LevelShift=" << kcell.levelShift();
    out << ")";
    return out;
}

