//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_controller.h
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

#ifndef __vst_synth_controller__
#define __vst_synth_controller__

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Steinberg {
namespace Vst {
namespace TemplateSynth {

/*
	The Controller handles the GUI and MIDI Mapping details
	In VST3, all non-note messages (ie everything BUT noteOn and noteOff) are 
	handled via a controller mapping. In some cases, we have to create dummy
	controls that are not intended for the final GUI so that we can get these
	messages. WP

	NOTE: Multiple Inheriance
		  EditController - the base controller stuff 
		  IMidiMapping - the MIDI Mapping Interface allowing us to RX MIDI

*/
class Controller: public EditController, public IMidiMapping
{
public:
	// --- EditController Overrides
	tresult PLUGIN_API initialize(FUnknown* context);
	tresult PLUGIN_API terminate();
	
	// --- serialize-read from file
	tresult PLUGIN_API setComponentState(IBStream* fileStream);

	// --- IMidiMapping
	virtual tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/);
	
	// --- create the View object (GUI or Custom GUI)
	virtual IPlugView* PLUGIN_API createView(FIDString name);

	// --- oridinarily not needed; see documentation on Automation for using these
	virtual ParamValue PLUGIN_API normalizedParamToPlain(ParamID id, ParamValue valueNormalized);
	virtual ParamValue PLUGIN_API plainParamToNormalized(ParamID id, ParamValue plainValue);

	// --- Our COM Creating Method
	static FUnknown* createInstance (void*) {return (IEditController*)new Controller();}

	// --- our globally unique ID value
	static FUID cid;

	// --- helper function for serialization
	tresult PLUGIN_API setParamNormalizedFromFile(ParamID tag, ParamValue value);

	// --- define the controller and interface
	OBJ_METHODS (Controller, EditController)
	DEFINE_INTERFACES
		DEF_INTERFACE (IMidiMapping)
	END_DEFINE_INTERFACES (EditController)
	REFCOUNT_METHODS(EditController)
};

}}} // namespaces

#endif
