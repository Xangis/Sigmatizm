#pragma once

struct AdsrParameters
{
	int attackTimeMsec;
	int decayTimeMsec;
	int releaseTimeMsec;
	int sustainLevelPercent;
	bool enabled;
};