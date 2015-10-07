# Sigmatizm

A virtual additive synthesizer for Linux and Windows with MIDI support. It is designed
for live play and was originally released on http://zetacentauri.com and via the
Ubuntu Software Center.

![Sigmatizm Screenshot](https://github.com/Xangis/Sigmatizm/blob/master/images/Sigmatizm0.96.png)

Additive synthesis works by using a large number of sine wave oscillators at different 
frequencies to create complex musical timbres. It is based on the mathematical theories 
of Joseph Fourier and has been used in the popular Kawai K5000 series of hardware 
synthesizers. SigmaTizm has 128 harmonic oscillators, allowing it to create rather 
complex timbres.

No plugin host or additional software and hardware is required to run it. It should run 
on any windows PC created within the last 5 years. Additive synthesis is fairly 
CPU-intensive, so playing a large number of notes at once or configuring a large number 
of harmonics (>64) can cause high CPU usage which can result in sound glitches on slower 
systems.

To get started, click the folder icon to load a predefined "patch" and press keys on your 
keyboard or click the keyboard keys with your mouse to play sound. For detailed 
instructions, click the "?" icon.

A prebuilt Windows installer and Linux binaries are in the installer folder.

Building this application requires wxWidgets. It has been built with v2.8 and 2.9. It also
uses PortAudio and RtMidi.

This application was partially but not fully ported to OSX. It will probably not build 
right out of the box, but if you can get wxWidgets and GCC to play nice, it might not be
too tough to build.

Current Features:

- Notes can be played using the computer keyboard and mouse.
- Can be used as a controller of external MIDI.
- Can be controlled via an external MIDI device (keyboard, etc.)
- Additive synthesis engine with 128 harmonic oscillators.
- Allows detailed point-and-click editing of harmonics.
- User-configurable vibrato LFO with multiple waveforms.
- Adjustable ADSR envelope settings.
- 7-octave keyboard.
- Separate attack and sustain harmonics for changing sound timbres.
- Pitch wheel control lets you 'bend' notes up or down in frequency.
- Click-and-drag to modify harmonics while sound is playing for interesting sound morphs.
- Selection of sound output devices using ASIO, MME, or DirectSound drivers (on Windows).
