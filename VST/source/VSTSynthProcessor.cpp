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

#include "VSTSynthProcessor.h"
#include "VSTSynthController.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ftypes.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/futils.h"
#include "logscale.h"
#include "SynthParamLimits.h"

// --- Synth Stuff
#define SYNTH_PROC_BLOCKSIZE 32 // 32 samples per processing block = 0.7 mSec = OK for tactile response WP

// --- MIDI Logging -- comment out to disable
#define LOG_MIDI 1

namespace Steinberg {
namespace Vst {
namespace TemplateSynth {

// --- for versioning in serialization
static uint64 TemplateSynthVersion = 0;

// --- the unique identifier (use guidgen.exe to generate)
FUID Processor::cid (0xD57A3323, 0x18A13663, 0xB763124B, 0x38337056);

// this defines a logarithmig scaling for the filter Fc control
LogScale<ParamValue> filterLogScale2 (0.0,		/* VST GUI Variable MIN */
									 1.0,		/* VST GUI Variable MAX */
									 80.0,		/* filter fc LOW */
									 18000.0,	/* filter fc HIGH */
									 0.5,		/* the value at position 0.5 will be: */
									 1800.0);	/* 1800 Hz */
Processor::Processor()
{
	// --- we are a Processor
	setControllerClass(Controller::cid);

	// --- our inits
	//
}

/* 
	Processor::initialize()
	Call the base class
	Add a Stereo Audio Output
	Add a MIDI event inputs (16: one for each channel)
*/
tresult PLUGIN_API Processor::initialize(FUnknown* context)
{
	tresult result = AudioEffect::initialize(context);
	if (result == kResultTrue)
	{
		// stereo output bus
		addAudioOutput(STR16 ("Audio Output"), SpeakerArr::kStereo);

		// MIDI event input bus, 16 channels
		addEventInput(STR16 ("Event Input"), 16);
	}

	return result;
}
/* 
	Processor::setState()
	This is the READ part of the serialization process. We get the stream interface and use it
	to read from the filestream.

	NOTE: The datatypes/read order must EXACTLY match the getState() version or crashes may happen or variables
	      not initialized properly.
*/
tresult PLUGIN_API Processor::setState(IBStream* fileStream)
{
	IBStreamer s(fileStream, kLittleEndian);
	uint64 version = 0;

	// --- needed to convert to our UINT reads
	uint32 udata = 0;
	int32 data = 0;

	// --- read the version
	if(!s.readInt64u(version)) return kResultFalse;

	// --- do next version...
	if (version >= 1)
	{
		// add v1 stuff here
	}
	return kResultTrue;
}

/* 
	Processor::getState()
	This is the WRITE part of the serialization process. We get the stream interface and use it
	to write to the filestream. This is important because it is how the Factory Default is set
	at startup, as well as when writing presets.
*/
tresult PLUGIN_API Processor::getState(IBStream* fileStream)
{
	// --- get a stream I/F
	IBStreamer s(fileStream, kLittleEndian);

	// --- TemplateSynthVersion - place this at top so versioning can be used during the READ operation
	if(!s.writeInt64u(TemplateSynthVersion)) return kResultFalse;

	return kResultTrue;
}

/* 
	Processor::setBusArrangements()
	Client queries us for our supported Busses; this is where you can modify to support mono, surround, etc...
*/
tresult PLUGIN_API Processor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, 
												   SpeakerArrangement* outputs, int32 numOuts)
{
	// we only support one stereo output bus
	if (numIns == 0 && numOuts == 1 && outputs[0] == SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
	}
	return kResultFalse;
}

/* 
	Processor::canProcessSampleSize()
	Client queries us for our supported sample lengths
*/
tresult PLUGIN_API Processor::canProcessSampleSize(int32 symbolicSampleSize)
{
	// this is a challenge in the book; here is where you say you support it but
	// you will need to deal with different buffers in the process() method
//	if (symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64)

	// --- currently 32 bit only
	if (symbolicSampleSize == kSample32)
	{
		return kResultTrue;
	}
	return kResultFalse;
}

/* 
	Processor::loadSamples()
	For sample-based synths, this loads wave file guts into buffers
	Not used (and removed) for non-sample-based synths
*/
bool Processor::loadSamples()
{
	// get our DLL path with my helper function; remember to delete when done
	char* pDLLPath = getMyDLLDirectory(USTRING("TemplateSynth.vst3"));

	// show the wait cursor; this may be a lenghty process but we
	// are frontloading the file reads
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	ShowCursor(true);

	// --- LOAD SAMPLES HERE ---


	// --- when done:
	delete [] pDLLPath;

	return true;
}

/* 
	Processor::setActive()
	This is the analog of prepareForPlay() in RAFX since the Sample Rate is now set.

	VST3 plugins may be turned on or off; you are supposed to dynamically delare stuff when activated
	then delete when de-activated. So, we handle our voice stack here. 
*/
tresult PLUGIN_API Processor::setActive(TBool state)
{
	if(state)
	{
		// --- do ON stuff; dynamic allocations
	}
	else
	{
		// --- do OFF stuff; delete stuff allocated above
	}

	// --- base class method call is last
	return AudioEffect::setActive (state);
}

/* 
	Processor::update()
	Brute force update of synth parameters
*/
void Processor::update()
{
	


}

/* 
	Processor::doControlUpdate()
	Find and issue Control Changes (same as userInterfaceChange() in RAFX)
	returns true if a control was changed
*/
bool Processor::doControlUpdate(ProcessData& data)
{
	bool paramChange = false;

	// --- check
	if(!data.inputParameterChanges)
		return paramChange;

	// --- get the param count and setup a loop for processing queue data
	int32 count = data.inputParameterChanges->getParameterCount();
	
	// --- make sure there is something there
	if(count <= 0)
		return paramChange;

	// --- loop
	for(int32 i=0; i<count; i++)
	{
		// get the message queue for ith parameter
		IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);

		if(queue)
		{
			// --- check for control points
			if(queue->getPointCount() <= 0) return false;

			int32 sampleOffset = 0.0;
			ParamValue value = 0.0;
			ParamID pid = queue->getParameterId();

			// this is the same as userInterfaceChange(); these only are updated if a change has 
			// occurred (a control got moved)
			//
			// NOTE: same as with VST sample synth, we are taking the last value in the queue: queue->getPointCount()-1
			//       the value parameter is [0..1] so MUST BE COOKED before using
			//
			// NOTE: These are NOT MIDI Events! You can't get the channel directly
			// TODO: maybe try to make this nearly sample accurate <- from Steinberg...

			// --- get the last point in queue
			if(queue->getPoint(queue->getPointCount()-1, /* last update point */
				sampleOffset,			/* sample offset */
				value) == kResultTrue)	/* value = [0..1] */
			{
				// --- at least one param changed
				paramChange = true;

				switch(pid) // same as RAFX uControlID
				{
					// case MIDI_MODWHEEL: etc...
				}	
			}
		}
	}

	// --- check and update
	// if(paramChange)
	//	update();

	return paramChange;
}

bool Processor::doProcessEvent(Event& vstEvent)
{
	bool noteEvent = false;
	
	// --- process Note On or Note Off messages
	switch(vstEvent.type)
	{
		// --- NOTE ON
		case Event::kNoteOnEvent:
		{
			// --- get the channel/note/vel
			UINT uMIDIChannel = (UINT)vstEvent.noteOn.channel;
			UINT uMIDINote = (UINT)vstEvent.noteOn.pitch;
			UINT uMIDIVelocity = (UINT)(127.0*vstEvent.noteOn.velocity);

			break;
		}

		// --- NOTE OFF
		case Event::kNoteOffEvent:
		{
			// --- get the channel/note/vel
			UINT uMIDIChannel = (UINT)vstEvent.noteOff.channel;
			UINT uMIDINote = (UINT)vstEvent.noteOff.pitch;
			UINT uMIDIVelocity = (UINT)(127.0*vstEvent.noteOff.velocity);

			break;
		}

		// --- polyphonic aftertouch 0xAn
		case Event::kPolyPressureEvent:
		{			
			// --- get the channel
			UINT uMIDIChannel = (UINT)vstEvent.polyPressure.channel;

			break;
		}
	}

	// -- note event occurred?
	return noteEvent;
}

/* 
	Processor::process()
	The most important function handles:
		Control Changes (same as userInterfaceChange() in RAFX)
		Synth voice rendering
		Output GUI Changes (allows you to write back to the GUI, advanced.
*/
tresult PLUGIN_API Processor::process(ProcessData& data)
{
	// --- check for conrol chages and update synth if needed
	doControlUpdate(data);

	// --- we process 32 samples at a time; MIDI events are then accurate to 0.7 mSec
	const int32 kBlockSize = SYNTH_PROC_BLOCKSIZE;
	
	// 32-bit is float
	// if doing a 64-bit version, you need to replace with double*
	// initialize audio output buffers
	float* buffers[OUTPUT_CHANNELS]; // Precision is float - need to change this do DOUBLE if supporting 64 bit

	// 32-bit is float
	// if doing a 64-bit version, you need to replace with double* here too
	for(int i = 0; i < OUTPUT_CHANNELS; i++)
	{
		// data.outputs[0] = BUS 0
		buffers[i] = (float*)data.outputs[0].channelBuffers32[i];
		memset (buffers[i], 0, data.numSamples * sizeof(float));
	}

	// --- total number of samples in the input Buffer
	int32 numSamples = data.numSamples;

	// --- this is used when we need to shove an event into the next block 
	int32 samplesProcessed = 0;

	// --- get our list of events
	IEventList* inputEvents = data.inputEvents;
	Event e = {0};
	Event* eventPtr = 0;
	int32 eventIndex = 0;

	// --- count of events
	int32 numEvents = inputEvents ? inputEvents->getEventCount () : 0;
	
	// get the first event
	if(numEvents)
	{
		inputEvents->getEvent (0, e);
		eventPtr = &e;
	}
	
	while(numSamples > 0)
	{
		// bound the samples to process to BLOCK SIZE (32)
		int32 samplesToProcess = std::min<int32> (kBlockSize, numSamples);
		
		while(eventPtr != 0)
		{
			// --- if the event is not in the current processing block 
			//     then adapt offset for next block
			if (e.sampleOffset > samplesToProcess)
			{
				e.sampleOffset -= samplesToProcess;
				break;
			}

			// --- find MIDI note-on/off and broadcast
			doProcessEvent(e);

			// --- get next event
			eventIndex++;
			if (eventIndex < numEvents)
			{
				if (inputEvents->getEvent (eventIndex, e) == kResultTrue)
				{
					e.sampleOffset -= samplesProcessed;
				}
				else
				{
					eventPtr = 0;
				}
			}
			else
			{
				eventPtr = 0;
			}
		}	

		// the loop - samplesToProcess is more like framesToProcess
		for(int32 j=0; j<samplesToProcess; j++)
		{
			// just clear buffers
			buffers[0][j] = 0.0;	// left
			buffers[1][j] = 0.0;	// right
		}

		// --- update the counter
		for(int i = 0; i < OUTPUT_CHANNELS; i++) 
			buffers[i] += samplesToProcess;

		// --- update the samples processed/to process
		numSamples -= samplesToProcess;
		samplesProcessed += samplesToProcess;

	} // end while (numSamples > 0)

	// can write OUT to the GUI like this:
	if (data.outputParameterChanges)
	{

	}
//	if(getActiveVoiceCount() == 0 && data.numOutputs > 0)
	if(true)
	{
		data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
	}

	return kResultTrue;
}

}}} // namespaces
