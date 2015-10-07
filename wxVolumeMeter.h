#pragma once

#include "wx/gauge.h"
#include "wx/image.h"
#include "wx/bitmap.h"

class wxVolumeMeter : public wxGauge
{
public:
	wxVolumeMeter() : wxGauge() {}
	~wxVolumeMeter() {}
    wxVolumeMeter( wxWindow* parent, wxWindowID id, int range,
		const wxPoint &pos, const wxSize &size, long style = wxNO_BORDER )
    {
        Create( parent, id, range, pos, size, style|wxGA_HORIZONTAL );
    }
    void Create( wxWindow* parent, wxWindowID id, int range,
        const wxPoint &pos, const wxSize &size, long style = wxNO_BORDER );
	void OnPaint( wxPaintEvent& event );
	// We can pass in a bitmap or an image in order to create custom knobs.
	void SetBitmap( wxBitmap * image);
	void SetBitmap( wxImage * image);
	void SetFloatValue( float value );
	virtual bool AcceptsFocusFromKeyboard() const { return false; } 
	virtual bool AcceptsFocus() const { return false; }
	DECLARE_EVENT_TABLE();
private:
   	wxImage _image;
	wxBitmap _bitmap;
};
