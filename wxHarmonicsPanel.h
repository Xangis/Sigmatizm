#pragma once

#include "wx/wx.h"
#include "HarmonicsCallback.h"

#define MAX_HARMONICS 128

class wxHarmonicsPanel : public wxPanel
{
public:
	wxHarmonicsPanel(wxStaticText* polyphonyText, wxStaticText* peakLevelText);
	~wxHarmonicsPanel() { }
    wxHarmonicsPanel( wxStaticText* polyphonyText, wxStaticText* peakLevelText, wxWindow* parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    void Create( wxWindow* parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
	void OnPaint( wxPaintEvent& event );
	void OnClick( wxMouseEvent& event );
    void OnMouseUp( wxMouseEvent& event );
    void OnMouseMove( wxMouseEvent& event );
	void OnRightClick( wxMouseEvent& event );
    void OnRightUp( wxMouseEvent& event );
    void OnMouseLeave( wxMouseEvent& event );
    void OnMouseEnter( wxMouseEvent& event );
	void SetHarmonicLevels( float* initialLevels, float* finalLevels );
	void SetInitialHarmonics( float* initialLevels );
	void SetFinalHarmonics( float* finalLevels );
	bool GetInitialVisible();
	void RecalculateLevels();
	void Normalize(int polyphony);
	void SetZoomLevel(float zoomLevel);
	void SetInitialVisible(bool state);
    void UpdateHarmonicSetting(wxMouseEvent& event, bool leftClick);
	float* GetHarmonicLevels();
	void SetHarmonicsCallback(HarmonicsCallback* callback);
	DECLARE_EVENT_TABLE();
private:
	HarmonicsCallback* _callback;
	float _initialHarmonicLevels[MAX_HARMONICS];
	float _finalHarmonicLevels[MAX_HARMONICS];
	wxStaticText* _polyphonyText;
	wxStaticText* _peakLevelText;
	float _zoomLevel;
	bool _leftDragging;
    bool _rightDragging;
	bool _initialVisible;
};