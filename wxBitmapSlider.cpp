#include "wxBitmapSlider.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "wx/dcbuffer.h"

BEGIN_EVENT_TABLE(wxBitmapSlider, wxControl)
    EVT_PAINT(wxBitmapSlider::OnPaint)
    EVT_KEY_DOWN(wxBitmapSlider::OnKeyDown)
    EVT_KEY_UP(wxBitmapSlider::OnKeyUp)
    EVT_LEFT_UP(wxBitmapSlider::OnMouseRelease)
	EVT_MOTION(wxBitmapSlider::OnMouseMove)
    EVT_LEFT_DOWN(wxBitmapSlider::OnMouse)
	EVT_LEAVE_WINDOW(wxBitmapSlider::OnLeave)
    EVT_ENTER_WINDOW(wxBitmapSlider::OnEnter)
END_EVENT_TABLE()

wxBitmapSlider::wxBitmapSlider( wxWindow* parent, wxWindowID id, float value, float minValue, float maxValue, const wxPoint &pos , const wxSize &size, long style)
{
    Create( parent, id, value, minValue, maxValue, pos, size, style );
}

void wxBitmapSlider::Create(wxWindow* parent, wxWindowID id, float value, float minValue, float maxValue, const wxPoint &pos, const wxSize &size, long style)
{
	_dragging = false;
	_style = style;
	_minValue = minValue;
	_maxValue = maxValue;
	_value = value;
	_xSize = size.GetX();
	_ySize = size.GetY();
    wxControl::Create( parent, id, pos, size, style|wxBORDER_NONE );
}

wxSize wxBitmapSlider::DoGetBestSize() const
{
	return wxSize(_xSize, _ySize);
}

void wxBitmapSlider::OnPaint( wxPaintEvent& event )
{
    // Draw the image if we have one, otherwise draw a default graphic.
    wxBufferedPaintDC dc(this);
    wxSize size = GetSize();
    if( _backgroundImage.Ok() && _indicatorImage.Ok() )
    {
		wxBitmap bkBitmap( _backgroundImage );
		dc.DrawBitmap( bkBitmap, 0, 0, true );
        wxBitmap indBitmap( _indicatorImage );
		int xMargin = _indicatorImage.GetWidth() / 2;
		int yMargin = _indicatorImage.GetHeight() / 2;
		int drawableYSize = size.y - _indicatorImage.GetHeight();
		int drawableXSize = size.x - _indicatorImage.GetWidth();
		if( _style & wxSL_VERTICAL )
		{
			int position = ((_value / _maxValue * (float)drawableYSize));
			if( _style & wxSL_INVERSE )
			{
				position = drawableYSize - position;
			}
			dc.DrawBitmap( indBitmap, ((size.x/2) - xMargin), position, true);
		}
		else if( _style & wxSL_HORIZONTAL )
		{
			int position = ((_value / _maxValue * (float)drawableXSize));
			if( _style & wxSL_INVERSE )
			{
				position = drawableXSize - position;
			}
			dc.DrawBitmap( indBitmap, position, ((size.y/2) - yMargin), true);
		}
    }
    else
    {
		if( _style & wxSL_VERTICAL )
		{
			dc.Clear();
			dc.SetBrush(*wxWHITE_BRUSH);
			dc.SetPen(*wxGREEN_PEN);
	        dc.DrawLine(size.x/2, 0, size.x/2, size.y);
			dc.DrawRectangle(size.x/2 - 3, (_value / _maxValue * (float)size.y) - 2, 7, 5);
		}
		else
		{
			dc.Clear();
			dc.SetBrush(*wxWHITE_BRUSH);
			dc.SetPen(*wxGREEN_PEN);
	        dc.DrawLine(0, size.y/2, size.x, size.y/2);
			dc.DrawRectangle((_value / _maxValue * (float)size.x) - 2, size.y/2 - 3, 5, 7 );
		}
    }
	event.Skip();
}

void wxBitmapSlider::OnMouseRelease(wxMouseEvent &event)
{
	if( _dragging )
	{
		_dragging = false;
		int x;
		int y;
		event.GetPosition(&x, &y);
		UpdatePosition(x, y);
		event.Skip();

		wxCommandEvent cevent( wxEVT_SCROLL_THUMBRELEASE, GetId() );
		cevent.SetInt( _value );
		cevent.SetEventObject( this );

		GetEventHandler()->ProcessEvent( cevent );
	}
}

void wxBitmapSlider::OnMouseMove(wxMouseEvent &event)
{
	if( _dragging )
	{
		int x;
		int y;
		event.GetPosition(&x, &y);
		UpdatePosition(x, y);
	}
	event.Skip();
}

void wxBitmapSlider::OnLeave(wxMouseEvent &event)
{
	if( _dragging )
	{
		_dragging = false;
		wxCommandEvent cevent( wxEVT_SCROLL_THUMBRELEASE, GetId() );
		cevent.SetInt( _value );
		cevent.SetEventObject( this );

		GetEventHandler()->ProcessEvent( cevent );
	}
	event.Skip();
}

void wxBitmapSlider::OnEnter(wxMouseEvent& event)
{
    if( event.LeftIsDown() )
    {
        _dragging = true;
    }
    event.Skip();
}

void wxBitmapSlider::UpdatePosition(int x, int y)
{
	int drawableWidth;
	int drawableHeight;
	float xMargin = 0.0f;
	float yMargin = 0.0f;
	GetClientSize(&drawableWidth, &drawableHeight);

    if( _backgroundImage.Ok() && _indicatorImage.Ok() )
    {
		drawableHeight -= _indicatorImage.GetHeight();
		drawableWidth -= _indicatorImage.GetWidth();
		xMargin = _indicatorImage.GetWidth() / 2;
		yMargin = _indicatorImage.GetHeight() / 2;
	}

	if( _style & wxSL_VERTICAL )
	{
		float val = (((float)y - yMargin) / (float)drawableHeight) * _maxValue;
		if( val < _minValue ) val = _minValue;
		if( val > _maxValue ) val = _maxValue;
		if( _style & wxSL_INVERSE )
		{
			val = _maxValue - val;
		}
		if( _value != val )
		{
			_value = val;
			Refresh();
		}
	}
	else
	{
		float val = (float)x / ((float)drawableWidth) * _maxValue;
		if( val < _minValue ) val = _minValue;
		if( val > _maxValue ) val = _maxValue;
		if( _style & wxSL_INVERSE )
		{
			val = _maxValue - val;
		}
		if( _value != val )
		{
			_value = val;
			Refresh();
		}
	}

    wxCommandEvent cevent( wxEVT_SCROLL_CHANGED, GetId() );
    cevent.SetInt( _value );
    cevent.SetEventObject( this );

    GetEventHandler()->ProcessEvent( cevent );
}

void wxBitmapSlider::OnMouse(wxMouseEvent& event)
{
	_dragging = true;
	int x;
	int y;
	event.GetPosition(&x, &y);
	UpdatePosition(x, y);
	event.Skip();
}

void wxBitmapSlider::SetMin(float minValue)
{
	if( _minValue != minValue )
	{
		_minValue = minValue;
		if( _value < _minValue )
			_value = _minValue;
		Refresh();
	}
}

void wxBitmapSlider::SetMax(float maxValue)
{
	if( _maxValue != maxValue )
	{
		_maxValue = maxValue;
		if( _value > _maxValue )
			_value = _maxValue;
		Refresh();
	}
}

float wxBitmapSlider::GetValue()
{
	return _value;
}

float wxBitmapSlider::GetMin()
{
	return _minValue;
}

float wxBitmapSlider::GetMax()
{
	return _maxValue;
}

void wxBitmapSlider::SetValue(float value)
{
	if( _value != value )
	{
		_value = value;
		Refresh();
	}
}

/**
    @brief  Set bitmap for drawing button.
*/
void wxBitmapSlider::SetBitmaps( wxImage* backgroundImage, wxImage* indicatorImage )
{
    if( !backgroundImage || !indicatorImage )
    {
        return;
    }
    // Need to do a wxMutex.lock() here;
	_backgroundImage = *backgroundImage;
    _indicatorImage = *indicatorImage;
    // Need to do a wxMutex.unlock() here;
}

/**
    @brief  Set bitmap for drawing button.
*/
void wxBitmapSlider::SetBitmaps( wxBitmap* backgroundImage, wxBitmap* indicatorImage )
{
    if( !backgroundImage || !indicatorImage )
    {
        return;
    }
    // Need to do a wxMutex.lock() here;
	_backgroundImage = backgroundImage->ConvertToImage();
    _indicatorImage = indicatorImage->ConvertToImage();
    // Need to do a wxMutex.unlock() here;
}

void wxBitmapSlider::OnKeyDown( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	event.Skip(false);
}

void wxBitmapSlider::OnKeyUp( wxKeyEvent& event )
{
	wxWindow* parent = this->GetParent();
	if( parent )
	{
		parent->GetEventHandler()->ProcessEvent( event );
	}
	event.Skip(false);
}
