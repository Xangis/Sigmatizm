#include "SynthParametersDlg.h"
#include "wx/stdpaths.h"

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "sampledatadlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "spin.xpm"

IMPLEMENT_DYNAMIC_CLASS( SynthParametersDlg, wxPanel )

BEGIN_EVENT_TABLE( SynthParametersDlg, wxPanel )
    EVT_SPIN_DOWN( ID_TARGETPOLYPHONYSPIN, SynthParametersDlg::OnPolyphonySpinDown )
    EVT_SPIN_UP( ID_TARGETPOLYPHONYSPIN, SynthParametersDlg::OnPolyphonySpinUp )
	EVT_KEY_DOWN(SynthParametersDlg::OnKeyDown)
	EVT_KEY_UP(SynthParametersDlg::OnKeyUp)
END_EVENT_TABLE()

SynthParametersDlg::SynthParametersDlg( )
{
}

SynthParametersDlg::~SynthParametersDlg()
{
}

SynthParametersDlg::SynthParametersDlg( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, pos, size, style|wxWANTS_CHARS);
}

bool SynthParametersDlg::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    // Set member variables here
    _textColour.Set( 255, 204, 102 );
    _backgroundColour.Set( 102, 51, 0 );
	_peakLevelText = NULL;
	_polyphonyText = NULL;
	_harmonicPanel = NULL;
	_targetPolyphony = NULL;
	_targetPolyphonySpin = NULL;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    return true;
}

void SynthParametersDlg::SetHarmonicsCallback(HarmonicsCallback* callback)
{
	_harmonicPanel->SetHarmonicsCallback(callback);
}

void SynthParametersDlg::CreateControls()
{    
    SynthParametersDlg* itemDialog1 = this;

    this->SetBackgroundColour(_backgroundColour);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

	// These have to be created here because they're needed by the harmonics panel.
    _peakLevelText = new wxStaticText( itemDialog1, wxID_STATIC, _("Peak Level:      "), wxDefaultPosition, wxSize(-1,-1) );
    _peakLevelText->SetForegroundColour(_textColour);
	_polyphonyText = new wxStaticText( itemDialog1, wxID_STATIC, _("Safe Polyphony:      "), wxDefaultPosition, wxSize(-1,-1) );
    _polyphonyText->SetForegroundColour(_textColour);

	wxBoxSizer* sizer8 = new wxBoxSizer(wxHORIZONTAL);
	_harmonicPanel = new wxHarmonicsPanel( _polyphonyText, _peakLevelText, itemDialog1, ID_HARMONIC_PANEL, wxDefaultPosition, wxSize(640, 300));
	_harmonicPanel->SetBackgroundColour(wxColour( 0x22, 0x11, 0 ));
	_harmonicPanel->SetForegroundColour(wxColour( 0xCC, 0x66, 0 ));
	sizer8->Add(_harmonicPanel, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	itemBoxSizer2->Add(sizer8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer5->Add(_peakLevelText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    itemBoxSizer5->Add(_polyphonyText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* static3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Target Polyphony:"), wxDefaultPosition, wxDefaultSize );
    static3->SetForegroundColour(_textColour);
	itemBoxSizer5->Add(static3, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	_targetPolyphony = new wxStaticText( itemDialog1, ID_TARGETPOLYPHONYTEXT, _("2"), wxDefaultPosition, wxSize( 20, -1 ), 0 );
	_targetPolyphony->SetForegroundColour(_textColour);
	_targetPolyphony->SetBackgroundColour(_backgroundColour);
    itemBoxSizer5->Add(_targetPolyphony, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxImage spinImage(spin_xpm);
    wxInitAllImageHandlers();
    _targetPolyphonySpin = new wxBitmapSpinButton( itemDialog1, ID_TARGETPOLYPHONYSPIN, wxDefaultPosition, wxSize( 16, 22 ), wxSP_ARROW_KEYS );
    _targetPolyphonySpin->SetBitmap( &spinImage );
    itemBoxSizer5->Add(_targetPolyphonySpin, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 3);

	itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);
}

void SynthParametersDlg::SetHarmonicLevels( float* initialLevels, float* finalLevels )
{
	_harmonicPanel->SetHarmonicLevels(initialLevels, finalLevels);
}

/**
* Gets harmonic levels that have been set in the dialog.  Memory must be deleted by the caller.
*/
float* SynthParametersDlg::GetHarmonicLevels()
{
	return( _harmonicPanel->GetHarmonicLevels() );
}

void SynthParametersDlg::Normalize()
{
	int notes = atoi(_targetPolyphony->GetLabel().ToAscii());
	_harmonicPanel->Normalize(notes);
}

void SynthParametersDlg::OnPolyphonySpinUp( wxSpinEvent& event )
{
    int value = atoi(_targetPolyphony->GetLabel().ToAscii());
	++value;
	_targetPolyphony->SetLabel( wxString::Format( _("%d"), value ) );
    event.Skip();
}

void SynthParametersDlg::OnPolyphonySpinDown( wxSpinEvent& event )
{
    int value = atoi(_targetPolyphony->GetLabel().ToAscii());
    if( value > 0 )
    {
        --value;
        _targetPolyphony->SetLabel( wxString::Format( _("%d"), value ) );
    }
    event.Skip();
}

void SynthParametersDlg::OnKeyDown(wxKeyEvent& event)
{
	GetParent()->GetEventHandler()->ProcessEvent(event);
	//event.Skip(false);
}

void SynthParametersDlg::OnKeyUp(wxKeyEvent& event)
{
	GetParent()->GetEventHandler()->ProcessEvent(event);
	//event.Skip(false);
}
