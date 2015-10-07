#pragma once

#include "wx/button.h"

class wxKeylessButton : public wxButton
{
public:
	wxKeylessButton() : wxButton() {}
	~wxKeylessButton() {}
    wxKeylessButton( wxWindow* parent, wxWindowID id, const wxString &text, const wxPoint& pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBU_AUTODRAW )
    {
        Create( parent, id, text, pos, size, style );
    }
    void Create( wxWindow* parent, wxWindowID id, const wxString &text, const wxPoint& pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBU_AUTODRAW );
	void OnKeyDown( wxKeyEvent& event );
	void OnKeyUp( wxKeyEvent& event );
	void OnChar( wxKeyEvent& event );
	virtual bool AcceptsFocusFromKeyboard() const { return false; }
	DECLARE_EVENT_TABLE();
};
