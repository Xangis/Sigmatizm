#include "MidiUtil.h"

wxString MidiUtil::GetMidiError(int error)
{
	switch( error )
	{
	case MMSYSERR_ALLOCATED:
		return wxString(_("The specified resource is already allocated."));
	case MMSYSERR_BADDEVICEID:
		return wxString(_("The specified device identifier is out of range."));
	case MMSYSERR_INVALFLAG:
		return wxString(_("The flags specified by dwFlags are invalid."));
	case MMSYSERR_INVALPARAM:
		return wxString(_("The specified pointer or structure is invalid."));
	case MMSYSERR_NOMEM:	
		return wxString(_("The system is unable to allocate or lock memory."));
	case MIDIERR_NODEVICE:
		return wxString(_("No MIDI port was found for the MIDI mapper."));
	case MMSYSERR_INVALHANDLE:
		return wxString(_("The specified device handle is invalid."));
	default:
		return wxString(_("Unknown error."));
	}
}