//-----------------------------------------------------------------------------
// Project     : MINI SYNTH
// Version     : 3.6.0
// Created by  : Will Pirkle
// Description : Analog-Style Synth
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

#include "VSTSynthController.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "base/source/fstring.h"
#include "logscale.h"
#include "base/source/fstreamer.h"
#include "synthfunctions.h"
#include "synthparamlimits.h" // param limits file

namespace Steinberg {
namespace Vst {
namespace TemplateSynth {

// --- the unique identifier (use guidgen.exe to generate)
FUID Controller::cid (0x845207E6, 0xB5E24721, 0xA7B72D0E, 0x73E24C82);

// this defines a logarithmig scaling for the filter Fc control
LogScale<ParamValue> filterLogScale (MIN_UNIPOLAR,	/* VST GUI Variable MIN (0) */
									 MAX_UNIPOLAR,	/* VST GUI Variable MAX (1) */
									 MIN_FILTER_FC,	/* filter fc LOW */
									 MAX_FILTER_FC,	/* filter fc HIGH */
									 FILTER_RAW_MAP,/* the value at position 0.5 will be: */
									 FILTER_COOKED_MAP);/* 1800 Hz */

/* 
	Controller::initialize()

	This is where we describe the Default GUI controls and link them to control IDs.
	This must be done if you want a Custom GUI too so you have to implement this no matter what.
*/
tresult PLUGIN_API Controller::initialize(FUnknown* context)
{
	// --- base class does its thing
	tresult result = EditController::initialize(context);

	// --- now define the controls
	if (result == kResultTrue)
	{
		// --- Init parameters
		Parameter* param;

		// MIDI Params - these have no knobs in main GUI but do have to appear in default
		// NOTE: this is for VST3 ONLY! Not needed in AU or RAFX
		param = new RangeParameter(USTRING("PitchBend"), MIDI_PITCHBEND, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);

		param = new RangeParameter(USTRING("MIDI Vol"), MIDI_VOLUME_CC7, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);

		param = new RangeParameter(USTRING("MIDI Pan"), MIDI_PAN_CC10, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);

		param = new RangeParameter(USTRING("MIDI Mod Wheel"), MIDI_MODWHEEL, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);

		param = new RangeParameter(USTRING("MIDI Expression"), MIDI_EXPRESSION_CC11, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);
		
		param = new RangeParameter(USTRING("MIDI Channel Pressure"), MIDI_CHANNEL_PRESSURE, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);

		param = new RangeParameter(USTRING("MIDI Sustain Pedal"), MIDI_SUSTAIN_PEDAL, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);

		param = new RangeParameter(USTRING("All Notes Off"), MIDI_ALL_NOTES_OFF, USTRING(""), 
								   MIN_UNIPOLAR, MAX_UNIPOLAR, DEFAULT_UNIPOLAR);
		param->setPrecision(1); // fractional sig digits
		parameters.addParameter(param);
	}

	return kResultTrue;
}

/* 
	Controller::terminate()
	The End.
*/
tresult PLUGIN_API Controller::terminate()
{
	return EditController::terminate();
}

/* 
	Controller::setParamNormalizedFromFile()
	This sets the normalized (raw) parameter value
	from the file's cooked (plain) value

	tag - the Index value of a control
	value - the cooked value from the file
*/
tresult PLUGIN_API Controller::setParamNormalizedFromFile(ParamID tag, ParamValue value)
{
	// --- get the parameter
	Parameter* pParam = EditController::getParameterObject(tag);

	// --- verify pointer
	if(!pParam) return kResultFalse; 

	//  --- convert serialized value to normalized (raw)
	return setParamNormalized(tag, pParam->toNormalized(value));
}

/* 
	Controller::setComponentState()
	This is the serialization-read function so the GUI can
	be updated from a preset or startup.

	fileStream - the IBStream interface from the client
*/
tresult PLUGIN_API Controller::setComponentState(IBStream* fileStream)
{
	// --- make a streamer interface using the 
	//     IBStream* fileStream; this is for PC so
	//     data is LittleEndian
	IBStreamer s(fileStream, kLittleEndian);

	// --- variables for reading
	uint64 version = 0;
	double dDoubleParam = 0;

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

/* --- IMIDIMapping Interface
	Controller::getMidiControllerAssignment()

	The client queries this 129 times for 130 possible control messages, see ivstsmidicontrollers.h for 
	the VST defines for kPitchBend, kCtrlModWheel, etc... for each MIDI Channel in our Event Bus

	We respond with our ControlID value that we'll use to process the MIDI Messages in Processor::process().

	On the default GUI, these controls will actually move with the MIDI messages, but we don't want that on 
	the final UI so that we can have any Modulation Matrix mapping we want.
*/
tresult PLUGIN_API Controller::getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/)
{
	// NOTE: we only have one EventBus(0)
	//       but it has 16 channels on it
	if(busIndex == 0)
	{
		id = -1;
		switch (midiControllerNumber)
		{
			// these messages handled in the Processor::process() method
			case kPitchBend: 
				id = MIDI_PITCHBEND; 
				break;
			case kCtrlModWheel: 
				id = MIDI_MODWHEEL; 
				break;
			case kCtrlVolume: 
				id = MIDI_VOLUME_CC7; 
				break;
			case kCtrlPan: 
				id = MIDI_PAN_CC10; 
				break;
			case kCtrlExpression: 
				id = MIDI_EXPRESSION_CC11; 
				break;
			case kAfterTouch: 
				id = MIDI_CHANNEL_PRESSURE; 
				break;
			case kCtrlSustainOnOff: 
				id = MIDI_SUSTAIN_PEDAL; 
				break;
			case kCtrlAllNotesOff: 
				id = MIDI_ALL_NOTES_OFF; 
				break;
		}
		return id >= 0 ? kResultTrue : kResultFalse;
	}

	return kResultFalse;
}

/* 
	Controller::createView()
	Create our VSTGUI editor from the UIDESC file.
*/
IPlugView* PLUGIN_API Controller::createView(FIDString _name)
{
	ConstString name(_name);
	if(name == ViewType::kEditor)
	{
		// --- create the editor using the .uidesc file (in our Resources, edited with WYSIWYG editor in client)
		return new VST3Editor (this, "Editor", "vstgui.uidesc");
	}
	return 0;
}

/* See Automation in the docs
	Non-linear Scaling  
	If the DSP representation of a value does not scale in a linear way to the exported 
	normalized representation (which can happen when a decibel scale is used for example),
	the edit controller must provide a conversion to a plain representation. 
	This allows the host to move automation data (being in GUI representation) 
	and keep the original value relations intact. 
	(Steinberg::Vst::IEditController::normalizedParamToPlain / Steinberg::Vst::IEditController::plainParamToNormalized). 

	*** NOTE ***
	We do not use these since our controls are linear or logscale-controlled.
	I am just leaving them here in case you need to implement them. See docs.
*/
ParamValue PLUGIN_API Controller::plainParamToNormalized(ParamID tag, ParamValue plainValue)
{
	return EditController::plainParamToNormalized(tag, plainValue);
}
ParamValue PLUGIN_API Controller::normalizedParamToPlain(ParamID tag, ParamValue valueNormalized)
{
	return EditController::normalizedParamToPlain(tag, valueNormalized);
}

}}} // namespaces
