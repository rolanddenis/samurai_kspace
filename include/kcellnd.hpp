#pragma once

#include <type_traits>

#include "kcell.hpp"
#include "kcell_tuple.hpp"



template <typename... T>
struct KCellND : KCellTuple<T...>
{
    static_assert(sizeof...(T) > 0, "KCellND cannot be of dimension 0");
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

    static constexpr auto dimension() noexcept
    {
        return KCellND::apply(
            [](auto... cell) { return (cell.topology() + ...); }
        );
    }

    /*
    template <
        std::size_t I,
        std::ptrdiff_t Steps = 1
    >
    static constexpr auto next() noexcept
    {
        return KCellND::
    }*/

};

/// Deduction guide
template <typename... T>
KCellND(T...) -> KCellND<std::decay_t<T>...>;

template <typename... T, typename = std::enable_if_t<(sizeof...(T) > 0)>>
std::ostream& operator<< (std::ostream& out, KCellND<T...> const& kcell)
{
    out << "KCellND{";
    kcell.foreach([&out] (auto cell) { out << cell << ", "; });
    out << "}";
    return out;
}

