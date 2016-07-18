#ifndef __synthparamlimits__
#define __synthparamlimits__


//-----------------------------------------------------------------------------
// TemplateSynth Parameters
//-----------------------------------------------------------------------------
enum {     	
	MIDI_PITCHBEND,
	MIDI_MODWHEEL,
	MIDI_VOLUME_CC7,
	MIDI_PAN_CC10,
	MIDI_EXPRESSION_CC11,
	MIDI_SUSTAIN_PEDAL,
	MIDI_CHANNEL_PRESSURE,
	MIDI_ALL_NOTES_OFF,

	NUMBER_OF_SYNTH_PARAMETERS // alsways last
};


// --- define the HI, LO and DEFAULT values for our controls
#define MIN_VOICE_MODE 0
#define MAX_VOICE_MODE 5
#define DEFAULT_VOICE_MODE 0

#define MIN_LOOP_MODE 0
#define MAX_LOOP_MODE 2
#define DEFAULT_LOOP_MODE 0

// VS
#define MIN_PATH_MODE 0
#define MAX_PATH_MODE 3
#define DEFAULT_PATH_MODE 0

#define MIN_OCTAVE -4
#define MAX_OCTAVE +4
#define DEFAULT_OCTAVE 0

#define MIN_HARD_SYNC_RATIO 1.0
#define MAX_HARD_SYNC_RATIO 4.0
#define DEFAULT_HARD_SYNC_RATIO 1.0

#define MIN_NOISE_OSC_AMP_DB -96.0
#define MAX_NOISE_OSC_AMP_DB 0.0
#define DEFAULT_NOISE_OSC_AMP_DB -96.0

#define MIN_PULSE_WIDTH_PCT 1.0
#define MAX_PULSE_WIDTH_PCT 99.0
#define DEFAULT_PULSE_WIDTH_PCT 50.0

#define MIN_DETUNE_CENTS -50.0
#define MAX_DETUNE_CENTS 50.0
#define DEFAULT_DETUNE_CENTS 0.0

#define MIN_PORTAMENTO_TIME_MSEC 0.0
#define MAX_PORTAMENTO_TIME_MSEC 5000.0
#define DEFAULT_PORTAMENTO_TIME_MSEC 0.0

// for all EG Int EXCEPT LFO->PAN Int
#define MIN_BIPOLAR -1.0
#define MAX_BIPOLAR 1.0
#define DEFAULT_BIPOLAR 0.0

#define MIN_UNIPOLAR 0.0
#define MAX_UNIPOLAR 1.0
#define DEFAULT_UNIPOLAR 0.0
#define DEFAULT_UNIPOLAR_HALF 0.5

#define MIN_FILTER_FC 80.0
#define MAX_FILTER_FC 18000.0
#define DEFAULT_FILTER_FC 10000.0
#define FILTER_RAW_MAP 0.5
#define FILTER_COOKED_MAP 1800.0

#define MIN_FILTER_Q 1.0
#define MAX_FILTER_Q 10.0
#define DEFAULT_FILTER_Q 1.0

#define MIN_LFO_RATE 0.02
#define MAX_LFO_RATE 20.0
#define DEFAULT_LFO_RATE 0.5	

#define MIN_SLOW_LFO_RATE 0.02
#define MAX_SLOW_LFO_RATE 2.0
#define DEFAULT_SLOW_LFO_RATE 0.1

#define MIN_LFO_WAVEFORM 0
#define MAX_LFO_WAVEFORM 7
#define DEFAULT_LFO_WAVEFORM 0	

#define MIN_EG_ATTACK_TIME 0.0
#define MAX_EG_ATTACK_TIME 5000.0
#define DEFAULT_EG_ATTACK_TIME 100.0	

#define MIN_EG_DECAY_TIME 0.0
#define MAX_EG_DECAY_TIME 5000.0
#define DEFAULT_EG_DECAY_TIME 100.0	

#define MIN_EG_DECAYRELEASE_TIME 0.0
#define MAX_EG_DECAYRELEASE_TIME 10000.0
#define DEFAULT_EG_DECAYRELEASE_TIME 1000.0	

#define MIN_EG_SUSTAIN_LEVEL 0.0
#define MAX_EG_SUSTAIN_LEVEL 1.0
#define DEFAULT_EG_SUSTAIN_LEVEL 0.707	

#define MIN_EG_RELEASE_TIME 0.0
#define MAX_EG_RELEASE_TIME 10000.0
#define DEFAULT_EG_RELEASE_TIME 2000.0	

#define MIN_OUTPUT_AMPLITUDE_DB -96.0
#define MAX_OUTPUT_AMPLITUDE_DB 24.0
#define DEFAULT_OUTPUT_AMPLITUDE_DB 0.0	

// NOTE these are +/- units, so +/-1 to +/-12 semis
#define MIN_PITCHBEND_RANGE 1
#define MAX_PITCHBEND_RANGE 12
#define DEFAULT_PITCHBEND_RANGE 1	

// on/off switches
#define MIN_ONOFF_SWITCH 0
#define MAX_ONOFF_SWITCH 1
#define DEFAULT_ONOFF_SWITCH 0	

#define MIN_FILTER_KEYTRACK_INTENSITY 0.5
#define MAX_FILTER_KEYTRACK_INTENSITY 2.0
#define DEFAULT_FILTER_KEYTRACK_INTENSITY 0.5

#define MIN_MIDI_VALUE 0
#define MAX_MIDI_VALUE 127
#define DEFAULT_MIDI_VALUE 64

#define DEFAULT_LEGATO_MODE 0
#define DEFAULT_RESET_TO_ZERO 0
#define DEFAULT_FILTER_KEYTRACK 0
#define DEFAULT_VELOCITY_TO_ATTACK 0
#define DEFAULT_NOTE_TO_DECAY 0
#define DEFAULT_MIDI_PITCHBEND 0
#define DEFAULT_MIDI_MODWHEEL 0
#define DEFAULT_MIDI_VOLUME 127
#define DEFAULT_MIDI_PAN 64
#define DEFAULT_MIDI_EXPRESSION 0

#endif	// __synthparamlimits__

