#include "wxMidiSettingsDlg.h"
#include "wx/stdpaths.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Image resources
#include "greenbtn.xpm"
#include "darkbtn.xpm"

IMPLEMENT_DYNAMIC_CLASS( wxMidiSettingsDlg, wxDialog )

BEGIN_EVENT_TABLE( wxMidiSettingsDlg, wxDialog )
    EVT_CLOSE( wxMidiSettingsDlg::OnClose )
    EVT_BUTTON( ID_BUTTON_CLOSE, wxMidiSettingsDlg::OnCloseButton )
    EVT_SPIN_UP( ID_INPUTCHANNELSPIN, wxMidiSettingsDlg::OnInputChannelSpinUp )
    EVT_SPIN_DOWN( ID_INPUTCHANNELSPIN, wxMidiSettingsDlg::OnInputChannelSpinDown )
    EVT_SPIN_UP( ID_OUTPUTCHANNELSPIN, wxMidiSettingsDlg::OnOutputChannelSpinUp )
    EVT_SPIN_DOWN( ID_OUTPUTCHANNELSPIN, wxMidiSettingsDlg::OnOutputChannelSpinDown )
	EVT_CHOICE( ID_MIDI_OUT_DEVICE, wxMidiSettingsDlg::OnChangeMidiOutDevice )
    EVT_CHOICE( ID_MIDI_IN_DEVICE, wxMidiSettingsDlg::OnChangeMidiInDevice )
	EVT_CHECKBOX( ID_CHK_MIDIOUT, wxMidiSettingsDlg::OnMidiOutputEnabledCheck )
END_EVENT_TABLE()

wxMidiSettingsDlg::wxMidiSettingsDlg( )
{
}

wxMidiSettingsDlg::~wxMidiSettingsDlg()
{
}

wxMidiSettingsDlg::wxMidiSettingsDlg( wxWindow* parent, MidiSettingsInterface* callback, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, callback, id, caption, pos, size, style);
}

bool wxMidiSettingsDlg::Create( wxWindow* parent, MidiSettingsInterface* callback, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    // Set member variables here
	_callback = callback;
    _btnClose = NULL;
	_chkMidiOutEnabled = NULL;
	_inDevice = NULL;
	_outDevice = NULL;
	_txtMidiOutputEnabled = NULL;
	_txtMidiOutputDevice = NULL;
	_txtMidiOutputChannel = NULL;
	_txtMidiInputChannel = NULL;
	_txtMidiInputDevice = NULL;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    return true;
}

void wxMidiSettingsDlg::CreateControls()
{
    wxMidiSettingsDlg* itemDialog1 = this;

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

    _txtMidiOutputDevice = new wxStaticText( itemDialog1, wxID_STATIC, _("MIDI Out Device:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(_txtMidiOutputDevice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	// Look for MIDI output devices before creating choice box.
    int numDevices = _callback->_midiOutDevice->getPortCount();
	int count;
	wxArrayString outDeviceList;
	for( count = 0; count < numDevices; count++ )
	{
        outDeviceList.Add( wxString::FromAscii(_callback->_midiOutDevice->getPortName(count).c_str()) );
	}
    _outDevice = new wxKeylessChoice( itemDialog1, ID_MIDI_OUT_DEVICE, wxDefaultPosition, wxDefaultSize, outDeviceList );
	_outDevice->SetSelection(0);
    itemBoxSizer3->Add(_outDevice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	if( numDevices == 0 )
	{
		_outDevice->Enable(false);
	}

    _txtMidiInputDevice = new wxStaticText( itemDialog1, wxID_STATIC, _("MIDI In Device:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(_txtMidiInputDevice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	// Look for MIDI input devices before creating choice box.
    numDevices = _callback->_midiInDevice->getPortCount();
	wxArrayString inDeviceList;
	for( count = 0; count < numDevices; count++ )
	{
        inDeviceList.Add( wxString::FromAscii(_callback->_midiInDevice->getPortName(count).c_str()) );
	}
    _inDevice = new wxKeylessChoice( itemDialog1, ID_MIDI_IN_DEVICE, wxDefaultPosition, wxDefaultSize, inDeviceList );
	if( numDevices > 0 )
	{
		_inDevice->SetSelection(0);
	}
	else
	{
		_inDevice->Enable(false);
	}
    itemBoxSizer3->Add(_inDevice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    _txtMidiInputChannel = new wxStaticText( itemDialog1, wxID_STATIC, _("MIDI Input Channel:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(_txtMidiInputChannel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    _inputChannelText = new wxStaticText( itemDialog1, ID_INPUTCHANNELTEXT, _T("1"), wxDefaultPosition, wxSize( 22, -1 ), 0 );
    itemBoxSizer5->Add(_inputChannelText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _inputChannelSpin = new wxBitmapSpinButton( itemDialog1, ID_INPUTCHANNELSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS );
    _inputChannelSpin->SetBitmap( &spinImage );
    itemBoxSizer5->Add(_inputChannelSpin, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

    _txtMidiOutputChannel = new wxStaticText( itemDialog1, wxID_STATIC, _("MIDI Output Channel:"), wxDefaultPosition, wxDefaultSize );
    itemBoxSizer3->Add(_txtMidiOutputChannel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    _outputChannelText = new wxStaticText( itemDialog1, ID_OUTPUTCHANNELTEXT, _T("1"), wxDefaultPosition, wxSize( 22, -1 ), 0 );
    itemBoxSizer7->Add(_outputChannelText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _outputChannelSpin = new wxBitmapSpinButton( itemDialog1, ID_OUTPUTCHANNELSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS );
    _outputChannelSpin->SetBitmap( &spinImage );
    itemBoxSizer7->Add(_outputChannelSpin, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	itemBoxSizer3->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

	_txtMidiOutputEnabled = new wxStaticText( itemDialog1, wxID_STATIC, _("MIDI Output Enabled:"), wxDefaultPosition, wxDefaultSize );
    itemBoxSizer3->Add(_txtMidiOutputEnabled, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	wxBitmap* bitmap1 = new wxBitmap(greenbtn_xpm, wxBITMAP_TYPE_XPM );
	wxBitmap* bitmap2 = new wxBitmap(darkbtn_xpm, wxBITMAP_TYPE_XPM );
	_chkMidiOutEnabled = new wxSwitch( itemDialog1, ID_CHK_MIDIOUT, _T(""), wxDefaultPosition, wxSize(22,22) );
	if( bitmap1->Ok() && bitmap2->Ok() )
	{
		_chkMidiOutEnabled->SetBitmaps(bitmap1, bitmap2);
	}
	itemBoxSizer3->Add(_chkMidiOutEnabled, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    _btnClose = new wxButton( itemDialog1, ID_BUTTON_CLOSE, _("Close"));
    itemBoxSizer2->Add(_btnClose, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
}

void wxMidiSettingsDlg::OnMidiOutputEnabledCheck(wxCommandEvent&)
{
	_callback->EnableMidiOutput(_chkMidiOutEnabled->GetValue());
}

bool wxMidiSettingsDlg::SetForegroundColour(const wxColour &colour)
{
	_textColour = colour;
	_outDevice->SetForegroundColour(_textColour);
	_inDevice->SetForegroundColour(_textColour);
    _inputChannelText->SetForegroundColour(_textColour);
    _outputChannelText->SetForegroundColour(_textColour);
    _chkMidiOutEnabled->SetForegroundColour(_textColour);
    _txtMidiOutputEnabled->SetForegroundColour(_textColour);
    _txtMidiOutputDevice->SetForegroundColour(_textColour);
    _txtMidiOutputChannel->SetForegroundColour(_textColour);
    _txtMidiInputChannel->SetForegroundColour(_textColour);
	_txtMidiInputDevice->SetForegroundColour(_textColour);
	_btnClose->SetForegroundColour(_textColour);
	wxDialog::SetForegroundColour(colour);
	Refresh();
	return true;
}

bool wxMidiSettingsDlg::SetBackgroundColour(const wxColour &colour)
{
	_backgroundColour = colour;
	_outDevice->SetBackgroundColour(_backgroundColour);
	_inDevice->SetBackgroundColour(_backgroundColour);
	_btnClose->SetBackgroundColour(_backgroundColour);
	wxDialog::SetBackgroundColour(colour);
	Refresh();
	return true;
}

void wxMidiSettingsDlg::SetMidiOutputCheckbox(bool enabled)
{
	_chkMidiOutEnabled->SetValue(enabled);
}

void wxMidiSettingsDlg::OnClose( wxCloseEvent& event )
{
    // TODO: On close, populate values back into MIDI settings data.
	EndModal(0);
    //Destroy();
    event.Skip();
}

void wxMidiSettingsDlg::OnCloseButton( wxCommandEvent& event )
{
    // TODO: On close, populate values back into MIDI settings data.
    EndModal(0);
    event.Skip();
}

void wxMidiSettingsDlg::OnInputChannelSpinUp( wxSpinEvent& event )
{
    // Set MIDI Channel
    int value = atoi(_inputChannelText->GetLabel().ToAscii());
	++value;
	if( value > 16 )
	{
		value = 1;
	}
	_inputChannelText->SetLabel( wxString::Format( _("%d"), value ) );
	_callback->SelectMidiInputChannel(value);
    event.Skip();
}

void wxMidiSettingsDlg::OnInputChannelSpinDown( wxSpinEvent& event )
{
    // Set MIDI Channel
    int value = atoi(_inputChannelText->GetLabel().ToAscii());
	--value;
	if( value < 1 )
	{
		value = 16;
	}
	_inputChannelText->SetLabel( wxString::Format( _("%d"), value ) );
    _callback->SelectMidiInputChannel(value);
    event.Skip();
}

void wxMidiSettingsDlg::OnOutputChannelSpinDown( wxSpinEvent& event )
{
    // Set MIDI Channel
    int value = atoi(_outputChannelText->GetLabel().ToAscii());
	--value;
	if( value < 1 )
	{
		value = 16;
	}
	_outputChannelText->SetLabel( wxString::Format( _("%d"), value ) );
    _callback->SelectMidiOutputChannel(value);
    event.Skip();
}

void wxMidiSettingsDlg::OnOutputChannelSpinUp( wxSpinEvent& event )
{
    // Set MIDI Channel
    int value = atoi(_outputChannelText->GetLabel().ToAscii());
	++value;
	if( value > 16 )
	{
		value = 1;
	}
	_outputChannelText->SetLabel( wxString::Format( _("%d"), value ) );
    _callback->SelectMidiOutputChannel(value);
    event.Skip();
}

void wxMidiSettingsDlg::OnChangeMidiOutDevice( wxCommandEvent& )
{
	int selection = _outDevice->GetCurrentSelection( );
	_callback->SelectMidiOutputDevice(selection);
}

void wxMidiSettingsDlg::OnChangeMidiInDevice( wxCommandEvent& )
{
	int selection = _inDevice->GetCurrentSelection( );
	_callback->SelectMidiInputDevice(selection);
}

/**
* Sets the selected input device index.
*/
void wxMidiSettingsDlg::SetMidiInputDeviceIndex(unsigned int index)
{
	if( _inDevice->GetCount() > index )
		_inDevice->SetSelection(index);
}

/**
* Sets the selected output device index.
*/
void wxMidiSettingsDlg::SetMidiOutputDeviceIndex(unsigned int index)
{
	if( _outDevice->GetCount() > index )
		_outDevice->SetSelection(index);
}

/**
* Sets the MIDI input channel spinner text.
*/
void wxMidiSettingsDlg::SetMidiInputChannel(int channel)
{
	if( channel > 0 && channel < 17 )
		_inputChannelText->SetLabel(wxString::Format(_("%d"), channel ));
}

/**
* Sets the MIDI output channel spinner text.
*/
void wxMidiSettingsDlg::SetMidiOutputChannel(int channel)
{
	if( channel > 0 && channel < 17 )
		_outputChannelText->SetLabel(wxString::Format(_("%d"), channel ));
}
