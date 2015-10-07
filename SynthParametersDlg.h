#pragma once

#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wxBitmapSpinButton.h"
#include "wxBitmapSlider.h"
#include "wxKeylessButton.h"
#include "wxHarmonicsPanel.h"

#define ID_SYNTHPARAMETER_DLG 13000
#define SYMBOL_SYNTHPARAMETERDLG_STYLE wxNO_BORDER|wxWANTS_CHARS
#define SYMBOL_SYNTHPARAMETERDLG_IDNAME ID_SYNTHPARAMETER_DLG
#define SYMBOL_SYNTHPARAMETERDLG_SIZE wxSize(500, 540)
#define SYMBOL_SYNTHPARAMETERDLG_POSITION wxDefaultPosition
#define ID_SPIN_SAMPLENOTE 13001
#define ID_SPIN_KEYBOARDNOTE 13002
#define ID_SPIN_VOLUME 13003
#define ID_BUTTON_BROWSE 13004
#define ID_TEXT_FILENAME 13005
#define ID_CHECK_LOOP 13008
#define ID_HARMONIC_PANEL 13009
#define ID_TARGETPOLYPHONYTEXT 13010
#define ID_TARGETPOLYPHONYSPIN 13011

class SynthParametersDlg : public wxPanel
{
    DECLARE_DYNAMIC_CLASS( SynthParametersDlg )
    DECLARE_EVENT_TABLE()
public:
    SynthParametersDlg();
    ~SynthParametersDlg();
    SynthParametersDlg( wxWindow* parent, wxWindowID id = SYMBOL_SYNTHPARAMETERDLG_IDNAME, const wxPoint& pos = SYMBOL_SYNTHPARAMETERDLG_POSITION, const wxSize& size = SYMBOL_SYNTHPARAMETERDLG_SIZE, long style = SYMBOL_SYNTHPARAMETERDLG_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SYNTHPARAMETERDLG_IDNAME, const wxPoint& pos = SYMBOL_SYNTHPARAMETERDLG_POSITION, const wxSize& size = SYMBOL_SYNTHPARAMETERDLG_SIZE, long style = SYMBOL_SYNTHPARAMETERDLG_STYLE );
    void CreateControls();
	void OnPolyphonySpinDown( wxSpinEvent& event );
	void OnPolyphonySpinUp( wxSpinEvent& event );
	void Normalize();
	void SetHarmonicLevels( float* initialLevels, float* finalLevels );
	float* GetHarmonicLevels();
	void SetHarmonicsCallback(HarmonicsCallback* callback);
    void OnKeyDown( wxKeyEvent& event );
    void OnKeyUp( wxKeyEvent& event );
	wxHarmonicsPanel* GetHarmonicPanel() { return _harmonicPanel; }
private:
    wxColour _backgroundColour;
    wxColour _textColour;
	wxHarmonicsPanel* _harmonicPanel;
	wxStaticText* _polyphonyText;
	wxStaticText* _peakLevelText;
	wxStaticText* _targetPolyphony;
	wxBitmapSpinButton* _targetPolyphonySpin;
};
