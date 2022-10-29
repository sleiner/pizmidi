/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../_common/ClickableLabel.h"
#include "../_common/VSTSlider.h"
#include "PianoRoll.h"
#include "PizLooper.h"

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

class KeySelector : public juce::MidiKeyboardComponent
{
public:
    KeySelector(juce::MidiKeyboardState& state)
        : MidiKeyboardComponent(state, MidiKeyboardComponent::horizontalKeyboard)
    {
        s = &state;
        this->setColour(MidiKeyboardComponent::textLabelColourId, juce::Colours::transparentBlack);
    }

    ~KeySelector() override
    {
    }

private:
    bool mouseDownOnKey(int midiNoteNumber, const juce::MouseEvent& e) override
    {
        if (s->isNoteOn(this->getMidiChannel(), midiNoteNumber))
        {
            s->noteOff(this->getMidiChannel(), midiNoteNumber, 1.f);
        }
        else
        {
            s->noteOn(this->getMidiChannel(), midiNoteNumber, 1.f);
        }
        return false;
    }

    juce::MidiKeyboardState* s;
};

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PizLooperEditor : public juce::AudioProcessorEditor,
                        public juce::ChangeListener,
                        public juce::FileDragAndDropTarget,
                        public ClickableLabelListener,
                        public juce::Timer,
                        public juce::MidiKeyboardStateListener,
                        public juce::Button::Listener,
                        public juce::ComboBox::Listener,
                        public juce::Slider::Listener,
                        public juce::Label::Listener
{
public:
    //==============================================================================
    PizLooperEditor(PizLooper* const ownerFilter);
    ~PizLooperEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& filenames, int mouseX, int mouseY) override;
    //    void sliderDragStarted (Slider* slider); //slider mousedown
    //    void sliderDragEnded (Slider* slider); //slider mouseup
    void timerCallback() override;
    void buttonStateChanged(juce::Button* button) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    void clickableLabelMouseDown(ClickableLabel* label, const juce::MouseEvent& e) override
    {
    }

    void clickableLabelMouseDoubleClick(ClickableLabel* label, const juce::MouseEvent& e) override
    {
        if (label == nameLabel.get())
            label->edit();
    }

    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    //[/UserMethods]

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override;
    void labelTextChanged(juce::Label* labelThatHasChanged) override;

    // Binary resources:
    static const char* piznew40_png;
    static const int piznew40_pngSize;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    juce::TooltipWindow tooltipWindow;
    void updateParametersFromFilter();
    void updateControls(int param, float value, bool forCurProgram);
    void updateSlotButtons();
    juce::ComponentBoundsConstrainer resizeLimits;
    juce::TextButton* getButtonForSlot(int slot);
    int getButtonIndex(Component* button);
    int lastActiveLoop;
    PianoRoll* pianoRoll;
    bool noSnap;
    int loopDragStart;
    juce::MidiKeyboardState keySelectorState;
    juce::MidiKeyboardComponent* keyboard;
    juce::Path internalPath1;
    juce::Path internalPath2;

    // handy wrapper method to avoid having to cast the filter to a PizLooper
    // every time we need it..
    PizLooper* getFilter() const throw()
    {
        return (PizLooper*) getAudioProcessor();
    }

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<Timeline> timeline;
    std::unique_ptr<juce::TextButton> textButton1;
    std::unique_ptr<juce::TextButton> textButton2;
    std::unique_ptr<juce::TextButton> textButton3;
    std::unique_ptr<juce::TextButton> textButton4;
    std::unique_ptr<juce::TextButton> textButton5;
    std::unique_ptr<juce::TextButton> textButton6;
    std::unique_ptr<juce::TextButton> textButton7;
    std::unique_ptr<juce::TextButton> textButton8;
    std::unique_ptr<juce::TextButton> textButton9;
    std::unique_ptr<juce::TextButton> textButton10;
    std::unique_ptr<juce::TextButton> textButton11;
    std::unique_ptr<juce::TextButton> textButton12;
    std::unique_ptr<juce::TextButton> textButton13;
    std::unique_ptr<juce::TextButton> textButton14;
    std::unique_ptr<juce::TextButton> textButton15;
    std::unique_ptr<juce::TextButton> textButton16;
    std::unique_ptr<juce::TextButton> b_Play;
    std::unique_ptr<juce::TextButton> b_Record;
    std::unique_ptr<juce::TextButton> b_Overdub;
    std::unique_ptr<juce::TextButton> b_Thru;
    std::unique_ptr<juce::TextButton> b_Clear;
    std::unique_ptr<juce::ComboBox> stepsizeBox;
    std::unique_ptr<VSTSlider> s_Transpose;
    std::unique_ptr<VSTSlider> s_Octave;
    std::unique_ptr<VSTSlider> s_Velocity;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<VSTSlider> s_Start;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<VSTSlider> s_End;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<VSTSlider> s_Stretch;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<juce::ComboBox> loopmodeBox;
    std::unique_ptr<juce::ComboBox> notetriggerBox;
    std::unique_ptr<juce::ComboBox> syncmodeBox;
    std::unique_ptr<VSTSlider> s_Root;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<VSTSlider> s_Low;
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<VSTSlider> s_High;
    std::unique_ptr<juce::Label> label11;
    std::unique_ptr<VSTSlider> s_TrigChan;
    std::unique_ptr<juce::Label> label12;
    std::unique_ptr<juce::TextButton> b_Reload;
    std::unique_ptr<juce::ComboBox> quantizeBox;
    std::unique_ptr<juce::Label> label21;
    std::unique_ptr<VSTSlider> s_Shift;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label23;
    std::unique_ptr<ClickableLabel> nameLabel;
    std::unique_ptr<juce::TextButton> b_Save;
    std::unique_ptr<juce::Label> label22;
    std::unique_ptr<juce::Label> label18;
    std::unique_ptr<juce::Label> loopinfoLabel;
    std::unique_ptr<juce::Label> loopinfoLabel2;
    std::unique_ptr<juce::Label> label17;
    std::unique_ptr<VSTSlider> s_Channel;
    std::unique_ptr<juce::Label> label19;
    std::unique_ptr<juce::Label> label20;
    std::unique_ptr<VSTSlider> s_FixedLength;
    std::unique_ptr<juce::TextButton> b_Filt;
    std::unique_ptr<PianoPort> viewport;
    std::unique_ptr<juce::ResizableCornerComponent> resizer;
    std::unique_ptr<juce::TextButton> b_NoteToggle;
    std::unique_ptr<VSTSlider> s_PlayGroup;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<VSTSlider> s_MuteGroup;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<juce::ToggleButton> b_Snap;
    std::unique_ptr<juce::ComboBox> quantizeBox2;
    std::unique_ptr<juce::ToggleButton> b_ForceToKey;
    std::unique_ptr<KeySelector> keySelector;
    std::unique_ptr<juce::TextButton> b_ShiftUp;
    std::unique_ptr<juce::TextButton> b_ShiftDown;
    std::unique_ptr<juce::ToggleButton> b_SingleLoop;
    std::unique_ptr<VSTSlider> s_MasterVelocity;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<juce::ImageButton> aboutButton;
    std::unique_ptr<juce::TextButton> b_Triplet;
    std::unique_ptr<juce::TextButton> b_Dotted;
    std::unique_ptr<juce::TextButton> b_ZoomOut;
    std::unique_ptr<juce::TextButton> b_ZoomIn;
    std::unique_ptr<juce::Label> numerator;
    std::unique_ptr<juce::Label> denominator;
    std::unique_ptr<juce::ToggleButton> b_UseScaleChannel;
    std::unique_ptr<VSTSlider> s_ScaleChannel;
    std::unique_ptr<juce::Label> label25;
    std::unique_ptr<VSTSlider> s_MasterTranspose;
    std::unique_ptr<juce::Label> label26;
    std::unique_ptr<juce::ToggleButton> b_WaitForBar;
    std::unique_ptr<juce::ComboBox> midiOutDeviceBox;
    std::unique_ptr<juce::Label> label27;
    std::unique_ptr<juce::ToggleButton> b_UseTrChannel;
    std::unique_ptr<juce::ToggleButton> b_ImmediateTranspose;
    std::unique_ptr<VSTSlider> s_NumLoops;
    std::unique_ptr<VSTSlider> s_NextSlot;
    std::unique_ptr<juce::Label> label16;
    std::unique_ptr<juce::ComboBox> forceModeBox;
    std::unique_ptr<juce::Viewport> kbport;
    std::unique_ptr<juce::TextButton> b_RemoveBar;
    std::unique_ptr<juce::TextButton> b_AddBar;
    std::unique_ptr<juce::Label> LengthLabel;
    std::unique_ptr<juce::TextButton> textButton17;
    std::unique_ptr<juce::TextButton> textButton18;
    std::unique_ptr<juce::TextButton> textButton19;
    std::unique_ptr<juce::TextButton> textButton20;
    std::unique_ptr<juce::TextButton> textButton21;
    std::unique_ptr<juce::TextButton> textButton22;
    std::unique_ptr<juce::TextButton> textButton23;
    std::unique_ptr<juce::TextButton> textButton24;
    std::unique_ptr<juce::TextButton> textButton25;
    std::unique_ptr<juce::TextButton> textButton26;
    std::unique_ptr<juce::TextButton> textButton27;
    std::unique_ptr<juce::TextButton> textButton28;
    std::unique_ptr<juce::TextButton> textButton29;
    std::unique_ptr<juce::TextButton> textButton30;
    std::unique_ptr<juce::TextButton> textButton31;
    std::unique_ptr<juce::TextButton> textButton32;
    std::unique_ptr<juce::TextButton> textButton33;
    std::unique_ptr<juce::TextButton> textButton34;
    std::unique_ptr<juce::TextButton> textButton35;
    std::unique_ptr<juce::TextButton> textButton36;
    std::unique_ptr<juce::TextButton> textButton37;
    std::unique_ptr<juce::TextButton> textButton38;
    std::unique_ptr<juce::TextButton> textButton39;
    std::unique_ptr<juce::TextButton> textButton40;
    std::unique_ptr<juce::TextButton> textButton41;
    std::unique_ptr<juce::TextButton> textButton42;
    std::unique_ptr<juce::TextButton> textButton43;
    std::unique_ptr<juce::TextButton> textButton44;
    std::unique_ptr<juce::TextButton> textButton45;
    std::unique_ptr<juce::TextButton> textButton46;
    std::unique_ptr<juce::TextButton> textButton47;
    std::unique_ptr<juce::TextButton> textButton48;
    std::unique_ptr<juce::TextButton> textButton49;
    std::unique_ptr<juce::TextButton> textButton50;
    std::unique_ptr<juce::TextButton> textButton51;
    std::unique_ptr<juce::TextButton> textButton52;
    std::unique_ptr<juce::TextButton> textButton53;
    std::unique_ptr<juce::TextButton> textButton54;
    std::unique_ptr<juce::TextButton> textButton55;
    std::unique_ptr<juce::TextButton> textButton56;
    std::unique_ptr<juce::TextButton> textButton57;
    std::unique_ptr<juce::TextButton> textButton58;
    std::unique_ptr<juce::TextButton> textButton59;
    std::unique_ptr<juce::TextButton> textButton60;
    std::unique_ptr<juce::TextButton> textButton61;
    std::unique_ptr<juce::TextButton> textButton62;
    std::unique_ptr<juce::TextButton> textButton63;
    std::unique_ptr<juce::TextButton> textButton64;
    std::unique_ptr<juce::TextButton> textButton65;
    std::unique_ptr<juce::TextButton> textButton66;
    std::unique_ptr<juce::TextButton> textButton67;
    std::unique_ptr<juce::TextButton> textButton68;
    std::unique_ptr<juce::TextButton> textButton69;
    std::unique_ptr<juce::TextButton> textButton70;
    std::unique_ptr<juce::TextButton> textButton71;
    std::unique_ptr<juce::TextButton> textButton72;
    std::unique_ptr<juce::TextButton> textButton73;
    std::unique_ptr<juce::TextButton> textButton74;
    std::unique_ptr<juce::TextButton> textButton75;
    std::unique_ptr<juce::TextButton> textButton76;
    std::unique_ptr<juce::TextButton> textButton77;
    std::unique_ptr<juce::TextButton> textButton78;
    std::unique_ptr<juce::TextButton> textButton79;
    std::unique_ptr<juce::TextButton> textButton80;
    std::unique_ptr<juce::TextButton> textButton81;
    std::unique_ptr<juce::TextButton> textButton82;
    std::unique_ptr<juce::TextButton> textButton83;
    std::unique_ptr<juce::TextButton> textButton84;
    std::unique_ptr<juce::TextButton> textButton85;
    std::unique_ptr<juce::TextButton> textButton86;
    std::unique_ptr<juce::TextButton> textButton87;
    std::unique_ptr<juce::TextButton> textButton88;
    std::unique_ptr<juce::TextButton> textButton89;
    std::unique_ptr<juce::TextButton> textButton90;
    std::unique_ptr<juce::TextButton> textButton91;
    std::unique_ptr<juce::TextButton> textButton92;
    std::unique_ptr<juce::TextButton> textButton93;
    std::unique_ptr<juce::TextButton> textButton94;
    std::unique_ptr<juce::TextButton> textButton95;
    std::unique_ptr<juce::TextButton> textButton96;
    std::unique_ptr<juce::TextButton> textButton97;
    std::unique_ptr<juce::TextButton> textButton98;
    std::unique_ptr<juce::TextButton> textButton99;
    std::unique_ptr<juce::TextButton> textButton100;
    std::unique_ptr<juce::TextButton> textButton101;
    std::unique_ptr<juce::TextButton> textButton102;
    std::unique_ptr<juce::TextButton> textButton103;
    std::unique_ptr<juce::TextButton> textButton104;
    std::unique_ptr<juce::TextButton> textButton105;
    std::unique_ptr<juce::TextButton> textButton106;
    std::unique_ptr<juce::TextButton> textButton107;
    std::unique_ptr<juce::TextButton> textButton108;
    std::unique_ptr<juce::TextButton> textButton109;
    std::unique_ptr<juce::TextButton> textButton110;
    std::unique_ptr<juce::TextButton> textButton111;
    std::unique_ptr<juce::TextButton> textButton112;
    std::unique_ptr<juce::TextButton> textButton113;
    std::unique_ptr<juce::TextButton> textButton114;
    std::unique_ptr<juce::TextButton> textButton115;
    std::unique_ptr<juce::TextButton> textButton116;
    std::unique_ptr<juce::TextButton> textButton117;
    std::unique_ptr<juce::TextButton> textButton118;
    std::unique_ptr<juce::TextButton> textButton119;
    std::unique_ptr<juce::TextButton> textButton120;
    std::unique_ptr<juce::TextButton> textButton121;
    std::unique_ptr<juce::TextButton> textButton122;
    std::unique_ptr<juce::TextButton> textButton123;
    std::unique_ptr<juce::TextButton> textButton124;
    std::unique_ptr<juce::TextButton> textButton125;
    std::unique_ptr<juce::TextButton> textButton126;
    std::unique_ptr<juce::TextButton> textButton127;
    std::unique_ptr<juce::TextButton> textButton128;
    std::unique_ptr<juce::TextButton> b_Transpose10;
    std::unique_ptr<juce::ToggleButton> b_KeepLength;
    std::unique_ptr<VSTSlider> s_RecCC;
    std::unique_ptr<VSTSlider> s_PlayCC;
    std::unique_ptr<VSTSlider> s_VelocitySens;
    std::unique_ptr<juce::Label> label24;
    std::unique_ptr<juce::TextButton> b_Monitor;
    std::unique_ptr<VSTSlider> s_TransposeChannel;
    std::unique_ptr<juce::Label> label28;
    juce::Image cachedImage_piznew40_png_1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PizLooperEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
