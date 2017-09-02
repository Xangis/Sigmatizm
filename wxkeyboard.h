#ifndef _WXKEYBOARD_H_
#define _WXKEYBOARD_H_

#define MAX_OCTAVES 7
#define MAX_NOTES 128
#define MAX_HARMONICS 128
#define BUFFERLENGTH 256
#define SAMPLE_RATE (44100)
#define MINIPANELWIDTH 120
#define MINIPANELHEIGHT 100

/*!
 * Includes
 */
#include "wx/spinctrl.h"
#include "wx/filesys.h"
#include "wx/fs_zip.h"
#include "wx/html/helpctrl.h"
#include "wx/stdpaths.h"
#include "../wxAudioControls/wxBitmapSpinButton.h"
#include "../wxAudioControls/wxBitmapSlider.h"
#include "../wxAudioControls/wxOctaveCtrl.h"
#include "../wxAudioControls/wxKeylessButton.h"
#include "../wxAudioControls/wxKeylessChoice.h"
#include "../wxAudioControls/OctaveCallback.h"
#include "../wxAudioControls/wxSettingsFile.h"
#include "../wxAudioControls/wxVolumeMeter.h"
#include "../wxAudioControls/wxKeylessBitmapButton.h"
#include "../wxAudioControls/AdsrParameters.h"
#include "../wxAudioControls/LfoParameters.h"
#include "../StreamingAudioLib/Wavetable.h"
#include "SynthParametersDlg.h"
#ifndef VST
#include "../wxAudioControls/AudioSettingsInterface.h"
#include "../wxAudioControls/MidiSettingsInterface.h"
#include "portaudio.h"
#else
#include "aeffectx.h"
#include "audioeffectx.h"
#endif
#include "wxMiniWavePanel.h"
#define INITGUID

/*!
 * Forward declarations
 */
class wxSpinCtrl;

/*!
 * Control identifiers
 */
#define ID_KEYBOARD_DLG 10000
#define SYMBOL_WXKEYBOARD_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxWANTS_CHARS|wxMINIMIZE_BOX
#define SYMBOL_WXKEYBOARD_TITLE _("SigmaTizm")
#define SYMBOL_WXKEYBOARD_IDNAME ID_KEYBOARD_DLG
#define SYMBOL_WXKEYBOARD_SIZE wxSize(400, 300)
#define SYMBOL_WXKEYBOARD_POSITION wxDefaultPosition
#define ID_BANKSPIN 10001
#define ID_PATCHSPIN 10003
#define ID_KEYBOARD1 10006
#define ID_KEYBOARD2 10007
#define ID_KEYBOARD3 10008
#define ID_KEYBOARD4 10009
#define ID_KEYBOARD5 10010
#define ID_KEYBOARD6 10011
#define ID_KEYBOARD7 10012
#define ID_PATCHTEXT 10013
#define ID_CHANNELTEXT 10014
#define ID_PITCHWHEEL 10015
#define ID_MODWHEEL 10016
#define ID_PANICBUTTON 10017
#define ID_INFOBUTTON 10018
#define ID_HELPBUTTON 10019
#define ID_SAVEBUTTON 10021
#define ID_LOADBUTTON 10022
#define ID_MIDIBUTTON 10023
#define ID_BANKTEXT 10025
#define ID_VOLUME_LEFT 10027
#define ID_VOLUME_RIGHT 10028
#define ID_FILTERBUTTON 10029
#define ID_ADSRBUTTON 10030
#define ID_PARAMETERSBUTTON 10031
#define ID_LFOBUTTON 10032
#define ID_INITIALWAVE_PANEL 10033
#define ID_FINALWAVE_PANEL 10034
#define ID_BUTTON_NORMALIZE 10035
#define ID_BUTTON_INITIAL 10036
#define ID_BUTTON_FINAL 10037
#define ID_BUTTON_CLEAR 10038
#define ID_BUTTON_GENERATE 10039
#define ID_CHOICE_WAVEFORM 10040
#define ID_ZOOM_LEVEL 10041
#define ID_BUTTON_LEFT 10042
#define ID_BUTTON_RIGHT 10043
#define ID_LFO_FREQUENCY 10044
#define ID_LFO_WAVEFORM 10045
#define ID_TXT_FREQUENCY 10046
#define ID_ADSR_ATTACK 10047
#define ID_ADSR_DECAY 10048
#define ID_ADSR_SUSTAIN 10049
#define ID_ADSR_RELEASE 10050
#define ID_TXT_ATTACK 10051
#define ID_TXT_DECAY 10052
#define ID_TXT_SUSTAIN 10053
#define ID_TXT_RELEASE 10054
#define ID_BUTTON_SETTINGS 10055

/*!
 * Compatibility
 */
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wxKeyboard class declaration
 */
#ifndef VST
class wxKeyboard: public wxDialog, public OctaveCallback, public MidiSettingsInterface, public HarmonicsCallback, public AudioSettingsInterface
#else
class wxKeyboard: public wxDialog, public OctaveCallback, public HarmonicsCallback, public AudioEffectX
#endif
{
    DECLARE_DYNAMIC_CLASS( wxKeyboard )
    DECLARE_EVENT_TABLE()
public:
	virtual ~wxKeyboard();
    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WXKEYBOARD_IDNAME, const wxString& caption = SYMBOL_WXKEYBOARD_TITLE, const wxPoint& pos = SYMBOL_WXKEYBOARD_POSITION, const wxSize& size = SYMBOL_WXKEYBOARD_SIZE, long style = SYMBOL_WXKEYBOARD_STYLE );
    void CreateControls();
    void OnCloseWindow( wxCloseEvent& event );
    void OnKeyDown( wxKeyEvent& event );
	void OnHarmonicsChanged( float* initialHarmonics, float* finalHarmonics );
    void OnKeyUp( wxKeyEvent& event );
    void OnBankSpinUp( wxSpinEvent& event );
    void OnBankSpinDown( wxSpinEvent& event );
    void OnPatchSpinUp( wxSpinEvent& event );
    void OnPatchSpinDown( wxSpinEvent& event );
	void OnMidiSettings( wxCommandEvent& event );
	void OnPitchWheel( wxScrollEvent& event );
	void OnModWheel( wxScrollEvent& event );
	void OnPanic( wxCommandEvent& event );
	void OnInfo( wxCommandEvent& event );
	void OnHelp( wxCommandEvent& event );
	void OnAttack( wxScrollEvent& event );
	void OnDecay( wxScrollEvent& event );
	void OnSustain( wxScrollEvent& event );
	void OnRelease( wxScrollEvent& event );
	void OnLfoWaveform( wxCommandEvent& event );
	void OnSliderRelease( wxMouseEvent& event );
	void OnFilter( wxCommandEvent& event );
	void OnButtonLeft( wxCommandEvent& event );
	void OnButtonRight( wxCommandEvent& event );
	void OnLoadConfig( wxCommandEvent& event );
	void OnSaveConfig( wxCommandEvent& event );
	void OnPaintInitialWave( wxPaintEvent& event );
	void OnPaintFinalWave( wxPaintEvent& event );
	void OnNormalize( wxCommandEvent& event );
	void OnInitial( wxCommandEvent& event );
	void OnFinal( wxCommandEvent& event );
	void OnZoom( wxScrollEvent& event );
	void OnFrequency( wxScrollEvent& event );
#ifndef VST
    wxKeyboard();
    wxKeyboard(wxWindow* parent, wxWindowID id = SYMBOL_WXKEYBOARD_IDNAME, const wxString& caption = SYMBOL_WXKEYBOARD_TITLE, const wxPoint& pos = SYMBOL_WXKEYBOARD_POSITION, const wxSize& size = SYMBOL_WXKEYBOARD_SIZE, long style = SYMBOL_WXKEYBOARD_STYLE );
	int RenderAudio( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags );
    void OnSettings( wxCommandEvent& event );
    // AudioSettingsInterface methods.
    virtual void SelectAudioInputDevice(PaStreamParameters* device);
	virtual void SelectAudioOutputDevice(PaStreamParameters* device);
    // End AudioSettingsInterface methods.
#else
    wxKeyboard();
    wxKeyboard( audioMasterCallback audioMaster );
    wxKeyboard( audioMasterCallback audioMaster, wxWindow* parent, wxWindowID id = SYMBOL_WXKEYBOARD_IDNAME, const wxString& caption = SYMBOL_WXKEYBOARD_TITLE, const wxPoint& pos = SYMBOL_WXKEYBOARD_POSITION, const wxSize& size = SYMBOL_WXKEYBOARD_SIZE, long style = SYMBOL_WXKEYBOARD_STYLE );
	int RenderAudio( const void *input, void *output, unsigned long frameCount );
    // VST Methods
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstInt32 getNumMidiInputChannels ();
	virtual VstInt32 getNumMidiOutputChannels ();
    virtual void setSampleRate (float sampleRate);
	virtual VstInt32 processEvents (VstEvents* events);
    virtual void idle ();
    virtual long open( void* ptr );
    virtual void close();
    virtual VstInt32 dispatcher (VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
    // End VST Methods
#endif
    void ResetFocus();
	float InterpolateHarmonic(int note, int harmonicNumber);
	int GetOctaveByNote( int note );
	int GetNoteValue( int value );
	void OnClear( wxCommandEvent& event );
	void OnGenerate( wxCommandEvent& event );
	void NoteOff( int note );
	void AllControllersOff( void );
	void AllSoundOff( void );
    void SendMidiMessage( unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, bool shortmsg = false );
	void SetPatch( int patch, bool receivedFromMidi = false );
	void SetBank( int bank, bool receivedFromMidi = false );
    void OnMouseRelease( wxMouseEvent &event );
    wxBitmap GetBitmapResource( const wxString& name );
    wxIcon GetIconResource( const wxString& name );
    static bool ShowToolTips();
	float GetEnvelopeVolume(int note, int playbackSamplePosition, int offset = 0);
	// OctaveCallback members.
	void PlayNote( int note, bool receivedFromMidi = false );
	void StopNote( int note, bool receivedFromMidi = false );
	void AllNotesOff( bool receivedFromMidi = false );
	void OnRightClick( int note );
    void ProcessMidiMessage(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
	// End OctaveCallback members.
	// MidiSettingsInterface methods.
	void SelectMidiInputDevice(int number);
	void SelectMidiOutputDevice(int number);
	void SelectMidiInputChannel(int number);
	void SelectMidiOutputChannel(int number);
	void EnableMidiOutput(bool enabled);
	// End MidiSettingsInterface methods.
	bool _done;
private:
	//DelayEffect* _delayEffect;
	float _millisecondsPerSample;
	wxMiniWavePanel* _initialWave;
	wxMiniWavePanel* _finalWave;
	WaveTable* _waveTable;
	int _numOctaves;
	wxSettingsFile* _settingsFile;
	wxStaticText* _bankText;
	wxStaticText* _patchText;
	wxStaticText* _txtZoomLevel;
	wxStaticText* _txtFrequency;
	wxStaticText* _txtAttack;
	wxStaticText* _txtDecay;
	wxStaticText* _txtSustain;
	wxStaticText* _txtRelease;
    wxBitmapSpinButton* _bankSpin;
    wxBitmapSpinButton* _patchSpin;
	wxKeylessChoice* _chWaveform;
	wxKeylessChoice* _chLFOWaveform;
	wxKeylessButton* _btnInitial;
	wxKeylessButton* _btnFinal;
	wxKeylessButton* _btnNormalize;
	wxKeylessButton* _btnGenerate;
	wxKeylessButton* _btnClear;
	wxKeylessBitmapButton* _btnLeft;
	wxKeylessBitmapButton* _btnRight;
	wxKeylessBitmapButton* _panicButton;
	wxKeylessBitmapButton* _infoButton;
	wxKeylessBitmapButton* _helpButton;
	wxKeylessBitmapButton* _saveButton;
	wxKeylessBitmapButton* _loadButton;
	wxKeylessBitmapButton* _midiButton;
	wxBitmapSlider* _slFrequency;
    wxBitmapSlider* _pitchWheel;
    wxBitmapSlider* _modWheel;
	wxBitmapSlider* _slAttack;
	wxBitmapSlider* _slDecay;
	wxBitmapSlider* _slSustain;
	wxBitmapSlider* _slRelease;
	wxSlider* _slZoomLevel;
#ifndef linux
	wxVolumeMeter* _volumeMeterL;
	wxVolumeMeter* _volumeMeterR;
#endif
    wxColour _textColour;
    wxColour _backgroundColour;
	wxHtmlHelpController* _helpCtrl;
    int _inputChannel;
	int _outputChannel;
	int _midiInputDeviceNumber;
	int _midiOutputDeviceNumber;
    int _ipatch;
    int _ibank;
	int _bufferSize; // Buffer size in number of milliseconds.
	float _lfoWavetablePosition;
    // TODO: Move everything that is an array of MAX_NOTE info into its own struct.
    bool _playing[MAX_NOTES];
	bool _releasing[MAX_NOTES];
    float _volume[MAX_NOTES];
    unsigned int _numSamplesReleased[MAX_NOTES];
	unsigned int _playbackPosition[MAX_NOTES]; // The number of samples that have played since the note has been triggered.  Used for envelope calculation.
	float _wavetablePosition[MAX_NOTES][MAX_HARMONICS]; // The wavetable positions for each note.
	float _initialHarmonicLevels[MAX_HARMONICS]; // Levels of each harmonic.
	float _finalHarmonicLevels[MAX_HARMONICS];
	double _masterVolume;
    wxOctaveCtrl* _octave[MAX_OCTAVES];
	bool _midiOutputEnabled;
	wxIcon _icon;
	int _sampleRate;
	AdsrParameters _adsrParameters;
	LFOParameters _lfoParameters;
#ifndef VST
    wxKeylessBitmapButton* _settingsButton;
	PaStream *_buffer;
#endif
	SynthParametersDlg* _timbrePanel;
};

#endif
