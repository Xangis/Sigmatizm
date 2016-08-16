#pragma once

// wxWidgets dialog used for changing MIDI settings.

#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wxKeylessChoice.h"
#include "wxBitmapSpinButton.h"
#include "AudioSettingsInterface.h"
#include "wxSwitch.h"

#define ID_AUDIOSETTINGS_DLG 14000
#define SYMBOL_AUDIOSETTINGSDLG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_AUDIOSETTINGSDLG_TITLE _("Audio Settings")
#define SYMBOL_AUDIOSETTINGSDLG_IDNAME ID_AUDIOSETTINGS_DLG
#define SYMBOL_AUDIOSETTINGSDLG_SIZE wxSize(400, 300)
#define SYMBOL_AUDIOSETTINGSDLG_POSITION wxDefaultPosition
#define ID_BUTTON_CLOSE 14001
#define ID_MIDI_IN_DEVICE 14003
#define ID_MIDI_OUT_DEVICE 14004

class wxAudioSettings : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( wxAudioSettings )
    DECLARE_EVENT_TABLE()
public:
    wxAudioSettings();
    ~wxAudioSettings();
    wxAudioSettings( wxWindow* parent, AudioSettingsInterface* callback, wxWindowID id = SYMBOL_AUDIOSETTINGSDLG_IDNAME, const wxString& caption = SYMBOL_AUDIOSETTINGSDLG_TITLE, const wxPoint& pos = SYMBOL_AUDIOSETTINGSDLG_POSITION, const wxSize& size = SYMBOL_AUDIOSETTINGSDLG_SIZE, long style = SYMBOL_AUDIOSETTINGSDLG_STYLE );
    bool Create( wxWindow* parent, AudioSettingsInterface* callback, wxWindowID id = SYMBOL_AUDIOSETTINGSDLG_IDNAME, const wxString& caption = SYMBOL_AUDIOSETTINGSDLG_TITLE, const wxPoint& pos = SYMBOL_AUDIOSETTINGSDLG_POSITION, const wxSize& size = SYMBOL_AUDIOSETTINGSDLG_SIZE, long style = SYMBOL_AUDIOSETTINGSDLG_STYLE );
    void CreateControls();
    void OnClose( wxCloseEvent& event );
    void OnCloseButton( wxCommandEvent& event );
	void OnChangeMidiOutDevice( wxCommandEvent& event );
    //void OnChangeMidiInDevice( wxCommandEvent& event );
	bool SetForegroundColour(const wxColour &colour);
	bool SetBackgroundColour(const wxColour &colour);
	//void SetMidiInputDeviceIndex(unsigned int index);
	void SetMidiOutputDeviceIndex(unsigned int index);
private:
	wxKeylessChoice* _outDevice;
    //wxKeylessChoice* _inDevice;
    wxButton* _btnClose;
    wxColour _backgroundColour;
    wxColour _textColour;
	AudioSettingsInterface* _callback;
	wxStaticText* _txtMidiOutputDevice;
	//wxStaticText* _txtMidiInputDevice;
};
