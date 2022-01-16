// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

namespace piz
{

/// Encapsulates the state of the whole plugin, including the current processor, bank and program.
class PluginState
{
    using Identifier  = juce::Identifier;
    using String      = juce::String;
    using UndoManager = juce::UndoManager;
    using ValueTree   = juce::ValueTree;
    using XmlElement  = juce::XmlElement;

public:
    //==============================================================================================
    PluginState (Identifier type, String processor, int bank, int program)
        : undoManager_ (nullptr),
          valueTree_ (type)
    {
        valueTree_.setProperty (idOfSchemaVersion(), kSchemaVersion, undoManager_);

        setCurrentProcessor (processor);
        setCurrentBank (bank);
        setCurrentProgram (program);
    }

    //==============================================================================================
    int getCurrentBank() const
    {
        return valueTree_.getProperty (idOfCurrentBank());
    }

    void setCurrentBank (int bank)
    {
        valueTree_.setProperty (idOfCurrentBank(), bank, undoManager_);
    }

    //==============================================================================================
    int getCurrentProgram() const
    {
        return valueTree_.getProperty (idOfCurrentProgram());
    }

    void setCurrentProgram (int program)
    {
        valueTree_.setProperty (idOfCurrentProgram(), program, undoManager_);
    }

    //==============================================================================================
    String getCurrentProcessor() const
    {
        return valueTree_.getProperty (idOfCurrentProcessor()).toString();
    }

    void setCurrentProcessor (String processor)
    {
        valueTree_.setProperty (idOfCurrentProcessor(), processor, undoManager_);
    }

    //==============================================================================================
    ValueTree getStateOf (int bank, int program) const
    {
        return valueTree_
            .getChildWithName (String (bank))
            .getChildWithName (String (program))
            .createCopy();
    }

    void updateStateOf (int bank, int program, ValueTree newState)
    {
        valueTree_
            .getOrCreateChildWithName (String (bank), undoManager_)
            .getOrCreateChildWithName (String (program), undoManager_)
            .copyPropertiesAndChildrenFrom (newState, nullptr);
    }

    //==============================================================================================
    Identifier getType() const { return valueTree_.getType(); }

    void loadFrom (XmlElement const& xml)
    {
        if (xml.hasTagName (this->getType()))
        {
            auto tree               = ValueTree::fromXml (xml);
            int loadedSchemaVersion = tree.getProperty (idOfSchemaVersion());
            if (loadedSchemaVersion == kSchemaVersion)
            {
                valueTree_ = tree.createCopy();
            }
        }
    }

    std::unique_ptr<XmlElement> createXml() { return valueTree_.createXml(); }

private:
    static Identifier idOfCurrentBank() { return { "CurrentBank" }; }
    static Identifier idOfCurrentProgram() { return { "CurrentProgram" }; }
    static Identifier idOfCurrentProcessor() { return { "CurrentProcessor" }; }
    static Identifier idOfSchemaVersion() { return { "SchemaVersion" }; }

    static int const kSchemaVersion = 1;

    UndoManager* undoManager_;
    ValueTree valueTree_;
};

} // namespace piz
