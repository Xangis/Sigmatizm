#pragma once

struct LFOParameters
{
	float lfoFrequency;
	int lfoWaveform;
	bool enabled;
	float depth; // Depth expressed as a range from 0.0 to 1.0
};