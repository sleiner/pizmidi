// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <utility>

#include <juce_core/juce_core.h>

namespace piz
{

/// A wrapper around juce::Array with custom extensions.
/// Note that this class is very different from piz::Array.
template <typename ElementType,
          typename TypeOfCriticalSectionToUse = juce::DummyCriticalSection,
          int minimumAllocatedSize            = 0>
class JArray : public juce::Array<ElementType, TypeOfCriticalSectionToUse, minimumAllocatedSize>
{
public:
    using juce::Array<ElementType, TypeOfCriticalSectionToUse, minimumAllocatedSize>::Array;
    using juce::Array<ElementType, TypeOfCriticalSectionToUse, minimumAllocatedSize>::operator=;

    template <class UnaryPredicate>
    ElementType findIf (UnaryPredicate p) const
    {
        for (auto&& element : *this)
        {
            if (p (element))
            {
                return element;
            }
        }
        return ElementType();
    }
};

} // namespace piz
