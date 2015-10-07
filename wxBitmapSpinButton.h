#ifndef _WXBITMAPSPINBUTTON_H_
#define _WXBITMAPSPINBUTTON_H_

#include "wx/image.h"
#include "wx/spinctrl.h"

class wxBitmapSpinButton : public wxSpinButton
{
public:
	wxBitmapSpinButton() : wxSpinButton() {}
	~wxBitmapSpinButton() {}
    wxBitmapSpinButton( wxWindow* parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize, long style = wxSP_ARROW_KEYS)
    {
        Create( parent, id, pos, size, style );
    }
    void Create( wxWindow* parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize, long style = wxSP_ARROW_KEYS);
	void OnKeyDown( wxKeyEvent& event );
	void OnKeyUp( wxKeyEvent& event );
        void OnPaint( wxPaintEvent& event );
	virtual bool AcceptsFocusFromKeyboard() const { return false; } 
	virtual bool AcceptsFocus() const { return false; }
	// We can pass in a bitmap or an image in order to create custom knobs.
	void SetBitmap( wxBitmap * image);
	void SetBitmap( wxImage * image);
	DECLARE_EVENT_TABLE();
private:
   	wxImage _image;
};

#endif
