#ifndef PIZ_NOTE_SLIDER_HEADER
#define PIZ_NOTE_SLIDER_HEADER

class NoteSlider : public Slider
{
public:
	NoteSlider(String name) : Slider(name), allText("Off"), bottomOctave(-2)
	{
		this->setRange(-1,127,1);
	};
	~NoteSlider() {};

	String getTextFromValue(double value)
	{
		const int n = roundToInt(value);
		if (n<0) return allText;
		return getNoteName(n,bottomOctave) + " ("+String((int)value)+")";
	}

	double getValueFromText(const String &text)
	{
		if (!text.compareIgnoreCase(allText)) return -1;
		if (text.containsAnyOf("abcdefgABCDEFG")) {
			bool hasOctave;
			int n = getNoteValue(text,bottomOctave,hasOctave);
			if (n!=NOT_A_NOTE)
				return n;
			return getValue();
		}
		return text.getDoubleValue();
	}

	void setAllText(String text)
	{
		allText = text;
		updateText();
	}
	void setBottomOctave(int o)
	{
		bottomOctave = o;
	}

private:
	String allText;
	int bottomOctave;
};

#endif