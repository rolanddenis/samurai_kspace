#pragma once

#include <array>
#include <xtensor/xfixed.hpp>

namespace details
{
    template <typename T>
    struct ArrayData
    {
        using type = T;
    };

    template <typename T, std::size_t N>
    struct ArrayData<std::array<T, N>>
    {
        using type = typename ArrayData<T>::type;
    };

    template <typename T>
    using array_data_type = typename ArrayData<T>::type;

    template <
        typename T,
        std::size_t... S
    >
    auto get_shape_impl(xt::xshape<S...> shape, T) noexcept
    {
        return shape;
    }

    template <
        typename T,
        std::size_t N,
        std::size_t... S
    >
    auto get_shape_impl(xt::xshape<S...>, std::array<T, N>) noexcept
    {
        return get_shape_impl(xt::xshape<S..., N>{}, T{});
    }

    template <typename T, std::size_t N>
    auto get_shape(std::array<T, N> const& array) noexcept
    {
        return get_shape_impl(xt::xshape<>{}, array);
    }

}

template <typename T, std::size_t N>
auto xadapt_array(std::array<T, N> const& array) noexcept
{
    return xt::adapt(reinterpret_cast<details::array_data_type<T> const*>(&array), details::get_shape(array));
}

template <typename T, std::size_t N>
auto array_to_fixed(std::array<T, N> const& array) noexcept
{
    return xt::xtensor_fixed<details::array_data_type<T>, decltype(details::get_shape(array))>(xadapt_array(array));
}