#ifndef imagePluginFilter_PAD_H
#define imagePluginFilter_PAD_H

#include <memory>

#include "../_common/PizAudioProcessor.h"

using namespace juce;

class MidiPad  : public Button
{
public:
    //==============================================================================
  	MidiPad ();
    ~MidiPad();

    void resized();
    void buttonClicked (Button*);
    void setColour(const Colour&);
    void setTextColour(const Colour&);
	int getIndex() {return index;}
	String getIconPath();
	void setIconPath(String name);
	void setText(const String& name);
	void setButtonText(const String& newText);
	String getText();

    //==============================================================================
    void setImages (const Drawable* normalImage);
	bool setImageFromFile (File file);

    //==============================================================================
    const Drawable* getCurrentImage() const throw();
    const Drawable* getNormalImage() const throw();
	const Colour& getBackgroundColour() const throw();

    String Description;
	float imageSize;
	void setCenteredText(bool centered);

    //==============================================================================
    juce_UseDebuggingNewOperator

protected:
	void drawButtonBackground (Graphics& g,
                               Button& button,
                               const Colour& backgroundColour,
                               bool isMouseOverButton,
                               bool isButtonDown);

    void paintButton (Graphics& g,
                      bool isMouseOverButton,
                      bool isButtonDown);


private:
	bool centeredText;
	int index;
    std::unique_ptr<Drawable> normalImage;
    Colour bgColour, textColour;
    void deleteImages();
	String iconPath;
	Label* text;

	MidiPad (const MidiPad&);
    const MidiPad& operator= (const MidiPad&);
};

#endif
