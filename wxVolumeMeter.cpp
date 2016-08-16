#include "wxVolumeMeter.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

BEGIN_EVENT_TABLE(wxVolumeMeter,wxGauge)
    EVT_PAINT(wxVolumeMeter::OnPaint)
END_EVENT_TABLE()

void wxVolumeMeter::Create(wxWindow* parent, wxWindowID id, int range,
        const wxPoint &pos, const wxSize &size, long style )
{
    wxGauge::Create( parent, id, range, pos, size, style|wxGA_HORIZONTAL );
}

void wxVolumeMeter::OnPaint( wxPaintEvent& event )
{
    // Draw the image if we have one, otherwise draw a default graphic.
    if( !_bitmap.Ok() && _image.Ok() )
    {
		_bitmap = wxBitmap(_image);
    }

	if( _bitmap.Ok() )
	{
        wxPaintDC dc(this);
		dc.DrawBitmap( _bitmap, 0, 0, true );
	}

	event.Skip(true);
}

/**
    @brief  Set bitmap for drawing control.
*/
void wxVolumeMeter::SetBitmap( wxImage * image)
{
    if( !image )
    {
        return;
    }
	_image = *image;
}

/**
    @brief  Set bitmap for drawing control.
*/
void wxVolumeMeter::SetBitmap( wxBitmap * image)
{
    if( !image )
    {
        return;
    }
	_image = image->ConvertToImage();
}

/**
* Sets the level of the volume meter as a float (percent of max)
*/
void wxVolumeMeter::SetFloatValue( float value )
{
#ifdef linux
    return;
#endif
    int newvalue = (value * this->GetRange());
    this->SetValue(newvalue);
    Refresh();
}
