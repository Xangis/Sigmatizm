#pragma once

class AudioUtil
{
public:
	static double GetFrequencyFromMIDINote(int noteNumber);
    static double ScaleFrequency(double frequency, int numSemitones);
};