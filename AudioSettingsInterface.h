#pragma once

#include "portaudio.h"

class AudioSettingsInterface
{
public:
	virtual void SelectAudioInputDevice(PaStreamParameters* device) = 0;
	virtual void SelectAudioOutputDevice(PaStreamParameters* device) = 0;
};