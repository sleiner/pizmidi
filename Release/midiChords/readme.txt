Insert Piz Here-> midiChords
(See https://github.com/sleiner/pizmidi for links to source code and other plugins)

Allows mapping of every input MIDI note to 0 or more output notes. 

Welcome to the hastily-compiled documentation copied from KVR forum posts. Anything not discussed here may or may not be documented in GUI tooltips.

To install: put "midiChords.dll" and the "midiChords" folder in your VST folder.

Input/trigger notes are on the bottom; output notes/chords on top. Shift-drag or right-click-drag the trigger keyboard to set bypassed keys (orange). These notes are passed through without being affected by Transpose/Out Channel settings.

The copy/paste buttons use the same format as the saved chords/mappings, so you can copy from the GUI and paste into a text file and vice-versa. Ctrl-click on Paste uses the same pitches as the copied chord, otherwise it's relative to the new trigger note. 

There is also a text box to type in chords directly. It can read four different formats:
- Note names, like "C E G Bb D" for a C9 chord
- Absolute note names, with octave number, i.e. "C3 E3 G3 Bb3 D4"
- Interval names, like "1 3 5 b7 9" or "Root Maj3 P5 Min7 Maj9" etc.
- Semitone offset from trigger note "0 4 7 10 14" (default, see below) 

With any format, to assign channels to notes, use decimal points: "C.1 E.2 G.3 Bb.4 D.5"

The "Paste" button can also use any of these formats directly, if you copied them from a website or other source (so you don't have to paste them into the text box first).

Keyboard View:
Each chord may contain notes for multiple channels. Right-clicking notes on the chord keyboard allows you to choose which channels each note is sent on. The channel slider above the Learn button lets you filter which input channel(s) are learned. This setting is also used for setting chord notes with left-click. 

Guitar View:
Also known as "Generic Fretted Stringed Instrument View." Some presets are provided, or custom setups can be defined, with up to 16 strings in any tuning. Each string is sent on its own MIDI channel, so you may want to use the main Out Channel setting to send them all in one channel. When switching from keyboard to guitar (or when changing tuning or number of strings), midiChords attempts to form a guitar chord using the same pitches. Any notes outside the guitar's range will be deleted.

Guitar definitions are filled in from text files in the "guitars" folder.

Strumming:
Click "Strum" button to toggle strumming on/off. The other strum controls are disabled when Strum is off. Strum direction is set per trigger note; other settings are global.
Strum Direction - Chord notes are sorted by channel first. "Up" will order notes from Channel 1 to Channel 16, then from note 0 to 127. "Down" is the reverse.
Max Time - Sets the maximum time between the first and last notes of the strum. The actual time also depends on the Speed setting.
Speed - controls the speed of the strum, where 0% corresponds to Max Time and 100% is instantaneous.
Vel. Ramp - Applies a change in velocity over the course of the strum. Negative value means the velocity decreases over time.
Accel - Speed up (positive values) or slow down (negative values) over the course of the strum. Overall strum time remains the same.

Presets menu:
Presets are saved to and loaded from the "mappings" folder. Any files in .chords, .fxp, or .xml (Steinerg Chorder presets) formats can be loaded. Presets can also be loaded by drag & drop.

Saved chord shape menu:
The "Chords" menu is built from plain text files in the "chords" folder. "Chords.txt" shows for the Keyboard View; for "guitar" view the chords file that is used is defined in the guitar preset (for example "Guitar Chords.txt" only shows for the Guitar View). "User.txt" is appended at the end for all views. Chord shapes are defined as the difference in semitones from the trigger note, in a space-separated list. Everything to the left of the colon is the chord name displayed in the menu.

Example:

Major Triad: 0 4 7

; comment
# Section header
-- separator

Chords saved from the plugin GUI are added to "chords/User.txt".


Interval reference:

-12: Octave below
-11: M7 below (Inverted m2)
-10: m7 below (Inverted M2)
 -9: M6 below (Inverted m3)
 -8: m6 below (Inverted M3)
 -7: P5 below (Inverted P4)
 -6: b5 below (Inverted b5)
 -5: P4 below (Inverted P5)
 -4: M3 below (Inverted m6)
 -3: m3 below (Inverted M6)
 -2: M2 below (Inverted m7)
 -1: m2 below (Inverted M7)
  0: Unison
  1: Minor 2nd
  2: Major 2nd
  3: Minor 3rd
  4: Major 3rd
  5: Perfect 4th
  6: Flat 5th/Augmented 4th
  7: Perfect 5th
  8: Minor 6th (or #5)
  9: Major 6th
 10: Minor 7th
 11: Major 7th
 12: Octave
 13: Flat 9th
 14: 9th
 15: Minor 10th (or #9)
 16: Major 10th
 17: 11th
 18: Augmented 11th
 19: Perfect 12th
 20: Flat 13th
 21: 13th