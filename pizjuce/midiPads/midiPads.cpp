#include "midiPads.h"
#include "midiPadsEditor.h"

//==============================================================================
PizAudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new midiPads();
}

//==============================================================================
MidiPadsPrograms::MidiPadsPrograms ()
: values_("midiPadsValues")
{
    //default values
    for (int p=0;p<numPrograms;p++)
    {
        ValueTree progv("ProgValues");
        progv.setProperty("progIndex",p,0);
        for (int i=0;i<numPads;i++) {
            x[p][i]=0.5f;
            y[p][i]=0.5f;
            ValueTree pv("PadValues");
            pv.setProperty("padIndex",i,0);
            pv.setProperty("Ydata1",60,0); //note
            pv.setProperty("Ycc",i,0); //y cc
            pv.setProperty("Ytype",0,0); //0==note, 1==cc, 2==program change???
            pv.setProperty("Ydata2",127,0); //data2
            pv.setProperty("Yoff",0,0); //y off value
            pv.setProperty("trigger",0,0); //trigger note
            pv.setProperty("Xcc",1,0); //x cc
            pv.setProperty("Xoff",64,0); //x off value
            pv.setProperty("UseX",false,0); // use x-position?
            pv.setProperty("UseY",false,0); // use y-position?
            pv.setProperty("UseYCC",false,0); // (don't) send y-cc with note
            pv.setProperty("UseXPB",false,0); // (don't) use x as pitch bend
            pv.setProperty("toggle",false,0); // toggle mode off
            pv.setProperty("SendOff",false,0); // send off value

            pv.setProperty("icon",String(i+1)+".svg",0); //icon filename
            pv.setProperty("text","Pad " + String(i+1),0); //pad name
            pv.setProperty("padcolor",Colour(0xFF000000).toString(),0);
            pv.setProperty("roundness",0.4f,0);
            pv.setProperty("iconsize",0.5f,0);
            pv.setProperty("centeredText",false,0);

            pv.setProperty("lastx",64,0);
            pv.setProperty("lasty",0.0f,0);
            pv.setProperty("togglestate",false,0);
            pv.setProperty("showdots",true,0);
            pv.setProperty("showvalues",true,0);

            progv.addChild(pv,i,0);
        }

        ValueTree gv("GlobalValues");
        gv.setProperty("VelOffset",0.5f,0); // velocity offset
        gv.setProperty("CCOffset",0.5f,0); // cc value offset
        gv.setProperty("ChIn",0.0f/15.0f,0); // in channel
        gv.setProperty("ChOut",0.0f/15.0f,0); // out channel
        gv.setProperty("UseTrigger",0.0f,0); // use triggering?
        gv.setProperty("NoteOnTrig",0.0f,0); // "piezo trigger mode"
        gv.setProperty("UseVel",1.0f,0); // use trigger velocity?
        gv.setProperty("Thru",1.f,0); // midi thru?
        gv.setProperty("UseAft",0.f,0); // send aftertouch?
        gv.setProperty("Mono",0.f,0); // mono mode

        //default GUI size
        gv.setProperty("lastUIWidth",400,0);
        gv.setProperty("lastUIHeight",400,0);

        //colors, etc
        gv.setProperty("bghue",0.1f,0);
        gv.setProperty("bgsat",0.1f,0);
        gv.setProperty("bgbri",0.8f,0);
        gv.setProperty("contrast",0.3f,0);
        gv.setProperty("squares",16,0);
        gv.setProperty("editmode",true,0);
        gv.setProperty("usemouseup",true,0);
        gv.setProperty("hex",false,0);

        //program name
        gv.setProperty("name","Default",0);

        progv.addChild(gv,numPads,0);

        ValueTree layout("Layout");
        for (int i=0;i<numPads;i++)
        {
            ValueTree padpos("PadPosition");
            padpos.setProperty("visible",i<(int)gv.getProperty("squares"),0);
            padpos.setProperty("x",0.f,0);
            padpos.setProperty("y",0.f,0);
            padpos.setProperty("w",0.1f,0);
            padpos.setProperty("h",0.1f,0);
            layout.addChild(padpos,i,0);
        }
        progv.addChild(layout,numPads+1,0);

        this->values_.addChild(progv,p,0);
    }
}

//==============================================================================
midiPads::midiPads()
: programs(0), layouts(0)
{
    pluginPath = getCurrentPath() + File::getSeparatorString() + "midiPads";
    layoutPath = pluginPath + File::getSeparatorString() + "midiPadLayouts";
    presetPath = pluginPath + File::getSeparatorString() + "midiPadPresets";
    bankPath   = pluginPath + File::getSeparatorString() + "midiPadBanks";
    iconPath   = pluginPath + File::getSeparatorString() + "midiPadIcons";

    layouts = new PadLayouts();
    fillLayouts();
    programs = new MidiPadsPrograms();

    if (!loadXmlBank(File(pluginPath + File::getSeparatorString()
        + File::getSpecialLocation(File::currentApplicationFile).getFileNameWithoutExtension() + ".mpadb")))
    {
        if (!loadFxb(File(pluginPath + File::getSeparatorString()
            + File::getSpecialLocation(File::currentApplicationFile).getFileNameWithoutExtension() + ".fxb")))
        {
            loadDefaultPrograms();
        }
    }

    init = true;
    setCurrentProgram (0);

    //initialize temporary parameters
    triggervel = 0;
    for (int i=0;i<128;i++)
        isplaying[i]=false;
    for (int i=0;i<numPads;i++) {
        buttondown[i] = false;
        midilisten[i] = 0.0f;
        triggered[i] = false;
        send[i] = 0.0f;
        sendoff[i] = 0.0f;
        lastx[i] = programs->getForPad(curProgram,i,"lastx");
        lasty[i] = SendOff[i] ? Yoff[i] : (int)programs->getForPad(curProgram,i,"lasty");
        lastxccvalue[i] = -1;
        lastyccvalue[i] = -1;
        dotmoved[i] = false;
    }
}

midiPads::~midiPads()
{
    if (programs) deleteAndZero(programs);
    if (layouts) deleteAndZero(layouts);
}

//==============================================================================
float midiPads::getParameter (int index) { return param[index]; }

void midiPads::setParameter (int index, float newValue)
{
    //if (param[index] != newValue) {
        param[index] = newValue;
        if (index<kNumGlobalParams)
            programs->set(curProgram,getGlobalParamValueName(index),newValue);
        else if (index>=ypos) {
            programs->y[curProgram][index-ypos] = newValue;
            buttondown[index-ypos] = true;
            dotmoved[index-ypos] = true;
        }
        else {
            programs->x[curProgram][index-xpos] = newValue;
            buttondown[index-xpos] = true;
            dotmoved[index-xpos] = true;
        }
        sendChangeMessage();
    //}
}

const String midiPads::getParameterName (int index)
{
    for (int i=0;i<numPads;i++) {
        if (index == i+xpos) return "x pos " + String(i+1);
        else if (index == i+ypos) return "y pos " + String(i+1);
    }
    if      (index == kVelOffset) return "velocity";
    else if (index == kCCOffset) return "ccvalue";
    else if (index == kChOut) return "out channel";
    else if (index == kMono) return "mono mode";
    else if (index == kUseTrigger) return "use trigger";
    else if (index == kNoteOnTrig) return "ext noteon trig";
    else if (index == kUseVel) return "use trig vel";
    else if (index == kChIn) return "in channel";
    else if (index == kThru) return "midi thru";
    else if (index == kSendAft) return "send aftertouch";
    else return String();
}

const String midiPads::getParameterText (int index)
{
    if (index<getNumParameters())
        return String(param[index],3);
    return String();
}

const String midiPads::getInputChannelName (const int channelIndex) const
{
    if (channelIndex<getNumInputChannels())
        return String(JucePlugin_Name) + String(channelIndex + 1);
    return String();
}

const String midiPads::getOutputChannelName (const int channelIndex) const
{
    if (channelIndex<getNumOutputChannels())
        return String(JucePlugin_Name) + String(channelIndex + 1);
    return String();
}

bool midiPads::isInputChannelStereoPair (int index) const
{
    return false;
}

bool midiPads::isOutputChannelStereoPair (int index) const
{
    return false;
}


//======================Programs================================================
void midiPads::setCurrentProgram (int index)
{
    //save non-parameter info to the old program, except the first time
    if (!init)
        copySettingsToProgram(curProgram);
    else
        init = false;

    //then set the new program
    curProgram = index;
    programs->values_.setProperty("lastProgram",index,0);
    for (int i=0;i<kNumGlobalParams;i++) {
        param[i] = programs->get(index,getGlobalParamValueName(i));
    }

    squares = programs->get(index,"squares");
    lastUIWidth = programs->get(index,"lastUIWidth");
    lastUIHeight = programs->get(index,"lastUIHeight");
    for (int i=0;i<numPads;i++) {
        param[xpos+i] = programs->x[index][xpos+i];
        param[ypos+i] = programs->y[index][ypos+i];
        icon[i] = programs->getForPad(index,i,"icon");
        text[i] = programs->getForPad(index,i,"text");
        padcolor[i] = Colour::fromString(programs->getForPad(index,i,"padcolor").toString());
        Ydata1[i] = programs->getForPad(index,i,"Ydata1");
        Ycc[i] = programs->getForPad(index,i,"Ycc");
        Ytype[i] = programs->getForPad(index,i,"Ytype");
        Ydata2[i] = programs->getForPad(index,i,"Ydata2");
        Yoff[i] = programs->getForPad(index,i,"Yoff");
        Xoff[i] = programs->getForPad(index,i,"Xoff");
        trigger[i] = programs->getForPad(index,i,"trigger");
        Xcc[i] = programs->getForPad(index,i,"Xcc");
        SendOff[i] = programs->getForPad(index,i,"SendOff");
        UseX[i] = programs->getForPad(index,i,"UseX");
        UseY[i] = programs->getForPad(index,i,"UseY");
        UseYCC[i] = programs->getForPad(index,i,"UseYCC");
        UseXPB[i] = programs->getForPad(index,i,"UseXPB");
        lastx[i] = programs->getForPad(index,i,"lastx");
        lasty[i] = programs->getForPad(index,i,"lasty");
        toggle[i] = programs->getForPad(index,i,"toggle");
        togglestate[i] = programs->getForPad(index,i,"togglestate");
        showdots[i] = programs->getForPad(index,i,"showdots");
        showvalues[i] = programs->getForPad(index,i,"showvalues");
        roundness[i] = programs->getForPad(index,i,"roundness");
        iconsize[i] = programs->getForPad(index,i,"iconsize");
        centeredText[i] = programs->getForPad(index,i,"centeredText");
    }
    bghue = programs->get(index,"bghue");
    bgsat = programs->get(index,"bgsat");
    bgbri = programs->get(index,"bgbri");
    contrast = programs->get(index,"contrast");
    editmode = programs->get(index,"editmode");
    usemouseup = programs->get(index,"usemouseup");
    hex = programs->get(index,"hex");

    sendChangeMessage();

}

void midiPads::changeProgramName(int index, const String &newName) {
    programs->set(index,"name",newName);
}

const String midiPads::getProgramName(int index) {
    return programs->get(index,"name");
}

int midiPads::getCurrentProgram() {
    return curProgram;
}

//==============================================================================
void midiPads::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //    OutputDebugString("prepareToPlay");
    // do your pre-playback setup stuff here..
}

void midiPads::releaseResources()
{
    //    OutputDebugString("releaseResources");
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void midiPads::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    MidiBuffer midiout;
    bool discard = param[kThru]<0.5f; // if midi thru is off, discard original message
    int outch = (int)(param[kChOut]*15.1f);
    MidiBuffer::Iterator mid_buffer_iter(midiMessages);
    MidiMessage midi_message(0xfe);
    int sample_number;
    while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
        //program change
        if (midi_message.isProgramChange() ) {// && (midi_message.isForChannel(channel) || channel==0)) {
            if (midi_message.getProgramChangeNumber()<getNumPrograms())
                setCurrentProgram(midi_message.getProgramChangeNumber());
        }
        for (int i=0;i<numPads;i++) {
            if (isPadVisible(i))
            {
                //midi learn y
                if (midilisten[i]==1.0f) {
                    if (midi_message.isNoteOn() && (midi_message.getVelocity() > 1)) {
                        midilisten[i] = 0;
                        Ydata1[i]=midi_message.getNoteNumber();
                        Ytype[i]=0;
                        Ydata2[i]=midi_message.getVelocity();
                        sendChangeMessage();
                    }
                    else if (midi_message.isController()) {
                        midilisten[i] = 0;
                        Ycc[i]=midi_message.getControllerNumber();
                        Ytype[i]=1;
                        Ydata2[i]=midi_message.getControllerValue();
                        sendChangeMessage();
                    }
                }
                // learn y-off
                if (midilisten[i]==0.9f) {
                    if (midi_message.isNoteOn() && (midi_message.getVelocity() > 1)) {
                        midilisten[i] = 0;
                        Yoff[i]=midi_message.getVelocity();
                        sendChangeMessage();
                    }
                    else if (midi_message.isController()) {
                        midilisten[i] = 0;
                        Yoff[i]=midi_message.getControllerValue();
                        sendChangeMessage();
                    }
                }
                // learn x
                else if (midilisten[i]==0.8f) {
                    if (midi_message.isController()) {
                        midilisten[i] = 0;
                        Xcc[i]=midi_message.getControllerNumber();
                        sendChangeMessage();
                    }
                    else if (midi_message.isPitchWheel()) {
                        midilisten[i] = 0;
                        UseXPB[i] = true;
                        sendChangeMessage();
                    }
                }
                // learn x-off
                else if (midilisten[i]==0.7f) {
                    if (midi_message.isController()) {
                        midilisten[i] = 0;
                        Xoff[i]=midi_message.getControllerValue();
                        sendChangeMessage();
                    }
                }
                // learn trigger
                if (midilisten[i]==0.6f) {
                    if (midi_message.isNoteOn() && (midi_message.getVelocity() > 1)) {
                        midilisten[i] = 0;
                        trigger[i]=midi_message.getNoteNumber();
                        sendChangeMessage();
                    }
                    //else if (midi_message.isController()) {
                    //    midilisten[i] = 0;
                    //    trigger[i]=midi_message.getControllerNumber();
                    //    sendChangeMessage(this);
                    //}
                }
                //midi triggering
                else if (param[kUseTrigger]>=0.5f) {
                    int inch = 1+(int)(param[kChIn]*15.1f);
                    if (midi_message.isNoteOn() && midi_message.isForChannel(inch)) {
                        if (param[kNoteOnTrig]>=0.5f) {
                            trig = true;
                            triggervel = midi_message.getVelocity();
                            sendChangeMessage();
                        }
                        else if (midi_message.getNoteNumber() == trigger[i]) {
                            int note = Ydata1[i];
                            if (Ytype[i]==0) {
                                // note on
                                int vel;
                                if (param[kUseVel]>=0.5f) vel=(int)( (float)((midi_message.getVelocity())*(getParameter(kVelOffset)*2)) );
                                else vel = jlimit(1,127,(int)(Ydata2[i]*(getParameter(kVelOffset)*2)));
                                if (toggle[i]) {
                                    if (togglestate[i]) {
                                        if (isplaying[note]) {
                                            midiout.addEvent(MidiMessage(0x80|outch,note,Yoff[i],0),0); //noteoff
                                            isplaying[note] = false;
                                        }
                                        togglestate[i] = false;
                                    }
                                    else {
                                        if (!isplaying[note]) {
                                            midiout.addEvent(MidiMessage(0x90|outch,note,vel,0),sample_number);
                                            isplaying[note] = true;
                                        }
                                        togglestate[i] = true;
                                    }
                                }
                                else {
                                    if (isplaying[note]) {
                                        midiout.addEvent(MidiMessage(0x80|outch,note,Yoff[i],0),0); //noteoff
                                    }
                                    midiout.addEvent(MidiMessage(0x90|outch,note,vel,0),sample_number);
                                    isplaying[note] = true;
                                    triggered[i]=true;
                                    trig=true;
                                }
                                discard = true; //kill trigger message
                            }
                            else {
                                // cc on
                                int value = jlimit(0,127,(int)(Ydata2[i]*(getParameter(kCCOffset)*2)));
                                if (toggle[i]) {
                                    if (togglestate[i]) {
                                        midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],Yoff[i],0),0);
                                        togglestate[i] = false;
                                    }
                                    else {
                                        midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],value,0),sample_number);
                                        togglestate[i] = true;
                                    }
                                }
                                else {
                                    midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],value,0),sample_number);
                                    triggered[i]=true;
                                    trig=true;
                                }
                                discard = true; //kill trigger message
                            }
                            sendChangeMessage();
                        }
                    }
                    else if (midi_message.isNoteOff() && midi_message.isForChannel(inch)) {
                        if (midi_message.getNoteNumber() == trigger[i]) {
                            discard = true;
                            if (!toggle[i])
                            {
                                if (param[kNoteOnTrig]<0.5f) {
                                    if (Ytype[i]==0) {
                                        // note off
                                        midiout.addEvent(MidiMessage(0x80|outch,Ydata1[i],Yoff[i],0),sample_number);
                                        isplaying[Ydata1[i]]=false;
                                    }
                                    else {
                                        if (SendOff[i]) { // if sending cc off value
                                            midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],Yoff[i],0),sample_number);
                                        }
                                    }
                                    triggered[i]=false;
                                    trig=true;
                                    sendChangeMessage();
                                }
                            }
                        }
                    }
                }
            }
        }
        //if thru is on, copy original message
        if (!discard) midiout.addEvent(midi_message,sample_number);
    }
    //midi sending part
    for (int i=0;i<numPads;i++) {
        if (buttondown[i]) {
            buttondown[i]=false;
            if (UseX[i]) {
                if (UseXPB[i]) {
                    int value = jlimit(0,16383,roundFloatToInt( (param[i+xpos]*16383.0f)*(getParameter(kCCOffset)*2) ));
                    midiout.addEvent(MidiMessage(0xE0|outch,value & 0x007f,(value & 0x3f80)>>7,0),0);
                }
                else {
                    int value = jlimit(0,127,(int)( (param[i+xpos]*127.1)*(getParameter(kCCOffset)*2) ));
                    if (value!=lastxccvalue[i]) {
                        lastxccvalue[i]=value;
                        midiout.addEvent(MidiMessage(0xB0|outch,Xcc[i],value,0),0);
                    }
                }
            }
            if (param[kSendAft]>=0.5f && Ytype[i]==0) {
                midiout.addEvent(MidiMessage(0xA0|outch,Ydata1[i],jlimit(0,127,(int)(param[i+ypos]*127.1f)),0),0);
            }
            if (UseYCC[i] && Ytype[i]==0) {
                int value = jlimit(0,127,(int)( (param[i+ypos]*127.1)*(getParameter(kCCOffset)*2) ));
                if (value!=lastyccvalue[i]) {
                    lastyccvalue[i]=value;
                    midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],value,0),0);
                }
            }
        }
        if (send[i]>=0.5f) {
            send[i]=0;
            if (Ytype[i]==0) { //note
                int note = Ydata1[i];
                if (isplaying[note]) {
                    //if (useaft>=0.5f) {
                    //    MidiMessage aft_message(0xA0|outch,note,0,0);
                    //    midiout.addEvent(aft_message,0);
                    //}
                    //noteoff
                    midiout.addEvent(MidiMessage(0x80|outch,note,Yoff[i],0),0);
                }
                if (param[kMono]>=0.5f) {
                    //mono mode: kill any playing notes before starting a new one
                    for (int n=0;n<128;n++) {
                        if (isplaying[n]) {
                            //noteoff
                            midiout.addEvent(MidiMessage(0x80|outch,n,0,0),0);
                            isplaying[n]=false;
                        }
                    }
                }
                int vel;
                if (param[kNoteOnTrig]>=0.5f)
                    vel = jlimit(0,127,(int)(triggervel*(getParameter(kVelOffset)*2)));
                else if (UseY[i])
                    vel = jlimit(0,127,(int)((param[i+ypos]*127.1)*(getParameter(kVelOffset)*2)));
                else
                    vel = jlimit(0,127,(int)(Ydata2[i]*(getParameter(kVelOffset)*2)));
                midiout.addEvent(MidiMessage(0x90|outch,note,vel,0),1); //noteon
                isplaying[note] = true;
                if (param[kSendAft]>=0.5f) {
                    midiout.addEvent(MidiMessage(0xA0|outch,note,jlimit(0,127,(int)(param[i+ypos]*127.1f)),0),2);
                }
                if (UseYCC[i]) {
                    int val;
                    if (UseY[i])
                        val = jlimit(0,127,(int)( (param[i+ypos]*127.1)*(getParameter(kCCOffset)*2) ));
                    else
                        val = jlimit(0,127,(int)((float)Ydata2[i]*(getParameter(kCCOffset)*2)));
                    if (val!=lastyccvalue[i]) {
                        lastyccvalue[i]=val;
                        midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],val,0),2);
                    }
                }
            }
            else { //CC
                if (UseY[i]) {
                    int value = jlimit(0,127,(int)( (param[i+ypos]*127.1f)*(getParameter(kCCOffset)*2) ));
                    if (value!=lastyccvalue[i]) {
                        lastyccvalue[i]=value;
                        midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],value,0),0);
                    }
                }
                else if (!UseX[i]) {
                    int value = jlimit(0,127,(int)(Ydata2[i]*(getParameter(kCCOffset)*2)));
                    midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],value,0),0); //on value
                }
            }
        }
        else if (sendoff[i]>=0.5f) {
            sendoff[i]=0;
            if (UseX[i] && SendOff[i]) {
                //x-off value
                if (UseXPB[i])
                    midiout.addEvent(MidiMessage(0xE0|outch,0,Xoff[i],0),0);
                else
                    midiout.addEvent(MidiMessage(0xB0|outch,Xcc[i],Xoff[i],0),0);
            }
            if (Ytype[i]==0) {
                int note = Ydata1[i];
                if (param[kSendAft]>=0.5f)
                    midiout.addEvent(MidiMessage(0xA0|outch,note,0,0),0);
                if (UseYCC[i] && SendOff[i])
                    midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],Yoff[i],0),0); //y-off value
                midiout.addEvent(MidiMessage(0x80|outch,note,Yoff[i],0),0); //noteoff
                isplaying[note]=false;
            }
            else {
                if ((SendOff[i] || toggle[i]) && ((!UseX[i]) || UseY[i]))
                    midiout.addEvent(MidiMessage(0xB0|outch,Ycc[i],Yoff[i],0),0); //y-off value
            }
        }
    }
    midiMessages.clear();
    midiMessages = midiout;
}
//==============================================================================
AudioProcessorEditor* midiPads::createEditor()
{
    return new midiPadsEditor (this);
}

void midiPads::copySettingsToProgram(int index)
{
    programs->set(index,"bghue",bghue);
    programs->set(index,"bgsat",bgsat);
    programs->set(index,"bgbri",bgbri);
    programs->set(index,"contrast",contrast);
    programs->set(index,"lastUIHeight",lastUIHeight);
    programs->set(index,"lastUIWidth",lastUIWidth);
    programs->set(index,"squares",squares);
    programs->set(index,"editmode",editmode);
    programs->set(index,"hex",hex);
    programs->set(index,"usemouseup",usemouseup);
    for (int i=0;i<numPads;i++) {
        programs->setForPad(index,i,"centeredText",centeredText[i]);
        programs->setForPad(index,i,"iconsize",iconsize[i]);
        programs->setForPad(index,i,"roundness",roundness[i]);
        programs->setForPad(index,i,"showdots",showdots[i]);
        programs->setForPad(index,i,"showvalues",showvalues[i]);
        programs->setForPad(index,i,"icon",icon[i]);
        programs->setForPad(index,i,"text",text[i]);
        programs->setForPad(index,i,"padcolor",padcolor[i].toString());
        programs->setForPad(index,i,"Ydata1",Ydata1[i]);
        programs->setForPad(index,i,"Ycc",Ycc[i]);
        programs->setForPad(index,i,"Ytype",Ytype[i]);
        programs->setForPad(index,i,"Ydata2",Ydata2[i]);
        programs->setForPad(index,i,"Yoff",Yoff[i]);
        programs->setForPad(index,i,"trigger",trigger[i]);
        programs->setForPad(index,i,"Xcc",Xcc[i]);
        programs->setForPad(index,i,"Xoff",Xoff[i]);
        programs->setForPad(index,i,"SendOff",SendOff[i]);
        programs->setForPad(index,i,"UseX",UseX[i]);
        programs->setForPad(index,i,"UseY",UseY[i]);
        programs->setForPad(index,i,"UseYCC",UseYCC[i]);
        programs->setForPad(index,i,"UseXPB",UseXPB[i]);
        programs->setForPad(index,i,"lastx",lastx[i]);
        programs->setForPad(index,i,"lasty",lasty[i]);
        programs->setForPad(index,i,"toggle",toggle[i]);
        programs->setForPad(index,i,"togglestate",togglestate[i]);
    }
}

//==============================================================================
void midiPads::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    copySettingsToProgram(curProgram);
    MemoryOutputStream stream(destData,false);
    programs->values_.getChild(curProgram).writeToStream(stream);
    //programs->values_.getChild(curProgram).writeToStream(MemoryOutputStream(262144,256,&destData));
}

void midiPads::getStateInformation(juce::MemoryBlock &destData)
{
    copySettingsToProgram(curProgram);
    MemoryOutputStream stream(destData,false);
    programs->values_.writeToStream(stream);
    //programs->values_.writeToStream(MemoryOutputStream(262144,256,&destData));
}

void midiPads::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    //check for old format
    auto xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            changeProgramName(getCurrentProgram(),xmlState->getStringAttribute ("progname", "Default"));
            for (int i=0;i<kNumGlobalParams;i++) {
                param[i] = (float) xmlState->getDoubleAttribute (String(i), param[i]);
            }
            lastUIWidth = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            bghue = (float) xmlState->getDoubleAttribute ("Hue", bghue);
            bgsat = (float) xmlState->getDoubleAttribute ("Sat", bgsat);
            bgbri = (float) xmlState->getDoubleAttribute ("Bri", bgbri);
            contrast = (float) xmlState->getDoubleAttribute ("Contrast", contrast);
            squares = xmlState->getIntAttribute ("squares", squares);
            editmode = xmlState->getBoolAttribute ("editmode", editmode);
            hex = xmlState->getBoolAttribute ("hex", hex);
            usemouseup = xmlState->getBoolAttribute ("usemouseup", usemouseup);
            if (hex) setLayout(curProgram,hexagonpads);
            else {
                switch(squares)
                {
                case 1:  setLayout(curProgram,onepad); break;
                case 4:  setLayout(curProgram,fourpads); break;
                case 5:  setLayout(curProgram,foursliders); break;
                case 10: setLayout(curProgram,tenpads); break;
                case 14: setLayout(curProgram,twelvepads); break;
                case 16: setLayout(curProgram,sixteenpads); break;
                case 17: setLayout(curProgram,sixteensliders); break;
                case 28: setLayout(curProgram,arrangeit28); break;
                case 33: setLayout(curProgram,mixerpads); break;
                case 40: setLayout(curProgram,hexagonpads); break;
                case 46: setLayout(curProgram,arrangeit39); break;
                case 48: setLayout(curProgram,arrangeit48); break;
                case 52: setLayout(curProgram,arrangeit51); break;
                case 64: setLayout(curProgram,sixtyfourpads); break;
                default: setLayout(curProgram,sixteenpads); break;
                }
            }

            for (int i=0;i<squares;i++) {
                centeredText[i] = false;
                iconsize[i] = 0.5f;
                roundness[i] = (float) xmlState->getDoubleAttribute ("roundness", roundness[i]);
                showdots[i] = xmlState->getBoolAttribute ("showdots", showdots[i]);
                showdots[i] = xmlState->getBoolAttribute ("showdots"+String(i), showdots[i]);
                showvalues[i] = xmlState->getBoolAttribute ("showvalues"+String(i), showvalues[i]);
                icon[i] = xmlState->getStringAttribute ("icon"+String(i), icon[i]);
                text[i] = xmlState->getStringAttribute ("text"+String(i), text[i]);
                padcolor[i] = Colour(xmlState->getIntAttribute ("padcolor"+String(i), padcolor[i].getARGB()));
                Ydata1[i] = (int)(127.1* xmlState->getDoubleAttribute ("Ydata1"+String(i), Ydata1[i]*midiScaler));
                Ycc[i] = (int)(127.1* xmlState->getDoubleAttribute ("Ycc"+String(i), Ycc[i]*midiScaler));
                Ytype[i] = roundToInt( xmlState->getDoubleAttribute ("Ytype"+String(i), Ytype[i]));
                Ydata2[i] = (int)(127.1* xmlState->getDoubleAttribute ("Ydata2"+String(i), Ydata2[i]*midiScaler));
                Yoff[i] = (int)(127.1* xmlState->getDoubleAttribute ("Yoff"+String(i), Yoff[i]*midiScaler));
                trigger[i] = (int)(127.1* xmlState->getDoubleAttribute ("trigger"+String(i), trigger[i]*midiScaler));
                Xcc[i] = (int)(127.1* xmlState->getDoubleAttribute ("Xcc"+String(i), Xcc[i]*midiScaler));
                Xoff[i] = (int)(127.1* xmlState->getDoubleAttribute ("Xoff"+String(i), Xoff[i]*midiScaler));
                SendOff[i] = xmlState->getBoolAttribute ("SendOff"+String(i), SendOff[i]);
                UseX[i] = xmlState->getDoubleAttribute ("UseX"+String(i), UseX[i])>=0.5f;
                UseY[i] = xmlState->getDoubleAttribute ("UseY"+String(i), UseY[i])>=0.5f;
                UseYCC[i] = xmlState->getBoolAttribute ("UseYCC"+String(i), UseYCC[i]);
                UseXPB[i] = xmlState->getBoolAttribute ("UseXPB"+String(i), UseXPB[i]);
                lastx[i] = xmlState->getIntAttribute ("lastx"+String(i), lastx[i]);
                lasty[i] = xmlState->getIntAttribute ("lasty"+String(i), lasty[i]);
                toggle[i] = xmlState->getBoolAttribute ("toggle"+String(i), toggle[i]);
                togglestate[i] = xmlState->getBoolAttribute ("togglestate"+String(i), togglestate[i]);
            }
            sendChangeMessage ();
        }
    }
    else
    {
        programs->values_.removeChild(programs->values_.getChild(curProgram),0);
        MemoryInputStream stream(data,sizeInBytes,false);
        programs->values_.addChild(ValueTree::readFromStream(stream),curProgram,0);
        programs->values_.getChild(curProgram).setProperty("progIndex",curProgram,0);
        init=true;
        setCurrentProgram(curProgram);
    }
}

void midiPads::setStateInformation (const void* data, int sizeInBytes)
{
    //check for old format
    auto const xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != 0)
    {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            for (int p=0;p<getNumPrograms();p++) {
                String prefix = "P" + String(p) + ".";
                for (int i=0;i<kNumGlobalParams;i++) {
                    programs->set(p,getGlobalParamValueName(i),(float) xmlState->getDoubleAttribute (prefix+String(i), programs->get(p,getGlobalParamValueName(i))));
                }
                programs->set(p,"lastUIWidth",xmlState->getIntAttribute (prefix+"uiWidth", programs->get(p,"lastUIWidth")));
                programs->set(p,"lastUIHeight",xmlState->getIntAttribute (prefix+"uiHeight", programs->get(p,"lastUIHeight")));
                programs->set(p,"bghue",(float) xmlState->getDoubleAttribute (prefix+"Hue", programs->get(p,"bghue")));
                programs->set(p,"bgsat",(float) xmlState->getDoubleAttribute (prefix+"Sat", programs->get(p,"bgsat")));
                programs->set(p,"bgbri",(float) xmlState->getDoubleAttribute (prefix+"Bri", programs->get(p,"bgbri")));
                programs->set(p,"contrast",(float) xmlState->getDoubleAttribute (prefix+"Contrast", programs->get(p,"contrast")));
                programs->set(p,"squares",xmlState->getIntAttribute (prefix+"squares", programs->get(p,"squares")));
                programs->set(p,"editmode",xmlState->getBoolAttribute (prefix+"editmode", programs->get(p,"editmode")));
                programs->set(p,"hex",xmlState->getBoolAttribute (prefix+"hex", programs->get(p,"hex")));
                programs->set(p,"usemouseup",xmlState->getBoolAttribute (prefix+"usemouseup", programs->get(p,"usemouseup")));
                if (programs->get(p,"hex"))
                    setLayout(p,hexagonpads);
                else {
                    switch((int)programs->get(p,"squares"))
                    {
                    case 1:  setLayout(p,onepad); break;
                    case 4:  setLayout(p,fourpads); break;
                    case 5:  setLayout(p,foursliders); break;
                    case 10: setLayout(p,tenpads); break;
                    case 14: setLayout(p,twelvepads); break;
                    case 16: setLayout(p,sixteenpads); break;
                    case 17: setLayout(p,sixteensliders); break;
                    case 28: setLayout(p,arrangeit28); break;
                    case 33: setLayout(p,mixerpads); break;
                    case 40: setLayout(p,hexagonpads); break;
                    case 46: setLayout(p,arrangeit39); break;
                    case 48: setLayout(p,arrangeit48); break;
                    case 52: setLayout(p,arrangeit51); break;
                    case 64: setLayout(p,sixtyfourpads); break;
                    default: setLayout(p,sixteenpads); break;
                    }
                }
                for (int i=0;i<(int)(programs->get(p,"squares"));i++) {
                    programs->setForPad(p,i,"showdots",xmlState->getBoolAttribute (prefix+"showdots", programs->getForPad(p,i,"showdots")));
                    programs->setForPad(p,i,"showdots",xmlState->getBoolAttribute (prefix+"showdots"+String(i), programs->getForPad(p,i,"showdots")));
                    programs->setForPad(p,i,"showvalues",xmlState->getBoolAttribute (prefix+"showvalues"+String(i), programs->getForPad(p,i,"showvalues")));
                    programs->setForPad(p,i,"iconsize",0.5f);
                    programs->setForPad(p,i,"centeredText",false);
                    programs->setForPad(p,i,"roundness",(float) xmlState->getDoubleAttribute (prefix+"roundness", programs->getForPad(p,i,"roundness")));
                    programs->setForPad(p,i,"icon",xmlState->getStringAttribute (prefix+"icon"+String(i), programs->getForPad(p,i,"icon")));
                    programs->setForPad(p,i,"text",xmlState->getStringAttribute (prefix+"text"+String(i), programs->getForPad(p,i,"text")));
                    programs->setForPad(p,i,"padcolor",
                        Colour(xmlState->getIntAttribute(prefix+"padcolor"+String(i),Colour::fromString(programs->getForPad(p,i,"padcolor").toString()).getARGB())).toString()
                        );
                    programs->setForPad(p,i,"Ydata1",(int)(127.1* xmlState->getDoubleAttribute (prefix+"Ydata1"+String(i), programs->getForPad(p,i,"Ydata1"))));
                    programs->setForPad(p,i,"Ycc",(int)(127.1* xmlState->getDoubleAttribute (prefix+"Ycc"+String(i), programs->getForPad(p,i,"Ycc"))));
                    programs->setForPad(p,i,"Ytype",roundToInt( xmlState->getDoubleAttribute (prefix+"Ytype"+String(i), programs->getForPad(p,i,"Ytype"))));
                    programs->setForPad(p,i,"Ydata2",(int)(127.1* xmlState->getDoubleAttribute (prefix+"Ydata2"+String(i), programs->getForPad(p,i,"Ydata2"))));
                    programs->setForPad(p,i,"Yoff",(int)(127.1* xmlState->getDoubleAttribute (prefix+"Yoff"+String(i), programs->getForPad(p,i,"Yoff"))));
                    programs->setForPad(p,i,"trigger",(int)(127.1* xmlState->getDoubleAttribute (prefix+"trigger"+String(i), programs->getForPad(p,i,"trigger"))));
                    programs->setForPad(p,i,"Xcc",(int)(127.1* xmlState->getDoubleAttribute (prefix+"Xcc"+String(i), programs->getForPad(p,i,"Xcc"))));
                    programs->setForPad(p,i,"Xoff",(int)(127.1* xmlState->getDoubleAttribute (prefix+"Xoff"+String(i), programs->getForPad(p,i,"Xoff"))));
                    programs->setForPad(p,i,"SendOff",xmlState->getBoolAttribute (prefix+"SendOff"+String(i), programs->getForPad(p,i,"SendOff")));
                    programs->setForPad(p,i,"UseX",xmlState->getDoubleAttribute (prefix+"UseX"+String(i), programs->getForPad(p,i,"UseX"))>=0.5f);
                    programs->setForPad(p,i,"UseY",xmlState->getDoubleAttribute (prefix+"UseY"+String(i), programs->getForPad(p,i,"UseY"))>=0.5f);
                    programs->setForPad(p,i,"UseYCC",xmlState->getBoolAttribute (prefix+"UseYCC"+String(i), programs->getForPad(p,i,"UseYCC")));
                    programs->setForPad(p,i,"UseXPB",xmlState->getBoolAttribute (prefix+"UseXPB"+String(i), programs->getForPad(p,i,"UseXPB")));
                    programs->setForPad(p,i,"lastx",xmlState->getIntAttribute (prefix+"lastx"+String(i), programs->getForPad(p,i,"lastx")));
                    programs->setForPad(p,i,"lasty",xmlState->getIntAttribute (prefix+"lasty"+String(i), programs->getForPad(p,i,"lasty")));
                    programs->setForPad(p,i,"toggle",xmlState->getBoolAttribute (prefix+"toggle"+String(i), programs->getForPad(p,i,"toggle")));
                    programs->setForPad(p,i,"togglestate",xmlState->getBoolAttribute (prefix+"togglestate"+String(i), programs->getForPad(p,i,"togglestate")));
                }
                programs->set(p,"name",xmlState->getStringAttribute (prefix+"progname", programs->get(p,"name")));
            }
            init=true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
    else
    {
        MemoryInputStream stream(data,sizeInBytes,false);
        ValueTree vt = ValueTree::readFromStream(stream);
        if (vt.isValid())
        {
            programs->values_.removeAllChildren(0);
            for (int i=0;i<vt.getNumChildren();i++)
            {
                programs->values_.addChild(vt.getChild(i).createCopy(),i,0);
            }
        }
        init=true;
        setCurrentProgram(vt.getProperty("lastProgram",0));
    }
}

void midiPads::saveXmlPatch(int index, File file)
{
    copySettingsToProgram(curProgram);
    auto xml(programs->values_.getChild(index).createXml());
    xml->writeToFile(file," ");
}

void midiPads::saveXmlBank(File file)
{
    copySettingsToProgram(curProgram);
    auto xml(programs->values_.createXml());
    xml->writeToFile(file," ");
}

bool midiPads::loadXmlPatch(int index, File file)
{
    if (!file.exists()) return false;
    XmlDocument xmldoc(file.loadFileAsString());
    auto xmlState(xmldoc.getDocumentElement());
    programs->values_.removeChild(programs->values_.getChild(index),0);
    programs->values_.addChild(ValueTree::fromXml(*xmlState),index,0);
    programs->values_.getChild(index).setProperty("progIndex",index,0);
    if (curProgram==index) {
        init=true;
        setCurrentProgram(index);
    }
    return true;
}

bool midiPads::loadXmlBank(File file)
{
    if (!file.exists()) return false;
    String xml = file.loadFileAsString();
    XmlDocument xmldoc(xml);
    auto xmlState(xmldoc.getDocumentElement());
    ValueTree vt = ValueTree::fromXml(*xmlState);
    if (vt.isValid())
    {
        programs->values_.removeAllChildren(0);
        for (int i=0;i<vt.getNumChildren();i++)
        {
            programs->values_.addChild(vt.getChild(i).createCopy(),i,0);
        }
    }
    init=true;
    setCurrentProgram(0);
    return true;
}

bool midiPads::loadFxb(File file)
{
    if (!file.exists()) return false;
    MemoryBlock bank = MemoryBlock(0,true);
    file.loadFileAsData(bank);
    bank.removeSection(0,16);
    if (!strncmp((char*)bank.getData(),"m16p",4))
    {
        bank.removeSection(0,144);
        setStateInformation(bank.getData(), (int)bank.getSize());
        setCurrentProgram(0);
        return true;
    }
    return false;
}

bool midiPads::loadFxp(File file)
{
    if (!file.exists()) return false;
    MemoryBlock data = MemoryBlock(0,true);
    file.loadFileAsData(data);
    data.removeSection(0,16);
    if (!strncmp((char*)data.getData(),"m16p",4))
    {
        data.removeSection(0,44);
        setCurrentProgramStateInformation(data.getData(), (int)data.getSize());
        return true;
    }
    return false;
}

void midiPads::saveXmlLayout(File file)
{
    auto xml(programs->values_.getChild(curProgram).getChildWithName("Layout").createXml());
    xml->writeToFile(file," ");
}

void midiPads::loadXmlLayout(File file)
{
    if (file.exists())
    {
        XmlDocument xmldoc(file.loadFileAsString());
        auto xmlState(xmldoc.getDocumentElement());
        ValueTree program = programs->values_.getChild(curProgram);
        int index = program.indexOf(program.getChildWithName("Layout"));
        program.removeChild(index,0);
        program.addChild(ValueTree::fromXml(*xmlState),index,0);
    }
}

juce::Rectangle<int> midiPads::getPadBounds(int index)
{
    juce::Rectangle<float> rect = programs->getPadBounds(curProgram,index);
    return juce::Rectangle<int>(roundToInt(rect.getX()*lastUIWidth),
                                roundToInt(rect.getY()*lastUIHeight),
                                roundToInt(rect.getWidth()*lastUIWidth),
                                roundToInt(rect.getHeight()*lastUIHeight));
}

void midiPads::setPadPosition(int index, float x, float y)
{
    programs->setPadPosition(curProgram, index, x, y);
}

void midiPads::setPadSize(int index, float w, float h)
{
    programs->setPadSize(curProgram, index, w, h);
}

bool midiPads::isPadVisible(int index)
{
    return programs->getPadVisible(curProgram,index);
}

void midiPads::setPadVisible(int index, bool visibility)
{
    programs->setPadVisible(curProgram,index,visibility);
}

void midiPads::fillLayouts()
{
    for (int i=0;i<numLayouts;i++)
    {

        if (onepad==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","1 Pad",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p==0,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0), 0.0151f, 0.0649f, 0.9698f, 0.9243f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (fourpads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","4 Pads",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<4,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0), 0.01f, 0.0603f, 0.4799f, 0.4606f);
            PadLayouts::setPadLayout(layout.getChild(1), 0.51f, 0.0603f, 0.4799f, 0.4606f);
            PadLayouts::setPadLayout(layout.getChild(2), 0.01f, 0.5301f, 0.4799f, 0.4606f);
            PadLayouts::setPadLayout(layout.getChild(3), 0.51f, 0.5301f, 0.4799f, 0.4606f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (foursliders==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","4 Sliders",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<4,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0), 0.0192f, 0.0649f, 0.23f, 0.921f);
            PadLayouts::setPadLayout(layout.getChild(1), 0.2636f, 0.0649f, 0.23f, 0.921f);
            PadLayouts::setPadLayout(layout.getChild(2), 0.5064f, 0.0649f, 0.23f, 0.921f);
            PadLayouts::setPadLayout(layout.getChild(3), 0.7508f, 0.0649f, 0.23f, 0.921f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (tenpads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","10 Pads",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<10,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0), 0.0187f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(1), 0.2627f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(2), 0.5070f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(3), 0.7512f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(4), 0.0187f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(5), 0.2627f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(6), 0.5070f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(7), 0.7512f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(8), 0.0187f, 0.5317f, 0.4793f, 0.4637f);
            PadLayouts::setPadLayout(layout.getChild(9), 0.5073f, 0.5317f, 0.4793f, 0.4637f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (twelvepads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","2 Pads, 12 Sliders",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<14,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0086f, 0.0618f, 0.4786f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.4940f, 0.0618f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.5798f, 0.0618f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.6638f, 0.0618f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.7479f, 0.0618f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.8319f, 0.0618f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.9160f, 0.0618f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.0086f, 0.5318f, 0.4786f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.4940f, 0.5318f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.5815f, 0.5318f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.6638f, 0.5318f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.7479f, 0.5318f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.8319f, 0.5318f, 0.0755f, 0.4588f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.9160f, 0.5318f, 0.0755f, 0.4588f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (sixteenpads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","16 Pads",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<16,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0187f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.2627f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.5070f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.7512f, 0.0649f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.0187f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.2627f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.5070f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.7512f, 0.2983f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.0187f, 0.5317f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.2627f, 0.5317f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.5070f, 0.5317f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.7555f, 0.5317f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.0187f, 0.7650f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.2627f, 0.7650f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.5070f, 0.7650f, 0.2303f, 0.2319f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.7512f, 0.7650f, 0.2303f, 0.2319f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (sixteensliders==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","16 Sliders",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<16,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0058f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.0675f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.1293f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.1911f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.2529f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.3147f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.3764f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.4382f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.5000f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.5618f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.6236f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.6853f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.7471f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.8089f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.8707f, 0.0619f, 0.0603f, 0.9305f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.9325f, 0.0619f, 0.0603f, 0.9305f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (sixtyfourpads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","64 Pads",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<64,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0031f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.1280f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.2516f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.3752f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.5000f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.6266f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.7516f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.8766f, 0.0538f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.0031f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.1280f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.2516f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.3752f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.5000f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.6266f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.7516f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.8766f, 0.1728f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.0031f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.1280f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.2516f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.3752f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.5000f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.6266f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.7516f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.8766f, 0.2910f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.0031f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.1280f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.2516f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.3752f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.5000f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.6266f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.7516f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.8766f, 0.4092f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.0031f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.1280f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.2516f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.3752f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.5000f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.6266f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.7516f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(39), 0.8766f, 0.5288f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(40), 0.0031f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(41), 0.1280f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(42), 0.2516f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(43), 0.3752f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(44), 0.5000f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(45), 0.6266f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(46), 0.7516f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(47), 0.8766f, 0.6455f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(48), 0.0031f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(49), 0.1280f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(50), 0.2516f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(51), 0.3752f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(52), 0.5000f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(53), 0.6266f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(54), 0.7516f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(55), 0.8766f, 0.7637f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(56), 0.0031f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(57), 0.1280f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(58), 0.2516f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(59), 0.3752f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(60), 0.5000f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(61), 0.6266f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(62), 0.7516f, 0.8803f, 0.1203f, 0.1211f);
            PadLayouts::setPadLayout(layout.getChild(63), 0.8766f, 0.8803f, 0.1203f, 0.1211f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (hexagonpads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","Hexagons",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<40,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0370f, 0.1504f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.0370f, 0.3372f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.0370f, 0.5241f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.0370f, 0.7110f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.1358f, 0.2438f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.1358f, 0.4307f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.1358f, 0.6175f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.1358f, 0.8044f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.2346f, 0.1504f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.2346f, 0.3372f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.2346f, 0.5241f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.2346f, 0.7110f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.3333f, 0.2438f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.3333f, 0.4307f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.3333f, 0.6175f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.3333f, 0.8044f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.4321f, 0.1504f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.4321f, 0.3372f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.4321f, 0.5241f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.4321f, 0.7110f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.5309f, 0.2438f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.5309f, 0.4307f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.5309f, 0.6175f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.5309f, 0.8044f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.6296f, 0.1504f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.6296f, 0.3372f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.6296f, 0.5241f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.6296f, 0.7110f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.7284f, 0.2438f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.7284f, 0.4307f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.7284f, 0.6175f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.7284f, 0.8044f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.8272f, 0.1504f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.8272f, 0.3372f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.8272f, 0.5241f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.8272f, 0.7110f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.1358f, 0.0584f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.3333f, 0.0584f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.5309f, 0.0584f, 0.1317f, 0.1869f);
            PadLayouts::setPadLayout(layout.getChild(39), 0.7284f, 0.0584f, 0.1317f, 0.1869f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (mixerpads==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","8 Ch Mixer",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<32,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0029f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.1273f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.2515f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.3750f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.5000f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.6265f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.7517f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.8765f, 0.0541f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.0029f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.1273f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.2515f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.3750f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.5000f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.6265f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.7517f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.8765f, 0.1731f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.0029f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.1273f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.2515f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.3750f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.5000f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.6265f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.7517f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.8765f, 0.2905f, 0.1207f, 0.1206f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.0029f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.1273f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.2515f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.3750f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.5000f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.6265f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.7517f, 0.4100f, 0.1207f, 0.5792f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.8765f, 0.4100f, 0.1207f, 0.5792f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (arrangeit28==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","2 XY, 5 Sliders, 21 Buttons",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<28,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0134f, 0.4030f, 0.5854f, 0.5822f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.6276f, 0.0696f, 0.3611f, 0.3126f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.0113f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.0113f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.0134f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.1019f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.1019f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.1049f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.1914f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.1914f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.1924f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.2788f, 0.0652f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.2788f, 0.1778f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.2798f, 0.2904f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.3673f, 0.0667f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.3673f, 0.1793f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.3673f, 0.2919f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.4537f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.4537f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.4547f, 0.2919f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.5411f, 0.0637f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.5411f, 0.1763f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.5422f, 0.2889f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.6317f, 0.4030f, 0.0576f, 0.5822f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.9269f, 0.4030f, 0.0576f, 0.5822f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.7047f, 0.4030f, 0.0576f, 0.5822f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.7788f, 0.4030f, 0.0576f, 0.5822f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.8529f, 0.4030f, 0.0576f, 0.5822f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (arrangeit39==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","2 XY, 12 Sliders, 25 Buttons",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<39,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.5000f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.4383f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.3776f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.3169f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.2572f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.1975f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.1368f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.0755f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.5607f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.6214f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.0154f, 0.0696f, 0.0494f, 0.2993f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.6811f, 0.0696f, 0.0494f, 0.2993f);

            PadLayouts::setPadLayout(layout.getChild(12), 0.0165f, 0.3852f, 0.4979f, 0.6000f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.7479f, 0.0696f, 0.2387f, 0.2993f);

            PadLayouts::setPadLayout(layout.getChild(14), 0.5566f, 0.4119f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.5566f, 0.5244f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.5576f, 0.6370f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.5576f, 0.7511f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.5576f, 0.8637f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.6492f, 0.4119f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.6492f, 0.5244f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.6492f, 0.6370f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.6492f, 0.7511f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.6492f, 0.8637f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.7376f, 0.4119f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.7376f, 0.5244f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.7376f, 0.6370f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.7376f, 0.7511f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.7376f, 0.8637f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.8251f, 0.4119f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.8251f, 0.5244f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.8251f, 0.6370f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.8251f, 0.7511f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.8251f, 0.8637f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.9125f, 0.4119f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.9125f, 0.5244f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.9125f, 0.6370f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.9125f, 0.7511f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.9125f, 0.8637f, 0.0576f, 0.0948f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (arrangeit51==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","2 XY, 49 Buttons",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<51,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(22), 0.0216f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.1121f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.2016f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.2891f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.3776f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.4640f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(40), 0.5514f, 0.0681f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(43), 0.6399f, 0.0681f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(1),  0.7243f, 0.0681f, 0.2623f, 0.3126f);

            PadLayouts::setPadLayout(layout.getChild(23), 0.0237f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.1152f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.2027f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.2901f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.3776f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.4650f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(41), 0.5525f, 0.1807f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(44), 0.6399f, 0.1807f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(24), 0.0237f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.1152f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.2027f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.2901f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.3776f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(39), 0.4650f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(42), 0.5525f, 0.2933f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(45), 0.6399f, 0.2933f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(0),  0.0165f, 0.4030f, 0.4979f, 0.5822f);

            PadLayouts::setPadLayout(layout.getChild(2),  0.5535f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.6420f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.7294f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.8179f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(46), 0.9033f, 0.4030f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(3),  0.5535f, 0.5156f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.6420f, 0.5156f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.7294f, 0.5156f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.8179f, 0.5156f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(47), 0.9033f, 0.5156f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(4),  0.5556f, 0.6281f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.6440f, 0.6281f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.7315f, 0.6281f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.8189f, 0.6281f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(48), 0.9054f, 0.6281f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(5),  0.5556f, 0.7452f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.6440f, 0.7452f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.7315f, 0.7422f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.8189f, 0.7422f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(49), 0.9054f, 0.7422f, 0.0576f, 0.0948f);

            PadLayouts::setPadLayout(layout.getChild(6),  0.5556f, 0.8548f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.6440f, 0.8548f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.7315f, 0.8548f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.8189f, 0.8548f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(50), 0.9054f, 0.8548f, 0.0576f, 0.0948f);

            layouts->values_.addChild(layout,i,0);
        }
        else if (arrangeit48==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","6 Mixer Blocks",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<48,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.2541f, 0.1778f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.0124f, 0.0800f, 0.0411f, 0.4178f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.0761f, 0.1793f, 0.0411f, 0.3185f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.0700f, 0.0815f, 0.2459f, 0.0593f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.1327f, 0.1778f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.1893f, 0.1778f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.2541f, 0.2904f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.2562f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.9280f, 0.1763f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.6862f, 0.0800f, 0.0411f, 0.4178f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.7500f, 0.1778f, 0.0411f, 0.3185f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.7449f, 0.0815f, 0.2459f, 0.0593f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.8066f, 0.1763f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.8632f, 0.1763f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.9280f, 0.2889f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.9300f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.5905f, 0.1778f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.3488f, 0.0800f, 0.0411f, 0.4178f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.4115f, 0.1793f, 0.0411f, 0.3185f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.4064f, 0.0815f, 0.2459f, 0.0593f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.4691f, 0.1778f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.5257f, 0.1778f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.5905f, 0.2904f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.5926f, 0.4030f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.2531f, 0.6578f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.0124f, 0.5615f, 0.0411f, 0.4178f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.0751f, 0.6593f, 0.0411f, 0.3185f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.0700f, 0.5630f, 0.2459f, 0.0593f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.1327f, 0.6578f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.1883f, 0.6578f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.2531f, 0.7704f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.2562f, 0.8830f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.9280f, 0.6563f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.6852f, 0.5615f, 0.0411f, 0.4178f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.7500f, 0.6578f, 0.0411f, 0.3185f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.7438f, 0.5630f, 0.2459f, 0.0593f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.8056f, 0.6563f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.8632f, 0.6563f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.9280f, 0.7689f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(39), 0.9290f, 0.8830f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(40), 0.5905f, 0.6578f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(41), 0.3477f, 0.5615f, 0.0411f, 0.4178f);
            PadLayouts::setPadLayout(layout.getChild(42), 0.4115f, 0.6593f, 0.0411f, 0.3185f);
            PadLayouts::setPadLayout(layout.getChild(43), 0.4053f, 0.5630f, 0.2459f, 0.0593f);
            PadLayouts::setPadLayout(layout.getChild(44), 0.4681f, 0.6578f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(45), 0.5257f, 0.6578f, 0.0411f, 0.3200f);
            PadLayouts::setPadLayout(layout.getChild(46), 0.5905f, 0.7704f, 0.0576f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(47), 0.5926f, 0.8830f, 0.0576f, 0.0948f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (arrangeit64==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","8 Mixer Blocks w/Sends",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<64,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0155f, 0.1809f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.0113f, 0.0985f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.0835f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.1299f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.1773f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.0825f, 0.2765f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.0825f, 0.3471f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.0825f, 0.4176f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.0155f, 0.6794f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.0113f, 0.5971f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.0835f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.1299f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.1773f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.0825f, 0.7750f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.0825f, 0.8456f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.0825f, 0.9162f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.2691f, 0.1809f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.2649f, 0.0985f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.3371f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.3835f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.4309f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.3361f, 0.2765f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.3361f, 0.3471f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.3361f, 0.4176f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.2691f, 0.6794f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.2649f, 0.5971f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.3371f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.3835f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.4309f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.3361f, 0.7750f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.3361f, 0.8456f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.3361f, 0.9162f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.5227f, 0.1809f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.5186f, 0.0985f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.5907f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.6371f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.6845f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.5897f, 0.2765f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.5897f, 0.3471f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(39), 0.5897f, 0.4176f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(40), 0.5227f, 0.6794f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(41), 0.5186f, 0.5971f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(42), 0.5907f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(43), 0.6371f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(44), 0.6845f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(45), 0.5897f, 0.7750f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(46), 0.5897f, 0.8456f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(47), 0.5897f, 0.9162f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(48), 0.7763f, 0.1809f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(49), 0.7722f, 0.0985f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(50), 0.8443f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(51), 0.8907f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(52), 0.9381f, 0.1941f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(53), 0.8433f, 0.2765f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(54), 0.8433f, 0.3471f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(55), 0.8433f, 0.4176f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(56), 0.7763f, 0.6794f, 0.0495f, 0.3118f);
            PadLayouts::setPadLayout(layout.getChild(57), 0.7722f, 0.5971f, 0.2186f, 0.0706f);
            PadLayouts::setPadLayout(layout.getChild(58), 0.8443f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(59), 0.8907f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(60), 0.9381f, 0.6926f, 0.0412f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(61), 0.8433f, 0.7750f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(62), 0.8433f, 0.8456f, 0.1351f, 0.0588f);
            PadLayouts::setPadLayout(layout.getChild(63), 0.8433f, 0.9162f, 0.1351f, 0.0588f);
            layouts->values_.addChild(layout,i,0);
        }
        else if (arrangeit55==i)
        {
            ValueTree layout("Layout");
            layout.setProperty("name","1 XY, 6 Sliders, 48 Buttons",0);
            for (int p=0;p<numPads;p++) {
                ValueTree padpos("PadPosition");
                padpos.setProperty("visible",p<55,0);
                layout.addChild(padpos,p,0);
            }
            PadLayouts::setPadLayout(layout.getChild(0),  0.0082f, 0.5215f, 0.4562f, 0.4622f);
            PadLayouts::setPadLayout(layout.getChild(1),  0.0082f, 0.0785f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(2),  0.0082f, 0.1822f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(3),  0.0082f, 0.2859f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(4),  0.0082f, 0.3896f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(5),  0.0896f, 0.0785f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(6),  0.0896f, 0.1822f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(7),  0.0896f, 0.2859f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(8),  0.0896f, 0.3896f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(9),  0.1689f, 0.0785f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(10), 0.1689f, 0.1822f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(11), 0.1689f, 0.2859f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(12), 0.1689f, 0.3896f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(13), 0.2482f, 0.0785f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(14), 0.2482f, 0.1822f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(15), 0.2482f, 0.2859f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(16), 0.2482f, 0.3896f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(17), 0.3265f, 0.0785f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(18), 0.3265f, 0.1822f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(19), 0.3265f, 0.2859f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(20), 0.3265f, 0.3896f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(21), 0.4047f, 0.0785f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(22), 0.4047f, 0.1822f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(23), 0.4047f, 0.2859f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(24), 0.4047f, 0.3896f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(25), 0.5345f, 0.5778f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(26), 0.5345f, 0.6815f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(27), 0.5345f, 0.7852f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(28), 0.5345f, 0.8889f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(29), 0.6159f, 0.5778f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(30), 0.6159f, 0.6815f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(31), 0.6159f, 0.7852f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(32), 0.6159f, 0.8889f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(33), 0.6944f, 0.5778f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(34), 0.6944f, 0.6815f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(35), 0.6944f, 0.7852f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(36), 0.6944f, 0.8889f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(37), 0.7737f, 0.5778f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(38), 0.7737f, 0.6815f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(39), 0.7737f, 0.7852f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(40), 0.7737f, 0.8889f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(41), 0.8518f, 0.5778f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(42), 0.8518f, 0.6815f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(43), 0.8518f, 0.7852f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(44), 0.8518f, 0.8889f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(45), 0.9311f, 0.5778f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(46), 0.9311f, 0.6815f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(47), 0.9311f, 0.7852f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(48), 0.9311f, 0.8889f, 0.0577f, 0.0948f);
            PadLayouts::setPadLayout(layout.getChild(49), 0.5345f, 0.0756f, 0.0577f, 0.4059f);
            PadLayouts::setPadLayout(layout.getChild(50), 0.6138f, 0.0756f, 0.0577f, 0.4059f);
            PadLayouts::setPadLayout(layout.getChild(51), 0.6914f, 0.0756f, 0.0577f, 0.4059f);
            PadLayouts::setPadLayout(layout.getChild(52), 0.7706f, 0.0756f, 0.0577f, 0.4059f);
            PadLayouts::setPadLayout(layout.getChild(53), 0.8496f, 0.0756f, 0.0577f, 0.4059f);
            PadLayouts::setPadLayout(layout.getChild(54), 0.9310f, 0.0756f, 0.0577f, 0.4059f);
            layouts->values_.addChild(layout,i,0);
        }
#ifdef _DEBUG
        auto xml(layouts->values_.getChild(i).createXml());
        File file(layoutPath+File::getSeparatorString() + "default" + File::getSeparatorString() + layouts->values_.getChild(i).getProperty("name").toString()+".mpadl");
        xml->writeToFile(file," ");
#endif
    }
#ifdef _DEBUG
    //ScopedPointer<XmlElement> xml(layouts->createXml());
    //xml->writeToFile(File("C:\\midiPadsLayouts.xml")," ");
#endif
}

void midiPads::loadDefaultPrograms()
{
    for(int i=0;i<getNumPrograms();i++)
    {
        switch(i)
        {
        case 0:
            programs->set(i,"lastUIWidth",400);
            programs->set(i,"lastUIHeight",400);
            programs->set(i,"squares",10);
            programs->set(i,"name","Pads");
            programs->setForPad(i,8,"UseX",true);
            programs->setForPad(i,9,"UseX",true);
            programs->setForPad(i,8,"UseY",true);
            programs->setForPad(i,9,"UseY",true);
            programs->setForPad(i,8,"UseYCC",true);
            programs->setForPad(i,9,"UseYCC",true);
            programs->setForPad(i,8,"SendOff",false);
            programs->setForPad(i,9,"SendOff",false);
            programs->setForPad(i,8,"roundness",0.2f);
            programs->setForPad(i,9,"roundness",0.2f);
            setLayout(i,tenpads);
            break;
        case 1:
            programs->set(i,"name","Sliders");
            programs->set(i,"squares",17);
            programs->set(i,"lastUIWidth",640);
            programs->set(i,"lastUIHeight",400);
            for (int pad=0;pad<16;pad++) {
                programs->setPadVisible(i,pad,true);
                programs->setForPad(i,pad,"UseY",true);
                programs->setForPad(i,pad,"Ycc",i); //data1
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"SendOff",false);
            }
            setLayout(i,sixteensliders);
            break;
        case 2:
            programs->set(i,"name","X-Y Sliders");
            programs->set(i,"squares",5);
            programs->set(i,"lastUIWidth",640);
            programs->set(i,"lastUIHeight",400);
            for (int pad=0;pad<4;pad++) {
                programs->setForPad(i,pad,"Ycc",i); //data1
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"Xcc",1+pad); //x cc
                programs->setForPad(i,pad,"UseX",true);
                programs->setForPad(i,pad,"UseY",true);
                programs->setForPad(i,pad,"SendOff",false);
            }
            setLayout(i,foursliders);
            break;
        case 3:
            programs->set(i,"name","Big X-Y Pad");
            programs->set(i,"squares",1);
            programs->set(i,"lastUIWidth",400);
            programs->set(i,"lastUIHeight",400);
            programs->setForPad(i,0,"UseX",true);
            programs->setForPad(i,0,"UseY",true);
            programs->setForPad(i,0,"Ycc",74); //data1
            programs->setForPad(i,0,"Ytype",1); //type,CC
            programs->setForPad(i,0,"Xcc",1); //x cc
            programs->setForPad(i,0,"icon",String());
            programs->setForPad(i,0,"text",String());
            programs->setForPad(i,0,"roundness",0.05f);
            programs->setForPad(i,0,"SendOff",false);
            setLayout(i,onepad);
            break;
        case 4:
            programs->set(i,"lastUIWidth",400);
            programs->set(i,"lastUIHeight",400);
            programs->set(i,"name","Mighty Pea");
            programs->set(i,"contrast",0.1f);
            programs->set(i,"bgbri",0.3f);
            programs->set(i,"bgsat",0.0f);
            for (int pad=0;pad<16;pad++) {
                programs->setForPad(i,pad,"roundness",0.3f);
                String padicon=String();
                padicon << "Mighty Pea" << File::getSeparatorString() <<
                    String(pad+1) << ".svg";
                programs->setForPad(i,pad,"icon",padicon);
                programs->setForPad(i,pad,"padcolor",Colour(0xFFFFFFFF).toString());
            }
            programs->setForPad(i,0,"text","Kick");
            programs->setForPad(i,1,"text","Snare");
            programs->setForPad(i,2,"text","HiHat C");
            programs->setForPad(i,3,"text","HiHat O");
            programs->setForPad(i,4,"text","Ride");
            programs->setForPad(i,5,"text","Crash");
            programs->setForPad(i,6,"text","Cowbell");
            programs->setForPad(i,7,"text","Bell");
            programs->setForPad(i,8,"text","Tom 3");
            programs->setForPad(i,9,"text","Tom 2");
            programs->setForPad(i,10,"text","Tom 1");
            programs->setForPad(i,11,"text","Clap");
            programs->setForPad(i,12,"text",String());
            programs->setForPad(i,13,"text",String());
            programs->setForPad(i,14,"text",String());
            programs->setForPad(i,15,"text",String());
            setLayout(i,sixteenpads);
            break;
        case 5:
            programs->set(i,"name","64 Pads");
            programs->set(i,"squares",64);
            for (int pad=0;pad<64;pad++) {
                programs->setForPad(i,pad,"roundness",0.05f);
                programs->setForPad(i,pad,"showdots",false);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
            }
            setLayout(i,sixtyfourpads);
            break;
        case 6:
            programs->set(i,"name","hexagons");
            programs->set(i,"squares",40);
            programs->set(i,"hex",true);
            for (int pad=0;pad<40;pad++) {
                programs->setForPad(i,pad,"showdots",false);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
            }
            setLayout(i,hexagonpads);
            break;
        case 7:
            programs->set(i,"name","8 Channel Mixer");
            programs->set(i,"squares",33);
            programs->set(i,"lastUIHeight",550);
            programs->set(i,"lastUIWidth",550);
            programs->set(i,"bgbri",0.3f);
            for (int pad=0;pad<32;pad++) {
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"Ytype",1);
                if (pad<8) {
                    programs->setForPad(i,pad,"toggle",true);
                    programs->setForPad(i,pad,"SendOff",true);
                    programs->setForPad(i,pad,"padcolor",Colour(0xFFFF0000).toString());
                    programs->setForPad(i,pad,"text","Mute");
                }
                else if (pad<16) {
                    programs->setForPad(i,pad,"toggle",true);
                    programs->setForPad(i,pad,"SendOff",true);
                    programs->setForPad(i,pad,"padcolor",Colour(0xFF00FF00).toString());
                    programs->setForPad(i,pad,"text","Solo");
                }
                else if (pad<24) {
                    programs->setForPad(i,pad,"UseX",true);
                    programs->setForPad(i,pad,"SendOff",false);
                    programs->setForPad(i,pad,"Ydata2",63);
                    programs->setForPad(i,pad,"Xcc",programs->getForPad(i,pad,"Ycc"));
                    programs->setForPad(i,pad,"Ycc",1);
                    programs->setForPad(i,pad,"text","Pan");
                    programs->setForPad(i,pad,"icon",String("pancenter.svg"));
                }
                else {
                    programs->setForPad(i,pad,"UseX",false);
                    programs->setForPad(i,pad,"UseY",true);
                    programs->setForPad(i,pad,"SendOff",false);
                    programs->setForPad(i,pad,"text",String(pad-23));
                }
            }
            setLayout(i,mixerpads);
            break;
        case 8:
            programs->set(i,"name",String("KVR"));
            programs->set(i,"squares",16);
            programs->set(i,"bgsat",0.0f);
            programs->set(i,"bgbri",0.198275864f);
            programs->set(i,"contrast",1.0f);
            for (int pad=0;pad<16;pad++) {
                programs->setForPad(i,pad,"roundness",0.1f);
                programs->setForPad(i,pad,"showdots",false);
                programs->setForPad(i,pad,"icon",String("hihi.svg"));
                programs->setForPad(i,pad,"text",String());
                if (pad<4) programs->setForPad(i,pad,"padcolor",Colour(0xFF304050).toString());
                else if (pad<8) programs->setForPad(i,pad,"padcolor",Colour(0xFF304060).toString());
                else if (pad<12) programs->setForPad(i,pad,"padcolor",Colour(0xFF304050).toString());
                else  programs->setForPad(i,pad,"padcolor",Colour(0xFF304060).toString());
            }
            setLayout(i,sixteenpads);
            break;
        case 9:
            programs->set(i,"name",String("2 Pads, 12 Sliders"));
            programs->set(i,"squares",14);
            for (int pad=0;pad<14;pad++) {
                programs->setForPad(i,pad,"roundness",0.1f);
                programs->setForPad(i,pad,"showdots",true);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
                programs->setForPad(i,pad,"UseY",true);
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"SendOff",false);
            }
            programs->setForPad(i,0,"UseX",true);
            programs->setForPad(i,7,"UseX",true);
            programs->setForPad(i,0,"roundness",0.f);
            programs->setForPad(i,7,"roundness",0.f);
            setLayout(i,twelvepads);
            break;
        case 10:
            programs->set(i,"name",String("2/5/21"));
            programs->set(i,"squares",28);
            for (int pad=0;pad<28;pad++) {
                programs->setForPad(i,pad,"roundness",0.1f);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
                programs->setForPad(i,pad,"UseY",pad<2 || pad>22);
                programs->setForPad(i,pad,"showdots",pad<2 || pad>22);
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"SendOff",false);
            }
            programs->setForPad(i,0,"UseX",true);
            programs->setForPad(i,1,"UseX",true);
            programs->setForPad(i,0,"roundness",0.f);
            programs->setForPad(i,1,"roundness",0.f);
            setLayout(i,arrangeit28);
            break;
        case 11:
            programs->set(i,"name",String("2/12/25"));
            programs->set(i,"squares",46);
            for (int pad=0;pad<39;pad++) {
                programs->setForPad(i,pad,"roundness",0.1f);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
                programs->setForPad(i,pad,"UseY",pad<14);
                programs->setForPad(i,pad,"showdots",pad<14);
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"SendOff",false);
            }
            programs->setForPad(i,12,"UseX",true);
            programs->setForPad(i,13,"UseX",true);
            programs->setForPad(i,12,"roundness",0.f);
            programs->setForPad(i,13,"roundness",0.f);
            setLayout(i,arrangeit39);
            break;
        case 12:
            programs->set(i,"name",String("2/49"));
            programs->set(i,"squares",52);
            for (int pad=0;pad<51;pad++) {
                programs->setForPad(i,pad,"roundness",0.1f);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
                programs->setForPad(i,pad,"UseY",pad<2);
                programs->setForPad(i,pad,"showdots",pad<2);
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"SendOff",false);
            }
            programs->setForPad(i,0,"UseX",true);
            programs->setForPad(i,1,"UseX",true);
            programs->setForPad(i,0,"roundness",0.f);
            programs->setForPad(i,1,"roundness",0.f);
            setLayout(i,arrangeit51);
            break;
        case 13:
            programs->set(i,"name",String("6 Mixer Blocks"));
            programs->set(i,"squares",48);
            for (int pad=0;pad<48;pad++) {
                programs->setForPad(i,pad,"roundness",0.1f);
                programs->setForPad(i,pad,"icon",String());
                programs->setForPad(i,pad,"text",String());
                programs->setForPad(i,pad,"Ytype",1); //type,CC
                programs->setForPad(i,pad,"SendOff",false);
                switch(pad)
                {
                case 0:
                case 6:
                case 7:
                case 8:
                case 14:
                case 15:
                case 16:
                case 22:
                case 23:
                case 24:
                case 30:
                case 31:
                case 32:
                case 38:
                case 39:
                case 40:
                case 46:
                case 47:
                    //buttons
                    programs->setForPad(i,pad,"showdots",false);
                    programs->setForPad(i,pad,"UseY",false);
                    programs->setForPad(i,pad,"UseX",false);
                    programs->setForPad(i,pad,"toggle",true);
                    programs->setForPad(i,pad,"SendOff",true);
                    break;

                case 1:
                case 2:
                case 4:
                case 5:
                case 9:
                case 10:
                case 12:
                case 13:
                case 17:
                case 18:
                case 20:
                case 21:
                case 25:
                case 26:
                case 28:
                case 29:
                case 33:
                case 34:
                case 36:
                case 37:
                case 41:
                case 42:
                case 44:
                case 45:
                    //vertical sliders
                    programs->setForPad(i,pad,"showdots",true);
                    programs->setForPad(i,pad,"UseY",true);
                    programs->setForPad(i,pad,"UseX",false);
                    break;

                case 3:
                case 11:
                case 19:
                case 27:
                case 35:
                case 43:
                    //horizontal sliders
                    programs->setForPad(i,pad,"showdots",false);
                    programs->setForPad(i,pad,"UseY",false);
                    programs->setForPad(i,pad,"UseX",true);
                    programs->setForPad(i,pad,"Ydata2",63);
                    break;

                default:
                    break;
                }
            }
            setLayout(i,arrangeit48);
            break;
        default:
            setLayout(i,sixteenpads);
            break;
        }
#ifdef _DEBUG
        // File file(presetPath+File::getSeparatorString() + "default" + File::getSeparatorString() + programs->get(i,"name").toString()+".mpads");
        // this->saveXmlPatch(i,file);
#endif
    }
}


void midiPads::copyPadSettings(int source, int dest)
{
    centeredText[dest]   = centeredText[source];
    iconsize[dest]   = iconsize[source];
    roundness[dest]  = roundness[source];
    padcolor[dest]   = padcolor[source];
    showdots[dest]   = showdots[source];
    showvalues[dest] = showvalues[source];
    toggle[dest]     = toggle[source];
    icon[dest]       = icon[source];
    SendOff[dest]    = SendOff[source];
    text[dest]       = text[source];
    UseX[dest]       = UseX[source];
    UseXPB[dest]     = UseXPB[source];
    UseY[dest]       = UseY[source];
    UseYCC[dest]     = UseYCC[source];
    Xcc[dest]        = Xcc[source];
    Xoff[dest]       = Xoff[source];
    Ycc[dest]        = Ycc[source];
    Ydata1[dest]     = Ydata1[source];
    Ydata2[dest]     = Ydata2[source];
    Yoff[dest]       = Yoff[source];
    Ytype[dest]      = Ytype[source];
}
