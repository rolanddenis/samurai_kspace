#pragma once

#include <cstddef>
#include <ostream>

struct Interval
{
    std::size_t a, b, step = 1;

    Interval& operator<<= (std::size_t s) noexcept
    {
        a <<= s;
        b = ((b - 1) << s) + 1;
        step <<= s;
        return *this;
    }

    Interval& operator>>= (std::size_t s) noexcept
    {
        a >>= s;
        b = ((b - 1) >> s) + 1;
        step >>= s;
        if (step == 0) step = 1;
        return *this;
    }

    Interval& operator+= (std::ptrdiff_t v) noexcept { a += v; b += v; return *this; }
};

inline Interval operator<< (Interval i, std::size_t s) noexcept
{
    i <<= s;
    return i;
}

inline Interval operator>> (Interval i, std::size_t s) noexcept
{
    i >>= s;
    return i;
}

inline Interval operator+ (Interval i, std::ptrdiff_t v) noexcept
{
    i += v;
    return i;
}

std::ostream & operator<< (std::ostream & out, Interval const& i)
{
    out << "[" << i.a << "," << i.b << "[:" << i.step;
    return out;
}
