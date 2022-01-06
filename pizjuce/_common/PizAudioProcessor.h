#ifndef PIZAUDIOPROCESSOR_H
#define PIZAUDIOPROCESSOR_H

#ifdef _MSC_VER
 #pragma warning (disable : 4100)
#endif

class ReaProject;

#include "juce_audio_processors/juce_audio_processors.h"

using namespace juce;

class PizAudioProcessor : public AudioProcessor
{
public:
	PizAudioProcessor() :
		AudioProcessor(),
		bottomOctave(-2),
		reaper(false),
		currentPath(((File::getSpecialLocation(File::currentApplicationFile)).getParentDirectory()).getFullPathName())
	{
	}

	int getBottomOctave() {
		if (reaper) {
			int szout = 0;
			int reaperOctave = 0;
			void* p = 0;
			p = get_config_var("midioctoffs",&szout);
			if (p) {
				reaperOctave = *(int*)p;
				bottomOctave = reaperOctave - 2;
			}
		}
		return bottomOctave;
	}

	bool loadDefaultFxb()
	{
		//look for a default bank
		String defaultBank  = currentPath + File::getSeparatorString()
			+ File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + ".fxb";
		if (File(defaultBank).exists()) {
			juce::MemoryBlock bank = juce::MemoryBlock(0,true);
			File(defaultBank).loadFileAsData(bank);
			bank.removeSection(0,0xA0);
			setStateInformation(bank.getData(), (int)bank.getSize());
			return true;
		}
		return false;
	}

	bool loadFxbFile(File file)
	{
		if (file.existsAsFile()) {
			juce::MemoryBlock bank = juce::MemoryBlock(0,true);
			file.loadFileAsData(bank);
			bank.removeSection(0,0xA0);
			setStateInformation(bank.getData(), (int)bank.getSize());
			return true;
		}
		return false;
	}

	bool loadDefaultFxp()
	{
		//look for a default patch
		String defaultBank = currentPath + File::getSeparatorString()
			+ File::getSpecialLocation(File::currentApplicationFile).getFileNameWithoutExtension() + ".fxp";
		if (File(defaultBank).exists()) {
			juce::MemoryBlock bank = juce::MemoryBlock(0,true);
			File(defaultBank).loadFileAsData(bank);
			bank.removeSection(0,0x3C);
			setCurrentProgramStateInformation(bank.getData(), (int)bank.getSize());
			return true;
		}
		return false;
	}

	bool loadFxpFile(File file)
	{
		if (file.existsAsFile()) {
			juce::MemoryBlock bank = juce::MemoryBlock(0,true);
			file.loadFileAsData(bank);
			bank.removeSection(0,0x3C);
			setCurrentProgramStateInformation(bank.getData(), (int)bank.getSize());
			return true;
		}
		return false;
	}

	int bottomOctave;
	String getCurrentPath() {return currentPath;}

	double (*TimeMap2_timeToBeats)(ReaProject *proj, double tpos, int *measures, int *cml, double *fullbeats, int *cdenom);
	double (*GetPlayPosition)();
	double (*GetPlayPosition2)();
	double (*GetCursorPosition)();
	void* (*get_config_var)(const char* name, int* szout);
	bool reaper;
	String hostInfo;

private:
	String currentPath;
};

#endif
