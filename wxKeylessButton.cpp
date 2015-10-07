#include "wxKeylessButton.h"

BEGIN_EVENT_TABLE(wxKeylessButton,wxButton)
    EVT_KEY_DOWN(wxKeylessButton::OnKeyDown)
    EVT_KEY_UP(wxKeylessButton::OnKeyUp)
	EVT_CHAR(wxKeylessButton::OnChar)
END_EVENT_TABLE()

void wxKeylessButton::Create(wxWindow* parent, wxWindowID id, const wxString &text, const wxPoint &pos, const wxSize &size, long style )
{
    wxButton::Create( parent, id, text, pos, size, style );
}

void wxKeylessButton::OnKeyDown( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxKeylessButton::OnKeyUp( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxKeylessButton::OnChar( wxKeyEvent& event)
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}