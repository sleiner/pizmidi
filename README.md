# pizMidi plugins

This project is a reboot of the legendary [pizMidi plugins](https://web.archive.org/web/20180218071308/http://thepiz.org/plugins/).

## Status

### Quickly tested, seems to work fine

-   AudioToCC
-   BigClock
-   Image
-   KVR Browser
-   midiChordAnalyzer
-   midiChords
-   midiChs
-   midiCurve: Help text is never shown
-   midiIn: Channel selector combobox only shows channel when expanded
-   midiKeyboard: Toggle mode is not working
-   midiMonitor: text of checkboxes is truncated
-   midiOut: Channel selector combobox only shows channel when expanded
-   midiPads
-   midiPBCurve: Help text is never shown
-   midiPCGUI
-   midiStep

### Serious problems are known

-   Middy Morphy: sends nothing, UI looks very weird
-   midiLooper: Does not play back anything

## How to build

### macOS

1. Install **prerequisites**:
    - [Command Line Tools for Xcode](https://developer.apple.com/xcode/resources/), e.g. `xcode-select --install`
    - [CMake](https://cmake.org) >= 3.21, e.g. `brew install cmake` via [Homebrew](https://brew.sh)
    - [Ninja](https://ninja-build.org), e.g. `brew install ninja` via [Homebrew](https://brew.sh)
2. **Clone** this repository:
    ```bash
    git clone https://github.com/sleiner/pizmidi.git
    ```
3. Configure and build via **CMake**:
    ```bash
    # From the repository root:
    cmake -S . --preset=mac-native
    cmake --build --preset=mac-native --parallel
    ```
    This will build the plugins for the CPU architecture your own Mac.
    If you want to build a [universal binary](https://developer.apple.com/documentation/apple-silicon/building-a-universal-macos-binary), just swap `mac-native` for `mac-universal`.
    Note that this will increase your compile times.
4. Build the **package**:
   After the previous step, the plugins lie in your `build/` folder.
   There, they will (most likely) not be found by any [DAW](https://en.wikipedia.org/wiki/Digital_audio_workstation) on your system.
   You can either copy them over manually to `/Library/Audio/Plug-Ins/<Plugin-Format>` or build an installer package which does that for you:
    ```bash
    # From the repository root:
    cmake --build --preset=mac-native --parallel --target package
    ```
    Afterwards, you will find a `.pkg` file under `build/mac-native/packages/`.
    Double-click to execute it and install the plugins on your system.

### Ubuntu

1. Install **prerequisites**:
    - A C compiler, e.g. `clang` or `g++`.
    - [CMake](https://cmake.org) >= 3.21
      Note that on Ubuntu 20.04, this is not provided by the stock `apt` repositories.
      Installation instructions are provided on the CMake website.
    - [Ninja](https://ninja-build.org), e.g. `sudo apt install ninja-build`
    - [Dependencies for JUCE](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md) (a library that all of the plugins depend on):
        ```bash
        sudo apt install libasound2-dev libjack-jackd2-dev \
                         ladspa-sdk \
                         libcurl4-openssl-dev  \
                         libfreetype6-dev \
                         libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
                         libwebkit2gtk-4.0-dev \
                         libglu1-mesa-dev mesa-common-dev
        ```
2. **Clone** this repository:
    ```bash
    git clone https://github.com/sleiner/pizmidi.git
    ```
3. Configure and build via **CMake**:
    ```bash
    # From the repository root:
    cmake -S . --preset=linux-native
    cmake --build --preset=linux-native --parallel
    ```
4. Optionally, you can build a **ZIP file** containing all of the plugins:
    ```bash
    # From the repository root:
    cmake --build --preset=linux-native --parallel --target package
    ```
    After the previous step, the plugins lie in your `build/` folder.
    There, they will (most likely) not be found by any [DAW](https://en.wikipedia.org/wiki/Digital_audio_workstation) on your system.
    You must copy them to one of the folders where your DAW of choice is looking for plugins.
