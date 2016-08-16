#include "AudioUtil.h"
#include <math.h>

/**
 * Takes a frequency and calculates a new frequency based on the number of semitones shifted.
 *
 * Only works with an integer number of steps on a 12-tone equal temperament scale.
 */
double AudioUtil::ScaleFrequency(double frequency, int numSemitones)
{
	double ratio = pow(1.0594630943592952, numSemitones);
	return ratio;
}

/**
 * Gets the frequency of a MIDI note to 3 decimal places.
 */
double AudioUtil::GetFrequencyFromMIDINote(int noteNumber)
{
	switch( noteNumber )
	{
	case 0: // C-1
		return 8.176;
	case 1:
		return 8.662;
	case 2:
		return 9.177;
	case 3:
		return 9.723;
	case 4:
		return 10.301;
	case 5:
		return 10.913;
	case 6:
		return 11.562;
	case 7:
		return 12.250;
	case 8:
		return 12.978;
	case 9: // A-1
		return 13.750;
	case 10:
		return 14.568;
	case 11:
		return 15.434;
	case 12: // C0
		return 16.352;
	case 13:
		return 17.324;
	case 14:
		return 18.354;
	case 15:
		return 19.445;
	case 16:
		return 20.602;
	case 17:
		return 21.827;
	case 18:
		return 23.125;
	case 19:
		return 24.500;
	case 20:
		return 25.957;
	case 21:
		return 27.500;
	case 22:
		return 29.135;
	case 23:
		return 30.868;
	case 24: // C1
		return 32.703;
	case 25:
		return 34.648;
	case 26:
		return 36.708;
	case 27:
		return 38.891;
	case 28:
		return 41.203;
	case 29:
		return 43.654;
	case 30:
		return 46.249;
	case 31:
		return 48.999;
	case 32:
		return 51.913;
	case 33: // A1
		return 55.000;
	case 34:
		return 58.270;
	case 35:
		return 61.735;
	case 36: // C2
		return 65.406;
	case 37:
		return 69.296;
	case 38:
		return 73.416;
	case 39:
		return 77.782;
	case 40:
		return 82.407;
	case 41:
		return 87.307;
	case 42:
		return 92.499;
	case 43:
		return 97.999;
	case 44:
		return 103.826;
	case 45: // A2
		return 110.000;
	case 46:
		return 116.541;
	case 47:
		return 123.471;
	case 48: // C3
		return 130.813;
	case 49:
		return 138.591;
	case 50:
		return 146.832;
	case 51:
		return 155.563;
	case 52:
		return 164.814;
	case 53:
		return 174.614;
	case 54:
		return 184.997;
	case 55:
		return 195.998;
	case 56:
		return 207.652;
	case 57: // A3
		return 220.000;
	case 58:
		return 233.082;
	case 59:
		return 246.942;
	case 60: // C4
		return 261.626;
	case 61:
		return 277.183;
	case 62:
		return 293.665;
	case 63:
		return 311.127;
	case 64:
		return 329.628;
	case 65:
		return 349.228;
	case 66:
		return 369.994;
	case 67:
		return 391.995;
	case 68:
		return 415.305;
	case 69: // A4
		return 440.000;
	case 70:
		return 466.164;
	case 71:
		return 493.883;
	case 72: // C5
		return 523.251;
	case 73:
		return 554.365;
	case 74:
		return 587.330;
	case 75:
		return 622.254;
	case 76:
		return 659.255;
	case 77:
		return 698.456;
	case 78:
		return 739.989;
	case 79:
		return 783.991;
	case 80:
		return 830.609;
	case 81: // A5
		return 880.000;
	case 82:
		return 932.328;
	case 83:
		return 987.767;
	case 84: // C6
		return 1046.502;
	case 85:
		return 1108.731;
	case 86:
		return 1174.659;
	case 87:
		return 1244.508;
	case 88:
		return 1318.510;
	case 89:
		return 1396.913;
	case 90:
		return 1479.978;
	case 91:
		return 1567.982;
	case 92:
		return 1661.219;
	case 93: // A6
		return 1760.000;
	case 94:
		return 1864.655;
	case 95:
		return 1975.533;
	case 96: // C7
		return 2093.005;
	case 97:
		return 2217.461;
	case 98:
		return 2349.318;
	case 99:
		return 2489.016;
	case 100:
		return 2637.020;
	case 101:
		return 2793.826;
	case 102:
		return 2959.955;
	case 103:
		return 3135.963;
	case 104:
		return 3322.438;
	case 105: // A7
		return 3520.000;
	case 106:
		return 3729.310;
	case 107:
		return 3951.066;
	case 108: // C8
		return 4186.009;
	case 109:
		return 4434.922;
	case 110:
		return 4698.636;
	case 111:
		return 4978.032;
	case 112:
		return 5274.041;
	case 113:
		return 5587.652;
	case 114:
		return 5919.911;
	case 115:
		return 6271.927;
	case 116:
		return 6644.875;
	case 117: // A8
		return 7040.00;
	case 118:
		return 7548.620;
	case 119:
		return 7902.133;
	case 120: // C9
		return 8372.018;
	case 121:
		return 8869.844;
	case 122:
		return 9397.273;
	case 123:
		return 9956.063;
	case 124:
		return 10548.08;
	case 125:
		return 11175.30;
	case 126:
		return 11839.82;
	case 127: // G9
		return 12543.85;
	default:
			return 0.0;
	}
}