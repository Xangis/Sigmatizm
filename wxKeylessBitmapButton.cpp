#include "wxKeylessBitmapButton.h"

BEGIN_EVENT_TABLE(wxKeylessBitmapButton,wxBitmapButton)
    EVT_KEY_DOWN(wxKeylessBitmapButton::OnKeyDown)
    EVT_KEY_UP(wxKeylessBitmapButton::OnKeyUp)
	EVT_CHAR(wxKeylessBitmapButton::OnChar)
END_EVENT_TABLE()

void wxKeylessBitmapButton::Create(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint &pos, const wxSize &size, long style )
{
    wxBitmapButton::Create( parent, id, bitmap, pos, size, style );
}

void wxKeylessBitmapButton::OnKeyDown( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxKeylessBitmapButton::OnKeyUp( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxKeylessBitmapButton::OnChar( wxKeyEvent& event)
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}