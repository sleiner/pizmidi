// SPDX-FileCopyrightText:  2021 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#include "BankStorage.h"

BankStorage::BankStorage(juce::Identifier name, int numBanks, int numPrograms)
    : values_(name), numBanks_(numBanks), numPrograms_(numPrograms)
{
    for (int b = 0; b < getNumBanks(); b++)
    {
        juce::ValueTree bank("BankValues");
        bank.setProperty("bankIndex", b, 0);

        for (int p = 0; p < getNumPrograms(); p++)
        {
            juce::ValueTree pv("ProgValues");
            bank.addChild(pv, p, 0);
        }
        values_.addChild(bank, b, 0);
    }
    juce::ValueTree settings("GlobalSettings");
    values_.addChild(settings, getNumBanks(), 0);
}

void BankStorage::set(int bank, int program, juce::String name, juce::var value)
{
    values_.getChild(bank).getChild(program).setProperty(name, value, 0);
}

juce::var BankStorage::get(int bank, int program, juce::String name)
{
    return values_.getChild(bank).getChild(program).getProperty(name);
}

void BankStorage::setGlobal(juce::String name, juce::var value)
{
    values_.getChildWithName("GlobalSettings").setProperty(name, value, nullptr);
}

juce::var BankStorage::getGlobal(juce::String name)
{
    return values_.getChildWithName("GlobalSettings").getProperty(name);
}

void BankStorage::loadFrom(juce::ValueTree const& vt)
{
    if (vt.isValid())
    {
        values_.removeAllChildren(0);
        for (int i = 0; i < vt.getNumChildren(); i++)
        {
            values_.addChild(vt.getChild(i).createCopy(), i, 0);
        }
    }
}

bool BankStorage::loadNoteMatrixFrom(juce::ValueTree const& vt, int program)
{
    if (vt.getChildWithName("NoteMatrix_T0").isValid())
    {
        values_.removeChild(values_.getChild(program), 0);
        values_.addChild(vt, program, 0);
        values_.getChild(program).setProperty("progIndex", program, 0);

        return true;
    }

    return false;
}

void BankStorage::dumpTo(juce::MemoryBlock& destination)
{
    juce::MemoryOutputStream stream(destination, false);
    values_.writeToStream(stream);
}

void BankStorage::dumpProgramTo(int bank, int program, juce::MemoryBlock& destination)
{
    juce::MemoryOutputStream stream(destination, false);
    values_.getChild(bank).getChild(program).writeToStream(stream);
}
