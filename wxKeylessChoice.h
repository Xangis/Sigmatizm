#ifndef _WXKEYLESSCHOICE_H_
#define _WXKEYLESSCHOICE_H_

#include "wx/choice.h"

class wxKeylessChoice : public wxChoice
{
public:
	wxKeylessChoice() : wxChoice() {}
	~wxKeylessChoice() {}
    wxKeylessChoice( wxWindow* parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, const wxArrayString& choices, long style = wxNO_BORDER )
    {
        Create( parent, id, pos, size, choices, style );
    }
    void Create( wxWindow* parent, wxWindowID id,
        const wxPoint &pos, const wxSize &size, const wxArrayString& choices, long style = wxNO_BORDER );
	void OnKeyDown( wxKeyEvent& event );
	void OnKeyUp( wxKeyEvent& event );
	void OnChar( wxKeyEvent& event );
	virtual bool AcceptsFocusFromKeyboard() const { return false; } 
	DECLARE_EVENT_TABLE();
};

#endif
