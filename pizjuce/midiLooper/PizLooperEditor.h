#pragma once

#include "../_common/ClickableLabel.h"
#include "../_common/VSTSlider.h"
#include "PizLooper.h"
#include "ui/CustomLookAndFeel.h"
#include "ui/KeySelector.h"
#include "ui/PianoRoll.h"
#include "ui/PianoRollViewport.h"

class PizLooperEditor : public juce::AudioProcessorEditor,
                        public juce::ChangeListener,
                        public juce::FileDragAndDropTarget,
                        public ClickableLabelListener,
                        public juce::Timer,
                        public juce::MidiKeyboardStateListener,
                        public juce::Button::Listener
{
public:
    PizLooperEditor(PizLooper* ownerFilter);
    ~PizLooperEditor() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& filenames, int mouseX, int mouseY) override;

    void timerCallback() override;
    void buttonStateChanged(juce::Button* button) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    void clickableLabelMouseDown(ClickableLabel* label, const juce::MouseEvent& e) override;
    void clickableLabelMouseDoubleClick(ClickableLabel* label, const juce::MouseEvent& e) override;

    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* piznew40_png;
    static const int piznew40_pngSize;

private:
    CustomLookAndFeel lookAndFeel;

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
    // every time we need it
    PizLooper* getFilter() const noexcept;

    //==============================================================================
    std::unique_ptr<juce::ImageButton> aboutButton;

    // global plugin settings
    std::unique_ptr<juce::Label> hostSyncModeLabel;
    std::unique_ptr<juce::ComboBox> syncModeComboBox;

    std::unique_ptr<juce::Label> loopStepSizeLabel;
    std::unique_ptr<juce::ComboBox> stepSizeComboBox;

    std::unique_ptr<juce::Label> recordLengthLabel;
    std::unique_ptr<VSTSlider> fixedLengthSlider;

    std::unique_ptr<juce::Label> quantizeInputLabel;
    std::unique_ptr<juce::ComboBox> quantizeComboBox;

    std::unique_ptr<juce::TextButton> overdubButton;

    std::unique_ptr<juce::ComboBox> midiOutDeviceComboBox;
    std::unique_ptr<juce::Label> midiOutDeviceLabel;

    std::unique_ptr<juce::ToggleButton> singleLoopToggleButton;

    std::unique_ptr<VSTSlider> masterVelocitySlider;
    std::unique_ptr<juce::Label> masterVelocityLabel;

    std::unique_ptr<VSTSlider> masterTransposeSlider;
    std::unique_ptr<juce::Label> masterTransposeLabel;

    std::unique_ptr<juce::TextButton> midiThruButton;
    std::unique_ptr<juce::TextButton> monitorButton;

    // slot buttons
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

    // slot title
    std::unique_ptr<ClickableLabel> patternNameLabel;
    std::unique_ptr<juce::TextButton> clearButton;
    std::unique_ptr<juce::TextButton> saveButton;
    std::unique_ptr<juce::TextButton> reloadButton;

    // loop settings
    std::unique_ptr<juce::TextButton> recordButton;
    std::unique_ptr<juce::TextButton> playButton;
    std::unique_ptr<juce::ToggleButton> waitForBarButton;

    std::unique_ptr<juce::ComboBox> loopModeComboBox;

    std::unique_ptr<VSTSlider> playGroupSlider;
    std::unique_ptr<juce::Label> playGroupLabel;

    std::unique_ptr<VSTSlider> rootNoteSlider;
    std::unique_ptr<juce::Label> rootNoteLabel;

    std::unique_ptr<VSTSlider> numLoopsSlider;
    std::unique_ptr<VSTSlider> nextSlotSlider;

    std::unique_ptr<VSTSlider> muteGroupSlider;
    std::unique_ptr<juce::Label> muteGroupLabel;

    std::unique_ptr<juce::Label> loopInfoLabel;
    std::unique_ptr<juce::Label> loopInfoLabel2;

    std::unique_ptr<juce::Label> loopSettingsLabel;

    // loop manipulation
    std::unique_ptr<juce::ToggleButton> useTransposeChannelButton;
    std::unique_ptr<juce::ToggleButton> immediateTransposeButton;
    std::unique_ptr<juce::TextButton> transpose10Button;

    std::unique_ptr<VSTSlider> scaleChannelSlider;
    std::unique_ptr<juce::Label> scaleChannelLabel;

    std::unique_ptr<VSTSlider> transposeChannelSlider;
    std::unique_ptr<juce::Label> transposeChannelLabel;

    std::unique_ptr<VSTSlider> transposeSlider;
    std::unique_ptr<juce::Label> transposeLabel;
    std::unique_ptr<VSTSlider> octaveSlider;
    std::unique_ptr<juce::Label> octaveLabel;
    std::unique_ptr<VSTSlider> velocitySlider;
    std::unique_ptr<juce::Label> velocityLabel;

    std::unique_ptr<juce::ToggleButton> forceToKeyButton;
    std::unique_ptr<juce::ComboBox> forceModeComboBox;

    std::unique_ptr<juce::ToggleButton> useScaleChannelButton;

    std::unique_ptr<juce::TextButton> shiftDownButton;
    std::unique_ptr<KeySelector> keySelector;
    std::unique_ptr<juce::TextButton> shiftUpButton;

    std::unique_ptr<VSTSlider> loopStartSlider;
    std::unique_ptr<juce::Label> startOffsetLabel;

    std::unique_ptr<VSTSlider> loopEndOffsetSlider;
    std::unique_ptr<juce::Label> loopEndOffsetLabel;

    std::unique_ptr<VSTSlider> shiftPatternSlider;
    std::unique_ptr<juce::Label> shiftPatternLabel;

    std::unique_ptr<VSTSlider> stretchSlider;
    std::unique_ptr<juce::Label> speedLabel;

    std::unique_ptr<juce::Label> loopManipulationLabel;

    // Note Triggering
    std::unique_ptr<juce::ComboBox> noteTriggerComboBox;

    std::unique_ptr<juce::TextButton> noteToggleButton;

    std::unique_ptr<VSTSlider> minTriggerNoteSlider;
    std::unique_ptr<juce::Label> minTriggerNoteLabel;

    std::unique_ptr<VSTSlider> maxTriggerNoteSlider;
    std::unique_ptr<juce::Label> maxTriggerNoteLabel;

    std::unique_ptr<VSTSlider> triggerChannelSlider;
    std::unique_ptr<juce::Label> triggerChannelLabel;

    std::unique_ptr<VSTSlider> velocitySensSlider;
    std::unique_ptr<juce::Label> velocitySenSlider;

    std::unique_ptr<juce::Label> noteTriggeringLabel;

    std::unique_ptr<VSTSlider> slotIOChannelSlider;
    std::unique_ptr<juce::Label> slotIOChannelLabel;

    std::unique_ptr<juce::TextButton> transformFilterButton;

    // piano roll
    std::unique_ptr<juce::ToggleButton> snapButton;
    std::unique_ptr<juce::ComboBox> quantize2ComboBox;

    std::unique_ptr<juce::TextButton> tripletButton;
    std::unique_ptr<juce::TextButton> dottedButton;

    std::unique_ptr<juce::Label> numeratorLabel;
    std::unique_ptr<juce::Label> denominatorLabel;

    std::unique_ptr<juce::Label> zoomLabel;
    std::unique_ptr<juce::TextButton> zoomOutButton;
    std::unique_ptr<juce::TextButton> zoomInButton;

    std::unique_ptr<juce::TextButton> addBarButton;
    std::unique_ptr<juce::Label> patternLengthLabel;
    std::unique_ptr<juce::TextButton> removeBarButton;

    std::unique_ptr<juce::Viewport> keyboardViewport;
    std::unique_ptr<Timeline> timeline;
    std::unique_ptr<PianoRollViewport> pianoRollViewport;

    std::unique_ptr<juce::ResizableCornerComponent> resizer;

    std::unique_ptr<juce::ToggleButton> keepLengthToggleButton;
    std::unique_ptr<VSTSlider> recCCSlider;
    std::unique_ptr<VSTSlider> playCCSlider;

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

    juce::Image cachedImage_piznew40_png_1;

    void zoomXViewport(bool out);
    void zoomYViewport(bool out);

    void handleMouseWheelMove(const juce::MouseWheelDetails& details);
    void handleMouseWheelMove(bool out, int positionIncrement);

    void handleOverdubButtonClick() const;
    void handleThruButtonClick() const;
    void handleClearButtonClick() const;
    void handleReloadButtonClick() const;
    void handleSaveButtonClick() const;
    void handleTransformFilterButtonClick() const;
    void handleNoteToggleButtonClick() const;
    void handleSnapButtonClick();
    void handleForceToKeyButtonClick() const;
    void handleShiftUpButtonClick() const;
    void handleShiftDownButtonClick() const;
    void handleSingleLoopButtonClick() const;
    static void handleAboutButtonClick();
    void handleTripletButtonClick();
    void handleDottedButtonClick();
    void handleZoomOutButtonClick();
    void handleZoomInButtonClick();
    void handleUseScaleChannelButtonClick();
    void handleWaitForBarButtonClick() const;
    void handleUseTrChannelButtonClick();
    void handleImmediateTransposeButtonClick() const;
    void handleRemoveBarButtonClick();
    void handleAddBarButtonClick();
    void handleTranspose10ButtonClick() const;
    void handleKeepLengthButtonClick() const;
    void handleMonitorButtonClick() const;

    void handleStepSizeComboBoxChange() const;
    void handleLoopModeComboBoxChange() const;
    void handleNoteTriggerComboBoxChange() const;
    void handleSyncModeComboBoxChange() const;
    void handleQuantizeComboBoxChange() const;
    void handleQuantize2ComboBoxChange();
    void handleMidiOutDeviceComboBoxChange() const;
    void handleForceModeComboBoxChange() const;

    void handleTransposeSliderChange() const;
    void handleOctaveSliderChange() const;
    void handleVelocitySliderChange() const;
    void handleLoopStartSliderChange() const;
    void handleEndSliderChange() const;
    void handleStretchSliderChange() const;
    void handleRootSliderChange() const;
    void handleLowSliderChange() const;
    void handleHighSliderChange() const;
    void handleTrigChanSliderChange() const;
    void handleShiftSliderChange() const;
    void handleChannelSliderChange() const;
    void handleFixedLengthSliderChange() const;
    void handlePlayGroupSliderChange() const;
    void handleMuteGroupSliderChange() const;
    void handleMasterVelocitySliderChange() const;
    void handleScaleChannelSliderChange() const;
    void handleMasterTransposeSliderChange() const;
    void handleNumLoopsSliderChange() const;
    void handleNextSlotSliderChange() const;
    void handleRecCCSliderChange() const;
    void handlePlayCCSliderChange() const;
    void handleVelocitySensSliderChange() const;
    void handleTransposeChannelSliderChange() const;

    void handleDenominatorLabelChange();
    void handleLengthLabelChange();
    void handleNumeratorLabelTextChange();
    void handleNameLabelTextChange() const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PizLooperEditor)
};
