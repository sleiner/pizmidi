#ifndef PIZ_CC_SLIDER_HEADER
#define PIZ_CC_SLIDER_HEADER

class CCSlider : public Slider
{
public:
	CCSlider(String name) : Slider(name), allText("Any")
	{
		this->setRange(-1,127,1);
	};
	~CCSlider() {};

	const String getTextFromValue(double value)
	{
		if (value<0) return allText;
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