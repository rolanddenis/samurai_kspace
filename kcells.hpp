#pragma once

#include <cstdlib>
#include <tuple>
#include <utility>
#include <ostream>
#include <tuple>

#include "kcell_tuple.hpp"

/// Collection of KCell
template <typename... T>
struct KCells : KCellTuple<T...>
{
    constexpr KCells(...) {}

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

    template <typename Index>
    static constexpr auto shift(Index && i) noexcept
    {
        return KCells::apply(
            [&i] (auto... cell) { return std::make_tuple(cell.shift(i)...); }
        );
    }
    
    template <typename Function, typename Index>
    static constexpr void shift(Function && fn, std::size_t level, Index && i)
    {
        return KCells::apply(
            [&fn, &level, &i] (auto... cell) { (cell.shift(fn, level, i), ...); }
        );
    }

};

/// Deduction guide
template <typename... T>
KCells(T...) -> KCells<std::decay_t<T>...>;

/// Concatenation of KCells
template <typename... T, typename... U>
constexpr KCells<T..., U...> operator+ (KCells<T...>, KCells<U...>) noexcept
{
    return {};
}

template <typename... T>
std::ostream& operator<< (std::ostream& out, KCells<T...> kcells)
{
    out << "KCells{";
    kcells.foreach([&out] (auto cell) { out << cell << ", "; });
    out << "}";
    return out;
}
