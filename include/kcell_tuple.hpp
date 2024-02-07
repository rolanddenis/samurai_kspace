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
    /// Replace void return by None type in order to be assigned
    template <typename Function, typename... Args>
    constexpr decltype(auto) valid_return(Function && fn, Args && ... args)
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
        // FIXME: find a better way...
        if constexpr (sizeof...(I) > 1)
            // Using std::tuple{} to enforce execution order of the fold expression
            return std::tuple{valid_return(fn, kcells.template get<I>()) ...};
        else
            // With only one argument, that is possibly a tuple, use make_tuple to enforce creating nested tuples
            // instead of calling copy constructor.
            return std::make_tuple(valid_return(fn, kcells.template get<I>()) ...);

    }

    template <
        typename Function,
        typename... T
    >
    constexpr auto foreach(Function && fn, KCellTuple<T...> const& kcells)
    {
        return foreach_impl(
            std::forward<Function>(fn),
            kcells,
            std::make_index_sequence<KCellTuple<T...>::size()>{}
        );
    }

    template <
        typename Function,
        typename... T,
        std::size_t... I
    >
    constexpr auto enumerate_impl(Function && fn, KCellTuple<T...> const& kcells, std::index_sequence<I...>)
    {
        if constexpr (sizeof...(I) > 1)
            // Using std::tuple{} to enforce execution order of the fold expression
            return std::tuple{valid_return(fn, std::integral_constant<std::size_t, I>{}, kcells.template get<I>()) ...};
        else
            // With only one argument, that is possibly a tuple, use make_tuple to enforce creating nested tuples
            // instead of calling copy constructor.
            return std::make_tuple(valid_return(fn, std::integral_constant<std::size_t, I>{}, kcells.template get<I>()) ...);
    }

    template <
        typename Function,
        typename... T
    >
    constexpr auto enumerate(Function && fn, KCellTuple<T...> const& kcells)
    {
        return enumerate_impl(
            std::forward<Function>(fn),
            kcells,
            std::make_index_sequence<KCellTuple<T...>::size()>{}
        );
    }

    template <
        typename Function,
        typename... T,
        std::size_t... I
    >
    constexpr decltype(auto) apply_impl(Function && fn, KCellTuple<T...> const& kcells, std::index_sequence<I...>)
    {
        return std::forward<Function>(fn)(kcells.template get<I>() ...);
    }

    template <
        typename Function,
        typename... T
    >
    constexpr decltype(auto) apply(Function && fn, KCellTuple<T...> const& kcells)
    {
        return apply_impl(
            std::forward<Function>(fn),
            kcells,
            std::make_index_sequence<KCellTuple<T...>::size()>{}
        );
    }

    template <typename T> struct is_kcell : std::false_type {};

    template <typename T> constexpr bool is_kcell_v = is_kcell<T>::value;

    template <
        typename... T,
        typename KCell
    >
    static constexpr bool has(KCellTuple<T...>, KCell) noexcept
    {
        return (std::is_same_v<T, KCell> || ... || false);
    }

    template <
        typename... U
    >
    static constexpr auto unique_helper(KCellTuple<U...>, KCellTuple<> const&) noexcept
    {
        // "KCellTuple<> const&" in the parameter list to delay instantiation check
        return KCellTuple<U...>{};
    }

    template <
        typename... U,
        typename Head,
        typename... Tail
    >
    static constexpr auto unique_helper(KCellTuple<U...>, KCellTuple<Head, Tail...>) noexcept
    {
        if constexpr (KCellTuple<U...>::template has(Head{}))
            return unique_helper(KCellTuple<U...>{}, KCellTuple<Tail...>{});
        else
            return unique_helper(KCellTuple<U..., Head>{}, KCellTuple<Tail...>{});
    }

    template <
        typename... T,
        typename... U, // Fake variadic template to delay instantiation check
        typename = std::enable_if_t<sizeof...(U) == 0>
    >
    static constexpr auto unique(KCellTuple<T...>) noexcept
    {
        return unique_helper(KCellTuple<U...>{}, KCellTuple<T...>{});
    }
}

/// Simple tuple of KCell 
template <typename... T>
struct KCellTuple
{
    static_assert((details::is_kcell_v<T> && ...), "KCellTuple must be composed of KCell");

    template <std::size_t I>
    using kcell_type = std::tuple_element_t<I, std::tuple<T...>>;

    constexpr KCellTuple(...) {}

    /// Number of elements in the tuple
    static constexpr std::size_t size() noexcept { return sizeof...(T); }

    /// Get the element at given index
    template <std::size_t I>
    static constexpr auto get() noexcept
    {
        return std::decay_t<decltype(std::get<I>(std::declval<std::tuple<T...>>()))>{};
    }

    /// Call the given function to each element and pack the returned values in a tuple
    template <typename Function>
    static constexpr auto foreach(Function && fn)
    {
        return details::foreach(std::forward<Function>(fn), KCellTuple{});
    }

    /** For each element, call the function with its index and the element
     * and pack the returned values in a tuple
     */
    template <typename Function>
    static constexpr auto enumerate(Function && fn)
    {
        return details::enumerate(std::forward<Function>(fn), KCellTuple{});
    }

    /// Unfold all elements as the arguments of the given function
    template <typename Function>
    static constexpr decltype(auto) apply(Function && fn)
    {
        return details::apply(std::forward<Function>(fn), KCellTuple{});
    }

    /// Membership operator (O(N))
    template <typename KCell>
    static constexpr bool has(KCell const& kcell) noexcept
    {
        return details::has(KCellTuple{}, kcell);
    }

    /// Remove the duplicated element type and returns the resulting KCellTuple (O(N²))
    static constexpr auto unique() noexcept
    {
        return details::unique(KCellTuple{});
    }
};

/// Deduction guide
template <typename... T>
KCellTuple(T...) -> KCellTuple<std::decay_t<T>...>;

template <typename... T>
KCellTuple(std::tuple<T...>) -> KCellTuple<T...>;

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
    kcells.enumerate([&out, &kcells] (auto i, auto cell) { out << cell << ((i < kcells.size() - 1) ? ", " : ""); });
    out << "}";
    return out;
}
