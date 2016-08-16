#include "wxBitmapSpinButton.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

BEGIN_EVENT_TABLE(wxBitmapSpinButton, wxSpinButton)
    EVT_KEY_DOWN(wxBitmapSpinButton::OnKeyDown)
    EVT_KEY_UP(wxBitmapSpinButton::OnKeyUp)
    EVT_PAINT(wxBitmapSpinButton::OnPaint)
END_EVENT_TABLE()

void wxBitmapSpinButton::Create(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
{
    wxSpinButton::Create( parent, id, pos, size, style );
}

void wxBitmapSpinButton::OnPaint( wxPaintEvent& event )
{
    // Draw the image if we have one, otherwise draw a default graphic.
    if( _image.Ok() )
    {
        wxPaintDC dc(this);
		wxBitmap buttonBitmap( _image );
		dc.DrawBitmap( buttonBitmap, 0, 0, true );
    }

	event.Skip(false);
}

void wxBitmapSpinButton::OnKeyDown( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxBitmapSpinButton::OnKeyUp( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	//event.Skip(true);
}

/**
    @brief  Set bitmap for drawing button.
*/
void wxBitmapSpinButton::SetBitmap( wxImage * image)
{
    if( !image )
    {
        return;
    }
    //_lightLock.lock();
	_image = *image;
    //_lightLock.unlock();
}

/**
    @brief  Set bitmap for drawing button.
*/
void wxBitmapSpinButton::SetBitmap( wxBitmap * image)
{
    if( !image )
    {
        return;
    }
    //_lightLock.lock();
	_image = image->ConvertToImage();
    //_lightLock.unlock();
}

