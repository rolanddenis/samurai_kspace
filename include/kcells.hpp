#pragma once

#include <cstdlib>
#include <tuple>
#include <utility>
#include <ostream>
#include <tuple>
#include <type_traits>

#include "kcell_tuple.hpp"

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

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto next() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template next<Steps>() + ...); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto prev() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template prev<Steps>() + ...); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto nextIncident() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template nextIncident<Steps>() + ...); }
        );
    }

    template <std::ptrdiff_t Levels = 1>
    static constexpr auto up() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template up<Levels>() + ...); }
        );
    }

    template <std::ptrdiff_t Levels = 1>
    static constexpr auto down() noexcept
    {
        return KCells::apply(
            [] (auto... cell) { return (cell.template down<Levels>() + ...); }
        );
    }

    template <
        typename... Index,
        typename = std::enable_if_t<(kcell_size() > 0 && sizeof...(Index) == kcell_size())>
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
        typename = std::enable_if_t<(kcell_size() > 0 && sizeof...(Index) == kcell_size())>
    >
    static constexpr void shift(Function && fn, std::size_t level, Index && ... i)
    {
        return KCells::apply(
            [&fn, &level, &i...] (auto... cell) { (cell.shift(fn, level, i...), ...); }
        );
    }

};

/// Deduction guide
template <typename... T>
KCells(T...) -> KCells<std::decay_t<T>...>;

template <typename... T>
KCells(std::tuple<T...>) -> KCells<T...>;

/// Concatenation of KCells
template <typename... T, typename... U>
constexpr KCells<T..., U...> operator+ (KCells<T...>, KCells<U...>) noexcept
{
    return {};
}

/// Appending a new KCell
template <
    typename... T,
    bool Open,
    std::ptrdiff_t IndexShift,
    std::ptrdiff_t LevelShift
>
constexpr KCells<T..., KCell<Open, IndexShift, LevelShift>> operator+ (KCells<T...>, KCell<Open, IndexShift, LevelShift>) noexcept
{
    return {};
}


template <typename... T>
std::ostream& operator<< (std::ostream& out, KCells<T...> const& kcells)
{
    out << "KCells{";
    kcells.enumerate([&out, &kcells] (auto i, auto cell) { out << cell << ((i < kcells.size() - 1) ? ", " : ""); });
    out << "}";
    return out;
}
