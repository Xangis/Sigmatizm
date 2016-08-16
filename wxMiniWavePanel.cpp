#include "wxMiniWavePanel.h"

BEGIN_EVENT_TABLE(wxMiniWavePanel, wxPanel)
    EVT_PAINT(wxMiniWavePanel::OnPaint)
END_EVENT_TABLE()

wxMiniWavePanel::wxMiniWavePanel() : wxPanel()
{
}

wxMiniWavePanel::wxMiniWavePanel(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
{
    Create( parent, id, pos, size );
}

void wxMiniWavePanel::Create(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
{
    _xSize = size.GetX();
    _ySize = size.GetY();
    wxPanel::Create( parent, id, pos, size, wxNO_BORDER|wxWANTS_CHARS );
}

void wxMiniWavePanel::SetData(float* data)
{
    memcpy(&height[0], data, (sizeof(float) * _xSize));
    Refresh();
}

/**
* Custom paint routine that shows harmonics on the panel.
*/
void wxMiniWavePanel::OnPaint( wxPaintEvent& event )
{
	wxColour foregroundColour = wxColour( 102, 51, 0 );
	wxColour brushColour = wxColour(224,224, 0);
    // Draw the image if we have one, otherwise draw a default graphic.
	wxPaintDC dc(this);
    dc.SetPen(wxPen(foregroundColour));
    dc.DrawLine(0,(_ySize/2), _xSize, (_ySize/2));
	dc.SetPen(wxPen(brushColour));
	dc.SetBrush(wxBrush(brushColour));
    int previousHeight = (((float)_ySize/2.0) - height[0]*(float)(_ySize/2.0));
    dc.DrawPoint(0, previousHeight );
    for( int i = 0; i < _xSize; i++ )
    {
        dc.DrawLine(i-1, previousHeight, i, (((float)_ySize/2.0) - height[i]*(float)(_ySize/2.0)));
        previousHeight = ((float)(_ySize/2.0) - height[i]*(float)(_ySize/2.0));
        //dc.DrawPoint(i, (40.0 - height[i]*40.0));
    }
	event.Skip(false);
}

/**
* This is required to prevent the control from defaulting to size 80,100.
*/
wxSize wxMiniWavePanel::DoGetBestSize() const
{
	return wxSize(_xSize, _ySize);
}
