//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_processor.cpp
// Created by  : Steinberg, 02/2010
// Description : 
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2013, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __vst_synth_processor__
#define __vst_synth_processor__

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "public.sdk/source/vst/vstparameters.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/ustring.h" // for dealing with sample paths

#include "synthfunctions.h"

#define MAX_VOICES 16
#define OUTPUT_CHANNELS 2 // stereo only!

namespace Steinberg {
namespace Vst {
namespace TemplateSynth {

/*
	The  Processor handles the audio processing, in this case voice rendering. It is derived from AudioEffect
	which has much of the functionality hidden in it.
*/
class Processor : public AudioEffect
{
public:
	// --- constructor
	Processor();
	
	// --- One time init to define our I/O 
	tresult PLUGIN_API initialize(FUnknown* context);

	// --- Define the audio I/O we support
	tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);
	
	// --- Define our word-length capabilities (currently 32 bit only)
	tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize);

	// --- Turn on/off; this is equivalent to prepareForPlay() in RAFX
	tresult PLUGIN_API setActive(TBool state);

	// --- Save and load presets from a file stream
	tresult PLUGIN_API setState(IBStream* fileStream);
	tresult PLUGIN_API getState(IBStream* fileStream);

	// --- The all important process method where the audio is rendered
	tresult PLUGIN_API process(ProcessData& data);
	
	// --- our COM creation method
	static FUnknown* createInstance (void*) { return (IAudioProcessor*)new Processor (); }

	// --- our Globally Unique ID
	static FUID cid;

protected:
	// --- functions to reduce size of process()
	bool doControlUpdate(ProcessData& data);

	// --- for MIDI note-on/off
	bool doProcessEvent(Event& vstEvent);
	
	// updates all voices at once
	void update();

	// to load up the samples in new voices
	bool loadSamples();

	// for portamento
	double m_dLastNoteFrequency;

	// our recieve channel
	UINT m_uMidiRxChannel;
	
	// for converting the log filter control value
	Parameter* m_pFilterLogParam;

	// --- your variables here:


	#if defined _WINDOWS || defined _WINDLL
	char* getMyDLLDirectory(UString cPluginName)
	{
	//	TCHAR s("DigiSynth");
		HMODULE hmodule = GetModuleHandle(cPluginName);

		TCHAR dir[MAX_PATH];
		memset(&dir[0], 0, MAX_PATH*sizeof(TCHAR));
		dir[MAX_PATH-1] = '\0';

		if(hmodule)
			GetModuleFileName(hmodule, &dir[0], MAX_PATH);
		else
			return NULL;

		// convert to UString
		UString DLLPath(&dir[0], MAX_PATH);

		char* pFullPath = new char[MAX_PATH];
		char* pDLLRoot = new char[MAX_PATH];

		DLLPath.toAscii(pFullPath, MAX_PATH);

		int nLenDir = strlen(pFullPath);
		int nLenDLL = wcslen(cPluginName) + 1;	// +1 is for trailing backslash
		memcpy(pDLLRoot, pFullPath, nLenDir-nLenDLL);
		pDLLRoot[nLenDir-nLenDLL] = '\0';

		delete [] pFullPath;

		// caller must delete this after use
		return pDLLRoot;
	}
	#endif
};

}}} // namespaces

#endif
