#ifndef _WXBITMAPSLIDER_H_
#define _WXBITMAPSLIDER_H_

#include "wx/control.h"
#include "wx/slider.h"
#include "wx/image.h"

class wxBitmapSlider : public wxControl
{
public:
	wxBitmapSlider() : wxControl() {}
	~wxBitmapSlider() {}
    wxBitmapSlider( wxWindow* parent, wxWindowID id, float value = 0.0f, float minValue = 0.0f, float maxValue = 100.0f,
        const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxSL_HORIZONTAL);
    void Create(  wxWindow* parent, wxWindowID id, float value = 0.0f, float minValue = 0.0f, float maxValue = 100.0f,
        const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxSL_HORIZONTAL);
	void OnPaint( wxPaintEvent& event );
	void OnKeyDown( wxKeyEvent& event );
	void OnKeyUp( wxKeyEvent& event );
	void OnMouse( wxMouseEvent& event );
	void OnSize( wxSizeEvent& event );
	void OnLeave( wxMouseEvent& event );
    void OnEnter( wxMouseEvent& event );
	void SetMin( float minValue );
	void SetMax( float maxValue );
	virtual wxSize DoGetBestSize() const;
	float GetMin();
	float GetMax();
	float GetValue();
	void SetValue( float value );
	void OnMouseRelease( wxMouseEvent& event );
	void OnMouseMove( wxMouseEvent& event );
	// We can pass in a bitmap or an image in order to create custom knobs.
	void SetBitmaps( wxBitmap* backgroundImage, wxBitmap* indicatorImage );
	void SetBitmaps( wxImage* backgroundImage, wxImage* indicatorImage );
	virtual bool AcceptsFocusFromKeyboard() const { return false; } 
	virtual bool AcceptsFocus() const { return false; }
	DECLARE_EVENT_TABLE();
private:
   	wxImage _backgroundImage;
    wxImage _indicatorImage;
	bool _dragging;
	int _style;
	float _value;
	float _minValue;
	float _maxValue;
	int _xSize;
	int _ySize;
	void UpdatePosition(int x, int y);
};

#endif