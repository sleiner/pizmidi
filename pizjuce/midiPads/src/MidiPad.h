#ifndef MIDIPADS_PAD_H
#define MIDIPADS_PAD_H

#include <memory>

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

#define midiScaler (0.007874016f)

class MidiPad  : public Button
{
public:
    //==============================================================================
  	MidiPad (int _index);
    ~MidiPad();

    void resized();
    void buttonClicked (Button*);
    void setColour(const Colour&);
    bool isPlaying;
	void setXFloat(float v) {x=v;}
	void setYFloat(float v) {y=v;}
	void setXInt(int v) {x=v*midiScaler;}
	void setYInt(int v) {y=v*midiScaler;}
	int getIndex() {return index;}
	String getIconPath();
	void setIconPath(String name);
	void setText(const String& name);
	String getText();

    //==============================================================================
    void setImages (const Drawable* normalImage);
	bool setImageFromFile (File file);

    //==============================================================================
    void setBackgroundColours (const Colour& toggledOffColour,
                               const Colour& toggledOnColour);

    const Colour& getBackgroundColour() const throw();

    //==============================================================================
    const Drawable* getCurrentImage() const throw();
    const Drawable* getNormalImage() const throw();

    String Description;
    bool showx, showy;
    float roundness;
    bool showdot;
	bool showvalues;
	float imageSize;
	void setCenteredText(bool centered);
    void setHex(bool newhex);
    bool isHex();

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
    float x;
    float y;
	int index;
    bool hitTest(int x, int y);
    std::unique_ptr<Drawable> normalImage;
    Colour backgroundOff, backgroundOn;
    Path hexpath;
    bool hex;
    void deleteImages();
	String iconPath;
	Label* text;

	MidiPad (const MidiPad&);
    const MidiPad& operator= (const MidiPad&);
};

#endif
