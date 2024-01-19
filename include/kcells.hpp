#pragma once

#include <cstdlib>
#include <tuple>
#include <utility>
#include <ostream>
#include <tuple>
#include <type_traits>

#include "kcell_tuple.hpp"

// Forward declaration
template <typename... T>
struct KCells;

namespace details
{
    template <
        typename... U,
        typename... V
    >
    constexpr auto subtraction_helper(KCells<> const&, KCells<U...> const&, KCells<V...> const& result) noexcept
    {
        return result;
    }

    template <
        typename THead,
        typename... TTail,
        typename... U,
        typename... V
    >
    constexpr auto subtraction_helper(KCells<THead, TTail...> const&, KCells<U...> const& rhs, KCells<V...> const& result) noexcept
    {
        if constexpr (KCells<U...>::has(THead{}))
            return subtraction_helper(KCells<TTail...>{}, rhs, result);
        else
            return subtraction_helper(KCells<TTail...>{}, rhs, KCells<V..., THead>{});
    }

    /// Removing from lhs the KCell from rhs
    template <
        typename... T,
        typename... U,
        typename... V,
        typename = std::enable_if<sizeof...(V) == 0>
    >
    constexpr auto subtraction(KCells<T...> const& lhs, KCells<U...> const& rhs) noexcept
    {
        return details::subtraction_helper(lhs, rhs, KCells<V...>{});
    }
}

/// Collection of KCell
template <typename... T>
struct KCells : KCellTuple<T...>
{

    static_assert(((T::size() == KCells::template kcell_type<0>::size()) && ...), "All KCell must live in the same space's dimension");

    constexpr KCells(...) {}

    static constexpr std::size_t kcell_size() noexcept
    {
        if constexpr (sizeof...(T) > 0)
            return KCells::template kcell_type<0>::size();
        else
            return 0;
    }

    /// Returns the index shift per dimension for each cell (tuple of (tuple | int))
    static constexpr auto indexShift() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return std::make_tuple(cell.indexShift()...); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto next() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template next<Steps>() + ... + KCells<>{} ); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto prev() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template prev<Steps>() + ... + KCells<>{} ); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto incident() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template incident<Steps>() + ... + KCells<>{}); }
        );
    }

    static constexpr auto lowerIncident() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.lowerIncident() + ... + KCells<>{}); }
        );
    }

    static constexpr auto upperIncident() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.upperIncident() + ... + KCells<>{}); }
        );
    }

    static constexpr auto properNeighborhood() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.properNeighborhood() + ... + KCells<>{}); }
        );
    }

    static constexpr auto neighborhood() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.neighborhood() + ... + KCells<>{}); }
        );
    }

    template <std::ptrdiff_t Levels = 1>
    static constexpr auto up() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template up<Levels>() + ... + KCells<>{}); }
        );
    }

    template <std::ptrdiff_t Levels = 1>
    static constexpr auto down() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template down<Levels>() + ... + KCells<>{}); }
        );
    }

    template <
        typename... Index,
        typename = std::enable_if_t<(kcell_size() == 0 || sizeof...(Index) == kcell_size())>
    >
    static constexpr auto shift(Index && ... i) noexcept
    {
        return KCells::apply(
            [&i...] (auto... cell) { return std::make_tuple(cell.shift(i...)...); }
        );
    }
    
    template <
        typename Function,
        typename... Index,
        typename = std::enable_if_t<(kcell_size() == 0 || sizeof...(Index) == kcell_size())>
    >
    static constexpr void shift(Function && fn, std::size_t level, Index && ... i)
    {
        return KCells::apply(
            [&fn, &level, &i...] (auto... cell) { (cell.shift(fn, level, i...), ...); }
        );
    }

    /// Remove the duplicated element type and returns the resulting KCellTuple (O(N²))
    static constexpr auto unique() noexcept
    {
        // FIXME: CRTP?
        return KCellTuple<T...>::unique().apply(
            [] (auto... kcell) { return KCells<decltype(kcell)...>{}; }
        );
    }
};

/// Deduction guide
template <typename... T>
KCells(T...) -> KCells<std::decay_t<T>...>;

template <typename... T>
KCells(std::tuple<T...>) -> KCells<T...>;

template <typename... T>
KCells(KCellTuple<T...>) -> KCells<T...>;

/// Concatenation of KCells
template <typename... T, typename... U>
constexpr KCells<T..., U...> operator+ (KCells<T...>, KCells<U...>) noexcept
{
    return {};
}

/// Appending a new KCell
template <
    typename... T,
    typename U,
    typename = std::enable_if<details::is_kcell_v<U>>
>
constexpr KCells<T..., U> operator+ (KCells<T...>, U) noexcept
{
    return {};
}

/// Prepending a new KCell
template <
    typename... T,
    typename U,
    typename = std::enable_if<details::is_kcell_v<U>>
>
constexpr KCells<U, T...> operator+ (U, KCells<T...>) noexcept
{
    return {};
}

/// Removing from lhs all the KCell from rhs
template <
    typename... T,
    typename... U
>
constexpr auto operator- (KCells<T...> const& lhs, KCells<U...> const& rhs) noexcept
{
    return details::subtraction(lhs, rhs);
}

/// Removing from lhs a KCell
template <
    typename... T,
    typename U,
    typename = std::enable_if<details::is_kcell_v<U>>
>
constexpr auto operator- (KCells<T...>, U) noexcept
{
    return KCells<T...>{} - KCells<U>{};
}

template <typename... T>
std::ostream& operator<< (std::ostream& out, KCells<T...> const& kcells)
{
    out << "KCells{";
    kcells.enumerate([&out, &kcells] (auto i, auto cell) { out << cell << ((i < kcells.size() - 1) ? ", " : ""); });
    out << "}";
    return out;
}
