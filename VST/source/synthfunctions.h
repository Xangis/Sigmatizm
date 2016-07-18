#pragma once
#include "pluginconstants.h"
#include "time.h"
#include "lookuptables.h"
#include <cmath>

#define CONVEX_LIMIT 0.00398107
#define CONCAVE_LIMIT 0.99601893
#define ARC4RANDOMMAX 4294967295 // (2^32 - 1)

#define EXTRACT_BITS(the_val, bits_start, bits_len) ((the_val >> (bits_start - 1)) & ((1 << bits_len) - 1))

//----------------------------------------------------------------
// --- MIDI Constants
//----------------------------------------------------------------
//
// --- CHANNEL VOICE MESSAGES
const unsigned char NOTE_OFF = 0x80;
const unsigned char NOTE_ON = 0x90;
const unsigned char POLY_PRESSURE = 0xA0;
const unsigned char CONTROL_CHANGE = 0xB0;
const unsigned char PROGRAM_CHANGE = 0xC0;
const unsigned char CHANNEL_PRESSURE = 0xD0;
const unsigned char PITCH_BEND = 0xE0;

// --- CONTINUOUS CONTROLLERS
const unsigned char MOD_WHEEL = 0x01;
const unsigned char VOLUME_CC07 = 0x07;
const unsigned char PAN_CC10 = 0x0A;
const unsigned char EXPRESSION_CC11 = 0x0B;
const unsigned char JOYSTICK_X = 0x10;
const unsigned char JOYSTICK_Y = 0x11;
const unsigned char SUSTAIN_PEDAL = 0x40;
const unsigned char RESET_ALL_CONTROLLERS = 0x79;
const unsigned char ALL_NOTES_OFF = 0x7B;

// --- SYSTEM MESSAGES
const unsigned char SYSTEM_EXCLUSIVE = 0xF0;
const unsigned char MIDI_TIME_CODE = 0xF1;
const unsigned char SONG_POSITION_POINTER = 0xF2;
const unsigned char SONG_SELECT = 0xF3;
const unsigned char TUNE_REQUEST = 0xF6;
const unsigned char END_OF_EXCLUSIVE = 0xF7;
const unsigned char TIMING_CLOCK = 0xF8;
const unsigned char START = 0xFA;
const unsigned char CONTINUE = 0xFB;
const unsigned char SToP = 0xFC;
const unsigned char ACTIVE_SENSING = 0xFE;
const unsigned char SYSTEM_RESET = 0xFF;

// --- FOR Synth Projects
enum midiChannels{MIDI_CH_1 = 0, MIDI_CH_2, MIDI_CH_3, MIDI_CH_4, MIDI_CH_5, MIDI_CH_6, MIDI_CH_7,
				  MIDI_CH_8, MIDI_CH_9, MIDI_CH_10, MIDI_CH_11, MIDI_CH_12, MIDI_CH_13,
				  MIDI_CH_14, MIDI_CH_15, MIDI_CH_16, MIDI_CH_ALL};
//----------------------------------------------------------------
// --- END  MIDI Constants
//----------------------------------------------------------------

// --- Modulation Matrix Stuff --- //
enum sources{
	SOURCE_NONE = 0,

	SOURCE_UI_OSC1_FO,
	SOURCE_UI_OSC2_FO,
	SOURCE_UI_TO_OSC3_FO,
	SOURCE_UI_TO_OSC4_FO,
	SOURCE_UI_TO_ALL_OSC_FO,

	SOURCE_UI_FILTER1_FC,
	SOURCE_UI_FILTER2_FC,
	SOURCE_UI_ALL_FILTER_FC,
	SOURCE_UI_FILTER1_Q,
	SOURCE_UI_FILTER2_Q,
	SOURCE_UI_ALL_FILTER_Q,

	SOURCE_LFO1,
	SOURCE_LFO2,
	SOURCE_LFO1Q,					/* QuadPhase outputs */
	SOURCE_LFO2Q,
	SOURCE_EG1,						/* Exponential outputs */
	SOURCE_EG2,
	SOURCE_EG3,
	SOURCE_EG4,
	SOURCE_BIASED_EG1,				/* Exponential outputs with Sustain subtracted*/
	SOURCE_BIASED_EG2,
	SOURCE_BIASED_EG3,
	SOURCE_BIASED_EG4,
	SOURCE_VELOCITY,	
	SOURCE_MIDI_VOLUME_CC07,		/* MIDI Volume */
	SOURCE_MIDI_PAN_CC10,			/* MIDI Pan */
	SOURCE_MIDI_EXPRESSION_CC11,	/* MIDI Expression */
	SOURCE_MODWHEEL,
	SOURCE_PITCHBEND,
	SOURCE_SUSTAIN_PEDAL,
	SOURCE_MIDI_NOTE_NUM, 
	SOURCE_MIDI_JS_X, 
	SOURCE_MIDI_JS_Y, 
	MAX_SOURCES
};

enum destinations{
	DEST_NONE = 0,

	// --- GUI control layer modulation (-1)
	DEST_UI_OSC1_FO,
	DEST_UI_OSC2_FO,
	DEST_UI_TO_OSC3_FO,
	DEST_UI_TO_OSC4_FO,
	DEST_UI_TO_ALL_OSC_FO,

	DEST_UI_FILTER1_FC,
	DEST_UI_FILTER2_FC,
	DEST_UI_ALL_FILTER_FC,
	DEST_UI_FILTER1_Q,
	DEST_UI_FILTER2_Q,
	DEST_UI_ALL_FILTER_Q,

	// --- LAYER 0 DESTINATIONS 
	//     add more L0 destinations in this chunk
	//     see CModulationMatrix::checkDestinationLayer())
	DEST_LFO1_FO, // <- keep this first
	DEST_LFO2_FO,
	DEST_ALL_LFO_FO,
	DEST_LFO1_OUTPUT_AMP,
	DEST_LFO2_OUTPUT_AMP,
	DEST_ALL_LFO_OUTPUT_AMP,
	DEST_EG1_ATTACK_SCALING,
	DEST_EG2_ATTACK_SCALING,
	DEST_EG3_ATTACK_SCALING,
	DEST_EG4_ATTACK_SCALING,
	DEST_ALL_EG_ATTACK_SCALING,
	DEST_EG1_DECAY_SCALING,
	DEST_EG2_DECAY_SCALING,
	DEST_EG3_DECAY_SCALING,
	DEST_EG4_DECAY_SCALING,
	DEST_ALL_EG_DECAY_SCALING,
	DEST_EG1_SUSTAIN_OVERRIDE,
	DEST_EG2_SUSTAIN_OVERRIDE,
	DEST_EG3_SUSTAIN_OVERRIDE,
	DEST_EG4_SUSTAIN_OVERRIDE,
	DEST_ALL_EG_SUSTAIN_OVERRIDE, // <- keep this last
	// --- END OF LAYER 0 DESTINATIONS

	// --- LAYER 1 DESTINATIONS 
	DEST_HARD_SYNC_RATIO,
	DEST_OSC1_FO,
	DEST_OSC2_FO,
	DEST_OSC3_FO,
	DEST_OSC4_FO,
	DEST_ALL_OSC_FO,
	DEST_OSC1_PULSEWIDTH,
	DEST_OSC2_PULSEWIDTH,
	DEST_OSC3_PULSEWIDTH,
	DEST_OSC4_PULSEWIDTH,
	DEST_ALL_OSC_PULSEWIDTH,
	DEST_OSC1_FO_RATIO,
	DEST_OSC2_FO_RATIO,
	DEST_OSC3_FO_RATIO,
	DEST_OSC4_FO_RATIO,
	DEST_ALL_OSC_FO_RATIO,
	DEST_OSC1_OUTPUT_AMP,
	DEST_OSC2_OUTPUT_AMP,
	DEST_OSC3_OUTPUT_AMP,
	DEST_OSC4_OUTPUT_AMP,
	DEST_ALL_OSC_OUTPUT_AMP,
	DEST_FILTER1_FC,
	DEST_FILTER2_FC,
	DEST_ALL_FILTER_FC,
	DEST_FILTER1_KEYTRACK,
	DEST_FILTER2_KEYTRACK,
	DEST_ALL_FILTER_KEYTRACK, // the control value, overriding the GUI
	DEST_FILTER1_Q,
	DEST_FILTER2_Q,
	DEST_ALL_FILTER_Q,
	DEST_VS_AC_AXIS,
	DEST_VS_BD_AXIS,
	DEST_DCA_VELOCITY,
	DEST_DCA_EG,
	DEST_DCA_AMP,
	DEST_DCA_PAN,
	// --- END OF LAYER 1 DESTINATIONS

	MAX_DESTINATIONS
};

enum transform{
	TRANSFORM_NONE = 0,
	TRANSFORM_UNIPOLAR_TO_BIPOLAR,
	TRANSFORM_BIPOLAR_TO_UNIPOLAR,
	TRANSFORM_MIDI_NORMALIZE,
	TRANSFORM_INVERT_MIDI_NORMALIZE,
	TRANSFORM_MIDI_TO_BIPOLAR,
	TRANSFORM_MIDI_TO_PAN,
	TRANSFORM_MIDI_SWITCH,
	TRANSFORM_MIDI_TO_ATTENUATION,
	TRANSFORM_NOTE_NUMBER_TO_FREQUENCY,
	MAX_TRANSFORMS /* not needed? */
};

struct modMatrixRow
{
	// --- index of source
	UINT uSourceIndex;

	// --- index of destination
	UINT uDestinationIndex;

	// --- for GUI -> Control
	double dUIContolValue;

	// --- needs to default to 1.0 in case no GUI control
	double* pModIntensity;	// <- "ucControl" in MMA DLS
	double* pModRange;		// <- "lScale" in MMA DLS

	// --- transform on Source
	UINT uSourceTransform;

	// --- to easily turn on/off a modulation routing
	bool bEnable;
};

struct noteRegion
{
	// note range of the Region
	UINT uMIDINoteLow;
	UINT uMIDINoteHigh;

	// velocity range of the Region
	UINT uMIDIVelocityLow;
	UINT uMIDIVelocityHigh;
};

inline modMatrixRow** createModMatrixCore()
{
	modMatrixRow** pCore = new modMatrixRow*[MAX_SOURCES*MAX_DESTINATIONS];
	memset(pCore, 0, MAX_SOURCES*MAX_DESTINATIONS*sizeof(modMatrixRow*));
	return pCore;
}

inline modMatrixRow* createModMatrixRow(UINT uSource, UINT uDestination, double* pIntensity, double* pRange, UINT uTransform, bool bEnable = true)
{
	modMatrixRow* pRow = new modMatrixRow;
	pRow->uSourceIndex = uSource;
	pRow->uDestinationIndex = uDestination;
	pRow->pModIntensity = pIntensity; // 0->1
	pRow->pModRange = pRange;
	pRow->uSourceTransform = uTransform; // for AmpMod
	pRow->bEnable = bEnable; // on/off
	pRow->dUIContolValue = 0.0;

	return pRow;
}

struct globalOscillatorParams
{
	// --- common
	double dOscFo;
	double dFoRatio;
	double dAmplitude;	// 0->1 from GUI
	double dPulseWidthControl; // from GUI
	int nOctave;			// octave tweak
	int nSemitones;		// semitones tweak
	int nCents;			// cents tweak
	UINT uWaveform;		// to store type

	// --- LFOs
	UINT uLFOMode;		// to store MODE

	// --- CSampleOscillators
	UINT uLoopMode;
};

struct globalFilterParams
{
	// --- the user's cutoff frequency control position
	double dFcControl;
	double dQControl;
	double dAuxControl;
	double dSaturation;
	UINT uFilterType;		
	UINT uNLP;
};

struct globalEGParams
{
	double dAttackTime_mSec;	// att: is a time duration
	double dDecayTime_mSec;	// dcy: is a time to decay 1->0
	double dReleaseTime_mSec;	// rel: is a time to decay 1->0
	double dSustainLevel;
	double dShutdownTime_mSec; // shutdown is a time
	bool bResetToZero;
	bool bLegatoMode;
};

struct globalDCAParams
{
	double dAmplitude_dB;		// the user's control setting in dB
	double dPanControl;
};

struct globalNanoSynthParams
{
	globalOscillatorParams	osc1Params;
	globalOscillatorParams	osc2Params;
	globalOscillatorParams	lfo1Params;
	globalFilterParams		filter1Params;
	globalEGParams			eg1Params;
	globalDCAParams			dcaParams;
};

struct globalVoiceParams
{
	// --- common
	UINT uVoiceMode;
	double dHSRatio;
	double dPortamentoTime_mSec;

	// --- ranges
	double dOscFoPitchBendModRange;
	double dFilterModRange;
	double dAmpModRange;
	double dOscFoModRange;
	double dOscHardSyncModRange;

	// --- intensities
	double dFilterKeyTrackIntensity;

	double dLFO1OscModIntensity;
	double dLFO1HSModIntensity;
	double dLFO1Filter1ModIntensity;
	double dLFO1Filter2ModIntensity;	
	double dLFO1DCAAmpModIntensity;
	double dLFO1DCAPanModIntensity;

	double dLFO2OscModIntensity;
	double dLFO2HSModIntensity;
	double dLFO2Filter1ModIntensity;
	double dLFO2Filter2ModIntensity;	
	double dLFO2DCAAmpModIntensity;
	double dLFO2DCAPanModIntensity;

	double dEG1OscModIntensity;
	double dEG1Filter1ModIntensity;
	double dEG1Filter2ModIntensity;
	double dEG1DCAAmpModIntensity;

	double dEG2OscModIntensity;
	double dEG2Filter1ModIntensity;
	double dEG2Filter2ModIntensity;
	double dEG2DCAAmpModIntensity;

	double dEG3OscModIntensity;
	double dEG3Filter1ModIntensity;
	double dEG3Filter2ModIntensity;
	double dEG3DCAAmpModIntensity;

	double dEG4OscModIntensity;
	double dEG4Filter1ModIntensity;
	double dEG4Filter2ModIntensity;
	double dEG4DCAAmpModIntensity;

	// --- vector synth
	double dOrbitXAmp;
	double dOrbitYAmp;
	double dAmplitude_A;
	double dAmplitude_B;
	double dAmplitude_C;
	double dAmplitude_D;
	double dAmplitude_ACmix;
	double dAmplitude_BDmix;
	UINT uVectorPathMode;

	// --- DX synth
	double dOp1Feedback;
	double dOp2Feedback;
	double dOp3Feedback;
	double dOp4Feedback;
};

struct globalSynthParams
{
	globalVoiceParams		voiceParams;
	globalOscillatorParams	osc1Params;
	globalOscillatorParams	osc2Params;
	globalOscillatorParams	osc3Params;
	globalOscillatorParams	osc4Params;
	globalOscillatorParams	lfo1Params;
	globalOscillatorParams	lfo2Params;
	globalFilterParams		filter1Params;
	globalFilterParams		filter2Params;
	globalEGParams			eg1Params;
	globalEGParams			eg2Params;
	globalEGParams			eg3Params;
	globalEGParams			eg4Params;
	globalDCAParams			dcaParams;
};
	
// Yamaha DX uses 0->99 in a dB fashion which corresponds to DX controls
// I used a DX100 to estimate the breakpoints and wrote this emperically 
// from my own experiments
inline double calculateDXAmplitude(double dDXLevel)
{
	double dDXAmplitude = 0.0;

	// if level = 0, leave it otherwise convert
	if(dDXLevel != 0.0)
	{
		dDXAmplitude = dDXLevel;

		// empirically found using a DX100
		dDXAmplitude -= 99.0;
		dDXAmplitude /= 1.32;

		// set dB-ish version
		dDXAmplitude = (pow(10.0, dDXAmplitude/20.0));
	}

	return dDXAmplitude;
}

// Helpers for advanced users who make their own GUI
//
// returns "plain" or cooked version of fVar
inline float cookVSTGUIVariable(float fMin, float fMax, float fVar)
{
	return (fMax - fMin)*fVar + fMin;
}

// returns normalized version of fVar
inline float convertToVSTGUIVariable(float fMin, float fMax, float fVar)
{
	float fDiff = fMax - fMin;
	float fRawData = (fVar - fMin)/fDiff;
	return fRawData;
}

inline void checkWrapIndex(double& dIndex)
{
	while(dIndex < 0.0)
		dIndex += 1.0;

	while(dIndex >= 1.0)
		dIndex -= 1.0;
}

static inline float fastlog2 (float x)
{
  union { float f; unsigned int i; } vx = { x };
  union { unsigned int i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
  float y = vx.i;
  y *= 1.1920928955078125e-7f;

  return y - 124.22551499f
           - 1.498030302f * mx.f 
           - 1.72587999f / (0.3520887068f + mx.f);
}

static inline float fastpow2 (float p)
{
  float offset = (p < 0) ? 1.0f : 0.0f;
  float clipp = (p < -126) ? -126.0f : p;
  int w = clipp;
  float z = clipp - w + offset;
  union { unsigned int i; float f; } v = { static_cast<unsigned int> ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

  return v.f;
}

static inline float fastexp (float p)
{
  return fastpow2 (1.442695040f * p);
}


static inline float fasttan (float x)
{
  static const float halfpi = 1.5707963267948966f;
  return sin (x) / sin (x + halfpi);
}

static inline float fasttanh (float p)
{
//  return -1.0f + 2.0f / (1.0f + fastexp (-2.0f * p));
  return p/(fabs(2*p)+3/(2+2*p*2*p));
}

inline double fastPow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

inline double semitonesBetweenFrequencies(double dStartFrequency, double dEndFrequency)
{
	return fastlog2(dEndFrequency/dStartFrequency)*12.0;
//	return Log2(dEndFrequency/dStartFrequency)*12.0;
}

/* pitchShiftMultiplier()

	returns a multiplier for a given pitch shift in semitones
	to shift octaves,     call pitchShiftMultiplier(octaveValue*12.0);
	to shift semitones,   call pitchShiftMultiplier(semitonesValue);
	to shift cents,       call pitchShiftMultiplier(centsValue/100.0);
*/
inline double pitchShiftMultiplier(double dPitchShiftSemitones)
{
	if(dPitchShiftSemitones == 0)
		return 1.0;

	// 2^(N/12)
//	return fastPow(2.0, dPitchShiftSemitones/12.0);
	return pow(2.0, dPitchShiftSemitones/12.0);
}

/* paramToTimeCents()

	converts a time parameter (secs) to TimeCents
	See the MMA MIDI DLS Level 1 or 2 Spec

*/
inline double paramToTimeCents(double dParam)
{
	return 1200.0*65536.0*fastlog2(dParam);
}

/* timeCentsToParam()
	
	converts a timeCents to a time (sec) value
	See the MMA MIDI DLS Level 1 or 2 Spec

*/
inline double timeCentsToParam(double dTimeCents)
{
	//return fastpow2(dTimeCents/(1200.0*65536.0));
	// return fastpow2(dTimeCents/(1200.0*65536.0));
	return pow(2.0, dTimeCents/(1200.0*65536.0));
}

/* mmaMIDItoParamScaling()
	
	calculates the scaled version of a parameter based on a MIDI value
	See the MMA MIDI DLS Level 1 or 2 Spec
	
	uMIDIValue = the MIDI (0 -> 127) value to apply
	fParameter = the parameter to scale
	fScaling = the scaling amount, 0 -> 1.0

	Examples:
		Velocity to EG Attack Time: attack time decreases as velocity increases
									uMIDIValue = velocity
									fParameter = attack time (mSec)
									fScaling = Vel to EG Attack Scale

		Key to EG Decay Time: decay shortens as MIDI note number increases
							  uMIDIValue = MIDI note number
							  fParameter = decay time (mSec)
							  fScaling = Vel to EG Attack Scale


*/
inline double mmaMIDItoParamScaling(UINT uMIDIValue, float fParameter, float fScaling)
{
	return timeCentsToParam(paramToTimeCents(fScaling)*(double(uMIDIValue)/128.0) + paramToTimeCents(fParameter));
}

/* mmaMIDItoAtten_dB()
	
	calculates the dB of attenuation according to MMA DLS spec
	
	uMIDIValue = the MIDI (0 -> 127) value to convert
*/
inline double mmaMIDItoAtten_dB(UINT uMIDIValue)
{
	if(uMIDIValue == 0)
		return -96.0; // dB floor

	return 20.0*log10((127.0*127.0)/((float)uMIDIValue*(float)uMIDIValue));
}

/* mmaMIDItoAtten()
	
	calculates the raw attenuation according to MMA DLS spec
	
	uMIDIValue = the MIDI (0 -> 127) value to convert
*/
inline double mmaMIDItoAtten(UINT uMIDIValue)
{
	if(uMIDIValue == 0)
		return 0.0; // floor

	return ((double)uMIDIValue*(double)uMIDIValue)/(127.0*127.0);;
}

/* capChargeTransform()
	
	calculates the expDecayTransform of the input

	dValue = the unipolar (0 -> 1) value to convert

	returns a value from 0 up to 1.0
*/
inline double capChargeTransform(double dValue)
{
	if(dValue == 0.0)
		return 0.0;

	return 1.0 - exp(-6.0*dValue);
}

/* capDischargeTransform()
	
	calculates the capDischargeTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double capDischargeTransform(double dValue)
{
	if(dValue == 1.0)
		return 0.0;

	return exp(-6.0*dValue);
}


/* linearIn_dBTransform()
	
	calculates the linearIn_dBTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double linearIn_dB_AttackTransform(double dValue)
{
	if(dValue == 0.0)
		return 0.0;

	return pow(10.0, ((1.0 - dValue)*-96.0)/20.0);
}

/* linearIn_dBTransform()
	
	calculates the linearIn_dBTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double linearIn_dB_DecayTransform(double dValue)
{
	if(dValue == 1.0)
		return 0.0;

	return pow(10.0, (dValue*-96.0)/20.0);
}


/* convexTransform()
	
	calculates the convexTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double convexTransform(double dValue)
{
	if(dValue <= CONVEX_LIMIT)
		return 0.0;

	return 1.0 + (5.0/12.0)*log10(dValue);
}

/* convexInvertedTransform()
	
	calculates the convexInvertedTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double convexInvertedTransform(double dValue)
{
	if(dValue >= CONCAVE_LIMIT)
		return 0.0;

	return 1.0 + (5.0/12.0)*log10(1.0 - dValue);
}

/* concaveTransform()
	
	calculates the concaveTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double concaveTransform(double dValue)
{
	if(dValue >= CONCAVE_LIMIT)
		return 1.0;

	return -(5.0/12.0)*log10(1.0 - dValue);
}

/* concaveInvertedTransform()
	
	calculates the concaveInvertedTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double concaveInvertedTransform(double dValue)
{
	if(dValue <= CONVEX_LIMIT)
		return 1.0;

	return -(5.0/12.0)*log10(dValue);
}

/* unipolarToBipolar()
	
	calculates the bipolar (-1 -> +1) value from a unipolar (0 -> 1) value
	
	dValue = the value to convert
*/
inline double unipolarToBipolar(double dValue)
{
	return 2.0*dValue - 1.0;
}

/* midiToBipolar()
	
	calculates the bipolar (-1 -> +1) value from a unipolar MIDI (0 -> 127) value
	
	UINT uMIDIValue = the MIDI value to convert
*/
inline double midiToBipolar(UINT uMIDIValue)
{
	return 2.0*(double)uMIDIValue/127.0 - 1.0;
}

/* midiToPanValue()
	
	calculates the pan value (-1 -> +1) value from a unipolar MIDI (0 -> 127) value
	
	UINT uMIDIValue = the MIDI value to convert
*/
inline double midiToPanValue(UINT uMIDIValue)
{
	// see MMA DLS Level 2 Spec; controls are asymmetrical
	if(uMIDIValue == 64)
		return 0.0;
	else if (uMIDIValue <= 1) // 0 or 1
		return -1.0;

	return 2.0*(double)uMIDIValue/127.0 - 1.0;
}

/* bipolarToUnipolar()
	
	calculates the unipolar (0 -> 1) value from a bipolar (-1 -> +1) value
	
	dValue = the value to convert
*/
inline double bipolarToUnipolar(double dValue)
{
	return 0.5*dValue + 0.5;
}

/* midiToUnipolar()
	
	calculates the unipolar (0 -> 1) value from a MIDI (0 -> 127) value
	
	dValue = the value to convert
*/
inline double midiToUnipolar(UINT uMIDIValue)
{
	return (double)uMIDIValue/127.0;
}

/* unipolarToMIDI()
	
	calculates the MIDI (0 -> 127) value from a unipolar (0 -> 1) value
	
	dValue = the value to convert
*/
inline UINT unipolarToMIDI(float fUnipolarValue)
{
	return fUnipolarValue*127.0;
}

/* calculatePanValues()
	
	calculates the left and right pan values
	
	dPanMod = bipolar (-1 -> 1) pan modulation value

	returns are via pass-by-reference mechanism
*/
inline void calculatePanValues(double dPanMod, double& dLeftPanValue, double& dRightPanValue)
{
	//dLeftPanValue = fastcos((pi/4.0)*(dPanMod + 1.0));
	//dRightPanValue = fastsin((pi/4.0)*(dPanMod + 1.0));

	dLeftPanValue = cos((pi/4.0)*(dPanMod + 1.0));
	dRightPanValue = sin((pi/4.0)*(dPanMod + 1.0));

	dLeftPanValue = fmax(dLeftPanValue, (double)0.0);
	dLeftPanValue = fmin(dLeftPanValue, (double)1.0);
	dRightPanValue = fmax(dRightPanValue, (double)0.0);
	dRightPanValue = fmin(dRightPanValue, (double)1.0);
}

/* calcValueVoltOctaveExp()
	
	returns the volt/octave exponential value
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = on the range of 0 -> 1
*/
inline double calcValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue)
{
	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	if(dLowLimit == 0)
		return dControlValue;
	
	// exp control
	return dLowLimit*fastpow2(dControlValue*dOctaves);
}

/* calcInverseValueVoltOctaveExp()
	
	returns a value between 0 -> 1 representing the volt/octave control location
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = value between low and high limit
*/
inline double calcInverseValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue)
{
	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	if(dLowLimit == 0)
		return dControlValue;
	
	return fastlog2(dControlValue/dLowLimit)/dOctaves;
}

inline double log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log((double)2 );  
}


/* calcModulatedValueExp()
	
	returns the new exponentially modulated control value
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = current value of control
	dModValue = moudulator: on the range of 0 -> 1
	bExpUserControl = type of control on GUI; TRUE if user is adjusting a volt/octave control FALSE if linear
*/
inline double calcModulatedValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue, 
											  double dModValue, bool bExpUserControl)
{
	dModValue = bipolarToUnipolar(dModValue);

//	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	double dOctaves = log2(dHighLimit/dLowLimit);
	
	// location of control relative to 50% point
	double dControlOffset = bExpUserControl ? calcInverseValueVoltOctaveExp(dLowLimit, dHighLimit,dControlValue) - 0.5 : ((dControlValue - dLowLimit)/(dHighLimit - dLowLimit)) - 0.5;

	// exp control
//	double dExpControlOffset = dLowLimit*fastpow2(dControlOffset*dOctaves);
	double dExpControlOffset = dLowLimit*pow(2.0, dControlOffset*dOctaves);
	
	// mod exp control
	double dModulatedValueExp = dExpControlOffset*pow(2.0, dModValue*dOctaves);

	// bound
	dModulatedValueExp = fmin(dModulatedValueExp, dHighLimit);
	dModulatedValueExp = fmax(dModulatedValueExp, dLowLimit);

	// return it
	return dModulatedValueExp;
}

/* calcModulatedValueLin()
	
	returns the new linearly modulated control value
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = current value of control
	dModValue = moudulator: on the range of 0 -> 1
*/
inline double calcModulatedValueLin(double dLowLimit, double dHighLimit, double dControlValue, double dModValue)
{	
	// convert mod value to bipolar
	double dModulator = unipolarToBipolar(dModValue)*(dHighLimit - dLowLimit)/2.0;// + dLowLimit; 
	
	double dModulatedValue = dControlValue + dModulator;

	// bound
	dModulatedValue = fmin(dModulatedValue, dHighLimit);
	dModulatedValue = fmax(dModulatedValue, dLowLimit);

	// return it
	return dModulatedValue;
}



inline double doWhiteNoise()
{
    float fNoise = 0.0;
    
#if defined _WINDOWS || defined _WINDLL    
	// fNoise is 0 -> 32767.0
	fNoise = (float)rand();

	// normalize and make bipolar
	fNoise = 2.0*(fNoise/32767.0) - 1.0;
#else
  	// fNoise is 0 -> ARC4RANDOMMAX
	fNoise = (float)arc4random();
    
	// normalize and make bipolar
	fNoise = 2.0*(fNoise/ARC4RANDOMMAX) - 1.0;  
#endif
    
	return fNoise;
}

inline double doPNSequence(UINT& uPNRegister)
{
	// get the bits
	UINT b0 = EXTRACT_BITS(uPNRegister, 1, 1); // 1 = b0 is FIRST bit from right
	UINT b1 = EXTRACT_BITS(uPNRegister, 2, 1); // 1 = b1 is SECOND bit from right
	UINT b27 = EXTRACT_BITS(uPNRegister, 28, 1); // 28 = b27 is 28th bit from right
	UINT b28 = EXTRACT_BITS(uPNRegister, 29, 1); // 29 = b28 is 29th bit from right

	// form the XOR
	UINT b31 = b0^b1^b27^b28;

	// form the mask to OR with the register to load b31
	if(b31 == 1)
		b31 = 0x10000000;

	// shift one bit to right
	uPNRegister >>= 1; 

	// set the b31 bit
	uPNRegister |= b31;

	// convert the output into a floating point number, scaled by experimentation
	// to a range of o to +2.0
	float fOut = (float)(uPNRegister)/((pow((float)2.0,(float)32.0))/16.0);

	// shift down to form a result from -1.0 to +1.0
	fOut -= 1.0;

	return fOut;
}

const double B = 4.0/(float)pi;
const double C = -4.0/((float)pi*(float)pi);
const double P = 0.225;
// http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
// input is -pi to +pi
inline double parabolicSine(double x, bool bHighPrecision = true)
{
    double y = B * x + C * x * fabs(x);

    if(bHighPrecision)
        y = P * (y * fabs(y) - y) + y;   

	return y;
}

const double D = 5.0*(float)pi*(float)pi;
inline double BhaskaraISine(double x)
{
    double d = fabs(x);
	double sgn = d == 0 ? 1.0 : x/fabs(x);
	return 16.0*x*((float)pi - sgn*x)/(D - sgn*4.0*x*((float)pi - sgn*x));
}

#if defined _WINDOWS || defined _WINDLL
inline void copyWaveData(CWaveData* pSourceWaveData, CWaveData* pDestWaveData)
{
	if(!pSourceWaveData) return;

	pDestWaveData->m_pWaveBuffer = new float[pSourceWaveData->m_uSampleCount];

	// copy everything
	for(int i=0; i<(int)pSourceWaveData->m_uSampleCount; i++)
	{
		pDestWaveData->m_pWaveBuffer[i] = pSourceWaveData->m_pWaveBuffer[i];
		pDestWaveData->m_bWaveLoaded = pSourceWaveData->m_bWaveLoaded;
		pDestWaveData->m_uNumChannels = pSourceWaveData->m_uNumChannels;
		pDestWaveData->m_uSampleRate = pSourceWaveData->m_uSampleRate;
		pDestWaveData->m_uSampleCount = pSourceWaveData->m_uSampleCount;
		pDestWaveData->m_uLoopCount = pSourceWaveData->m_uLoopCount;
		pDestWaveData->m_uLoopStartIndex = pSourceWaveData->m_uLoopStartIndex;
		pDestWaveData->m_uLoopEndIndex = pSourceWaveData->m_uLoopEndIndex;
		pDestWaveData->m_uLoopType = pSourceWaveData->m_uLoopType;
		pDestWaveData->m_uMIDINote = pSourceWaveData->m_uMIDINote;
		pDestWaveData->m_uMIDIPitchFraction = pSourceWaveData->m_uMIDIPitchFraction;
		pDestWaveData->m_uSMPTEFormat = pSourceWaveData->m_uSMPTEFormat;
		pDestWaveData->m_uSMPTEOffset = pSourceWaveData->m_uSMPTEOffset;
	}
}
#endif


inline void RotatePoint45CW(double& x1, double& y1)
{
	double x = x1;
	double y = y1;
	x1 = (x + y)/1.41421356;
	y1 = (y - x)/1.41421356;
}

inline void RotatePoint45CCW(double& x1, double& y1)
{
	double x = x1;
	double y = y1;
	x1 = (x - y)/1.41421356;
	y1 = (x + y)/1.41421356;
}

inline void calculateVectorMixValues(double dOriginX, double dOriginY, 
									 double dPointX, double dPointY, 
									 double& dAmag, double& dBmag, double& dCmag, double& dDmag, 
									 double& dACMix, double& dBDMix, int nCellsPerSide, bool bJoystickCoords = true)
{
	double x = dPointX;
	double y = dPointY;

	double dUnitScalar = 1.41421356*nCellsPerSide; // ncells*sqrt(2)	

	if(!bJoystickCoords)
	{
		//RotatePoint(x, y, +45.0);
		RotatePoint45CCW(x, y);

		x /= dUnitScalar;
		y /= dUnitScalar;
	}

	// these are 0->1 unipolar
	dACMix = (x + 1.0)/2.0;
	dBDMix = (y + 1.0)/2.0;

	if(bJoystickCoords)
	{
		dPointX *= dUnitScalar;
		dPointY *= dUnitScalar;
	}

	dPointX += dOriginX;
	dPointY += dOriginY;

	// actual surface is -45 degrees from joystick axes
	if(bJoystickCoords)
		//RotatePoint(dPointX, dPointY, -45.0);
		RotatePoint45CW(dPointX, dPointY);

	dPointX *= 127;
	dPointY *= 127;
 	
	if (dPointX > 127)   /* Limit range to -128 - 127 */
		dPointX = 127;
	else if (dPointX < -128)
		dPointX = -128;
 
	if (dPointY > 127)   /* Limit range to -128 - 127 */
		dPointY = 127;
	else if (dPointY < -128)
		dPointY = -128;

	dPointX += 127;
	dPointY += 127;

	if(dPointX == 127 && dPointY == 127)
	{
		dAmag = 0.25;
		dBmag = 0.25;
		dCmag = 0.25;
		dDmag = 0.25;
		return;
	}

	// the Korg/Sequential Circuits VS Equations
	dBmag = dPointX*dPointY/645;   /* Calculate individual wave % */
	dCmag = dPointX*(255 - dPointY)/645; /* 645=(255^2/100)*127/128 */
	dDmag = (255-dPointX)*(255-dPointY)/645;
	dAmag = 100.0 - dBmag - dCmag - dDmag;
	
	// convert from percent
	dAmag /= 100.0;
	dBmag /= 100.0;
	dCmag /= 100.0;
	dDmag /= 100.0;

	// limit to 1.0 from rounding/truncation
	dAmag = fmin(1.0, dAmag);
	dBmag = fmin(1.0, dBmag);
	dCmag = fmin(1.0, dCmag);
	dDmag = fmin(1.0, dDmag);

	dAmag = fmax(0.0, dAmag);
	dBmag = fmax(0.0, dBmag);
	dCmag = fmax(0.0, dCmag);
	dDmag = fmax(0.0, dDmag);
}

inline void calculateVectorJoystickValues(double dPointX, double dPointY, 
										  double& dAmag, double& dBmag, double& dCmag, double& dDmag, 
										  double& dACMix, double& dBDMix)
{
	// these are 0->1 unipolar
	dACMix = (dPointX + 1.0)/2.0;
	dBDMix = (dPointY + 1.0)/2.0;
	
	dPointX = 127.0*unipolarToBipolar(dPointX);
	dPointY = 127.0*unipolarToBipolar(dPointY);
 	
	if (dPointX > 127)   /* Limit range to -128 - 127 */
		dPointX = 127;
	else if (dPointX < -128)
		dPointX = -128;
 
	if (dPointY > 127)   /* Limit range to -128 - 127 */
		dPointY = 127;
	else if (dPointY < -128)
		dPointY = -128;

	dPointX += 127;
	dPointY += 127;

	if(dPointX == 127 && dPointY == 127)
	{
		dAmag = 0.25;
		dBmag = 0.25;
		dCmag = 0.25;
		dDmag = 0.25;
		return;
	}

	// the Korg/Sequential Circuits VS Equations
	dBmag = dPointX*dPointY/645;   /* Calculate individual wave % */
	dCmag = dPointX*(255 - dPointY)/645; /* 645=(255^2/100)*127/128 */
	dDmag = (255-dPointX)*(255-dPointY)/645;
	dAmag = 100.0 - dBmag - dCmag - dDmag;
	
	// convert from percent
	dAmag /= 100.0;
	dBmag /= 100.0;
	dCmag /= 100.0;
	dDmag /= 100.0;

	// limit to 1.0 from rounding/truncation
	dAmag = fmin(1.0, dAmag);
	dBmag = fmin(1.0, dBmag);
	dCmag = fmin(1.0, dCmag);
	dDmag = fmin(1.0, dDmag);

	dAmag = fmax(0.0, dAmag);
	dBmag = fmax(0.0, dBmag);
	dCmag = fmax(0.0, dCmag);
	dDmag = fmax(0.0, dDmag);
}

// return cell
inline int translateRotorPointToGridCell(double& dOriginX, double& dOriginY, 
										 double& dRotorX, double& dRotorY,
										 int nCellsPerSide, 
										 bool bJoystickCoords = true)
{
	double dUnitScalar = 1.41421356*nCellsPerSide; // ncells*sqrt(2)	

	dRotorX *= dUnitScalar;
	dRotorY *= dUnitScalar;
	
	dOriginX *= dUnitScalar;
	dOriginY *= dUnitScalar;
	
	dRotorX += dOriginX;
	dRotorY += dOriginY;

	// actual surface is -45 degrees from joystick axes
	if(bJoystickCoords)
		// RotatePoint(dRotorX, dRotorY, -45.0);
		RotatePoint45CW(dRotorX, dRotorY);

	// now the testing
	double yL = -nCellsPerSide + 2.0*(nCellsPerSide - 1);
	double yH = nCellsPerSide;

	double xL = -nCellsPerSide;
	double xH = nCellsPerSide - 2.0*(nCellsPerSide - 1);

	dRotorX = fmax(dRotorX, (double)-nCellsPerSide);
	dRotorX = fmin(dRotorX, (double)nCellsPerSide);

	dRotorY = fmax(dRotorY, (double)-nCellsPerSide);
	dRotorY = fmin(dRotorY, (double)nCellsPerSide);

	int row = 0;
	int cell = 0;

	// find row
	for(int i=0; i<nCellsPerSide; i++)
	{
		if(dRotorY >= yL && dRotorY <= yH)
		{
			row = i;
			break;
		}
		yL -= 2.0;
		yH -= 2.0;
	}

	// find col
	for(int i=0; i<nCellsPerSide; i++)
	{
		if(dRotorX >= xL && dRotorX <= xH)
		{
			cell = row*nCellsPerSide + i;
			break;
		}
		xL += 2.0;
		xH += 2.0;
	}

	// now set origin and translated point
	dOriginX = (xL  + xH)/2.0;
	dOriginY = (yL  + yH)/2.0;

	dRotorX -= dOriginX;
	dRotorY -= dOriginY;

	return cell;
}


// 2-sample PolyBLEP
inline double doPolyBLEP_2(double dModulo, double dInc, double dHeight, bool bRisingEdge)
{
	// --- return value
	double dPolyBLEP = 0.0;
	
	// --- LEFT side of discontinuity
	//	   -1 < t < 0
	if(dModulo > 1.0 - dInc)
	{ 		
		// --- calculate distance
		double t = (dModulo - 1.0)/dInc; 

		// --- calculate residual
		dPolyBLEP = dHeight*(t*t + 2.0*t + 1.0); 
	} 
	// --- RIGHT side of discontinuity
	//     0 <= t < 1 
	else if(dModulo < dInc)
	{ 
		// --- calculate distance
		double t = dModulo/dInc; 

		// --- calculate residual
		dPolyBLEP = dHeight*(2.0*t - t*t - 1.0); 
	} 

	// --- subtract for falling, add for rising edge
	if(!bRisingEdge)
		dPolyBLEP *= -1.0;

	return dPolyBLEP; 
} 
	
// --- N Sample BLEP (uPointsPerSide = N)
inline double doBLEP_N(const double* pBLEPTable, double dTableLen, double dModulo, double dInc, double dHeight, 
					   bool bRisingEdge, double dPointsPerSide, bool bInterpolate = false)
{
	// return value
	double dBLEP = 0.0;

	// t = the distance from the discontinuity
	double t = 0.0;

	// --- find the center of table (discontinuity location)
	double dTableCenter = dTableLen/2.0 - 1;
	
	// LEFT side of edge
	// -1 < t < 0
	for(int i = 1; i <= (UINT)dPointsPerSide; i++)
	{
		if(dModulo > 1.0 - (double)i*dInc)
		{ 		
			// --- calculate distance
			t = (dModulo - 1.0)/(dPointsPerSide*dInc); 
			
			// --- get index
			float fIndex = (1.0 + t)*dTableCenter;

			// --- truncation
			if(bInterpolate)
			{
				dBLEP = pBLEPTable[(int)fIndex];
			}
			else
			{
				float fIndex = (1.0 + t)*dTableCenter;
				float frac = fIndex - int(fIndex);
				dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[(int)fIndex+1], frac);
			}
				
			// --- subtract for falling, add for rising edge
			if(!bRisingEdge)
				dBLEP *= -1.0;

			return dHeight*dBLEP;
		}
	}

	// RIGHT side of discontinuity
	// 0 <= t < 1 
	for(int i = 1; i <= (UINT)dPointsPerSide; i++)
	{
		if(dModulo < (double)i*dInc)
		{ 
			// calculate distance
			t = dModulo/(dPointsPerSide*dInc); 
			
			// --- get index
			float fIndex = t*dTableCenter + (dTableCenter + 1.0);

			// truncation
			if(bInterpolate)
			{
				dBLEP = pBLEPTable[(int)fIndex];
			}
			else
			{
				float frac = fIndex - int(fIndex);	
				if((int)fIndex+1 >= dTableLen)
					dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[0], frac);
				else
					dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[(int)fIndex+1], frac);
			}
				
			// subtract for falling, add for rising edge
			if(!bRisingEdge)
				dBLEP *= -1.0;

			return dHeight*dBLEP;
		} 
	}

	return 0.0;
}

const double dTanhTable[4096] = {-0.7615941559557649,-0.7613889639747039,-0.7611836193303799,-0.7609781219504299,-0.7607724717625293,-0.7605666686943914,-0.7603607126737682,-0.7601546036284503,-0.7599483414862672,-0.7597419261750872,-0.7595353576228174,-0.7593286357574048,-0.7591217605068352,-0.7589147317991343,-0.7587075495623672,-0.7585002137246392,-0.7582927242140953,-0.7580850809589211,-0.7578772838873421,-0.7576693329276246,-0.7574612280080754,-0.7572529690570421,-0.7570445560029133,-0.7568359887741188,-0.7566272672991297,-0.7564183915064583,-0.7562093613246588,-0.7560001766823269,-0.7557908375081005,-0.7555813437306594,-0.7553716952787256,-0.7551618920810637,-0.7549519340664807,-0.7547418211638260,-0.7545315533019927,-0.7543211304099163,-0.7541105524165758,-0.7538998192509930,-0.7536889308422342,-0.7534778871194084,-0.7532666880116691,-0.7530553334482134,-0.7528438233582829,-0.7526321576711634,-0.7524203363161851,-0.7522083592227229,-0.7519962263201966,-0.7517839375380707,-0.7515714928058553,-0.7513588920531056,-0.7511461352094219,-0.7509332222044507,-0.7507201529678840,-0.7505069274294598,-0.7502935455189622,-0.7500800071662217,-0.7498663123011150,-0.7496524608535657,-0.7494384527535439,-0.7492242879310671,-0.7490099663161992,-0.7487954878390520,-0.7485808524297846,-0.7483660600186034,-0.7481511105357629,-0.7479360039115653,-0.7477207400763611,-0.7475053189605491,-0.7472897404945762,-0.7470740046089383,-0.7468581112341797,-0.7466420603008940,-0.7464258517397235,-0.7462094854813602,-0.7459929614565453,-0.7457762795960695,-0.7455594398307733,-0.7453424420915475,-0.7451252863093326,-0.7449079724151195,-0.7446905003399496,-0.7444728700149148,-0.7442550813711578,-0.7440371343398724,-0.7438190288523033,-0.7436007648397467,-0.7433823422335500,-0.7431637609651127,-0.7429450209658854,-0.7427261221673713,-0.7425070645011254,-0.7422878478987550,-0.7420684722919201,-0.7418489376123331,-0.7416292437917593,-0.7414093907620172,-0.7411893784549782,-0.7409692068025671,-0.7407488757367622,-0.7405283851895955,-0.7403077350931530,-0.7400869253795743,-0.7398659559810535,-0.7396448268298393,-0.7394235378582345,-0.7392020889985965,-0.7389804801833383,-0.7387587113449271,-0.7385367824158861,-0.7383146933287933,-0.7380924440162827,-0.7378700344110437,-0.7376474644458219,-0.7374247340534188,-0.7372018431666922,-0.7369787917185567,-0.7367555796419831,-0.7365322068699990,-0.7363086733356893,-0.7360849789721956,-0.7358611237127174,-0.7356371074905110,-0.7354129302388911,-0.7351885918912297,-0.7349640923809571,-0.7347394316415615,-0.7345146096065900,-0.7342896262096479,-0.7340644813843991,-0.7338391750645668,-0.7336137071839328,-0.7333880776763387,-0.7331622864756853,-0.7329363335159328,-0.7327102187311015,-0.7324839420552717,-0.7322575034225833,-0.7320309027672374,-0.7318041400234949,-0.7315772151256776,-0.7313501280081685,-0.7311228786054109,-0.7308954668519101,-0.7306678926822324,-0.7304401560310057,-0.7302122568329196,-0.7299841950227258,-0.7297559705352383,-0.7295275833053329,-0.7292990332679482,-0.7290703203580855,-0.7288414445108087,-0.7286124056612449,-0.7283832037445844,-0.7281538386960809,-0.7279243104510516,-0.7276946189448773,-0.7274647641130030,-0.7272347458909377,-0.7270045642142547,-0.7267742190185917,-0.7265437102396511,-0.7263130378132002,-0.7260822016750712,-0.7258512017611616,-0.7256200380074341,-0.7253887103499174,-0.7251572187247055,-0.7249255630679584,-0.7246937433159024,-0.7244617594048300,-0.7242296112711003,-0.7239972988511387,-0.7237648220814380,-0.7235321808985575,-0.7232993752391241,-0.7230664050398320,-0.7228332702374428,-0.7225999707687861,-0.7223665065707595,-0.7221328775803284,-0.7218990837345269,-0.7216651249704575,-0.7214310012252914,-0.7211967124362684,-0.7209622585406978,-0.7207276394759581,-0.7204928551794969,-0.7202579055888317,-0.7200227906415500,-0.7197875102753087,-0.7195520644278355,-0.7193164530369282,-0.7190806760404550,-0.7188447333763554,-0.7186086249826391,-0.7183723507973877,-0.7181359107587534,-0.7178993048049602,-0.7176625328743042,-0.7174255949051525,-0.7171884908359451,-0.7169512206051936,-0.7167137841514826,-0.7164761814134689,-0.7162384123298824,-0.7160004768395258,-0.7157623748812751,-0.7155241063940798,-0.7152856713169628,-0.7150470695890208,-0.7148083011494243,-0.7145693659374184,-0.7143302638923220,-0.7140909949535290,-0.7138515590605077,-0.7136119561528014,-0.7133721861700284,-0.7131322490518826,-0.7128921447381329,-0.7126518731686242,-0.7124114342832773,-0.7121708280220889,-0.7119300543251319,-0.7116891131325557,-0.7114480043845864,-0.7112067280215270,-0.7109652839837570,-0.7107236722117338,-0.7104818926459917,-0.7102399452271427,-0.7099978298958769,-0.7097555465929618,-0.7095130952592434,-0.7092704758356462,-0.7090276882631729,-0.7087847324829052,-0.7085416084360036,-0.7082983160637077,-0.7080548553073367,-0.7078112261082891,-0.7075674284080430,-0.7073234621481566,-0.7070793272702681,-0.7068350237160961,-0.7065905514274397,-0.7063459103461784,-0.7061011004142730,-0.7058561215737651,-0.7056109737667775,-0.7053656569355148,-0.7051201710222629,-0.7048745159693898,-0.7046286917193454,-0.7043826982146618,-0.7041365353979538,-0.7038902032119188,-0.7036437015993367,-0.7033970305030706,-0.7031501898660670,-0.7029031796313556,-0.7026559997420501,-0.7024086501413472,-0.7021611307725286,-0.7019134415789595,-0.7016655825040900,-0.7014175534914542,-0.7011693544846717,-0.7009209854274464,-0.7006724462635682,-0.7004237369369115,-0.7001748573914370,-0.6999258075711909,-0.6996765874203054,-0.6994271968829989,-0.6991776359035761,-0.6989279044264285,-0.6986780023960342,-0.6984279297569582,-0.6981776864538530,-0.6979272724314583,-0.6976766876346011,-0.6974259320081968,-0.6971750054972481,-0.6969239080468463,-0.6966726396021711,-0.6964212001084904,-0.6961695895111614,-0.6959178077556296,-0.6956658547874305,-0.6954137305521881,-0.6951614349956168,-0.6949089680635200,-0.6946563297017918,-0.6944035198564159,-0.6941505384734668,-0.6938973854991092,-0.6936440608795988,-0.6933905645612825,-0.6931368964905977,-0.6928830566140740,-0.6926290448783319,-0.6923748612300842,-0.6921205056161353,-0.6918659779833819,-0.6916112782788133,-0.6913564064495112,-0.6911013624426502,-0.6908461462054976,-0.6905907576854143,-0.6903351968298543,-0.6900794635863655,-0.6898235579025892,-0.6895674797262611,-0.6893112290052110,-0.6890548056873629,-0.6887982097207357,-0.6885414410534430,-0.6882844996336933,-0.6880273854097907,-0.6877700983301343,-0.6875126383432191,-0.6872550053976357,-0.6869971994420712,-0.6867392204253084,-0.6864810682962269,-0.6862227430038029,-0.6859642444971094,-0.6857055727253163,-0.6854467276376912,-0.6851877091835987,-0.6849285173125015,-0.6846691519739597,-0.6844096131176320,-0.6841499006932752,-0.6838900146507442,-0.6836299549399932,-0.6833697215110750,-0.6831093143141416,-0.6828487332994443,-0.6825879784173337,-0.6823270496182605,-0.6820659468527751,-0.6818046700715282,-0.6815432192252705,-0.6812815942648538,-0.6810197951412302,-0.6807578218054530,-0.6804956742086764,-0.6802333523021563,-0.6799708560372501,-0.6797081853654168,-0.6794453402382175,-0.6791823206073157,-0.6789191264244769,-0.6786557576415697,-0.6783922142105652,-0.6781284960835374,-0.6778646032126640,-0.6776005355502257,-0.6773362930486072,-0.6770718756602968,-0.6768072833378870,-0.6765425160340743,-0.6762775737016603,-0.6760124562935507,-0.6757471637627561,-0.6754816960623928,-0.6752160531456818,-0.6749502349659498,-0.6746842414766292,-0.6744180726312586,-0.6741517283834825,-0.6738852086870516,-0.6736185134958236,-0.6733516427637626,-0.6730845964449398,-0.6728173744935336,-0.6725499768638299,-0.6722824035102220,-0.6720146543872111,-0.6717467294494064,-0.6714786286515254,-0.6712103519483940,-0.6709418992949467,-0.6706732706462272,-0.6704044659573876,-0.6701354851836898,-0.6698663282805051,-0.6695969952033143,-0.6693274859077084,-0.6690578003493883,-0.6687879384841654,-0.6685179002679613,-0.6682476856568088,-0.6679772946068513,-0.6677067270743435,-0.6674359830156516,-0.6671650623872530,-0.6668939651457375,-0.6666226912478063,-0.6663512406502731,-0.6660796133100640,-0.6658078091842179,-0.6655358282298862,-0.6652636704043337,-0.6649913356649382,-0.6647188239691910,-0.6644461352746975,-0.6641732695391764,-0.6639002267204610,-0.6636270067764987,-0.6633536096653512,-0.6630800353451956,-0.6628062837743232,-0.6625323549111410,-0.6622582487141713,-0.6619839651420517,-0.6617095041535360,-0.6614348657074937,-0.6611600497629107,-0.6608850562788891,-0.6606098852146480,-0.6603345365295231,-0.6600590101829673,-0.6597833061345507,-0.6595074243439610,-0.6592313647710035,-0.6589551273756014,-0.6586787121177961,-0.6584021189577473,-0.6581253478557333,-0.6578483987721512,-0.6575712716675169,-0.6572939665024657,-0.6570164832377523,-0.6567388218342507,-0.6564609822529551,-0.6561829644549796,-0.6559047684015585,-0.6556263940540467,-0.6553478413739194,-0.6550691103227733,-0.6547902008623255,-0.6545111129544152,-0.6542318465610025,-0.6539524016441695,-0.6536727781661201,-0.6533929760891806,-0.6531129953757994,-0.6528328359885477,-0.6525524978901194,-0.6522719810433315,-0.6519912854111242,-0.6517104109565609,-0.6514293576428291,-0.6511481254332399,-0.6508667142912283,-0.6505851241803540,-0.6503033550643010,-0.6500214069068780,-0.6497392796720186,-0.6494569733237817,-0.6491744878263513,-0.6488918231440374,-0.6486089792412753,-0.6483259560826264,-0.6480427536327788,-0.6477593718565462,-0.6474758107188697,-0.6471920701848168,-0.6469081502195823,-0.6466240507884880,-0.6463397718569834,-0.6460553133906458,-0.6457706753551803,-0.6454858577164199,-0.6452008604403262,-0.6449156834929896,-0.6446303268406285,-0.6443447904495911,-0.6440590742863545,-0.6437731783175249,-0.6434871025098385,-0.6432008468301614,-0.6429144112454894,-0.6426277957229489,-0.6423410002297966,-0.6420540247334198,-0.6417668692013369,-0.6414795336011973,-0.6411920179007818,-0.6409043220680029,-0.6406164460709044,-0.6403283898776626,-0.6400401534565856,-0.6397517367761141,-0.6394631398048213,-0.6391743625114134,-0.6388854048647294,-0.6385962668337418,-0.6383069483875565,-0.6380174494954128,-0.6377277701266844,-0.6374379102508786,-0.6371478698376375,-0.6368576488567374,-0.6365672472780892,-0.6362766650717395,-0.6359859022078692,-0.6356949586567953,-0.6354038343889698,-0.6351125293749810,-0.6348210435855529,-0.6345293769915460,-0.6342375295639571,-0.6339455012739197,-0.6336532920927042,-0.6333609019917180,-0.6330683309425063,-0.6327755789167510,-0.6324826458862723,-0.6321895318230284,-0.6318962366991153,-0.6316027604867678,-0.6313091031583589,-0.6310152646864006,-0.6307212450435439,-0.6304270442025790,-0.6301326621364357,-0.6298380988181833,-0.6295433542210310,-0.6292484283183282,-0.6289533210835643,-0.6286580324903695,-0.6283625625125147,-0.6280669111239117,-0.6277710782986132,-0.6274750640108139,-0.6271788682348494,-0.6268824909451973,-0.6265859321164775,-0.6262891917234519,-0.6259922697410246,-0.6256951661442427,-0.6253978809082961,-0.6251004140085176,-0.6248027654203834,-0.6245049351195131,-0.6242069230816703,-0.6239087292827620,-0.6236103536988398,-0.6233117963060996,-0.6230130570808815,-0.6227141359996707,-0.6224150330390975,-0.6221157481759368,-0.6218162813871098,-0.6215166326496823,-0.6212168019408667,-0.6209167892380212,-0.6206165945186504,-0.6203162177604049,-0.6200156589410826,-0.6197149180386279,-0.6194139950311325,-0.6191128898968351,-0.6188116026141224,-0.6185101331615286,-0.6182084815177357,-0.6179066476615741,-0.6176046315720225,-0.6173024332282083,-0.6170000526094075,-0.6166974896950452,-0.6163947444646958,-0.6160918168980830,-0.6157887069750803,-0.6154854146757109,-0.6151819399801483,-0.6148782828687159,-0.6145744433218880,-0.6142704213202894,-0.6139662168446959,-0.6136618298760341,-0.6133572603953826,-0.6130525083839710,-0.6127475738231807,-0.6124424566945452,-0.6121371569797504,-0.6118316746606342,-0.6115260097191871,-0.6112201621375529,-0.6109141318980279,-0.6106079189830619,-0.6103015233752580,-0.6099949450573732,-0.6096881840123180,-0.6093812402231572,-0.6090741136731099,-0.6087668043455496,-0.6084593122240047,-0.6081516372921582,-0.6078437795338484,-0.6075357389330689,-0.6072275154739689,-0.6069191091408533,-0.6066105199181830,-0.6063017477905751,-0.6059927927428029,-0.6056836547597965,-0.6053743338266425,-0.6050648299285850,-0.6047551430510250,-0.6044452731795207,-0.6041352202997884,-0.6038249843977019,-0.6035145654592934,-0.6032039634707530,-0.6028931784184294,-0.6025822102888299,-0.6022710590686209,-0.6019597247446277,-0.6016482073038348,-0.6013365067333867,-0.6010246230205870,-0.6007125561528997,-0.6004003061179485,-0.6000878729035177,-0.5997752564975523,-0.5994624568881577,-0.5991494740636005,-0.5988363080123083,-0.5985229587228702,-0.5982094261840368,-0.5978957103847207,-0.5975818113139961,-0.5972677289610997,-0.5969534633154305,-0.5966390143665502,-0.5963243821041833,-0.5960095665182170,-0.5956945675987023,-0.5953793853358532,-0.5950640197200476,-0.5947484707418270,-0.5944327383918973,-0.5941168226611282,-0.5938007235405542,-0.5934844410213744,-0.5931679750949528,-0.5928513257528183,-0.5925344929866654,-0.5922174767883538,-0.5919002771499089,-0.5915828940635223,-0.5912653275215514,-0.5909475775165199,-0.5906296440411183,-0.5903115270882035,-0.5899932266507995,-0.5896747427220973,-0.5893560752954554,-0.5890372243643997,-0.5887181899226237,-0.5883989719639891,-0.5880795704825256,-0.5877599854724314,-0.5874402169280728,-0.5871202648439854,-0.5868001292148735,-0.5864798100356103,-0.5861593073012390,-0.5858386210069716,-0.5855177511481904,-0.5851966977204474,-0.5848754607194647,-0.5845540401411352,-0.5842324359815219,-0.5839106482368587,-0.5835886769035504,-0.5832665219781733,-0.5829441834574747,-0.5826216613383733,-0.5822989556179603,-0.5819760662934982,-0.5816529933624218,-0.5813297368223386,-0.5810062966710281,-0.5806826729064433,-0.5803588655267093,-0.5800348745301253,-0.5797106999151630,-0.5793863416804682,-0.5790617998248604,-0.5787370743473328,-0.5784121652470531,-0.5780870725233630,-0.5777617961757793,-0.5774363362039928,-0.5771106926078700,-0.5767848653874520,-0.5764588545429555,-0.5761326600747729,-0.5758062819834720,-0.5754797202697967,-0.5751529749346669,-0.5748260459791792,-0.5744989334046065,-0.5741716372123982,-0.5738441574041809,-0.5735164939817585,-0.5731886469471118,-0.5728606163023993,-0.5725324020499572,-0.5722040041922996,-0.5718754227321187,-0.5715466576722850,-0.5712177090158473,-0.5708885767660337,-0.5705592609262502,-0.5702297615000828,-0.5699000784912962,-0.5695702119038347,-0.5692401617418224,-0.5689099280095629,-0.5685795107115402,-0.5682489098524185,-0.5679181254370420,-0.5675871574704361,-0.5672560059578067,-0.5669246709045406,-0.5665931523162059,-0.5662614501985523,-0.5659295645575109,-0.5655974953991945,-0.5652652427298978,-0.5649328065560978,-0.5646001868844540,-0.5642673837218081,-0.5639343970751848,-0.5636012269517914,-0.5632678733590185,-0.5629343363044402,-0.5626006157958137,-0.5622667118410801,-0.5619326244483642,-0.5615983536259751,-0.5612638993824060,-0.5609292617263343,-0.5605944406666223,-0.5602594362123173,-0.5599242483726512,-0.5595888771570411,-0.5592533225750896,-0.5589175846365850,-0.5585816633515012,-0.5582455587299978,-0.5579092707824209,-0.5575727995193027,-0.5572361449513620,-0.5568993070895041,-0.5565622859448213,-0.5562250815285929,-0.5558876938522854,-0.5555501229275529,-0.5552123687662368,-0.5548744313803666,-0.5545363107821596,-0.5541980069840212,-0.5538595199985453,-0.5535208498385141,-0.5531819965168989,-0.5528429600468595,-0.5525037404417449,-0.5521643377150933,-0.5518247518806325,-0.5514849829522799,-0.5511450309441425,-0.5508048958705175,-0.5504645777458922,-0.5501240765849441,-0.5497833924025415,-0.5494425252137432,-0.5491014750337989,-0.5487602418781497,-0.5484188257624273,-0.5480772267024554,-0.5477354447142492,-0.5473934798140155,-0.5470513320181533,-0.5467090013432535,-0.5463664878060994,-0.5460237914236671,-0.5456809122131250,-0.5453378501918343,-0.5449946053773499,-0.5446511777874190,-0.5443075674399830,-0.5439637743531762,-0.5436197985453272,-0.5432756400349582,-0.5429312988407856,-0.5425867749817199,-0.5422420684768665,-0.5418971793455250,-0.5415521076071898,-0.5412068532815505,-0.5408614163884917,-0.5405157969480935,-0.5401699949806312,-0.5398240105065760,-0.5394778435465950,-0.5391314941215510,-0.5387849622525034,-0.5384382479607075,-0.5380913512676154,-0.5377442721948762,-0.5373970107643352,-0.5370495669980353,-0.5367019409182163,-0.5363541325473157,-0.5360061419079681,-0.5356579690230064,-0.5353096139154611,-0.5349610766085604,-0.5346123571257315,-0.5342634554905995,-0.5339143717269882,-0.5335651058589201,-0.5332156579106169,-0.5328660279064987,-0.5325162158711857,-0.5321662218294969,-0.5318160458064511,-0.5314656878272670,-0.5311151479173629,-0.5307644261023574,-0.5304135224080693,-0.5300624368605177,-0.5297111694859225,-0.5293597203107041,-0.5290080893614841,-0.5286562766650850,-0.5283042822485304,-0.5279521061390455,-0.5275997483640571,-0.5272472089511935,-0.5268944879282850,-0.5265415853233642,-0.5261885011646653,-0.5258352354806257,-0.5254817882998847,-0.5251281596512845,-0.5247743495638700,-0.5244203580668896,-0.5240661851897942,-0.5237118309622387,-0.5233572954140811,-0.5230025785753830,-0.5226476804764102,-0.5222926011476320,-0.5219373406197223,-0.5215818989235590,-0.5212262760902243,-0.5208704721510056,-0.5205144871373945,-0.5201583210810875,-0.5198019740139868,-0.5194454459681991,-0.5190887369760369,-0.5187318470700181,-0.5183747762828663,-0.5180175246475113,-0.5176600921970882,-0.5173024789649389,-0.5169446849846112,-0.5165867102898598,-0.5162285549146455,-0.5158702188931363,-0.5155117022597069,-0.5151530050489391,-0.5147941272956220,-0.5144350690347520,-0.5140758303015331,-0.5137164111313771,-0.5133568115599032,-0.5129970316229391,-0.5126370713565202,-0.5122769307968904,-0.5119166099805023,-0.5115561089440163,-0.5111954277243023,-0.5108345663584387,-0.5104735248837129,-0.5101123033376217,-0.5097509017578710,-0.5093893201823762,-0.5090275586492622,-0.5086656171968640,-0.5083034958637261,-0.5079411946886034,-0.5075787137104606,-0.5072160529684729,-0.5068532125020261,-0.5064901923507165,-0.5061269925543510,-0.5057636131529477,-0.5054000541867355,-0.5050363156961547,-0.5046723977218568,-0.5043083003047046,-0.5039440234857728,-0.5035795673063477,-0.5032149318079275,-0.5028501170322225,-0.5024851230211549,-0.5021199498168596,-0.5017545974616837,-0.5013890659981868,-0.5010233554691415,-0.5006574659175330,-0.5002913973865596,-0.4999251499196326,-0.4995587235603769,-0.4991921183526304,-0.4988253343404447,-0.4984583715680852,-0.4980912300800308,-0.4977239099209747,-0.4973564111358239,-0.4969887337696996,-0.4966208778679375,-0.4962528434760878,-0.4958846306399150,-0.4955162394053986,-0.4951476698187330,-0.4947789219263274,-0.4944099957748062,-0.4940408914110090,-0.4936716088819910,-0.4933021482350227,-0.4929325095175903,-0.4925626927773958,-0.4921926980623569,-0.4918225254206079,-0.4914521749004985,-0.4910816465505953,-0.4907109404196809,-0.4903400565567546,-0.4899689950110323,-0.4895977558319468,-0.4892263390691477,-0.4888547447725016,-0.4884829729920925,-0.4881110237782214,-0.4877388971814067,-0.4873665932523845,-0.4869941120421085,-0.4866214536017502,-0.4862486179826988,-0.4858756052365617,-0.4855024154151644,-0.4851290485705506,-0.4847555047549824,-0.4843817840209404,-0.4840078864211238,-0.4836338120084505,-0.4832595608360572,-0.4828851329572997,-0.4825105284257529,-0.4821357472952107,-0.4817607896196864,-0.4813856554534127,-0.4810103448508420,-0.4806348578666463,-0.4802591945557170,-0.4798833549731660,-0.4795073391743248,-0.4791311472147452,-0.4787547791501989,-0.4783782350366784,-0.4780015149303962,-0.4776246188877858,-0.4772475469655010,-0.4768702992204166,-0.4764928757096281,-0.4761152764904523,-0.4757375016204267,-0.4753595511573104,-0.4749814251590837,-0.4746031236839481,-0.4742246467903269,-0.4738459945368651,-0.4734671669824292,-0.4730881641861078,-0.4727089862072112,-0.4723296331052720,-0.4719501049400450,-0.4715704017715069,-0.4711905236598574,-0.4708104706655180,-0.4704302428491333,-0.4700498402715703,-0.4696692629939189,-0.4692885110774919,-0.4689075845838250,-0.4685264835746772,-0.4681452081120305,-0.4677637582580901,-0.4673821340752849,-0.4670003356262669,-0.4666183629739120,-0.4662362161813197,-0.4658538953118132,-0.4654714004289396,-0.4650887315964700,-0.4647058888783995,-0.4643228723389475,-0.4639396820425575,-0.4635563180538974,-0.4631727804378596,-0.4627890692595609,-0.4624051845843429,-0.4620211264777717,-0.4616368950056385,-0.4612524902339590,-0.4608679122289744,-0.4604831610571505,-0.4600982367851785,-0.4597131394799749,-0.4593278692086815,-0.4589424260386653,-0.4585568100375193,-0.4581710212730617,-0.4577850598133366,-0.4573989257266138,-0.4570126190813889,-0.4566261399463837,-0.4562394883905457,-0.4558526644830489,-0.4554656682932932,-0.4550784998909048,-0.4546911593457365,-0.4543036467278672,-0.4539159621076028,-0.4535281055554754,-0.4531400771422440,-0.4527518769388943,-0.4523635050166388,-0.4519749614469171,-0.4515862463013957,-0.4511973596519682,-0.4508083015707554,-0.4504190721301051,-0.4500296714025929,-0.4496400994610212,-0.4492503563784205,-0.4488604422280485,-0.4484703570833904,-0.4480801010181593,-0.4476896741062960,-0.4472990764219691,-0.4469083080395753,-0.4465173690337390,-0.4461262594793127,-0.4457349794513772,-0.4453435290252415,-0.4449519082764427,-0.4445601172807461,-0.4441681561141457,-0.4437760248528640,-0.4433837235733517,-0.4429912523522883,-0.4425986112665821,-0.4422058003933697,-0.4418128198100171,-0.4414196695941186,-0.4410263498234978,-0.4406328605762072,-0.4402392019305281,-0.4398453739649713,-0.4394513767582765,-0.4390572103894128,-0.4386628749375786,-0.4382683704822015,-0.4378736971029386,-0.4374788548796765,-0.4370838438925314,-0.4366886642218489,-0.4362933159482045,-0.4358977991524033,-0.4355021139154800,-0.4351062603186994,-0.4347102384435559,-0.4343140483717742,-0.4339176901853086,-0.4335211639663437,-0.4331244697972940,-0.4327276077608042,-0.4323305779397494,-0.4319333804172347,-0.4315360152765955,-0.4311384826013978,-0.4307407824754376,-0.4303429149827418,-0.4299448802075674,-0.4295466782344024,-0.4291483091479647,-0.4287497730332036,-0.4283510699752985,-0.4279522000596598,-0.4275531633719288,-0.4271539599979773,-0.4267545900239083,-0.4263550535360553,-0.4259553506209831,-0.4255554813654873,-0.4251554458565948,-0.4247552441815632,-0.4243548764278814,-0.4239543426832697,-0.4235536430356791,-0.4231527775732921,-0.4227517463845227,-0.4223505495580157,-0.4219491871826477,-0.4215476593475265,-0.4211459661419912,-0.4207441076556126,-0.4203420839781928,-0.4199398951997656,-0.4195375414105962,-0.4191350227011814,-0.4187323391622497,-0.4183294908847614,-0.4179264779599080,-0.4175233004791133,-0.4171199585340324,-0.4167164522165525,-0.4163127816187924,-0.4159089468331029,-0.4155049479520664,-0.4151007850684974,-0.4146964582754423,-0.4142919676661795,-0.4138873133342191,-0.4134824953733035,-0.4130775138774068,-0.4126723689407356,-0.4122670606577281,-0.4118615891230548,-0.4114559544316183,-0.4110501566785532,-0.4106441959592265,-0.4102380723692373,-0.4098317860044166,-0.4094253369608278,-0.4090187253347667,-0.4086119512227611,-0.4082050147215712,-0.4077979159281894,-0.4073906549398403,-0.4069832318539809,-0.4065756467683008,-0.4061678997807214,-0.4057599909893968,-0.4053519204927133,-0.4049436883892897,-0.4045352947779772,-0.4041267397578592,-0.4037180234282517,-0.4033091458887030,-0.4029001072389939,-0.4024909075791375,-0.4020815470093796,-0.4016720256301981,-0.4012623435423036,-0.4008525008466392,-0.4004424976443801,-0.4000323340369344,-0.3996220101259425,-0.3992115260132772,-0.3988008818010438,-0.3983900775915801,-0.3979791134874566,-0.3975679895914760,-0.3971567060066735,-0.3967452628363169,-0.3963336601839064,-0.3959218981531747,-0.3955099768480871,-0.3950978963728412,-0.3946856568318670,-0.3942732583298273,-0.3938607009716171,-0.3934479848623638,-0.3930351101074276,-0.3926220768124007,-0.3922088850831080,-0.3917955350256069,-0.3913820267461869,-0.3909683603513703,-0.3905545359479115,-0.3901405536427973,-0.3897264135432472,-0.3893121157567127,-0.3888976603908777,-0.3884830475536588,-0.3880682773532044,-0.3876533498978957,-0.3872382652963458,-0.3868230236574003,-0.3864076250901372,-0.3859920697038662,-0.3855763576081300,-0.3851604889127029,-0.3847444637275917,-0.3843282821630352,-0.3839119443295045,-0.3834954503377028,-0.3830788002985653,-0.3826619943232595,-0.3822450325231847,-0.3818279150099725,-0.3814106418954864,-0.3809932132918219,-0.3805756293113065,-0.3801578900664995,-0.3797399956701923,-0.3793219462354082,-0.3789037418754023,-0.3784853827036614,-0.3780668688339042,-0.3776482003800814,-0.3772293774563751,-0.3768104001771993,-0.3763912686571996,-0.3759719830112533,-0.3755525433544693,-0.3751329498021880,-0.3747132024699816,-0.3742933014736535,-0.3738732469292387,-0.3734530389530038,-0.3730326776614464,-0.3726121631712959,-0.3721914955995128,-0.3717706750632888,-0.3713497016800471,-0.3709285755674420,-0.3705072968433588,-0.3700858656259141,-0.3696642820334556,-0.3692425461845619,-0.3688206581980427,-0.3683986181929386,-0.3679764262885211,-0.3675540826042926,-0.3671315872599862,-0.3667089403755660,-0.3662861420712265,-0.3658631924673930,-0.3654400916847216,-0.3650168398440987,-0.3645934370666413,-0.3641698834736969,-0.3637461791868435,-0.3633223243278893,-0.3628983190188728,-0.3624741633820628,-0.3620498575399586,-0.3616254016152890,-0.3612007957310136,-0.3607760400103213,-0.3603511345766317,-0.3599260795535938,-0.3595008750650866,-0.3590755212352191,-0.3586500181883297,-0.3582243660489867,-0.3577985649419879,-0.3573726149923608,-0.3569465163253623,-0.3565202690664787,-0.3560938733414257,-0.3556673292761482,-0.3552406369968206,-0.3548137966298461,-0.3543868083018573,-0.3539596721397156,-0.3535323882705116,-0.3531049568215647,-0.3526773779204231,-0.3522496516948636,-0.3518217782728920,-0.3513937577827425,-0.3509655903528779,-0.3505372761119895,-0.3501088151889970,-0.3496802077130481,-0.3492514538135194,-0.3488225536200150,-0.3483935072623675,-0.3479643148706373,-0.3475349765751130,-0.3471054925063107,-0.3466758627949745,-0.3462460875720763,-0.3458161669688154,-0.3453861011166187,-0.3449558901471406,-0.3445255341922627,-0.3440950333840943,-0.3436643878549715,-0.3432335977374577,-0.3428026631643432,-0.3423715842686454,-0.3419403611836085,-0.3415089940427035,-0.3410774829796281,-0.3406458281283065,-0.3402140296228894,-0.3397820875977542,-0.3393500021875042,-0.3389177735269692,-0.3384854017512052,-0.3380528869954940,-0.3376202293953435,-0.3371874290864875,-0.3367544862048855,-0.3363214008867227,-0.3358881732684099,-0.3354548034865832,-0.3350212916781044,-0.3345876379800601,-0.3341538425297627,-0.3337199054647491,-0.3332858269227816,-0.3328516070418471,-0.3324172459601574,-0.3319827438161489,-0.3315481007484827,-0.3311133168960443,-0.3306783923979434,-0.3302433273935143,-0.3298081220223150,-0.3293727764241280,-0.3289372907389594,-0.3285016651070392,-0.3280658996688213,-0.3276299945649829,-0.3271939499364249,-0.3267577659242715,-0.3263214426698703,-0.3258849803147919,-0.3254483790008299,-0.3250116388700011,-0.3245747600645449,-0.3241377427269234,-0.3237005869998213,-0.3232632930261459,-0.3228258609490267,-0.3223882909118154,-0.3219505830580861,-0.3215127375316344,-0.3210747544764782,-0.3206366340368571,-0.3201983763572320,-0.3197599815822856,-0.3193214498569221,-0.3188827813262666,-0.3184439761356655,-0.3180050344306863,-0.3175659563571172,-0.3171267420609674,-0.3166873916884663,-0.3162479053860643,-0.3158082833004321,-0.3153685255784602,-0.3149286323672596,-0.3144886038141612,-0.3140484400667157,-0.3136081412726935,-0.3131677075800847,-0.3127271391370988,-0.3122864360921644,-0.3118455985939297,-0.3114046267912615,-0.3109635208332459,-0.3105222808691873,-0.3100809070486093,-0.3096393995212535,-0.3091977584370801,-0.3087559839462675,-0.3083140761992121,-0.3078720353465282,-0.3074298615390480,-0.3069875549278213,-0.3065451156641153,-0.3061025438994147,-0.3056598397854213,-0.3052170034740540,-0.3047740351174487,-0.3043309348679579,-0.3038877028781508,-0.3034443393008131,-0.3030008442889469,-0.3025572179957703,-0.3021134605747174,-0.3016695721794385,-0.3012255529637992,-0.3007814030818809,-0.3003371226879803,-0.2998927119366095,-0.2994481709824956,-0.2990034999805806,-0.2985586990860214,-0.2981137684541895,-0.2976687082406707,-0.2972235186012655,-0.2967781996919881,-0.2963327516690670,-0.2958871746889445,-0.2954414689082765,-0.2949956344839324,-0.2945496715729949,-0.2941035803327601,-0.2936573609207369,-0.2932110134946471,-0.2927645382124252,-0.2923179352322184,-0.2918712047123859,-0.2914243468114995,-0.2909773616883425,-0.2905302495019106,-0.2900830104114108,-0.2896356445762618,-0.2891881521560935,-0.2887405333107471,-0.2882927882002744,-0.2878449169849389,-0.2873969198252138,-0.2869487968817832,-0.2865005483155415,-0.2860521742875930,-0.2856036749592523,-0.2851550504920434,-0.2847063010477002,-0.2842574267881656,-0.2838084278755921,-0.2833593044723411,-0.2829100567409829,-0.2824606848442963,-0.2820111889452688,-0.2815615692070962,-0.2811118257931823,-0.2806619588671390,-0.2802119685927857,-0.2797618551341498,-0.2793116186554656,-0.2788612593211749,-0.2784107772959265,-0.2779601727445759,-0.2775094458321853,-0.2770585967240232,-0.2766076255855647,-0.2761565325824905,-0.2757053178806875,-0.2752539816462480,-0.2748025240454702,-0.2743509452448571,-0.2738992454111170,-0.2734474247111631,-0.2729954833121134,-0.2725434213812901,-0.2720912390862199,-0.2716389365946336,-0.2711865140744658,-0.2707339716938549,-0.2702813096211427,-0.2698285280248744,-0.2693756270737981,-0.2689226069368652,-0.2684694677832292,-0.2680162097822466,-0.2675628331034757,-0.2671093379166775,-0.2666557243918141,-0.2662019926990499,-0.2657481430087504,-0.2652941754914825,-0.2648400903180139,-0.2643858876593135,-0.2639315676865504,-0.2634771305710942,-0.2630225764845150,-0.2625679055985825,-0.2621131180852663,-0.2616582141167355,-0.2612031938653586,-0.2607480575037031,-0.2602928052045355,-0.2598374371408210,-0.2593819534857231,-0.2589263544126037,-0.2584706400950227,-0.2580148107067377,-0.2575588664217040,-0.2571028074140741,-0.2566466338581979,-0.2561903459286219,-0.2557339438000897,-0.2552774276475409,-0.2548207976461117,-0.2543640539711340,-0.2539071967981360,-0.2534502263028409,-0.2529931426611676,-0.2525359460492299,-0.2520786366433366,-0.2516212146199912,-0.2511636801558916,-0.2507060334279297,-0.2502482746131916,-0.2497904038889569,-0.2493324214326990,-0.2488743274220843,-0.2484161220349722,-0.2479578054494153,-0.2474993778436581,-0.2470408393961381,-0.2465821902854843,-0.2461234306905180,-0.2456645607902517,-0.2452055807638897,-0.2447464907908270,-0.2442872910506498,-0.2438279817231349,-0.2433685629882493,-0.2429090350261503,-0.2424493980171852,-0.2419896521418909,-0.2415297975809937,-0.2410698345154091,-0.2406097631262415,-0.2401495835947840,-0.2396892961025184,-0.2392289008311142,-0.2387683979624293,-0.2383077876785091,-0.2378470701615865,-0.2373862455940815,-0.2369253141586012,-0.2364642760379392,-0.2360031314150758,-0.2355418804731773,-0.2350805233955960,-0.2346190603658698,-0.2341574915677222,-0.2336958171850617,-0.2332340374019817,-0.2327721524027605,-0.2323101623718606,-0.2318480674939286,-0.2313858679537951,-0.2309235639364743,-0.2304611556271637,-0.2299986432112438,-0.2295360268742783,-0.2290733068020132,-0.2286104831803768,-0.2281475561954795,-0.2276845260336136,-0.2272213928812527,-0.2267581569250518,-0.2262948183518467,-0.2258313773486543,-0.2253678341026716,-0.2249041888012759,-0.2244404416320244,-0.2239765927826540,-0.2235126424410809,-0.2230485907954003,-0.2225844380338866,-0.2221201843449923,-0.2216558299173485,-0.2211913749397642,-0.2207268196012262,-0.2202621640908986,-0.2197974085981229,-0.2193325533124173,-0.2188675984234768,-0.2184025441211726,-0.2179373905955523,-0.2174721380368388,-0.2170067866354309,-0.2165413365819026,-0.2160757880670027,-0.2156101412816547,-0.2151443964169567,-0.2146785536641806,-0.2142126132147724,-0.2137465752603516,-0.2132804399927107,-0.2128142076038156,-0.2123478782858044,-0.2118814522309881,-0.2114149296318495,-0.2109483106810433,-0.2104815955713957,-0.2100147844959042,-0.2095478776477373,-0.2090808752202341,-0.2086137774069041,-0.2081465844014269,-0.2076792963976519,-0.2072119135895979,-0.2067444361714531,-0.2062768643375744,-0.2058091982824876,-0.2053414382008865,-0.2048735842876331,-0.2044056367377571,-0.2039375957464558,-0.2034694615090933,-0.2030012342212008,-0.2025329140784760,-0.2020645012767827,-0.2015959960121509,-0.2011273984807760,-0.2006587088790188,-0.2001899274034050,-0.1997210542506255,-0.1992520896175352,-0.1987830337011532,-0.1983138866986625,-0.1978446488074095,-0.1973753202249040,-0.1969059011488185,-0.1964363917769882,-0.1959667923074105,-0.1954971029382451,-0.1950273238678129,-0.1945574552945964,-0.1940874974172393,-0.1936174504345457,-0.1931473145454804,-0.1926770899491682,-0.1922067768448937,-0.1917363754321011,-0.1912658859103937,-0.1907953084795336,-0.1903246433394416,-0.1898538906901965,-0.1893830507320354,-0.1889121236653527,-0.1884411096907000,-0.1879700090087862,-0.1874988218204766,-0.1870275483267929,-0.1865561887289128,-0.1860847432281698,-0.1856132120260526,-0.1851415953242050,-0.1846698933244255,-0.1841981062286672,-0.1837262342390370,-0.1832542775577958,-0.1827822363873576,-0.1823101109302900,-0.1818379013893129,-0.1813656079672989,-0.1808932308672728,-0.1804207702924109,-0.1799482264460414,-0.1794755995316433,-0.1790028897528465,-0.1785300973134314,-0.1780572224173286,-0.1775842652686186,-0.1771112260715313,-0.1766381050304456,-0.1761649023498895,-0.1756916182345394,-0.1752182528892199,-0.1747448065189034,-0.1742712793287097,-0.1737976715239059,-0.1733239833099059,-0.1728502148922701,-0.1723763664767050,-0.1719024382690629,-0.1714284304753418,-0.1709543433016844,-0.1704801769543787,-0.1700059316398567,-0.1695316075646951,-0.1690572049356137,-0.1685827239594763,-0.1681081648432894,-0.1676335277942026,-0.1671588130195076,-0.1666840207266384,-0.1662091511231705,-0.1657342044168211,-0.1652591808154482,-0.1647840805270505,-0.1643089037597671,-0.1638336507218772,-0.1633583216217995,-0.1628829166680919,-0.1624074360694515,-0.1619318800347140,-0.1614562487728532,-0.1609805424929809,-0.1605047614043464,-0.1600289057163362,-0.1595529756384738,-0.1590769713804190,-0.1586008931519680,-0.1581247411630525,-0.1576485156237398,-0.1571722167442324,-0.1566958447348673,-0.1562193998061161,-0.1557428821685843,-0.1552662920330111,-0.1547896296102689,-0.1543128951113633,-0.1538360887474324,-0.1533592107297464,-0.1528822612697075,-0.1524052405788494,-0.1519281488688370,-0.1514509863514660,-0.1509737532386625,-0.1504964497424827,-0.1500190760751125,-0.1495416324488671,-0.1490641190761909,-0.1485865361696568,-0.1481088839419658,-0.1476311626059471,-0.1471533723745573,-0.1466755134608802,-0.1461975860781263,-0.1457195904396327,-0.1452415267588624,-0.1447633952494044,-0.1442851961249728,-0.1438069295994066,-0.1433285958866696,-0.1428501952008498,-0.1423717277561590,-0.1418931937669326,-0.1414145934476289,-0.1409359270128293,-0.1404571946772371,-0.1399783966556782,-0.1394995331630996,-0.1390206044145699,-0.1385416106252786,-0.1380625520105354,-0.1375834287857705,-0.1371042411665337,-0.1366249893684941,-0.1361456736074401,-0.1356662940992785,-0.1351868510600343,-0.1347073447058508,-0.1342277752529883,-0.1337481429178246,-0.1332684479168541,-0.1327886904666875,-0.1323088707840518,-0.1318289890857892,-0.1313490455888574,-0.1308690405103290,-0.1303889740673909,-0.1299088464773442,-0.1294286579576036,-0.1289484087256973,-0.1284680989992662,-0.1279877289960641,-0.1275072989339567,-0.1270268090309216,-0.1265462595050477,-0.1260656505745351,-0.1255849824576945,-0.1251042553729467,-0.1246234695388225,-0.1241426251739621,-0.1236617224971149,-0.1231807617271389,-0.1226997430830005,-0.1222186667837739,-0.1217375330486410,-0.1212563420968908,-0.1207750941479189,-0.1202937894212274,-0.1198124281364245,-0.1193310105132237,-0.1188495367714440,-0.1183680071310089,-0.1178864218119466,-0.1174047810343890,-0.1169230850185720,-0.1164413339848344,-0.1159595281536181,-0.1154776677454672,-0.1149957529810280,-0.1145137840810483,-0.1140317612663775,-0.1135496847579654,-0.1130675547768627,-0.1125853715442198,-0.1121031352812870,-0.1116208462094138,-0.1111385045500485,-0.1106561105247381,-0.1101736643551274,-0.1096911662629590,-0.1092086164700730,-0.1087260151984058,-0.1082433626699910,-0.1077606591069578,-0.1072779047315311,-0.1067950997660314,-0.1063122444328736,-0.1058293389545675,-0.1053463835537167,-0.1048633784530185,-0.1043803238752637,-0.1038972200433356,-0.1034140671802102,-0.1029308655089555,-0.1024476152527311,-0.1019643166347878,-0.1014809698784674,-0.1009975752072021,-0.1005141328445138,-0.1000306430140145,-0.0995471059394051,-0.0990635218444754,-0.0985798909531035,-0.0980962134892556,-0.0976124896769855,-0.0971287197404340,-0.0966449039038288,-0.0961610423914840,-0.0956771354277994,-0.0951931832372607,-0.0947091860444383,-0.0942251440739876,-0.0937410575506483,-0.0932569266992438,-0.0927727517446811,-0.0922885329119503,-0.0918042704261240,-0.0913199645123572,-0.0908356153958867,-0.0903512233020305,-0.0898667884561879,-0.0893823110838387,-0.0888977914105426,-0.0884132296619396,-0.0879286260637484,-0.0874439808417671,-0.0869592942218721,-0.0864745664300180,-0.0859897976922369,-0.0855049882346384,-0.0850201382834087,-0.0845352480648107,-0.0840503178051830,-0.0835653477309400,-0.0830803380685712,-0.0825952890446410,-0.0821102008857878,-0.0816250738187242,-0.0811399080702363,-0.0806547038671831,-0.0801694614364963,-0.0796841810051801,-0.0791988628003100,-0.0787135070490335,-0.0782281139785686,-0.0777426838162040,-0.0772572167892988,-0.0767717131252813,-0.0762861730516496,-0.0758005967959702,-0.0753149845858784,-0.0748293366490773,-0.0743436532133378,-0.0738579345064978,-0.0733721807564620,-0.0728863921912015,-0.0724005690387531,-0.0719147115272193,-0.0714288198847675,-0.0709428943396298,-0.0704569351201026,-0.0699709424545458,-0.0694849165713829,-0.0689988576991002,-0.0685127660662466,-0.0680266419014329,-0.0675404854333317,-0.0670542968906767,-0.0665680765022626,-0.0660818244969441,-0.0655955411036361,-0.0651092265513131,-0.0646228810690084,-0.0641365048858140,-0.0636500982308803,-0.0631636613334153,-0.0626771944226846,-0.0621906977280104,-0.0617041714787715,-0.0612176159044031,-0.0607310312343956,-0.0602444176982949,-0.0597577755257014,-0.0592711049462702,-0.0587844061897101,-0.0582976794857833,-0.0578109250643054,-0.0573241431551442,-0.0568373339882200,-0.0563504977935047,-0.0558636348010217,-0.0553767452408450,-0.0548898293430994,-0.0544028873379595,-0.0539159194556495,-0.0534289259264429,-0.0529419069806618,-0.0524548628486767,-0.0519677937609059,-0.0514806999478150,-0.0509935816399169,-0.0505064390677708,-0.0500192724619821,-0.0495320820532019,-0.0490448680721265,-0.0485576307494971,-0.0480703703160992,-0.0475830870027624,-0.0470957810403596,-0.0466084526598067,-0.0461211020920627,-0.0456337295681285,-0.0451463353190466,-0.0446589195759010,-0.0441714825698168,-0.0436840245319591,-0.0431965456935334,-0.0427090462857846,-0.0422215265399969,-0.0417339866874929,-0.0412464269596338,-0.0407588475878184,-0.0402712488034830,-0.0397836308381009,-0.0392959939231817,-0.0388083382902714,-0.0383206641709513,-0.0378329717968381,-0.0373452613995832,-0.0368575332108723,-0.0363697874624251,-0.0358820243859946,-0.0353942442133669,-0.0349064471763605,-0.0344186335068262,-0.0339308034366464,-0.0334429571977348,-0.0329550950220358,-0.0324672171415242,-0.0319793237882049,-0.0314914151941119,-0.0310034915913086,-0.0305155532118868,-0.0300276002879665,-0.0295396330516955,-0.0290516517352487,-0.0285636565708280,-0.0280756477906617,-0.0275876256270039,-0.0270995903121342,-0.0266115420783575,-0.0261234811580032,-0.0256354077834247,-0.0251473221869995,-0.0246592246011281,-0.0241711152582340,-0.0236829943907630,-0.0231948622311831,-0.0227067190119836,-0.0222185649656750,-0.0217304003247884,-0.0212422253218750,-0.0207540401895061,-0.0202658451602720,-0.0197776404667820,-0.0192894263416637,-0.0188012030175629,-0.0183129707271428,-0.0178247297030837,-0.0173364801780826,-0.0168482223848527,-0.0163599565561231,-0.0158716829246379,-0.0153834017231564,-0.0148951131844522,-0.0144068175413130,-0.0139185150265400,-0.0134302058729473,-0.0129418903133621,-0.0124535685806233,-0.0119652409075821,-0.0114769075271006,-0.0109885686720521,-0.0105002245753201,-0.0100118754697984,-0.0095235215883899,-0.0090351631640072,-0.0085468004295710,-0.0080584336180106,-0.0075700629622630,-0.0070816886952724,-0.0065933110499900,-0.0061049302593734,-0.0056165465563863,-0.0051281601739979,-0.0046397713451825,-0.0041513803029190,-0.0036629872801907,-0.0031745925099844,-0.0026861962252906,-0.0021977986591024,-0.0017094000444155,-0.0012210006142275,-0.0007326006015376,-0.0002442002393461,0.0002442002393461,0.0007326006015376,0.0012210006142275,0.0017094000444155,0.0021977986591024,0.0026861962252906,0.0031745925099844,0.0036629872801907,0.0041513803029190,0.0046397713451825,0.0051281601739979,0.0056165465563863,0.0061049302593734,0.0065933110499900,0.0070816886952724,0.0075700629622630,0.0080584336180106,0.0085468004295710,0.0090351631640072,0.0095235215883899,0.0100118754697984,0.0105002245753201,0.0109885686720521,0.0114769075271006,0.0119652409075821,0.0124535685806233,0.0129418903133621,0.0134302058729473,0.0139185150265400,0.0144068175413130,0.0148951131844522,0.0153834017231564,0.0158716829246378,0.0163599565561230,0.0168482223848526,0.0173364801780825,0.0178247297030836,0.0183129707271427,0.0188012030175628,0.0192894263416636,0.0197776404667819,0.0202658451602719,0.0207540401895060,0.0212422253218749,0.0217304003247882,0.0222185649656749,0.0227067190119835,0.0231948622311830,0.0236829943907629,0.0241711152582339,0.0246592246011280,0.0251473221869994,0.0256354077834246,0.0261234811580031,0.0266115420783574,0.0270995903121341,0.0275876256270038,0.0280756477906616,0.0285636565708279,0.0290516517352486,0.0295396330516954,0.0300276002879664,0.0305155532118867,0.0310034915913085,0.0314914151941120,0.0319793237882050,0.0324672171415244,0.0329550950220359,0.0334429571977349,0.0339308034366465,0.0344186335068263,0.0349064471763606,0.0353942442133670,0.0358820243859947,0.0363697874624252,0.0368575332108724,0.0373452613995833,0.0378329717968382,0.0383206641709514,0.0388083382902715,0.0392959939231818,0.0397836308381010,0.0402712488034831,0.0407588475878185,0.0412464269596339,0.0417339866874930,0.0422215265399970,0.0427090462857848,0.0431965456935335,0.0436840245319592,0.0441714825698169,0.0446589195759011,0.0451463353190467,0.0456337295681286,0.0461211020920628,0.0466084526598068,0.0470957810403596,0.0475830870027624,0.0480703703160992,0.0485576307494971,0.0490448680721265,0.0495320820532019,0.0500192724619821,0.0505064390677708,0.0509935816399169,0.0514806999478150,0.0519677937609059,0.0524548628486767,0.0529419069806618,0.0534289259264429,0.0539159194556495,0.0544028873379595,0.0548898293430994,0.0553767452408450,0.0558636348010217,0.0563504977935047,0.0568373339882200,0.0573241431551442,0.0578109250643054,0.0582976794857833,0.0587844061897101,0.0592711049462702,0.0597577755257014,0.0602444176982949,0.0607310312343956,0.0612176159044031,0.0617041714787715,0.0621906977280104,0.0626771944226846,0.0631636613334153,0.0636500982308803,0.0641365048858140,0.0646228810690084,0.0651092265513131,0.0655955411036361,0.0660818244969441,0.0665680765022626,0.0670542968906767,0.0675404854333317,0.0680266419014329,0.0685127660662466,0.0689988576991002,0.0694849165713829,0.0699709424545458,0.0704569351201026,0.0709428943396298,0.0714288198847675,0.0719147115272193,0.0724005690387531,0.0728863921912015,0.0733721807564620,0.0738579345064978,0.0743436532133378,0.0748293366490773,0.0753149845858784,0.0758005967959702,0.0762861730516496,0.0767717131252813,0.0772572167892988,0.0777426838162040,0.0782281139785685,0.0787135070490334,0.0791988628003099,0.0796841810051799,0.0801694614364962,0.0806547038671830,0.0811399080702362,0.0816250738187241,0.0821102008857877,0.0825952890446409,0.0830803380685711,0.0835653477309399,0.0840503178051829,0.0845352480648106,0.0850201382834086,0.0855049882346383,0.0859897976922368,0.0864745664300178,0.0869592942218720,0.0874439808417670,0.0879286260637483,0.0884132296619394,0.0888977914105425,0.0893823110838386,0.0898667884561878,0.0903512233020304,0.0908356153958866,0.0913199645123571,0.0918042704261239,0.0922885329119502,0.0927727517446810,0.0932569266992437,0.0937410575506484,0.0942251440739877,0.0947091860444384,0.0951931832372608,0.0956771354277995,0.0961610423914841,0.0966449039038289,0.0971287197404341,0.0976124896769856,0.0980962134892557,0.0985798909531036,0.0990635218444755,0.0995471059394052,0.1000306430140146,0.1005141328445140,0.1009975752072022,0.1014809698784676,0.1019643166347879,0.1024476152527312,0.1029308655089556,0.1034140671802103,0.1038972200433357,0.1043803238752638,0.1048633784530186,0.1053463835537168,0.1058293389545676,0.1063122444328737,0.1067950997660315,0.1072779047315312,0.1077606591069579,0.1082433626699911,0.1087260151984059,0.1092086164700730,0.1096911662629590,0.1101736643551274,0.1106561105247381,0.1111385045500485,0.1116208462094138,0.1121031352812870,0.1125853715442198,0.1130675547768627,0.1135496847579654,0.1140317612663775,0.1145137840810483,0.1149957529810280,0.1154776677454672,0.1159595281536181,0.1164413339848344,0.1169230850185720,0.1174047810343890,0.1178864218119466,0.1183680071310089,0.1188495367714440,0.1193310105132237,0.1198124281364245,0.1202937894212274,0.1207750941479189,0.1212563420968908,0.1217375330486410,0.1222186667837739,0.1226997430830005,0.1231807617271389,0.1236617224971149,0.1241426251739621,0.1246234695388225,0.1251042553729467,0.1255849824576945,0.1260656505745351,0.1265462595050477,0.1270268090309216,0.1275072989339567,0.1279877289960641,0.1284680989992662,0.1289484087256973,0.1294286579576036,0.1299088464773442,0.1303889740673909,0.1308690405103290,0.1313490455888574,0.1318289890857892,0.1323088707840518,0.1327886904666875,0.1332684479168541,0.1337481429178246,0.1342277752529883,0.1347073447058508,0.1351868510600343,0.1356662940992785,0.1361456736074401,0.1366249893684941,0.1371042411665337,0.1375834287857705,0.1380625520105354,0.1385416106252786,0.1390206044145699,0.1394995331630996,0.1399783966556781,0.1404571946772370,0.1409359270128291,0.1414145934476288,0.1418931937669325,0.1423717277561589,0.1428501952008497,0.1433285958866695,0.1438069295994065,0.1442851961249727,0.1447633952494043,0.1452415267588623,0.1457195904396325,0.1461975860781261,0.1466755134608800,0.1471533723745572,0.1476311626059470,0.1481088839419657,0.1485865361696566,0.1490641190761908,0.1495416324488670,0.1500190760751124,0.1504964497424826,0.1509737532386624,0.1514509863514659,0.1519281488688369,0.1524052405788493,0.1528822612697074,0.1533592107297463,0.1538360887474323,0.1543128951113632,0.1547896296102688,0.1552662920330112,0.1557428821685844,0.1562193998061162,0.1566958447348674,0.1571722167442325,0.1576485156237399,0.1581247411630526,0.1586008931519681,0.1590769713804191,0.1595529756384739,0.1600289057163363,0.1605047614043465,0.1609805424929810,0.1614562487728533,0.1619318800347141,0.1624074360694517,0.1628829166680920,0.1633583216217996,0.1638336507218773,0.1643089037597673,0.1647840805270506,0.1652591808154483,0.1657342044168212,0.1662091511231706,0.1666840207266385,0.1671588130195077,0.1676335277942027,0.1681081648432895,0.1685827239594764,0.1690572049356138,0.1695316075646952,0.1700059316398568,0.1704801769543787,0.1709543433016844,0.1714284304753418,0.1719024382690629,0.1723763664767050,0.1728502148922701,0.1733239833099059,0.1737976715239059,0.1742712793287097,0.1747448065189034,0.1752182528892199,0.1756916182345394,0.1761649023498895,0.1766381050304456,0.1771112260715313,0.1775842652686186,0.1780572224173286,0.1785300973134314,0.1790028897528465,0.1794755995316433,0.1799482264460414,0.1804207702924109,0.1808932308672728,0.1813656079672989,0.1818379013893129,0.1823101109302900,0.1827822363873576,0.1832542775577958,0.1837262342390370,0.1841981062286672,0.1846698933244255,0.1851415953242050,0.1856132120260526,0.1860847432281698,0.1865561887289128,0.1870275483267929,0.1874988218204766,0.1879700090087862,0.1884411096907000,0.1889121236653527,0.1893830507320354,0.1898538906901965,0.1903246433394416,0.1907953084795336,0.1912658859103937,0.1917363754321011,0.1922067768448937,0.1926770899491682,0.1931473145454804,0.1936174504345457,0.1940874974172393,0.1945574552945964,0.1950273238678129,0.1954971029382451,0.1959667923074105,0.1964363917769882,0.1969059011488185,0.1973753202249040,0.1978446488074095,0.1983138866986625,0.1987830337011532,0.1992520896175352,0.1997210542506255,0.2001899274034050,0.2006587088790187,0.2011273984807759,0.2015959960121508,0.2020645012767826,0.2025329140784759,0.2030012342212007,0.2034694615090932,0.2039375957464557,0.2044056367377570,0.2048735842876329,0.2053414382008864,0.2058091982824875,0.2062768643375743,0.2067444361714530,0.2072119135895978,0.2076792963976518,0.2081465844014268,0.2086137774069040,0.2090808752202340,0.2095478776477372,0.2100147844959041,0.2104815955713956,0.2109483106810432,0.2114149296318494,0.2118814522309880,0.2123478782858043,0.2128142076038155,0.2132804399927106,0.2137465752603515,0.2142126132147723,0.2146785536641805,0.2151443964169565,0.2156101412816548,0.2160757880670028,0.2165413365819027,0.2170067866354311,0.2174721380368389,0.2179373905955524,0.2184025441211727,0.2188675984234769,0.2193325533124174,0.2197974085981230,0.2202621640908987,0.2207268196012263,0.2211913749397644,0.2216558299173486,0.2221201843449924,0.2225844380338867,0.2230485907954004,0.2235126424410810,0.2239765927826541,0.2244404416320245,0.2249041888012760,0.2253678341026717,0.2258313773486544,0.2262948183518468,0.2267581569250519,0.2272213928812528,0.2276845260336137,0.2281475561954796,0.2286104831803769,0.2290733068020133,0.2295360268742784,0.2299986432112439,0.2304611556271637,0.2309235639364743,0.2313858679537951,0.2318480674939286,0.2323101623718606,0.2327721524027605,0.2332340374019817,0.2336958171850617,0.2341574915677222,0.2346190603658698,0.2350805233955960,0.2355418804731773,0.2360031314150758,0.2364642760379392,0.2369253141586012,0.2373862455940815,0.2378470701615865,0.2383077876785091,0.2387683979624293,0.2392289008311142,0.2396892961025184,0.2401495835947840,0.2406097631262415,0.2410698345154091,0.2415297975809937,0.2419896521418909,0.2424493980171852,0.2429090350261503,0.2433685629882493,0.2438279817231349,0.2442872910506498,0.2447464907908270,0.2452055807638897,0.2456645607902517,0.2461234306905180,0.2465821902854843,0.2470408393961381,0.2474993778436581,0.2479578054494153,0.2484161220349722,0.2488743274220843,0.2493324214326990,0.2497904038889569,0.2502482746131916,0.2507060334279297,0.2511636801558916,0.2516212146199912,0.2520786366433366,0.2525359460492299,0.2529931426611676,0.2534502263028409,0.2539071967981360,0.2543640539711340,0.2548207976461117,0.2552774276475409,0.2557339438000897,0.2561903459286219,0.2566466338581979,0.2571028074140741,0.2575588664217040,0.2580148107067377,0.2584706400950227,0.2589263544126037,0.2593819534857231,0.2598374371408209,0.2602928052045355,0.2607480575037030,0.2612031938653585,0.2616582141167354,0.2621131180852662,0.2625679055985824,0.2630225764845149,0.2634771305710941,0.2639315676865502,0.2643858876593134,0.2648400903180138,0.2652941754914824,0.2657481430087503,0.2662019926990498,0.2666557243918140,0.2671093379166774,0.2675628331034756,0.2680162097822464,0.2684694677832291,0.2689226069368651,0.2693756270737981,0.2698285280248743,0.2702813096211426,0.2707339716938548,0.2711865140744657,0.2716389365946335,0.2720912390862198,0.2725434213812900,0.2729954833121133,0.2734474247111630,0.2738992454111169,0.2743509452448572,0.2748025240454703,0.2752539816462482,0.2757053178806875,0.2761565325824906,0.2766076255855647,0.2770585967240233,0.2775094458321853,0.2779601727445760,0.2784107772959266,0.2788612593211750,0.2793116186554657,0.2797618551341499,0.2802119685927858,0.2806619588671391,0.2811118257931824,0.2815615692070963,0.2820111889452689,0.2824606848442964,0.2829100567409830,0.2833593044723413,0.2838084278755922,0.2842574267881657,0.2847063010477003,0.2851550504920435,0.2856036749592524,0.2860521742875931,0.2865005483155415,0.2869487968817833,0.2873969198252139,0.2878449169849390,0.2882927882002745,0.2887405333107471,0.2891881521560935,0.2896356445762618,0.2900830104114108,0.2905302495019106,0.2909773616883425,0.2914243468114995,0.2918712047123859,0.2923179352322184,0.2927645382124252,0.2932110134946471,0.2936573609207369,0.2941035803327601,0.2945496715729949,0.2949956344839324,0.2954414689082765,0.2958871746889445,0.2963327516690670,0.2967781996919881,0.2972235186012655,0.2976687082406707,0.2981137684541895,0.2985586990860214,0.2990034999805806,0.2994481709824956,0.2998927119366095,0.3003371226879803,0.3007814030818809,0.3012255529637992,0.3016695721794385,0.3021134605747174,0.3025572179957703,0.3030008442889469,0.3034443393008131,0.3038877028781508,0.3043309348679579,0.3047740351174487,0.3052170034740540,0.3056598397854213,0.3061025438994147,0.3065451156641153,0.3069875549278213,0.3074298615390480,0.3078720353465282,0.3083140761992121,0.3087559839462675,0.3091977584370801,0.3096393995212535,0.3100809070486093,0.3105222808691873,0.3109635208332459,0.3114046267912615,0.3118455985939297,0.3122864360921644,0.3127271391370988,0.3131677075800847,0.3136081412726935,0.3140484400667157,0.3144886038141612,0.3149286323672596,0.3153685255784602,0.3158082833004321,0.3162479053860643,0.3166873916884663,0.3171267420609673,0.3175659563571171,0.3180050344306862,0.3184439761356654,0.3188827813262665,0.3193214498569220,0.3197599815822855,0.3201983763572319,0.3206366340368569,0.3210747544764781,0.3215127375316343,0.3219505830580859,0.3223882909118153,0.3228258609490266,0.3232632930261458,0.3237005869998212,0.3241377427269233,0.3245747600645448,0.3250116388700010,0.3254483790008299,0.3258849803147917,0.3263214426698702,0.3267577659242714,0.3271939499364248,0.3276299945649828,0.3280658996688212,0.3285016651070392,0.3289372907389593,0.3293727764241279,0.3298081220223150,0.3302433273935142,0.3306783923979433,0.3311133168960444,0.3315481007484828,0.3319827438161490,0.3324172459601575,0.3328516070418472,0.3332858269227817,0.3337199054647492,0.3341538425297628,0.3345876379800603,0.3350212916781045,0.3354548034865833,0.3358881732684100,0.3363214008867229,0.3367544862048856,0.3371874290864876,0.3376202293953436,0.3380528869954941,0.3384854017512053,0.3389177735269693,0.3393500021875043,0.3397820875977543,0.3402140296228896,0.3406458281283066,0.3410774829796282,0.3415089940427036,0.3419403611836086,0.3423715842686455,0.3428026631643433,0.3432335977374577,0.3436643878549716,0.3440950333840944,0.3445255341922628,0.3449558901471406,0.3453861011166187,0.3458161669688154,0.3462460875720763,0.3466758627949745,0.3471054925063107,0.3475349765751130,0.3479643148706373,0.3483935072623675,0.3488225536200150,0.3492514538135194,0.3496802077130481,0.3501088151889970,0.3505372761119895,0.3509655903528779,0.3513937577827425,0.3518217782728920,0.3522496516948636,0.3526773779204231,0.3531049568215647,0.3535323882705116,0.3539596721397156,0.3543868083018573,0.3548137966298461,0.3552406369968206,0.3556673292761482,0.3560938733414257,0.3565202690664787,0.3569465163253623,0.3573726149923608,0.3577985649419879,0.3582243660489867,0.3586500181883297,0.3590755212352191,0.3595008750650866,0.3599260795535938,0.3603511345766317,0.3607760400103213,0.3612007957310136,0.3616254016152890,0.3620498575399586,0.3624741633820628,0.3628983190188728,0.3633223243278893,0.3637461791868435,0.3641698834736969,0.3645934370666413,0.3650168398440987,0.3654400916847216,0.3658631924673930,0.3662861420712265,0.3667089403755660,0.3671315872599862,0.3675540826042926,0.3679764262885211,0.3683986181929386,0.3688206581980427,0.3692425461845619,0.3696642820334556,0.3700858656259141,0.3705072968433588,0.3709285755674420,0.3713497016800471,0.3717706750632888,0.3721914955995127,0.3726121631712958,0.3730326776614463,0.3734530389530037,0.3738732469292386,0.3742933014736534,0.3747132024699815,0.3751329498021879,0.3755525433544692,0.3759719830112532,0.3763912686571995,0.3768104001771992,0.3772293774563750,0.3776482003800813,0.3780668688339042,0.3784853827036613,0.3789037418754022,0.3793219462354082,0.3797399956701922,0.3801578900664994,0.3805756293113063,0.3809932132918218,0.3814106418954863,0.3818279150099724,0.3822450325231846,0.3826619943232594,0.3830788002985652,0.3834954503377027,0.3839119443295044,0.3843282821630351,0.3847444637275916,0.3851604889127028,0.3855763576081301,0.3859920697038664,0.3864076250901372,0.3868230236574005,0.3872382652963459,0.3876533498978957,0.3880682773532045,0.3884830475536589,0.3888976603908778,0.3893121157567127,0.3897264135432473,0.3901405536427974,0.3905545359479116,0.3909683603513704,0.3913820267461871,0.3917955350256070,0.3922088850831081,0.3926220768124008,0.3930351101074277,0.3934479848623639,0.3938607009716172,0.3942732583298274,0.3946856568318671,0.3950978963728413,0.3955099768480872,0.3959218981531748,0.3963336601839065,0.3967452628363170,0.3971567060066736,0.3975679895914761,0.3979791134874567,0.3983900775915802,0.3988008818010438,0.3992115260132772,0.3996220101259425,0.4000323340369344,0.4004424976443801,0.4008525008466392,0.4012623435423036,0.4016720256301981,0.4020815470093796,0.4024909075791375,0.4029001072389939,0.4033091458887030,0.4037180234282517,0.4041267397578592,0.4045352947779772,0.4049436883892897,0.4053519204927133,0.4057599909893968,0.4061678997807214,0.4065756467683008,0.4069832318539809,0.4073906549398403,0.4077979159281894,0.4082050147215712,0.4086119512227611,0.4090187253347667,0.4094253369608278,0.4098317860044166,0.4102380723692373,0.4106441959592265,0.4110501566785532,0.4114559544316183,0.4118615891230548,0.4122670606577281,0.4126723689407356,0.4130775138774068,0.4134824953733034,0.4138873133342191,0.4142919676661795,0.4146964582754423,0.4151007850684974,0.4155049479520664,0.4159089468331029,0.4163127816187924,0.4167164522165525,0.4171199585340324,0.4175233004791133,0.4179264779599080,0.4183294908847614,0.4187323391622497,0.4191350227011814,0.4195375414105962,0.4199398951997656,0.4203420839781928,0.4207441076556126,0.4211459661419912,0.4215476593475265,0.4219491871826477,0.4223505495580157,0.4227517463845227,0.4231527775732921,0.4235536430356791,0.4239543426832697,0.4243548764278814,0.4247552441815631,0.4251554458565947,0.4255554813654873,0.4259553506209830,0.4263550535360552,0.4267545900239081,0.4271539599979773,0.4275531633719287,0.4279522000596598,0.4283510699752984,0.4287497730332034,0.4291483091479646,0.4295466782344022,0.4299448802075674,0.4303429149827417,0.4307407824754375,0.4311384826013976,0.4315360152765954,0.4319333804172346,0.4323305779397493,0.4327276077608041,0.4331244697972939,0.4335211639663436,0.4339176901853085,0.4343140483717741,0.4347102384435559,0.4351062603186993,0.4355021139154799,0.4358977991524032,0.4362933159482044,0.4366886642218488,0.4370838438925313,0.4374788548796766,0.4378736971029387,0.4382683704822015,0.4386628749375787,0.4390572103894129,0.4394513767582766,0.4398453739649714,0.4402392019305282,0.4406328605762072,0.4410263498234979,0.4414196695941187,0.4418128198100172,0.4422058003933698,0.4425986112665821,0.4429912523522884,0.4433837235733518,0.4437760248528641,0.4441681561141458,0.4445601172807462,0.4449519082764427,0.4453435290252416,0.4457349794513774,0.4461262594793128,0.4465173690337390,0.4469083080395754,0.4472990764219693,0.4476896741062961,0.4480801010181594,0.4484703570833905,0.4488604422280486,0.4492503563784207,0.4496400994610213,0.4500296714025929,0.4504190721301051,0.4508083015707554,0.4511973596519682,0.4515862463013957,0.4519749614469171,0.4523635050166388,0.4527518769388943,0.4531400771422440,0.4535281055554754,0.4539159621076028,0.4543036467278672,0.4546911593457365,0.4550784998909048,0.4554656682932932,0.4558526644830489,0.4562394883905457,0.4566261399463837,0.4570126190813889,0.4573989257266138,0.4577850598133366,0.4581710212730617,0.4585568100375193,0.4589424260386653,0.4593278692086815,0.4597131394799749,0.4600982367851785,0.4604831610571505,0.4608679122289744,0.4612524902339590,0.4616368950056385,0.4620211264777717,0.4624051845843429,0.4627890692595609,0.4631727804378596,0.4635563180538974,0.4639396820425575,0.4643228723389475,0.4647058888783995,0.4650887315964700,0.4654714004289396,0.4658538953118132,0.4662362161813197,0.4666183629739120,0.4670003356262669,0.4673821340752849,0.4677637582580901,0.4681452081120305,0.4685264835746772,0.4689075845838250,0.4692885110774919,0.4696692629939189,0.4700498402715703,0.4704302428491333,0.4708104706655180,0.4711905236598574,0.4715704017715069,0.4719501049400450,0.4723296331052720,0.4727089862072112,0.4730881641861078,0.4734671669824292,0.4738459945368651,0.4742246467903269,0.4746031236839481,0.4749814251590837,0.4753595511573104,0.4757375016204267,0.4761152764904523,0.4764928757096281,0.4768702992204166,0.4772475469655010,0.4776246188877858,0.4780015149303962,0.4783782350366784,0.4787547791501989,0.4791311472147452,0.4795073391743248,0.4798833549731660,0.4802591945557170,0.4806348578666462,0.4810103448508420,0.4813856554534127,0.4817607896196863,0.4821357472952106,0.4825105284257528,0.4828851329572997,0.4832595608360571,0.4836338120084504,0.4840078864211237,0.4843817840209403,0.4847555047549823,0.4851290485705505,0.4855024154151643,0.4858756052365616,0.4862486179826987,0.4866214536017502,0.4869941120421086,0.4873665932523846,0.4877388971814067,0.4881110237782214,0.4884829729920926,0.4888547447725017,0.4892263390691478,0.4895977558319469,0.4899689950110324,0.4903400565567546,0.4907109404196809,0.4910816465505954,0.4914521749004986,0.4918225254206080,0.4921926980623571,0.4925626927773958,0.4929325095175903,0.4933021482350227,0.4936716088819910,0.4940408914110090,0.4944099957748062,0.4947789219263274,0.4951476698187330,0.4955162394053986,0.4958846306399150,0.4962528434760878,0.4966208778679375,0.4969887337696996,0.4973564111358239,0.4977239099209747,0.4980912300800308,0.4984583715680852,0.4988253343404447,0.4991921183526304,0.4995587235603769,0.4999251499196326,0.5002913973865596,0.5006574659175330,0.5010233554691415,0.5013890659981868,0.5017545974616837,0.5021199498168596,0.5024851230211549,0.5028501170322225,0.5032149318079275,0.5035795673063477,0.5039440234857728,0.5043083003047046,0.5046723977218568,0.5050363156961547,0.5054000541867355,0.5057636131529477,0.5061269925543510,0.5064901923507165,0.5068532125020261,0.5072160529684729,0.5075787137104606,0.5079411946886034,0.5083034958637261,0.5086656171968640,0.5090275586492622,0.5093893201823762,0.5097509017578710,0.5101123033376217,0.5104735248837129,0.5108345663584387,0.5111954277243023,0.5115561089440163,0.5119166099805023,0.5122769307968904,0.5126370713565202,0.5129970316229391,0.5133568115599032,0.5137164111313771,0.5140758303015331,0.5144350690347520,0.5147941272956220,0.5151530050489391,0.5155117022597069,0.5158702188931363,0.5162285549146455,0.5165867102898598,0.5169446849846112,0.5173024789649389,0.5176600921970882,0.5180175246475113,0.5183747762828663,0.5187318470700181,0.5190887369760369,0.5194454459681991,0.5198019740139868,0.5201583210810875,0.5205144871373945,0.5208704721510056,0.5212262760902243,0.5215818989235590,0.5219373406197223,0.5222926011476320,0.5226476804764102,0.5230025785753830,0.5233572954140811,0.5237118309622387,0.5240661851897942,0.5244203580668896,0.5247743495638700,0.5251281596512845,0.5254817882998847,0.5258352354806257,0.5261885011646653,0.5265415853233642,0.5268944879282850,0.5272472089511934,0.5275997483640570,0.5279521061390454,0.5283042822485303,0.5286562766650850,0.5290080893614840,0.5293597203107041,0.5297111694859223,0.5300624368605176,0.5304135224080692,0.5307644261023573,0.5311151479173628,0.5314656878272669,0.5318160458064510,0.5321662218294968,0.5325162158711856,0.5328660279064988,0.5332156579106169,0.5335651058589203,0.5339143717269883,0.5342634554905996,0.5346123571257316,0.5349610766085605,0.5353096139154611,0.5356579690230066,0.5360061419079683,0.5363541325473158,0.5367019409182164,0.5370495669980353,0.5373970107643352,0.5377442721948762,0.5380913512676155,0.5384382479607075,0.5387849622525034,0.5391314941215510,0.5394778435465950,0.5398240105065760,0.5401699949806312,0.5405157969480935,0.5408614163884917,0.5412068532815505,0.5415521076071898,0.5418971793455250,0.5422420684768665,0.5425867749817199,0.5429312988407856,0.5432756400349582,0.5436197985453272,0.5439637743531762,0.5443075674399830,0.5446511777874190,0.5449946053773499,0.5453378501918343,0.5456809122131250,0.5460237914236671,0.5463664878060994,0.5467090013432535,0.5470513320181533,0.5473934798140155,0.5477354447142492,0.5480772267024554,0.5484188257624273,0.5487602418781497,0.5491014750337989,0.5494425252137432,0.5497833924025415,0.5501240765849441,0.5504645777458922,0.5508048958705175,0.5511450309441425,0.5514849829522799,0.5518247518806325,0.5521643377150933,0.5525037404417449,0.5528429600468595,0.5531819965168989,0.5535208498385141,0.5538595199985453,0.5541980069840212,0.5545363107821596,0.5548744313803666,0.5552123687662368,0.5555501229275529,0.5558876938522854,0.5562250815285929,0.5565622859448213,0.5568993070895041,0.5572361449513620,0.5575727995193027,0.5579092707824209,0.5582455587299978,0.5585816633515012,0.5589175846365850,0.5592533225750896,0.5595888771570411,0.5599242483726512,0.5602594362123173,0.5605944406666223,0.5609292617263343,0.5612638993824060,0.5615983536259751,0.5619326244483642,0.5622667118410801,0.5626006157958137,0.5629343363044402,0.5632678733590185,0.5636012269517914,0.5639343970751848,0.5642673837218081,0.5646001868844540,0.5649328065560978,0.5652652427298978,0.5655974953991945,0.5659295645575109,0.5662614501985523,0.5665931523162059,0.5669246709045406,0.5672560059578067,0.5675871574704361,0.5679181254370420,0.5682489098524185,0.5685795107115402,0.5689099280095629,0.5692401617418224,0.5695702119038347,0.5699000784912962,0.5702297615000828,0.5705592609262502,0.5708885767660336,0.5712177090158473,0.5715466576722849,0.5718754227321186,0.5722040041922994,0.5725324020499570,0.5728606163023992,0.5731886469471117,0.5735164939817584,0.5738441574041809,0.5741716372123981,0.5744989334046063,0.5748260459791792,0.5751529749346669,0.5754797202697966,0.5758062819834720,0.5761326600747730,0.5764588545429557,0.5767848653874521,0.5771106926078701,0.5774363362039929,0.5777617961757793,0.5780870725233631,0.5784121652470531,0.5787370743473329,0.5790617998248604,0.5793863416804683,0.5797106999151631,0.5800348745301254,0.5803588655267095,0.5806826729064434,0.5810062966710282,0.5813297368223386,0.5816529933624218,0.5819760662934982,0.5822989556179603,0.5826216613383733,0.5829441834574747,0.5832665219781733,0.5835886769035504,0.5839106482368587,0.5842324359815219,0.5845540401411352,0.5848754607194647,0.5851966977204474,0.5855177511481904,0.5858386210069716,0.5861593073012390,0.5864798100356103,0.5868001292148735,0.5871202648439854,0.5874402169280728,0.5877599854724314,0.5880795704825256,0.5883989719639891,0.5887181899226237,0.5890372243643997,0.5893560752954554,0.5896747427220973,0.5899932266507995,0.5903115270882035,0.5906296440411183,0.5909475775165199,0.5912653275215514,0.5915828940635223,0.5919002771499089,0.5922174767883538,0.5925344929866654,0.5928513257528183,0.5931679750949528,0.5934844410213744,0.5938007235405542,0.5941168226611282,0.5944327383918973,0.5947484707418270,0.5950640197200476,0.5953793853358532,0.5956945675987023,0.5960095665182170,0.5963243821041833,0.5966390143665502,0.5969534633154305,0.5972677289610997,0.5975818113139961,0.5978957103847207,0.5982094261840368,0.5985229587228702,0.5988363080123083,0.5991494740636005,0.5994624568881577,0.5997752564975523,0.6000878729035177,0.6004003061179485,0.6007125561528997,0.6010246230205870,0.6013365067333867,0.6016482073038348,0.6019597247446277,0.6022710590686209,0.6025822102888299,0.6028931784184294,0.6032039634707530,0.6035145654592934,0.6038249843977019,0.6041352202997884,0.6044452731795207,0.6047551430510250,0.6050648299285850,0.6053743338266425,0.6056836547597965,0.6059927927428029,0.6063017477905751,0.6066105199181830,0.6069191091408533,0.6072275154739689,0.6075357389330689,0.6078437795338484,0.6081516372921582,0.6084593122240047,0.6087668043455496,0.6090741136731099,0.6093812402231572,0.6096881840123180,0.6099949450573732,0.6103015233752580,0.6106079189830619,0.6109141318980279,0.6112201621375529,0.6115260097191871,0.6118316746606340,0.6121371569797504,0.6124424566945452,0.6127475738231806,0.6130525083839709,0.6133572603953825,0.6136618298760341,0.6139662168446958,0.6142704213202893,0.6145744433218879,0.6148782828687158,0.6151819399801481,0.6154854146757108,0.6157887069750803,0.6160918168980829,0.6163947444646959,0.6166974896950452,0.6170000526094076,0.6173024332282083,0.6176046315720226,0.6179066476615741,0.6182084815177358,0.6185101331615286,0.6188116026141225,0.6191128898968352,0.6194139950311325,0.6197149180386280,0.6200156589410827,0.6203162177604050,0.6206165945186504,0.6209167892380213,0.6212168019408667,0.6215166326496823,0.6218162813871098,0.6221157481759368,0.6224150330390975,0.6227141359996707,0.6230130570808815,0.6233117963060996,0.6236103536988398,0.6239087292827620,0.6242069230816703,0.6245049351195131,0.6248027654203834,0.6251004140085176,0.6253978809082961,0.6256951661442427,0.6259922697410246,0.6262891917234519,0.6265859321164775,0.6268824909451973,0.6271788682348494,0.6274750640108139,0.6277710782986132,0.6280669111239117,0.6283625625125147,0.6286580324903695,0.6289533210835643,0.6292484283183282,0.6295433542210310,0.6298380988181833,0.6301326621364357,0.6304270442025790,0.6307212450435439,0.6310152646864006,0.6313091031583589,0.6316027604867678,0.6318962366991153,0.6321895318230284,0.6324826458862723,0.6327755789167510,0.6330683309425063,0.6333609019917180,0.6336532920927042,0.6339455012739197,0.6342375295639571,0.6345293769915460,0.6348210435855529,0.6351125293749810,0.6354038343889698,0.6356949586567953,0.6359859022078692,0.6362766650717395,0.6365672472780892,0.6368576488567374,0.6371478698376375,0.6374379102508786,0.6377277701266844,0.6380174494954128,0.6383069483875565,0.6385962668337418,0.6388854048647294,0.6391743625114134,0.6394631398048213,0.6397517367761141,0.6400401534565856,0.6403283898776626,0.6406164460709044,0.6409043220680029,0.6411920179007818,0.6414795336011973,0.6417668692013369,0.6420540247334198,0.6423410002297966,0.6426277957229489,0.6429144112454894,0.6432008468301614,0.6434871025098385,0.6437731783175249,0.6440590742863545,0.6443447904495911,0.6446303268406285,0.6449156834929896,0.6452008604403262,0.6454858577164199,0.6457706753551803,0.6460553133906458,0.6463397718569834,0.6466240507884880,0.6469081502195823,0.6471920701848167,0.6474758107188696,0.6477593718565461,0.6480427536327786,0.6483259560826263,0.6486089792412751,0.6488918231440373,0.6491744878263513,0.6494569733237816,0.6497392796720185,0.6500214069068779,0.6503033550643009,0.6505851241803540,0.6508667142912282,0.6511481254332397,0.6514293576428291,0.6517104109565608,0.6519912854111241,0.6522719810433314,0.6525524978901194,0.6528328359885476,0.6531129953757994,0.6533929760891806,0.6536727781661202,0.6539524016441696,0.6542318465610026,0.6545111129544153,0.6547902008623256,0.6550691103227733,0.6553478413739194,0.6556263940540467,0.6559047684015585,0.6561829644549797,0.6564609822529552,0.6567388218342508,0.6570164832377524,0.6572939665024659,0.6575712716675171,0.6578483987721513,0.6581253478557334,0.6584021189577474,0.6586787121177962,0.6589551273756015,0.6592313647710035,0.6595074243439610,0.6597833061345508,0.6600590101829674,0.6603345365295231,0.6606098852146480,0.6608850562788891,0.6611600497629107,0.6614348657074937,0.6617095041535360,0.6619839651420517,0.6622582487141713,0.6625323549111410,0.6628062837743232,0.6630800353451956,0.6633536096653512,0.6636270067764987,0.6639002267204610,0.6641732695391764,0.6644461352746975,0.6647188239691910,0.6649913356649382,0.6652636704043337,0.6655358282298862,0.6658078091842179,0.6660796133100640,0.6663512406502731,0.6666226912478063,0.6668939651457375,0.6671650623872530,0.6674359830156516,0.6677067270743435,0.6679772946068513,0.6682476856568088,0.6685179002679613,0.6687879384841654,0.6690578003493883,0.6693274859077084,0.6695969952033143,0.6698663282805051,0.6701354851836898,0.6704044659573876,0.6706732706462272,0.6709418992949467,0.6712103519483940,0.6714786286515254,0.6717467294494064,0.6720146543872111,0.6722824035102220,0.6725499768638299,0.6728173744935336,0.6730845964449398,0.6733516427637626,0.6736185134958236,0.6738852086870516,0.6741517283834825,0.6744180726312586,0.6746842414766292,0.6749502349659498,0.6752160531456818,0.6754816960623928,0.6757471637627561,0.6760124562935507,0.6762775737016603,0.6765425160340743,0.6768072833378870,0.6770718756602968,0.6773362930486072,0.6776005355502257,0.6778646032126640,0.6781284960835374,0.6783922142105652,0.6786557576415697,0.6789191264244769,0.6791823206073157,0.6794453402382175,0.6797081853654168,0.6799708560372501,0.6802333523021563,0.6804956742086764,0.6807578218054530,0.6810197951412302,0.6812815942648538,0.6815432192252705,0.6818046700715281,0.6820659468527751,0.6823270496182604,0.6825879784173337,0.6828487332994442,0.6831093143141416,0.6833697215110750,0.6836299549399931,0.6838900146507442,0.6841499006932751,0.6844096131176320,0.6846691519739597,0.6849285173125014,0.6851877091835987,0.6854467276376911,0.6857055727253163,0.6859642444971094,0.6862227430038028,0.6864810682962269,0.6867392204253083,0.6869971994420712,0.6872550053976356,0.6875126383432190,0.6877700983301343,0.6880273854097907,0.6882844996336934,0.6885414410534431,0.6887982097207358,0.6890548056873630,0.6893112290052110,0.6895674797262612,0.6898235579025893,0.6900794635863655,0.6903351968298543,0.6905907576854143,0.6908461462054977,0.6911013624426502,0.6913564064495114,0.6916112782788134,0.6918659779833820,0.6921205056161353,0.6923748612300842,0.6926290448783320,0.6928830566140740,0.6931368964905978,0.6933905645612825,0.6936440608795990,0.6938973854991092,0.6941505384734668,0.6944035198564159,0.6946563297017918,0.6949089680635200,0.6951614349956168,0.6954137305521881,0.6956658547874305,0.6959178077556296,0.6961695895111614,0.6964212001084904,0.6966726396021711,0.6969239080468463,0.6971750054972481,0.6974259320081968,0.6976766876346011,0.6979272724314583,0.6981776864538530,0.6984279297569582,0.6986780023960342,0.6989279044264285,0.6991776359035761,0.6994271968829989,0.6996765874203054,0.6999258075711909,0.7001748573914370,0.7004237369369115,0.7006724462635682,0.7009209854274464,0.7011693544846717,0.7014175534914542,0.7016655825040900,0.7019134415789595,0.7021611307725286,0.7024086501413472,0.7026559997420501,0.7029031796313556,0.7031501898660670,0.7033970305030706,0.7036437015993367,0.7038902032119188,0.7041365353979538,0.7043826982146618,0.7046286917193454,0.7048745159693898,0.7051201710222629,0.7053656569355148,0.7056109737667775,0.7058561215737651,0.7061011004142730,0.7063459103461784,0.7065905514274397,0.7068350237160961,0.7070793272702681,0.7073234621481566,0.7075674284080430,0.7078112261082891,0.7080548553073367,0.7082983160637077,0.7085416084360036,0.7087847324829052,0.7090276882631729,0.7092704758356462,0.7095130952592434,0.7097555465929618,0.7099978298958769,0.7102399452271427,0.7104818926459917,0.7107236722117338,0.7109652839837570,0.7112067280215270,0.7114480043845864,0.7116891131325557,0.7119300543251319,0.7121708280220889,0.7124114342832773,0.7126518731686242,0.7128921447381328,0.7131322490518826,0.7133721861700284,0.7136119561528014,0.7138515590605077,0.7140909949535290,0.7143302638923220,0.7145693659374183,0.7148083011494243,0.7150470695890207,0.7152856713169628,0.7155241063940798,0.7157623748812751,0.7160004768395257,0.7162384123298823,0.7164761814134688,0.7167137841514825,0.7169512206051936,0.7171884908359449,0.7174255949051525,0.7176625328743040,0.7178993048049602,0.7181359107587533,0.7183723507973876,0.7186086249826391,0.7188447333763554,0.7190806760404550,0.7193164530369280,0.7195520644278356,0.7197875102753087,0.7200227906415500,0.7202579055888319,0.7204928551794970,0.7207276394759582,0.7209622585406980,0.7211967124362685,0.7214310012252914,0.7216651249704577,0.7218990837345271,0.7221328775803285,0.7223665065707595,0.7225999707687862,0.7228332702374428,0.7230664050398320,0.7232993752391242,0.7235321808985576,0.7237648220814381,0.7239972988511388,0.7242296112711003,0.7244617594048302,0.7246937433159025,0.7249255630679584,0.7251572187247055,0.7253887103499175,0.7256200380074342,0.7258512017611616,0.7260822016750712,0.7263130378132002,0.7265437102396511,0.7267742190185917,0.7270045642142547,0.7272347458909377,0.7274647641130030,0.7276946189448773,0.7279243104510516,0.7281538386960809,0.7283832037445844,0.7286124056612449,0.7288414445108087,0.7290703203580855,0.7292990332679482,0.7295275833053329,0.7297559705352383,0.7299841950227258,0.7302122568329196,0.7304401560310057,0.7306678926822324,0.7308954668519101,0.7311228786054109,0.7313501280081685,0.7315772151256776,0.7318041400234949,0.7320309027672374,0.7322575034225833,0.7324839420552717,0.7327102187311015,0.7329363335159328,0.7331622864756853,0.7333880776763387,0.7336137071839328,0.7338391750645668,0.7340644813843991,0.7342896262096479,0.7345146096065900,0.7347394316415615,0.7349640923809571,0.7351885918912297,0.7354129302388911,0.7356371074905110,0.7358611237127174,0.7360849789721956,0.7363086733356893,0.7365322068699990,0.7367555796419831,0.7369787917185567,0.7372018431666922,0.7374247340534188,0.7376474644458219,0.7378700344110437,0.7380924440162827,0.7383146933287933,0.7385367824158861,0.7387587113449271,0.7389804801833383,0.7392020889985965,0.7394235378582345,0.7396448268298393,0.7398659559810535,0.7400869253795743,0.7403077350931530,0.7405283851895955,0.7407488757367622,0.7409692068025671,0.7411893784549782,0.7414093907620172,0.7416292437917593,0.7418489376123331,0.7420684722919200,0.7422878478987549,0.7425070645011254,0.7427261221673712,0.7429450209658854,0.7431637609651126,0.7433823422335500,0.7436007648397467,0.7438190288523032,0.7440371343398723,0.7442550813711577,0.7444728700149147,0.7446905003399496,0.7449079724151195,0.7451252863093326,0.7453424420915475,0.7455594398307733,0.7457762795960694,0.7459929614565453,0.7462094854813602,0.7464258517397235,0.7466420603008939,0.7468581112341797,0.7470740046089383,0.7472897404945762,0.7475053189605491,0.7477207400763611,0.7479360039115652,0.7481511105357628,0.7483660600186034,0.7485808524297847,0.7487954878390521,0.7490099663161993,0.7492242879310671,0.7494384527535440,0.7496524608535657,0.7498663123011151,0.7500800071662217,0.7502935455189623,0.7505069274294599,0.7507201529678841,0.7509332222044508,0.7511461352094220,0.7513588920531056,0.7515714928058555,0.7517839375380708,0.7519962263201966,0.7522083592227230,0.7524203363161852,0.7526321576711635,0.7528438233582830,0.7530553334482135,0.7532666880116691,0.7534778871194084,0.7536889308422342,0.7538998192509931,0.7541105524165758,0.7543211304099164,0.7545315533019928,0.7547418211638260,0.7549519340664807,0.7551618920810637,0.7553716952787256,0.7555813437306594,0.7557908375081005,0.7560001766823269,0.7562093613246588,0.7564183915064583,0.7566272672991297,0.7568359887741188,0.7570445560029133,0.7572529690570421,0.7574612280080754,0.7576693329276246,0.7578772838873421,0.7580850809589211,0.7582927242140953,0.7585002137246392,0.7587075495623672,0.7589147317991343,0.7591217605068352,0.7593286357574048,0.7595353576228174,0.7597419261750872,0.7599483414862672,0.7601546036284503,0.7603607126737682,0.7605666686943914,0.7607724717625293,0.7609781219504299,0.7611836193303799,0.7613889639747039,0.7615941559557649};

inline double tanh_table(double dX)
{
	if(dX < -1.0) return -1.0;
	if(dX > 1.0) return 1.0;
	if(dX == 0) return 0.0;
	return dTanhTable[(int)(bipolarToUnipolar(dX)*4095.0)];

}