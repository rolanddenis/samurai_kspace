#pragma once

#include <cstdlib>
#include <tuple>
#include <utility>
#include <ostream>
#include <tuple>

// Pre-declaration
template <
    bool Open,
    std::ptrdiff_t IndexShift,
    std::ptrdiff_t LevelShift
>
struct KCell;

// Pre-declaration
template <typename... T>
struct KCellTuple;

// No return
struct None {};

namespace details
{
    template <typename Function, typename... Args>
    auto valid_return(Function && fn, Args && ... args)
    {
        if constexpr (std::is_void_v<std::invoke_result_t<Function, Args...>>)
        {
            std::forward<Function>(fn)(std::forward<Args>(args) ...);
            return None{};
        }
        else
            return std::forward<Function>(fn)(std::forward<Args>(args) ...);
    }

    template <
        typename Function,
        typename... T,
        std::size_t... I
    >
    constexpr auto foreach_impl(Function && fn, KCellTuple<T...> const& kcells, std::index_sequence<I...>)
    {
        return std::make_tuple(valid_return(fn, kcells.template get<I>()) ...);
    }

    template <
        typename Function,
        typename... T
    >
    static constexpr void foreach(Function && fn, KCellTuple<T...> const& kcells)
    {
        foreach_impl(
            std::forward<Function>(fn),
            kcells,
            std::make_index_sequence<kcells.size()>{}
        );
    }

    template <
        typename Function,
        typename... T,
        std::size_t... I
    >
    constexpr auto enumerate_impl(Function && fn, KCellTuple<T...> const& kcells, std::index_sequence<I...>)
    {
        return std::make_tuple(valid_return(fn, I, kcells.template get<I>()) ...);
    }

    template <
        typename Function,
        typename... T
    >
    static constexpr auto enumerate(Function && fn, KCellTuple<T...> const& kcells)
    {
        return enumerate_impl(
            std::forward<Function>(fn),
            kcells,
            std::make_index_sequence<kcells.size()>{}
        );
    }

    template <
        typename Function,
        typename... T,
        std::size_t... I
    >
    constexpr auto apply_impl(Function && fn, KCellTuple<T...> const& kcells, std::index_sequence<I...>)
    {
        return fn(kcells.template get<I>() ...);
    }

    template <
        typename Function,
        typename... T
    >
    static constexpr auto apply(Function && fn, KCellTuple<T...> const& kcells)
    {
        return apply_impl(
            std::forward<Function>(fn),
            kcells,
            std::make_index_sequence<kcells.size()>{}
        );
    }

    template <typename T> struct is_kcell : std::false_type {};

    template <
        bool Open,
        std::ptrdiff_t IndexShift,
        std::ptrdiff_t LevelShift
    >
    struct is_kcell<KCell<Open, IndexShift, LevelShift>> : std::true_type {};

    template <typename T> constexpr bool is_kcell_v = is_kcell<T>::value;

}

/// Simple tuple of KCell 
template <typename... T>
struct KCellTuple
{
    static_assert((details::is_kcell_v<T> && ...), "KCellTuple must be composed of KCell");

    template <std::size_t I>
    using kcell_type = std::tuple_element_t<I, std::tuple<T...>>;

    constexpr KCellTuple(...) {}

    static constexpr std::size_t size() noexcept { return sizeof...(T); }

    template <std::size_t I>
    static constexpr auto get() noexcept
    {
        return std::decay_t<decltype(std::get<I>(std::declval<std::tuple<T...>>()))>{};
    }

    template <typename Function>
    static constexpr void foreach(Function && fn)
    {
        details::foreach(std::forward<Function>(fn), KCellTuple{});
    }

    template <typename Function>
    static constexpr auto apply(Function && fn)
    {
        return details::apply(std::forward<Function>(fn), KCellTuple{});
    }

    template <typename Function>
    static constexpr auto enumerate(Function && fn)
    {
        return details::enumerate(std::forward<Function>(fn), KCellTuple{});
    }
};

/// Deduction guide
template <typename... T>
KCellTuple(T...) -> KCellTuple<std::decay_t<T>...>;

template <
    std::size_t I,
    typename... T
>
constexpr auto get(KCellTuple<T...>) noexcept
{
    return KCellTuple<T...>{}.template get<I>();
}

template <typename... T>
constexpr std::size_t size(KCellTuple<T...>) noexcept
{
    return KCellTuple<T...>{}.template size();
}

template <typename... T>
std::ostream& operator<< (std::ostream& out, KCellTuple<T...> const& kcells)
{
    out << "KCellTuple{";
    kcells.foreach([&out] (auto cell) { out << cell << ", "; });
    out << "}";
    return out;
}
