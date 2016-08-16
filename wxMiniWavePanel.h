#pragma once

#include "wx/wx.h"

class wxMiniWavePanel : public wxPanel
{
public:
	wxMiniWavePanel();
	~wxMiniWavePanel() { }
    wxMiniWavePanel( wxWindow* parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    void Create( wxWindow* parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
	void OnPaint( wxPaintEvent& event );
    virtual wxSize DoGetBestSize() const;
    void SetData(float* data);
	DECLARE_EVENT_TABLE();
private:
    int _xSize;
    int _ySize;
    float height[120];
};