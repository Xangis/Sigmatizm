
#if !defined( _OCTAVECALLBACK_H_ )
#define _OCTAVECALLBACK_H_

/**
     @brief     Base class to derive from in order to receive note on/off data from the wxOctaveCtrl control.
     @note      This is required when using the wxOctaveCtrl control.
*/
class OctaveCallback
{
public:
		virtual void PlayNote( int note, bool receivedFromMidi = false ) = 0;
		virtual void StopNote( int note, bool receivedFromMidi = false ) = 0;
		virtual void AllNotesOff( bool receivedFromMidi = false ) = 0;
		virtual void OnRightClick( int note ) = 0;
};

#endif
