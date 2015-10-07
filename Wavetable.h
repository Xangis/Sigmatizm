#pragma once

#define TABLESIZE 32768
#define NUM_WAVEFORMS 7
#define WAVEFORM_SINE 0
#define WAVEFORM_SQUARE 1
#define WAVEFORM_SAW 2
#define WAVEFORM_TRIANGLE 3
#define WAVEFORM_SINC 4
#define WAVEFORM_PULSE 5
#define WAVEFORM_TRAPEZOID 6
#define PI 3.14159265358979323846

/**
* Stores wavetable data for various waveforms.
*/
class WaveTable
{
public:
	void CreateWavetables();
	float _waveformTable[NUM_WAVEFORMS][TABLESIZE];
    int GetNumWaveforms();
    const char* GetWaveformName(int number);
};