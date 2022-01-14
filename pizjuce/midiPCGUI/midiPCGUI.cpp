#include "midiPCGUI.h"
#include "midiPCGUIEditor.h"
#include "../_common/MIDI.h"



//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new midiPCGUI();
}

midiPCGUIProgram::midiPCGUIProgram ()
{
    //default values
	for (int i=0;i<numParams;i++) param[i]=0.f;
	param[kMode]=1.f;
	param[kThru]=1.f;
	param[kTrigger]=0.4f;
	param[kBankTrigger]=0.4f;
	param[kInc]=0.4f;
	param[kDec]=0.4f;

    //program name
	name = "Default";
}

//==============================================================================
midiPCGUI::midiPCGUI()
{
    programs = new midiPCGUIProgram[getNumPrograms()];

    if (!loadDefaultFxb())
	{
		for (int i=0;i<128;i++) {
			programs[i].setName("Program " + String(i+1));
		}
	}

    init=true;
    setCurrentProgram (0);

	trigger=false;
	triggerbank=false;
	inc=false;
	dec=false;
	program=0;
	bankmsb=0;
	banklsb=0;
	sentinc=false;
	sentdec=false;
	senttrig=false;
	sentbank=false;
    for (int i=0;i<numParams;i++) automated[i]=false;
    mode=continuous;

	wait=false;
	delaytime=(int)(getSampleRate()*0.002f);
	counter=0;
	triggerdelta=0;
	for (int i=0;i<16;i++) {
		actualProgram[i]=1;
		actualBankMSB[i]=1;
		actualBankLSB[i]=1;
	}
}

midiPCGUI::~midiPCGUI()
{
	if(programs) delete[] programs;
}

//==============================================================================
int midiPCGUI::getNumParameters()
{
    return numParams;
}

float midiPCGUI::getParameter (int index)
{
    return param[index];
}

void midiPCGUI::setParameter (int index, float newValue)
{
    midiPCGUIProgram* ap = &programs[curProgram];
	switch(index)
	{
	case kMode:
		if (newValue<0.5f) mode=continuous;
		else mode=triggered;
		param[index] = ap->param[index] = newValue;
		break;
	case kPCListen:
		pclisten = newValue>=0.5f;
		param[index] = ap->param[index] = newValue;
		break;
	case kThru:
		thru = newValue>=0.5f;
		param[index] = ap->param[index] = newValue;
		break;
	case kTrigger:
		param[index]=newValue;
		if (newValue==1.f){// && !senttrig) {
			trigger=true;
			senttrig=true;
		}
		else if (newValue<1.f && senttrig) senttrig=false;
		break;
	case kBankTrigger:
		param[index]=newValue;
		if (newValue==1.f){// && !sentbank) {
			triggerbank=true;
			sentbank=true;
		}
		else if (newValue<1.f && sentbank) sentbank=false;
		break;
	case kInc:
		param[index]=newValue;
		if (newValue==1.f){// && !sentinc) {
			inc=true;
			sentinc=true;
		}
		else if (newValue<1.f && sentinc) sentinc=false;
		break;
	case kDec:
		param[index]=newValue;
		if (newValue==1.f){// && !sentdec) {
			dec=true;
			sentdec=true;
		}
		else if (newValue<1.f && sentdec) sentdec=false;
		break;
	case kProgram:
			program=FLOAT_TO_MIDI2(newValue);
		if (mode==continuous && !automated[index]) trigger=true;
		param[index] = ap->param[index] = newValue;
		break;
	case kBankMSB:
			bankmsb=FLOAT_TO_MIDI2(newValue);
		if (mode==continuous) triggerbank=true;
		param[index] = ap->param[index] = newValue;
		break;
	case kBankLSB:
			banklsb=FLOAT_TO_MIDI2(newValue);
		if (mode==continuous) triggerbank=true;
		param[index] = ap->param[index] = newValue;
		break;
	default:
			param[index] = ap->param[index] = newValue;
			break;
	}
    if (index==kMode || index>kBankTrigger) {
        for (int p=0;p<getNumPrograms();p++) {
            programs[p].param[index]=param[index];
        }
    }
    sendChangeMessage();
}

const String midiPCGUI::getParameterName (int index)
{
   switch(index){
      case kProgram: return "Program"; break;
      case kBankMSB: return "Bank MSB"; break;
      case kBankLSB: return "Bank LSB"; break;
      case kMode: return "Mode"; break;
      case kTrigger: return "PC Trig."; break;
      case kBankTrigger: return "Bank Trig."; break;
      case kInc: return "PC Incr."; break;
      case kDec: return "PC Decr."; break;
      case kChannel: return "Channel"; break;
      case kPCListen: return "PC Listen"; break;
      case kThru: return "Thru"; break;
	  default: return String(); break;
   }
}

const String midiPCGUI::getParameterText (int index)
{
	char* text;
	text = new char[24];
	switch(index){
	  case kMode:
		if (mode==continuous) strcpy(text, "Direct");
		else strcpy(text, "Triggered");
		break;
	  case kPCListen:
		if (pclisten) strcpy(text, "Yes");
		else strcpy(text, "No");
		break;
	  case kThru:
		if (thru) strcpy(text, "Yes");
		else strcpy(text, "No");
		break;
	  case kProgram:
           if (program==0) strcpy(text, "Off");
           else sprintf(text, "%d", program);
           break;
	  case kBankMSB:
           if (bankmsb==0) strcpy(text, "Off");
           else sprintf(text, "%d", bankmsb);
           break;
	  case kBankLSB:
           if (banklsb==0) strcpy(text, "Off");
           else sprintf(text, "%d", banklsb);
           break;
	  case kChannel: sprintf(text, "%d",FLOAT_TO_CHANNEL015(param[index])+1); break;
	  case kTrigger:
		  if (param[index]==1.f) strcpy(text, "Triggered!");
		  else strcpy(text, "Trigger-->");
		  break;
      case kBankTrigger:
		  if (param[index]<1.f) strcpy(text, "Trigger-->");
		  else strcpy(text, "Triggered!");
		  break;
	  case kInc:
		  if (param[index]<1.f) strcpy(text, "Trigger-->");
		  else strcpy(text, "Triggered!");
		  break;
	  case kDec:
		  if (param[index]<1.f) strcpy(text, "Trigger-->");
		  else strcpy(text, "Triggered!");
		  break;
	  default:
           sprintf(text, "%d", roundToInt(param[index]*100.0f));
           break;
   }
   return String(text);
}

const String midiPCGUI::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String midiPCGUI::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool midiPCGUI::isInputChannelStereoPair (int index) const
{
    if (getNumInputChannels()==2) return true;
    else return false;
}

bool midiPCGUI::isOutputChannelStereoPair (int index) const
{
    if (getNumOutputChannels()==2) return true;
    else return false;
}
void midiPCGUI::setCurrentProgram (int index)
{
	midiPCGUIProgram* ap = &programs[index];
    curProgram = index;
    for (int i=0;i<getNumParameters();i++) {
        setParameter(i, ap->param[i]);
    }
    if (program>0) trigger=true;
    if (banklsb>0 || bankmsb>0) triggerbank=true;
}

void midiPCGUI::changeProgramName(int index, const String &newName) {
    programs[curProgram].name = newName;
}

const String midiPCGUI::getProgramName(int index) {
    return programs[index].name;
}

int midiPCGUI::getCurrentProgram() {
    return curProgram;
}
//==============================================================================
AudioProcessorEditor* midiPCGUI::createEditor()
{
    return new midiPCGUIEditor (this);
}
//==============================================================================
void midiPCGUI::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
	delaytime=(int)(sampleRate*0.002f);
}

void midiPCGUI::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void midiPCGUI::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    for (int i = 0; i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

	int listenchannel = FLOAT_TO_CHANNEL015(param[kChannel]);
	MidiBuffer output;

	for(auto&& msgMetadata : midiMessages)
	{
		auto midi_message = msgMetadata.getMessage();

		bool discard = !thru;
		const uint8* midiData = midi_message.getRawData();
		unsigned char status     = midiData[0] & 0xf0;   // scraping  channel
		char channel    = midiData[0] & 0x0f;   // isolating channel (0-15)
		char data1      = midiData[1] & 0x7f;
		char data2	    = midiData[2] & 0x7f;

		//only look at the selected channel
		if (channel == listenchannel)
		{
			if (status==MIDI_PROGRAMCHANGE) {
				if (pclisten) {
					setCurrentProgram(data1);
					updateHostDisplay();
					discard=true;
				}
				else if (thru) {
					actualProgram[channel]=data1+1;
					sendChangeMessage();
				}
			}
			else if (status==MIDI_CONTROLCHANGE && thru) {
				if (data1==MIDI_BANK_CHANGE) {
					actualBankMSB[channel]=data2+1;
					sendChangeMessage();
				}
				else if (data1==0x20) {
					actualBankLSB[channel]=data2+1;
					sendChangeMessage();
				}
			}
    	} // if listenchannel==channel
		if (!discard) output.addEvent(midi_message,msgMetadata.samplePosition);
     } //for() inputs loop

	if (triggerbank) {
        if (!(trigger && program!=0)) triggerbank=false;
		//create GUI triggered message
		if (bankmsb!=0) {
    		MidiMessage msb(MIDI_CONTROLCHANGE | listenchannel,MIDI_BANK_CHANGE,bankmsb-1,0);
			output.addEvent(msb,0);
			actualBankMSB[listenchannel]=bankmsb;
        }
        if (banklsb!=0) {
    		MidiMessage lsb(MIDI_CONTROLCHANGE | listenchannel,0x20,banklsb-1,0);
			output.addEvent(lsb,0);
			actualBankLSB[listenchannel]=banklsb;
        }
		setParameterNotifyingHost(kBankTrigger,0.4f);
	}
	if (trigger) {
		trigger=false;
        if(program!=0) {
			actualProgram[listenchannel]=program;
            if (triggerbank) {
                //delay program change if bank was sent
                wait=true;
                triggerbank=false;
				if (buffer.getNumSamples()>delaytime) {
                    wait=false;
                    counter=0;
					MidiMessage progch = MidiMessage::programChange(listenchannel+1,program-1);
					output.addEvent(progch,delaytime);
					setParameterNotifyingHost(kTrigger,0.4f);
                }
            }
            else {
        		//create GUI triggered message
				output.addEvent(MidiMessage::programChange(listenchannel+1,program-1),triggerdelta);
				setParameterNotifyingHost(kTrigger,0.4f);
            }
        }
	}
	else if (inc) {
		//create GUI triggered message
		inc=false;
		program=actualProgram[listenchannel];
		++program;
		if (program>128) program=1;
		if (program!=0) {
			output.addEvent(MidiMessage::programChange(listenchannel+1,program-1),0);
			actualProgram[listenchannel]=program;
        }
		setParameterNotifyingHost(kProgram,MIDI_TO_FLOAT2(program));
		setParameterNotifyingHost(kInc,0.4f);
	}
	else if (dec) {
		//create GUI triggered message
		dec=false;
		program=actualProgram[listenchannel];
		--program;
		if (program<1) program=128;
		if (program!=0) {
			output.addEvent(MidiMessage::programChange(listenchannel+1,program-1),0);
			actualProgram[listenchannel]=program;
        }
		setParameterNotifyingHost(kProgram,MIDI_TO_FLOAT2(program));
		setParameterNotifyingHost(kDec,0.4f);
	}

	midiMessages.clear();
	midiMessages = output;
}

//==============================================================================
void midiPCGUI::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    XmlElement xmlState ("MIDIPCGUISETTINGS");

    // add some attributes to it..
    xmlState.setAttribute ("pluginVersion", 2);

    xmlState.setAttribute ("program", getCurrentProgram());
    xmlState.setAttribute ("progname", getProgramName(getCurrentProgram()));
    for (int i=0;i<getNumParameters();i++)
        xmlState.setAttribute (String(i), param[i]);

	XmlElement* names = new XmlElement("names");
	for (int i=0;i<progNames.names.size();i++) {
		XmlElement* name = new XmlElement("name");
		name->setAttribute("c",progNames.names[i].channel);
		name->setAttribute("b",progNames.names[i].bank);
		name->setAttribute("p",progNames.names[i].program);
		name->setAttribute("n",progNames.names[i].name);
		names->addChildElement(name);
	}
	xmlState.addChildElement(names);


    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}

void midiPCGUI::getStateInformation(MemoryBlock &destData) {
    // make sure the non-parameter settings are copied to the current program

    XmlElement xmlState ("MIDIPCGUISETTINGS");
    xmlState.setAttribute ("pluginVersion", 2);
    xmlState.setAttribute ("program", getCurrentProgram());
    for (int p=0;p<getNumPrograms();p++) {
        String prefix = "P" + String(p) + "_";
        xmlState.setAttribute (prefix+"progname", programs[p].name);
        for (int i=0;i<getNumParameters();i++) {
            xmlState.setAttribute (prefix+String(i), programs[p].param[i]);
        }
    }

	XmlElement* names = new XmlElement("names");
	for (int i=0;i<progNames.names.size();i++) {
		XmlElement* name = new XmlElement("name");
		name->setAttribute("c",progNames.names[i].channel);
		name->setAttribute("b",progNames.names[i].bank);
		name->setAttribute("p",progNames.names[i].program);
		name->setAttribute("n",progNames.names[i].name);
		names->addChildElement(name);
	}
	xmlState.addChildElement(names);

	copyXmlToBinary (xmlState, destData);
}

void midiPCGUI::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MIDIPCGUISETTINGS"))
        {
            // ok, now pull out our parameters..
            changeProgramName(getCurrentProgram(),xmlState->getStringAttribute ("progname", "Default"));
            for (int i=0;i<getNumParameters();i++) {
                param[i] = (float) xmlState->getDoubleAttribute (String(i), param[i]);
            }
			XmlElement* n = xmlState->getChildByName("names");
			if (n) {
				for (auto *e : xmlState->getChildIterator()) {
					progNames.setNameFor(e->getIntAttribute("c"),
										 e->getIntAttribute("b"),
										 e->getIntAttribute("p"),
										 e->getStringAttribute("n"));
				}
			}
            sendChangeMessage ();
        }
    }
}

void midiPCGUI::setStateInformation (const void* data, int sizeInBytes) {
    auto xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        if (xmlState->hasTagName ("MIDIPCGUISETTINGS"))
        {
            for (int p=0;p<getNumPrograms();p++) {
				String prefix;
				if (xmlState->getIntAttribute("pluginVersion")<2)
					prefix = "P" + String(p) + ".";
				else prefix = "P" + String(p) + "_";
                for (int i=0;i<getNumParameters();i++) {
                    programs[p].param[i] = (float) xmlState->getDoubleAttribute (prefix+String(i), programs[p].param[i]);
                }
                programs[p].name = xmlState->getStringAttribute (prefix+"progname", programs[p].name);
            }
			XmlElement* n = xmlState->getChildByName("names");
			if (n) {
				for (auto *e : xmlState->getChildIterator()) {
					progNames.setNameFor(e->getIntAttribute("c"),
										 e->getIntAttribute("b"),
										 e->getIntAttribute("p"),
										 e->getStringAttribute("n"));
				}
			}
            init=true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
}
