/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  17 Aug 2011 6:31:51am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PizButton.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PizButton::PizButton ()
    : Button (String()),
      cachedImage_pirate_png (nullptr)
{
    cachedImage_pirate_png = ImageCache::getFromMemory (pirate_png, pirate_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PizButton::~PizButton()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PizButton::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PizButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    g.setColour (Colours::black);
    g.drawImageWithin (cachedImage_pirate_png,
                       proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (1.0000f),
                       RectanglePlacement::centred,
                       false);
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Button" className="PizButton" componentName=""
                 parentClasses="public Button" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0" buttonState="normal" enabled="1"/>
  <BACKGROUND backgroundColour="0" buttonState="over" enabled="0"/>
  <BACKGROUND backgroundColour="0" buttonState="down" enabled="0"/>
  <BACKGROUND backgroundColour="0" buttonState="normal on" enabled="0"/>
  <BACKGROUND backgroundColour="0" buttonState="over on" enabled="0"/>
  <BACKGROUND backgroundColour="0" buttonState="down on" enabled="0"/>
  <BACKGROUND backgroundColour="0" buttonState="common background" enabled="1">
    <IMAGE pos="0% 0% 100% 100%" resource="pirate_png" opacity="1" mode="1"/>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: pirate_png, 8193, "../../../My Documents/My Pictures/pirate.png"
static const unsigned char resource_PizButton_pirate_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,139,0,0,0,78,8,6,0,0,0,16,60,229,248,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,0,6,98,
75,71,68,0,255,0,255,0,255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,216,11,3,19,51,47,22,6,239,93,0,0,0,25,116,69,88,116,67,111,109,109,101,110,116,
0,67,114,101,97,116,101,100,32,119,105,116,104,32,71,73,77,80,87,129,14,23,0,0,31,92,73,68,65,84,120,218,237,157,121,124,84,213,217,199,191,207,57,119,54,66,66,2,136,44,26,34,178,91,80,68,251,42,214,181,
174,109,181,104,181,46,216,214,226,66,173,90,151,182,182,117,183,238,86,219,90,235,218,106,221,45,218,214,133,138,85,113,1,139,27,8,34,10,130,84,54,65,69,20,12,132,100,102,238,61,207,251,199,185,51,76,
64,32,97,145,68,121,194,253,132,201,103,146,185,247,220,223,121,246,231,119,97,139,108,145,38,138,124,149,47,126,175,157,119,62,37,151,203,221,214,144,203,145,203,231,201,230,243,228,242,121,84,21,17,
33,153,72,144,74,36,72,38,18,164,147,73,146,201,228,136,113,147,38,221,190,5,44,95,18,217,123,231,157,247,95,188,116,233,51,159,213,213,145,205,229,136,162,136,200,57,92,225,80,45,190,142,156,67,85,155,
190,88,34,88,99,48,198,248,239,34,152,146,215,214,90,82,201,36,237,202,202,232,88,89,121,192,216,73,147,198,108,1,75,11,145,129,61,123,234,146,101,203,200,230,114,69,173,144,205,231,113,206,109,246,115,
51,198,20,181,82,42,145,32,149,76,82,85,94,206,155,179,102,201,22,176,108,98,233,211,189,187,46,169,173,45,130,34,151,207,19,181,0,80,52,87,172,49,36,75,64,84,85,81,193,140,185,115,191,210,238,192,6,203,
158,131,6,93,93,221,185,179,86,149,151,107,50,145,80,224,75,123,36,19,9,173,42,47,215,234,206,157,117,207,65,131,174,222,162,89,154,32,131,251,245,211,5,139,22,81,87,95,207,138,134,134,86,169,57,54,134,
230,105,147,78,83,150,201,208,173,83,39,94,159,62,93,182,128,37,150,190,53,53,186,120,233,82,234,234,235,169,207,102,183,168,212,85,36,147,74,81,150,201,208,177,178,146,119,230,204,249,106,154,171,206,
29,58,104,58,153,252,82,155,151,141,125,164,147,73,237,220,161,131,126,37,0,178,251,192,129,218,177,178,82,19,65,176,229,230,111,192,145,8,2,237,88,89,169,67,6,14,252,244,75,7,146,190,53,53,154,73,165,
84,48,10,168,136,168,96,21,100,203,205,95,219,33,168,128,138,9,84,36,80,43,86,141,88,13,68,226,181,52,218,38,157,214,254,61,122,180,126,109,211,163,91,55,205,20,76,141,196,7,70,5,163,178,5,40,77,0,139,
196,96,17,77,5,162,87,159,55,92,39,63,247,103,125,247,213,191,234,208,253,119,81,83,178,134,101,153,140,246,171,169,217,164,160,217,36,14,83,207,109,183,213,5,139,22,81,159,205,34,2,134,4,138,195,136,
3,21,28,130,26,69,163,175,94,180,211,188,155,35,49,22,132,179,70,124,159,11,207,58,28,193,97,20,114,54,195,215,134,12,227,163,79,86,52,250,157,170,242,114,58,119,236,200,244,217,179,55,250,189,53,27,243,
143,237,177,211,78,19,203,219,180,209,89,243,231,123,160,96,227,45,18,113,232,1,59,50,107,226,61,76,24,115,19,195,143,57,24,227,90,125,2,249,139,3,141,8,29,219,5,88,205,227,68,112,86,73,68,89,246,30,178,
203,106,239,93,178,108,25,211,103,207,166,178,188,92,247,25,60,184,71,139,4,75,239,234,106,125,121,202,148,193,203,86,52,196,59,66,192,40,70,2,134,30,178,27,127,187,225,215,84,166,13,53,221,202,185,230,
226,31,114,195,21,167,197,80,49,45,110,47,151,30,136,196,71,252,127,164,240,111,147,67,221,219,32,65,48,180,201,36,81,227,189,60,1,66,132,229,117,245,24,1,83,60,175,149,107,185,116,217,50,94,153,58,245,
127,61,183,221,86,91,20,88,170,42,42,116,230,188,121,136,26,4,87,188,34,139,242,224,45,231,113,215,31,206,34,239,178,56,163,160,150,164,58,190,63,116,15,239,194,224,90,16,84,180,196,185,50,49,40,20,81,
69,98,47,75,0,81,131,98,209,77,12,151,194,231,169,137,232,212,101,43,84,253,249,56,167,88,43,204,152,49,13,196,160,4,241,121,53,94,203,134,92,142,89,243,231,83,85,81,161,155,29,44,189,171,171,53,157,74,
233,146,218,90,0,156,56,212,0,4,88,49,252,227,175,23,243,205,189,123,18,185,136,140,88,140,179,32,121,234,53,203,132,55,223,243,55,67,100,83,88,196,13,187,73,198,96,140,162,88,68,44,198,72,81,169,96,252,
93,52,226,48,102,211,128,165,80,201,70,12,86,12,7,125,99,39,14,221,111,8,182,184,82,6,52,226,181,49,247,48,242,214,243,233,187,221,214,88,107,80,243,249,107,184,164,182,150,116,42,165,189,171,171,55,8,
52,193,250,254,98,251,138,10,125,119,222,60,74,63,93,177,136,134,164,13,12,59,106,31,246,30,210,7,113,22,53,89,66,177,136,58,84,45,9,18,132,217,60,101,101,105,150,215,53,180,64,183,82,169,204,164,57,246,
251,7,177,77,231,118,244,238,209,149,108,62,203,123,239,125,192,135,139,87,48,250,153,23,153,251,65,45,145,110,26,173,232,156,195,24,67,143,234,14,92,115,254,79,217,111,143,237,9,163,101,136,9,40,40,110,
112,36,109,158,3,247,236,75,239,94,23,179,235,55,79,1,183,230,13,215,144,205,242,238,188,121,180,175,168,208,79,107,107,101,253,86,166,185,137,181,1,3,244,141,153,51,63,55,53,111,196,34,68,220,127,243,
69,28,180,111,95,111,116,81,84,242,64,2,81,3,132,160,22,17,88,184,120,57,23,92,117,39,143,61,57,1,71,4,24,111,154,20,52,254,106,124,170,186,73,128,81,226,73,98,8,56,234,208,255,227,134,171,78,35,97,41,
70,253,43,55,132,65,196,178,188,174,158,127,62,49,150,241,175,189,203,35,163,198,227,164,240,174,8,17,112,78,138,191,177,230,79,55,168,56,127,109,26,187,68,248,12,202,249,231,28,205,207,78,30,138,37,66,
112,40,82,244,168,20,159,162,18,39,168,113,40,1,39,158,121,35,143,60,253,50,52,1,192,153,84,138,157,122,247,230,229,169,83,101,147,129,101,80,159,62,58,125,246,108,26,114,185,53,120,237,150,202,182,150,
217,19,31,0,151,197,137,67,92,18,31,44,11,145,184,248,158,123,115,68,24,161,65,130,215,223,158,205,53,127,120,152,113,47,77,33,31,123,118,70,29,14,83,146,118,216,52,121,131,149,126,135,96,141,112,198,
201,7,114,201,185,39,224,178,138,152,104,141,75,228,156,195,170,37,146,136,28,1,247,254,125,12,127,185,231,113,254,55,231,83,84,20,136,64,101,21,192,175,238,3,40,54,126,191,34,106,216,115,247,126,252,
254,242,17,244,236,210,17,136,80,17,140,218,149,102,94,124,40,173,162,160,130,168,197,16,113,198,69,119,112,223,63,198,162,26,54,233,218,211,201,36,253,182,219,142,201,51,102,200,70,7,75,255,237,182,211,
89,239,191,79,46,159,95,75,136,103,216,190,186,61,175,60,249,103,196,229,145,192,98,156,65,108,72,232,66,130,68,154,220,138,229,36,131,54,40,66,104,12,9,205,33,46,65,100,132,87,38,207,228,182,187,71,243,
239,167,94,69,173,197,185,208,47,204,38,82,247,5,176,136,49,136,58,14,222,119,23,238,185,245,28,18,121,139,154,28,42,107,143,121,28,46,214,4,254,112,146,224,95,79,142,231,188,203,110,103,209,210,28,98,
20,23,133,107,241,141,44,24,133,72,176,56,110,184,234,116,142,61,124,47,172,203,227,240,192,48,46,129,177,17,121,13,49,38,229,157,94,245,218,78,85,17,205,177,112,9,236,125,216,89,124,188,180,22,23,53,
125,99,37,19,9,122,110,179,13,211,154,152,147,177,77,2,74,143,30,58,115,222,60,242,97,184,78,111,121,73,109,150,129,59,244,161,111,143,78,168,128,65,17,155,228,157,119,63,226,182,59,71,179,219,238,187,
98,77,228,195,106,141,112,36,98,85,172,116,237,92,201,225,135,252,31,199,29,121,0,70,148,55,166,189,75,228,116,83,41,150,98,232,110,5,218,181,73,241,248,125,151,210,38,48,136,248,60,123,28,59,175,37,90,
145,216,112,10,206,56,132,136,29,122,119,97,248,15,15,231,173,105,239,50,123,238,66,214,214,181,89,136,176,118,236,215,157,135,239,188,128,125,118,235,141,209,16,181,198,107,13,252,223,199,40,11,23,231,
56,122,248,53,60,248,200,243,124,188,164,158,84,89,154,76,219,42,38,78,153,203,41,191,184,158,57,239,127,210,36,19,84,42,145,115,44,89,182,140,190,219,109,119,201,199,75,150,92,186,193,154,101,151,254,
253,245,205,119,223,93,171,70,89,137,188,128,72,34,2,133,27,127,119,38,71,31,54,4,167,134,251,70,62,207,111,46,187,133,92,164,108,221,177,130,95,254,108,24,63,60,106,31,2,9,189,170,118,226,77,86,108,153,
21,112,2,161,19,30,24,249,28,247,62,252,52,111,78,159,135,138,241,59,78,53,246,127,40,81,225,235,131,41,31,234,27,177,92,115,209,112,78,62,102,31,66,49,152,120,215,42,134,64,29,78,140,7,190,186,88,219,
52,202,134,52,94,198,248,165,181,73,206,62,239,79,220,253,200,203,132,81,232,195,109,19,97,157,224,4,48,130,85,195,97,7,238,194,109,191,63,135,64,242,168,56,68,87,238,95,163,126,85,199,190,246,46,63,60,
227,183,124,182,60,68,213,53,202,241,104,156,143,1,65,84,215,107,95,37,19,9,6,246,234,197,196,105,211,100,189,193,178,215,160,65,167,188,54,109,218,109,13,77,237,51,17,137,93,192,36,66,72,69,219,36,245,
117,89,242,234,29,117,85,239,215,136,115,116,170,106,203,25,39,29,206,9,199,31,68,89,42,196,68,150,188,17,140,42,70,227,132,84,100,145,132,35,68,120,119,214,71,60,248,175,231,185,243,190,81,172,200,154,
120,97,93,188,88,177,243,219,220,149,18,127,82,219,87,119,98,194,51,55,129,230,49,26,129,19,36,81,70,46,202,18,72,14,84,81,18,30,198,77,52,220,170,138,77,164,25,118,202,149,140,126,126,114,236,191,37,
188,175,38,1,1,202,229,231,253,136,19,135,29,132,9,179,24,155,192,73,0,210,224,175,63,206,126,255,249,174,167,184,228,186,123,177,8,81,20,109,178,172,84,58,149,226,235,253,251,143,24,55,121,242,237,235,
5,150,178,76,70,235,234,235,155,161,214,13,38,246,220,93,33,171,132,69,84,64,194,248,102,122,253,129,40,6,37,29,24,142,255,254,183,57,230,200,61,216,121,135,30,16,229,65,192,169,128,201,131,75,97,212,
32,146,69,53,65,104,224,95,163,254,203,63,30,127,137,49,255,157,178,18,31,234,104,238,190,50,226,187,242,239,184,225,108,14,221,111,71,239,76,58,229,209,231,166,113,242,207,174,164,95,207,110,252,238,
210,211,248,250,206,221,227,116,93,243,242,182,198,65,3,202,128,61,78,229,227,207,86,32,26,161,128,85,120,248,238,223,178,231,215,187,99,197,128,90,156,40,162,121,2,77,18,105,68,131,75,112,226,207,174,
230,233,177,83,136,84,80,220,250,109,136,102,72,89,38,67,93,125,189,52,219,103,233,220,161,67,49,217,214,44,135,81,172,15,235,138,201,115,7,113,52,164,197,148,121,33,177,21,144,115,142,73,83,103,240,192,
67,207,242,216,232,87,200,70,66,191,62,61,72,5,133,168,41,2,241,225,161,154,8,171,194,215,122,119,231,123,135,238,206,143,143,251,22,153,84,138,169,111,191,67,46,92,143,168,73,12,187,13,234,197,229,231,
254,0,49,222,252,153,160,140,111,29,243,11,26,114,240,225,146,58,170,218,181,101,159,61,190,22,231,54,76,179,226,199,8,8,36,34,83,86,198,51,99,223,136,221,105,195,221,55,255,138,131,190,209,7,209,192,
155,81,241,225,178,53,1,33,142,215,167,206,225,240,31,94,204,196,169,179,189,201,146,66,94,121,211,74,62,12,233,220,161,195,37,203,235,235,47,109,114,6,119,151,126,253,116,241,210,165,205,175,101,160,
104,188,195,87,205,146,104,97,87,168,22,119,72,228,34,175,226,21,242,78,121,123,214,251,156,127,229,223,168,25,52,140,31,156,113,19,15,61,250,18,142,148,207,65,56,231,125,9,28,17,14,53,74,151,14,25,126,
117,250,119,153,57,225,46,134,31,181,79,92,190,145,181,106,62,196,98,172,245,179,62,40,87,95,122,18,145,203,17,57,239,187,52,228,66,106,151,215,123,13,128,165,93,121,89,60,91,36,168,248,112,88,34,127,
243,194,72,113,100,80,245,65,190,32,56,117,24,241,17,11,226,16,77,114,194,113,7,17,224,0,139,74,196,244,89,239,17,136,196,166,84,240,137,96,37,114,112,251,125,207,114,240,177,23,49,231,253,197,94,91,198,
179,77,186,233,50,8,141,100,241,210,165,236,210,175,159,54,25,44,211,231,204,33,140,162,245,45,127,149,92,148,174,251,189,171,253,196,130,77,241,196,179,47,115,234,5,55,211,117,135,163,57,235,194,187,
121,104,244,56,66,202,16,9,176,68,36,34,37,138,34,196,36,72,5,105,190,253,173,189,49,235,168,24,168,137,157,67,167,32,134,91,174,251,57,253,123,116,245,26,80,64,93,72,202,134,156,113,242,17,24,9,169,108,
19,114,204,17,123,19,136,7,190,168,1,113,56,19,225,36,201,153,191,185,145,238,3,14,103,226,212,249,184,24,94,51,254,247,33,106,50,56,145,56,152,82,68,67,190,117,240,16,191,129,156,229,237,233,11,136,92,
193,162,25,212,40,115,63,252,140,35,126,124,5,23,94,117,55,14,193,137,178,106,94,239,139,232,112,10,163,136,233,115,230,52,205,103,233,209,173,155,190,183,96,193,230,202,180,251,168,166,232,40,139,239,
127,17,111,206,146,40,135,28,184,7,3,250,215,128,58,22,127,178,140,177,47,77,96,214,255,22,17,74,137,246,90,131,36,16,34,155,66,52,207,181,23,14,231,199,199,238,67,224,148,72,124,122,204,137,33,208,16,
197,226,36,131,19,8,92,22,37,239,75,25,241,205,87,34,48,101,244,217,245,71,44,94,94,199,118,93,182,102,220,127,110,98,216,73,151,50,254,181,183,25,122,208,174,220,241,199,115,64,67,162,216,124,92,127,
219,104,174,184,225,1,196,9,109,203,44,115,39,221,135,152,28,234,50,220,124,199,19,92,114,253,93,132,42,62,157,31,95,132,215,95,155,167,208,218,163,91,55,222,91,176,64,214,90,27,90,244,233,230,108,235,
140,93,200,56,26,112,226,98,247,198,33,162,228,49,60,254,212,43,60,254,244,43,197,36,160,35,242,230,5,133,117,140,162,170,81,118,223,113,91,46,248,249,241,12,217,165,47,145,134,184,216,52,88,39,88,141,
8,77,0,26,34,212,97,208,120,137,108,1,197,160,198,171,99,205,114,221,21,167,51,252,140,171,153,251,193,71,108,183,227,209,68,46,66,73,81,159,143,8,53,34,112,160,70,8,128,234,46,109,81,21,44,66,93,125,
200,39,181,33,175,77,152,206,5,87,221,193,236,5,139,64,125,248,239,10,190,221,106,90,250,139,149,207,195,65,35,228,244,171,169,209,53,169,160,47,88,193,196,17,83,12,32,81,84,99,181,142,98,36,118,21,75,
146,80,10,24,53,88,11,233,52,148,149,181,161,75,151,78,244,168,217,134,154,109,187,176,85,85,134,161,223,250,6,29,170,202,17,162,213,211,36,113,84,236,100,205,106,87,85,192,100,145,168,28,209,58,108,34,
205,81,63,185,150,167,158,159,92,44,75,116,217,186,29,163,239,190,140,234,234,173,252,231,168,1,227,152,50,117,14,251,126,255,2,255,57,70,72,39,19,212,103,115,159,63,107,45,108,54,144,172,130,7,166,151,
140,159,52,210,44,11,23,47,110,17,117,223,130,18,54,106,226,56,42,65,32,57,14,252,230,78,12,26,208,151,175,245,238,198,142,95,235,203,214,91,85,122,135,54,238,79,114,206,249,236,171,70,168,70,88,172,159,
123,14,29,9,27,224,172,18,185,112,101,68,86,138,8,89,45,189,246,185,247,80,92,2,165,14,146,41,30,25,253,58,111,188,53,51,14,170,35,182,239,177,45,143,254,245,60,186,109,83,9,145,55,93,38,46,138,86,85,
85,174,12,2,156,82,159,205,53,199,149,219,44,178,42,30,26,129,165,190,161,133,181,11,88,33,80,199,241,223,219,131,243,206,57,129,173,218,101,16,66,31,69,0,184,108,163,149,141,13,68,241,134,59,137,188,
249,72,40,57,201,33,106,215,26,45,173,91,229,69,190,229,34,8,56,237,23,127,102,228,99,227,81,35,88,81,156,26,150,213,46,35,95,191,2,23,85,97,213,20,207,204,169,174,54,172,223,28,246,134,205,37,171,226,
161,24,63,244,169,169,209,92,24,182,168,147,53,192,95,126,127,46,127,184,252,36,182,234,224,43,213,26,87,174,125,196,128,239,73,21,193,73,105,133,90,99,205,164,241,192,132,16,68,102,131,243,20,190,66,
236,88,94,239,120,232,241,255,18,89,139,224,168,222,182,35,214,194,162,143,107,25,254,203,27,177,226,112,82,162,180,68,168,91,190,156,214,54,175,145,11,67,250,148,76,12,20,193,178,228,179,207,90,128,175,
226,251,72,69,12,34,1,167,14,59,144,239,30,50,0,227,28,26,197,59,53,174,4,139,176,74,191,172,162,34,107,60,156,108,120,74,75,81,84,28,101,169,20,9,11,149,101,73,254,254,151,139,152,240,244,205,244,223,
190,26,12,188,61,109,30,245,185,32,78,226,41,42,17,129,179,204,249,240,195,198,189,51,173,68,74,113,81,52,67,45,102,206,216,39,52,200,36,148,139,126,51,2,137,106,81,2,48,14,89,77,55,72,115,144,184,225,
154,78,65,212,162,172,224,229,167,255,68,215,174,91,99,194,60,73,147,231,150,235,206,226,176,227,126,73,186,77,91,146,25,193,228,87,78,138,57,28,243,230,47,166,21,98,165,17,46,138,96,105,17,140,5,162,
241,142,132,99,142,56,144,128,21,136,4,45,102,141,85,92,172,161,12,213,93,58,32,26,162,65,68,62,52,244,235,213,137,87,158,185,153,108,131,98,242,203,113,100,128,200,103,139,76,200,242,186,160,208,56,216,
170,164,20,23,43,193,178,222,25,219,141,169,84,36,78,233,27,122,213,116,68,227,144,217,167,205,77,11,24,51,242,53,156,194,20,128,67,16,13,48,68,168,66,251,242,52,65,85,18,141,114,128,224,76,4,26,224,136,
152,59,111,65,139,9,137,155,5,150,18,92,4,165,105,222,150,144,95,241,105,114,97,199,29,106,226,106,117,236,90,181,128,121,180,98,89,160,248,122,101,166,21,21,140,129,72,35,223,1,167,17,66,128,113,17,203,
163,4,15,63,254,124,107,196,74,35,92,180,164,9,175,18,159,196,209,171,87,77,43,26,88,92,233,109,75,163,31,9,106,224,214,191,61,70,168,142,214,62,129,89,4,75,96,109,11,58,45,37,147,73,211,234,69,149,200,
102,184,249,142,199,125,133,167,21,142,118,151,226,162,8,22,219,34,192,82,80,210,150,247,231,127,216,42,163,135,85,29,226,187,238,31,205,146,229,33,34,218,42,245,138,253,92,176,24,211,34,160,98,227,122,
224,243,47,78,246,221,237,234,51,177,133,52,185,117,9,174,185,229,81,238,184,255,185,146,170,172,182,8,96,197,81,127,60,141,160,64,27,174,191,101,164,239,163,118,141,235,78,173,6,44,37,184,40,254,47,147,
74,181,132,173,232,227,12,129,127,61,241,50,104,2,113,96,92,224,155,142,108,130,35,79,189,146,223,221,248,47,126,117,217,157,52,228,253,96,91,75,50,159,138,96,197,160,234,184,243,129,209,124,244,201,242,
149,229,137,86,168,41,75,113,81,4,75,85,187,118,45,68,117,251,186,201,196,183,222,99,212,179,147,9,113,136,248,22,133,71,255,61,158,49,47,190,233,107,45,34,44,92,248,1,206,233,202,80,106,179,187,185,26,
135,255,16,210,134,107,110,124,144,40,242,195,255,173,149,137,166,20,23,69,176,204,152,51,71,146,65,176,153,79,205,21,23,91,69,57,245,231,191,99,246,194,79,124,155,138,4,92,121,227,195,241,246,140,135,
187,108,130,22,131,20,252,28,144,138,146,115,33,55,222,246,15,62,254,44,235,125,21,149,226,228,67,107,146,100,16,48,163,164,69,161,145,163,146,73,111,222,8,68,227,8,66,113,160,80,159,87,78,56,227,6,86,
100,133,23,94,154,202,123,115,22,82,80,36,168,210,117,235,173,26,221,128,200,41,133,90,175,152,164,159,220,19,227,155,153,156,217,228,183,42,20,71,194,9,181,117,202,181,55,141,140,93,169,184,222,213,40,
174,110,37,38,104,21,60,52,2,75,215,142,29,91,136,229,143,119,170,131,183,103,206,229,216,17,87,242,159,177,211,113,37,77,39,73,11,169,84,208,168,229,192,18,241,242,148,121,236,122,224,217,108,179,227,
209,140,121,105,102,220,235,106,190,144,93,29,168,144,151,60,23,93,115,47,249,40,30,77,17,64,29,70,87,150,50,90,139,172,138,135,70,118,103,250,156,57,210,54,147,209,229,205,152,21,218,180,18,225,212,50,
238,181,183,121,241,181,183,40,178,172,169,210,175,79,23,212,133,241,107,175,229,63,169,205,115,196,241,231,83,159,247,115,141,207,141,159,194,126,187,247,141,217,147,54,125,232,42,46,96,194,244,153,220,
255,207,103,113,152,184,83,206,130,241,67,254,98,2,136,114,177,98,108,217,200,105,155,201,52,234,146,91,77,179,0,116,106,223,190,197,156,176,195,98,53,242,190,0,6,149,2,253,133,176,215,30,187,22,179,92,
5,229,242,167,219,71,211,224,196,83,84,136,210,144,205,145,40,180,80,126,1,46,131,75,24,206,191,248,110,239,121,169,162,49,109,215,158,187,14,224,232,195,246,68,92,54,238,228,107,249,42,230,243,112,176,
26,88,222,91,176,64,202,50,153,77,180,245,132,70,95,98,253,207,138,204,74,130,136,193,136,37,157,176,236,58,104,59,142,26,186,31,189,106,186,22,231,160,193,191,191,170,60,19,79,33,26,175,230,131,20,127,
251,251,40,84,149,194,228,82,251,138,10,162,194,196,64,33,52,215,2,220,226,27,138,243,223,181,249,122,199,184,184,13,33,10,49,26,112,211,221,79,242,198,180,255,197,142,174,31,57,73,167,133,7,110,253,37,
55,93,61,130,5,147,239,231,178,115,143,167,186,75,71,159,236,138,249,226,76,129,183,174,133,56,53,101,153,204,106,157,253,171,153,161,130,244,171,169,225,141,153,51,55,122,113,81,212,162,18,22,35,24,193,
197,188,105,190,33,114,207,221,250,179,239,55,6,113,216,33,187,83,211,173,163,175,222,154,28,159,53,36,56,246,199,87,240,202,27,239,248,56,40,18,158,122,97,18,39,28,123,32,85,229,22,35,194,152,241,147,
169,171,207,55,114,124,50,169,32,142,164,252,109,136,108,136,117,6,156,224,60,115,31,198,133,69,83,209,220,155,229,68,16,13,49,198,48,99,254,135,92,113,237,61,190,136,232,188,9,85,17,78,29,126,56,21,105,
75,168,66,50,9,167,253,248,32,78,249,209,193,252,251,201,215,249,195,109,15,241,214,140,133,168,24,68,195,226,252,230,230,148,192,90,250,213,212,48,113,250,244,117,107,22,128,137,211,167,75,199,202,202,
77,160,89,194,24,36,1,134,4,38,238,11,185,232,156,99,152,53,233,78,254,249,183,95,113,198,137,251,83,221,181,45,234,28,136,69,52,69,219,36,60,118,255,197,28,178,239,96,2,235,239,254,171,147,103,176,239,
208,115,25,55,105,38,121,53,188,48,238,109,84,27,251,2,237,202,5,35,65,129,87,11,27,25,4,195,152,151,102,176,255,247,206,231,138,235,31,68,221,250,19,9,170,68,72,100,113,65,138,51,126,125,11,13,33,132,
145,159,202,68,61,221,200,79,127,120,176,215,94,40,106,20,34,67,202,89,134,30,178,51,47,140,186,150,219,175,63,155,173,171,202,1,27,147,23,109,94,233,88,89,201,196,53,60,137,100,163,14,198,55,193,10,161,
88,172,129,254,189,182,229,151,167,29,205,183,14,24,140,37,139,33,137,106,232,205,10,10,18,197,233,115,41,105,165,76,241,179,139,111,227,190,191,143,241,102,197,10,86,149,35,190,189,39,143,62,249,34,185,
168,241,236,208,227,247,92,192,222,187,12,36,50,57,79,182,131,227,233,23,223,227,7,35,126,75,136,31,244,158,243,250,61,180,105,67,220,133,223,188,240,214,169,98,69,185,238,214,127,115,229,13,15,53,30,
218,85,216,125,151,62,60,117,207,165,228,141,150,12,175,121,211,41,26,155,79,43,52,228,225,23,23,253,133,191,63,50,150,80,163,205,106,126,214,54,24,191,86,40,15,238,219,119,68,186,25,101,0,67,0,2,129,
73,98,172,103,91,148,248,89,130,34,2,198,208,161,60,201,45,87,157,201,184,81,215,113,232,254,59,98,201,249,141,239,194,152,62,35,196,105,136,146,192,186,164,143,116,108,154,81,163,39,112,196,73,151,115,
223,67,79,199,213,32,135,70,17,161,115,60,60,106,44,185,200,173,54,100,182,85,135,42,212,133,168,68,24,53,212,231,133,225,103,94,78,168,113,164,133,227,249,23,39,196,62,205,186,129,34,90,224,129,241,95,
70,29,227,39,207,229,170,27,70,226,180,116,158,219,55,105,31,180,207,46,68,56,140,11,17,49,68,177,77,148,66,235,84,156,173,78,5,240,167,43,79,225,95,247,92,76,247,206,29,124,15,178,241,220,117,43,123,
140,77,145,147,87,10,190,95,97,19,197,147,214,27,226,242,164,83,41,6,247,237,59,98,109,239,89,107,97,101,238,135,31,190,190,83,159,62,151,44,94,186,180,105,109,151,226,135,191,7,244,238,202,217,35,14,
99,247,193,3,153,51,247,125,150,45,111,64,212,242,205,221,118,224,241,145,87,49,120,64,39,172,179,177,215,233,57,101,157,17,79,173,161,1,98,44,86,12,99,39,78,229,246,123,159,231,132,211,175,226,161,39,
95,98,238,252,15,80,181,77,14,107,246,248,122,95,250,245,234,134,113,73,66,27,114,195,109,163,120,254,149,183,189,103,160,126,120,109,200,224,158,12,30,216,171,241,44,209,26,205,142,128,90,255,136,10,
148,143,63,203,115,248,176,243,88,214,144,143,189,104,175,21,11,185,159,108,54,79,125,78,233,215,175,47,54,240,191,99,138,36,68,209,202,158,23,5,17,165,186,75,21,39,28,119,16,139,23,125,202,212,119,230,
199,165,143,149,180,236,165,36,20,165,243,78,72,97,226,105,253,242,73,201,68,130,1,61,123,242,242,212,169,135,173,189,156,209,4,233,223,163,135,206,156,59,119,221,14,175,17,42,210,9,198,255,231,102,170,
59,165,177,34,212,69,41,238,253,251,104,68,132,31,31,119,32,184,168,17,176,68,29,22,37,82,135,73,182,225,133,241,147,121,250,133,183,184,255,161,255,176,116,69,222,239,28,197,83,111,168,41,225,102,88,
247,162,116,238,88,201,115,163,174,165,107,219,20,38,145,162,215,144,147,249,104,201,10,32,242,4,207,226,184,246,252,227,57,105,216,33,159,195,232,244,121,201,66,23,179,49,9,206,150,113,216,49,191,97,
252,27,211,81,13,60,177,144,184,198,167,37,22,65,9,112,12,253,246,94,124,103,255,193,28,250,237,33,104,212,64,160,1,145,83,74,235,160,138,34,46,192,73,142,201,111,205,227,194,171,238,101,226,148,153,132,
106,99,212,196,217,232,162,27,108,16,18,68,228,1,23,167,24,154,7,150,192,90,122,119,239,206,180,247,222,91,231,2,108,100,2,194,128,95,159,126,4,231,157,62,148,72,197,211,88,24,131,186,8,99,5,220,202,97,
8,79,75,98,136,36,98,222,7,159,113,215,131,99,185,119,228,191,249,172,118,133,167,229,66,99,142,52,137,251,112,89,133,51,109,221,137,19,17,97,224,14,219,115,246,201,223,165,125,135,14,12,253,193,121,136,
239,152,141,75,1,194,109,215,252,148,163,14,29,210,68,176,120,170,176,200,4,28,119,202,53,60,51,110,74,60,190,239,169,67,188,131,234,26,249,104,177,62,193,136,247,81,42,203,83,156,116,252,119,24,254,131,
3,216,186,125,89,163,75,240,101,142,130,230,140,176,54,205,152,113,83,185,250,79,15,240,198,91,179,232,219,179,43,135,29,178,47,253,123,118,166,172,109,134,87,39,191,195,31,111,122,132,122,23,215,203,
212,52,43,158,106,46,1,225,70,165,54,181,54,193,21,191,57,142,159,12,219,159,72,13,98,98,146,190,184,192,86,72,101,120,213,234,168,203,70,156,126,238,237,60,254,204,120,180,168,53,54,126,194,106,77,140,
180,150,4,143,222,243,11,190,177,235,64,31,217,172,194,236,36,206,83,108,168,196,28,189,33,104,34,197,79,126,126,29,15,61,241,90,76,211,81,122,206,141,1,188,26,156,99,240,216,184,168,248,221,67,118,231,
244,19,191,203,192,62,219,96,109,33,2,40,104,136,2,135,176,159,161,250,116,201,18,42,43,219,33,68,197,222,95,135,240,196,51,83,24,126,230,53,132,154,192,104,190,201,80,89,31,106,211,102,197,106,147,103,
204,144,65,125,250,172,177,247,69,163,144,103,95,120,131,80,202,72,137,195,168,207,136,57,147,47,50,34,172,220,113,134,179,207,191,147,127,63,243,74,124,129,110,147,13,97,173,9,130,42,121,106,186,87,123,
154,120,181,171,237,29,103,252,226,171,6,128,18,37,12,39,159,121,53,255,124,98,130,167,75,95,141,97,71,63,183,198,213,248,68,212,183,88,40,60,242,196,171,28,248,189,243,25,246,147,107,121,107,198,194,
152,215,214,120,22,75,159,67,244,33,183,203,211,161,170,28,67,20,159,97,236,216,170,114,240,55,7,211,177,125,69,179,214,46,147,74,49,168,79,159,102,1,165,217,154,165,84,218,87,84,232,146,218,218,85,78,
49,64,36,98,143,93,119,224,156,83,191,199,222,67,6,160,56,2,3,38,10,137,212,251,42,170,144,8,210,180,239,115,100,76,146,92,120,120,194,23,68,111,84,56,91,49,204,152,120,47,237,219,132,158,33,209,218,98,
157,201,107,30,33,138,114,216,100,146,37,203,96,248,105,87,50,238,213,25,228,37,68,156,139,125,40,183,65,26,175,144,25,176,8,223,255,206,62,92,113,241,201,148,151,101,49,234,169,62,214,102,29,85,193,26,
248,245,85,35,185,245,238,199,139,204,83,107,251,188,170,138,10,214,151,142,125,189,179,64,159,214,214,74,175,234,106,74,67,107,193,83,149,142,159,48,157,35,135,95,70,231,62,71,211,99,199,99,184,250,15,
255,36,175,43,67,59,17,131,115,158,210,84,92,12,148,205,208,74,22,33,12,216,125,24,191,253,227,127,248,224,147,220,106,41,127,7,152,68,91,30,251,207,27,236,186,255,201,60,247,202,52,34,242,62,217,171,
54,230,239,221,48,251,40,42,224,44,78,12,15,142,26,199,215,247,59,145,231,255,59,7,85,187,78,109,33,34,168,83,218,150,181,1,19,179,110,175,37,52,238,85,93,189,222,64,217,32,205,82,42,85,177,150,89,253,
207,250,4,118,77,183,173,152,48,230,6,172,228,65,219,224,116,5,169,100,25,149,61,143,140,115,38,155,171,49,168,84,169,43,59,15,232,193,65,251,238,198,246,221,59,146,205,102,153,247,209,10,30,25,245,60,
179,230,46,36,140,64,98,173,184,102,47,104,195,206,165,64,210,104,13,156,117,242,80,126,115,230,247,48,49,0,68,45,70,13,145,201,162,81,192,228,55,103,49,247,131,69,188,244,234,44,238,26,249,84,204,94,
229,62,151,208,168,170,162,130,37,27,0,146,141,10,22,240,143,147,121,119,254,252,70,233,246,66,118,192,0,55,253,238,28,142,249,206,174,4,46,130,64,153,189,48,199,78,251,14,111,81,237,134,18,248,157,106,
10,197,198,216,52,170,248,232,233,139,209,125,130,49,150,132,83,22,76,27,73,66,234,61,83,39,138,74,14,52,133,9,18,236,113,240,89,76,159,243,17,224,112,142,98,157,205,149,156,97,58,153,100,155,173,183,
102,214,252,249,27,229,62,111,180,98,196,204,121,243,100,200,142,59,190,94,94,86,182,234,165,163,214,242,179,95,93,207,173,247,61,203,219,243,151,112,231,67,227,56,234,196,11,91,92,147,161,132,196,201,
66,19,79,71,186,34,85,188,126,17,207,46,147,120,189,136,136,140,242,209,146,101,158,42,54,102,100,48,146,194,224,120,108,244,203,188,243,191,133,62,59,237,34,36,158,169,46,101,138,168,44,47,103,183,1,
3,182,223,88,64,97,83,93,125,239,238,221,245,131,143,63,102,217,138,21,24,140,15,52,92,129,244,198,121,140,74,225,65,14,45,7,50,69,34,50,41,232,195,194,227,93,226,145,147,77,109,45,37,174,204,155,8,52,
193,41,199,237,195,85,23,158,132,209,60,130,33,4,234,115,202,1,71,156,203,204,217,31,196,22,71,139,221,131,170,202,86,149,149,116,106,223,158,183,155,144,100,107,81,210,167,123,119,109,219,166,77,161,
90,82,194,180,211,242,31,211,187,242,81,194,178,153,206,87,212,138,209,159,143,56,82,23,78,25,169,227,159,184,94,47,252,197,241,186,109,151,246,197,103,98,75,201,251,203,203,202,190,28,207,119,30,208,
179,103,9,104,182,28,77,62,196,168,177,162,86,208,192,88,53,88,21,177,42,198,20,223,147,73,167,245,107,219,111,223,250,65,82,42,251,12,30,220,99,251,109,182,209,54,233,180,26,217,242,0,112,154,248,196,
248,198,135,95,55,99,68,219,164,211,90,211,185,139,238,53,104,208,41,124,217,165,115,135,14,154,46,60,77,126,203,209,164,35,157,76,106,231,14,29,54,155,22,217,236,78,80,223,154,26,93,188,116,41,117,245,
245,45,135,170,172,5,73,38,149,162,44,147,161,99,101,37,239,172,210,109,255,149,3,75,169,12,238,215,79,23,44,90,68,93,125,61,43,26,26,90,6,117,217,23,44,214,24,218,164,211,148,101,50,116,235,212,137,215,
215,208,226,184,69,74,100,207,65,131,174,174,238,220,89,171,202,203,53,153,72,124,169,205,75,50,145,208,170,242,114,173,238,220,89,247,28,52,232,234,150,122,79,90,13,106,251,116,239,174,75,106,107,201,
230,243,228,226,163,53,106,30,107,12,201,68,130,100,34,65,42,145,160,170,162,130,25,115,231,182,138,251,208,170,85,220,192,158,61,117,201,178,101,100,115,185,34,136,178,249,252,106,108,214,155,67,140,
49,164,74,64,145,74,38,169,42,47,231,205,89,179,90,237,154,127,233,236,225,222,59,239,188,255,226,165,75,159,249,172,174,142,108,46,71,20,69,68,206,225,10,135,106,241,117,20,63,248,169,201,139,37,130,
53,198,63,216,42,110,68,55,37,175,173,181,164,146,73,218,149,149,209,177,178,242,128,177,147,38,141,249,50,173,237,87,218,121,218,107,231,157,79,201,229,114,183,53,228,114,69,173,148,203,231,253,211,93,
69,138,90,33,153,72,144,78,38,73,38,147,35,198,77,154,116,251,22,143,114,139,108,145,117,200,255,3,156,83,60,176,165,50,226,130,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PizButton::pirate_png = (const char*) resource_PizButton_pirate_png;
const int PizButton::pirate_pngSize = 8193;