#include "SettingsDlg.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

IMPLEMENT_DYNAMIC_CLASS( SettingsDlg, wxDialog )

BEGIN_EVENT_TABLE( SettingsDlg, wxDialog )
    EVT_CLOSE( SettingsDlg::OnClose )
    EVT_BUTTON( ID_BTN_OK, SettingsDlg::OnOK )
END_EVENT_TABLE()

SettingsDlg::SettingsDlg( )
{
}

SettingsDlg::~SettingsDlg()
{
}

SettingsDlg::SettingsDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

bool SettingsDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    // Set member variables here
    _textColour.Set( 255, 204, 153 );
    _backgroundColour.Set( 102, 51, 0 );
    _spnNumOctaves = NULL;
    _btnClose = NULL;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    return true;
}

void SettingsDlg::CreateControls()
{    
    SettingsDlg* itemDialog1 = this;

    this->SetBackgroundColour(_backgroundColour);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemBoxSizer3 = new wxFlexGridSizer(4, 2, 0, 0);
    itemBoxSizer2->Add(itemBoxSizer3, 0, 0, 0 );

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Number of Octaves:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetForegroundColour(_textColour);
    itemBoxSizer3->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _spnNumOctaves = new wxSpinCtrl( itemDialog1, ID_SPN_NUMOCTAVES, _T("7"), wxDefaultPosition, wxSize(54, -1), wxSP_ARROW_KEYS, 3, 7, 64 );
    itemBoxSizer3->Add(_spnNumOctaves, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    _btnClose = new wxButton( itemDialog1, ID_BTN_OK, _T("OK" ));
    itemBoxSizer7->Add(_btnClose, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

}

int SettingsDlg::GetNumOctaves()
{
	return _spnNumOctaves->GetValue();
}

void SettingsDlg::SetNumOctaves(int octaves)
{
	_spnNumOctaves->SetValue(octaves);
}

void SettingsDlg::OnOK( wxCommandEvent& event )
{
	EndModal(0);
    event.Skip();
}

void SettingsDlg::OnClose( wxCloseEvent& event )
{
    EndModal(0);
    event.Skip();
}
