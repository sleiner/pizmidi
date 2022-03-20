// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <array>
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <utility>

#include <juce_core/juce_core.h>

namespace piz
{

/// A thin wrapper around std::array.
///
/// JUCE has different conventions than the STL, notably the use of int for indices instead of
/// std::size_t as well as always checking array bounds at runtime. This class adapts std::array
/// to these conventions, but otherwise keeps the API design of std::array which is quite different
/// from juce::Array.
template <typename ElementType, std::size_t N>
class Array : public std::array<ElementType, N>
{
    using Base = std::array<ElementType, N>;

public:
    using size_type              = int;
    using value_type             = typename Base::value_type;
    using difference_type        = typename Base::difference_type;
    using reference              = typename Base::reference;
    using const_reference        = typename Base::const_reference;
    using pointer                = typename Base::pointer;
    using const_pointer          = typename Base::const_pointer;
    using iterator               = typename Base::iterator;
    using const_iterator         = typename Base::const_iterator;
    using reverse_iterator       = typename Base::reverse_iterator;
    using const_reverse_iterator = typename Base::const_reverse_iterator;

    using std::array<ElementType, N>::array;

    constexpr reference at (int pos) { return Base::at (toIdx (pos)); }
    constexpr const_reference at (int pos) const { return Base::at (toIdx (pos)); }

    // indexed access is always bounds-checked
    constexpr reference operator[] (int pos) { return Base::at (toIdx (pos)); }
    constexpr const_reference operator[] (int pos) const { return Base::at (toIdx (pos)); }

    constexpr size_type size() const noexcept { return toInt (Base::size()); }
    constexpr size_type max_size() const noexcept { return toInt (Base::max_size()); }

private:
    static int toInt (std::size_t s)
    {
        if (s > INT_MAX)
        {
            throw std::out_of_range ("Array index was too large to fit into int");
        }
        return static_cast<int> (s);
    }

    static std::size_t toIdx (int i)
    {
        if (i < 0)
        {
            throw std::out_of_range ("Array index was < 0");
        }
        return static_cast<std::size_t> (i);
    }
};

} // namespace piz
