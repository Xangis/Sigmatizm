# Sigmatizm makefile.  Requires wxWidgets.
# by default, wx-config from the PATH is used
WX_CONFIG := wx-config

# Main executable file
PROGRAM = installer/Sigmatizm.app/Contents/MacOS/Sigmatizm

# Directory containing library portions of code.
INCLUDEDIR2 = /usr/local/include
INCLUDEDIR3 = ../../lib/rtmidi-2.1.0/
LIBDIR = /usr/local/lib

# Object files
OBJECTS = wxMiniWavePanel.o SettingsDlg.o SynthParametersDlg.o wxHarmonicsPanel.o wxkeyboard.o wxkeyboardapp.o $(INCLUDEDIR3)/RtMidi.o ../wxAudioControls/wxSettingsFile.o ../wxAudioControls/wxBitmapSlider.o ../wxAudioControls/wxMidiSettingsDlg.o ../wxAudioControls/wxVolumeMeter.o ../wxAudioControls/wxSwitch.o ../wxAudioControls/wxKeylessChoice.o ../wxAudioControls/wxOctaveCtrl.o ../wxAudioControls/wxBitmapSpinButton.o ../wxAudioControls/wxKeylessButton.o ../wxAudioControls/wxKeylessBitmapButton.o ../StreamingAudioLib/AudioUtil.o ../StreamingAudioLib/Wavetable.o ../wxAudioControls/wxAudioSettings.o ../AudioEffect/DelayEffect.o

CXX = $(shell $(WX_CONFIG) --cxx)
#CXX = $(shell $(WX_CONFIG)) --cxx -ggdb

.SUFFIXES:	.o .cpp

.cpp.o :
#	$(CXX) -g -ggdb -c -D__MACOSX_CORE__ -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<
	$(CXX) -c -O3 -D__MACOSX_CORE__ -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -D__MACOSX_CORE__ -o $(PROGRAM) $(OBJECTS) -L$(LIBDIR) `$(WX_CONFIG) --libs` -framework CoreMidi -framework CoreAudio -framework CoreFoundation -lportaudio

clean: 
	rm -f *.o ../wxAudioControls/*.o ../AudioEffect/*.o $(PROGRAM)
