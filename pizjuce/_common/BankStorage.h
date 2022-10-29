// SPDX-FileCopyrightText:  2021 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

class BankStorage
{
public:
    BankStorage (juce::Identifier name, int numBanks = 128, int numPrograms = 128);

    void set (int bank, int program, juce::String name, juce::var value);
    juce::var get (int bank, int program, juce::String name);

    void setGlobal (juce::String name, juce::var value);
    juce::var getGlobal (juce::String name);

    void loadFrom (juce::ValueTree const& vt);
    bool loadNoteMatrixFrom (juce::ValueTree const& vt, int program);
    void dumpTo (juce::MemoryBlock& destination);
    void dumpProgramTo (int bank, int program, juce::MemoryBlock& destination);

    int getNumBanks() const { return numBanks_; }
    int getNumPrograms() const { return numPrograms_; }

protected:
    juce::ValueTree values_;

private:
    int numBanks_;
    int numPrograms_;
};
