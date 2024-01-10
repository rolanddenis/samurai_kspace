#pragma once

#include <type_traits>

#include "kcell.hpp"
#include "kcell_tuple.hpp"



template <typename... T>
struct KCellND : KCellTuple<T...>
{
    static_assert(((T::levelShift() == KCellND::template kcell_type<0>::levelShift()) && ...), "All KCell must have the same level shift");

    constexpr KCellND(...) {}

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

};

/// Deduction guide
template <typename... T>
KCellND(T...) -> KCellND<std::decay_t<T>...>;
