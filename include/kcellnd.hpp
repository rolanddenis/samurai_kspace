#pragma once

#include <type_traits>
#include <array>

#include "kcell.hpp"
#include "kcell_tuple.hpp"

// Pre-declaration
template <typename... T>
struct KCellND;

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

    /// Factories
    template <
        std::size_t Topology,
        std::size_t... I
    >
    constexpr auto make_KCellND_helper(std::index_sequence<I...>) noexcept
    {
        return KCellND<KCell<(Topology & (1 << I)) != 0> ...>{};
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
        return KCellND::template get<I>.isOpen();
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

    /// Neighborhood of incident cells of dimension dim-1 (eg for a face in 2D, it returns it's edges)
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

    /// Apply the level/index shifts of the current cell to a given index (or interval)
    template <
        typename... Index,
        typename = std::enable_if_t<sizeof...(Index) == sizeof...(T)>
    >
    static constexpr auto shift(Index &&... index) noexcept
    {
        auto indices = std::make_tuple(index...);

        return KCellND::enumerate(
            [&indices] (auto i, auto cell) { return cell.shift(std::get<decltype(i)::value>(indices)); }
        );
    }

    template <
        typename Function,
        typename... Index,
        typename = std::enable_if_t<sizeof...(Index) == sizeof...(T)>
    >
    static constexpr auto shift(Function && fn, std::size_t level, Index && ... index)
    {
        auto indices = shift(index...);
        return std::apply(
            [&fn, level] (auto... idx)
            {
                return std::forward<Function>(fn)(level + levelShift(), idx...);
            },
            indices
        );
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

