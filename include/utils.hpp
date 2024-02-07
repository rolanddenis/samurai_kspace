#pragma once

#include <array>
#include <utility>
#include <string>

#include <xtensor/xfixed.hpp>

constexpr bool is_open(std::size_t topology, std::size_t direction) noexcept;

namespace details
{
    template <
        std::size_t... I
    >
    constexpr auto decompose_topology(std::size_t topology, std::index_sequence<I...>)
    {
        return std::array<bool, sizeof...(I)>{is_open(topology, I) ...};
    }

    template <
        std::size_t... I
    >
    constexpr std::size_t cell_dimension(std::size_t topology, std::index_sequence<I...>)
    {
        return ((is_open(topology, I) ? 1ul : 0ul) + ...);
    }

    template <
        std::size_t... I
    >
    auto topology_as_xtensor(std::size_t topology, std::index_sequence<I...>)
    {
        return xt::xtensor_fixed<int, xt::xshape<sizeof...(I)>>{(is_open(topology, I) ? 1 : 0) ...};
    }

}

/// @brief Returns true if the cell of given topology (integer) is open along given direction
/// @param topology     Topology of the cell as an integer.
/// @param direction    Direction to consider.
/// @return             True if the cell is open along given direction, false if closed.
constexpr bool is_open(std::size_t topology, std::size_t direction) noexcept
{
    return (topology >> direction) % 2 != 0;
}

/** @brief Decompose a N-dimensional topology (integer) as an array of topology per dimension
 *  @tparam SpaceDimension  Dimension of the space
 *  @param topology         Topology of the cell as an integer
 *  @return                 Array of boolean with the topology (open <=> true) per dimension
 */
template <
    std::size_t SpaceDimension
>
constexpr std::array<bool, SpaceDimension> decompose_topology(std::size_t topology) noexcept
{
    return details::decompose_topology(topology, std::make_index_sequence<SpaceDimension>{});
}

/// @brief Dimension of a cell given it's topology
/// @tparam SpaceDimension  Dimension of the space
/// @param topology         Topology of the cell as an integer
/// @return                 Dimension of the cell
template <
    std::size_t SpaceDimension
>
constexpr std::size_t cell_dimension(std::size_t topology) noexcept
{
    return details::cell_dimension(topology, std::make_index_sequence<SpaceDimension>{});
}

template <
    std::size_t SpaceDimension
>
std::string topology_as_string(std::size_t topology)
{
    std::string s = "0b";
    for (std::size_t i = 0; i < SpaceDimension; ++i)
        s += (is_open(topology, i) ? "1" : "0");
    return s;
}

template <
    std::size_t SpaceDimension
>
auto topology_as_xtensor(std::size_t topology)
{
    return details::topology_as_xtensor(topology, std::make_index_sequence<SpaceDimension>{});
}