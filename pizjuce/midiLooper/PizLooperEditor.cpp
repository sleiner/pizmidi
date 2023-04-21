#include "PizLooperEditor.h"

#include <memory>

using juce::jmax;
using juce::jmin;
using juce::roundToInt;

PizLooperEditor::PizLooperEditor(PizLooper* const ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    setLookAndFeel(&lookAndFeel);

    aboutButton = std::make_unique<juce::ImageButton>("new button");
    addAndMakeVisible(aboutButton.get());
    aboutButton->setTooltip("Insert Piz Here-> midiLooper v1.3  https://github.com/sleiner/pizmidi");
    aboutButton->setButtonText(juce::String());
    aboutButton->setImages(false, true, true, juce::Image(), 1.000f, juce::Colour(0x00000000), juce::Image(), 1.000f, juce::Colour(0x00000000), juce::Image(), 1.000f, juce::Colour(0x00000000));
    aboutButton->setBounds(9, 1, 136, 47);
    aboutButton->onClick = []
    {
        handleAboutButtonClick();
    };
    aboutButton->setTooltip(L"Insert Piz Here-> midiLooper v" + juce::String(JucePlugin_VersionString) + " https://github.com/sleiner/pizmidi");

    hostSyncModeLabel = std::make_unique<juce::Label>("Sync", "Host Sync Mode");
    addAndMakeVisible(hostSyncModeLabel.get());
    hostSyncModeLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    hostSyncModeLabel->setJustificationType(juce::Justification::centred);
    hostSyncModeLabel->setEditable(false, false, false);
    hostSyncModeLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    hostSyncModeLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    hostSyncModeLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    hostSyncModeLabel->setBounds(162, 0, 95, 16);

    syncModeComboBox = std::make_unique<juce::ComboBox>("Sync");
    addAndMakeVisible(syncModeComboBox.get());
    syncModeComboBox->setTooltip("\"PPQ\" modes always follow host timeline, which may not work in all hosts. \"Sample\" mode ignores the host\'s timeline, but the host\'s tempo is still followed.");
    syncModeComboBox->setEditableText(false);
    syncModeComboBox->setJustificationType(juce::Justification::centredLeft);
    syncModeComboBox->setTextWhenNothingSelected("PPQ (Recstart)");
    syncModeComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    syncModeComboBox->addItem("PPQ (Host 0)", 1);
    syncModeComboBox->addItem("PPQ (Recstart)", 2);
    syncModeComboBox->addItem("Sample", 3);
    syncModeComboBox->onChange = [this]
    {
        handleSyncModeComboBoxChange();
    };

    syncModeComboBox->setBounds(159, 15, 99, 16);

    loopStepSizeLabel = std::make_unique<juce::Label>("LoopStepSize", "Loop Step Size");
    addAndMakeVisible(loopStepSizeLabel.get());
    loopStepSizeLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    loopStepSizeLabel->setJustificationType(juce::Justification::centred);
    loopStepSizeLabel->setEditable(false, false, false);
    loopStepSizeLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    loopStepSizeLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopStepSizeLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopStepSizeLabel->setBounds(261, 0, 84, 16);

    stepSizeComboBox = std::make_unique<juce::ComboBox>("Loop Step Size");
    addAndMakeVisible(stepSizeComboBox.get());
    stepSizeComboBox->setTooltip("Recording length will be quantized to this step size.");
    stepSizeComboBox->setEditableText(false);
    stepSizeComboBox->setJustificationType(juce::Justification::centredLeft);
    stepSizeComboBox->setTextWhenNothingSelected("16th Note");
    stepSizeComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    stepSizeComboBox->addItem("1 Bar", 1);
    stepSizeComboBox->addItem("3 Beats", 2);
    stepSizeComboBox->addItem("2 Beats", 3);
    stepSizeComboBox->addItem("1 Beat", 4);
    stepSizeComboBox->addItem("16th Note", 5);
    stepSizeComboBox->addItem("1 Tick", 6);
    stepSizeComboBox->setBounds(264, 15, 77, 16);
    stepSizeComboBox->onChange = [this]
    {
        handleStepSizeComboBoxChange();
    };

    recordLengthLabel = std::make_unique<juce::Label>("RecordLengthLabel", "Record Length");
    addAndMakeVisible(recordLengthLabel.get());
    recordLengthLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    recordLengthLabel->setJustificationType(juce::Justification::centred);
    recordLengthLabel->setEditable(false, false, false);
    recordLengthLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    recordLengthLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    recordLengthLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    recordLengthLabel->setBounds(347, 0, 83, 16);

    fixedLengthSlider = std::make_unique<VSTSlider>("Recording Length");
    addAndMakeVisible(fixedLengthSlider.get());
    fixedLengthSlider->setTooltip("If set to \"Manual\", recording will go on as long as the record button is on. Otherwise, length will be limited to this number of steps (based on \"Loop Step Size\" setting).");
    fixedLengthSlider->setRange(0, 32, 1);
    fixedLengthSlider->setSliderStyle(juce::Slider::LinearBar);
    fixedLengthSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    fixedLengthSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e707070));
    fixedLengthSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    fixedLengthSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    fixedLengthSlider->onValueChange = [this]
    {
        handleFixedLengthSliderChange();
    };
    fixedLengthSlider->setBounds(347, 15, 86, 16);

    quantizeComboBox = std::make_unique<juce::ComboBox>("Input Quantize Step");
    addAndMakeVisible(quantizeComboBox.get());
    quantizeComboBox->setTooltip("Recorded events will be quantized to this step size");
    quantizeComboBox->setEditableText(false);
    quantizeComboBox->setJustificationType(juce::Justification::centredLeft);
    quantizeComboBox->setTextWhenNothingSelected("32nd");
    quantizeComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    quantizeComboBox->addItem("Off", 1);
    quantizeComboBox->addItem("8th", 2);
    quantizeComboBox->addItem("16th", 3);
    quantizeComboBox->addItem("32nd", 4);
    quantizeComboBox->addItem("64th", 5);
    quantizeComboBox->onChange = [this]
    {
        handleQuantizeComboBoxChange();
    };

    quantizeComboBox->setBounds(439, 15, 77, 16);

    quantizeInputLabel = std::make_unique<juce::Label>("QuantizeLabel", "Quantize Input");
    addAndMakeVisible(quantizeInputLabel.get());
    quantizeInputLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    quantizeInputLabel->setJustificationType(juce::Justification::centred);
    quantizeInputLabel->setEditable(false, false, false);
    quantizeInputLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    quantizeInputLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    quantizeInputLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    quantizeInputLabel->setBounds(433, 0, 87, 16);

    overdubButton = std::make_unique<juce::TextButton>("Overdub");
    addAndMakeVisible(overdubButton.get());
    overdubButton->setTooltip("Toggle overdub recording");
    overdubButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    overdubButton->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    overdubButton->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    overdubButton->setBounds(523, 12, 80, 20);
    overdubButton->onClick = [this]
    {
        handleOverdubButtonClick();
    };

    midiOutDeviceComboBox = std::make_unique<juce::ComboBox>("midiOutDevice");
    addAndMakeVisible(midiOutDeviceComboBox.get());
    midiOutDeviceComboBox->setTooltip("Send output to selected MIDI port in addition to VST host output");
    midiOutDeviceComboBox->setEditableText(false);
    midiOutDeviceComboBox->setJustificationType(juce::Justification::centredLeft);
    midiOutDeviceComboBox->setTextWhenNothingSelected("--");
    midiOutDeviceComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    midiOutDeviceComboBox->onChange = [this]
    {
        handleMidiOutDeviceComboBoxChange();
    };

    midiOutDeviceComboBox->setBounds(633, 15, 158, 16);

    midiOutDeviceLabel = std::make_unique<juce::Label>("QuantizeLabel", "MIDI Output Device");
    addAndMakeVisible(midiOutDeviceLabel.get());
    midiOutDeviceLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    midiOutDeviceLabel->setJustificationType(juce::Justification::centred);
    midiOutDeviceLabel->setEditable(false, false, false);
    midiOutDeviceLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    midiOutDeviceLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    midiOutDeviceLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    midiOutDeviceLabel->setBounds(627, 0, 117, 16);

    singleLoopToggleButton = std::make_unique<juce::ToggleButton>("Single Loop");
    addAndMakeVisible(singleLoopToggleButton.get());
    singleLoopToggleButton->setTooltip("When checked, switching from a playing slot to another slot will automatically play the new slot and stop the previous one");
    singleLoopToggleButton->setButtonText("Play active slot only");
    singleLoopToggleButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    singleLoopToggleButton->setBounds(155, 36, 122, 16);
    singleLoopToggleButton->onClick = [this]
    {
        handleSingleLoopButtonClick();
    };

    masterVelocitySlider = std::make_unique<VSTSlider>("VMasterVelocity");
    addAndMakeVisible(masterVelocitySlider.get());
    masterVelocitySlider->setTooltip("Global velocity adjustment applied to all played notes");
    masterVelocitySlider->setRange(0, 200, 1);
    masterVelocitySlider->setSliderStyle(juce::Slider::LinearBar);
    masterVelocitySlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    masterVelocitySlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e707070));
    masterVelocitySlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    masterVelocitySlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    masterVelocitySlider->onValueChange = [this]
    {
        handleMasterVelocitySliderChange();
    };

    masterVelocitySlider->setBounds(371, 36, 72, 16);

    masterVelocityLabel = std::make_unique<juce::Label>("new label", "Master Velocity:");
    addAndMakeVisible(masterVelocityLabel.get());
    masterVelocityLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    masterVelocityLabel->setJustificationType(juce::Justification::centredRight);
    masterVelocityLabel->setEditable(false, false, false);
    masterVelocityLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    masterVelocityLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    masterVelocityLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    masterVelocityLabel->setBounds(283, 36, 88, 16);

    masterTransposeSlider = std::make_unique<VSTSlider>("MasterTranspose");
    addAndMakeVisible(masterTransposeSlider.get());
    masterTransposeSlider->setTooltip("Global transposition applied to all played notes (after Force to Scale)");
    masterTransposeSlider->setRange(-12, 12, 1);
    masterTransposeSlider->setSliderStyle(juce::Slider::LinearBar);
    masterTransposeSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    masterTransposeSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e707070));
    masterTransposeSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    masterTransposeSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    masterTransposeSlider->onValueChange = [this]
    {
        handleMasterTransposeSliderChange();
    };

    masterTransposeSlider->setBounds(554, 36, 72, 16);

    masterTransposeLabel = std::make_unique<juce::Label>("new label", "Master Transpose:");
    addAndMakeVisible(masterTransposeLabel.get());
    masterTransposeLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    masterTransposeLabel->setJustificationType(juce::Justification::centredRight);
    masterTransposeLabel->setEditable(false, false, false);
    masterTransposeLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    masterTransposeLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    masterTransposeLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    masterTransposeLabel->setBounds(452, 36, 102, 16);

    midiThruButton = std::make_unique<juce::TextButton>("MIDI Thru");
    addAndMakeVisible(midiThruButton.get());
    midiThruButton->setTooltip("Toggle MIDI Thru (Notes selected for Note Triggering and Scale Channel are always blocked)");
    midiThruButton->setButtonText("Thru");
    midiThruButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    midiThruButton->setBounds(634, 33, 40, 20);
    midiThruButton->onClick = [this]
    {
        handleThruButtonClick();
    };

    monitorButton = std::make_unique<juce::TextButton>("MIDI_Monitor");
    addAndMakeVisible(monitorButton.get());
    monitorButton->setTooltip("Monitor input MIDI through active slot\'s settings (Transpose, Scale, I/O Channel)");
    monitorButton->setButtonText("Monitor");
    monitorButton->onClick = [this]
    {
        handleMonitorButtonClick();
    };
    monitorButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    monitorButton->setBounds(678, 33, 55, 20);

    textButton1 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton1.get());
    textButton1->setButtonText("1");
    textButton1->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton1->setRadioGroupId(1);

    textButton1->setBounds(0, 61, 25, 24);

    textButton2 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton2.get());
    textButton2->setButtonText("2");
    textButton2->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton2->setRadioGroupId(1);
    textButton2->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton2->setBounds(25, 61, 23, 24);

    textButton3 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton3.get());
    textButton3->setButtonText("3");
    textButton3->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton3->setRadioGroupId(1);

    textButton3->setBounds(48, 61, 23, 24);

    textButton4 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton4.get());
    textButton4->setButtonText("4");
    textButton4->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton4->setRadioGroupId(1);

    textButton4->setBounds(71, 61, 23, 24);

    textButton5 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton5.get());
    textButton5->setButtonText("5");
    textButton5->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton5->setRadioGroupId(1);

    textButton5->setBounds(94, 61, 23, 24);

    textButton6 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton6.get());
    textButton6->setButtonText("6");
    textButton6->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton6->setRadioGroupId(1);

    textButton6->setBounds(117, 61, 23, 24);

    textButton7 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton7.get());
    textButton7->setButtonText("7");
    textButton7->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton7->setRadioGroupId(1);

    textButton7->setBounds(140, 61, 23, 24);

    textButton8 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton8.get());
    textButton8->setButtonText("8");
    textButton8->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton8->setRadioGroupId(1);

    textButton8->setBounds(163, 61, 23, 24);

    textButton9 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton9.get());
    textButton9->setButtonText("9");
    textButton9->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton9->setRadioGroupId(1);

    textButton9->setBounds(186, 61, 23, 24);

    textButton10 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton10.get());
    textButton10->setButtonText("10");
    textButton10->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton10->setRadioGroupId(1);

    textButton10->setBounds(209, 61, 25, 24);

    textButton11 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton11.get());
    textButton11->setButtonText("11");
    textButton11->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton11->setRadioGroupId(1);

    textButton11->setBounds(234, 61, 25, 24);

    textButton12 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton12.get());
    textButton12->setButtonText("12");
    textButton12->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton12->setRadioGroupId(1);

    textButton12->setBounds(259, 61, 25, 24);

    textButton13 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton13.get());
    textButton13->setButtonText("13");
    textButton13->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton13->setRadioGroupId(1);

    textButton13->setBounds(284, 61, 25, 24);

    textButton14 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton14.get());
    textButton14->setButtonText("14");
    textButton14->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton14->setRadioGroupId(1);

    textButton14->setBounds(309, 61, 25, 24);

    textButton15 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton15.get());
    textButton15->setButtonText("15");
    textButton15->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton15->setRadioGroupId(1);

    textButton15->setBounds(334, 61, 25, 24);

    textButton16 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton16.get());
    textButton16->setButtonText("16");
    textButton16->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton16->setRadioGroupId(1);

    textButton16->setBounds(359, 61, 28, 24);

    patternNameLabel = std::make_unique<ClickableLabel>("Name", "Bassline (4 bars)");
    addAndMakeVisible(patternNameLabel.get());
    patternNameLabel->setTooltip("Current pattern name (double-click to edit)");
    patternNameLabel->setFont(juce::Font(26.30f, juce::Font::plain).withTypefaceStyle("Bold"));
    patternNameLabel->setJustificationType(juce::Justification::centredLeft);
    patternNameLabel->setEditable(false, true, false);
    patternNameLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    patternNameLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    patternNameLabel->onTextChange = [this]
    {
        handleNameLabelTextChange();
    };
    patternNameLabel->setBounds(4, 87, 256, 27);

    clearButton = std::make_unique<juce::TextButton>("Clear");
    addAndMakeVisible(clearButton.get());
    clearButton->setTooltip("Erase MIDI data from the current slot");
    clearButton->setConnectedEdges(juce::Button::ConnectedOnRight);
    clearButton->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    clearButton->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    clearButton->setColour(juce::TextButton::textColourOnId, juce::Colours::azure);
    clearButton->setBounds(264, 90, 39, 22);
    clearButton->onClick = [this]
    {
        handleClearButtonClick();
    };

    saveButton = std::make_unique<juce::TextButton>("Save");
    addAndMakeVisible(saveButton.get());
    saveButton->setTooltip("Save a MIDI file of the current pattern (Ctrl-click: save to the \"midiloops\" folder with the current name)");
    saveButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    saveButton->onClick = [this]
    {
        handleSaveButtonClick();
    };
    saveButton->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    saveButton->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    saveButton->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    saveButton->setBounds(304, 90, 39, 22);

    reloadButton = std::make_unique<juce::TextButton>("Load");
    addAndMakeVisible(reloadButton.get());
    reloadButton->setTooltip("Load MIDI file (Ctrl-click: load MIDI file with the current pattern name from the \"midiloops\" folder)");
    reloadButton->setConnectedEdges(juce::Button::ConnectedOnLeft);
    reloadButton->onClick = [this]
    {
        handleReloadButtonClick();
    };
    reloadButton->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    reloadButton->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    reloadButton->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    reloadButton->setBounds(344, 90, 39, 22);

    playButton = std::make_unique<juce::TextButton>("Play");
    addAndMakeVisible(playButton.get());
    playButton->setTooltip("Toggle playback of current slot");
    playButton->setButtonText("PLAY");
    playButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnTop);
    playButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xd213540e));
    playButton->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff00c400));
    playButton->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    playButton->setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    playButton->setBounds(78, 117, 72, 50);

    recordButton = std::make_unique<juce::TextButton>("Record");
    addAndMakeVisible(recordButton.get());
    recordButton->setTooltip("Toggle recording to current slot");
    recordButton->setButtonText("RECORD ");
    recordButton->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop);
    recordButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xd2a90000));
    recordButton->setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    recordButton->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    recordButton->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    recordButton->setBounds(6, 117, 72, 50);

    waitForBarButton = std::make_unique<juce::ToggleButton>("WaitForBar");
    addAndMakeVisible(waitForBarButton.get());
    waitForBarButton->setTooltip("When checked, play/stop of this slot will happen at the start of the bar after");
    waitForBarButton->setButtonText("Wait for Next Bar");
    waitForBarButton->onClick = [this]
    {
        handleWaitForBarButtonClick();
    };
    waitForBarButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    waitForBarButton->setBounds(175, 123, 107, 16);

    loopModeComboBox = std::make_unique<juce::ComboBox>("Loop Mode");
    addAndMakeVisible(loopModeComboBox.get());
    loopModeComboBox->setTooltip("Playback Mode: \"Sync Loop\" follows the host timeline. \"Loop after rec\" is the same but also plays automatically as soon as recording ends. \"Unsync\" modes play the pattern from the beginning as soon as playback is started.");
    loopModeComboBox->setEditableText(false);
    loopModeComboBox->setJustificationType(juce::Justification::centredLeft);
    loopModeComboBox->setTextWhenNothingSelected("Unsync 1-shot");
    loopModeComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    loopModeComboBox->addItem("Loop after rec", 1);
    loopModeComboBox->addItem("Sync loop", 2);
    loopModeComboBox->addItem("Unsync 1-shot", 3);
    loopModeComboBox->addItem("Unsync loop", 4);
    loopModeComboBox->onChange = [this]
    {
        handleLoopModeComboBoxChange();
    };
    loopModeComboBox->setBounds(175, 144, 110, 16);

    playGroupSlider = std::make_unique<VSTSlider>("TriggerChannel");
    addAndMakeVisible(playGroupSlider.get());
    playGroupSlider->setTooltip("Slots with the same Play Group number will all start/stop at the same time");
    playGroupSlider->setRange(0, 16, 1);
    playGroupSlider->setSliderStyle(juce::Slider::LinearBar);
    playGroupSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    playGroupSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    playGroupSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    playGroupSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    playGroupSlider->onValueChange = [this]
    {
        handlePlayGroupSliderChange();
    };

    playGroupSlider->setBounds(312, 136, 64, 20);

    playGroupLabel = std::make_unique<juce::Label>("Trigger Channel", "Play Group");
    addAndMakeVisible(playGroupLabel.get());
    playGroupLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    playGroupLabel->setJustificationType(juce::Justification::centred);
    playGroupLabel->setEditable(false, false, false);
    playGroupLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    playGroupLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    playGroupLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    playGroupLabel->setBounds(313, 119, 64, 16);

    rootNoteSlider = std::make_unique<VSTSlider>("Root Note");
    addAndMakeVisible(rootNoteSlider.get());
    rootNoteSlider->setTooltip("Transposed note triggering and Scale Channel input will transpose the pattern relative to this note");
    rootNoteSlider->setRange(-1, 127, 1);
    rootNoteSlider->setSliderStyle(juce::Slider::LinearBar);
    rootNoteSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    rootNoteSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    rootNoteSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    rootNoteSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    rootNoteSlider->onValueChange = [this]
    {
        handleRootSliderChange();
    };

    rootNoteSlider->setBounds(76, 174, 64, 20);

    rootNoteLabel = std::make_unique<juce::Label>("new label", "Root Note:");
    addAndMakeVisible(rootNoteLabel.get());
    rootNoteLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    rootNoteLabel->setJustificationType(juce::Justification::centred);
    rootNoteLabel->setEditable(false, false, false);
    rootNoteLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    rootNoteLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    rootNoteLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    rootNoteLabel->setBounds(15, 176, 64, 16);

    numLoopsSlider = std::make_unique<VSTSlider>("NumLoops");
    addAndMakeVisible(numLoopsSlider.get());
    numLoopsSlider->setTooltip("Number of times to loop playback");
    numLoopsSlider->setRange(0, 64, 1);
    numLoopsSlider->setSliderStyle(juce::Slider::LinearBar);
    numLoopsSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    numLoopsSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    numLoopsSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    numLoopsSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    numLoopsSlider->onValueChange = [this]
    {
        handleNumLoopsSliderChange();
    };

    numLoopsSlider->setBounds(155, 174, 64, 20);

    nextSlotSlider = std::make_unique<VSTSlider>("NextSlot");
    addAndMakeVisible(nextSlotSlider.get());
    nextSlotSlider->setTooltip("What to do after the selected number of loops have played");
    nextSlotSlider->setRange(0, 16, 1);
    nextSlotSlider->setSliderStyle(juce::Slider::LinearBar);
    nextSlotSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    nextSlotSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    nextSlotSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    nextSlotSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    nextSlotSlider->onValueChange = [this]
    {
        handleNextSlotSliderChange();
    };

    nextSlotSlider->setBounds(234, 174, 64, 20);

    muteGroupSlider = std::make_unique<VSTSlider>("TriggerChannel");
    addAndMakeVisible(muteGroupSlider.get());
    muteGroupSlider->setTooltip("Only one slot with the same Mute Group number can be played at the same time");
    muteGroupSlider->setRange(0, 16, 1);
    muteGroupSlider->setSliderStyle(juce::Slider::LinearBar);
    muteGroupSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    muteGroupSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    muteGroupSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    muteGroupSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    muteGroupSlider->onValueChange = [this]
    {
        handleMuteGroupSliderChange();
    };

    muteGroupSlider->setBounds(312, 174, 64, 20);

    muteGroupLabel = std::make_unique<juce::Label>("Trigger Channel", "Mute Group");
    addAndMakeVisible(muteGroupLabel.get());
    muteGroupLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    muteGroupLabel->setJustificationType(juce::Justification::centred);
    muteGroupLabel->setEditable(false, false, false);
    muteGroupLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    muteGroupLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    muteGroupLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    muteGroupLabel->setBounds(307, 157, 74, 16);

    loopSettingsLabel = std::make_unique<juce::Label>("Sync:", "Loop Settings");
    addAndMakeVisible(loopSettingsLabel.get());
    loopSettingsLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    loopSettingsLabel->setJustificationType(juce::Justification::centred);
    loopSettingsLabel->setEditable(false, false, false);
    loopSettingsLabel->setColour(juce::Label::textColourId, juce::Colour(0xff9f9f9f));
    loopSettingsLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopSettingsLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopSettingsLabel->setBounds(12, 247, 125, 16);

    useTransposeChannelButton = std::make_unique<juce::ToggleButton>("new toggle button");
    addAndMakeVisible(useTransposeChannelButton.get());
    useTransposeChannelButton->setTooltip("When checked, notes on selected \"Transpose Ch\" will apply to \"Semitones\" and \"Octave\" settings, relative to \"Root Note\"");
    useTransposeChannelButton->setButtonText("Use Transp Ch");
    useTransposeChannelButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    useTransposeChannelButton->setBounds(148, 252, 130, 17);
    useTransposeChannelButton->onClick = [this]
    {
        handleUseTrChannelButtonClick();
    };

    immediateTransposeButton = std::make_unique<juce::ToggleButton>("new toggle button");
    addAndMakeVisible(immediateTransposeButton.get());
    immediateTransposeButton->setTooltip("When checked, playing notes will be split and transposed immediately on changes to Semitones / Octave / Force to Scale / Master Transpose settings");
    immediateTransposeButton->setButtonText("Split");
    immediateTransposeButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    immediateTransposeButton->setBounds(247, 252, 48, 17);
    immediateTransposeButton->onClick = [this]
    {
        handleImmediateTransposeButtonClick();
    };

    transpose10Button = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(transpose10Button.get());
    transpose10Button->setButtonText("transpose channel 10");
    transpose10Button->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    transpose10Button->setBounds(306, 252, 72, 13);
    transpose10Button->onClick = [this]
    {
        handleTranspose10ButtonClick();
    };

    scaleChannelSlider = std::make_unique<VSTSlider>("ScaleChannel");
    addAndMakeVisible(scaleChannelSlider.get());
    scaleChannelSlider->setTooltip("Input notes on this channel will affect Semitones, Octave, and/or Force to Scale settings where \"Use Scale Ch\" is enabled");
    scaleChannelSlider->setRange(1, 16, 1);
    scaleChannelSlider->setSliderStyle(juce::Slider::LinearBar);
    scaleChannelSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    scaleChannelSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    scaleChannelSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    scaleChannelSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    scaleChannelSlider->onValueChange = [this]
    {
        handleScaleChannelSliderChange();
    };

    scaleChannelSlider->setBounds(10, 283, 60, 20);

    scaleChannelLabel = std::make_unique<juce::Label>("scale ch", "Scale Ch");
    addAndMakeVisible(scaleChannelLabel.get());
    scaleChannelLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    scaleChannelLabel->setJustificationType(juce::Justification::centred);
    scaleChannelLabel->setEditable(false, false, false);
    scaleChannelLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    scaleChannelLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    scaleChannelLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    scaleChannelLabel->setBounds(13, 267, 54, 16);

    transposeChannelSlider = std::make_unique<VSTSlider>("TransposeChannel");
    addAndMakeVisible(transposeChannelSlider.get());
    transposeChannelSlider->setTooltip("Input notes on this channel will affect Semitones, Octave, and/or Force to Scale settings where \"Use Transp Ch\" is enabled");
    transposeChannelSlider->setRange(1, 16, 1);
    transposeChannelSlider->setSliderStyle(juce::Slider::LinearBar);
    transposeChannelSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    transposeChannelSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    transposeChannelSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    transposeChannelSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    transposeChannelSlider->onValueChange = [this]
    {
        handleTransposeChannelSliderChange();
    };

    transposeChannelSlider->setBounds(76, 283, 60, 20);

    transposeChannelLabel = std::make_unique<juce::Label>("tr ch", "Transpose Ch");
    addAndMakeVisible(transposeChannelLabel.get());
    transposeChannelLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    transposeChannelLabel->setJustificationType(juce::Justification::centred);
    transposeChannelLabel->setEditable(false, false, false);
    transposeChannelLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    transposeChannelLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    transposeChannelLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    transposeChannelLabel->setBounds(66, 267, 80, 16);

    transposeSlider = std::make_unique<VSTSlider>("Transpose");
    addAndMakeVisible(transposeSlider.get());
    transposeSlider->setTooltip("Transposition applied to the current slot");
    transposeSlider->setRange(-12, 12, 1);
    transposeSlider->setSliderStyle(juce::Slider::LinearBar);
    transposeSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    transposeSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    transposeSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    transposeSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    transposeSlider->onValueChange = [this]
    {
        handleTransposeSliderChange();
    };

    transposeSlider->setBounds(146, 283, 72, 20);

    transposeLabel = std::make_unique<juce::Label>("new label", "Semitones");
    addAndMakeVisible(transposeLabel.get());
    transposeLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    transposeLabel->setJustificationType(juce::Justification::centred);
    transposeLabel->setEditable(false, false, false);
    transposeLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    transposeLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    transposeLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    transposeLabel->setBounds(146, 267, 72, 16);

    octaveSlider = std::make_unique<VSTSlider>("Octave Shift");
    addAndMakeVisible(octaveSlider.get());
    octaveSlider->setTooltip("Transposition by octave for the current slot");
    octaveSlider->setRange(-4, 4, 1);
    octaveSlider->setSliderStyle(juce::Slider::LinearBar);
    octaveSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    octaveSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    octaveSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    octaveSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    octaveSlider->onValueChange = [this]
    {
        handleOctaveSliderChange();
    };

    octaveSlider->setBounds(226, 283, 72, 20);

    octaveLabel = std::make_unique<juce::Label>("new label", "Octave");
    addAndMakeVisible(octaveLabel.get());
    octaveLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    octaveLabel->setJustificationType(juce::Justification::centred);
    octaveLabel->setEditable(false, false, false);
    octaveLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    octaveLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    octaveLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    octaveLabel->setBounds(226, 267, 72, 16);

    velocitySlider = std::make_unique<VSTSlider>("Velocity Offset");
    addAndMakeVisible(velocitySlider.get());
    velocitySlider->setTooltip("Velocity adjustment for the current slot");
    velocitySlider->setRange(0, 200, 1);
    velocitySlider->setSliderStyle(juce::Slider::LinearBar);
    velocitySlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    velocitySlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    velocitySlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    velocitySlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    velocitySlider->onValueChange = [this]
    {
        handleVelocitySliderChange();
    };

    velocitySlider->setBounds(306, 283, 72, 20);

    velocityLabel = std::make_unique<juce::Label>("new label", "Velocity");
    addAndMakeVisible(velocityLabel.get());
    velocityLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    velocityLabel->setJustificationType(juce::Justification::centred);
    velocityLabel->setEditable(false, false, false);
    velocityLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    velocityLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    velocityLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    velocityLabel->setBounds(301, 267, 80, 16);

    forceToKeyButton = std::make_unique<juce::ToggleButton>("new toggle button");
    addAndMakeVisible(forceToKeyButton.get());
    forceToKeyButton->setTooltip("When checked, played notes will be fitted to the defined scale");
    forceToKeyButton->setButtonText("Force to Scale");
    forceToKeyButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    forceToKeyButton->setBounds(11, 312, 99, 17);
    forceToKeyButton->onClick = [this]
    {
        handleForceToKeyButtonClick();
    };

    forceModeComboBox = std::make_unique<juce::ComboBox>("Force To Scale Mode");
    addAndMakeVisible(forceModeComboBox.get());
    forceModeComboBox->setEditableText(false);
    forceModeComboBox->setJustificationType(juce::Justification::centredLeft);
    forceModeComboBox->setTextWhenNothingSelected("Nearest");
    forceModeComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    forceModeComboBox->addItem("Nearest", 1);
    forceModeComboBox->addItem("Up", 2);
    forceModeComboBox->addItem("Down", 3);
    forceModeComboBox->addItem("Block", 4);
    forceModeComboBox->onChange = [this]
    {
        handleForceModeComboBoxChange();
    };

    forceModeComboBox->setBounds(110, 313, 61, 16);

    useScaleChannelButton = std::make_unique<juce::ToggleButton>("new toggle button");
    addAndMakeVisible(useScaleChannelButton.get());
    useScaleChannelButton->setTooltip("When checked, input notes on \"Scale Ch\" will be used to define the scale");
    useScaleChannelButton->setButtonText("Use Scale Channel");
    useScaleChannelButton->onClick = [this]
    {
        handleUseScaleChannelButtonClick();
    };
    useScaleChannelButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    useScaleChannelButton->setBounds(11, 329, 129, 17);

    shiftDownButton = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(shiftDownButton.get());
    shiftDownButton->setTooltip("Shift selected notes one semitone down");
    shiftDownButton->setButtonText("<");
    shiftDownButton->setConnectedEdges(juce::Button::ConnectedOnRight);
    shiftDownButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    shiftDownButton->setBounds(178, 314, 21, 30);
    shiftDownButton->onClick = [this]
    {
        handleShiftDownButtonClick();
    };

    keySelector = std::make_unique<KeySelector>(ownerFilter->keySelectorState);
    addAndMakeVisible(keySelector.get());
    keySelector->setName("new component");
    keySelector->setBounds(199, 315, 154, 28);

    shiftUpButton = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(shiftUpButton.get());
    shiftUpButton->setTooltip("Shift selected notes one semitone up");
    shiftUpButton->setButtonText(">");
    shiftUpButton->setConnectedEdges(juce::Button::ConnectedOnLeft);
    shiftUpButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    shiftUpButton->setBounds(353, 314, 21, 30);
    shiftUpButton->onClick = [this]
    {
        handleShiftUpButtonClick();
    };

    loopStartSlider = std::make_unique<VSTSlider>("Loop Start");
    addAndMakeVisible(loopStartSlider.get());
    loopStartSlider->setTooltip("Offsets the loop start time by this number of beats");
    loopStartSlider->setRange(-8, 8, 1);
    loopStartSlider->setSliderStyle(juce::Slider::LinearBar);
    loopStartSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    loopStartSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    loopStartSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    loopStartSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    loopStartSlider->onValueChange = [this]
    {
        handleLoopStartSliderChange();
    };

    loopStartSlider->setBounds(66, 367, 72, 20);

    startOffsetLabel = std::make_unique<juce::Label>("new label", "Start Offset");
    addAndMakeVisible(startOffsetLabel.get());
    startOffsetLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    startOffsetLabel->setJustificationType(juce::Justification::centred);
    startOffsetLabel->setEditable(false, false, false);
    startOffsetLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    startOffsetLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    startOffsetLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    startOffsetLabel->setBounds(66, 351, 72, 16);

    loopEndOffsetSlider = std::make_unique<VSTSlider>("Loop End");
    addAndMakeVisible(loopEndOffsetSlider.get());
    loopEndOffsetSlider->setTooltip("Offsets the loop end time by this number of beats");
    loopEndOffsetSlider->setRange(-8, 8, 1);
    loopEndOffsetSlider->setSliderStyle(juce::Slider::LinearBar);
    loopEndOffsetSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    loopEndOffsetSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    loopEndOffsetSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    loopEndOffsetSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    loopEndOffsetSlider->onValueChange = [this]
    {
        handleEndSliderChange();
    };

    loopEndOffsetSlider->setBounds(146, 367, 72, 20);

    loopEndOffsetLabel = std::make_unique<juce::Label>("new label", "End Offset");
    addAndMakeVisible(loopEndOffsetLabel.get());
    loopEndOffsetLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    loopEndOffsetLabel->setJustificationType(juce::Justification::centred);
    loopEndOffsetLabel->setEditable(false, false, false);
    loopEndOffsetLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    loopEndOffsetLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopEndOffsetLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopEndOffsetLabel->setBounds(146, 351, 72, 16);

    shiftPatternSlider = std::make_unique<VSTSlider>("Shift");
    addAndMakeVisible(shiftPatternSlider.get());
    shiftPatternSlider->setTooltip("Shifts the pattern by this number of beats, with wraparound");
    shiftPatternSlider->setRange(-8, 8, 1);
    shiftPatternSlider->setSliderStyle(juce::Slider::LinearBar);
    shiftPatternSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    shiftPatternSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    shiftPatternSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    shiftPatternSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    shiftPatternSlider->onValueChange = [this]
    {
        handleShiftSliderChange();
    };

    shiftPatternSlider->setBounds(226, 367, 72, 20);

    shiftPatternLabel = std::make_unique<juce::Label>("Shift", "Beat Shift");
    addAndMakeVisible(shiftPatternLabel.get());
    shiftPatternLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    shiftPatternLabel->setJustificationType(juce::Justification::centred);
    shiftPatternLabel->setEditable(false, false, false);
    shiftPatternLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    shiftPatternLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    shiftPatternLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    shiftPatternLabel->setBounds(226, 351, 72, 16);

    stretchSlider = std::make_unique<VSTSlider>("Loop Stretch");
    addAndMakeVisible(stretchSlider.get());
    stretchSlider->setTooltip("Playback speed, relative to host tempo");
    stretchSlider->setRange(-10, 10, 1);
    stretchSlider->setSliderStyle(juce::Slider::LinearBar);
    stretchSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    stretchSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    stretchSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    stretchSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    stretchSlider->onValueChange = [this]
    {
        handleStretchSliderChange();
    };

    stretchSlider->setBounds(306, 367, 72, 20);

    speedLabel = std::make_unique<juce::Label>("new label", "Speed");
    addAndMakeVisible(speedLabel.get());
    speedLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    speedLabel->setJustificationType(juce::Justification::centred);
    speedLabel->setEditable(false, false, false);
    speedLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    speedLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    speedLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    speedLabel->setBounds(306, 351, 72, 16);

    noteTriggerComboBox = std::make_unique<juce::ComboBox>("Note Trigger");
    addAndMakeVisible(noteTriggerComboBox.get());
    noteTriggerComboBox->setTooltip("For \"Transpose\" modes, pattern will be transposed relative to \"Root Note\"");
    noteTriggerComboBox->setEditableText(false);
    noteTriggerComboBox->setJustificationType(juce::Justification::centredLeft);
    noteTriggerComboBox->setTextWhenNothingSelected("Mono (Transposed)");
    noteTriggerComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    noteTriggerComboBox->addItem("Off", 1);
    noteTriggerComboBox->addItem("Mono (Transpose)", 2);
    noteTriggerComboBox->addItem("Poly (Transpose)", 3);
    noteTriggerComboBox->addItem("Mono (Orig. Key)", 4);
    noteTriggerComboBox->addSeparator();
    noteTriggerComboBox->onChange = [this]
    {
        handleNoteTriggerComboBoxChange();
    };

    noteTriggerComboBox->setBounds(146, 402, 106, 16);

    noteToggleButton = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(noteToggleButton.get());
    noteToggleButton->setTooltip("When enabled, Note On events will toggle playback, ignoring Note Off events; otherwise Note Off will stop playback");
    noteToggleButton->setButtonText("Toggle");
    noteToggleButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    noteToggleButton->setBounds(257, 402, 40, 16);
    noteToggleButton->onClick = [this]
    {
        handleNoteToggleButtonClick();
    };

    minTriggerNoteSlider = std::make_unique<VSTSlider>("Low Note");
    addAndMakeVisible(minTriggerNoteSlider.get());
    minTriggerNoteSlider->setTooltip("Lowest note to use for triggering");
    minTriggerNoteSlider->setRange(-1, 127, 1);
    minTriggerNoteSlider->setSliderStyle(juce::Slider::LinearBar);
    minTriggerNoteSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    minTriggerNoteSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    minTriggerNoteSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    minTriggerNoteSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    minTriggerNoteSlider->onValueChange = [this]
    {
        handleLowSliderChange();
    };

    minTriggerNoteSlider->setBounds(18, 440, 64, 20);

    minTriggerNoteLabel = std::make_unique<juce::Label>("new label", "Low Note");
    addAndMakeVisible(minTriggerNoteLabel.get());
    minTriggerNoteLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    minTriggerNoteLabel->setJustificationType(juce::Justification::centred);
    minTriggerNoteLabel->setEditable(false, false, false);
    minTriggerNoteLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    minTriggerNoteLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    minTriggerNoteLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    minTriggerNoteLabel->setBounds(18, 424, 64, 16);

    maxTriggerNoteSlider = std::make_unique<VSTSlider>("High Note");
    addAndMakeVisible(maxTriggerNoteSlider.get());
    maxTriggerNoteSlider->setTooltip("Highest note to use for triggering");
    maxTriggerNoteSlider->setRange(-1, 127, 1);
    maxTriggerNoteSlider->setSliderStyle(juce::Slider::LinearBar);
    maxTriggerNoteSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    maxTriggerNoteSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    maxTriggerNoteSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    maxTriggerNoteSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    maxTriggerNoteSlider->onValueChange = [this]
    {
        handleHighSliderChange();
    };

    maxTriggerNoteSlider->setBounds(90, 440, 64, 20);

    maxTriggerNoteLabel = std::make_unique<juce::Label>("new label", "High Note");
    addAndMakeVisible(maxTriggerNoteLabel.get());
    maxTriggerNoteLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    maxTriggerNoteLabel->setJustificationType(juce::Justification::centred);
    maxTriggerNoteLabel->setEditable(false, false, false);
    maxTriggerNoteLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    maxTriggerNoteLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    maxTriggerNoteLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    maxTriggerNoteLabel->setBounds(90, 424, 64, 16);

    triggerChannelSlider = std::make_unique<VSTSlider>("TriggerChannel");
    addAndMakeVisible(triggerChannelSlider.get());
    triggerChannelSlider->setTooltip("Channel to use for trigger notes");
    triggerChannelSlider->setRange(1, 16, 1);
    triggerChannelSlider->setSliderStyle(juce::Slider::LinearBar);
    triggerChannelSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    triggerChannelSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    triggerChannelSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    triggerChannelSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    triggerChannelSlider->onValueChange = [this]
    {
        handleTrigChanSliderChange();
    };

    triggerChannelSlider->setBounds(162, 440, 64, 20);

    triggerChannelLabel = std::make_unique<juce::Label>("Trigger Channel", "Channel");
    addAndMakeVisible(triggerChannelLabel.get());
    triggerChannelLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    triggerChannelLabel->setJustificationType(juce::Justification::centred);
    triggerChannelLabel->setEditable(false, false, false);
    triggerChannelLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    triggerChannelLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    triggerChannelLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    triggerChannelLabel->setBounds(161, 424, 64, 16);

    velocitySensSlider = std::make_unique<VSTSlider>("Velocity Sensitivity");
    addAndMakeVisible(velocitySensSlider.get());
    velocitySensSlider->setTooltip("Velocity Sensitivity (Input Velocity -> Output Velocity)");
    velocitySensSlider->setRange(0, 200, 1);
    velocitySensSlider->setSliderStyle(juce::Slider::LinearBar);
    velocitySensSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    velocitySensSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    velocitySensSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    velocitySensSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    velocitySensSlider->onValueChange = [this]
    {
        handleVelocitySensSliderChange();
    };

    velocitySensSlider->setBounds(234, 440, 64, 20);

    velocitySenSlider = std::make_unique<juce::Label>("new label", "VeloSens");
    addAndMakeVisible(velocitySenSlider.get());
    velocitySenSlider->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    velocitySenSlider->setJustificationType(juce::Justification::centred);
    velocitySenSlider->setEditable(false, false, false);
    velocitySenSlider->setColour(juce::Label::textColourId, juce::Colours::white);
    velocitySenSlider->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    velocitySenSlider->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    velocitySenSlider->setBounds(225, 424, 80, 16);

    slotIOChannelSlider = std::make_unique<VSTSlider>("Channel");
    addAndMakeVisible(slotIOChannelSlider.get());
    slotIOChannelSlider->setTooltip("Input and output channel for the current slot");
    slotIOChannelSlider->setRange(0, 16, 1);
    slotIOChannelSlider->setSliderStyle(juce::Slider::LinearBar);
    slotIOChannelSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    slotIOChannelSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    slotIOChannelSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    slotIOChannelSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    slotIOChannelSlider->onValueChange = [this]
    {
        handleChannelSliderChange();
    };

    slotIOChannelSlider->setBounds(314, 414, 64, 20);

    slotIOChannelLabel = std::make_unique<juce::Label>("I/O Channel", "I/O Channel");
    addAndMakeVisible(slotIOChannelLabel.get());
    slotIOChannelLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    slotIOChannelLabel->setJustificationType(juce::Justification::centred);
    slotIOChannelLabel->setEditable(false, false, false);
    slotIOChannelLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    slotIOChannelLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    slotIOChannelLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slotIOChannelLabel->setBounds(309, 398, 74, 16);

    transformFilterButton = std::make_unique<juce::TextButton>("Transform/Filter");
    addAndMakeVisible(transformFilterButton.get());
    transformFilterButton->setTooltip("Transform: all events in the pattern are channelized to the selected channel; Filter: only events with the selected channel will be output");
    transformFilterButton->setButtonText("Transform");
    transformFilterButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    transformFilterButton->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    transformFilterButton->setBounds(314, 440, 64, 20);
    transformFilterButton->onClick = [this]
    {
        handleTransformFilterButtonClick();
    };

    noteTriggeringLabel = std::make_unique<juce::Label>("Sync:", "Note Triggering");
    addAndMakeVisible(noteTriggeringLabel.get());
    noteTriggeringLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    noteTriggeringLabel->setJustificationType(juce::Justification::centred);
    noteTriggeringLabel->setEditable(false, false, false);
    noteTriggeringLabel->setColour(juce::Label::textColourId, juce::Colour(0xff9f9f9f));
    noteTriggeringLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    noteTriggeringLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    noteTriggeringLabel->setBounds(12, 469, 125, 16);

    textButton17 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton17.get());
    textButton17->setButtonText("1");
    textButton17->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton17->setRadioGroupId(1);

    textButton17->setBounds(4, -97, 25, 24);

    textButton18 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton18.get());
    textButton18->setButtonText("2");
    textButton18->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton18->setRadioGroupId(1);
    textButton18->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton18->setBounds(29, -97, 23, 24);

    textButton19 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton19.get());
    textButton19->setButtonText("3");
    textButton19->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton19->setRadioGroupId(1);

    textButton19->setBounds(52, -97, 23, 24);

    textButton20 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton20.get());
    textButton20->setButtonText("4");
    textButton20->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton20->setRadioGroupId(1);

    textButton20->setBounds(75, -97, 23, 24);

    textButton21 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton21.get());
    textButton21->setButtonText("5");
    textButton21->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton21->setRadioGroupId(1);

    textButton21->setBounds(98, -97, 23, 24);

    textButton22 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton22.get());
    textButton22->setButtonText("6");
    textButton22->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton22->setRadioGroupId(1);

    textButton22->setBounds(121, -97, 23, 24);

    textButton23 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton23.get());
    textButton23->setButtonText("7");
    textButton23->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton23->setRadioGroupId(1);

    textButton23->setBounds(144, -97, 23, 24);

    textButton24 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton24.get());
    textButton24->setButtonText("8");
    textButton24->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton24->setRadioGroupId(1);

    textButton24->setBounds(167, -97, 23, 24);

    textButton25 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton25.get());
    textButton25->setButtonText("9");
    textButton25->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton25->setRadioGroupId(1);

    textButton25->setBounds(190, -97, 23, 24);

    textButton26 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton26.get());
    textButton26->setButtonText("10");
    textButton26->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton26->setRadioGroupId(1);

    textButton26->setBounds(213, -97, 25, 24);

    textButton27 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton27.get());
    textButton27->setButtonText("11");
    textButton27->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton27->setRadioGroupId(1);

    textButton27->setBounds(238, -97, 25, 24);

    textButton28 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton28.get());
    textButton28->setButtonText("12");
    textButton28->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton28->setRadioGroupId(1);

    textButton28->setBounds(263, -97, 25, 24);

    textButton29 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton29.get());
    textButton29->setButtonText("13");
    textButton29->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton29->setRadioGroupId(1);

    textButton29->setBounds(288, -97, 25, 24);

    textButton30 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton30.get());
    textButton30->setButtonText("14");
    textButton30->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton30->setRadioGroupId(1);

    textButton30->setBounds(313, -97, 25, 24);

    textButton31 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton31.get());
    textButton31->setButtonText("15");
    textButton31->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton31->setRadioGroupId(1);

    textButton31->setBounds(338, -97, 25, 24);

    textButton32 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton32.get());
    textButton32->setButtonText("16");
    textButton32->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton32->setRadioGroupId(1);

    textButton32->setBounds(363, -97, 28, 24);

    textButton33 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton33.get());
    textButton33->setButtonText("1");
    textButton33->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton33->setRadioGroupId(1);

    textButton33->setBounds(1, -73, 25, 24);

    textButton34 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton34.get());
    textButton34->setButtonText("2");
    textButton34->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton34->setRadioGroupId(1);
    textButton34->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton34->setBounds(26, -73, 23, 24);

    textButton35 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton35.get());
    textButton35->setButtonText("3");
    textButton35->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton35->setRadioGroupId(1);

    textButton35->setBounds(49, -73, 23, 24);

    textButton36 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton36.get());
    textButton36->setButtonText("4");
    textButton36->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton36->setRadioGroupId(1);

    textButton36->setBounds(72, -73, 23, 24);

    textButton37 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton37.get());
    textButton37->setButtonText("5");
    textButton37->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton37->setRadioGroupId(1);

    textButton37->setBounds(95, -73, 23, 24);

    textButton38 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton38.get());
    textButton38->setButtonText("6");
    textButton38->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton38->setRadioGroupId(1);

    textButton38->setBounds(118, -73, 23, 24);

    textButton39 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton39.get());
    textButton39->setButtonText("7");
    textButton39->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton39->setRadioGroupId(1);

    textButton39->setBounds(141, -73, 23, 24);

    textButton40 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton40.get());
    textButton40->setButtonText("8");
    textButton40->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton40->setRadioGroupId(1);

    textButton40->setBounds(164, -73, 23, 24);

    textButton41 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton41.get());
    textButton41->setButtonText("9");
    textButton41->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton41->setRadioGroupId(1);

    textButton41->setBounds(187, -73, 23, 24);

    textButton42 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton42.get());
    textButton42->setButtonText("10");
    textButton42->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton42->setRadioGroupId(1);

    textButton42->setBounds(210, -73, 25, 24);

    textButton43 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton43.get());
    textButton43->setButtonText("11");
    textButton43->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton43->setRadioGroupId(1);

    textButton43->setBounds(235, -73, 25, 24);

    textButton44 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton44.get());
    textButton44->setButtonText("12");
    textButton44->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton44->setRadioGroupId(1);

    textButton44->setBounds(260, -73, 25, 24);

    textButton45 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton45.get());
    textButton45->setButtonText("13");
    textButton45->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton45->setRadioGroupId(1);

    textButton45->setBounds(285, -73, 25, 24);

    textButton46 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton46.get());
    textButton46->setButtonText("14");
    textButton46->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton46->setRadioGroupId(1);

    textButton46->setBounds(310, -73, 25, 24);

    textButton47 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton47.get());
    textButton47->setButtonText("15");
    textButton47->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton47->setRadioGroupId(1);

    textButton47->setBounds(335, -73, 25, 24);

    textButton48 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton48.get());
    textButton48->setButtonText("16");
    textButton48->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton48->setRadioGroupId(1);

    textButton48->setBounds(360, -73, 28, 24);

    textButton49 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton49.get());
    textButton49->setButtonText("1");
    textButton49->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton49->setRadioGroupId(1);

    textButton49->setBounds(-2, -40, 25, 24);

    textButton50 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton50.get());
    textButton50->setButtonText("2");
    textButton50->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton50->setRadioGroupId(1);
    textButton50->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton50->setBounds(23, -40, 23, 24);

    textButton51 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton51.get());
    textButton51->setButtonText("3");
    textButton51->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton51->setRadioGroupId(1);

    textButton51->setBounds(46, -40, 23, 24);

    textButton52 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton52.get());
    textButton52->setButtonText("4");
    textButton52->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton52->setRadioGroupId(1);

    textButton52->setBounds(69, -40, 23, 24);

    textButton53 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton53.get());
    textButton53->setButtonText("5");
    textButton53->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton53->setRadioGroupId(1);

    textButton53->setBounds(92, -40, 23, 24);

    textButton54 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton54.get());
    textButton54->setButtonText("6");
    textButton54->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton54->setRadioGroupId(1);

    textButton54->setBounds(115, -40, 23, 24);

    textButton55 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton55.get());
    textButton55->setButtonText("7");
    textButton55->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton55->setRadioGroupId(1);

    textButton55->setBounds(138, -40, 23, 24);

    textButton56 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton56.get());
    textButton56->setButtonText("8");
    textButton56->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton56->setRadioGroupId(1);

    textButton56->setBounds(161, -40, 23, 24);

    textButton57 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton57.get());
    textButton57->setButtonText("9");
    textButton57->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton57->setRadioGroupId(1);

    textButton57->setBounds(184, -40, 23, 24);

    textButton58 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton58.get());
    textButton58->setButtonText("10");
    textButton58->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton58->setRadioGroupId(1);

    textButton58->setBounds(207, -40, 25, 24);

    textButton59 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton59.get());
    textButton59->setButtonText("11");
    textButton59->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton59->setRadioGroupId(1);

    textButton59->setBounds(232, -40, 25, 24);

    textButton60 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton60.get());
    textButton60->setButtonText("12");
    textButton60->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton60->setRadioGroupId(1);

    textButton60->setBounds(257, -40, 25, 24);

    textButton61 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton61.get());
    textButton61->setButtonText("13");
    textButton61->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton61->setRadioGroupId(1);

    textButton61->setBounds(282, -40, 25, 24);

    textButton62 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton62.get());
    textButton62->setButtonText("14");
    textButton62->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton62->setRadioGroupId(1);

    textButton62->setBounds(307, -40, 25, 24);

    textButton63 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton63.get());
    textButton63->setButtonText("15");
    textButton63->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton63->setRadioGroupId(1);

    textButton63->setBounds(332, -40, 25, 24);

    textButton64 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton64.get());
    textButton64->setButtonText("16");
    textButton64->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton64->setRadioGroupId(1);

    textButton64->setBounds(357, -40, 28, 24);

    textButton65 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton65.get());
    textButton65->setButtonText("1");
    textButton65->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton65->setRadioGroupId(1);

    textButton65->setBounds(383, -93, 25, 24);

    textButton66 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton66.get());
    textButton66->setButtonText("2");
    textButton66->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton66->setRadioGroupId(1);
    textButton66->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton66->setBounds(408, -93, 23, 24);

    textButton67 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton67.get());
    textButton67->setButtonText("3");
    textButton67->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton67->setRadioGroupId(1);

    textButton67->setBounds(431, -93, 23, 24);

    textButton68 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton68.get());
    textButton68->setButtonText("4");
    textButton68->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton68->setRadioGroupId(1);

    textButton68->setBounds(454, -93, 23, 24);

    textButton69 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton69.get());
    textButton69->setButtonText("5");
    textButton69->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton69->setRadioGroupId(1);

    textButton69->setBounds(477, -93, 23, 24);

    textButton70 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton70.get());
    textButton70->setButtonText("6");
    textButton70->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton70->setRadioGroupId(1);

    textButton70->setBounds(500, -93, 23, 24);

    textButton71 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton71.get());
    textButton71->setButtonText("7");
    textButton71->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton71->setRadioGroupId(1);

    textButton71->setBounds(523, -93, 23, 24);

    textButton72 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton72.get());
    textButton72->setButtonText("8");
    textButton72->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton72->setRadioGroupId(1);

    textButton72->setBounds(546, -93, 23, 24);

    textButton73 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton73.get());
    textButton73->setButtonText("9");
    textButton73->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton73->setRadioGroupId(1);

    textButton73->setBounds(569, -93, 23, 24);

    textButton74 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton74.get());
    textButton74->setButtonText("10");
    textButton74->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton74->setRadioGroupId(1);

    textButton74->setBounds(592, -93, 25, 24);

    textButton75 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton75.get());
    textButton75->setButtonText("11");
    textButton75->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton75->setRadioGroupId(1);

    textButton75->setBounds(617, -93, 25, 24);

    textButton76 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton76.get());
    textButton76->setButtonText("12");
    textButton76->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton76->setRadioGroupId(1);

    textButton76->setBounds(642, -93, 25, 24);

    textButton77 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton77.get());
    textButton77->setButtonText("13");
    textButton77->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton77->setRadioGroupId(1);

    textButton77->setBounds(667, -93, 25, 24);

    textButton78 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton78.get());
    textButton78->setButtonText("14");
    textButton78->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton78->setRadioGroupId(1);

    textButton78->setBounds(692, -93, 25, 24);

    textButton79 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton79.get());
    textButton79->setButtonText("15");
    textButton79->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton79->setRadioGroupId(1);

    textButton79->setBounds(717, -93, 25, 24);

    textButton80 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton80.get());
    textButton80->setButtonText("16");
    textButton80->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton80->setRadioGroupId(1);

    textButton80->setBounds(742, -93, 28, 24);

    textButton81 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton81.get());
    textButton81->setButtonText("1");
    textButton81->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton81->setRadioGroupId(1);

    textButton81->setBounds(371, -76, 25, 24);

    textButton82 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton82.get());
    textButton82->setButtonText("2");
    textButton82->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton82->setRadioGroupId(1);
    textButton82->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton82->setBounds(396, -76, 23, 24);

    textButton83 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton83.get());
    textButton83->setButtonText("3");
    textButton83->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton83->setRadioGroupId(1);

    textButton83->setBounds(419, -76, 23, 24);

    textButton84 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton84.get());
    textButton84->setButtonText("4");
    textButton84->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton84->setRadioGroupId(1);

    textButton84->setBounds(442, -76, 23, 24);

    textButton85 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton85.get());
    textButton85->setButtonText("5");
    textButton85->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton85->setRadioGroupId(1);

    textButton85->setBounds(465, -76, 23, 24);

    textButton86 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton86.get());
    textButton86->setButtonText("6");
    textButton86->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton86->setRadioGroupId(1);

    textButton86->setBounds(488, -76, 23, 24);

    textButton87 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton87.get());
    textButton87->setButtonText("7");
    textButton87->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton87->setRadioGroupId(1);

    textButton87->setBounds(511, -76, 23, 24);

    textButton88 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton88.get());
    textButton88->setButtonText("8");
    textButton88->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton88->setRadioGroupId(1);

    textButton88->setBounds(534, -76, 23, 24);

    textButton89 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton89.get());
    textButton89->setButtonText("9");
    textButton89->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton89->setRadioGroupId(1);

    textButton89->setBounds(557, -76, 23, 24);

    textButton90 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton90.get());
    textButton90->setButtonText("10");
    textButton90->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton90->setRadioGroupId(1);

    textButton90->setBounds(580, -76, 25, 24);

    textButton91 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton91.get());
    textButton91->setButtonText("11");
    textButton91->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton91->setRadioGroupId(1);

    textButton91->setBounds(605, -76, 25, 24);

    textButton92 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton92.get());
    textButton92->setButtonText("12");
    textButton92->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton92->setRadioGroupId(1);

    textButton92->setBounds(630, -76, 25, 24);

    textButton93 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton93.get());
    textButton93->setButtonText("13");
    textButton93->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton93->setRadioGroupId(1);

    textButton93->setBounds(655, -76, 25, 24);

    textButton94 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton94.get());
    textButton94->setButtonText("14");
    textButton94->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton94->setRadioGroupId(1);

    textButton94->setBounds(680, -76, 25, 24);

    textButton95 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton95.get());
    textButton95->setButtonText("15");
    textButton95->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton95->setRadioGroupId(1);

    textButton95->setBounds(705, -76, 25, 24);

    textButton96 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton96.get());
    textButton96->setButtonText("16");
    textButton96->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton96->setRadioGroupId(1);

    textButton96->setBounds(730, -76, 28, 24);

    textButton97 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton97.get());
    textButton97->setButtonText("1");
    textButton97->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton97->setRadioGroupId(1);

    textButton97->setBounds(413, -59, 25, 24);

    textButton98 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton98.get());
    textButton98->setButtonText("2");
    textButton98->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton98->setRadioGroupId(1);
    textButton98->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton98->setBounds(438, -59, 23, 24);

    textButton99 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton99.get());
    textButton99->setButtonText("3");
    textButton99->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton99->setRadioGroupId(1);

    textButton99->setBounds(461, -59, 23, 24);

    textButton100 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton100.get());
    textButton100->setButtonText("4");
    textButton100->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton100->setRadioGroupId(1);

    textButton100->setBounds(484, -59, 23, 24);

    textButton101 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton101.get());
    textButton101->setButtonText("5");
    textButton101->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton101->setRadioGroupId(1);

    textButton101->setBounds(507, -59, 23, 24);

    textButton102 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton102.get());
    textButton102->setButtonText("6");
    textButton102->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton102->setRadioGroupId(1);

    textButton102->setBounds(530, -59, 23, 24);

    textButton103 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton103.get());
    textButton103->setButtonText("7");
    textButton103->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton103->setRadioGroupId(1);

    textButton103->setBounds(553, -59, 23, 24);

    textButton104 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton104.get());
    textButton104->setButtonText("8");
    textButton104->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton104->setRadioGroupId(1);

    textButton104->setBounds(576, -59, 23, 24);

    textButton105 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton105.get());
    textButton105->setButtonText("9");
    textButton105->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton105->setRadioGroupId(1);

    textButton105->setBounds(599, -59, 23, 24);

    textButton106 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton106.get());
    textButton106->setButtonText("10");
    textButton106->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton106->setRadioGroupId(1);

    textButton106->setBounds(622, -59, 25, 24);

    textButton107 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton107.get());
    textButton107->setButtonText("11");
    textButton107->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton107->setRadioGroupId(1);

    textButton107->setBounds(647, -59, 25, 24);

    textButton108 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton108.get());
    textButton108->setButtonText("12");
    textButton108->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton108->setRadioGroupId(1);

    textButton108->setBounds(672, -59, 25, 24);

    textButton109 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton109.get());
    textButton109->setButtonText("13");
    textButton109->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton109->setRadioGroupId(1);

    textButton109->setBounds(697, -59, 25, 24);

    textButton110 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton110.get());
    textButton110->setButtonText("14");
    textButton110->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton110->setRadioGroupId(1);

    textButton110->setBounds(722, -59, 25, 24);

    textButton111 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton111.get());
    textButton111->setButtonText("15");
    textButton111->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton111->setRadioGroupId(1);

    textButton111->setBounds(747, -59, 25, 24);

    textButton112 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton112.get());
    textButton112->setButtonText("16");
    textButton112->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton112->setRadioGroupId(1);

    textButton112->setBounds(772, -59, 28, 24);

    textButton113 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton113.get());
    textButton113->setButtonText("1");
    textButton113->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton113->setRadioGroupId(1);

    textButton113->setBounds(408, -29, 25, 24);

    textButton114 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton114.get());
    textButton114->setButtonText("2");
    textButton114->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton114->setRadioGroupId(1);
    textButton114->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton114->setBounds(433, -29, 23, 24);

    textButton115 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton115.get());
    textButton115->setButtonText("3");
    textButton115->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton115->setRadioGroupId(1);

    textButton115->setBounds(456, -29, 23, 24);

    textButton116 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton116.get());
    textButton116->setButtonText("4");
    textButton116->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton116->setRadioGroupId(1);

    textButton116->setBounds(479, -29, 23, 24);

    textButton117 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton117.get());
    textButton117->setButtonText("5");
    textButton117->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton117->setRadioGroupId(1);

    textButton117->setBounds(502, -29, 23, 24);

    textButton118 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton118.get());
    textButton118->setButtonText("6");
    textButton118->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton118->setRadioGroupId(1);

    textButton118->setBounds(525, -29, 23, 24);

    textButton119 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton119.get());
    textButton119->setButtonText("7");
    textButton119->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton119->setRadioGroupId(1);

    textButton119->setBounds(548, -29, 23, 24);

    textButton120 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton120.get());
    textButton120->setButtonText("8");
    textButton120->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton120->setRadioGroupId(1);

    textButton120->setBounds(571, -29, 23, 24);

    textButton121 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton121.get());
    textButton121->setButtonText("9");
    textButton121->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton121->setRadioGroupId(1);

    textButton121->setBounds(594, -29, 23, 24);

    textButton122 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton122.get());
    textButton122->setButtonText("10");
    textButton122->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton122->setRadioGroupId(1);

    textButton122->setBounds(617, -29, 25, 24);

    textButton123 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton123.get());
    textButton123->setButtonText("11");
    textButton123->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton123->setRadioGroupId(1);

    textButton123->setBounds(642, -29, 25, 24);

    textButton124 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton124.get());
    textButton124->setButtonText("12");
    textButton124->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton124->setRadioGroupId(1);

    textButton124->setBounds(667, -29, 25, 24);

    textButton125 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton125.get());
    textButton125->setButtonText("13");
    textButton125->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton125->setRadioGroupId(1);

    textButton125->setBounds(692, -29, 25, 24);

    textButton126 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton126.get());
    textButton126->setButtonText("14");
    textButton126->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton126->setRadioGroupId(1);

    textButton126->setBounds(717, -29, 25, 24);

    textButton127 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton127.get());
    textButton127->setButtonText("15");
    textButton127->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton127->setRadioGroupId(1);

    textButton127->setBounds(742, -29, 25, 24);

    textButton128 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(textButton128.get());
    textButton128->setButtonText("16");
    textButton128->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton128->setRadioGroupId(1);

    textButton128->setBounds(767, -29, 28, 24);

    loopInfoLabel = std::make_unique<juce::Label>("Loop Info", "label text");
    addAndMakeVisible(loopInfoLabel.get());
    loopInfoLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    loopInfoLabel->setJustificationType(juce::Justification::centredLeft);
    loopInfoLabel->setEditable(false, false, false);
    loopInfoLabel->setColour(juce::Label::textColourId, juce::Colours::black);
    loopInfoLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopInfoLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopInfoLabel->setBounds(14, 205, 272, 16);

    loopInfoLabel2 = std::make_unique<juce::Label>("Loop Info 2", "label text");
    addAndMakeVisible(loopInfoLabel2.get());
    loopInfoLabel2->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    loopInfoLabel2->setJustificationType(juce::Justification::centredLeft);
    loopInfoLabel2->setEditable(false, false, false);
    loopInfoLabel2->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopInfoLabel2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopInfoLabel2->setBounds(14, 225, 272, 16);

    loopManipulationLabel = std::make_unique<juce::Label>("Sync:", "Loop Manipulation");
    addAndMakeVisible(loopManipulationLabel.get());
    loopManipulationLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    loopManipulationLabel->setJustificationType(juce::Justification::centred);
    loopManipulationLabel->setEditable(false, false, false);
    loopManipulationLabel->setColour(juce::Label::textColourId, juce::Colour(0xff9f9f9f));
    loopManipulationLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopManipulationLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopManipulationLabel->setBounds(14, 391, 125, 16);

    snapButton = std::make_unique<juce::ToggleButton>("new toggle button");
    addAndMakeVisible(snapButton.get());
    snapButton->setTooltip("Toggle Snap to Grid");
    snapButton->setButtonText("Snap");
    snapButton->setBounds(392, 61, 59, 24);
    snapButton->onClick = [this]
    {
        handleSnapButtonClick();
    };

    quantize2ComboBox = std::make_unique<juce::ComboBox>("PR Quantize Step");
    addAndMakeVisible(quantize2ComboBox.get());
    quantize2ComboBox->setTooltip("Grid Size");
    quantize2ComboBox->setEditableText(false);
    quantize2ComboBox->setJustificationType(juce::Justification::centredLeft);
    quantize2ComboBox->setTextWhenNothingSelected("32nd");
    quantize2ComboBox->setTextWhenNoChoicesAvailable("(no choices)");
    quantize2ComboBox->addItem("4th", 1);
    quantize2ComboBox->addItem("8th", 2);
    quantize2ComboBox->addItem("16th", 3);
    quantize2ComboBox->addItem("32nd", 4);
    quantize2ComboBox->addItem("64th", 5);
    quantize2ComboBox->onChange = [this]
    {
        handleQuantize2ComboBoxChange();
    };

    quantize2ComboBox->setBounds(453, 64, 50, 18);

    tripletButton = std::make_unique<juce::TextButton>("Triplet");
    addAndMakeVisible(tripletButton.get());
    tripletButton->setTooltip("Toggle Triplet Note Grid");
    tripletButton->setButtonText("3");
    tripletButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    tripletButton->setRadioGroupId(2);
    tripletButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff8d8d8d));
    tripletButton->setBounds(506, 64, 18, 18);
    tripletButton->onClick = [this]
    {
        handleTripletButtonClick();
    };

    dottedButton = std::make_unique<juce::TextButton>("Dotted");
    addAndMakeVisible(dottedButton.get());
    dottedButton->setTooltip("Toggle Dotted Note Grid");
    dottedButton->setButtonText(".");
    dottedButton->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    dottedButton->setRadioGroupId(2);
    dottedButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff8d8d8d));
    dottedButton->setBounds(528, 64, 18, 18);
    dottedButton->onClick = [this]
    {
        handleDottedButtonClick();
    };

    numeratorLabel = std::make_unique<juce::Label>("new label", "4");
    addAndMakeVisible(numeratorLabel.get());
    numeratorLabel->setTooltip("Time Sig Numerator");
    numeratorLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    numeratorLabel->setJustificationType(juce::Justification::centredRight);
    numeratorLabel->setEditable(true, true, false);
    numeratorLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    numeratorLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    numeratorLabel->onTextChange = [this]
    {
        handleNumeratorLabelTextChange();
    };
    numeratorLabel->setBounds(555, 64, 27, 18);

    denominatorLabel = std::make_unique<juce::Label>("new label", "4");
    addAndMakeVisible(denominatorLabel.get());
    denominatorLabel->setTooltip("Time Sig Denominator");
    denominatorLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    denominatorLabel->setJustificationType(juce::Justification::centredLeft);
    denominatorLabel->setEditable(true, true, false);
    denominatorLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    denominatorLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    denominatorLabel->onTextChange = [this]
    {
        handleDenominatorLabelChange();
    };

    denominatorLabel->setBounds(584, 64, 29, 18);

    keepLengthToggleButton = std::make_unique<juce::ToggleButton>("OverdubMode");
    addAndMakeVisible(keepLengthToggleButton.get());
    keepLengthToggleButton->setTooltip("When checked, overdubbing will loop record into existing loop length");
    keepLengthToggleButton->setButtonText("Keep Length");
    keepLengthToggleButton->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    keepLengthToggleButton->setBounds(520, -2, 83, 16);
    keepLengthToggleButton->onClick = [this]
    {
        handleKeepLengthButtonClick();
    };

    zoomLabel = std::make_unique<juce::Label>("new label", "Zoom");
    addAndMakeVisible(zoomLabel.get());
    zoomLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    zoomLabel->setJustificationType(juce::Justification::centredLeft);
    zoomLabel->setEditable(false, false, false);
    zoomLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    zoomLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    zoomLabel->setBounds(613, 64, 46, 18);

    zoomOutButton = std::make_unique<juce::TextButton>("ZoomOut");
    addAndMakeVisible(zoomOutButton.get());
    zoomOutButton->setTooltip("Zoom Out (Ctrl-click for vertical)");
    zoomOutButton->setButtonText("-");
    zoomOutButton->setConnectedEdges(juce::Button::ConnectedOnRight);
    zoomOutButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));
    zoomOutButton->setBounds(656, 64, 18, 18);
    zoomOutButton->onClick = [this]
    {
        handleZoomOutButtonClick();
    };

    zoomInButton = std::make_unique<juce::TextButton>("ZoomIn");
    addAndMakeVisible(zoomInButton.get());
    zoomInButton->setTooltip("Zoom In (Ctrl-click for vertical)");
    zoomInButton->setButtonText("+");
    zoomInButton->setConnectedEdges(juce::Button::ConnectedOnLeft);
    zoomInButton->setRadioGroupId(2);
    zoomInButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));
    zoomInButton->setBounds(674, 64, 18, 18);
    zoomInButton->onClick = [this]
    {
        handleZoomInButtonClick();
    };

    removeBarButton = std::make_unique<juce::TextButton>("RemoveBar");
    addAndMakeVisible(removeBarButton.get());
    removeBarButton->setTooltip("Remove bar");
    removeBarButton->setButtonText("-");
    removeBarButton->setConnectedEdges(juce::Button::ConnectedOnRight);
    removeBarButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));
    removeBarButton->setBounds(706, 64, 18, 18);
    removeBarButton->onClick = [this]
    {
        handleRemoveBarButtonClick();
    };

    patternLengthLabel = std::make_unique<juce::Label>("Length",
                                                       "4");
    addAndMakeVisible(patternLengthLabel.get());
    patternLengthLabel->setTooltip("Pattern length in bars");
    patternLengthLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    patternLengthLabel->setJustificationType(juce::Justification::centred);
    patternLengthLabel->setEditable(true, true, false);
    patternLengthLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    patternLengthLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    patternLengthLabel->onTextChange = [this]
    {
        handleLengthLabelChange();
    };

    patternLengthLabel->setBounds(724, 65, 49, 16);

    addBarButton = std::make_unique<juce::TextButton>("AddBar");
    addAndMakeVisible(addBarButton.get());
    addBarButton->setTooltip("Add bar");
    addBarButton->setButtonText("+");
    addBarButton->setConnectedEdges(juce::Button::ConnectedOnLeft);
    addBarButton->setRadioGroupId(2);
    addBarButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));
    addBarButton->setBounds(773, 64, 18, 18);
    addBarButton->onClick = [this]
    {
        handleAddBarButtonClick();
    };

    keyboardViewport = std::make_unique<juce::Viewport>("Keyboard View");
    addAndMakeVisible(keyboardViewport.get());
    keyboardViewport->setScrollBarsShown(false, false);
    keyboardViewport->setScrollBarThickness(16);
    keyboardViewport->setViewedComponent(new juce::MidiKeyboardComponent(ownerFilter->kbstate, juce::MidiKeyboardComponent::verticalKeyboardFacingRight));

    timeline = std::make_unique<Timeline>();
    addAndMakeVisible(timeline.get());
    timeline->setName("timeline");

    pianoRollViewport = std::make_unique<PianoRollViewport>("Piano Roll View");
    addAndMakeVisible(pianoRollViewport.get());
    pianoRollViewport->setScrollBarThickness(16);
    pianoRollViewport->setViewedComponent(new PianoRoll(this->getFilter(), this, timeline.get()));

    pianoRollViewport->onMouseWheelMove = [this](const juce::MouseEvent& e, const juce::MouseWheelDetails& details)
    {
        handleMouseWheelMove(details);
    };

    resizer = std::make_unique<juce::ResizableCornerComponent>(this, &resizeLimits);
    addAndMakeVisible(resizer.get());

    recCCSlider = std::make_unique<VSTSlider>("recCC");
    addAndMakeVisible(recCCSlider.get());
    recCCSlider->setTooltip("CC Number to toggle recording to active slot");
    recCCSlider->setRange(-2, 127, 1);
    recCCSlider->setSliderStyle(juce::Slider::LinearBar);
    recCCSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    recCCSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    recCCSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    recCCSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    recCCSlider->onValueChange = [this]
    {
        handleRecCCSliderChange();
    };

    recCCSlider->setBounds(11, 149, 64, 20);

    playCCSlider = std::make_unique<VSTSlider>("playCC");
    addAndMakeVisible(playCCSlider.get());
    playCCSlider->setTooltip("CC Number to toggle play for active slot");
    playCCSlider->setRange(-2, 127, 1);
    playCCSlider->setSliderStyle(juce::Slider::LinearBar);
    playCCSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    playCCSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    playCCSlider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    playCCSlider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    playCCSlider->onValueChange = [this]
    {
        handlePlayCCSliderChange();
    };

    playCCSlider->setBounds(81, 149, 64, 20);

    cachedImage_piznew40_png_1 = juce::ImageCache::getFromMemory(piznew40_png, piznew40_pngSize);

    pianoRollViewport->setTimeline(timeline.get());
    pianoRollViewport->setKeyboard(keyboardViewport.get());
    this->setMouseClickGrabsKeyboardFocus(false);
    for (int slot = 0; slot < numSlots; slot++)
    {
        getButtonForSlot(slot)->addListener(this);
        getButtonForSlot(slot)->addMouseListener(this, false);
        getButtonForSlot(slot)->setMouseClickGrabsKeyboardFocus(false);
    }

    playCCSlider->setVisible(false);
    recCCSlider->setVisible(false);

    playButton->setMouseClickGrabsKeyboardFocus(false);
    recordButton->setMouseClickGrabsKeyboardFocus(false);
    overdubButton->setMouseClickGrabsKeyboardFocus(false);
    midiThruButton->setMouseClickGrabsKeyboardFocus(false);
    clearButton->setMouseClickGrabsKeyboardFocus(false);
    fixedLengthSlider->setMouseClickGrabsKeyboardFocus(false);
    stepSizeComboBox->setMouseClickGrabsKeyboardFocus(false);
    transposeSlider->setMouseClickGrabsKeyboardFocus(false);
    octaveSlider->setMouseClickGrabsKeyboardFocus(false);
    velocitySlider->setMouseClickGrabsKeyboardFocus(false);
    transposeLabel->setMouseClickGrabsKeyboardFocus(false);
    octaveLabel->setMouseClickGrabsKeyboardFocus(false);
    velocityLabel->setMouseClickGrabsKeyboardFocus(false);
    loopStartSlider->setMouseClickGrabsKeyboardFocus(false);
    startOffsetLabel->setMouseClickGrabsKeyboardFocus(false);
    loopEndOffsetSlider->setMouseClickGrabsKeyboardFocus(false);
    loopEndOffsetLabel->setMouseClickGrabsKeyboardFocus(false);
    stretchSlider->setMouseClickGrabsKeyboardFocus(false);
    speedLabel->setMouseClickGrabsKeyboardFocus(false);
    loopModeComboBox->setMouseClickGrabsKeyboardFocus(false);
    noteTriggerComboBox->setMouseClickGrabsKeyboardFocus(false);
    syncModeComboBox->setMouseClickGrabsKeyboardFocus(false);
    rootNoteSlider->setMouseClickGrabsKeyboardFocus(false);
    rootNoteLabel->setMouseClickGrabsKeyboardFocus(false);
    minTriggerNoteSlider->setMouseClickGrabsKeyboardFocus(false);
    minTriggerNoteLabel->setMouseClickGrabsKeyboardFocus(false);
    maxTriggerNoteSlider->setMouseClickGrabsKeyboardFocus(false);
    maxTriggerNoteLabel->setMouseClickGrabsKeyboardFocus(false);
    triggerChannelSlider->setMouseClickGrabsKeyboardFocus(false);
    triggerChannelLabel->setMouseClickGrabsKeyboardFocus(false);
    reloadButton->setMouseClickGrabsKeyboardFocus(false);
    quantizeComboBox->setMouseClickGrabsKeyboardFocus(false);
    playGroupLabel->setMouseClickGrabsKeyboardFocus(false);
    shiftPatternSlider->setMouseClickGrabsKeyboardFocus(false);
    shiftPatternLabel->setMouseClickGrabsKeyboardFocus(false);
    muteGroupLabel->setMouseClickGrabsKeyboardFocus(false);
    patternNameLabel->setMouseClickGrabsKeyboardFocus(false);
    saveButton->setMouseClickGrabsKeyboardFocus(false);
    hostSyncModeLabel->setMouseClickGrabsKeyboardFocus(false);
    noteTriggeringLabel->setMouseClickGrabsKeyboardFocus(false);
    loopInfoLabel->setMouseClickGrabsKeyboardFocus(false);
    loopInfoLabel2->setMouseClickGrabsKeyboardFocus(false);
    loopManipulationLabel->setMouseClickGrabsKeyboardFocus(false);
    slotIOChannelSlider->setMouseClickGrabsKeyboardFocus(false);
    slotIOChannelLabel->setMouseClickGrabsKeyboardFocus(false);
    transformFilterButton->setMouseClickGrabsKeyboardFocus(false);
    singleLoopToggleButton->setMouseClickGrabsKeyboardFocus(false);
    masterVelocityLabel->setMouseClickGrabsKeyboardFocus(false);
    tripletButton->setMouseClickGrabsKeyboardFocus(false);
    dottedButton->setMouseClickGrabsKeyboardFocus(false);
    zoomLabel->setMouseClickGrabsKeyboardFocus(false);
    zoomInButton->setMouseClickGrabsKeyboardFocus(false);
    zoomOutButton->setMouseClickGrabsKeyboardFocus(false);
    useScaleChannelButton->setMouseClickGrabsKeyboardFocus(false);
    useTransposeChannelButton->setMouseClickGrabsKeyboardFocus(false);
    waitForBarButton->setMouseClickGrabsKeyboardFocus(false);
    immediateTransposeButton->setMouseClickGrabsKeyboardFocus(false);
    forceToKeyButton->setMouseClickGrabsKeyboardFocus(false);
    snapButton->setMouseClickGrabsKeyboardFocus(false);
    nextSlotSlider->setMouseClickGrabsKeyboardFocus(false);
    numLoopsSlider->setMouseClickGrabsKeyboardFocus(false);
    addBarButton->setMouseClickGrabsKeyboardFocus(false);
    removeBarButton->setMouseClickGrabsKeyboardFocus(false);
    recCCSlider->setMouseClickGrabsKeyboardFocus(false);
    playCCSlider->setMouseClickGrabsKeyboardFocus(false);
    velocitySensSlider->setMouseClickGrabsKeyboardFocus(false);
    monitorButton->setMouseClickGrabsKeyboardFocus(false);

    stretchSlider->setOwner(getAudioProcessor(), kStretch);
    maxTriggerNoteSlider->setOwner(getAudioProcessor(), kNHigh);
    minTriggerNoteSlider->setOwner(getAudioProcessor(), kNLow);
    rootNoteSlider->setOwner(getAudioProcessor(), kRoot);
    transposeSlider->setOwner(getAudioProcessor(), kTranspose);
    octaveSlider->setOwner(getAudioProcessor(), kOctave);
    velocitySlider->setOwner(getAudioProcessor(), kVelocity);
    loopStartSlider->setOwner(getAudioProcessor(), kLoopStart);
    loopEndOffsetSlider->setOwner(getAudioProcessor(), kLoopEnd);
    shiftPatternSlider->setOwner(getAudioProcessor(), kShift);
    slotIOChannelSlider->setOwner(getAudioProcessor(), kChannel);
    triggerChannelSlider->setOwner(getAudioProcessor(), kTrigChan);
    fixedLengthSlider->setOwner(getAudioProcessor(), kFixedLength);
    playGroupSlider->setOwner(getAudioProcessor(), kPlayGroup);
    muteGroupSlider->setOwner(getAudioProcessor(), kMuteGroup);
    masterVelocitySlider->setOwner(getAudioProcessor(), kMasterVelocity);
    masterTransposeSlider->setOwner(getAudioProcessor(), kMasterTranspose);
    scaleChannelSlider->setOwner(getAudioProcessor(), kScaleChannel);
    transposeChannelSlider->setOwner(getAudioProcessor(), kTransposeChannel);
    numLoopsSlider->setOwner(getAudioProcessor(), kNumLoops);
    nextSlotSlider->setOwner(getAudioProcessor(), kNextSlot);
    recCCSlider->setOwner(getAudioProcessor(), kRecCC);
    playCCSlider->setOwner(getAudioProcessor(), kPlayCC);
    velocitySensSlider->setOwner(getAudioProcessor(), kVeloSens);

    numLoopsSlider->addMouseListener(this, true);
    nextSlotSlider->addMouseListener(this, true);
    slotIOChannelSlider->addMouseListener(this, true);
    playGroupSlider->addMouseListener(this, true);
    muteGroupSlider->addMouseListener(this, true);
    stretchSlider->addMouseListener(this, true);
    transposeSlider->addMouseListener(this, true);
    octaveSlider->addMouseListener(this, true);
    loopStartSlider->addMouseListener(this, true);
    loopEndOffsetSlider->addMouseListener(this, true);
    shiftPatternSlider->addMouseListener(this, true);
    velocitySlider->addMouseListener(this, true);
    velocitySensSlider->addMouseListener(this, true);
    masterVelocitySlider->addMouseListener(this, true);
    masterTransposeSlider->addMouseListener(this, true);
    numLoopsSlider->setDoubleClickReturnValue(true, 0);
    nextSlotSlider->setDoubleClickReturnValue(true, 0);
    slotIOChannelSlider->setDoubleClickReturnValue(true, 0);
    playGroupSlider->setDoubleClickReturnValue(true, 0);
    muteGroupSlider->setDoubleClickReturnValue(true, 0);
    stretchSlider->setDoubleClickReturnValue(true, 0);
    transposeSlider->setDoubleClickReturnValue(true, 0);
    octaveSlider->setDoubleClickReturnValue(true, 0);
    loopStartSlider->setDoubleClickReturnValue(true, 0);
    loopEndOffsetSlider->setDoubleClickReturnValue(true, 0);
    shiftPatternSlider->setDoubleClickReturnValue(true, 0);
    velocitySlider->setDoubleClickReturnValue(true, 100);
    velocitySensSlider->setDoubleClickReturnValue(true, 0);
    masterVelocitySlider->setDoubleClickReturnValue(true, 0);
    masterTransposeSlider->setDoubleClickReturnValue(true, 0);

    recordButton->addMouseListener(this, false);
    playButton->addMouseListener(this, false);

    playCCSlider->addMouseListener(this, true);
    playCCSlider->setSliderSnapsToMousePosition(false);
    playCCSlider->setDoubleClickReturnValue(true, -1);

    recCCSlider->addMouseListener(this, true);
    recCCSlider->setSliderSnapsToMousePosition(false);
    recCCSlider->setDoubleClickReturnValue(true, -1);

    minTriggerNoteSlider->setDoubleClickReturnValue(true, 0);
    minTriggerNoteSlider->setSliderSnapsToMousePosition(false);
    minTriggerNoteSlider->addMouseListener(this, true);

    maxTriggerNoteSlider->setDoubleClickReturnValue(true, 127);
    maxTriggerNoteSlider->setSliderSnapsToMousePosition(false);
    maxTriggerNoteSlider->addMouseListener(this, true);

    rootNoteSlider->setDoubleClickReturnValue(true, 60);
    rootNoteSlider->setSliderSnapsToMousePosition(false);
    rootNoteSlider->addMouseListener(this, true);

    resizer->setMouseClickGrabsKeyboardFocus(false);

    timeline->addChangeListener(this);
    pianoRollViewport->addChangeListener(this);

    midiOutDeviceComboBox->setMouseClickGrabsKeyboardFocus(false);
    midiOutDeviceComboBox->addItem(juce::String("--"), 1);
    for (int i = 0; i < ownerFilter->devices.size(); i++)
    {
        midiOutDeviceComboBox->addItem(ownerFilter->devices[i].name, i + 2);
    }
    midiOutDeviceComboBox->setSelectedId(1);

    loopInfoLabel2->setText("Stopped", juce::dontSendNotification);

    resizeLimits.setSizeLimits(385, 248, 1600, 900);
    ownerFilter->addChangeListener(this);
    ownerFilter->info->addChangeListener(this);
    pianoRoll = (PianoRoll*) pianoRollViewport->getViewedComponent();
    pianoRollViewport->setPlayBar(pianoRoll->getPlayBar());
    pianoRoll->setSize(500, 1280);
    pianoRoll->setSequence(ownerFilter->getActiveLoop());
    pianoRoll->addChangeListener(this);
    keyboard = (juce::MidiKeyboardComponent*) keyboardViewport->getViewedComponent();
    keyboard->setScrollButtonsVisible(false);
    keyboard->setBounds(0, 0, 25, pianoRoll->getHeight());
    keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
    keyboard->setOctaveForMiddleC(ownerFilter->bottomOctave + 5);
    lastActiveLoop = ownerFilter->getCurrentProgram();
    //viewport->setViewPosition(0,pianoRoll->getHeight()/2-viewport->getHeight()/2);
    //ownerFilter->setPRSetting("x",viewport->getViewPositionX());
    //ownerFilter->setPRSetting("y",viewport->getViewPositionY());
    //ownerFilter->setPRSetting("width",pianoRoll->getWidth());
    //ownerFilter->setPRSetting("height",pianoRoll->getHeight());
    timeline->setPianoRoll(pianoRoll);
    noSnap = true;
    patternNameLabel->setListener(this);
    keySelector->setKeyWidth(22.f);
    keySelector->setAvailableRange(0, 11);
    keySelector->addChangeListener(this);
    ownerFilter->keySelectorState.addListener(this);
    startTimer(75);

    setSize(ownerFilter->lastUIWidth, ownerFilter->lastUIHeight);
    updateParametersFromFilter();
}

PizLooperEditor::~PizLooperEditor()
{
    stopTimer();
    getFilter()->dispatchPendingMessages();
    getFilter()->info->dispatchPendingMessages();
    getFilter()->removeChangeListener(this);
    getFilter()->info->removeChangeListener(this);
    getFilter()->keySelectorState.removeListener(this);

    zoomLabel                 = nullptr;
    timeline                  = nullptr;
    textButton1               = nullptr;
    textButton2               = nullptr;
    textButton3               = nullptr;
    textButton4               = nullptr;
    textButton5               = nullptr;
    textButton6               = nullptr;
    textButton7               = nullptr;
    textButton8               = nullptr;
    textButton9               = nullptr;
    textButton10              = nullptr;
    textButton11              = nullptr;
    textButton12              = nullptr;
    textButton13              = nullptr;
    textButton14              = nullptr;
    textButton15              = nullptr;
    textButton16              = nullptr;
    playButton                = nullptr;
    recordButton              = nullptr;
    overdubButton             = nullptr;
    midiThruButton            = nullptr;
    clearButton               = nullptr;
    stepSizeComboBox          = nullptr;
    transposeSlider           = nullptr;
    octaveSlider              = nullptr;
    velocitySlider            = nullptr;
    transposeLabel            = nullptr;
    octaveLabel               = nullptr;
    velocityLabel             = nullptr;
    loopStartSlider           = nullptr;
    startOffsetLabel          = nullptr;
    loopEndOffsetSlider       = nullptr;
    loopEndOffsetLabel        = nullptr;
    stretchSlider             = nullptr;
    speedLabel                = nullptr;
    loopModeComboBox          = nullptr;
    noteTriggerComboBox       = nullptr;
    syncModeComboBox          = nullptr;
    rootNoteSlider            = nullptr;
    rootNoteLabel             = nullptr;
    minTriggerNoteSlider      = nullptr;
    minTriggerNoteLabel       = nullptr;
    maxTriggerNoteSlider      = nullptr;
    maxTriggerNoteLabel       = nullptr;
    triggerChannelSlider      = nullptr;
    triggerChannelLabel       = nullptr;
    reloadButton              = nullptr;
    quantizeComboBox          = nullptr;
    loopStepSizeLabel         = nullptr;
    shiftPatternSlider        = nullptr;
    shiftPatternLabel         = nullptr;
    quantizeInputLabel        = nullptr;
    patternNameLabel          = nullptr;
    saveButton                = nullptr;
    hostSyncModeLabel         = nullptr;
    noteTriggeringLabel       = nullptr;
    loopInfoLabel             = nullptr;
    loopInfoLabel2            = nullptr;
    loopManipulationLabel     = nullptr;
    slotIOChannelSlider       = nullptr;
    slotIOChannelLabel        = nullptr;
    recordLengthLabel         = nullptr;
    fixedLengthSlider         = nullptr;
    transformFilterButton     = nullptr;
    pianoRollViewport         = nullptr;
    resizer                   = nullptr;
    noteToggleButton          = nullptr;
    playGroupSlider           = nullptr;
    playGroupLabel            = nullptr;
    muteGroupSlider           = nullptr;
    muteGroupLabel            = nullptr;
    snapButton                = nullptr;
    quantize2ComboBox         = nullptr;
    forceToKeyButton          = nullptr;
    keySelector               = nullptr;
    shiftUpButton             = nullptr;
    shiftDownButton           = nullptr;
    singleLoopToggleButton    = nullptr;
    masterVelocitySlider      = nullptr;
    masterVelocityLabel       = nullptr;
    aboutButton               = nullptr;
    tripletButton             = nullptr;
    dottedButton              = nullptr;
    zoomOutButton             = nullptr;
    zoomInButton              = nullptr;
    numeratorLabel            = nullptr;
    denominatorLabel          = nullptr;
    useScaleChannelButton     = nullptr;
    scaleChannelSlider        = nullptr;
    scaleChannelLabel         = nullptr;
    masterTransposeSlider     = nullptr;
    masterTransposeLabel      = nullptr;
    waitForBarButton          = nullptr;
    midiOutDeviceComboBox     = nullptr;
    midiOutDeviceLabel        = nullptr;
    useTransposeChannelButton = nullptr;
    immediateTransposeButton  = nullptr;
    numLoopsSlider            = nullptr;
    nextSlotSlider            = nullptr;
    loopSettingsLabel         = nullptr;
    forceModeComboBox         = nullptr;
    keyboardViewport          = nullptr;
    removeBarButton           = nullptr;
    addBarButton              = nullptr;
    patternLengthLabel        = nullptr;
    textButton17              = nullptr;
    textButton18              = nullptr;
    textButton19              = nullptr;
    textButton20              = nullptr;
    textButton21              = nullptr;
    textButton22              = nullptr;
    textButton23              = nullptr;
    textButton24              = nullptr;
    textButton25              = nullptr;
    textButton26              = nullptr;
    textButton27              = nullptr;
    textButton28              = nullptr;
    textButton29              = nullptr;
    textButton30              = nullptr;
    textButton31              = nullptr;
    textButton32              = nullptr;
    textButton33              = nullptr;
    textButton34              = nullptr;
    textButton35              = nullptr;
    textButton36              = nullptr;
    textButton37              = nullptr;
    textButton38              = nullptr;
    textButton39              = nullptr;
    textButton40              = nullptr;
    textButton41              = nullptr;
    textButton42              = nullptr;
    textButton43              = nullptr;
    textButton44              = nullptr;
    textButton45              = nullptr;
    textButton46              = nullptr;
    textButton47              = nullptr;
    textButton48              = nullptr;
    textButton49              = nullptr;
    textButton50              = nullptr;
    textButton51              = nullptr;
    textButton52              = nullptr;
    textButton53              = nullptr;
    textButton54              = nullptr;
    textButton55              = nullptr;
    textButton56              = nullptr;
    textButton57              = nullptr;
    textButton58              = nullptr;
    textButton59              = nullptr;
    textButton60              = nullptr;
    textButton61              = nullptr;
    textButton62              = nullptr;
    textButton63              = nullptr;
    textButton64              = nullptr;
    textButton65              = nullptr;
    textButton66              = nullptr;
    textButton67              = nullptr;
    textButton68              = nullptr;
    textButton69              = nullptr;
    textButton70              = nullptr;
    textButton71              = nullptr;
    textButton72              = nullptr;
    textButton73              = nullptr;
    textButton74              = nullptr;
    textButton75              = nullptr;
    textButton76              = nullptr;
    textButton77              = nullptr;
    textButton78              = nullptr;
    textButton79              = nullptr;
    textButton80              = nullptr;
    textButton81              = nullptr;
    textButton82              = nullptr;
    textButton83              = nullptr;
    textButton84              = nullptr;
    textButton85              = nullptr;
    textButton86              = nullptr;
    textButton87              = nullptr;
    textButton88              = nullptr;
    textButton89              = nullptr;
    textButton90              = nullptr;
    textButton91              = nullptr;
    textButton92              = nullptr;
    textButton93              = nullptr;
    textButton94              = nullptr;
    textButton95              = nullptr;
    textButton96              = nullptr;
    textButton97              = nullptr;
    textButton98              = nullptr;
    textButton99              = nullptr;
    textButton100             = nullptr;
    textButton101             = nullptr;
    textButton102             = nullptr;
    textButton103             = nullptr;
    textButton104             = nullptr;
    textButton105             = nullptr;
    textButton106             = nullptr;
    textButton107             = nullptr;
    textButton108             = nullptr;
    textButton109             = nullptr;
    textButton110             = nullptr;
    textButton111             = nullptr;
    textButton112             = nullptr;
    textButton113             = nullptr;
    textButton114             = nullptr;
    textButton115             = nullptr;
    textButton116             = nullptr;
    textButton117             = nullptr;
    textButton118             = nullptr;
    textButton119             = nullptr;
    textButton120             = nullptr;
    textButton121             = nullptr;
    textButton122             = nullptr;
    textButton123             = nullptr;
    textButton124             = nullptr;
    textButton125             = nullptr;
    textButton126             = nullptr;
    textButton127             = nullptr;
    textButton128             = nullptr;
    transpose10Button         = nullptr;
    keepLengthToggleButton    = nullptr;
    recCCSlider               = nullptr;
    playCCSlider              = nullptr;
    velocitySensSlider        = nullptr;
    velocitySenSlider         = nullptr;
    monitorButton             = nullptr;
    transposeChannelSlider    = nullptr;
    transposeChannelLabel     = nullptr;
}

//==============================================================================
void PizLooperEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff202029));

    {
        int x = 1, y = 393, width = 303, height = 78;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               176.0f - 1.0f + x,
                                               352.0f - 393.0f + y,
                                               fillColour2,
                                               176.0f - 1.0f + x,
                                               443.0f - 393.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        int x = 305, y = 393, width = 81, height = 78;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff162938);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               190.0f - 305.0f + x,
                                               369.0f - 393.0f + y,
                                               fillColour2,
                                               190.0f - 305.0f + x,
                                               419.0f - 393.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        float x = 390.0f, y = 62.0f, width = static_cast<float>(getWidth() - 391), height = 40.0f;
        auto fillColour = juce::Colour(0xffa0a0a0);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        int x = 152, y = -1, width = getWidth() - 151, height = 56;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);
        auto strokeColour = juce::Colour(0xff46495f);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               160.0f - 152.0f + x,
                                               static_cast<float>(-30) - static_cast<float>(-1) + y,
                                               fillColour2,
                                               161.0f - 152.0f + x,
                                               43.0f - static_cast<float>(-1) + y,
                                               false));
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    {
        int x = 0, y = 0, width = 152, height = 55;
        auto fillColour = juce::Colour(0xff202029);

        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
    }

    {
        float x = 6.0f, y = 317.0f, width = 136.0f, height = 90.0f;
        auto fillColour = juce::Colour(0xff404049);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
    }

    {
        int x = 1, y = 246, width = 385, height = 147;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               159.0f - 1.0f + x,
                                               215.0f - 246.0f + y,
                                               fillColour2,
                                               159.0f - 1.0f + x,
                                               307.0f - 246.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        float x = 6.0f, y = 159.0f, width = 136.0f, height = 104.0f;
        auto fillColour = juce::Colour(0xff404049);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
    }

    {
        int x = 1, y = 109, width = 385, height = 140;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 1.0f + x,
                                               89.0f - 109.0f + y,
                                               fillColour2,
                                               100.0f - 1.0f + x,
                                               165.0f - 109.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        float x = 6.0f, y = static_cast<float>(-20), width = 139.0f, height = 67.0f;
        auto fillColour1 = juce::Colour(0xff002700), fillColour2 = juce::Colour(0xba267387);
        auto strokeColour = juce::Colour(0xffcbcbcb);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               95.0f - 6.0f + x,
                                               static_cast<float>(-25) - static_cast<float>(-20) + y,
                                               fillColour2,
                                               95.0f - 6.0f + x,
                                               107.0f - static_cast<float>(-20) + y,
                                               false));
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
        g.setColour(strokeColour);
        g.drawRoundedRectangle(x, y, width, height, 14.500f, 1.000f);
    }

    {
        int x = 12, y = 12, width = 41, height = 25;

        g.setColour(juce::Colours::black);
        g.drawImage(cachedImage_piznew40_png_1,
                    x,
                    y,
                    width,
                    height,
                    0,
                    0,
                    cachedImage_piznew40_png_1.getWidth(),
                    cachedImage_piznew40_png_1.getHeight());
    }

    {
        int x = 44, y = 23, width = 108, height = 20;
        juce::String text("midiLooper");
        auto fillColour = juce::Colour(0xffcbcbcb);

        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 46, y = 8, width = 108, height = 20;
        juce::String text("Insert Piz Here->");
        auto fillColour = juce::Colour(0xffbfbfbf);

        g.setColour(fillColour);
        g.setFont(juce::Font(10.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 9, y = 202, width = 369, height = 42;
        auto fillColour1 = juce::Colour(0xff433b22), fillColour2 = juce::Colour(0xff8b7a47);
        juce::Colour strokeColour = juce::Colours::black;

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 9.0f + x,
                                               194.0f - 202.0f + y,
                                               fillColour2,
                                               100.0f - 9.0f + x,
                                               214.0f - 202.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    {
        int x = 1, y = 85, width = 385, height = 32;
        auto fillColour1 = juce::Colour(0xff979797), fillColour2 = juce::Colour(0xff6f737a);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 1.0f + x,
                                               85.0f - 85.0f + y,
                                               fillColour2,
                                               100.0f - 1.0f + x,
                                               98.0f - 85.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        int x = 390, y = 85, width = getWidth() - 391, height = 20;
        auto fillColour = juce::Colour(0xff8c8c8c);

        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
    }

    {
        int x = 215, y = 177, width = 24, height = 15;
        juce::String text("->");
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 153, y = 162, width = 24, height = 15;
        juce::String text("PLAY");
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.setFont(juce::Font(10.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 230, y = 162, width = 29, height = 15;
        juce::String text("THEN");
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.setFont(juce::Font(10.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        float x = 6.0f, y = 458.0f, width = 136.0f, height = 27.0f;
        auto fillColour = juce::Colour(0xff404049);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
    }

    {
        int x = 574, y = 67, width = 20, height = 12;
        juce::String text("/");
        juce::Colour fillColour = juce::Colours::black;

        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        float x = 141.0f, y = 254.0f, width = 161.0f, height = 72.0f;
        auto fillColour = juce::Colour(0x86404049);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        float x = 8.0f, y = 311.0f, width = 371.0f, height = 36.0f;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff434450);
        auto strokeColour = juce::Colour(0xff111111);

        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               256.0f - 8.0f + x,
                                               265.0f - 311.0f + y,
                                               fillColour2,
                                               256.0f - 8.0f + x,
                                               355.0f - 311.0f + y,
                                               false));
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
        g.setColour(strokeColour);
        g.drawRoundedRectangle(x, y, width, height, 10.000f, 0.700f);
    }

    {
        int x = 723, y = 64, width = 51, height = 18;
        auto fillColour           = juce::Colour(0xffbababa);
        juce::Colour strokeColour = juce::Colours::black;

        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    if (getFilter()->getParameterForSlot(kUseTrChannel, getFilter()->getCurrentProgram()) >= 0.5f)
    {
        g.setColour(juce::Colour(0xff5b5b5b));
        g.strokePath(internalPath1, juce::PathStrokeType(5.0000f));
    }
    if (getFilter()->getParameterForSlot(kUseScaleChannel, getFilter()->getCurrentProgram()) >= 0.5f)
    {
        g.setColour(juce::Colour(0xff5b5b5b));
        g.strokePath(internalPath2, juce::PathStrokeType(5.0000f));
    }
}

void PizLooperEditor::resized()
{
    timeline->setBounds(415, 85, getWidth() - 430, 20);
    pianoRollViewport->setBounds(415, 105, getWidth() - 415, getHeight() - 105);
    resizer->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
    keyboardViewport->setBounds(390, 105, 25, getHeight() - 121);

    internalPath1.clear();
    internalPath1.startNewSubPath(141.0f, 294.0f);
    internalPath1.lineTo(136.0f, 294.0f);

    internalPath2.clear();
    internalPath2.startNewSubPath(29.0f, 310.0f);
    internalPath2.lineTo(29.0f, 301.0f);

    pianoRoll->setVisible(getWidth() > 390);
    getFilter()->setSize(getWidth(), getHeight());
}

void PizLooperEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    int index = getButtonIndex(buttonThatWasClicked);
    if (index > -1)
    {
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            getFilter()->toggleSlotPlaying(index);
        }
        else
        {
            getFilter()->setActiveSlot(index);
            buttonThatWasClicked->setToggleState(true, juce::dontSendNotification);
        }
    }
}

void PizLooperEditor::buttonStateChanged(juce::Button* button)
{
}

void PizLooperEditor::mouseDown(const juce::MouseEvent& e)
{
    int index = getButtonIndex(e.eventComponent);
    if (e.mods.isPopupMenu() && index != -1)
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kPlay, i, index == i ? 1.f : 0.f);
        }
        ((juce::Button*) e.eventComponent)->setToggleState(true, juce::dontSendNotification);
    }
}

void PizLooperEditor::mouseDrag(const juce::MouseEvent& e)
{
}

void PizLooperEditor::mouseUp(const juce::MouseEvent& e)
{
    Component* p = e.eventComponent->getParentComponent();
    if (e.eventComponent == playButton.get())
    {
        if (e.mods.isPopupMenu())
        {
            playCCSlider->setVisible(! playCCSlider->isVisible());
        }
        else if (e.mods.isMiddleButtonDown())
        {
            getFilter()->setParameter(kPlayCC, 0.f);
        }
        else if (e.mods.isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kPlay, i, playButton->getToggleState() ? 0.f : 1.f);
            }
        }
        else
        {
            getFilter()->toggleSlotPlaying(getFilter()->getCurrentProgram());
        }
    }
    else if (e.eventComponent == recordButton.get())
    {
        if (e.mods.isPopupMenu())
        {
            recCCSlider->setVisible(! recCCSlider->isVisible());
        }
        else if (e.mods.isMiddleButtonDown())
        {
            getFilter()->setParameter(kRecCC, 0.f);
        }
        else if (e.mods.isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kRecord, i, recordButton->getToggleState() ? 0.f : 1.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kRecord) >= 0.5)
            {
                getFilter()->notifyHostForActiveSlot(kRecord, 0.0);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kRecord, 1.0);
            }
        }
    }
    else if (p == rootNoteSlider.get() || p == maxTriggerNoteSlider.get() || p == minTriggerNoteSlider.get())
    {
        auto* slider = (juce::Slider*) p;
        if (e.mods.isMiddleButtonDown())
        {
            //middle-click midi learn
            slider->setValue(-1, juce::sendNotification);
        }
        else if (e.mods.isPopupMenu())
        {
            if (slider->isDoubleClickReturnEnabled())
            {
                double value = slider->getDoubleClickReturnValue();
                slider->setValue(value, juce::sendNotification);
            }
        }
    }
    else if (p == playCCSlider.get() || p == recCCSlider.get())
    {
        auto* slider = (juce::Slider*) p;
        if (e.mods.isMiddleButtonDown())
        {
            //middle-click midi learn
            slider->setValue(-2, juce::sendNotification);
        }
        else if (e.mods.isPopupMenu())
        {
            if (slider->isDoubleClickReturnEnabled())
            {
                double value = slider->getDoubleClickReturnValue();
                slider->setValue(value, juce::sendNotification);
            }
        }
    }
    else if (p == stretchSlider.get() || p == transposeSlider.get() || p == octaveSlider.get() || p == loopStartSlider.get() || p == loopEndOffsetSlider.get()
             || p == shiftPatternSlider.get() || p == velocitySlider.get() || p == velocitySensSlider.get() || p == masterVelocitySlider.get() || p == masterTransposeSlider.get()
             || p == playGroupSlider.get() || p == muteGroupSlider.get() || p == slotIOChannelSlider.get() || p == numLoopsSlider.get() || p == nextSlotSlider.get())
    {
        if (e.mods.isPopupMenu())
        {
            auto* slider = (juce::Slider*) p;
            if (slider->isDoubleClickReturnEnabled())
            {
                double value = slider->getDoubleClickReturnValue();
                slider->setValue(value, juce::sendNotification);
            }
        }
    }
}

void PizLooperEditor::filesDropped(const juce::StringArray& filenames, int mouseX, int mouseY)
{
    if (juce::File(filenames[0]).hasFileExtension("mid"))
    {
        getFilter()->loadMidiFile(juce::File(filenames[0]));
    }
}

bool PizLooperEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    juce::File file = juce::File(files[0]);
    if (file.hasFileExtension("mid"))
    {
        return true;
    }
    if (file.getFileName() == "midiLooperKey.txt")
    {
        return true;
    }
    return false;
}

void PizLooperEditor::timerCallback()
{
    pianoRoll->setPlayTime(960.0 * getFilter()->getPlayPosition(pianoRoll->playing, pianoRoll->recording));
}

void PizLooperEditor::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (source == &(getFilter()->keySelectorState))
    {
        getFilter()->notifyHostForActiveSlot(kNote0 + midiNoteNumber, 1.f);
        keySelector->repaint();
    }
}

void PizLooperEditor::handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (source == &(getFilter()->keySelectorState))
    {
        getFilter()->notifyHostForActiveSlot(kNote0 + midiNoteNumber, 0.f);
        keySelector->repaint();
    }
}

//==============================================================================
void PizLooperEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == getFilter() || getFilter()->newLoop)
    {
        updateParametersFromFilter();
    }
    else if (source == getFilter()->info)
    {
        loopInfoLabel2->setText(getFilter()->info->s, juce::sendNotification);
    }
    else if (source == keySelector.get())
    {
        for (int note = 0; note < 12; note++)
        {
            if (getFilter()->keySelectorState.isNoteOn(1, note) != getFilter()->getParamForActiveSlot(kNote0 + note) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kNote0 + note, getFilter()->keySelectorState.isNoteOn(keySelector->getMidiChannel(), note) ? 1.f : 0.f);
            }
        }
    }
    else if (source == timeline.get() || source == pianoRoll)
    {
        getFilter()->setLoopLength(lastActiveLoop, timeline->getLength());
        getFilter()->setLoopStart(lastActiveLoop, timeline->getStart());
        getFilter()->updateLoopInfo();
        if (getFilter()->currentNumEvents == 0)
        {
            loopInfoLabel->setText("No Loop", juce::dontSendNotification);
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff979797));
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff555555));
        }
        else
        {
            juce::String loopInfo = "Loop length: ";
            if (getFilter()->currentLength == 1.0)
            {
                loopInfo << "1 Beat (";
            }
            else
            {
                loopInfo << getFilter()->currentLength << " Beats (";
            }
            loopInfo << getFilter()->currentNumEvents << " Events)";
            loopInfoLabel->setText(loopInfo, juce::dontSendNotification);
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        }
    }
    else if (source == pianoRollViewport.get())
    {
        getFilter()->setPRSetting("x", pianoRollViewport->getViewPositionX(), false);
        getFilter()->setPRSetting("y", pianoRollViewport->getViewPositionY(), false);
    }
}

//==============================================================================
void PizLooperEditor::updateControls(int param, float value, bool forCurProgram)
{
    if (! forCurProgram)
    {
        if (param == kPlay)
        {
            updateSlotButtons();
        }
        return;
    }
    switch (param)
    {
        case kThru:
            midiThruButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            monitorButton->setEnabled(midiThruButton->getToggleState());
            break;
        case kMonitor:
            monitorButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kSync:
            syncModeComboBox->setText(getFilter()->getCurrentSlotParameterText(kSync), juce::dontSendNotification);
            break;
        case kRecStep:
            stepSizeComboBox->setText(getFilter()->getCurrentSlotParameterText(kRecStep), juce::dontSendNotification);
            break;
        case kQuantize:
            quantizeComboBox->setText(getFilter()->getCurrentSlotParameterText(kQuantize), juce::dontSendNotification);
            break;
        case kFixedLength:
            fixedLengthSlider->setVSTSlider(value);
            break;
        case kRecMode:
            if (value >= 0.8f)
            {
                overdubButton->setColour(juce::TextButton::buttonOnColourId, getLookAndFeel().findColour(juce::TextButton::buttonOnColourId));
                keepLengthToggleButton->setToggleState(false, juce::dontSendNotification);
            }
            else
            {
                overdubButton->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
                keepLengthToggleButton->setToggleState(true, juce::dontSendNotification);
            }
            overdubButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            keepLengthToggleButton->setVisible(value >= 0.5f);
            break;
        case kSingleLoop:
            singleLoopToggleButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kMasterVelocity:
            masterVelocitySlider->setVSTSlider(value);
            break;
        case kMasterTranspose:
            masterTransposeSlider->setVSTSlider(value);
            break;
        case kImmediateTranspose:
            immediateTransposeButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kRecCC:
            recCCSlider->setVSTSlider(value);
            break;
        case kPlayCC:
            playCCSlider->setVSTSlider(value);
            break;

        case kRecord:
            recordButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kPlay:
            playButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            playButton->setButtonText(value >= 0.5f ? "STOP" : "PLAY");
            updateSlotButtons();
            break;
        case kTranspose:
            transposeSlider->setIndex(lastActiveLoop * numParamsPerSlot + kTranspose);
            transposeSlider->setVSTSlider(value);
            break;
        case kOctave:
            octaveSlider->setIndex(lastActiveLoop * numParamsPerSlot + kOctave);
            octaveSlider->setVSTSlider(value);
            break;
        case kVelocity:
            velocitySlider->setIndex(lastActiveLoop * numParamsPerSlot + kVelocity);
            velocitySlider->setVSTSlider(value);
            break;
        case kVeloSens:
            velocitySensSlider->setIndex(lastActiveLoop * numParamsPerSlot + kVeloSens);
            velocitySensSlider->setVSTSlider(value);
            break;
        case kShift:
            shiftPatternSlider->setIndex(lastActiveLoop * numParamsPerSlot + kShift);
            shiftPatternSlider->setVSTSlider(value);
            break;
        case kLoopStart:
            loopStartSlider->setIndex(lastActiveLoop * numParamsPerSlot + kLoopStart);
            loopStartSlider->setVSTSlider(value);
            break;
        case kLoopEnd:
            loopEndOffsetSlider->setIndex(lastActiveLoop * numParamsPerSlot + kLoopEnd);
            loopEndOffsetSlider->setVSTSlider(value);
            break;
        case kStretch:
            stretchSlider->setIndex(lastActiveLoop * numParamsPerSlot + kStretch);
            stretchSlider->setVSTSlider(value);
            break;
        case kTrigger:
            loopModeComboBox->setText(getFilter()->getCurrentSlotParameterText(kTrigger), juce::dontSendNotification);
            break;
        case kNoteTrig:
            noteTriggerComboBox->setText(getFilter()->getCurrentSlotParameterText(kNoteTrig), juce::dontSendNotification);
            break;
        case kRoot:
            rootNoteSlider->setIndex(lastActiveLoop * numParamsPerSlot + kRoot);
            rootNoteSlider->setVSTSlider(value);
            break;
        case kNLow:
            minTriggerNoteSlider->setIndex(lastActiveLoop * numParamsPerSlot + kNLow);
            minTriggerNoteSlider->setVSTSlider(value);
            break;
        case kNHigh:
            maxTriggerNoteSlider->setIndex(lastActiveLoop * numParamsPerSlot + kNHigh);
            maxTriggerNoteSlider->setVSTSlider(value);
            break;
        case kChannel:
            slotIOChannelSlider->setIndex(lastActiveLoop * numParamsPerSlot + kChannel);
            slotIOChannelSlider->setVSTSlider(value);
            pianoRoll->defaultChannel = jmax(0, roundToInt(value * 16.f) - 1);
            keyboard->setMidiChannel(jmax(1, roundToInt(value * 16.f)));
            break;
        case kTrigChan:
            triggerChannelSlider->setIndex(lastActiveLoop * numParamsPerSlot + kTrigChan);
            triggerChannelSlider->setVSTSlider(value);
            break;
        case kFiltChan:
            if (value < 0.5f)
            {
                transformFilterButton->setButtonText("Transform");
            }
            else
            {
                transformFilterButton->setButtonText("Filter");
            }
            break;
        case kWaitForBar:
            waitForBarButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kNumLoops:
            numLoopsSlider->setIndex(lastActiveLoop * numParamsPerSlot + kNumLoops);
            numLoopsSlider->setVSTSlider(value);
            break;
        case kNextSlot:
            nextSlotSlider->setIndex(lastActiveLoop * numParamsPerSlot + kNextSlot);
            nextSlotSlider->setVSTSlider(value);
            break;
        case kPlayGroup:
            playGroupSlider->setIndex(lastActiveLoop * numParamsPerSlot + kPlayGroup);
            playGroupSlider->setVSTSlider(value);
            break;
        case kMuteGroup:
            muteGroupSlider->setIndex(lastActiveLoop * numParamsPerSlot + kMuteGroup);
            muteGroupSlider->setVSTSlider(value);
            break;
        case kForceToKey:
            forceToKeyButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kScaleChannel:
            scaleChannelSlider->setIndex(lastActiveLoop * numParamsPerSlot + kScaleChannel);
            scaleChannelSlider->setVSTSlider(value);
            break;
        case kTransposeChannel:
            transposeChannelSlider->setIndex(lastActiveLoop * numParamsPerSlot + kTransposeChannel);
            transposeChannelSlider->setVSTSlider(value);
            break;
        case kUseScaleChannel:
            useScaleChannelButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kUseTrChannel:
            useTransposeChannelButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kNoteToggle:
            noteToggleButton->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kForceToScaleMode:
            forceModeComboBox->setText(getFilter()->getCurrentSlotParameterText(kForceToScaleMode), juce::dontSendNotification);
            break;
        case kTranspose10:
            transpose10Button->setToggleState(value >= 0.5f, juce::dontSendNotification);
        default:
            break;
    }
}

void PizLooperEditor::updateSlotButtons()
{
    PizLooper* const filter = getFilter();
    for (int i = 0; i < numSlots; i++)
    {
        juce::TextButton* b = getButtonForSlot(i);
        if (filter->isSlotPlaying(i))
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff17e617));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff17e617));
        }
        else
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffd3d3d3));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff979797));
        }
        if (filter->isLoopEmpty(i))
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff979797));
            b->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff555555));
        }
        else
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            b->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        }
        b->setButtonText(juce::String(i + 1));
    }
}

void PizLooperEditor::updateParametersFromFilter()
{
    PizLooper* const filter = getFilter();

    float param[numParamsPerSlot + numGlobalParams];
    bool slotPlayState[numSlots];

    filter->getCallbackLock().enter();

    for (int i = 0; i < numParamsPerSlot + numGlobalParams; i++)
    {
        param[i] = filter->getParamForActiveSlot(i);
    }
    lastActiveLoop = filter->getCurrentProgram();
    for (int i = 0; i < numSlots; i++)
    {
        slotPlayState[i] = filter->isSlotPlaying(i);
    }

    const int w = filter->lastUIWidth;
    const int h = filter->lastUIHeight;

    const int n = filter->getNumerator(lastActiveLoop);
    const int d = filter->getDenominator(lastActiveLoop);

    const int newDevice = filter->devices.indexOf(filter->getActiveDevice());
    bool newLoop        = filter->newLoop;

    filter->getCallbackLock().exit();

    // and after releasing the lock, we're free to do the time-consuming UI stuff
    {
        snapButton->setToggleState(filter->getPRSetting("snap"), juce::dontSendNotification);
        pianoRoll->setSnap(snapButton->getToggleState());
        float q = filter->getPRSetting("stepsize");
        if (q == 0.0)
        {
            quantize2ComboBox->setText("4th", juce::dontSendNotification);
        }
        else if (q < 0.3)
        {
            quantize2ComboBox->setText("8th", juce::dontSendNotification);
        }
        else if (q < 0.6)
        {
            quantize2ComboBox->setText("16th", juce::dontSendNotification);
        }
        else if (q < 0.9)
        {
            quantize2ComboBox->setText("32nd", juce::dontSendNotification);
        }
        else
        {
            quantize2ComboBox->setText("64th", juce::dontSendNotification);
        }
        dottedButton->setToggleState(filter->getPRSetting("dotted"), juce::dontSendNotification);
        tripletButton->setToggleState(filter->getPRSetting("triplet"), juce::dontSendNotification);
        float tord = (filter->getPRSetting("triplet")) ? 1.5f : 1.f;
        if (filter->getPRSetting("dotted"))
        {
            tord = 0.666666667f;
        }
        if (q == 0.0)
        {
            pianoRoll->setNoteLength(1 * tord);
        }
        else if (q < 0.3)
        {
            pianoRoll->setNoteLength(2 * tord);
        }
        else if (q < 0.6)
        {
            pianoRoll->setNoteLength(4 * tord);
        }
        else if (q < 0.9)
        {
            pianoRoll->setNoteLength(8 * tord);
        }
        else
        {
            pianoRoll->setNoteLength(16 * tord);
        }
        pianoRoll->setDisplayLength((int) filter->getPRSetting("bars"));

        auto newHeight = (int) (128 * roundToInt((float) filter->getPRSetting("height") / 128.0f));
        pianoRoll->setSize(filter->getPRSetting("width"), newHeight);
        keyboard->setSize(25, pianoRoll->getHeight());
        keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
        pianoRollViewport->setViewPosition(filter->getPRSetting("x"),
                                           filter->getPRSetting("y"));
    }
    pianoRoll->setTimeSig(n, d);
    numeratorLabel->setText(juce::String(n), juce::dontSendNotification);
    denominatorLabel->setText(juce::String(d), juce::dontSendNotification);

    midiOutDeviceComboBox->setSelectedItemIndex(newDevice + 1, juce::dontSendNotification);

    for (int i = 0; i < numParamsPerSlot + numGlobalParams; i++)
    {
        updateControls(i, param[i], true);
    }

    //loop selector buttons
    for (int i = 0; i < numSlots; i++)
    {
        juce::TextButton* b = getButtonForSlot(i);
        if (slotPlayState[i])
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff17e617));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff17e617));
        }
        else
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffd3d3d3));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff979797));
        }
        if (filter->isLoopEmpty(i))
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff979797));
            b->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff555555));
        }
        else
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            b->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        }
        b->setButtonText(juce::String(i + 1));
    }
    getButtonForSlot(lastActiveLoop)->setToggleState(true, juce::dontSendNotification);

    patternNameLabel->setText(filter->getProgramName(lastActiveLoop), juce::dontSendNotification);

    if (filter->currentNumEvents == 0)
    {
        loopInfoLabel->setText("No Loop", juce::dontSendNotification);
    }
    else
    {
        juce::String loopInfo = "Loop length: ";
        if (filter->currentLength == 1.0)
        {
            loopInfo << "1 Beat (";
        }
        else
        {
            loopInfo << filter->currentLength << " Beats (";
        }
        loopInfo << filter->currentNumEvents << " Events)";
        loopInfoLabel->setText(loopInfo, juce::dontSendNotification);
    }
    noSnap = true;
    timeline->setLoop(filter->getLoopStart(lastActiveLoop), filter->getLoopLength(lastActiveLoop));
    noSnap = false;

    loopInfoLabel2->setText(filter->info->s, juce::dontSendNotification);

    if (newLoop)
    {
        pianoRoll->setSequence(filter->getActiveLoop());
        filter->newLoop = false;
    }
    patternLengthLabel->setText(juce::String(pianoRoll->getDisplayLength()), juce::dontSendNotification);
    setSize(w, h);
}

juce::TextButton* PizLooperEditor::getButtonForSlot(int slot)
{
    switch (slot)
    {
        case 0:
            return textButton1.get();
        case 1:
            return textButton2.get();
        case 2:
            return textButton3.get();
        case 3:
            return textButton4.get();
        case 4:
            return textButton5.get();
        case 5:
            return textButton6.get();
        case 6:
            return textButton7.get();
        case 7:
            return textButton8.get();
        case 8:
            return textButton9.get();
        case 9:
            return textButton10.get();
        case 10:
            return textButton11.get();
        case 11:
            return textButton12.get();
        case 12:
            return textButton13.get();
        case 13:
            return textButton14.get();
        case 14:
            return textButton15.get();
        case 15:
            return textButton16.get();
        case 16:
            return textButton17.get();
        case 17:
            return textButton18.get();
        case 18:
            return textButton19.get();
        case 19:
            return textButton20.get();
        case 20:
            return textButton21.get();
        case 21:
            return textButton22.get();
        case 22:
            return textButton23.get();
        case 23:
            return textButton24.get();
        case 24:
            return textButton25.get();
        case 25:
            return textButton26.get();
        case 26:
            return textButton27.get();
        case 27:
            return textButton28.get();
        case 28:
            return textButton29.get();
        case 29:
            return textButton30.get();
        case 30:
            return textButton31.get();
        case 31:
            return textButton32.get();
        case 32:
            return textButton33.get();
        case 33:
            return textButton34.get();
        case 34:
            return textButton35.get();
        case 35:
            return textButton36.get();
        case 36:
            return textButton37.get();
        case 37:
            return textButton38.get();
        case 38:
            return textButton39.get();
        case 39:
            return textButton40.get();
        case 40:
            return textButton41.get();
        case 41:
            return textButton42.get();
        case 42:
            return textButton43.get();
        case 43:
            return textButton44.get();
        case 44:
            return textButton45.get();
        case 45:
            return textButton46.get();
        case 46:
            return textButton47.get();
        case 47:
            return textButton48.get();
        case 48:
            return textButton49.get();
        case 49:
            return textButton50.get();
        case 50:
            return textButton51.get();
        case 51:
            return textButton52.get();
        case 52:
            return textButton53.get();
        case 53:
            return textButton54.get();
        case 54:
            return textButton55.get();
        case 55:
            return textButton56.get();
        case 56:
            return textButton57.get();
        case 57:
            return textButton58.get();
        case 58:
            return textButton59.get();
        case 59:
            return textButton60.get();
        case 60:
            return textButton61.get();
        case 61:
            return textButton62.get();
        case 62:
            return textButton63.get();
        case 63:
            return textButton64.get();
        case 64:
            return textButton65.get();
        case 65:
            return textButton66.get();
        case 66:
            return textButton67.get();
        case 67:
            return textButton68.get();
        case 68:
            return textButton69.get();
        case 69:
            return textButton70.get();
        case 70:
            return textButton71.get();
        case 71:
            return textButton72.get();
        case 72:
            return textButton73.get();
        case 73:
            return textButton74.get();
        case 74:
            return textButton75.get();
        case 75:
            return textButton76.get();
        case 76:
            return textButton77.get();
        case 77:
            return textButton78.get();
        case 78:
            return textButton79.get();
        case 79:
            return textButton80.get();
        case 80:
            return textButton81.get();
        case 81:
            return textButton82.get();
        case 82:
            return textButton83.get();
        case 83:
            return textButton84.get();
        case 84:
            return textButton85.get();
        case 85:
            return textButton86.get();
        case 86:
            return textButton87.get();
        case 87:
            return textButton88.get();
        case 88:
            return textButton89.get();
        case 89:
            return textButton90.get();
        case 90:
            return textButton91.get();
        case 91:
            return textButton92.get();
        case 92:
            return textButton93.get();
        case 93:
            return textButton94.get();
        case 94:
            return textButton95.get();
        case 95:
            return textButton96.get();
        case 96:
            return textButton97.get();
        case 97:
            return textButton98.get();
        case 98:
            return textButton99.get();
        case 99:
            return textButton100.get();
        case 100:
            return textButton101.get();
        case 101:
            return textButton102.get();
        case 102:
            return textButton103.get();
        case 103:
            return textButton104.get();
        case 104:
            return textButton105.get();
        case 105:
            return textButton106.get();
        case 106:
            return textButton107.get();
        case 107:
            return textButton108.get();
        case 108:
            return textButton109.get();
        case 109:
            return textButton110.get();
        case 110:
            return textButton111.get();
        case 111:
            return textButton112.get();
        case 112:
            return textButton113.get();
        case 113:
            return textButton114.get();
        case 114:
            return textButton115.get();
        case 115:
            return textButton116.get();
        case 116:
            return textButton117.get();
        case 117:
            return textButton118.get();
        case 118:
            return textButton119.get();
        case 119:
            return textButton120.get();
        case 120:
            return textButton121.get();
        case 121:
            return textButton122.get();
        case 122:
            return textButton123.get();
        case 123:
            return textButton124.get();
        case 124:
            return textButton125.get();
        case 125:
            return textButton126.get();
        case 126:
            return textButton127.get();
        case 127:
            return textButton128.get();
        default:
            return nullptr;
    }
}

int PizLooperEditor::getButtonIndex(Component* button)
{
    if (button == textButton1.get())
    {
        return 0;
    }
    if (button == textButton2.get())
    {
        return 1;
    }
    if (button == textButton3.get())
    {
        return 2;
    }
    if (button == textButton4.get())
    {
        return 3;
    }
    if (button == textButton5.get())
    {
        return 4;
    }
    if (button == textButton6.get())
    {
        return 5;
    }
    if (button == textButton7.get())
    {
        return 6;
    }
    if (button == textButton8.get())
    {
        return 7;
    }
    if (button == textButton9.get())
    {
        return 8;
    }
    if (button == textButton10.get())
    {
        return 9;
    }
    if (button == textButton11.get())
    {
        return 10;
    }
    if (button == textButton12.get())
    {
        return 11;
    }
    if (button == textButton13.get())
    {
        return 12;
    }
    if (button == textButton14.get())
    {
        return 13;
    }
    if (button == textButton15.get())
    {
        return 14;
    }
    if (button == textButton16.get())
    {
        return 15;
    }
    if (button == textButton17.get())
    {
        return 16;
    }
    if (button == textButton18.get())
    {
        return 17;
    }
    if (button == textButton19.get())
    {
        return 18;
    }
    if (button == textButton20.get())
    {
        return 19;
    }
    if (button == textButton21.get())
    {
        return 20;
    }
    if (button == textButton22.get())
    {
        return 21;
    }
    if (button == textButton23.get())
    {
        return 22;
    }
    if (button == textButton24.get())
    {
        return 23;
    }
    if (button == textButton25.get())
    {
        return 24;
    }
    if (button == textButton26.get())
    {
        return 25;
    }
    if (button == textButton27.get())
    {
        return 26;
    }
    if (button == textButton28.get())
    {
        return 27;
    }
    if (button == textButton29.get())
    {
        return 28;
    }
    if (button == textButton30.get())
    {
        return 29;
    }
    if (button == textButton31.get())
    {
        return 30;
    }
    if (button == textButton32.get())
    {
        return 31;
    }
    if (button == textButton33.get())
    {
        return 32;
    }
    if (button == textButton34.get())
    {
        return 33;
    }
    if (button == textButton35.get())
    {
        return 34;
    }
    if (button == textButton36.get())
    {
        return 35;
    }
    if (button == textButton37.get())
    {
        return 36;
    }
    if (button == textButton38.get())
    {
        return 37;
    }
    if (button == textButton39.get())
    {
        return 38;
    }
    if (button == textButton40.get())
    {
        return 39;
    }
    if (button == textButton41.get())
    {
        return 40;
    }
    if (button == textButton42.get())
    {
        return 41;
    }
    if (button == textButton43.get())
    {
        return 42;
    }
    if (button == textButton44.get())
    {
        return 43;
    }
    if (button == textButton45.get())
    {
        return 44;
    }
    if (button == textButton46.get())
    {
        return 45;
    }
    if (button == textButton47.get())
    {
        return 46;
    }
    if (button == textButton48.get())
    {
        return 47;
    }
    if (button == textButton49.get())
    {
        return 48;
    }
    if (button == textButton50.get())
    {
        return 49;
    }
    if (button == textButton51.get())
    {
        return 50;
    }
    if (button == textButton52.get())
    {
        return 51;
    }
    if (button == textButton53.get())
    {
        return 52;
    }
    if (button == textButton54.get())
    {
        return 53;
    }
    if (button == textButton55.get())
    {
        return 54;
    }
    if (button == textButton56.get())
    {
        return 55;
    }
    if (button == textButton57.get())
    {
        return 56;
    }
    if (button == textButton58.get())
    {
        return 57;
    }
    if (button == textButton59.get())
    {
        return 58;
    }
    if (button == textButton60.get())
    {
        return 59;
    }
    if (button == textButton61.get())
    {
        return 60;
    }
    if (button == textButton62.get())
    {
        return 61;
    }
    if (button == textButton63.get())
    {
        return 62;
    }
    if (button == textButton64.get())
    {
        return 63;
    }
    if (button == textButton65.get())
    {
        return 64;
    }
    if (button == textButton66.get())
    {
        return 65;
    }
    if (button == textButton67.get())
    {
        return 66;
    }
    if (button == textButton68.get())
    {
        return 67;
    }
    if (button == textButton69.get())
    {
        return 68;
    }
    if (button == textButton70.get())
    {
        return 69;
    }
    if (button == textButton71.get())
    {
        return 70;
    }
    if (button == textButton72.get())
    {
        return 71;
    }
    if (button == textButton73.get())
    {
        return 72;
    }
    if (button == textButton74.get())
    {
        return 73;
    }
    if (button == textButton75.get())
    {
        return 74;
    }
    if (button == textButton76.get())
    {
        return 75;
    }
    if (button == textButton77.get())
    {
        return 76;
    }
    if (button == textButton78.get())
    {
        return 77;
    }
    if (button == textButton79.get())
    {
        return 78;
    }
    if (button == textButton80.get())
    {
        return 79;
    }
    if (button == textButton81.get())
    {
        return 80;
    }
    if (button == textButton82.get())
    {
        return 81;
    }
    if (button == textButton83.get())
    {
        return 82;
    }
    if (button == textButton84.get())
    {
        return 83;
    }
    if (button == textButton85.get())
    {
        return 84;
    }
    if (button == textButton86.get())
    {
        return 85;
    }
    if (button == textButton87.get())
    {
        return 86;
    }
    if (button == textButton88.get())
    {
        return 87;
    }
    if (button == textButton89.get())
    {
        return 88;
    }
    if (button == textButton90.get())
    {
        return 89;
    }
    if (button == textButton91.get())
    {
        return 90;
    }
    if (button == textButton92.get())
    {
        return 91;
    }
    if (button == textButton93.get())
    {
        return 92;
    }
    if (button == textButton94.get())
    {
        return 93;
    }
    if (button == textButton95.get())
    {
        return 94;
    }
    if (button == textButton96.get())
    {
        return 95;
    }
    if (button == textButton97.get())
    {
        return 96;
    }
    if (button == textButton98.get())
    {
        return 97;
    }
    if (button == textButton99.get())
    {
        return 98;
    }
    if (button == textButton100.get())
    {
        return 99;
    }
    if (button == textButton101.get())
    {
        return 100;
    }
    if (button == textButton102.get())
    {
        return 101;
    }
    if (button == textButton103.get())
    {
        return 102;
    }
    if (button == textButton104.get())
    {
        return 103;
    }
    if (button == textButton105.get())
    {
        return 104;
    }
    if (button == textButton106.get())
    {
        return 105;
    }
    if (button == textButton107.get())
    {
        return 106;
    }
    if (button == textButton108.get())
    {
        return 107;
    }
    if (button == textButton109.get())
    {
        return 108;
    }
    if (button == textButton110.get())
    {
        return 109;
    }
    if (button == textButton111.get())
    {
        return 110;
    }
    if (button == textButton112.get())
    {
        return 111;
    }
    if (button == textButton113.get())
    {
        return 112;
    }
    if (button == textButton114.get())
    {
        return 113;
    }
    if (button == textButton115.get())
    {
        return 114;
    }
    if (button == textButton116.get())
    {
        return 115;
    }
    if (button == textButton117.get())
    {
        return 116;
    }
    if (button == textButton118.get())
    {
        return 117;
    }
    if (button == textButton119.get())
    {
        return 118;
    }
    if (button == textButton120.get())
    {
        return 119;
    }
    if (button == textButton121.get())
    {
        return 120;
    }
    if (button == textButton122.get())
    {
        return 121;
    }
    if (button == textButton123.get())
    {
        return 122;
    }
    if (button == textButton124.get())
    {
        return 123;
    }
    if (button == textButton125.get())
    {
        return 124;
    }
    if (button == textButton126.get())
    {
        return 125;
    }
    if (button == textButton127.get())
    {
        return 126;
    }
    if (button == textButton128.get())
    {
        return 127;
    }
    return -1;
}

// JUCER_RESOURCE: piznew40_png, 1803, "../../../My Pictures/piznew40.png"
static const unsigned char resource_PizLooperEditor_piznew40_png[] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 40, 0, 0, 0, 24, 8, 6, 0, 0, 1, 255, 131, 143, 86, 0, 0, 0, 9, 112, 72, 89, 115, 0, 0, 46, 35, 0, 0, 46, 35, 1, 120, 165, 63, 118, 0, 0, 0, 13, 116, 69, 88, 116, 67, 111, 109, 109, 101, 110, 116, 0, 104, 195, 123, 3, 0, 168, 254, 86, 169, 0, 0, 6, 164, 73, 68, 65, 84, 120, 156, 181, 87, 9, 84, 205, 105, 20, 255, 215, 123, 175, 69, 74, 90, 212, 107, 85, 74, 76, 26, 137, 142, 112, 44, 169, 168, 145, 37, 154, 49, 24, 145, 61, 164, 178, 150, 38, 106, 210, 12, 141, 148, 16, 102, 70, 165, 33, 45, 104, 179, 205, 88, 143, 194, 88, 178, 85, 42, 107, 171, 147, 97, 178, 68, 141, 234, 185, 115, 239, 247, 122, 233, 165, 158, 154, 49, 223, 57, 247, 252, 255, 255, 239, 187, 223, 125, 247, 254, 238, 250, 56, 78, 198, 50, 46, 187, 186, 7, 158, 220, 78, 0, 124, 183, 102, 59, 187, 54, 121, 137, 254, 126, 116, 16, 158, 23, 38, 130, 185, 137, 94, 14, 103, 209, 75, 63, 181, 229, 21, 171, 62, 198, 39, 217, 203, 153, 212, 13, 34, 226, 34, 110, 201, 161, 160, 232, 124, 12, 187, 138, 239, 158, 146, 205, 37, 65, 190, 83, 33, 35, 46, 144, 54, 5, 180, 145, 182, 39, 128, 49, 109, 94, 231, 9, 163, 135, 89, 85, 69, 133, 204, 107, 150, 192, 225, 71, 253, 211, 252, 95, 25, 131, 132, 94, 223, 75, 126, 207, 32, 39, 39, 183, 6, 42, 51, 222, 111, 112, 156, 214, 189, 11, 187, 0, 247, 67, 154, 190, 93, 107, 31, 164, 178, 115, 99, 89, 198, 211, 10, 243, 159, 41, 226, 174, 30, 143, 0, 168, 202, 2, 117, 53, 149, 159, 36, 7, 175, 239, 167, 52, 91, 38, 42, 79, 135, 105, 147, 134, 23, 52, 223, 10, 240, 118, 175, 190, 122, 44, 2, 136, 233, 69, 209, 1, 98, 130, 33, 3, 45, 178, 62, 16, 255, 234, 110, 146, 148, 33, 77, 208, 104, 75, 49, 233, 233, 104, 164, 244, 54, 213, 99, 82, 36, 248, 229, 159, 221, 14, 229, 185, 177, 239, 141, 12, 93, 61, 163, 14, 31, 250, 45, 238, 221, 122, 116, 249, 231, 102, 134, 132, 173, 190, 240, 174, 9, 21, 7, 130, 163, 246, 33, 131, 96, 169, 12, 227, 205, 23, 205, 116, 174, 28, 239, 100, 91, 136, 239, 30, 173, 206, 150, 92, 202, 10, 151, 168, 43, 181, 44, 209, 208, 27, 215, 127, 139, 132, 210, 43, 191, 208, 161, 29, 109, 234, 106, 171, 103, 19, 90, 254, 75, 167, 64, 13, 58, 139, 16, 173, 198, 203, 10, 10, 252, 92, 248, 243, 40, 12, 232, 103, 122, 183, 93, 53, 188, 60, 92, 158, 183, 6, 169, 37, 85, 223, 73, 132, 191, 10, 246, 193, 171, 226, 36, 48, 212, 211, 138, 147, 97, 81, 243, 154, 129, 102, 193, 53, 212, 34, 255, 204, 54, 120, 89, 252, 161, 23, 200, 109, 67, 7, 245, 41, 105, 243, 182, 130, 128, 159, 17, 187, 101, 25, 16, 142, 244, 171, 104, 86, 157, 128, 207, 91, 142, 71, 93, 145, 158, 52, 101, 12, 212, 61, 20, 39, 202, 219, 146, 67, 244, 61, 75, 173, 171, 114, 88, 90, 236, 90, 104, 40, 75, 131, 83, 201, 223, 209, 158, 191, 102, 119, 213, 13, 205, 130, 117, 180, 213, 55, 119, 68, 253, 118, 150, 254, 245, 223, 35, 49, 206, 185, 0, 177, 48, 45, 117, 194, 98, 194, 127, 16, 200, 21, 98, 188, 225, 99, 0, 103, 109, 105, 114, 167, 14, 61, 136, 33, 81, 131, 27, 42, 237, 93, 208, 237, 209, 61, 115, 205, 146, 201, 85, 6, 122, 90, 137, 156, 116, 36, 11, 28, 134, 247, 175, 120, 131, 41, 177, 59, 124, 49, 112, 211, 221, 70, 214, 44, 242, 112, 110, 164, 48, 161, 144, 112, 119, 29, 86, 133, 76, 163, 90, 10, 227, 201, 203, 7, 195, 227, 76, 134, 33, 57, 137, 242, 73, 32, 224, 71, 224, 209, 196, 188, 211, 209, 12, 223, 207, 63, 235, 121, 19, 195, 231, 15, 41, 45, 208, 25, 235, 191, 15, 152, 217, 72, 133, 97, 222, 116, 39, 73, 144, 26, 220, 60, 25, 5, 83, 198, 13, 5, 178, 228, 64, 204, 74, 76, 163, 109, 144, 135, 17, 16, 23, 185, 172, 254, 100, 18, 115, 200, 152, 118, 177, 192, 60, 204, 74, 217, 189, 186, 225, 98, 102, 248, 187, 188, 51, 209, 172, 234, 72, 66, 133, 200, 115, 170, 3, 172, 243, 155, 202, 52, 69, 246, 209, 31, 195, 86, 183, 17, 195, 64, 86, 96, 83, 117, 200, 78, 251, 1, 42, 175, 199, 125, 144, 102, 109, 173, 190, 245, 165, 135, 160, 224, 220, 118, 153, 66, 95, 20, 37, 130, 211, 8, 235, 199, 29, 17, 200, 161, 57, 111, 57, 10, 100, 196, 235, 214, 233, 173, 237, 10, 229, 196, 129, 47, 115, 77, 152, 49, 121, 100, 101, 66, 180, 95, 131, 190, 174, 38, 236, 141, 242, 5, 143, 47, 237, 153, 137, 207, 48, 119, 155, 43, 57, 122, 24, 147, 32, 43, 33, 218, 23, 126, 12, 242, 36, 193, 46, 82, 82, 130, 87, 76, 3, 239, 57, 227, 68, 84, 225, 207, 29, 12, 131, 254, 150, 38, 23, 56, 113, 156, 133, 142, 115, 28, 68, 149, 132, 121, 86, 226, 24, 140, 69, 134, 29, 254, 216, 83, 74, 203, 134, 178, 195, 48, 106, 168, 85, 137, 178, 146, 194, 106, 220, 118, 224, 236, 108, 44, 242, 41, 6, 23, 124, 51, 246, 73, 59, 154, 47, 38, 33, 164, 169, 247, 28, 215, 70, 252, 150, 107, 113, 198, 51, 51, 17, 94, 162, 51, 10, 236, 240, 111, 103, 139, 216, 174, 188, 188, 220, 2, 124, 76, 234, 16, 200, 109, 172, 73, 206, 118, 207, 176, 245, 212, 183, 220, 179, 178, 177, 234, 245, 32, 247, 196, 22, 102, 26, 150, 166, 130, 246, 46, 119, 98, 169, 35, 105, 34, 41, 118, 226, 142, 145, 155, 139, 221, 67, 106, 135, 199, 247, 175, 7, 52, 116, 19, 165, 233, 33, 250, 32, 159, 212, 62, 72, 97, 209, 66, 216, 199, 71, 249, 128, 173, 181, 121, 30, 94, 242, 70, 18, 118, 84, 41, 115, 19, 225, 237, 91, 167, 182, 50, 95, 81, 233, 164, 110, 93, 118, 109, 15, 96, 242, 81, 232, 150, 33, 143, 15, 146, 124, 139, 59, 125, 133, 61, 186, 159, 194, 217, 132, 185, 169, 161, 244, 48, 208, 136, 208, 88, 158, 198, 178, 91, 194, 52, 140, 207, 147, 95, 136, 207, 193, 72, 99, 12, 132, 154, 153, 62, 243, 198, 215, 230, 100, 108, 100, 141, 156, 234, 76, 113, 206, 78, 140, 186, 217, 48, 120, 64, 239, 92, 129, 128, 183, 12, 249, 186, 180, 82, 206, 108, 226, 216, 193, 213, 245, 248, 3, 235, 151, 127, 13, 145, 193, 115, 225, 13, 26, 76, 30, 161, 39, 213, 49, 138, 96, 250, 97, 191, 249, 19, 26, 144, 127, 149, 135, 187, 125, 45, 117, 39, 81, 69, 58, 60, 190, 17, 15, 115, 167, 57, 82, 74, 81, 137, 86, 224, 196, 3, 143, 60, 215, 129, 69, 76, 212, 75, 147, 76, 141, 117, 43, 13, 132, 90, 21, 170, 42, 202, 231, 21, 21, 248, 43, 187, 170, 40, 173, 212, 23, 106, 198, 186, 58, 218, 86, 230, 164, 111, 100, 104, 137, 219, 226, 126, 70, 146, 6, 132, 19, 2, 24, 234, 107, 1, 14, 108, 80, 148, 29, 3, 47, 209, 67, 230, 166, 122, 231, 93, 157, 108, 171, 16, 136, 58, 37, 69, 193, 14, 148, 175, 211, 65, 47, 73, 43, 71, 57, 75, 19, 15, 89, 94, 135, 147, 5, 101, 3, 33, 65, 53, 146, 80, 105, 171, 197, 74, 136, 13, 4, 120, 215, 197, 222, 6, 209, 113, 130, 17, 131, 45, 161, 6, 221, 72, 149, 202, 198, 202, 52, 253, 223, 40, 212, 122, 245, 221, 17, 182, 240, 173, 100, 110, 173, 185, 155, 204, 132, 203, 42, 147, 237, 149, 78, 138, 235, 215, 247, 147, 89, 235, 161, 57, 21, 99, 127, 241, 167, 80, 176, 219, 200, 33, 253, 74, 73, 65, 106, 67, 136, 38, 67, 132, 144, 33, 101, 137, 58, 163, 40, 41, 151, 25, 31, 8, 60, 158, 124, 208, 167, 80, 142, 150, 6, 146, 13, 210, 81, 36, 140, 35, 109, 246, 183, 160, 252, 90, 44, 24, 225, 251, 228, 47, 134, 48, 183, 139, 209, 77, 146, 137, 46, 197, 98, 116, 232, 124, 202, 74, 63, 45, 13, 181, 221, 152, 185, 123, 241, 221, 25, 137, 255, 49, 37, 148, 56, 113, 131, 118, 66, 178, 55, 49, 210, 201, 142, 12, 153, 11, 37, 88, 189, 223, 97, 118, 81, 77, 34, 4, 119, 110, 244, 162, 41, 225, 44, 206, 130, 151, 41, 216, 73, 225, 64, 159, 175, 240, 252, 8, 36, 198, 172, 0, 109, 77, 53, 56, 157, 18, 202, 98, 180, 165, 98, 47, 139, 15, 48, 194, 56, 172, 198, 59, 102, 38, 134, 58, 169, 148, 64, 20, 211, 84, 138, 42, 112, 222, 94, 229, 229, 38, 82, 82, 84, 216, 197, 181, 85, 206, 122, 26, 244, 0, 26, 152, 224, 233, 81, 160, 18, 65, 136, 156, 197, 30, 101, 55, 208, 162, 16, 93, 225, 143, 44, 150, 156, 116, 251, 144, 44, 67, 164, 16, 179, 158, 194, 18, 62, 143, 199, 20, 62, 145, 24, 204, 100, 16, 146, 228, 126, 66, 109, 83, 224, 44, 250, 23, 179, 161, 213, 93, 154, 249, 23, 244, 179, 48, 186, 72, 231, 84, 39, 169, 14, 147, 65, 71, 18, 130, 224, 202, 177, 205, 12, 32, 231, 81, 54, 229, 146, 11, 202, 72, 110, 254, 75, 221, 177, 72, 238, 3, 103, 123, 155, 251, 29, 129, 190, 213, 34, 25, 174, 26, 234, 170, 59, 177, 192, 103, 161, 11, 227, 241, 219, 177, 131, 119, 201, 139, 17, 164, 28, 33, 75, 255, 67, 176, 247, 190, 194, 57, 120, 71, 39, 117, 248, 223, 215, 218, 110, 170, 93, 104, 176, 220, 194, 53, 53, 130, 127, 0, 217, 46, 21, 116, 181, 241, 235, 130, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130, 0, 0 };

const char* PizLooperEditor::piznew40_png   = (const char*) resource_PizLooperEditor_piznew40_png;
const int PizLooperEditor::piznew40_pngSize = 1803;

void PizLooperEditor::clickableLabelMouseDown(ClickableLabel* label, const juce::MouseEvent& e)
{
}

void PizLooperEditor::clickableLabelMouseDoubleClick(ClickableLabel* label, const juce::MouseEvent& e)
{
    if (label == patternNameLabel.get())
    {
        label->edit();
    }
}

void PizLooperEditor::handleMonitorButtonClick() const
{
    if (getFilter()->getParamForActiveSlot(kMonitor) >= 0.5f)
    {
        getFilter()->setParameter(kMonitor, 0.0f);
    }
    else
    {
        getFilter()->setParameter(kMonitor, 1.0f);
    }
}

void PizLooperEditor::handleKeepLengthButtonClick() const
{
    if (getFilter()->getParamForActiveSlot(kRecMode) >= 0.8f)
    {
        getFilter()->notifyHostForActiveSlot(kRecMode, 0.7f);
    }
    else if (getFilter()->getParamForActiveSlot(kRecMode) >= 0.5f)
    {
        getFilter()->notifyHostForActiveSlot(kRecMode, 1.0f);
    }
}

void PizLooperEditor::handleTranspose10ButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kTranspose10, i, transpose10Button->getToggleState() ? 0.f : 1.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kTranspose10) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kTranspose10, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kTranspose10, 1.0f);
        }
    }
}

void PizLooperEditor::handleAddBarButtonClick()
{
    pianoRoll->addBar();
    patternLengthLabel->setText(juce::String(pianoRoll->getDisplayLength()), juce::dontSendNotification);
    getFilter()->setPRSetting("bars", pianoRoll->getDisplayLength(), false);
    getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
}

void PizLooperEditor::handleRemoveBarButtonClick()
{
    pianoRoll->removeBar();
    patternLengthLabel->setText(juce::String(pianoRoll->getDisplayLength()), juce::dontSendNotification);
    getFilter()->setPRSetting("bars", pianoRoll->getDisplayLength(), false);
    getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
}

void PizLooperEditor::handleImmediateTransposeButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kImmediateTranspose, i, immediateTransposeButton->getToggleState() ? 1.f : 0.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kImmediateTranspose) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kImmediateTranspose, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kImmediateTranspose, 1.0f);
        }
    }
}

void PizLooperEditor::handleUseTrChannelButtonClick()
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kUseTrChannel, i, useTransposeChannelButton->getToggleState() ? 1.f : 0.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kUseTrChannel) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kUseTrChannel, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kUseTrChannel, 1.0f);
        }
    }
    repaint(112, 284, 34, 20);
}

void PizLooperEditor::handleWaitForBarButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kWaitForBar, i, waitForBarButton->getToggleState() ? 1.f : 0.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kWaitForBar) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kWaitForBar, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kWaitForBar, 1.0f);
        }
    }
}

void PizLooperEditor::handleUseScaleChannelButtonClick()
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kUseScaleChannel, i, useScaleChannelButton->getToggleState() ? 1.f : 0.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kUseScaleChannel) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kUseScaleChannel, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kUseScaleChannel, 1.0f);
        }
    }
    repaint(24, 290, 22, 23);
}

void PizLooperEditor::handleZoomInButtonClick()
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        zoomYViewport(false);
    }
    else
    {
        zoomXViewport(false);
    }
}

void PizLooperEditor::handleZoomOutButtonClick()
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        zoomYViewport(true);
    }
    else
    {
        zoomXViewport(true);
    }
}

void PizLooperEditor::zoomXViewport(bool out)
{
    double y = (double) pianoRollViewport->getViewPositionY() / ((double) pianoRoll->getHeight() - pianoRollViewport->getHeight());
    double x = (double) pianoRollViewport->getViewPositionX() / ((double) pianoRoll->getWidth() - pianoRollViewport->getWidth());

    auto newWidth = out
                      ? jmax(100, (int) (pianoRoll->getWidth() * 0.75))
                      : (int) (pianoRoll->getWidth() * 1.33333333333);

    pianoRoll->setSize(newWidth, pianoRoll->getHeight());
    pianoRollViewport->setViewPositionProportionately(x, y);
    getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
    getFilter()->setPRSetting("x", pianoRollViewport->getViewPositionX(), false);
}

void PizLooperEditor::zoomYViewport(bool out)
{
    double y = (double) pianoRollViewport->getViewPositionY() / ((double) pianoRoll->getHeight() - pianoRollViewport->getHeight());
    double x = (double) pianoRollViewport->getViewPositionX() / ((double) pianoRoll->getWidth() - pianoRollViewport->getWidth());

    auto newHeight = out
                       ? jmax(128 * 3, (int) (128 * roundToInt((pianoRoll->getHeight() * 0.75) / 128.0f)))
                       : jmin(25600, (int) (128 * roundToInt((pianoRoll->getHeight() * 1.33333333333) / 128.0f)));
    pianoRoll->setSize(pianoRoll->getWidth(), newHeight);
    keyboard->setSize(25, pianoRoll->getHeight());
    keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
    pianoRollViewport->setViewPositionProportionately(x, y);
    getFilter()->setPRSetting("height", pianoRoll->getHeight(), false);
    getFilter()->setPRSetting("y", pianoRollViewport->getViewPositionY(), false);
}

void PizLooperEditor::handleMouseWheelMove(const juce::MouseWheelDetails& details)
{
    if (details.deltaY > 0)
    {
        handleMouseWheelMove(false, -10);
    }
    else if (details.deltaY < 0)
    {
        handleMouseWheelMove(true, 10);
    }
}

void PizLooperEditor::handleMouseWheelMove(bool out, int positionIncrement)
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown() && juce::ModifierKeys::getCurrentModifiers().isAltDown())
    {
        zoomYViewport(out);
    }
    else if (juce::ModifierKeys::getCurrentModifiers().isAltDown())
    {
        pianoRollViewport->setViewPosition(
            pianoRollViewport->getViewPositionX() + positionIncrement,
            pianoRollViewport->getViewPositionY());
    }
    else if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        zoomXViewport(out);
    }
    else
    {
        pianoRollViewport->setViewPosition(
            pianoRollViewport->getViewPositionX(),
            pianoRollViewport->getViewPositionY() + positionIncrement);
    }
}

void PizLooperEditor::handleDottedButtonClick()
{
    dottedButton->setToggleState(! getFilter()->getPRSetting("dotted"), juce::dontSendNotification);
    getFilter()->setPRSetting("dotted", dottedButton->getToggleState());
    pianoRoll->repaintBG();
}

void PizLooperEditor::handleTripletButtonClick()
{
    tripletButton->setToggleState(! getFilter()->getPRSetting("triplet"), juce::dontSendNotification);
    getFilter()->setPRSetting("triplet", tripletButton->getToggleState());
    pianoRoll->repaintBG();
}

void PizLooperEditor::handleAboutButtonClick()
{
    juce::URL("https://github.com/sleiner/pizmidi").launchInDefaultBrowser();
}

void PizLooperEditor::handleSingleLoopButtonClick() const
{
    if (getFilter()->getParamForActiveSlot(kSingleLoop) >= 0.5f)
    {
        getFilter()->notifyHostForActiveSlot(kSingleLoop, 0.0f);
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kSingleLoop, 1.0f);
    }
}

void PizLooperEditor::handleShiftDownButtonClick() const
{
    bool lastNotes[12];
    for (int i = 0; i < 12; i++)
    {
        lastNotes[i] = getFilter()->keySelectorState.isNoteOn(1, i);
    }
    getFilter()->keySelectorState.reset();
    if (lastNotes[0])
    {
        getFilter()->keySelectorState.noteOn(1, 11, 1.f);
    }
    for (int n = 1; n < 12; n++)
    {
        if (lastNotes[n])
        {
            getFilter()->keySelectorState.noteOn(1, n - 1, 1.f);
        }
    }
    //if (ModifierKeys::getCurrentModifiers().isCommandDown())
    //{
    //	for (int i=0;i<numSlots;i++) {
    //		getFilter()->notifyHost(kNote11,i,lastNotes[0]?1.f:0.f);
    //		for (int n=0;n<11;n++)
    //			getFilter()->notifyHost(kNote0+n-1,i,lastNotes[n]?1.f:0.f);
    //	}
    //}
    //else {
    getFilter()->notifyHostForActiveSlot(kNote11, lastNotes[0] ? 1.f : 0.f);
    for (int n = 0; n < 11; n++)
    {
        getFilter()->notifyHostForActiveSlot(kNote0 + n - 1, lastNotes[n] ? 1.f : 0.f);
    }
    //}
}

void PizLooperEditor::handleShiftUpButtonClick() const
{
    bool lastNotes[12];
    for (int i = 0; i < 12; i++)
    {
        lastNotes[i] = getFilter()->keySelectorState.isNoteOn(1, i);
    }
    getFilter()->keySelectorState.reset();
    if (lastNotes[11])
    {
        getFilter()->keySelectorState.noteOn(1, 0, 1.f);
    }
    for (int n = 0; n < 11; n++)
    {
        if (lastNotes[n])
        {
            getFilter()->keySelectorState.noteOn(1, n + 1, 1.f);
        }
    }
    //if (ModifierKeys::getCurrentModifiers().isCommandDown())
    //{
    //	for (int i=0;i<numSlots;i++) {
    //		getFilter()->notifyHost(kNote0,i,lastNotes[11] ? 1.f : 0.f);
    //		for (int n=0;n<11;n++) {
    //			getFilter()->notifyHost(kNote0+n+1,i,lastNotes[i]?1.f:0.f);
    //		}
    //	}
    //}
    //else {
    getFilter()->notifyHostForActiveSlot(kNote0, lastNotes[11] ? 1.f : 0.f);
    for (int n = 0; n < 11; n++)
    {
        getFilter()->notifyHostForActiveSlot(kNote0 + n + 1, lastNotes[n] ? 1.f : 0.f);
    }
    //}
}

void PizLooperEditor::handleForceToKeyButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kForceToKey, i, forceToKeyButton->getToggleState() ? 1.f : 0.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kForceToKey) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kForceToKey, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kForceToKey, 1.0f);
        }
    }
}

void PizLooperEditor::handleSnapButtonClick()
{
    snapButton->setToggleState(! getFilter()->getPRSetting("snap"), juce::dontSendNotification);
    getFilter()->setPRSetting("snap", snapButton->getToggleState());
    pianoRoll->repaintBG();
}

void PizLooperEditor::handleNoteToggleButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kNoteToggle, i, noteToggleButton->getToggleState() ? 0.f : 1.f);
        }
    }
    else
    {
        if (getFilter()->getParamForActiveSlot(kNoteToggle) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kNoteToggle, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kNoteToggle, 1.0f);
        }
    }
}

void PizLooperEditor::handleTransformFilterButtonClick() const
{
    if (getFilter()->getParamForActiveSlot(kFiltChan) >= 0.5f)
    {
        getFilter()->notifyHostForActiveSlot(kFiltChan, 0.0f);
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kFiltChan, 1.0f);
    }
}

void PizLooperEditor::handleSaveButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
    {
        //save with filename incremented
        getFilter()->setParameter(kSave, 1.0f);
    }
    else if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        getFilter()->setParameter(kSave, 0.9f);
    }
    else
    {
        juce::FileChooser myChooser("Save MIDI File...",
                                    juce::File(getFilter()->loopDir + juce::File::getSeparatorString() + patternNameLabel->getText()),
                                    "*.mid");

        if (myChooser.browseForFileToSave(true))
        {
            juce::File midiFile(myChooser.getResult());
            if (! midiFile.hasFileExtension("mid"))
            {
                midiFile = midiFile.withFileExtension("mid");
            }

            getFilter()->writeMidiFile(lastActiveLoop, midiFile);
        }
    }
}

void PizLooperEditor::handleReloadButtonClick() const
{
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        getFilter()->setParameter(kFile, 1.0f);
    }
    else
    {
        juce::FileChooser myChooser("Load MIDI File...",
                                    juce::File(getFilter()->loopDir),
                                    "*.mid");

        if (myChooser.browseForFileToOpen())
        {
            juce::File midiFile(myChooser.getResult());
            getFilter()->loadMidiFile(midiFile);
        }
    }
}

void PizLooperEditor::handleClearButtonClick() const
{
    getFilter()->setParameter(kFile, 0.0f);
}

void PizLooperEditor::handleThruButtonClick() const
{
    if (getFilter()->getParamForActiveSlot(kThru) >= 0.5f)
    {
        getFilter()->setParameter(kThru, 0.0f);
    }
    else
    {
        getFilter()->setParameter(kThru, 1.0f);
    }
}

void PizLooperEditor::handleOverdubButtonClick() const
{
    if (getFilter()->getParamForActiveSlot(kRecMode) >= 0.5f)
    {
        getFilter()->setParameter(kRecMode, 0.0f);
    }
    else
    {
        if (juce::ModifierKeys::getCurrentModifiers().isAltDown())
        {
            getFilter()->setParameter(kRecMode, 1.0f);
        }
        else
        {
            getFilter()->setParameter(kRecMode, 0.7f);
        }
    }
}

void PizLooperEditor::handleForceModeComboBoxChange() const
{
    auto selection = forceModeComboBox->getText();

    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            if (selection == "Nearest")
            {
                getFilter()->notifyHost(kForceToScaleMode, i, 0.f);
            }
            else if (selection == "Up")
            {
                getFilter()->notifyHost(kForceToScaleMode, i, 0.33f);
            }
            else if (selection == "Down")
            {
                getFilter()->notifyHost(kForceToScaleMode, i, 0.67f);
            }
            else if (selection == "Block")
            {
                getFilter()->notifyHost(kForceToScaleMode, i, 1.f);
            }
        }
    }
    else
    {
        if (selection == "Nearest")
        {
            getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 0.f);
        }
        else if (selection == "Up")
        {
            getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 0.33f);
        }
        else if (selection == "Down")
        {
            getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 0.67f);
        }
        else if (selection == "Block")
        {
            getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 1.f);
        }
    }
}

void PizLooperEditor::handleMidiOutDeviceComboBoxChange() const
{
    if (midiOutDeviceComboBox->getSelectedItemIndex() == 0)
    {
        getFilter()->setParameter(kMidiOutDevice, 0.0f);
        getFilter()->setActiveDevice(midiOutDeviceComboBox->getText());
    }
    else
    {
        getFilter()->setActiveDevice(midiOutDeviceComboBox->getText());
        getFilter()->setParameter(kMidiOutDevice, 1.0f);
        //getFilter()->setParameter(0,float(comboBox->getSelectedItemIndex()-1)/float(getFilter()->devices.size()-1)+0.00001f);
    }
}

void PizLooperEditor::handleQuantize2ComboBoxChange()
{
    auto selection = quantize2ComboBox->getText();
    if (selection == "4th")
    {
        getFilter()->setPRSetting("stepsize", 0.f);
    }
    else if (selection == "8th")
    {
        getFilter()->setPRSetting("stepsize", 0.25f);
    }
    else if (selection == "16th")
    {
        getFilter()->setPRSetting("stepsize", 0.55f);
    }
    else if (selection == "32nd")
    {
        getFilter()->setPRSetting("stepsize", 0.75f);
    }
    else if (selection == "64th")
    {
        getFilter()->setPRSetting("stepsize", 1.f);
    }
    pianoRoll->repaintBG();
}

void PizLooperEditor::handleQuantizeComboBoxChange() const
{
    auto selection = quantizeComboBox->getText();
    if (selection == "Off")
    {
        getFilter()->notifyHostForActiveSlot(kQuantize, 0.f);
    }
    else if (selection == "8th")
    {
        getFilter()->notifyHostForActiveSlot(kQuantize, 0.25f);
    }
    else if (selection == "16th")
    {
        getFilter()->notifyHostForActiveSlot(kQuantize, 0.55f);
    }
    else if (selection == "32nd")
    {
        getFilter()->notifyHostForActiveSlot(kQuantize, 0.75f);
    }
    else if (selection == "64th")
    {
        getFilter()->notifyHostForActiveSlot(kQuantize, 1.f);
    }
}

void PizLooperEditor::handleSyncModeComboBoxChange() const
{
    auto selection = syncModeComboBox->getText();
    if (selection == "PPQ (Host 0)")
    {
        getFilter()->notifyHostForActiveSlot(kSync, 0.f);
    }
    else if (selection == "PPQ (Recstart)")
    {
        getFilter()->notifyHostForActiveSlot(kSync, 0.25f);
    }
    else if (selection == "Sample")
    {
        getFilter()->notifyHostForActiveSlot(kSync, 1.f);
    }
}

void PizLooperEditor::handleNoteTriggerComboBoxChange() const
{
    auto selection = noteTriggerComboBox->getText();

    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            if (selection == "Off")
            {
                getFilter()->notifyHost(kNoteTrig, i, 0.f);
            }
            else if (selection == "Mono (Transpose)")
            {
                getFilter()->notifyHost(kNoteTrig, i, 0.05f);
            }
            else if (selection == "Poly (Transpose)")
            {
                getFilter()->notifyHost(kNoteTrig, i, 0.15f);
            }
            else if (selection == "Mono (Orig. Key)")
            {
                getFilter()->notifyHost(kNoteTrig, i, 0.25f);
            }
        }
    }
    else
    {
        if (selection == "Off")
        {
            getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.f);
        }
        else if (selection == "Mono (Transpose)")
        {
            getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.05f);
        }
        else if (selection == "Poly (Transpose)")
        {
            getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.15f);
        }
        else if (selection == "Mono (Orig. Key)")
        {
            getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.25f);
        }
    }
}

void PizLooperEditor::handleLoopModeComboBoxChange() const
{
    auto selection = loopModeComboBox->getText();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            if (selection == "Loop after rec")
            {
                getFilter()->notifyHost(kTrigger, i, 0.f);
            }
            else if (selection == "Sync loop")
            {
                getFilter()->notifyHost(kTrigger, i, 0.05f);
            }
            else if (selection == "Unsync 1-shot")
            {
                getFilter()->notifyHost(kTrigger, i, 0.15f);
            }
            else if (selection == "Unsync loop")
            {
                getFilter()->notifyHost(kTrigger, i, 0.25f);
            }
        }
    }
    else
    {
        if (selection == "Loop after rec")
        {
            getFilter()->notifyHostForActiveSlot(kTrigger, 0.f);
        }
        else if (selection == "Sync loop")
        {
            getFilter()->notifyHostForActiveSlot(kTrigger, 0.05f);
        }
        else if (selection == "Unsync 1-shot")
        {
            getFilter()->notifyHostForActiveSlot(kTrigger, 0.15f);
        }
        else if (selection == "Unsync loop")
        {
            getFilter()->notifyHostForActiveSlot(kTrigger, 0.25f);
        }
    }
}

void PizLooperEditor::handleStepSizeComboBoxChange() const
{
    auto selection = stepSizeComboBox->getText();
    if (selection == "1 Bar")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 0.f);
    }
    else if (selection == "3 Beats")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 0.15f);
    }
    else if (selection == "2 Beats")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 0.25f);
    }
    else if (selection == "1 Beat")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 0.35f);
    }
    else if (selection == "8th Note")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 0.45f);
    }
    else if (selection == "16th Note")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 0.55f);
    }
    else if (selection == "1 Tick")
    {
        getFilter()->notifyHostForActiveSlot(kRecStep, 1.0f);
    }
}

void PizLooperEditor::handleTransposeChannelSliderChange() const
{
    auto* slider = (VSTSlider*) transposeChannelSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kTransposeChannel, i, slider->mapToVSTRange());
                getFilter()->notifyHost(kScaleChannel, i, slider->mapToVSTRange());
            }
        }
        else
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kTransposeChannel, i, slider->mapToVSTRange());
            }
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kTransposeChannel, slider->mapToVSTRange());
        if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            getFilter()->notifyHostForActiveSlot(kScaleChannel, slider->mapToVSTRange());
        }
    }
}

void PizLooperEditor::handleVelocitySensSliderChange() const
{
    auto* slider = (VSTSlider*) velocitySensSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kVeloSens, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kVeloSens, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handlePlayCCSliderChange() const
{
    auto* slider = (VSTSlider*) playCCSlider.get();
    getFilter()->notifyHostForActiveSlot(kPlayCC, slider->mapToVSTRange());
}

void PizLooperEditor::handleRecCCSliderChange() const
{
    auto* slider = (VSTSlider*) recCCSlider.get();
    getFilter()->notifyHostForActiveSlot(kRecCC, slider->mapToVSTRange());
}

void PizLooperEditor::handleNextSlotSliderChange() const
{
    auto* slider = (VSTSlider*) nextSlotSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kNextSlot, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kNextSlot, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleNumLoopsSliderChange() const
{
    auto* slider = (VSTSlider*) numLoopsSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kNumLoops, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kNumLoops, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleMasterTransposeSliderChange() const
{
    auto* slider = (VSTSlider*) masterTransposeSlider.get();
    getFilter()->setParameterNotifyingHost(kMasterTranspose, slider->mapToVSTRange());
}

void PizLooperEditor::handleScaleChannelSliderChange() const
{
    auto* slider = (VSTSlider*) scaleChannelSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kScaleChannel, i, slider->mapToVSTRange());
                getFilter()->notifyHost(kTransposeChannel, i, slider->mapToVSTRange());
            }
        }
        else
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kScaleChannel, i, slider->mapToVSTRange());
            }
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kScaleChannel, slider->mapToVSTRange());
        if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            getFilter()->notifyHostForActiveSlot(kTransposeChannel, slider->mapToVSTRange());
        }
    }
}

void PizLooperEditor::handleMasterVelocitySliderChange() const
{
    auto* slider = (VSTSlider*) masterVelocitySlider.get();
    getFilter()->setParameterNotifyingHost(kMasterVelocity, slider->mapToVSTRange());
}

void PizLooperEditor::handleMuteGroupSliderChange() const
{
    auto* slider = (VSTSlider*) muteGroupSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kMuteGroup, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kMuteGroup, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handlePlayGroupSliderChange() const
{
    auto* slider = (VSTSlider*) playGroupSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kPlayGroup, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kPlayGroup, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleFixedLengthSliderChange() const
{
    auto* slider = (VSTSlider*) fixedLengthSlider.get();
    getFilter()->notifyHostForActiveSlot(kFixedLength, slider->mapToVSTRange());
}

void PizLooperEditor::handleChannelSliderChange() const
{
    auto* slider = (VSTSlider*) slotIOChannelSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kChannel, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kChannel, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleShiftSliderChange() const
{
    auto* slider = (VSTSlider*) shiftPatternSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kShift, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kShift, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleTrigChanSliderChange() const
{
    auto* slider = (VSTSlider*) triggerChannelSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kTrigChan, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kTrigChan, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleHighSliderChange() const
{
    auto* slider = (VSTSlider*) maxTriggerNoteSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kNHigh, i, slider->mapToVSTRange());
            if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                getFilter()->notifyHost(kNLow, i, slider->mapToVSTRange());
            }
        }
    }
    else if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
    {
        getFilter()->notifyHostForActiveSlot(kNHigh, slider->mapToVSTRange());
        getFilter()->notifyHostForActiveSlot(kNLow, slider->mapToVSTRange());
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kNHigh, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleLowSliderChange() const
{
    auto* slider = (VSTSlider*) minTriggerNoteSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kNLow, i, slider->mapToVSTRange());
            if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                getFilter()->notifyHost(kNHigh, i, slider->mapToVSTRange());
            }
        }
    }
    else if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
    {
        getFilter()->notifyHostForActiveSlot(kNLow, slider->mapToVSTRange());
        getFilter()->notifyHostForActiveSlot(kNHigh, slider->mapToVSTRange());
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kNLow, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleRootSliderChange() const
{
    auto* slider = (VSTSlider*) rootNoteSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kRoot, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kRoot, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleStretchSliderChange() const
{
    auto* slider = (VSTSlider*) stretchSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kStretch, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kStretch, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleEndSliderChange() const
{
    auto* slider = (VSTSlider*) loopEndOffsetSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kLoopEnd, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kLoopEnd, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleLoopStartSliderChange() const
{
    auto* slider = (VSTSlider*) loopStartSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kLoopStart, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kLoopStart, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleVelocitySliderChange() const
{
    auto* slider = (VSTSlider*) velocitySlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kVelocity, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kVelocity, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleOctaveSliderChange() const
{
    auto* slider = (VSTSlider*) octaveSlider.get();
    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kOctave, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kOctave, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleTransposeSliderChange() const
{
    auto* slider = (VSTSlider*) transposeSlider.get();

    if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kTranspose, i, slider->mapToVSTRange());
        }
    }
    else
    {
        getFilter()->notifyHostForActiveSlot(kTranspose, slider->mapToVSTRange());
    }
}

void PizLooperEditor::handleNameLabelTextChange() const
{
    getFilter()->changeProgramName(getFilter()->getCurrentProgram(), patternNameLabel->getText());
}

void PizLooperEditor::handleNumeratorLabelTextChange()
{
    int x = numeratorLabel->getText().getIntValue();
    if (x > 0)
    {
        pianoRoll->setTimeSig(x, pianoRoll->timeSigD);
        getFilter()->setTimeSig(lastActiveLoop, x, pianoRoll->timeSigD);
        pianoRoll->repaintBG();
    }
    else
    {
        numeratorLabel->setText(juce::String(getFilter()->getNumerator(lastActiveLoop)), juce::dontSendNotification);
    }
}

void PizLooperEditor::handleLengthLabelChange()
{
    pianoRoll->setDisplayLength(patternLengthLabel->getText().getIntValue());
    getFilter()->setPRSetting("bars", pianoRoll->getDisplayLength(), false);
    getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
}

void PizLooperEditor::handleDenominatorLabelChange()
{
    int x = denominatorLabel->getText().getIntValue();
    if (x > 0 && (x & (x - 1)) == 0)
    {
        pianoRoll->setTimeSig(pianoRoll->timeSigN, x);
        getFilter()->setTimeSig(lastActiveLoop, pianoRoll->timeSigN, x);
        pianoRoll->repaintBG();
    }
    else
    {
        denominatorLabel->setText(juce::String(getFilter()->getDenominator(lastActiveLoop)), juce::dontSendNotification);
    }
}

PizLooper* PizLooperEditor::getFilter() const noexcept
{
    return (PizLooper*) getAudioProcessor();
}
