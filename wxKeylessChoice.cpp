#include "wxKeylessChoice.h"

BEGIN_EVENT_TABLE(wxKeylessChoice,wxChoice)
    EVT_KEY_DOWN(wxKeylessChoice::OnKeyDown)
    EVT_KEY_UP(wxKeylessChoice::OnKeyUp)
	EVT_CHAR(wxKeylessChoice::OnChar)
END_EVENT_TABLE()

void wxKeylessChoice::Create(wxWindow* parent, wxWindowID id,
        const wxPoint &pos, const wxSize &size, const wxArrayString& choices, long style )
{
    wxChoice::Create( parent, id, pos, size, choices, style );
}

void wxKeylessChoice::OnKeyDown( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxKeylessChoice::OnKeyUp( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxKeylessChoice::OnChar( wxKeyEvent& )
{
}
