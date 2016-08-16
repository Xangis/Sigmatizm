#include "wxAudioSettings.h"
#include "wx/stdpaths.h"
#include "portaudio.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

IMPLEMENT_DYNAMIC_CLASS( wxAudioSettings, wxDialog )

BEGIN_EVENT_TABLE( wxAudioSettings, wxDialog )
    EVT_CLOSE( wxAudioSettings::OnClose )
    EVT_BUTTON( ID_BUTTON_CLOSE, wxAudioSettings::OnCloseButton )
	EVT_CHOICE( ID_MIDI_OUT_DEVICE, wxAudioSettings::OnChangeMidiOutDevice )
    //EVT_CHOICE( ID_MIDI_IN_DEVICE, wxAudioSettings::OnChangeMidiInDevice )
END_EVENT_TABLE()

wxAudioSettings::wxAudioSettings( )
{
}

wxAudioSettings::~wxAudioSettings()
{
}

wxAudioSettings::wxAudioSettings( wxWindow* parent, AudioSettingsInterface* callback, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, callback, id, caption, pos, size, style);
}

bool wxAudioSettings::Create( wxWindow* parent, AudioSettingsInterface* callback, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    // Set member variables here
	_callback = callback;
    _btnClose = NULL;
	//_inDevice = NULL;
	_outDevice = NULL;
	_txtMidiOutputDevice = NULL;
	//_txtMidiInputDevice = NULL;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
	//_mainDlg = (WhateverDlg*)parent;
    return true;
}

void wxAudioSettings::CreateControls()
{
    wxAudioSettings* itemDialog1 = this;

	wxImage spinImage;
	wxStandardPaths paths;
#ifdef WIN32
    wxString spinPath = wxString::Format(_("%s\\spin.bmp"), paths.GetDataDir().c_str());
#else
    wxString spinPath = _("spin.bmp");
#endif
    spinImage.LoadFile( spinPath, wxBITMAP_TYPE_BMP );

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemBoxSizer3 = new wxFlexGridSizer(5, 2, 0, 0);
    itemBoxSizer2->Add(itemBoxSizer3, 0, 0, 0 );

    _txtMidiOutputDevice = new wxStaticText( itemDialog1, wxID_STATIC, _("Audio Out Device:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(_txtMidiOutputDevice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    int numDevices;

    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        wxMessageBox( wxString::Format(_("ERROR: Pa_CountDevices returned 0x%x.\n"), numDevices ) );
    }

    const PaDeviceInfo *deviceInfo;
    wxArrayString empty;
    _outDevice = new wxKeylessChoice( itemDialog1, ID_MIDI_OUT_DEVICE, wxDefaultPosition, wxDefaultSize, empty );
    itemBoxSizer3->Add(_outDevice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	if( numDevices == 0 )
	{
		_outDevice->Enable(false);
	}

    // Get the list of available APIs.  We can use this to select devices by category.
    //PaHostApiIndex numApis = Pa_GetHostApiCount();
    //const PaHostApiInfo* apiInfo;
    //for( int j = 0; j < numApis; j++ )
    //{
    //    apiInfo = Pa_GetHostApiInfo(j);
    //}

    for( int i = 0; i < numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
        //int 	structVersion
        //const char * 	name
        //PaHostApiIndex 	hostApi
        //int 	maxInputChannels
        //int 	maxOutputChannels
        //PaTime 	defaultLowInputLatency
        //PaTime 	defaultLowOutputLatency
        //PaTime 	defaultHighInputLatency
        //PaTime 	defaultHighOutputLatency
        //double 	defaultSampleRate

        PaStreamParameters* outputParameters = new PaStreamParameters;
        memset( outputParameters, 0, sizeof( outputParameters ) ); //not necessary if you are filling in all the fields
        outputParameters->channelCount = 2;
        outputParameters->device = i;
        outputParameters->hostApiSpecificStreamInfo = NULL;
        outputParameters->sampleFormat = paFloat32;
        outputParameters->suggestedLatency = deviceInfo->defaultLowOutputLatency;
        outputParameters->hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

        int err = Pa_IsFormatSupported( NULL, outputParameters, 44100 );
        if( err == paFormatIsSupported )
        {
            int msec = deviceInfo->defaultLowOutputLatency * 1000.0;
		    // Add the device to a list box on the GUI.
            if( msec )
            {
                _outDevice->Insert(wxString::Format(_("%s (%d ms) [%s]"), wxString::FromAscii(deviceInfo->name).c_str(),
                    msec, wxString::FromAscii(Pa_GetHostApiInfo(deviceInfo->hostApi)->name).c_str()), 0, (void *)outputParameters );
            }
            else
            {
                _outDevice->Insert(wxString::Format(_("%s [%s]"), wxString::FromAscii(deviceInfo->name).c_str(),
                    wxString::FromAscii(Pa_GetHostApiInfo(deviceInfo->hostApi)->name).c_str()), 0, (void *)outputParameters );
            }
        }
	}

    //_txtMidiInputDevice = new wxStaticText( itemDialog1, wxID_STATIC, _("Audio In Device:"), wxDefaultPosition, wxDefaultSize, 0 );
    //itemBoxSizer3->Add(_txtMidiInputDevice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	//// Look for MIDI input devices before creating choice box.
	//numDevices = midiInGetNumDevs();

	//MIDIINCAPS midiInCaps;
 //   HRESULT hr
	//wxArrayString inDeviceList;
	//for( count = 0; count < numDevices; count++ )
	//{
	//	hr = midiInGetDevCaps( count, &midiInCaps, sizeof( midiInCaps ) );
	//	if( hr == MMSYSERR_BADDEVICEID || hr == MMSYSERR_INVALPARAM || hr == MMSYSERR_NODRIVER || hr == MMSYSERR_NOMEM )
	//	{
	//		continue;
	//	}
	//	// Add the device to a list box on the GUI.
	//	inDeviceList.Add( midiInCaps.szPname );
	//}
 //   _inDevice = new wxKeylessChoice( itemDialog1, ID_MIDI_IN_DEVICE, wxDefaultPosition, wxDefaultSize, inDeviceList );
	//if( numDevices > 0 )
	//{
	//	_inDevice->SetSelection(0);
	//}
	//else
	//{
	//	_inDevice->Enable(false);
	//}
 //   itemBoxSizer3->Add(_inDevice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    _btnClose = new wxButton( itemDialog1, ID_BUTTON_CLOSE, _("Close"));
    itemBoxSizer2->Add(_btnClose, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
}

bool wxAudioSettings::SetForegroundColour(const wxColour &colour)
{
	_textColour = colour;
	_outDevice->SetForegroundColour(_textColour);
	//_inDevice->SetForegroundColour(_textColour);
    _txtMidiOutputDevice->SetForegroundColour(_textColour);
	//_txtMidiInputDevice->SetForegroundColour(_textColour);
	_btnClose->SetForegroundColour(_textColour);
	wxDialog::SetForegroundColour(colour);
	Refresh();
	return true;
}

bool wxAudioSettings::SetBackgroundColour(const wxColour &colour)
{
	_backgroundColour = colour;
	_outDevice->SetBackgroundColour(_backgroundColour);
	//_inDevice->SetBackgroundColour(_backgroundColour);
	_btnClose->SetBackgroundColour(_backgroundColour);
	wxDialog::SetBackgroundColour(colour);
	Refresh();
	return true;
}

void wxAudioSettings::OnClose( wxCloseEvent& event )
{
    // TODO: On close, populate values back into MIDI settings data.
	EndModal(0);
    //Destroy();
    event.Skip();
}

void wxAudioSettings::OnCloseButton( wxCommandEvent& event )
{
    // TODO: On close, populate values back into MIDI settings data.
    EndModal(0);
    event.Skip();
}

void wxAudioSettings::OnChangeMidiOutDevice( wxCommandEvent& )
{
	// Subtract 1 because the first device is -1 (MIDI Mapper).
	int selection = _outDevice->GetCurrentSelection( );
	_callback->SelectAudioOutputDevice((PaStreamParameters*)_outDevice->GetClientData(selection));
}

//void wxAudioSettings::OnChangeMidiInDevice( wxCommandEvent& )
//{
//	// There is no MIDI Mapper for input.
//	int selection = _inDevice->GetCurrentSelection( );
//	_callback->SelectMidiInputDevice(selection);
//}

///**
//* Sets the selected input device index.
//*/
//void wxAudioSettings::SetMidiInputDeviceIndex(unsigned int index)
//{
//	if( _inDevice->GetCount() > index )
//		_inDevice->SetSelection(index);
//}

/**
* Sets the selected output device index.
*/
void wxAudioSettings::SetMidiOutputDeviceIndex(unsigned int index)
{
	if( _outDevice->GetCount() > index )
		_outDevice->SetSelection(index);
}
