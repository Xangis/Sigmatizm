#pragma once

#include "wx/wx.h"
#include "wx/spinctrl.h"

#define ID_SETTINGS_DLG 19000
#define SYMBOL_SETTINGSDLG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SETTINGSDLG_TITLE _("SigmaTizm Settings")
#define SYMBOL_SETTINGSDLG_IDNAME ID_SETTINGS_DLG
#define SYMBOL_SETTINGSDLG_SIZE wxSize(400, 440)
#define SYMBOL_SETTINGSDLG_POSITION wxDefaultPosition
#define ID_SPN_NUMOCTAVES 19001
#define ID_BTN_OK 19002

class SettingsDlg : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( SettingsDlg )
    DECLARE_EVENT_TABLE()
public:
    SettingsDlg();
    ~SettingsDlg();
    SettingsDlg( wxWindow* parent, wxWindowID id = SYMBOL_SETTINGSDLG_IDNAME, const wxString& caption = SYMBOL_SETTINGSDLG_TITLE, const wxPoint& pos = SYMBOL_SETTINGSDLG_POSITION, const wxSize& size = SYMBOL_SETTINGSDLG_SIZE, long style = SYMBOL_SETTINGSDLG_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SETTINGSDLG_IDNAME, const wxString& caption = SYMBOL_SETTINGSDLG_TITLE, const wxPoint& pos = SYMBOL_SETTINGSDLG_POSITION, const wxSize& size = SYMBOL_SETTINGSDLG_SIZE, long style = SYMBOL_SETTINGSDLG_STYLE );
    void CreateControls();
    void OnOK( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event );
	int GetNumOctaves();
	void SetNumOctaves( int octaves );
private:
    wxSpinCtrl* _spnNumOctaves;
    wxButton* _btnClose;
    wxColour _backgroundColour;
    wxColour _textColour;
};
