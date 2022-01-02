/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Feb 2012 7:51:26pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_PIZLOOPEREDITOR_PIZLOOPEREDITOR_D53500E8__
#define __JUCER_HEADER_PIZLOOPEREDITOR_PIZLOOPEREDITOR_D53500E8__

//[Headers]     -- You can add your own extra header files here --
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "../_common/ClickableLabel.h"
#include "../_common/VSTSlider.h"

#include "PizLooper.h"
#include "PianoRoll.h"

class KeySelector : public MidiKeyboardComponent
{
public:
    KeySelector(MidiKeyboardState &state)
		: MidiKeyboardComponent(state,MidiKeyboardComponent::horizontalKeyboard)
	{
		s=&state;
		this->setColour(MidiKeyboardComponent::textLabelColourId,Colours::transparentBlack);
	}
	~KeySelector(){}
private:
    bool mouseDownOnKey(int midiNoteNumber, const MouseEvent &e)
	{
        if (s->isNoteOn(this->getMidiChannel(),midiNoteNumber)) {
            s->noteOff(this->getMidiChannel(),midiNoteNumber,1.f);
        }
        else {
            s->noteOn(this->getMidiChannel(),midiNoteNumber,1.f);
        }
        return false;
	}
    MidiKeyboardState* s;
};



//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PizLooperEditor  : public AudioProcessorEditor,
                         public ChangeListener,
                         public FileDragAndDropTarget,
                         public ClickableLabelListener,
                         public Timer,
                         public MidiKeyboardStateListener,
                         public Button::Listener,
                         public ComboBox::Listener,
                         public Slider::Listener,
                         public Label::Listener
{
public:
    //==============================================================================
    PizLooperEditor (PizLooper* const ownerFilter);
    ~PizLooperEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source);
    bool isInterestedInFileDrag (const StringArray& files);
    void filesDropped (const StringArray& filenames, int mouseX, int mouseY);
//    void sliderDragStarted (Slider* slider); //slider mousedown
//    void sliderDragEnded (Slider* slider); //slider mouseup
	void timerCallback();
	void buttonStateChanged(Button* button);
    void mouseDrag (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
	void clickableLabelMouseDown(ClickableLabel *label, const MouseEvent &e) {}
	void clickableLabelMouseDoubleClick(ClickableLabel *label, const MouseEvent &e) {if (label==nameLabel) label->edit();}
	void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity);
	void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void labelTextChanged (Label* labelThatHasChanged);

    // Binary resources:
    static const char* piznew40_png;
    static const int piznew40_pngSize;


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    TooltipWindow tooltipWindow;
    void updateParametersFromFilter();
	void updateControls(int param, float value, bool forCurProgram);
	void updateSlotButtons();
    ComponentBoundsConstrainer resizeLimits;
	TextButton* getButtonForSlot(int slot);
	int getButtonIndex(Component* button);
	int lastActiveLoop;
    PianoRoll* pianoRoll;
	bool noSnap;
	int loopDragStart;
	MidiKeyboardState keySelectorState;
	MidiKeyboardComponent* keyboard;
	int counter;
	int demo;
    Path internalPath1;
    Path internalPath2;

    // handy wrapper method to avoid having to cast the filter to a PizLooper
    // every time we need it..
    PizLooper* getFilter() const throw() { return (PizLooper*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    Label* label;
    Timeline* timeline;
    TextButton* textButton1;
    TextButton* textButton2;
    TextButton* textButton3;
    TextButton* textButton4;
    TextButton* textButton5;
    TextButton* textButton6;
    TextButton* textButton7;
    TextButton* textButton8;
    TextButton* textButton9;
    TextButton* textButton10;
    TextButton* textButton11;
    TextButton* textButton12;
    TextButton* textButton13;
    TextButton* textButton14;
    TextButton* textButton15;
    TextButton* textButton16;
    TextButton* b_Play;
    TextButton* b_Record;
    TextButton* b_Overdub;
    TextButton* b_Thru;
    TextButton* b_Clear;
    ComboBox* stepsizeBox;
    VSTSlider* s_Transpose;
    VSTSlider* s_Octave;
    VSTSlider* s_Velocity;
    Label* label3;
    Label* label4;
    Label* label5;
    VSTSlider* s_Start;
    Label* label6;
    VSTSlider* s_End;
    Label* label7;
    VSTSlider* s_Stretch;
    Label* label8;
    ComboBox* loopmodeBox;
    ComboBox* notetriggerBox;
    ComboBox* syncmodeBox;
    VSTSlider* s_Root;
    Label* label9;
    VSTSlider* s_Low;
    Label* label10;
    VSTSlider* s_High;
    Label* label11;
    VSTSlider* s_TrigChan;
    Label* label12;
    TextButton* b_Reload;
    ComboBox* quantizeBox;
    Label* label21;
    VSTSlider* s_Shift;
    Label* label2;
    Label* label23;
    ClickableLabel* nameLabel;
    TextButton* b_Save;
    Label* label22;
    Label* label18;
    Label* loopinfoLabel;
    Label* loopinfoLabel2;
    Label* label17;
    VSTSlider* s_Channel;
    Label* label19;
    Label* label20;
    VSTSlider* s_FixedLength;
    TextButton* b_Filt;
    PianoPort* viewport;
    ResizableCornerComponent* resizer;
    TextButton* b_NoteToggle;
    VSTSlider* s_PlayGroup;
    Label* label13;
    VSTSlider* s_MuteGroup;
    Label* label14;
    ToggleButton* b_Snap;
    ComboBox* quantizeBox2;
    ToggleButton* b_ForceToKey;
    KeySelector* keySelector;
    TextButton* b_ShiftUp;
    TextButton* b_ShiftDown;
    ToggleButton* b_SingleLoop;
    VSTSlider* s_MasterVelocity;
    Label* label15;
    ImageButton* aboutButton;
    TextButton* b_Triplet;
    TextButton* b_Dotted;
    TextButton* b_ZoomOut;
    TextButton* b_ZoomIn;
    Label* numerator;
    Label* denominator;
    Label* loopinfoLabel3;
    ToggleButton* b_UseScaleChannel;
    VSTSlider* s_ScaleChannel;
    Label* label25;
    VSTSlider* s_MasterTranspose;
    Label* label26;
    ToggleButton* b_WaitForBar;
    ComboBox* midiOutDeviceBox;
    Label* label27;
    ToggleButton* b_UseTrChannel;
    ToggleButton* b_ImmediateTranspose;
    VSTSlider* s_NumLoops;
    VSTSlider* s_NextSlot;
    Label* label16;
    ComboBox* forceModeBox;
    Viewport* kbport;
    TextButton* b_RemoveBar;
    TextButton* b_AddBar;
    Label* LengthLabel;
    TextButton* textButton17;
    TextButton* textButton18;
    TextButton* textButton19;
    TextButton* textButton20;
    TextButton* textButton21;
    TextButton* textButton22;
    TextButton* textButton23;
    TextButton* textButton24;
    TextButton* textButton25;
    TextButton* textButton26;
    TextButton* textButton27;
    TextButton* textButton28;
    TextButton* textButton29;
    TextButton* textButton30;
    TextButton* textButton31;
    TextButton* textButton32;
    TextButton* textButton33;
    TextButton* textButton34;
    TextButton* textButton35;
    TextButton* textButton36;
    TextButton* textButton37;
    TextButton* textButton38;
    TextButton* textButton39;
    TextButton* textButton40;
    TextButton* textButton41;
    TextButton* textButton42;
    TextButton* textButton43;
    TextButton* textButton44;
    TextButton* textButton45;
    TextButton* textButton46;
    TextButton* textButton47;
    TextButton* textButton48;
    TextButton* textButton49;
    TextButton* textButton50;
    TextButton* textButton51;
    TextButton* textButton52;
    TextButton* textButton53;
    TextButton* textButton54;
    TextButton* textButton55;
    TextButton* textButton56;
    TextButton* textButton57;
    TextButton* textButton58;
    TextButton* textButton59;
    TextButton* textButton60;
    TextButton* textButton61;
    TextButton* textButton62;
    TextButton* textButton63;
    TextButton* textButton64;
    TextButton* textButton65;
    TextButton* textButton66;
    TextButton* textButton67;
    TextButton* textButton68;
    TextButton* textButton69;
    TextButton* textButton70;
    TextButton* textButton71;
    TextButton* textButton72;
    TextButton* textButton73;
    TextButton* textButton74;
    TextButton* textButton75;
    TextButton* textButton76;
    TextButton* textButton77;
    TextButton* textButton78;
    TextButton* textButton79;
    TextButton* textButton80;
    TextButton* textButton81;
    TextButton* textButton82;
    TextButton* textButton83;
    TextButton* textButton84;
    TextButton* textButton85;
    TextButton* textButton86;
    TextButton* textButton87;
    TextButton* textButton88;
    TextButton* textButton89;
    TextButton* textButton90;
    TextButton* textButton91;
    TextButton* textButton92;
    TextButton* textButton93;
    TextButton* textButton94;
    TextButton* textButton95;
    TextButton* textButton96;
    TextButton* textButton97;
    TextButton* textButton98;
    TextButton* textButton99;
    TextButton* textButton100;
    TextButton* textButton101;
    TextButton* textButton102;
    TextButton* textButton103;
    TextButton* textButton104;
    TextButton* textButton105;
    TextButton* textButton106;
    TextButton* textButton107;
    TextButton* textButton108;
    TextButton* textButton109;
    TextButton* textButton110;
    TextButton* textButton111;
    TextButton* textButton112;
    TextButton* textButton113;
    TextButton* textButton114;
    TextButton* textButton115;
    TextButton* textButton116;
    TextButton* textButton117;
    TextButton* textButton118;
    TextButton* textButton119;
    TextButton* textButton120;
    TextButton* textButton121;
    TextButton* textButton122;
    TextButton* textButton123;
    TextButton* textButton124;
    TextButton* textButton125;
    TextButton* textButton126;
    TextButton* textButton127;
    TextButton* textButton128;
    TextButton* b_Transpose10;
    ToggleButton* b_KeepLength;
    VSTSlider* s_RecCC;
    VSTSlider* s_PlayCC;
    VSTSlider* s_VelocitySens;
    Label* label24;
    TextButton* b_Monitor;
    VSTSlider* s_TransposeChannel;
    Label* label28;
    Image cachedImage_piznew40_png;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    PizLooperEditor (const PizLooperEditor&);
    const PizLooperEditor& operator= (const PizLooperEditor&);
};


#endif   // __JUCER_HEADER_PIZLOOPEREDITOR_PIZLOOPEREDITOR_D53500E8__
