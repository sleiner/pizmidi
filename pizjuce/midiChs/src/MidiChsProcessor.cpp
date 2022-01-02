#include "MidiChsProcessor.h"
#include "MidiChsEditor.h"


//==============================================================================
/**
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
PizAudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiChsProcessor();
}

//==============================================================================
MidiChsProcessor::MidiChsProcessor()
{
	if (!loadDefaultFxb())
	{
		lastUIWidth = 330;
		lastUIHeight = 126;

		fChannel1  =  1.0 * 0.0625;
		fChannel2  =  2.0 * 0.0625;
		fChannel3  =  3.0 * 0.0625;
		fChannel4  =  4.0 * 0.0625;
		fChannel5  =  5.0 * 0.0625;
		fChannel6  =  6.0 * 0.0625;
		fChannel7  =  7.0 * 0.0625;
		fChannel8  =  8.0 * 0.0625;
		fChannel9  =  9.0 * 0.0625;
		fChannel10 = 10.0 * 0.0625;
		fChannel11 = 11.0 * 0.0625;
		fChannel12 = 12.0 * 0.0625;
		fChannel13 = 13.0 * 0.0625;
		fChannel14 = 14.0 * 0.0625;
		fChannel15 = 15.0 * 0.0625;
		fChannel16 = 16.0 * 0.0625;
		fReset     = 0.0;
		fClear     = 0.0;

		bghue      = 0.0;
		bgsat      = 0.0;
		bgbri      = 0.0;
		contrast   = 1.0;
	}

}

MidiChsProcessor::~MidiChsProcessor()
{
}

//==============================================================================
int MidiChsProcessor::getNumParameters()
{
    return 22;
}

float MidiChsProcessor::getParameter (int index)
{
    switch (index) {
        case  0: return fChannel1; break;
        case  1: return fChannel2; break;
        case  2: return fChannel3; break;
        case  3: return fChannel4; break;
        case  4: return fChannel5; break;
        case  5: return fChannel6; break;
        case  6: return fChannel7; break;
        case  7: return fChannel8; break;
        case  8: return fChannel9; break;
        case  9: return fChannel10; break;
        case 10: return fChannel11; break;
        case 11: return fChannel12; break;
        case 12: return fChannel13; break;
        case 13: return fChannel14; break;
        case 14: return fChannel15; break;
        case 15: return fChannel16; break;
        case 16: return fReset; break;
        case 17: return fClear; break;

        case 18: return bghue; break;
        case 19: return bgsat; break;
        case 20: return bgbri; break;
        case 21: return contrast; break;

        default: return 0.0f; break;
    }
}

void MidiChsProcessor::setParameter (int index, float newValue)
{
    switch (index) {
        case  0: fChannel1 = newValue; break;
        case  1: fChannel2 = newValue; break;
        case  2: fChannel3 = newValue; break;
        case  3: fChannel4 = newValue; break;
        case  4: fChannel5 = newValue; break;
        case  5: fChannel6 = newValue; break;
        case  6: fChannel7 = newValue; break;
        case  7: fChannel8 = newValue; break;
        case  8: fChannel9 = newValue; break;
        case  9: fChannel10 = newValue; break;
        case 10: fChannel11 = newValue; break;
        case 11: fChannel12 = newValue; break;
        case 12: fChannel13 = newValue; break;
        case 13: fChannel14 = newValue; break;
        case 14: fChannel15 = newValue; break;
        case 15: fChannel16 = newValue; break;
        case 16: fReset = newValue; break;
        case 17: fClear = newValue; break;
        case 18: bghue = newValue; break;
        case 19: bgsat = newValue; break;
        case 20: bgbri = newValue; break;
        case 21: contrast = newValue; break;

        default: break;
    }
    sendChangeMessage ();
}

const String MidiChsProcessor::getParameterName (int index)
{
    switch (index) {
        case  0: return String("1"); break;
        case  1: return String("2"); break;
        case  2: return String("3"); break;
        case  3: return String("4"); break;
        case  4: return String("5"); break;
        case  5: return String("6"); break;
        case  6: return String("7"); break;
        case  7: return String("8"); break;
        case  8: return String("9"); break;
        case  9: return String("10"); break;
        case 10: return String("11"); break;
        case 11: return String("12"); break;
        case 12: return String("13"); break;
        case 13: return String("14"); break;
        case 14: return String("15"); break;
        case 15: return String("16"); break;
        case 16: return String("Reset"); break;
        case 17: return String("Clear"); break;
        case 18: return String("Hue"); break;
        case 19: return String("Saturation"); break;
        case 20: return String("Brightness"); break;
        case 21: return String("Contrast"); break;

        default: return String(); break;
    }
}

const String MidiChsProcessor::getParameterText (int index)
{
    switch (index) {
        case 0: return String((int)(fChannel1*16.0)); break;
        case 1: return String((int)(fChannel2*16.0)); break;
        case 2: return String((int)(fChannel3*16.0)); break;
        case 3: return String((int)(fChannel4*16.0)); break;
        case 4: return String((int)(fChannel5*16.0)); break;
        case 5: return String((int)(fChannel6*16.0)); break;
        case 6: return String((int)(fChannel7*16.0)); break;
        case 7: return String((int)(fChannel8*16.0)); break;
        case 8: return String((int)(fChannel9*16.0)); break;
        case 9: return String((int)(fChannel10*16.0)); break;
        case 10: return String((int)(fChannel11*16.0)); break;
        case 11: return String((int)(fChannel12*16.0)); break;
        case 12: return String((int)(fChannel13*16.0)); break;
        case 13: return String((int)(fChannel14*16.0)); break;
        case 14: return String((int)(fChannel15*16.0)); break;
        case 15: return String((int)(fChannel16*16.0)); break;
        case 16: return String(fReset); break;
        case 17: return String(fClear); break;
        case 18: return String(bghue); break;
        case 19: return String(bgsat); break;
        case 20: return String(bgbri); break;
        case 21: return String(contrast); break;

        default: return String(); break;
    }

}
const String MidiChsProcessor::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MidiChsProcessor::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MidiChsProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MidiChsProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}


//==============================================================================
void MidiChsProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
}

void MidiChsProcessor::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MidiChsProcessor::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    MidiBuffer::Iterator mid_buffer_iter(midiMessages);
    MidiMessage midi_message(0);
    MidiBuffer midiout;
    int sample_number;
    bool discard = false;
    while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
        MidiMessage out_message = midi_message;
        short ch[16];
	    ch[0]  = (short int)(fChannel1*16);
	    ch[1]  = (short int)(fChannel2*16);
	    ch[2]  = (short int)(fChannel3*16);
	    ch[3]  = (short int)(fChannel4*16);
	    ch[4]  = (short int)(fChannel5*16);
	    ch[5]  = (short int)(fChannel6*16);
	    ch[6]  = (short int)(fChannel7*16);
	    ch[7]  = (short int)(fChannel8*16);
	    ch[8]  = (short int)(fChannel9*16);
	    ch[9]  = (short int)(fChannel10*16);
	    ch[10] = (short int)(fChannel11*16);
	    ch[11] = (short int)(fChannel12*16);
	    ch[12] = (short int)(fChannel13*16);
	    ch[13] = (short int)(fChannel14*16);
	    ch[14] = (short int)(fChannel15*16);
	    ch[15] = (short int)(fChannel16*16);

        if (midi_message.getChannel()>0) {
            for (int i=0;i<16;i++) {
                if (midi_message.isForChannel(i+1)) {
                    if (ch[i] > 0) out_message.setChannel(ch[i]);
                    else discard=true;
                }
            }
        }
        if (!discard) midiout.addEvent(out_message,sample_number);
    }
    midiMessages.clear();
    midiMessages = midiout;
}

//==============================================================================
AudioProcessorEditor* MidiChsProcessor::createEditor()
{
    return new MidiChsEditor (this);
}

//==============================================================================
void MidiChsProcessor::getStateInformation (MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    XmlElement xmlState ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xmlState.setAttribute ("1", fChannel1);
    xmlState.setAttribute ("2", fChannel2);
    xmlState.setAttribute ("3", fChannel3);
    xmlState.setAttribute ("4", fChannel4);
    xmlState.setAttribute ("5", fChannel5);
    xmlState.setAttribute ("6", fChannel6);
    xmlState.setAttribute ("7", fChannel7);
    xmlState.setAttribute ("8", fChannel8);
    xmlState.setAttribute ("9", fChannel9);
    xmlState.setAttribute ("10", fChannel10);
    xmlState.setAttribute ("11", fChannel11);
    xmlState.setAttribute ("12", fChannel12);
    xmlState.setAttribute ("13", fChannel13);
    xmlState.setAttribute ("14", fChannel14);
    xmlState.setAttribute ("15", fChannel15);
    xmlState.setAttribute ("16", fChannel16);
    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("Hue", bghue);
    xmlState.setAttribute ("Sat", bgsat);
    xmlState.setAttribute ("Bri", bgbri);
    xmlState.setAttribute ("Contrast", contrast);
    xmlState.setAttribute ("uiWidth", lastUIWidth);
    xmlState.setAttribute ("uiHeight", lastUIHeight);

    // you could also add as many child elements as you need to here..


    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}

void MidiChsProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            fChannel1  = (float) xmlState->getDoubleAttribute ("1",  fChannel1 );
            fChannel2  = (float) xmlState->getDoubleAttribute ("2",  fChannel2 );
            fChannel3  = (float) xmlState->getDoubleAttribute ("3",  fChannel3 );
            fChannel4  = (float) xmlState->getDoubleAttribute ("4",  fChannel4 );
            fChannel5  = (float) xmlState->getDoubleAttribute ("5",  fChannel5 );
            fChannel6  = (float) xmlState->getDoubleAttribute ("6",  fChannel6 );
            fChannel7  = (float) xmlState->getDoubleAttribute ("7",  fChannel7 );
            fChannel8  = (float) xmlState->getDoubleAttribute ("8",  fChannel8 );
            fChannel9  = (float) xmlState->getDoubleAttribute ("9",  fChannel9 );
            fChannel10 = (float) xmlState->getDoubleAttribute ("10", fChannel10);
            fChannel11 = (float) xmlState->getDoubleAttribute ("11", fChannel11);
            fChannel12 = (float) xmlState->getDoubleAttribute ("12", fChannel12);
            fChannel13 = (float) xmlState->getDoubleAttribute ("13", fChannel13);
            fChannel14 = (float) xmlState->getDoubleAttribute ("14", fChannel14);
            fChannel15 = (float) xmlState->getDoubleAttribute ("15", fChannel15);
            fChannel16 = (float) xmlState->getDoubleAttribute ("16", fChannel16);
            bghue = (float) xmlState->getDoubleAttribute ("Hue", bghue);
            bgsat = (float) xmlState->getDoubleAttribute ("Sat", bgsat);
            bgbri = (float) xmlState->getDoubleAttribute ("Bri", bgbri);
            contrast = (float) xmlState->getDoubleAttribute ("Contrast", contrast);

            lastUIWidth = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);

            sendChangeMessage ();
        }
    }
}
