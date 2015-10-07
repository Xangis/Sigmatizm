#include "wxSwitch.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

BEGIN_EVENT_TABLE(wxSwitch, wxCheckBox)
    EVT_KEY_DOWN(wxSwitch::OnKeyDown)
    EVT_KEY_UP(wxSwitch::OnKeyUp)
    EVT_PAINT(wxSwitch::OnPaint)
END_EVENT_TABLE()

void wxSwitch::Create(wxWindow* parent, wxWindowID id, const wxString &label, const wxPoint &pos, const wxSize &size)
{
    wxCheckBox::Create( parent, id, label, pos, size, wxCHK_2STATE );
}

void wxSwitch::OnPaint( wxPaintEvent& event )
{
    // Draw the image if we have one, otherwise draw a default graphic.
    if( _onImage.Ok() && _offImage.Ok() )
    {
        wxPaintDC dc(this);
		bool state = IsChecked();
		// Figure out whether to draw the on image or the off image.
		if( state )
		{
			wxBitmap buttonBitmap( _onImage );
			dc.DrawBitmap( buttonBitmap, 0, 0, true );
		}
		else
		{
			wxBitmap buttonBitmap( _offImage );
			dc.DrawBitmap( buttonBitmap, 0, 0, true );
		}
    }

	event.Skip(false);
}

void wxSwitch::OnKeyDown( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->ProcessEvent( event );
	}
	//event.Skip(true);
}

void wxSwitch::OnKeyUp( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->ProcessEvent( event );
	}
	//event.Skip(true);
}

/**
    @brief  Set bitmap for drawing button.
*/
void wxSwitch::SetBitmaps( wxImage * onImage, wxImage * offImage )
{
    if( !onImage || !offImage )
    {
        return;
    }
    //_lightLock.lock();
	_onImage = *onImage;
	_offImage = *offImage;
    //_lightLock.unlock();
}

/**
    @brief  Set bitmap for drawing button.
*/
void wxSwitch::SetBitmaps( wxBitmap * onImage, wxBitmap * offImage )
{
    if( !onImage || !offImage )
    {
        return;
    }
    //_lightLock.lock();
	_onImage = onImage->ConvertToImage();
	_offImage = offImage->ConvertToImage();
    //_lightLock.unlock();
}
