#ifndef PIZ_CHANNEL_SLIDER_HEADER
#define PIZ_CHANNEL_SLIDER_HEADER

class ChannelSlider : public Slider
{
public:
	ChannelSlider(String name) : Slider(name), allText("Any")
	{
		this->setRange(0,16,1);
	};
	~ChannelSlider() {};

	String getTextFromValue(double value)
	{
		if (value<0.5) return allText;
		return String((int)value);
	}

	double getValueFromText(const String &text)
	{
		if (!text.compareIgnoreCase(allText)) return 0;
		return text.getDoubleValue();
	}

	void setAllText(String text)
	{
		allText = text;
		updateText();
	}
private:
	String allText;
};

#endif