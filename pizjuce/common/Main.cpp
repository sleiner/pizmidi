#include <memory>

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_audio_plugin_client/juce_audio_plugin_client.h"
#include "juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"
#include "juce_data_structures/juce_data_structures.h"

using namespace juce;

//==============================================================================
/** This is the application object that is started up when Juce starts. It handles
    the initialisation and shutdown of the whole application.
*/
class StandalonePizPlugin : public JUCEApplication
{
    /* Important! NEVER embed objects directly inside your JUCEApplication class! Use
       ONLY pointers to objects, which you should create during the initialise() method
       (NOT in the constructor!) and delete in the shutdown() method (NOT in the
       destructor!)

       This is because the application object gets created before Juce has been properly
       initialised, so any embedded objects would also get constructed too soon.
   */
    std::unique_ptr<ApplicationProperties> appProperties;
    std::unique_ptr<StandaloneFilterWindow> window;

public:
    //==============================================================================
    StandalonePizPlugin()
        : appProperties(nullptr), window(nullptr)
    {
        // NEVER do anything in here that could involve any Juce function being called
        // - leave all your startup tasks until the initialise() method.
    }

    ~StandalonePizPlugin()
    {
        // Your shutdown() method should already have done all the things necessary to
        // clean up this app object, so you should never need to put anything in
        // the destructor.

        // Making any Juce calls in here could be very dangerous...
    }

    PropertiesFile::Options getPropertyStorageOptions() const {
        PropertiesFile::Options options;

        options.applicationName = String(JucePlugin_Name);
        options.filenameSuffix = String("xml");
        options.folderName = File::getSpecialLocation(File::currentExecutableFile)
                                     .getParentDirectory()
                                     .getFullPathName();
        options.osxLibrarySubFolder = String("Application Support/" JucePlugin_Name);
        options.commonToAllUsers = false;
        options.ignoreCaseOfKeyNames = true;
        options.doNotSave = false;
        options.millisecondsBeforeSaving = 2000;
        options.storageFormat = PropertiesFile::storeAsXML;

        return options;
    }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        // just create the main window...
        appProperties = std::make_unique<ApplicationProperties>();
        appProperties->setStorageParameters(getPropertyStorageOptions());

		window = std::make_unique<StandaloneFilterWindow>(JucePlugin_Name,
                                                          Colours::white,
                                                          appProperties->getUserSettings(),
                                                          false);

        window->toFront (true);
        window->setVisible (true);
    }

    void shutdown()
    {
        // clear up..

        window.reset();
        appProperties.reset();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return String(JucePlugin_Name);
    }

    const String getApplicationVersion()
    {
        return String(JucePlugin_VersionString);
    }

    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

    void anotherInstanceStarted (const String& commandLine)
    {
    }
};


//==============================================================================
// This macro creates the application's main() function..
START_JUCE_APPLICATION (StandalonePizPlugin)
