#ifndef _WXSWITCH_H_
#define _WXSWITCH_H_

#include "wx/checkbox.h"
#include "wx/image.h"

class wxSwitch : public wxCheckBox
{
public:
	wxSwitch() : wxCheckBox() {}
	~wxSwitch() {}
    wxSwitch( wxWindow* parent, wxWindowID id, const wxString &label, const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize)
    {
        Create( parent, id, label, pos, size );
    }
    void Create( wxWindow* parent, wxWindowID id, const wxString &label, const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize);
	void OnPaint( wxPaintEvent& event );
	void OnKeyDown( wxKeyEvent& event );
	void OnKeyUp( wxKeyEvent& event );
	// We can pass in a bitmap or an image in order to create custom knobs.
	void SetBitmaps( wxBitmap * onImage, wxBitmap * offImage);
	void SetBitmaps( wxImage * onImage, wxImage * offImage);
	DECLARE_EVENT_TABLE();
private:
   	wxImage _onImage;
	wxImage _offImage;
};

#endif