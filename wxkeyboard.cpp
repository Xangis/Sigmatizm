// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/aboutdlg.h"
#include "../wxAudioControls/wxMidiSettingsDlg.h"
#include "../wxAudioControls/wxAudioSettings.h"
#include "../StreamingAudioLib/AudioUtil.h"
#include <vector>

#include "wx/wx.h"

#include "wxkeyboard.h"
#include "wxkeyboardapp.h"

// Load XPM Images
#include "left.xpm"
#include "right.xpm"
#include "info.xpm"
#include "help.xpm"
#include "midiport.xpm"
#include "exclamation.xpm"
#include "wrench.xpm"
#include "disk.xpm"
#include "btn1sml.xpm"
#include "btn1lrg.xpm"
#include "openfolder.xpm"
#include "spin.xpm"

IMPLEMENT_DYNAMIC_CLASS( wxKeyboard, wxDialog )

BEGIN_EVENT_TABLE( wxKeyboard, wxDialog )
    EVT_CLOSE( wxKeyboard::OnCloseWindow )
    EVT_KEY_DOWN( wxKeyboard::OnKeyDown )
    EVT_KEY_UP( wxKeyboard::OnKeyUp )
	EVT_BUTTON( ID_PANICBUTTON, wxKeyboard::OnPanic )
	EVT_BUTTON( ID_INFOBUTTON, wxKeyboard::OnInfo )
	EVT_BUTTON( ID_HELPBUTTON, wxKeyboard::OnHelp )
	EVT_BUTTON( ID_MIDIBUTTON, wxKeyboard::OnMidiSettings )
	EVT_BUTTON( ID_SAVEBUTTON, wxKeyboard::OnSaveConfig )
	EVT_BUTTON( ID_LOADBUTTON, wxKeyboard::OnLoadConfig )
	EVT_BUTTON( ID_FILTERBUTTON, wxKeyboard::OnFilter )
	EVT_BUTTON( ID_BUTTON_CLEAR, wxKeyboard::OnClear )
	EVT_BUTTON( ID_BUTTON_GENERATE, wxKeyboard::OnGenerate )
#ifndef VST
    EVT_BUTTON( ID_BUTTON_SETTINGS, wxKeyboard::OnSettings )
#endif
	EVT_SPIN_UP( ID_BANKSPIN, wxKeyboard::OnBankSpinUp )
    EVT_SPIN_DOWN( ID_BANKSPIN, wxKeyboard::OnBankSpinDown )
    EVT_SPIN_UP( ID_PATCHSPIN, wxKeyboard::OnPatchSpinUp )
    EVT_SPIN_DOWN( ID_PATCHSPIN, wxKeyboard::OnPatchSpinDown )
	EVT_COMMAND_SCROLL( ID_MODWHEEL, wxKeyboard::OnModWheel )
	EVT_COMMAND_SCROLL( ID_PITCHWHEEL, wxKeyboard::OnPitchWheel )
	EVT_COMMAND_SCROLL( ID_ZOOM_LEVEL, wxKeyboard::OnZoom )
	EVT_COMMAND_SCROLL( ID_LFO_FREQUENCY, wxKeyboard::OnFrequency )
	EVT_BUTTON( ID_BUTTON_NORMALIZE, wxKeyboard::OnNormalize )
	EVT_BUTTON( ID_BUTTON_INITIAL, wxKeyboard::OnInitial )
	EVT_BUTTON( ID_BUTTON_FINAL, wxKeyboard::OnFinal )
	EVT_BUTTON( ID_BUTTON_LEFT, wxKeyboard::OnButtonLeft )
	EVT_BUTTON( ID_BUTTON_RIGHT, wxKeyboard::OnButtonRight )
	EVT_CHOICE( ID_LFO_WAVEFORM, wxKeyboard::OnLfoWaveform )
    EVT_LEFT_UP(wxKeyboard::OnMouseRelease) // Catches note off outside of an octave control.
	EVT_COMMAND_SCROLL( ID_ADSR_ATTACK, wxKeyboard::OnAttack )
	EVT_COMMAND_SCROLL( ID_ADSR_DECAY, wxKeyboard::OnDecay )
	EVT_COMMAND_SCROLL( ID_ADSR_SUSTAIN, wxKeyboard::OnSustain )
	EVT_COMMAND_SCROLL( ID_ADSR_RELEASE, wxKeyboard::OnRelease )
END_EVENT_TABLE()

#ifndef VST
/**
* Callback for PortAudio rendering.
*/
int AudioCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	wxKeyboard* keyboard = (wxKeyboard *)userData;
	if( keyboard != NULL && !keyboard->_done )
	{
		return keyboard->RenderAudio(input, output, frameCount, timeInfo, statusFlags );
	}
	return 0;
}

wxKeyboard::wxKeyboard( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/**
* Empty constructor.  Must not be used in VST mode.
*/
wxKeyboard::wxKeyboard()
{
}
#else
wxKeyboard::wxKeyboard() : AudioEffectX((audioMasterCallback)NULL, 0, 0)
{
}

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new wxKeyboard(audioMaster, NULL, ID_KEYBOARD_DLG);
}

wxKeyboard::wxKeyboard( audioMasterCallback audioMaster) : AudioEffectX(audioMaster, 0, 0)
{
}

wxKeyboard::wxKeyboard( audioMasterCallback audioMaster, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) : AudioEffectX(audioMaster, 0, 0)
{
    Create(parent, id, caption, pos, size, style);
}
#endif

wxKeyboard::~wxKeyboard()
{
	_done = true;
	wxThread::Sleep(20);

#ifndef VST
	if( _midiInDevice != NULL )
	{
		delete _midiInDevice;
	}
	if( _midiOutDevice != NULL )
	{
		delete _midiOutDevice;
	}
	PaError err = Pa_AbortStream( _buffer );
	// Don't show error messages in the destructor -- we couldn't care less.
    //if( err != paNoError )
	//	wxMessageBox(  wxString::Format(_("PortAudio error: %s\n"), Pa_GetErrorText( err ) ));
	err = Pa_CloseStream( _buffer );
	//if( err != paNoError )
	//   wxMessageBox(  wxString::Format(_("PortAudio error: %s\n"), Pa_GetErrorText( err )) );
	err = Pa_Terminate();
	//if( err != paNoError )
	//   wxMessageBox(  wxString::Format(_("PortAudio error: %s\n"), Pa_GetErrorText( err )) );
#endif
}

/**
* Handles dialog creation, initializes audio.
*/
bool wxKeyboard::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
#ifdef VST
    // VST Init
    setNumInputs(0); // no inputs
    setNumOutputs(2); // 2 outputs, stereo
    canProcessReplacing();
    canDoubleReplacing(false);
    setEditor((wxKeyboardApp*)wxTheApp);
    isSynth();
    setUniqueID ('ZSig');
	suspend( );
    // End VST Init
#else
    _midiInDevice = new RtMidiIn();
	_midiOutDevice = new RtMidiOut();
#endif

	_done = false;
	_waveTable = new WaveTable();
	_waveTable->CreateWavetables();
	_sampleRate = SAMPLE_RATE;
	memset(&_playbackPosition[0], 0, (sizeof(unsigned int) * MAX_NOTES));
	memset(&_numSamplesReleased[0], 0, (sizeof(unsigned int) * MAX_NOTES));
	memset(&_playing[0], 0, (sizeof(bool) * MAX_NOTES ));
	memset(&_releasing[0], 0, (sizeof(bool) * MAX_NOTES ));
	memset(&_initialHarmonicLevels[0], 0, (sizeof(float) * MAX_HARMONICS ));
	memset(&_finalHarmonicLevels[0], 0, (sizeof(float) * MAX_HARMONICS ));
	memset(&_wavetablePosition[0], 0, (sizeof(float) * MAX_NOTES * MAX_HARMONICS));
    for( int k = 0; k < MAX_NOTES; k++ )
    {
        _volume[k] = 1.0;
    }
	_initialHarmonicLevels[0] = 0.5f;
	_finalHarmonicLevels[0] = 0.5f;
	_millisecondsPerSample = 1000.0f / _sampleRate;
	_masterVolume = 1.0;
	_midiOutputEnabled = true;
	_bufferSize = 20;
	_txtFrequency = NULL;
	_slFrequency = NULL;
	_chLFOWaveform = NULL;
	_btnNormalize = NULL;
	_btnInitial = NULL;
	_btnFinal = NULL;
	_btnClear = NULL;
	_btnLeft = NULL;
	_btnRight = NULL;
	_btnGenerate = NULL;
	_slZoomLevel = NULL;
	_slAttack = NULL;
	_slDecay = NULL;
    _slSustain = NULL;
    _slRelease = NULL;
	_txtZoomLevel = NULL;
	_txtAttack = NULL;
	_txtDecay = NULL;
	_txtSustain = NULL;
	_txtRelease = NULL;
	_lfoWavetablePosition = 0.0f;
	_settingsFile = new wxSettingsFile;
    _textColour.Set( 255, 204, 102 );
    _backgroundColour.Set( 102, 51, 0 );
	// Set up default ADSR settings.
	_adsrParameters.attackTimeMsec = 0;
	_adsrParameters.decayTimeMsec = 0;
	_adsrParameters.sustainLevelPercent = 100;
	_adsrParameters.releaseTimeMsec = 10;
	_adsrParameters.enabled = true;
	// Set up default LFO settings.
	_lfoParameters.lfoFrequency = 5;
	_lfoParameters.lfoWaveform = WAVEFORM_SINE;
	_lfoParameters.enabled = true;
	_lfoParameters.depth = 0.0f;
	_chWaveform = NULL;
	// Set up delay effect (requires the AudioEffect library, which is not included).
	//_delayEffect = new DelayEffect(true);
	//_delayEffect->SetDelayTime(200, 44100);
	//_delayEffect->SetWetMix(0.5);
#ifndef linux
	_volumeMeterL = NULL;
	_volumeMeterR = NULL;
#endif
	_initialWave = NULL;
	_finalWave = NULL;
    _bankSpin = NULL;
    _patchSpin = NULL;
	_bankText = NULL;
	_patchText = NULL;
	_panicButton = NULL;
	_infoButton = NULL;
	_helpButton = NULL;
	_saveButton = NULL;
	_loadButton = NULL;
	_midiButton = NULL;
	_inputChannel = 1;
	_outputChannel = 1;
	_ipatch = 1;
	_ibank = 1;
	_helpCtrl = NULL;
    _octave[0] = NULL;
    _octave[1] = NULL;
    _octave[2] = NULL;
    _octave[3] = NULL;
    _octave[4] = NULL;
    _octave[5] = NULL;
    _octave[6] = NULL;
	_numOctaves = 7;
	_midiInputDeviceNumber = 0;  // MIDI Mapper
	_midiOutputDeviceNumber = 0;  // Default Output
	EnableMidiOutput(false);
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
	ResetFocus();

	wxFileSystem::AddHandler(new wxZipFSHandler);
	_helpCtrl = new wxHtmlHelpController(wxHF_CONTENTS);
#ifndef __APPLE__
    wxString helpFile = _("sigmatizm.htb");
#else
    wxString helpFile = wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("sigmatizm.htb"));
#endif
    if( !_helpCtrl->AddBook(helpFile))
    {
        wxMessageBox( _("Unable to load help file.  Please make sure that sigmatizm.htb is in the program directory.") );
    }
#ifndef __APPLE__
        wxString filepath = _("sigmatizm.ico");
#else
        wxString filepath = wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("sigmatizm.ico"));
#endif
	if( _icon.LoadFile(filepath, wxBITMAP_TYPE_ICO ))
	{
		SetIcon(_icon);
	}

#ifndef VST
    // Open default MIDI devices.
    int numInDevices = _midiInDevice->getPortCount();
    if( numInDevices > 0 )
    {
	    SelectMidiInputDevice(_midiInputDeviceNumber);
    }
    int numOutDevices = _midiOutDevice->getPortCount();
    if( numOutDevices > 0 )
    {
	    SelectMidiOutputDevice(_midiOutputDeviceNumber);
    }

    if( numOutDevices < 1 || numInDevices < 1 )
    {
        wxString in = _("");
        wxString out = _("");
        if( numInDevices < 1 )
        {
            in = _("No MIDI input devices detected.  MIDI input is disabled.");
        }
        if( numOutDevices < 1 )
        {
            out = _("No MIDI output devices detected.  MIDI output is disabled.");
        }
        wxMessageBox(wxString::Format(_("%s\n\n%s"), in, out ));
    }

    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        wxMessageBox( _("PortAudio error: %s\n"), wxString::FromAscii(Pa_GetErrorText( err )) );
    }
    Pa_OpenDefaultStream( &_buffer, 0, 2, paFloat32, _sampleRate, BUFFERLENGTH, AudioCallback, this );
    err = Pa_StartStream( _buffer );
    if( err != paNoError )
    {
        wxMessageBox( _("PortAudio error: %s\n"), wxString::FromAscii(Pa_GetErrorText( err )) );
    }
#endif

	return true;
}


void wxKeyboard::CreateControls()
{
    wxImage spinImage(spin_xpm);
    wxImage sliderBk;
    wxImage sliderInd;
    wxImage hsliderBk;
    wxImage hsliderInd;
#ifndef __APPLE__
    sliderBk.LoadFile( _("sliderbk.bmp"), wxBITMAP_TYPE_BMP );
    sliderInd.LoadFile( _("sliderind.bmp"), wxBITMAP_TYPE_BMP );
    hsliderBk.LoadFile( _("hsliderbk.bmp"), wxBITMAP_TYPE_BMP );
    hsliderInd.LoadFile( _("hsliderind.bmp"), wxBITMAP_TYPE_BMP );
#else
    sliderBk.LoadFile(wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("sliderbk.bmp")), wxBITMAP_TYPE_BMP );
    sliderInd.LoadFile(wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("sliderind.bmp")), wxBITMAP_TYPE_BMP );
    hsliderBk.LoadFile(wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("hsliderbk.bmp")), wxBITMAP_TYPE_BMP );
    hsliderInd.LoadFile(wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("hsliderind.bmp")), wxBITMAP_TYPE_BMP );
#endif

     int buttonWidth = 76;
     int buttonHeight = 26;
     int buttonMargin = 3;
#ifdef linux
     buttonHeight = 28;
     buttonWidth = 84;
     buttonMargin = 1;
#endif
#ifdef __WXMAC__
     buttonHeight = 28;
     buttonWidth = 84;
     buttonMargin = 1;
#endif


#ifdef __WXMAC__
    // Required for the background color to take.
    wxPanel* itemDialog1 = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(mainSizer);
    mainSizer->Add(itemDialog1);
#else
    // On Windows, using an extra wxPanel adds unnecessary drawing flicker.
    wxKeyboard* itemDialog1 = this;
#endif

    itemDialog1->SetBackgroundColour(_backgroundColour);
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* horizTop = new wxBoxSizer(wxHORIZONTAL);

    // Show synth parameter edit dialog.
    _timbrePanel = new SynthParametersDlg(this);
    _timbrePanel->SetHarmonicsCallback(this);
    horizTop->Add(_timbrePanel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* sizer14 = new wxBoxSizer(wxVERTICAL);
    _txtZoomLevel = new wxStaticText( itemDialog1, wxID_STATIC, _("2x"), wxDefaultPosition, wxDefaultSize );
    _txtZoomLevel->SetForegroundColour(_textColour);
    sizer14->Add(_txtZoomLevel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 7);

	_slZoomLevel = new wxSlider( itemDialog1, ID_ZOOM_LEVEL, 2, 1, 10, wxDefaultPosition, wxSize(-1, 290), wxVERTICAL );
	_slZoomLevel->SetBackgroundColour(_backgroundColour);
	_slZoomLevel->SetForegroundColour(_textColour);
	_slZoomLevel->Connect(ID_ZOOM_LEVEL, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnSliderRelease), NULL, this);
	sizer14->Add(_slZoomLevel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);
	horizTop->Add(sizer14, 0, wxALIGN_LEFT|wxALIGN_TOP, 1);

	wxBoxSizer* vertTop = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* toprightsizer = new wxFlexGridSizer(6, 3, 0, 0);

	wxStaticText* staticText1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Attack (msec):"), wxDefaultPosition, wxDefaultSize, 0 );
    staticText1->SetForegroundColour(_textColour);
    toprightsizer->Add(staticText1, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_slAttack = new wxBitmapSlider( itemDialog1, ID_ADSR_ATTACK, 0, 0, 1000, wxDefaultPosition, wxSize(211,24) );
	_slAttack->SetBitmaps(&hsliderBk, &hsliderInd);
	_slAttack->SetValue(_adsrParameters.attackTimeMsec);
	toprightsizer->Add(_slAttack, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _txtAttack = new wxStaticText( itemDialog1, wxID_STATIC, _("0"), wxDefaultPosition, wxSize(32,-1), 0 );
    _txtAttack->SetForegroundColour(_textColour);
	_txtAttack->SetLabel(wxString::Format(_("%3d"), _adsrParameters.attackTimeMsec ));
    toprightsizer->Add(_txtAttack, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* staticText2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Decay (msec):"), wxDefaultPosition, wxDefaultSize, 0 );
    staticText2->SetForegroundColour(_textColour);
    toprightsizer->Add(staticText2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_slDecay = new wxBitmapSlider( itemDialog1, ID_ADSR_DECAY, 0, 0, 1000, wxDefaultPosition, wxSize(211,24));
	_slDecay->SetBitmaps(&hsliderBk, &hsliderInd);
	_slDecay->SetValue(_adsrParameters.decayTimeMsec);
	toprightsizer->Add(_slDecay, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _txtDecay = new wxStaticText( itemDialog1, wxID_STATIC, _("0"), wxDefaultPosition, wxSize(32,-1), 0 );
    _txtDecay->SetForegroundColour(_textColour);
	_txtDecay->SetLabel(wxString::Format(_("%3d"), _adsrParameters.decayTimeMsec ));
    toprightsizer->Add(_txtDecay, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* staticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Sustain (percent):"), wxDefaultPosition, wxDefaultSize, 0 );
    staticText3->SetForegroundColour(_textColour);
    toprightsizer->Add(staticText3, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_slSustain = new wxBitmapSlider( itemDialog1, ID_ADSR_SUSTAIN, 0, 0, 100, wxDefaultPosition, wxSize(211,24));
	_slSustain->SetBitmaps(&hsliderBk, &hsliderInd);
	_slSustain->SetValue(_adsrParameters.sustainLevelPercent);
	toprightsizer->Add(_slSustain, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _txtSustain = new wxStaticText( itemDialog1, wxID_STATIC, _("0"), wxDefaultPosition, wxSize(32,-1), 0 );
    _txtSustain->SetForegroundColour(_textColour);
	_txtSustain->SetLabel(wxString::Format(_("%3d"), _adsrParameters.sustainLevelPercent ));
    toprightsizer->Add(_txtSustain, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* staticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Release (msec):"), wxDefaultPosition, wxDefaultSize, 0 );
    staticText4->SetForegroundColour(_textColour);
    toprightsizer->Add(staticText4, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_slRelease = new wxBitmapSlider( itemDialog1, ID_ADSR_RELEASE, 0, 0, 1000, wxDefaultPosition, wxSize(211,24));
	_slRelease->SetBitmaps(&hsliderBk, &hsliderInd);
	_slRelease->SetValue(_adsrParameters.releaseTimeMsec);
	toprightsizer->Add(_slRelease, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _txtRelease = new wxStaticText( itemDialog1, wxID_STATIC, _("0"), wxDefaultPosition, wxSize(32,-1), 0 );
    _txtRelease->SetForegroundColour(_textColour);
	_txtRelease->SetLabel(wxString::Format(_("%3d"), _adsrParameters.releaseTimeMsec ));
    toprightsizer->Add(_txtRelease, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* staticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("LFO Freq (Hz):"), wxDefaultPosition, wxDefaultSize, 0 );
    staticText5->SetForegroundColour(_textColour);
    toprightsizer->Add(staticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_slFrequency = new wxBitmapSlider( itemDialog1, ID_LFO_FREQUENCY, 0, 0, 200, wxDefaultPosition, wxSize(211,24) );
	_slFrequency->SetBitmaps(&hsliderBk, &hsliderInd);
	_slFrequency->SetValue(_lfoParameters.lfoFrequency * 10);
	toprightsizer->Add(_slFrequency, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_txtFrequency = new wxStaticText( itemDialog1, ID_TXT_FREQUENCY, _("0.0"), wxDefaultPosition, wxSize(32, -1));
    _txtFrequency->SetForegroundColour(_textColour);
	_txtFrequency->SetLabel(wxString::Format(_("%3.1f"), _lfoParameters.lfoFrequency ));
	toprightsizer->Add(_txtFrequency, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* staticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("LFO Waveform:"), wxDefaultPosition, wxDefaultSize, 0 );
    staticText6->SetForegroundColour(_textColour);
    toprightsizer->Add(staticText6, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString choices;
    for( int i = 0; i < _waveTable->GetNumWaveforms(); i++ )
    {
        choices.Add(wxString::FromAscii(_waveTable->GetWaveformName(i)));
    }
	_chLFOWaveform = new wxKeylessChoice( itemDialog1, ID_LFO_WAVEFORM, wxDefaultPosition, wxSize(200,buttonHeight), choices);
	_chLFOWaveform->SetSelection(_lfoParameters.lfoWaveform);
#ifdef WIN32
    _chLFOWaveform->SetForegroundColour(_textColour);
	_chLFOWaveform->SetBackgroundColour(_backgroundColour);
#endif
	toprightsizer->Add(_chLFOWaveform, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    vertTop->Add(toprightsizer, 0, 0);

    wxFlexGridSizer* waveSizer = new wxFlexGridSizer(2, 2, 0, 0);
	_initialWave = new wxMiniWavePanel(itemDialog1, ID_INITIALWAVE_PANEL, wxDefaultPosition, wxSize(MINIPANELWIDTH,MINIPANELHEIGHT));
	_initialWave->SetBackgroundColour(wxColour( 0x22, 0x11, 0 ));
	waveSizer->Add(_initialWave, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 7);
	_finalWave = new wxMiniWavePanel(itemDialog1, ID_FINALWAVE_PANEL, wxDefaultPosition, wxSize(MINIPANELWIDTH,MINIPANELHEIGHT));
	_finalWave->SetBackgroundColour(wxColour( 0x22, 0x11, 0 ));
	waveSizer->Add(_finalWave, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 7 );

    wxStaticText* text9 = new wxStaticText(itemDialog1, wxID_STATIC, _("Attack"));
    text9->SetForegroundColour(_textColour);
    waveSizer->Add(text9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText* text10 = new wxStaticText(itemDialog1, wxID_STATIC, _("Sustain"));
    text10->SetForegroundColour(_textColour);
    waveSizer->Add(text10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 3);

	vertTop->Add(waveSizer, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    horizTop->Add(vertTop, 0, wxALIGN_LEFT|wxALIGN_TOP, 0);
	itemBoxSizer2->Add(horizTop, 0, wxALIGN_LEFT, 0);

    wxBoxSizer* horizTop3 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* static7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Wave:"), wxDefaultPosition, wxDefaultSize );
    static7->SetForegroundColour(_textColour);
    horizTop3->Add(static7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 4);

	wxArrayString harmonicChoices;
	harmonicChoices.Add(_("Square"));
	harmonicChoices.Add(_("Saw"));
	harmonicChoices.Add(_("Triangle"));
	harmonicChoices.Add(_("White Noise"));
	harmonicChoices.Add(_("Pink Noise"));
	harmonicChoices.Add(_("Brown Noise"));
	harmonicChoices.Add(_("Random Square"));
	harmonicChoices.Add(_("Random Saw"));
	harmonicChoices.Add(_("Random Triangle"));
	_chWaveform = new wxKeylessChoice( itemDialog1, ID_CHOICE_WAVEFORM, wxDefaultPosition, wxSize(90,buttonHeight), harmonicChoices);
	_chWaveform->SetSelection(0);
#ifdef WIN32
	_chWaveform->SetForegroundColour(_textColour);
	_chWaveform->SetBackgroundColour(_backgroundColour);
#endif
	horizTop3->Add(_chWaveform, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

	_btnGenerate = new wxKeylessButton( itemDialog1, ID_BUTTON_GENERATE, _T("Generate" ), wxDefaultPosition, wxSize(buttonWidth, buttonHeight));
    _btnGenerate->SetToolTip(_("Generate harmonics for the selected waveform"));
    horizTop3->Add(_btnGenerate, 0, wxALIGN_CENTER_VERTICAL|wxALL, buttonMargin );

    _btnClear = new wxKeylessButton( itemDialog1, ID_BUTTON_CLEAR, _T("Clear" ), wxDefaultPosition, wxSize(buttonWidth, buttonHeight));
    _btnClear->SetToolTip(_("Clear harmonic levels"));
    horizTop3->Add(_btnClear, 0, wxALIGN_CENTER_VERTICAL|wxALL, buttonMargin );

	_btnNormalize = new wxKeylessButton( itemDialog1, ID_BUTTON_NORMALIZE, _T("Normalize" ), wxDefaultPosition, wxSize(buttonWidth, buttonHeight));
    _btnNormalize->SetToolTip(_("Normalize harmonic levels for target polyphony"));
    horizTop3->Add(_btnNormalize, 0, wxALIGN_CENTER_VERTICAL|wxALL, buttonMargin );

	_btnInitial = new wxKeylessButton( itemDialog1, ID_BUTTON_INITIAL, _T("Attack" ), wxDefaultPosition, wxSize(buttonWidth, buttonHeight));
    _btnInitial->SetToolTip(_("Edit attack harmonics"));
    horizTop3->Add(_btnInitial, 0, wxALIGN_CENTER_VERTICAL|wxALL, buttonMargin );

	_btnLeft = new wxKeylessBitmapButton(itemDialog1, ID_BUTTON_LEFT, wxBitmap(left_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight));
    _btnLeft->SetToolTip(_("Copy final harmonics into initial harmonics"));
	horizTop3->Add(_btnLeft, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_btnRight = new wxKeylessBitmapButton(itemDialog1, ID_BUTTON_RIGHT, wxBitmap(right_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight));
    _btnRight->SetToolTip(_("Copy initial harmonics into final harmonics"));
	horizTop3->Add(_btnRight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );

    _btnFinal = new wxKeylessButton( itemDialog1, ID_BUTTON_FINAL, _T("Sustain" ), wxDefaultPosition, wxSize(buttonWidth, buttonHeight));
    _btnFinal->SetToolTip(_("Edit sustain harmonics"));
    // Make the sustain edit panel visible.
    _btnFinal->Enable(false);
    _timbrePanel->GetHarmonicPanel()->SetInitialVisible(false);
    horizTop3->Add(_btnFinal, 0, wxALIGN_CENTER_VERTICAL|wxALL, buttonMargin );

	_panicButton = new wxKeylessBitmapButton( itemDialog1, ID_PANICBUTTON, wxBitmap(exclamation_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_panicButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_panicButton->Connect(ID_PANICBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_panicButton->Connect(ID_PANICBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_panicButton->Connect(ID_PANICBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _panicButton->SetToolTip(_("Panic: Send all notes off MIDI message"));

	_infoButton = new wxKeylessBitmapButton( itemDialog1, ID_INFOBUTTON, wxBitmap(info_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_infoButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_infoButton->Connect(ID_INFOBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_infoButton->Connect(ID_INFOBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_infoButton->Connect(ID_INFOBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _infoButton->SetToolTip(_("About SigmaTizm"));

	_helpButton = new wxKeylessBitmapButton( itemDialog1, ID_HELPBUTTON, wxBitmap(help_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_helpButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_helpButton->Connect(ID_HELPBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_helpButton->Connect(ID_HELPBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_helpButton->Connect(ID_HELPBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _helpButton->SetToolTip(_("Help"));

#ifndef VST
	_settingsButton = new wxKeylessBitmapButton( itemDialog1, ID_BUTTON_SETTINGS, wxBitmap(wrench_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_settingsButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_settingsButton->Connect(ID_BUTTON_SETTINGS, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_settingsButton->Connect(ID_BUTTON_SETTINGS, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_settingsButton->Connect(ID_BUTTON_SETTINGS, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _settingsButton->SetToolTip(_("Change audio settings"));
#endif

	//wxBitmap filterBitmap( "filter.xpm", wxBITMAP_TYPE_XPM );
	//_filterButton = new wxBitmapButton( itemDialog1, ID_FILTERBUTTON, filterBitmap, wxDefaultPosition, wxSize( buttonHeight, buttonHeight ) );
	//itemBoxSizer3->Add(_filterButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	//_filterButton->Connect(ID_FILTERBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	//_filterButton->Connect(ID_FILTERBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	//_filterButton->Connect(ID_FILTERBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
 //   _filterButton->SetToolTip("Edit filter settings");

#ifndef DEMOVERSION
	_saveButton = new wxKeylessBitmapButton( itemDialog1, ID_SAVEBUTTON, wxBitmap(disk_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_saveButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_saveButton->Connect(ID_SAVEBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_saveButton->Connect(ID_SAVEBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_saveButton->Connect(ID_SAVEBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _saveButton->SetToolTip(_("Save Synthesizer Configuration to Disk"));
#endif

	_loadButton = new wxKeylessBitmapButton( itemDialog1, ID_LOADBUTTON, wxBitmap(openfolder_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_loadButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_loadButton->Connect(ID_LOADBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_loadButton->Connect(ID_LOADBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_loadButton->Connect(ID_LOADBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _loadButton->SetToolTip(_("Load Synthesizer Configuration from Disk"));

#ifndef linux
	wxBoxSizer* itemBoxSizer3b = new wxBoxSizer(wxVERTICAL);
	_volumeMeterL = new wxVolumeMeter( itemDialog1, ID_VOLUME_LEFT, 100, wxDefaultPosition, wxSize(100, 13));
	_volumeMeterL->Connect(ID_VOLUME_LEFT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	itemBoxSizer3b->Add(_volumeMeterL, 0, wxALL, 1);

	_volumeMeterR = new wxVolumeMeter( itemDialog1, ID_VOLUME_RIGHT, 100, wxDefaultPosition, wxSize(100, 13));
	_volumeMeterR->Connect(ID_VOLUME_RIGHT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	itemBoxSizer3b->Add(_volumeMeterR, 0, wxALL, 1);
	horizTop3->Add(itemBoxSizer3b, 0, wxALL, 1);
#endif

#ifndef VST
	_midiButton = new wxKeylessBitmapButton( itemDialog1, ID_MIDIBUTTON, wxBitmap(midiport_xpm), wxDefaultPosition, wxSize(buttonHeight, buttonHeight) );
	horizTop3->Add(_midiButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	_midiButton->Connect(ID_MIDIBUTTON, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_midiButton->Connect(ID_MIDIBUTTON, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);
	_midiButton->Connect(ID_MIDIBUTTON, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    _midiButton->SetToolTip(_("Change MIDI settings"));
#endif

	wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bank"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText4->SetForegroundColour(_textColour);
	itemStaticText4->Connect(wxID_STATIC, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    horizTop3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _bankText = new wxStaticText( itemDialog1, ID_BANKTEXT, _T("1  "), wxDefaultPosition, wxSize( 22, -1 ), 0 );
    _bankText->SetForegroundColour(_textColour);
	_bankText->Connect(ID_BANKTEXT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	horizTop3->Add(_bankText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_bankSpin = new wxBitmapSpinButton( itemDialog1, ID_BANKSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS|wxSP_VERTICAL );
    _bankSpin->SetBitmap( &spinImage );
    _bankSpin->SetRange(1, 128);
	_bankSpin->Connect(ID_BANKSPIN, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	_bankSpin->SetToolTip( _("Change patch bank" ));
	horizTop3->Add(_bankSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patch"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText8->SetForegroundColour(_textColour);
	itemStaticText8->Connect(wxID_STATIC, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    horizTop3->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _patchText = new wxStaticText( itemDialog1, ID_PATCHTEXT, _("1  "), wxDefaultPosition, wxSize( 22, -1 ), 0 );
    _patchText->SetForegroundColour(_textColour);
	_patchText->Connect(ID_PATCHTEXT, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
    horizTop3->Add(_patchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_patchSpin = new wxBitmapSpinButton( itemDialog1, ID_PATCHSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS|wxSP_VERTICAL );
    _patchSpin->SetBitmap( &spinImage );
    _patchSpin->SetRange(1, 128);
	_patchSpin->Connect(ID_PATCHSPIN, wxEVT_LEFT_UP, wxMouseEventHandler(wxKeyboard::OnMouseRelease), NULL, this);
	_patchSpin->SetToolTip( _("Change patch number") );
	horizTop3->Add(_patchSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	itemBoxSizer2->Add(horizTop3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	_pitchWheel = new wxBitmapSlider( itemDialog1, ID_PITCHWHEEL, 8192, 0, 16383, wxDefaultPosition, wxSize( 30, 108 ), wxSL_VERTICAL|wxSL_INVERSE );
    _pitchWheel->SetBitmaps( &sliderBk, &sliderInd );
    itemBoxSizer12->Add( _pitchWheel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	_pitchWheel->Connect(ID_PITCHWHEEL, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_pitchWheel->Connect(ID_PITCHWHEEL, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

	_modWheel = new wxBitmapSlider( itemDialog1, ID_MODWHEEL, 0, 0, 16383, wxDefaultPosition, wxSize( 30, 108 ), wxSL_VERTICAL|wxSL_INVERSE );
	_modWheel->SetBitmaps( &sliderBk, &sliderInd );
	itemBoxSizer12->Add( _modWheel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	_modWheel->Connect(ID_MODWHEEL, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_modWheel->Connect(ID_MODWHEEL, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

#ifndef __APPLE__
    wxString octavePath = _("octave.bmp");
#else
    wxString octavePath = wxString::Format(_("%s//%s"), wxStandardPaths::Get().GetResourcesDir(), _("octave.bmp"));
#endif
    wxBitmap _octaveBitmap;
    _octaveBitmap.LoadFile(octavePath, wxBITMAP_TYPE_BMP);
    wxBitmap* keyind1sml = new wxBitmap(btn1sml_xpm);
    wxBitmap* keyind1lrg = new wxBitmap(btn1lrg_xpm);
    _octave[0] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD1, _octaveBitmap, 24, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[0], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    _octave[0]->SetBitmaps(keyind1sml, keyind1lrg);
    _octave[0]->Connect(ID_KEYBOARD1, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
    _octave[0]->Connect(ID_KEYBOARD1, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[1] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD2, _octaveBitmap, 36, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[1], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[1]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[1]->Connect(ID_KEYBOARD2, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[1]->Connect(ID_KEYBOARD2, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[2] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD3, _octaveBitmap, 48, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[2], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[2]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[2]->Connect(ID_KEYBOARD3, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[2]->Connect(ID_KEYBOARD3, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[3] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD4, _octaveBitmap, 60, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[3], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[3]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[3]->Connect(ID_KEYBOARD4, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[3]->Connect(ID_KEYBOARD4, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[4] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD5, _octaveBitmap, 72, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[4], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[4]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[4]->Connect(ID_KEYBOARD5, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[4]->Connect(ID_KEYBOARD5, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[5] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD4, _octaveBitmap, 84, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[5], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[5]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[5]->Connect(ID_KEYBOARD6, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[5]->Connect(ID_KEYBOARD6, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

    _octave[6] = new wxOctaveCtrl( itemDialog1, ID_KEYBOARD5, _octaveBitmap, 96, this, wxDefaultPosition, wxSize(137, 99), wxNO_BORDER );
    itemBoxSizer12->Add(_octave[6], 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	_octave[6]->SetBitmaps(keyind1sml, keyind1lrg);
	_octave[6]->Connect(ID_KEYBOARD7, wxEVT_KEY_DOWN, wxKeyEventHandler(wxKeyboard::OnKeyDown), NULL, this);
	_octave[6]->Connect(ID_KEYBOARD7, wxEVT_KEY_UP, wxKeyEventHandler(wxKeyboard::OnKeyUp), NULL, this);

	// Set initial panel values.
	_timbrePanel->SetHarmonicLevels(&_initialHarmonicLevels[0], &_finalHarmonicLevels[0]);

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    _bankSpin->Refresh();
    _patchSpin->Refresh();
}

bool wxKeyboard::ShowToolTips()
{
    return true;
}

wxBitmap wxKeyboard::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("octave.bmp"))
    {
        wxBitmap bitmap(_T("octave.bmp"), wxBITMAP_TYPE_BMP);
        return bitmap;
    }
    return wxNullBitmap;
}

/**
* Enables or disables MIDI output.
*/
void wxKeyboard::EnableMidiOutput(bool enabled)
{
	_midiOutputEnabled = enabled;
}

wxIcon wxKeyboard::GetIconResource( const wxString& name )
{
    wxUnusedVar(name);
    return wxNullIcon;
}

/**
* Displays the MIDI settings dialog, changing settings based on responses.
*/
void wxKeyboard::OnMidiSettings( wxCommandEvent& )
{
#ifndef VST
	wxMidiSettingsDlg* dlg = new wxMidiSettingsDlg(this, this);
    dlg->SetForegroundColour(_textColour);
    dlg->SetBackgroundColour(_backgroundColour);
	dlg->SetSpinBitmap(spin_xpm);
	dlg->SetMidiOutputCheckbox(_midiOutputEnabled);
	dlg->SetMidiInputDeviceIndex(_midiInputDeviceNumber );
	dlg->SetMidiOutputDeviceIndex(_midiOutputDeviceNumber );
	dlg->SetMidiInputChannel(_inputChannel);
	dlg->SetMidiOutputChannel(_outputChannel);
	dlg->ShowModal();
	ResetFocus();
	delete dlg;
#endif
}

/**
* Changes the MIDI input channel.
*/
void wxKeyboard::SelectMidiInputChannel( int number )
{
    // Set MIDI Channel
	if( number > 16 )
	{
		number = 1;
	}
	AllNotesOff(); // We may or may not need to turn off all notes that were triggered by MIDI.
	_inputChannel = number;
}

/**
* Changes the MIDI output channel, turning off all notes on the previous channel.
*/
void wxKeyboard::SelectMidiOutputChannel( int number )
{
    // Set MIDI Channel
	if( number > 16 )
	{
		number = 1;
	}
	AllNotesOff();
	_outputChannel = number;
}

/**
* Handles bank up spinner events.
*/
void wxKeyboard::OnBankSpinUp( wxSpinEvent& event )
{
    // Set Bank
    int value = atoi(_bankText->GetLabel().ToAscii());
	++value;
	if( value > 128 )
	{
		value = 1;
	}
	SetBank( value );
	SetPatch( _ipatch );
	event.Skip();
}

/**
* Handles bank down spinner events.
*/
void wxKeyboard::OnBankSpinDown( wxSpinEvent& event )
{
    // Set Bank
    int value = atoi(_bankText->GetLabel().ToAscii());
	--value;
	if( value < 1 )
	{
		value = 128;
	}
	SetBank( value );
	SetPatch( _ipatch );
    event.Skip();
}

/**
* Changes patch banks, sending MIDI bank change messages.
*/
void wxKeyboard::SetBank( int bank, bool receivedFromMidi )
{
    // Send MIDI message
	// Bank change 0x00 0xB0 0x00 0x[bank]  - fine
	// Bank change 0x00 0x00 0x00 0x[bank]  - coarse
	_bankText->SetLabel( wxString::Format( _("%d"), bank ));
    _ibank = bank;
	if( _midiOutputEnabled && !receivedFromMidi )
	{
		// Coarse bank adjust: 00 (not used), 0xXX (bank), 0x00, 0xBX (channel)
		SendMidiMessage( 0, (bank-1), 0, (175 + _outputChannel) );
		// Fine bank adjust: 00 (not used), 0xXX (bank), 0x20, 0xBX (channel)
	}

}

/**
* Handles patch spinner up events.
*/
void wxKeyboard::OnPatchSpinUp( wxSpinEvent& event )
{
    // Set Patch
    int value = atoi(_patchText->GetLabel().ToAscii());
	++value;
	if( value > 128 )
	{
		value = 1;
	}
    // Send MIDI message
	// Patch change: 0xC[channel] + byte for patch change.
	SetPatch( value );
    event.Skip();
}

/**
* Handles patch spinner down events.
*/
void wxKeyboard::OnPatchSpinDown( wxSpinEvent& event )
{
    // Set Patch
    int value = atoi(_patchText->GetLabel().ToAscii());
	--value;
	if( value < 1 )
	{
		value = 128;
	}
	SetPatch( value );
	event.Skip();
}

/**
* Handles patch changes, sends MIDI patch change message.
*/
void wxKeyboard::SetPatch( int value, bool receivedFromMidi )
{
    // Send MIDI message
	// Patch change: 0xC[channel] + byte for patch change.
	_patchText->SetLabel( wxString::Format( _("%d"), value ));
    _ipatch = value;
	if( _midiOutputEnabled && !receivedFromMidi )
	{
		SendMidiMessage( 0, 0, (value - 1), (_outputChannel + 191), true );
	}
}

/**
* Handles window close events.
*/
void wxKeyboard::OnCloseWindow( wxCloseEvent& event )
{
    this->Destroy();
    wxThread::Sleep(12);
    event.Skip();
}

/**
* Handles key down events.
*/
void wxKeyboard::OnKeyDown( wxKeyEvent& event )
{
    int key = event.GetKeyCode();
	// Tab navigation explicitly disabled.
	if( key == WXK_TAB )
	{
        _octave[0]->SetFocus();
		return;
	}
	// Left and right arrows control the mod wheel -- currently not working.
	if( key == WXK_RIGHT || key == WXK_NUMPAD_RIGHT)
	{
		int max = _modWheel->GetMax();
		float val = _modWheel->GetValue();
		val += max / 10.0;
		if( val > max )
			val = max;
		_modWheel->SetValue(val);
		return;
	}
	if( key == WXK_LEFT || key == WXK_NUMPAD_LEFT )
	{
		int max = _modWheel->GetMax();
		int min = _modWheel->GetMin();
		float val = _modWheel->GetValue();
		val -= max / 10.0;
		if( val < min )
			val = min;
		_modWheel->SetValue(val);
		return;
	}
    int note = GetNoteValue( key );
	if( note == 0 )
	{
		event.Skip(true);
		return;
	}
	// Only play if the note is not already playing -- prevents retriggers.
	if( !_playing[note] )
	{
		_releasing[note] = false;
        _numSamplesReleased[note] = 0;
		PlayNote( note );
	}
	event.Skip(true);
}

/**
* Handles key up events, turns off a playing note.
*/
void wxKeyboard::OnKeyUp( wxKeyEvent& event )
{
	int key = event.GetKeyCode();
	int note = GetNoteValue( key );
	if( note == 0 )
	{
		event.Skip(true);
		return;
	}
	StopNote( note );
    event.Skip(true);
}

/**
* Gets the MIDI note number from a keyboard code.
*/
int wxKeyboard::GetNoteValue( int value )
{
	int note = 0;
    switch( value )
    {
    case WXK_F1:
        note = 24;
        break;
    case WXK_F2:
        note = 25;
        break;
    case WXK_F3:
        note = 26;
        break;
    case WXK_F4:
        note = 27;
        break;
    case WXK_F5:
        note = 28;
        break;
    case WXK_F6:
        note = 29;
        break;
    case WXK_F7:
        note = 30;
        break;
    case WXK_F8:
        note = 31;
        break;
    case WXK_F9:
        note = 32;
        break;
    case WXK_F10:
        note = 33;
        break;
    case WXK_F11:
        note = 34;
        break;
    case WXK_F12:
        note = 35;
        break;
    case '1':
        note = 36;
        break;
    case '2':
        note = 37;
        break;
    case '3':
        note = 38;
        break;
    case '4':
        note = 39;
        break;
    case '5':
        note = 40;
        break;
    case '6':
        note = 41;
        break;
    case '7':
        note = 42;
        break;
    case '8':
        note = 43;
        break;
    case '9':
        note = 44;
        break;
    case '0':
        note = 45;
        break;
    case '-':
        note = 46;
        break;
	case 43:
        note = 47;
        break;
    case 81: // q
        note = 48;
        break;
    case 87: // w
        note = 49;
        break;
    case 69: // e
        note = 50;
        break;
    case 82: // r
        note = 51;
        break;
    case 84: // t
        note = 52;
        break;
    case 89: // y
        note = 53;
        break;
    case 85: //
        note = 54;
        break;
    case 73: // i
        note = 55;
        break;
    case 79: // o
        note = 56;
        break;
    case 80: // p
        note = 57;
        break;
    case '[':
        note = 58;
        break;
    case ']':
        note = 59;
        break;
    case 65: // a
        note = 60;
        break;
    case 83: // s
        note = 61;
        break;
    case 68: // d
        note = 62;
        break;
    case 70: // f
        note = 63;
        break;
    case 71: // g
        note = 64;
        break;
    case 72: // h
        note = 65;
        break;
    case 74: // j
        note = 66;
        break;
    case 75: // k
        note = 67;
        break;
    case 76: // l
        note = 68;
        break;
    case ';':
        note = 69;
        break;
    case '\'':
        note = 70;
        break;
    case '\r':
        note = 71;
        break;
	case 90: // z
        note = 72;
        break;
    case 88: // x
        note = 73;
        break;
    case 67: // c
        note = 74;
        break;
    case 86: // v
        note = 75;
        break;
    case 66: // b
        note = 76;
        break;
    case 78: // n
        note = 77;
        break;
    case 77: // m
        note = 78;
        break;
    case ',':
        note = 79;
        break;
    case '.':
        note = 80;
        break;
    case '/':
        note = 81;
        break;
	case 306: // (right) shift
		note = 82;
		break;
	case 92: // backslash
		note = 83;
		break;
    default:
		note = 0;
        break;
    }
	return note;
}

/**
* Turns on a note and sends MIDI note on data.
*/
void wxKeyboard::PlayNote( int note, bool receivedFromMidi )
{
	if( _playing[note] == false )
	{
#ifdef DEMOVERSION
        // Limit polyphony for demo version.
        int noteCount = 0;
        for( int i = 0; i < MAX_NOTES; i++ )
        {
            if( _playing[i] ) ++noteCount;
        }
        if( noteCount > 0 )
        {
            return;
        }
#endif
		// Find out which octave control we're using so we can light the note on light.
		int octavenum = GetOctaveByNote(note);
		if( octavenum >= 0 && octavenum < MAX_OCTAVES )
		{
			// Turn on one of the 12 available notes in the octave.
			_octave[octavenum]->NoteIndicatorOn((note % 12));
		}

		if( _midiOutputEnabled && !receivedFromMidi )
		{
			// 00 (not used), 7F (velocity), 2B (note number), 9X (note on)+channel
			SendMidiMessage( 0, 127, note, (143 + _outputChannel) );
		}

		// Reset wavetable phase so we don't get clicks when note triggers.
		memset( _wavetablePosition[note], 0, (sizeof(float) * MAX_HARMONICS));
		// Reset envelope (playback) position.
		_playbackPosition[note] = 0;
		_playing[note] = true;
		_releasing[note] = false;
		_numSamplesReleased[note] = 0;
	}
}

/**
* Takes the note number and figures out which octave object it refers to.
*/
int wxKeyboard::GetOctaveByNote( int note )
{
	// Minimum octave and below is translated to minimum octave.
	if( note < 36 )
	{
		return 0;
	}
	// Maximum octave and above is translated to maximum octave.
	else if( note >= 96 )
	{
		return 6;
	}
	else
	{
		return (note - 24) / 12;
	}
}

/**
* Turns off a note, sends MIDI note off data.
*/
void wxKeyboard::StopNote( int note, bool receivedFromMidi )
{
	int octavenum = GetOctaveByNote(note);
	if( octavenum >= 0 && octavenum < MAX_OCTAVES )
	{
		// Turn off one of the 12 available notes in the octave.
		_octave[octavenum]->NoteIndicatorOff((note % 12 ));
	}

	if( _midiOutputEnabled && !receivedFromMidi )
	{
		// We send the note off regardless of whether it's actually on or not - we may 
		// have had a mistriggered event.
		// 00 (not used), 00 (velocity), 2B (note number), 80 (note off)+channel
		SendMidiMessage( 0, 0, note, (127 + _outputChannel) );
	}
	_numSamplesReleased[note] = 0;
	_releasing[note] = true;
	_playing[note] = false;
}

/**
* Processes mouse right clicks.  Turns off a playing note if it's on.
*/
void wxKeyboard::OnRightClick( int note )
{
	StopNote(note, false);
}

/**
* Callback for MIDI message data.
*/
void MidiMessageHandler( double deltatime, std::vector< unsigned char > *message, void *userData )
{
	wxKeyboard* key = (wxKeyboard*)userData;

    unsigned char a = 0;
    unsigned char b = 0;
    unsigned char c = 0;
    unsigned char d = 0;
    if( message->size() == 4 )
    {
        a = (*message)[3];
        b = (*message)[2];
        c = (*message)[1];
        d = (*message)[0];
    }
    else if( message->size() == 3 )
    {
        b = (*message)[2];
        c = (*message)[1];
        d = (*message)[0];
    }
    else if( message->size() == 2 )
    {
        c = (*message)[1];
        d = (*message)[0];
    }
    else if( message->size() == 1 )
    {
        d = (*message)[0];
    }
	key->ProcessMidiMessage(a, b, c, d);
}

/**
* Handles incoming MIDI message data.
*/
void wxKeyboard::ProcessMidiMessage(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4)
{
    //std::cout << "Received MIDI Message: " << (int)byte1 << "," << (int)byte2 << "," << (int)byte3 << "," << (int)byte4 << std::endl;
	// MIDI timimg clock pulse.  Doesn't mean anything to us.
	if( byte4 == 0xF8 )
	{
		return;
	}

	// Active sensing message.  Doesn't mean anything to us.
	if( byte4 == 0xFE )
	{
		return;
	}

	if( byte4 == (0x90 + _inputChannel - 1) )
	{
		if( byte2 > 0 )
		{
			_volume[byte3] = (float)byte2 / 127.0;
			PlayNote(byte3, true);
		}
		else
		{
			StopNote(byte3, true);
		}
		return;
	}
	// Note off (some keyboards send this instead of note on 0 velocity.
	if( byte4 == (0x80 + _inputChannel - 1)  )
	{
		StopNote( byte3, true );
		return;
	}
	// Program change.
	else if( byte4 == (0xC0 + _inputChannel - 1)  )
	{
		SetPatch( byte3, true );
		return;
	}
	else if( byte4 == (0xB0 + _inputChannel - 1)  )
	{
		// All notes off.
		if( byte3 == 0x7B && byte2 == 0x00 )
		{
			AllNotesOff(true);
            return;
		}
        else if( byte3 == 0x01 )
        {
            _modWheel->SetValue( byte2 * 128 );
            return;
        }
	}
    else if( byte4 == (0xE0 + _inputChannel - 1) )
    {
        // Pitch Wheel
        _pitchWheel->SetValue( byte2 * 128 );
        return;
    }
}

/**
* Processes MIDI input device selection changes.
*/
void wxKeyboard::SelectMidiInputDevice(int number)
{
#ifndef VST
	_midiInputDeviceNumber = number;
    try
    {
        _midiInDevice->closePort();
        _midiInDevice->openPort(number);
        _midiInDevice->setCallback(MidiMessageHandler, this);
    }
    catch( RtMidiError &error )
    {
        wxMessageBox(wxString::FromAscii(error.what()));
    }
#endif
}

/**
* Processes MIDI output device selection changes.
*/
void wxKeyboard::SelectMidiOutputDevice(int number)
{
#ifndef VST
	_midiOutputDeviceNumber = number;
    try
    {
        _midiOutDevice->closePort();
        _midiOutDevice->openPort(number);
    }
    catch( RtMidiError &error )
    {
        wxMessageBox(wxString::FromAscii(error.what()));
    }
#endif
}

/**
* Handles pitch wheel messages, sends MIDI controller data.
*/
void wxKeyboard::OnPitchWheel( wxScrollEvent& event )
{
	int value;
	if( event.GetEventType() == wxEVT_SCROLL_THUMBRELEASE )
	{
		value = 8192;
		_pitchWheel->SetValue(8192);
		Refresh();
	}
	else
	{
		value = _pitchWheel->GetValue();
	}
	// We may just want to use the fine value for this - coarse might be what hoses us.
	int coarse = value / 128;
	int fine = value % 128;
	if( _midiOutputEnabled )
	{
		// 00 (not used), XX (coarse), XX (fine), 0xEx (message + channel)
		SendMidiMessage( 0, coarse, fine, (223 + _outputChannel) );
	}
	event.Skip();
}

/**
* Handles mod wheel messages, sends MIDI controller data.
*/
void wxKeyboard::OnModWheel( wxScrollEvent& event )
{
	int value = _modWheel->GetValue();
	int coarse = value / 128;
	int fine = value % 128;
	if( _midiOutputEnabled )
	{
		// Modwheel message is two parts - coarse and fine.
		// 00 (not used), 0xXX (value:coarse), 0x01 (controller), 0xBX (175 + channel)
		//WORD loWord = MAKEWORD( coarse, 0 );
		//WORD hiWord = MAKEWORD( (175 + _outputChannel), 1 );
		//DWORD fullWord = MAKELONG( hiWord, loWord );
		SendMidiMessage( 0, coarse, 1, (175 + _outputChannel) );
		// 00 (not used), 0xXX (value:fine), 0x21 (controller), 0xBX (175 + channel)
		//loWord = MAKEWORD( fine, 0 );
		//hiWord = MAKEWORD( (175 + _outputChannel), 0x21 );
		//fullWord = MAKELONG( hiWord, loWord );
		SendMidiMessage( 0, fine, 0x21, (175 + _outputChannel) );
	}
	_lfoParameters.depth = ( (float)value / (float)_modWheel->GetMax() );
	event.Skip();
}

/**
* Handles panic button, turns everything off.
*/
void wxKeyboard::OnPanic( wxCommandEvent& event )
{
	AllNotesOff();
	AllControllersOff();
	AllSoundOff();
	ResetFocus();
	event.Skip();
}

/**
* Sends MIDI all sounds off message.
*/
void wxKeyboard::AllSoundOff( void )
{
	if( _midiOutputEnabled )
	{
		// 00 (not used), 0x00, 120, 0xBX (message + channel)
		SendMidiMessage( 0, 0, 120, (175 + _outputChannel) );
	}
}

/**
* Handles release of the left mouse button.
*/
void wxKeyboard::OnMouseRelease( wxMouseEvent& event )
{
	AllNotesOff();
	event.Skip();
}

/**
* Turns off all notes and indicators.
*/
void wxKeyboard::AllNotesOff( bool receivedFromMidi )
{
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		if( _playing[i] )
		{
			StopNote(i);
		}
		_playing[i] = false;
	}
	for(int i = 0; i < MAX_OCTAVES; i++ )
	{
		_octave[i]->TurnOffAll();
	}
	if( _midiOutputEnabled && !receivedFromMidi )
	{
		// 00 (not used), 0x00, 123, 0xBX (message + channel)
		SendMidiMessage( 0, 0, 123, (175 + _outputChannel) );
	}
	// Turn off any red dots.
	//Refresh();
}

/**
* Turns off all MIDI controllers.
*/
void wxKeyboard::AllControllersOff( void )
{
	if( _midiOutputEnabled )
	{
		// 00 (not used), 0x00, 121, 0xBX (message + channel)
		SendMidiMessage( 0, 0, 121, (175 + _outputChannel) );
	}
	// Reset modwheel position
	_modWheel->SetValue(0);
	_pitchWheel->SetValue(8192);
	Refresh();
}

/**
* Loads a synth config from disk and sets up the key map.
*/
void wxKeyboard::OnLoadConfig( wxCommandEvent& event )
{
	// This will probably only work right on Windows 7 or newer because XP has a different path structure.
	//wxString path = wxStandardPaths::Get().GetUserConfigDir() + _("\\..\\Local\\Sigmatizm");
	wxString path = _(".\\Patches");
	//wxMessageBox(path);
#ifndef __APPLE__
	wxFileDialog fdialog( this, _("Load A Config"), path, _(""), _("SigmaTizm Patches (*.sigm) |*.sigm||"), wxFD_OPEN );
#else
        wxString folderName = wxString::Format(_("%s/Patches/"), wxStandardPaths::Get().GetResourcesDir());
	wxFileDialog fdialog( this, _("Load A Config"), folderName, _(""), _("SigmaTizm Patches (*.sigm) |*.sigm||"), wxFD_OPEN );
#endif

	wxString filename;

	if( fdialog.ShowModal() != wxID_OK )
	{
		return;
	}

	wxString value;
	wxSettingsFile file;
	wxString fname = fdialog.GetPath();
	file.Load(fname);

    value = file.GetValue(_("Attack"));
    _adsrParameters.attackTimeMsec = atoi(value.ToAscii());
    _slAttack->SetValue(_adsrParameters.attackTimeMsec);
	_txtAttack->SetLabel(wxString::Format(_("%3d"), _adsrParameters.attackTimeMsec ));
    value = file.GetValue(_("Decay"));
    _adsrParameters.decayTimeMsec = atoi(value.ToAscii());
    _slDecay->SetValue(_adsrParameters.decayTimeMsec);
	_txtDecay->SetLabel(wxString::Format(_("%3d"), _adsrParameters.decayTimeMsec ));
    value = file.GetValue(_("Sustain"));
    _adsrParameters.sustainLevelPercent = atoi(value.ToAscii());
    _slSustain->SetValue(_adsrParameters.sustainLevelPercent);
	_txtSustain->SetLabel(wxString::Format(_("%3d"), _adsrParameters.sustainLevelPercent ));
    value = file.GetValue(_("Release"));
    _adsrParameters.releaseTimeMsec = atoi(value.ToAscii());
    _slRelease->SetValue(_adsrParameters.releaseTimeMsec);
	_txtRelease->SetLabel(wxString::Format(_("%3d"), _adsrParameters.releaseTimeMsec ));
    value = file.GetValue(_("LFOFrequency"));
    _lfoParameters.lfoFrequency = atof(value.ToAscii());
    _slFrequency->SetValue(_lfoParameters.lfoFrequency);
	_txtFrequency->SetLabel(wxString::Format(_("%3.1f"), _lfoParameters.lfoFrequency ));
    value = file.GetValue(_("LFOWaveform"));
    _lfoParameters.lfoWaveform = atoi(value.ToAscii());
    _chLFOWaveform->SetSelection(_lfoParameters.lfoWaveform);
    value = file.GetValue(_("ModWheel"));
    _modWheel->SetValue(atoi(value.ToAscii()));
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		value = file.GetValue(wxString::Format(_("AttackHarmonic%d"), i));
        _initialHarmonicLevels[i] = atof(value.ToAscii());
		value = file.GetValue(wxString::Format(_("SustainHarmonic%d"), i));
        _finalHarmonicLevels[i] = atof(value.ToAscii());
	}
    _timbrePanel->SetHarmonicLevels(_initialHarmonicLevels, _finalHarmonicLevels);
    ResetFocus();
    Refresh();
	event.Skip(false);
}

/**
* Saves a synth config to disk.
*/
void wxKeyboard::OnSaveConfig( wxCommandEvent& event )
{
#ifndef DEMOVERSION
	wxString path = wxStandardPaths::Get().GetUserConfigDir() + _("\\..\\Local\\Sigmatizm");
#ifndef __APPLE__
	wxFileDialog fdialog( this, _("Save Config As"), path, _(""), _("Sigmatizm Configurations (*.sigm) |*.sigm||"), wxFD_SAVE );
#else
        wxString folderName = wxString::Format(_("%s/Patches/"), wxStandardPaths::Get().GetResourcesDir());
	wxFileDialog fdialog( this, _("Save Config As"), folderName, _(""), _("Sigmatizm Configurations (*.sigm) |*.sigm||"), wxFD_SAVE );
#endif

	wxString filename;
	
	if( fdialog.ShowModal() != wxID_OK )
	{
		return;
	}

	wxSettingsFile file;
    file.SetValue(_("Attack"), _adsrParameters.attackTimeMsec );
    file.SetValue(_("Decay"), _adsrParameters.decayTimeMsec );
    file.SetValue(_("Sustain"), _adsrParameters.sustainLevelPercent );
    file.SetValue(_("Release"), _adsrParameters.releaseTimeMsec );
    file.SetValue(_("LFOFrequency"), _lfoParameters.lfoFrequency );
    file.SetValue(_("LFOWaveform"), _lfoParameters.lfoWaveform );
    file.SetValue(_("ModWheel"), _modWheel->GetValue() );
	for( int i = 0; i < MAX_NOTES; i++ )
	{
		file.SetValue(wxString::Format(_("AttackHarmonic%d"), i), _initialHarmonicLevels[i] );
		file.SetValue(wxString::Format(_("SustainHarmonic%d"), i), _finalHarmonicLevels[i] );
	}
	wxString name = fdialog.GetPath();
	file.Save(name);

    ResetFocus();
#endif
	event.Skip(false);
}

/**
* Shows help file.
*/
void wxKeyboard::OnHelp( wxCommandEvent& event )
{
	// Show help file.
	_helpCtrl->DisplayContents();
	ResetFocus();
	event.Skip();
}

/**
* Shows the dialog for editing filter settings.
*/
void wxKeyboard::OnFilter( wxCommandEvent& event )
{
	//wxFilterSettingsDlg* dlg = new wxFilterSettingsDlg(this);
 //   dlg->SetForegroundColour(wxColour( 150, 150, 250 ));
 //   dlg->SetBackgroundColour(wxColour( 0, 8, 40 ));
	//dlg->SetFilterParameters(_filterParameters);
	// Set enable state based on our settings.
//	if( _filterParameters.Frequency == 1.0f && _filterParameters.OneOverQ == 1.0f && _filterParameters.Type == LowPassFilter )
//	{
//		dlg->SetEnabled(false);
//	}
//	else
//	{
//		dlg->SetEnabled(true);
//	}
//	dlg->ShowModal();
//	// Enable or disable filter based on settings.
//	if( dlg->GetEnabled() == false )
//	{
//		// Set filter parameters to effectively bypassed.
//		_filterParameters.Frequency = 1.0f;
//		_filterParameters.OneOverQ = 1.0f;
//		_filterParameters.Type = LowPassFilter;
//	}
//	else
//	{
//		_filterParameters = dlg->GetFilterParameters();
//	}
//	// Change voice settings on ALL source voices.
//	for( int i = 0; i < MAX_NOTES; i++ )
//	{
//		if( _sample[i]->_sourceVoice != NULL )
//		{
//			HRESULT hr = _sample[i]->_sourceVoice->SetFilterParameters(&_filterParameters, XAUDIO2_COMMIT_NOW );
////			XAUDIO2_FILTER_PARAMETERS params;
////			_sample[i]->_sourceVoice->GetFilterParameters(&params);
//			_xAudio2->CommitChanges(XAUDIO2_COMMIT_ALL);
////			_sample[i]->_sourceVoice->GetFilterParameters(&params);
//			if( FAILED( hr ))
//			{
//				wxMessageBox( wxString::Format(_("Unable to create set source voice filter parameters: %d\n"), hr ));
//			}
//		}
//	}
	//delete dlg;
	ResetFocus();
	event.Skip();
}

/**
* Shows about box.
*/
void wxKeyboard::OnInfo( wxCommandEvent& event )
{
	// Show about box.
    wxAboutDialogInfo info;
    info.SetVersion(_("1.11"));
    info.SetCopyright(_("Copyright 2008-2016 Jason Champion"));
	info.AddDeveloper(_("Jason Champion"));
	info.SetIcon(_icon);
#ifdef DEMOVERSION
	info.SetLicense(_("The demo version of SigmaTizm may be distributed freely.\n\nThe software is provided 'as is', without warranty of any kind,\nexpress or implied, including but not limited to the warranties\nof merchantability, fitness for a particular purpose and\nnoninfringement.  In no event shall the authors or copyright\nholders be liable for any claim, damages or other liability,\nwhether in an action of contract, tort or otherwise, arising\nfrom, out of or in connection with the software or the use\nor other dealings in the software."));
    info.SetName(_("SigmaTizm Demo"));
	info.SetDescription(_("SigmaTizm uses the wxWidgets, PortAudio, and RtMidi libraries."));
#else
#ifdef VST
	info.SetLicense(_("SigmaTizm is copyrighted software and may not be distributed without a license.\n\nThe software is provided 'as is', without warranty of any kind,\nexpress or implied, including but not limited to the warranties\nof merchantability, fitness for a particular purpose and\nnoninfringement.  In no event shall the authors or copyright\nholders be liable for any claim, damages or other liability,\nwhether in an action of contract, tort or otherwise, arising\nfrom, out of or in connection with the software or the use\nor other dealings in the software.\n\nVST Plug-In Technology by Steinberg."));
    info.SetName(_("SigmaTizm VST"));
	info.SetDescription(_("SigmaTizm uses the wxWidgets libraries."));
#else
    info.SetLicense(_("Copyright 2008-2016 Jason Champion\n\nSigmaTizm is proprietary software and may not be distributed without a license.\n\nThe software is provided 'as is', without warranty of any kind,\nexpress or implied, including but not limited to the warranties\nof merchantability, fitness for a particular purpose and\nnoninfringement.  In no event shall the authors or copyright\nholders be liable for any claim, damages or other liability,\nwhether in an action of contract, tort or otherwise, arising\nfrom, out of or in connection with the software or the use\nor other dealings in the software."));
    info.SetName(_("SigmaTizm"));
	info.SetDescription(_("SigmaTizm uses the wxWidgets, PortAudio, and RtMidi libraries."));
#endif
#endif
	info.SetWebSite(_("http://zetacentauri.com"));

    wxAboutBox(info);
	ResetFocus();
    event.Skip();
}

#ifdef VST
/**
* Forwards the VST render call to the PortAudio RenderAudio call.
*/
void wxKeyboard::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    RenderAudio(*inputs, *outputs, sampleFrames);
}
#endif

/**
* Generates one buffer worth of audio and queues it up.  This is called by the PortAudio callback.
*/
#ifndef VST
int wxKeyboard::RenderAudio( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags )
#else
int wxKeyboard::RenderAudio( const void *input, void *output, unsigned long frameCount )
#endif
{
	float* buffer = (float*)output;
	float tmpval;
	memset(output, 0, (sizeof(float) * frameCount * 2));
	
	for( int i = 0; i < 128; i++ )
	{
		if( _playing[i] || _releasing[i] )
		{
			double frequency = AudioUtil::GetFrequencyFromMIDINote(i);
            double pitchWheelSemitoneAdjustment = (((double)(_pitchWheel->GetValue()) - 8192.0) / 4096.0);
            if( pitchWheelSemitoneAdjustment != 0.0f )
            {
        		double ratio = pow(1.0594630943592952, pitchWheelSemitoneAdjustment);
                frequency = frequency * ratio;
            }
			// Fill the buffer with samples.
			int count;
			// Precalculate our volume envelope so we don't recalculate it for each individual harmonic.
			float volEnvelope[BUFFERLENGTH];
			for( int j = 0; j < frameCount; j++ )
			{
				volEnvelope[j] = GetEnvelopeVolume(i, _playbackPosition[i], j) * _volume[i];
			}
			for( int harmonic = 0; harmonic < MAX_HARMONICS; harmonic++ )
			{
				if( _initialHarmonicLevels[harmonic] && _finalHarmonicLevels[harmonic] == 0.0f )
					continue;
				// Get the number of samples in a full cycle of the waveform at our current pitch.
				double samplesPerPeriod = _sampleRate / (frequency * (harmonic + 1));
				// Get the amount of forward progress in the wave table per sample expressed as a percentage.
				double sampleSkip = 1.0 / samplesPerPeriod;
				for( count = 0; count < frameCount; count++ )
				{
					while( _wavetablePosition[i][harmonic] > 1.0 )
					{
						_wavetablePosition[i][harmonic] -= 1.0;
					}
					tmpval = ((_waveTable->_waveformTable[WAVEFORM_SINE][(int)(_wavetablePosition[i][harmonic] * TABLESIZE)] ) * InterpolateHarmonic(i,harmonic) * volEnvelope[count]);
					buffer[count*2] += (tmpval);
					buffer[count*2+1] += (tmpval);
					// Jump ahead a certain percent of our wavetable.
					_wavetablePosition[i][harmonic] += sampleSkip;
				}
			}
			_playbackPosition[i] += frameCount;
            if( _releasing[i] )
            {
                _numSamplesReleased[i] += frameCount;
            }
		}
	}

	double lfoSamplesPerPeriod = _sampleRate / _lfoParameters.lfoFrequency;
	double lfoSkip = 1.0 / lfoSamplesPerPeriod;
	float maxLeft = 0.0f;
	float maxRight = 0.0f;
	int pos = 0;
	for( int cnt = 0; cnt < frameCount; cnt++ )
	{
		pos = cnt * 2;
		if( _lfoParameters.enabled )
		{
			buffer[pos] = ((1.0f - _lfoParameters.depth ) * buffer[pos]) + (_waveTable->_waveformTable[_lfoParameters.lfoWaveform][(int)(_lfoWavetablePosition * TABLESIZE)] * _lfoParameters.depth * buffer[pos]);
			buffer[pos+1] = ((1.0f - _lfoParameters.depth ) * buffer[pos+1]) + (_waveTable->_waveformTable[_lfoParameters.lfoWaveform][(int)(_lfoWavetablePosition * TABLESIZE)] * _lfoParameters.depth * buffer[pos+1]);
			_lfoWavetablePosition += lfoSkip;
			if( _lfoWavetablePosition > 1.0f )
			{
				_lfoWavetablePosition -= 1.0f;
			}
		}
		// Apply volume adjustment and then soft-clip as necessary.
		buffer[pos] = /*tanh*/(buffer[pos] * _masterVolume);
		buffer[pos + 1] = /*tanh*/(buffer[pos + 1] * _masterVolume);
		if( fabs(buffer[pos]) > maxLeft )
		{
			maxLeft = fabs(buffer[pos]);
		}
		if( fabs(buffer[pos + 1]) > maxRight )
		{
			maxRight = fabs(buffer[pos+1]);
		}
	}

	// TODO: Make this work, right now it just returns silence.
	//_delayEffect->ProcessSamples(buffer, frameCount * 2);

#ifndef linux
	if( _volumeMeterL != NULL && _volumeMeterR != NULL )
	{
		_volumeMeterL->SetFloatValue( maxLeft );
		_volumeMeterR->SetFloatValue( maxRight );
	}
#endif

	return 0;
}

float wxKeyboard::InterpolateHarmonic(int note, int harmonicNumber)
{
	if( _initialHarmonicLevels[harmonicNumber] == _finalHarmonicLevels[harmonicNumber] )
	{
		return _initialHarmonicLevels[harmonicNumber];
	}

	if( _releasing[note] )
	{
		return _finalHarmonicLevels[harmonicNumber];
	}

	float millisecond = ((float)(_playbackPosition[note]) * _millisecondsPerSample);
    // Handles the case of millisecond 0 when attack and decay are both 0.
	if( millisecond >= (_adsrParameters.attackTimeMsec + _adsrParameters.decayTimeMsec))
	{
		return _finalHarmonicLevels[harmonicNumber];
	}
	else
	{
		float percentOfTravel = millisecond / (_adsrParameters.attackTimeMsec + _adsrParameters.decayTimeMsec);
		return _initialHarmonicLevels[harmonicNumber] * (1.0 - percentOfTravel) + _finalHarmonicLevels[harmonicNumber] * percentOfTravel;
	}

	// This should never happen.
	return _initialHarmonicLevels[harmonicNumber];

}

/**
* Calculates the volume modification based on the envelope position, using the number
* of samples that have been played since the note has been triggered.
*/
float wxKeyboard::GetEnvelopeVolume(int note, int playbackSamplePosition, int offset)
{
    // Because we may finish playing audio for a note before an entire block is rendered, we need to
    // return zero volume when no sound should be playing.
    if( !_playing[note] && !_releasing[note] )
    {
        return 0.0f;
    }

	if( !_adsrParameters.enabled )
	{
		if( _releasing[note] == true )
		{
			_releasing[note] = false;
			_numSamplesReleased[note] = 0;
			return 0.0f;
		}
		return 1.0f;
	}
	if( (playbackSamplePosition+offset) == 0 )
	{
		return 0.0f;
	}

    float millisecond;
    if( _playing[note] )
    {
	    millisecond = ((float)(playbackSamplePosition + offset) * _millisecondsPerSample);
    }
    else // Releasing
    {
	    millisecond = ((float)(_numSamplesReleased[note] + offset) * _millisecondsPerSample);
    }

	if( _releasing[note] )
	{
		if( _adsrParameters.enabled && millisecond < _adsrParameters.releaseTimeMsec )
		{
			float ramp = (_adsrParameters.sustainLevelPercent * .01) * (((float)(_adsrParameters.releaseTimeMsec) - (float)(millisecond)) / (float)(_adsrParameters.releaseTimeMsec));
			return ramp;
		}
		else
		{
			_releasing[note] = false;
            _playing[note] = false;
			_numSamplesReleased[note] = 0;
			return 0.0f;
		}
	}

	// Attack.
	if( millisecond < _adsrParameters.attackTimeMsec )
	{
		float ramp = (float)millisecond / (float)(_adsrParameters.attackTimeMsec);
		return ramp;
	}
	// Decay
	else if( millisecond < (_adsrParameters.attackTimeMsec + _adsrParameters.decayTimeMsec) )
	{
		float ramp = (float)(millisecond - _adsrParameters.attackTimeMsec) / (float)(_adsrParameters.decayTimeMsec);
		// Value we're returning is the sustain level plus a fraction of the maximum based on
		// hor far we are through the decay.
		float value = ((float)(_adsrParameters.sustainLevelPercent) + ((100.0f - (float)(_adsrParameters.sustainLevelPercent)) * (1.0 - ramp))) * 0.01f;
		return value;
	}
	// Sustain.
	else
	{
		return (float)(_adsrParameters.sustainLevelPercent) * 0.01f;
	}
	// This should never happen.
	return 1.0f;
}

void wxKeyboard::OnHarmonicsChanged( float* initialHarmonics, float* finalHarmonics )
{
	memcpy(&_initialHarmonicLevels, initialHarmonics, (sizeof(float) * MAX_HARMONICS));
	memcpy(&_finalHarmonicLevels, finalHarmonics, (sizeof(float) * MAX_HARMONICS));
    float attack[MINIPANELWIDTH];
    memset(attack, 0, (sizeof(float) * MINIPANELWIDTH));
    float sustain[MINIPANELWIDTH];
    memset(sustain, 0, (sizeof(float) * MINIPANELWIDTH));
    float placeholder;
    for( int j = 0; j < MAX_HARMONICS; j++ )
    {
        placeholder = 0;
	    for(int i = 0; i < MINIPANELWIDTH; i++)
        {
            attack[i] = attack[i] + (_initialHarmonicLevels[j] * _waveTable->_waveformTable[WAVEFORM_SINE][(int)placeholder]);
            placeholder += (TABLESIZE * (j+1)) / MINIPANELWIDTH;
            while( placeholder > TABLESIZE )
                placeholder -= TABLESIZE;
        }
        placeholder = 0;
	    for(int i = 0; i < MINIPANELWIDTH; i++)
        {
            sustain[i] = sustain[i] + (_finalHarmonicLevels[j] * _waveTable->_waveformTable[WAVEFORM_SINE][(int)placeholder]);
            placeholder += (TABLESIZE * (j+1)) / MINIPANELWIDTH;
            while( placeholder > TABLESIZE )
                placeholder -= TABLESIZE;
        }
    }
    for( int k = 0; k < MINIPANELWIDTH; k++ )
    {
        if( attack[k] > 1.0 ) attack[k] = 1.0;
        if( sustain[k] > 1.0 ) sustain[k] = 1.0;
        if( sustain[k] < -1.0 ) sustain[k] = -1.0;
        if( sustain[k] < -1.0 ) sustain[k] = -1.0;
    }
    _initialWave->SetData(attack);
    _finalWave->SetData(sustain);
}

void wxKeyboard::SendMidiMessage(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, bool shortmsg)
{
	//WORD loWord = MAKEWORD( byte2, byte1 );
	//WORD hiWord = MAKEWORD( byte4, byte3 );
	//DWORD fullWord = MAKELONG( hiWord, loWord );
    //midiOutShortMsg(_midiOutDevice, fullWord );
    std::vector<unsigned char> msg;
    msg.push_back(byte4);
    msg.push_back(byte3);
    if(!shortmsg)
    {
      msg.push_back(byte2);
      if( byte1 != 0 )
      {
        msg.push_back(byte1);
      }
#ifndef VST
      _midiOutDevice->sendMessage(&msg);
#endif
    }
}

void wxKeyboard::OnPaintInitialWave(wxPaintEvent& event)
{
	wxColour brushColour = wxColour(224,224,0);
	wxPaintDC dc(this);
	dc.SetPen(wxPen(_textColour));
	dc.SetBrush(wxBrush(brushColour));
	dc.DrawRectangle(10, 20, 30, 40);
    _initialWave->OnPaint(event);
	event.Skip(true);
}

void wxKeyboard::OnPaintFinalWave(wxPaintEvent& event)
{
	wxColour brushColour = wxColour(224,224,0);
	wxPaintDC dc(this);
	dc.SetPen(wxPen(_textColour));
	dc.SetBrush(wxBrush(brushColour));
	dc.DrawRectangle(50, 60, 10, 20);
    _initialWave->OnPaint(event);
	event.Skip(true);
}

void wxKeyboard::OnNormalize( wxCommandEvent& event )
{
	_timbrePanel->Normalize();
	ResetFocus();
	event.Skip();
}

void wxKeyboard::OnInitial( wxCommandEvent& event )
{
	_timbrePanel->GetHarmonicPanel()->SetInitialVisible(true);
	_btnInitial->Enable(false);
	_btnFinal->Enable(true);
	ResetFocus();
	event.Skip();
}

void wxKeyboard::OnFinal( wxCommandEvent& event )
{
	_timbrePanel->GetHarmonicPanel()->SetInitialVisible(false);
	_btnInitial->Enable(true);
	_btnFinal->Enable(false);
	ResetFocus();
	event.Skip();
}

void wxKeyboard::OnClear( wxCommandEvent& event )
{
	float values[128];
	memset(values, 0, (sizeof(float) * 128));
	if( _timbrePanel->GetHarmonicPanel()->GetInitialVisible() )
	{
		_timbrePanel->GetHarmonicPanel()->SetInitialHarmonics(&values[0]);
	}
	else
	{
		_timbrePanel->GetHarmonicPanel()->SetFinalHarmonics(&values[0]);
	}
	Refresh();
	ResetFocus();
	event.Skip();
}

void wxKeyboard::OnGenerate( wxCommandEvent& event )
{
	float harmonicLevels[MAX_HARMONICS];
	memset(harmonicLevels, 0, (sizeof(float) * MAX_HARMONICS));
	switch( _chWaveform->GetSelection() )
	{
	case 0: // Square
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			if( (i == 0) || (i % 2) == 0 )
			{
				harmonicLevels[i] = 1.0f / (float)(i+1);
			}
		}
		break;
	case 1: // Saw
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			harmonicLevels[i] = 1.0f / (float)(i+1);
		}
		break;
	case 2: // Triangle
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			if( (i == 0) || (i % 4) == 0 )
			{
				harmonicLevels[i] = 1.0f / (float)((i + 1) * (i + 1));
			}
		}
		break;
	case 3: // White Noise
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			harmonicLevels[i] = 1.0f;
		}
		break;
	case 4: // Pink Noise
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			harmonicLevels[i] = AudioUtil::ScaleFrequency(1.0, 1-i);
		}
		break;
	case 5: // Brown Noise
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			harmonicLevels[i] = AudioUtil::ScaleFrequency(1.0, (2-i*2));
		}
		break;
	case 6: // Random Square
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			if( (i == 0) || (i % 2) == 0 )
			{
				harmonicLevels[i] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / (float)(i+1);
			}
		}
		break;
	case 7: // Random Saw
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			harmonicLevels[i] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / (float)(i+1);
		}
		break;
	case 8: // Random Triangle
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			if( (i == 0) || (i % 4) == 0 )
			{
				harmonicLevels[i] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / (float)((i + 1) * (i + 1));
			}
		}
		break;
	default:
		return;
	}
	if( _timbrePanel->GetHarmonicPanel()->GetInitialVisible() )
	{
		_timbrePanel->GetHarmonicPanel()->SetInitialHarmonics(&harmonicLevels[0]);
	}
	else
	{
		_timbrePanel->GetHarmonicPanel()->SetFinalHarmonics(&harmonicLevels[0]);
	}
	_timbrePanel->Normalize();
	ResetFocus();
	event.Skip();
}

void wxKeyboard::OnZoom( wxScrollEvent& event )
{
	int level = _slZoomLevel->GetValue();
	_timbrePanel->GetHarmonicPanel()->SetZoomLevel(level);
	//float peak = 1.0f / (float)level;
	wxString txt = wxString::Format(_("%dx"), level);
	_txtZoomLevel->SetLabel(txt);
	event.Skip();
}

/**
* Left arrow -- copies the final harmonic levels into the initial harmonic levels.
*/
void wxKeyboard::OnButtonLeft( wxCommandEvent& event )
{
	_timbrePanel->GetHarmonicPanel()->SetInitialHarmonics(&_finalHarmonicLevels[0]);
	ResetFocus();
	event.Skip();
}

/**
* Right arrow -- copies the initial harmonic levels into the final harmonic levels.
*/
void wxKeyboard::OnButtonRight( wxCommandEvent& event )
{
	_timbrePanel->GetHarmonicPanel()->SetFinalHarmonics(&_initialHarmonicLevels[0]);
	ResetFocus();
	event.Skip();
}

void wxKeyboard::ResetFocus()
{
	_octave[0]->SetFocus();
}

void wxKeyboard::OnSliderRelease(wxMouseEvent& event)
{
	ResetFocus();
	event.Skip();
}

void wxKeyboard::OnFrequency( wxScrollEvent& event )
{
	_lfoParameters.lfoFrequency = (float)_slFrequency->GetValue() / 10.0f;
	_txtFrequency->SetLabel( wxString::Format(_("%3.1f"), _lfoParameters.lfoFrequency));
	event.Skip();
}

void wxKeyboard::OnLfoWaveform( wxCommandEvent& event )
{
	_lfoParameters.lfoWaveform = _chLFOWaveform->GetSelection();
	event.Skip();
}

void wxKeyboard::OnAttack( wxScrollEvent& event )
{
	_adsrParameters.attackTimeMsec = _slAttack->GetValue();
	_txtAttack->SetLabel(wxString::Format(_("%3d"), _adsrParameters.attackTimeMsec ));
	event.Skip();
}

void wxKeyboard::OnDecay( wxScrollEvent& event )
{
	_adsrParameters.decayTimeMsec = _slDecay->GetValue();
	_txtDecay->SetLabel(wxString::Format(_("%3d"), _adsrParameters.decayTimeMsec ));
	event.Skip();
}

void wxKeyboard::OnSustain( wxScrollEvent& event )
{
	_adsrParameters.sustainLevelPercent = _slSustain->GetValue();
	_txtSustain->SetLabel(wxString::Format(_("%3d"), _adsrParameters.sustainLevelPercent ));
	event.Skip();
}

void wxKeyboard::OnRelease( wxScrollEvent& event )
{
	_adsrParameters.releaseTimeMsec = _slRelease->GetValue();
	_txtRelease->SetLabel(wxString::Format(_("%3d"), _adsrParameters.releaseTimeMsec ));
	event.Skip();
}

#ifndef VST
/**
* Handles displaying the 'select sound device' dialog.  Not available in VST mode.
*/
void wxKeyboard::OnSettings( wxCommandEvent& event )
{
    wxAudioSettings* dlg = new wxAudioSettings(this, this);
    if( dlg != NULL )
    {
        dlg->SetForegroundColour(_textColour);
        dlg->SetBackgroundColour(_backgroundColour);
        dlg->ShowInputDevice(false);
        dlg->ShowModal();
    }
    ResetFocus();
    event.Skip();
}

void wxKeyboard::SelectAudioInputDevice(PaStreamParameters* device)
{
    // Ignored.  No input.
}

/**
* Handles changing the audio output device.  Not available in VST mode.
*/
void wxKeyboard::SelectAudioOutputDevice(PaStreamParameters* device)
{
    int value;
    _done = true;
    //Pa_StopStream(_buffer);
    wxThread::Sleep(20);
    value = Pa_AbortStream(_buffer);
    value = Pa_CloseStream(_buffer);
    _buffer = NULL;
    //Pa_Initialize();
    value = Pa_OpenStream(&_buffer, NULL, device, _sampleRate, BUFFERLENGTH, paNoFlag, AudioCallback, this );
    if( value != 0 )
    {
        wxMessageBox(wxString::FromAscii(Pa_GetErrorText(value)));
        return;
    }
    value = Pa_StartStream( _buffer );
    _done = false;
}
#else
void wxKeyboard::idle()
{
    ProcessPendingEvents();
}

bool wxKeyboard::getEffectName (char* name)
{
    vst_strncpy (name, "SigmaTizm", kVstMaxEffectNameLen);
    return true;
}

bool wxKeyboard::getVendorString (char* text)
{
    vst_strncpy (text, "Zeta Centauri, Inc.", kVstMaxVendorStrLen);
    return true;
}

bool wxKeyboard::getProductString (char* text)
{
	vst_strncpy (text, "SigmaTizm", kVstMaxProductStrLen);
	return true;
}

VstInt32 wxKeyboard::getVendorVersion ()
{
    return 1000;
}

VstInt32 wxKeyboard::getNumMidiInputChannels ()
{
    return 1;
}

VstInt32 wxKeyboard::getNumMidiOutputChannels ()
{
    return 1;
}

/** 
* Process VST MIDI Data
*/
VstInt32 wxKeyboard::processEvents (VstEvents* ev)
{
    // TODO: Translate this for our MIDI callback.
	for (VstInt32 i = 0; i < ev->numEvents; i++)
	{
		if ((ev->events[i])->type != kVstMidiType)
			continue;

		//VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
		//char* midiData = event->midiData;
		//VstInt32 status = midiData[0] & 0xf0;	// ignoring channel
		//if (status == 0x90 || status == 0x80)	// we only look at notes
		//{
		//	VstInt32 note = midiData[1] & 0x7f;
		//	VstInt32 velocity = midiData[2] & 0x7f;
		//	if (status == 0x80)
		//		velocity = 0;	// note off by velocity 0
		//	if (!velocity && (note == currentNote))
		//		noteOff ();
		//	else
		//		noteOn (note, velocity, event->deltaFrames);
		//}
		//else if (status == 0xb0)
		//{
		//	if (midiData[1] == 0x7e || midiData[1] == 0x7b)	// all notes off
		//		noteOff ();
		//}
		//event++;
	}
	return 1;
}

void wxKeyboard::setSampleRate (float sampleRate)
{
    // TODO: Deal with this.
	AudioEffectX::setSampleRate (sampleRate);
    _sampleRate = sampleRate;
	//fScaler = (float)((double)kWaveSize / (double)sampleRate);
}

long wxKeyboard::open(void *ptr)
{
    //// I would like mainPanel to already exist and be running. Then just show it here instead for recreate it every time we call ETriggerEditor::open();

    //// create the class without physical windows first
    //mainPanel = new MainPanel( this );

    //mainPanel->Create( parent, 10005, wxPoint(0,0), wxSize(800,600), 0 );
   
    //mainPanel->Show( true );

    //// set focus to our panel when displayed for the first time
    //mainPanel->SetFocus();

    return true;
}

VstIntPtr wxKeyboard::dispatcher (VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
	VstIntPtr v = 0;
	
	switch (opcode)
	{
		case effClose:
            close();
            break;
        default:
            return AudioEffect::dispatcher(opcode, index, value, ptr, opt);
    }
    return v;
}

void wxKeyboard::close()
{
    //I would like to just Hide the MainPanel here, it needs to still run for the VST to work.

    wxMessageBox(_("VST CLOSE"));
} 
#endif
