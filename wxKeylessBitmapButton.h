#pragma once

#include "wx/bmpbuttn.h"

class wxKeylessBitmapButton : public wxBitmapButton
{
public:
	wxKeylessBitmapButton() : wxBitmapButton() {}
	~wxKeylessBitmapButton() {}
    wxKeylessBitmapButton( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBU_AUTODRAW|wxWANTS_CHARS )
    {
        Create( parent, id, bitmap, pos, size, style );
    }
    void Create( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBU_AUTODRAW|wxWANTS_CHARS );
	void OnKeyDown( wxKeyEvent& event );
	void OnKeyUp( wxKeyEvent& event );
	void OnChar( wxKeyEvent& event );
	bool AcceptsFocusFromKeyboard() const { return false; }
	bool AcceptsFocus() const { return false; }
	DECLARE_EVENT_TABLE();
};
