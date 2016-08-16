#pragma once

class HarmonicsCallback
{
public:
		virtual void OnHarmonicsChanged( float* initialHarmonics, float* finalHarmonics ) = 0;
};

