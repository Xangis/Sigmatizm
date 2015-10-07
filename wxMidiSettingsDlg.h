#ifndef _wxMidiSettingsDlg_H_
#define _wxMidiSettingsDlg_H_

// wxWidgets dialog used for changing MIDI settings.

#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wxKeylessChoice.h"
#include "wxBitmapSpinButton.h"
#include "MidiSettingsInterface.h"
#include "../wxAudioControls/wxSwitch.h"

#define ID_MIDISETTINGS_DLG 14000
#define SYMBOL_wxMidiSettingsDlg_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_wxMidiSettingsDlg_TITLE _("MIDI Settings")
#define SYMBOL_wxMidiSettingsDlg_IDNAME ID_MIDISETTINGS_DLG
#define SYMBOL_wxMidiSettingsDlg_SIZE wxSize(400, 300)
#define SYMBOL_wxMidiSettingsDlg_POSITION wxDefaultPosition
#define ID_BUTTON_CLOSE 14001
#define ID_CHK_MIDIOUT 14002
#define ID_MIDI_IN_DEVICE 14003
#define ID_MIDI_OUT_DEVICE 14004
#define ID_INPUTCHANNELSPIN 14005
#define ID_OUTPUTCHANNELSPIN 14006
#define ID_INPUTCHANNELTEXT 14008
#define ID_OUTPUTCHANNELTEXT 14009

class wxMidiSettingsDlg : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( wxMidiSettingsDlg )
    DECLARE_EVENT_TABLE()
public:
    wxMidiSettingsDlg();
    ~wxMidiSettingsDlg();
    wxMidiSettingsDlg( wxWindow* parent, MidiSettingsInterface* callback, wxWindowID id = SYMBOL_wxMidiSettingsDlg_IDNAME, const wxString& caption = SYMBOL_wxMidiSettingsDlg_TITLE, const wxPoint& pos = SYMBOL_wxMidiSettingsDlg_POSITION, const wxSize& size = SYMBOL_wxMidiSettingsDlg_SIZE, long style = SYMBOL_wxMidiSettingsDlg_STYLE );
    bool Create( wxWindow* parent, MidiSettingsInterface* callback, wxWindowID id = SYMBOL_wxMidiSettingsDlg_IDNAME, const wxString& caption = SYMBOL_wxMidiSettingsDlg_TITLE, const wxPoint& pos = SYMBOL_wxMidiSettingsDlg_POSITION, const wxSize& size = SYMBOL_wxMidiSettingsDlg_SIZE, long style = SYMBOL_wxMidiSettingsDlg_STYLE );
    void CreateControls();
    void OnClose( wxCloseEvent& event );
    void OnCloseButton( wxCommandEvent& event );
    void OnInputChannelSpinUp( wxSpinEvent& event );
    void OnInputChannelSpinDown( wxSpinEvent& event );
    void OnOutputChannelSpinUp( wxSpinEvent& event );
    void OnOutputChannelSpinDown( wxSpinEvent& event );
	void OnChangeMidiOutDevice( wxCommandEvent& event );
    void OnChangeMidiInDevice( wxCommandEvent& event );
	bool SetForegroundColour(const wxColour &colour);
	bool SetBackgroundColour(const wxColour &colour);
	void SetMidiOutputCheckbox(bool enabled);
	void OnMidiOutputEnabledCheck( wxCommandEvent& event);
	void SetMidiInputDeviceIndex(unsigned int index);
	void SetMidiOutputDeviceIndex(unsigned int index);
	void SetMidiInputChannel(int channel);
	void SetMidiOutputChannel(int channel);
private:
	wxKeylessChoice* _outDevice;
    wxKeylessChoice* _inDevice;
	wxBitmapSpinButton* _inputChannelSpin;
	wxBitmapSpinButton* _outputChannelSpin;
    wxButton* _btnClose;
    wxColour _backgroundColour;
    wxColour _textColour;
	wxSwitch* _chkMidiOutEnabled;
	MidiSettingsInterface* _callback;
	wxStaticText* _inputChannelText;
	wxStaticText* _outputChannelText;
	wxStaticText* _txtMidiOutputEnabled;
	wxStaticText* _txtMidiOutputDevice;
	wxStaticText* _txtMidiOutputChannel;
	wxStaticText* _txtMidiInputChannel;
	wxStaticText* _txtMidiInputDevice;
};

#endif