#pragma once

#include <cstdlib>
#include <tuple>
#include <utility>
#include <ostream>
#include <tuple>


/// Collection of KCell
template <typename... T>
struct KCells;

namespace details
{
    template <
        typename... T,
        typename Function,
        std::size_t... I
    >
    constexpr void foreach(Function && fn, KCells<T...> const& kcells, std::index_sequence<I...>)
    {
        (fn(kcells.template get<I>()), ...);
    }

    template <
        typename... T,
        typename Function,
        std::size_t... I
    >
    constexpr auto apply(Function && fn, KCells<T...> const& kcells, std::index_sequence<I...>)
    {
        return fn(kcells.template get<I>() ...);
    }
}

template <typename... T>
struct KCells
{
    constexpr KCells(...) {}

    static constexpr std::size_t size() noexcept { return sizeof...(T); }

    template <std::size_t I>
    static constexpr auto get() noexcept
    {
        return std::decay_t<decltype(std::get<I>(std::declval<std::tuple<T...>>()))>{};
    }

    template <typename Function>
    static constexpr void foreach(Function && fn)
    {
        details::foreach(
            std::forward<Function>(fn),
            KCells<T...>{},
            std::make_index_sequence<size()>{}
        );
    }

    template <typename Function>
    static constexpr auto apply(Function && fn)
    {
        return details::apply(
            std::forward<Function>(fn),
            KCells<T...>{},
            std::make_index_sequence<size()>{}
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto next() noexcept
    {
        return apply(
            [] (auto... cell) { return (cell.template next<Steps>() + ...); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto prev() noexcept
    {
        return apply(
            [] (auto... cell) { return (cell.template prev<Steps>() + ...); }
        );
    }

    template <std::ptrdiff_t Steps = 1>
    static constexpr auto nextIncident() noexcept
    {
        return apply(
            [] (auto... cell) { return (cell.template nextIncident<Steps>() + ...); }
        );
    }

    template <std::ptrdiff_t Levels = 1>
    static constexpr auto up() noexcept
    {
        return apply(
            [] (auto... cell) { return (cell.template up<Levels>() + ...); }
        );
    }

    template <std::ptrdiff_t Levels = 1>
    static constexpr auto down() noexcept
    {
        return apply(
            [] (auto... cell) { return (cell.template down<Levels>() + ...); }
        );
    }

    template <typename Index>
    static constexpr auto shift(Index && i) noexcept
    {
        return apply(
            [&i] (auto... cell) { return std::make_tuple(cell.shift(i)...); }
        );
    }
    
    template <typename Function, typename Index>
    static constexpr void shift(Function && fn, std::size_t level, Index && i)
    {
        return apply(
            [&fn, &level, &i] (auto... cell) { (cell.shift(fn, level, i), ...); }
        );
    }

};

/// Deduction guide
template <typename... T>
KCells(T...) -> KCells<std::decay_t<T>...>;

template <
    std::size_t I,
    typename... T
>
constexpr auto get(KCells<T...>) noexcept
{
    return KCells<T...>{}.template get<I>();
}

template <typename... T>
constexpr std::size_t size(KCells<T...>) noexcept
{
    return KCells<T...>{}.template size();
}

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
