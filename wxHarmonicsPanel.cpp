#include "wxHarmonicsPanel.h"

BEGIN_EVENT_TABLE(wxHarmonicsPanel, wxPanel)
    EVT_PAINT(wxHarmonicsPanel::OnPaint)
    EVT_LEFT_DOWN(wxHarmonicsPanel::OnClick)
	EVT_RIGHT_DOWN(wxHarmonicsPanel::OnRightClick)
    EVT_RIGHT_UP(wxHarmonicsPanel::OnRightUp)
    EVT_LEFT_UP(wxHarmonicsPanel::OnMouseUp)
    EVT_MOTION(wxHarmonicsPanel::OnMouseMove)
    EVT_LEAVE_WINDOW(wxHarmonicsPanel::OnMouseLeave)
    EVT_ENTER_WINDOW(wxHarmonicsPanel::OnMouseEnter)
END_EVENT_TABLE()

wxHarmonicsPanel::wxHarmonicsPanel(wxStaticText* polyphonyText, wxStaticText* peakLevelText) : wxPanel()
{
	memset(_initialHarmonicLevels, 0, (sizeof(float) * MAX_HARMONICS ));
	memset(_finalHarmonicLevels, 0, (sizeof(float) * MAX_HARMONICS ));
	_peakLevelText = peakLevelText;
	_polyphonyText = polyphonyText;
	_callback = NULL;
	_zoomLevel = 2.0f;
	_leftDragging = false;
    _rightDragging = false;
	_initialVisible = true;
}

wxHarmonicsPanel::wxHarmonicsPanel( wxStaticText* polyphonyText, wxStaticText* peakLevelText, wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
{
	memset(_initialHarmonicLevels, 0, (sizeof(float) * MAX_HARMONICS ));
	memset(_finalHarmonicLevels, 0, (sizeof(float) * MAX_HARMONICS ));
	_peakLevelText = peakLevelText;
	_polyphonyText = polyphonyText;
	_callback = NULL;
	_zoomLevel = 2.0f;
	_leftDragging = false;
    _rightDragging = false;
	_initialVisible = true;
    Create( parent, id, pos, size );
}

void wxHarmonicsPanel::Create(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
{
    wxPanel::Create( parent, id, pos, size, wxNO_BORDER|wxWANTS_CHARS );
}

void wxHarmonicsPanel::SetHarmonicLevels( float* initialLevels, float* finalLevels )
{
	memcpy( &_initialHarmonicLevels, initialLevels, (sizeof(float) * MAX_HARMONICS ));
	memcpy( &_finalHarmonicLevels, finalLevels, (sizeof(float) * MAX_HARMONICS ));
	if( _callback != NULL )
	{
		_callback->OnHarmonicsChanged( &_initialHarmonicLevels[0], &_finalHarmonicLevels[0] );
	}
	RecalculateLevels();
	Refresh();
}

void wxHarmonicsPanel::SetInitialHarmonics( float* initialLevels )
{
	memcpy( &_initialHarmonicLevels, initialLevels, (sizeof(float) * MAX_HARMONICS ));
	if( _callback != NULL )
	{
		_callback->OnHarmonicsChanged( &_initialHarmonicLevels[0], &_finalHarmonicLevels[0] );
	}
	RecalculateLevels();
	Refresh();
}

void wxHarmonicsPanel::SetFinalHarmonics( float* finalLevels )
{
	memcpy( &_finalHarmonicLevels, finalLevels, (sizeof(float) * MAX_HARMONICS ));
	if( _callback != NULL )
	{
		_callback->OnHarmonicsChanged( &_initialHarmonicLevels[0], &_finalHarmonicLevels[0] );
	}
	RecalculateLevels();
	Refresh();
}

void wxHarmonicsPanel::SetZoomLevel(float zoomLevel)
{
	_zoomLevel = zoomLevel;
	Refresh();
}

void wxHarmonicsPanel::RecalculateLevels()
{
	float total = 0.0f;
	for( int i = 0; i < MAX_HARMONICS; i++ )
	{
		if( _initialVisible )
		{
			total += _initialHarmonicLevels[i];
		}
		else
		{
			total += _finalHarmonicLevels[i];
		}
	}

	if( _peakLevelText != NULL )
	{
		_peakLevelText->SetLabel(wxString::Format(_("Peak Level: %6.3f"), total));
	}
	if( _polyphonyText != NULL )
	{
		if( total > 0.0f )
		{
			_polyphonyText->SetLabel(wxString::Format(_("Safe Polyphony: %d"), (int)(1.0f / total )));
		}
		else
		{
			_polyphonyText->SetLabel(_("Safe Polyphony: Inf"));
		}
	}
}

bool wxHarmonicsPanel::GetInitialVisible()
{
	return _initialVisible;
}

void wxHarmonicsPanel::Normalize(int polyphony)
{
	if( polyphony < 1 )
		return;

	float total = 0.0f;
	for( int i = 0; i < MAX_HARMONICS; i++ )
	{
		if( _initialVisible )
		{
			total += _initialHarmonicLevels[i];
		}
		else
		{
			total += _finalHarmonicLevels[i];
		}
	}

	float divisor = polyphony * total;
	if( divisor != 1.0f && divisor != 0.0f )
	{
		for( int i = 0; i < MAX_HARMONICS; i++ )
		{
			if( _initialVisible )
			{
				_initialHarmonicLevels[i] /= divisor;
			}
			else
			{
				_finalHarmonicLevels[i] /= divisor;
			}
		}
	}
	if( _callback != NULL )
	{
		_callback->OnHarmonicsChanged( &_initialHarmonicLevels[0], &_finalHarmonicLevels[0]  );
	}
	RecalculateLevels();
	Refresh();
}

/**
* Handles left clicks on the harmonics panel and sets new levels.
*/
void wxHarmonicsPanel::OnClick( wxMouseEvent& event )
{
    _leftDragging = true;
    _rightDragging = false;
    UpdateHarmonicSetting(event, true);
}


void wxHarmonicsPanel::UpdateHarmonicSetting(wxMouseEvent& event, bool leftClick)
{
    wxPoint location = event.GetPosition();
	int harmonic = location.x / 5;
    if( leftClick )
    {
	    float ysize = (float)(GetSize().GetY());
	    if( _initialVisible )
	    {
		    _initialHarmonicLevels[harmonic] = ( ( ysize - (float)(location.y)) / ysize ) / _zoomLevel;
	    }
	    else
	    {
		    _finalHarmonicLevels[harmonic] = ( ( ysize - (float)(location.y)) / ysize ) / _zoomLevel;
	    }
    }
    else
    {
	    if( _initialVisible )
	    {
		    _initialHarmonicLevels[harmonic] = 0.0f;
	    }
	    else
	    {
		    _finalHarmonicLevels[harmonic] = 0.0f;
	    }
    }

	if( _callback != NULL )
	{
		_callback->OnHarmonicsChanged( &_initialHarmonicLevels[0], &_finalHarmonicLevels[0] );
	}

    RecalculateLevels();
	Refresh();
    return;
}

/**
* Handles left clicks on the harmonics panel and sets new levels.
*/
void wxHarmonicsPanel::OnMouseUp( wxMouseEvent& event )
{
    _leftDragging = false;
    _rightDragging = false;
    UpdateHarmonicSetting(event, true);
}

void wxHarmonicsPanel::OnMouseMove( wxMouseEvent& event )
{
    if( _leftDragging )
    {
        UpdateHarmonicSetting(event, true);
    }
    else if( _rightDragging )
    {
        UpdateHarmonicSetting(event, false);
    }
    event.Skip();
}

void wxHarmonicsPanel::OnMouseLeave( wxMouseEvent& event )
{
    _leftDragging = false;
    _rightDragging = false;
    event.Skip();
}

void wxHarmonicsPanel::OnMouseEnter( wxMouseEvent& event )
{
    if( event.LeftIsDown() )
    {
        _leftDragging = true;
    }
    if( event.RightIsDown() )
    {
        _rightDragging = true;
    }
    event.Skip();
}

/**
* Handles right clicks on the harmonics panel and clears the level.
*/
void wxHarmonicsPanel::OnRightClick( wxMouseEvent& event )
{
    _rightDragging = true;
    _leftDragging = false;
    UpdateHarmonicSetting(event, false);
}

void wxHarmonicsPanel::OnRightUp( wxMouseEvent& event )
{
    _rightDragging = false;
    _leftDragging = false;
    UpdateHarmonicSetting(event, false);
}

/**
* Gets harmonic levels that have been set in the dialog.  Memory must be deleted by the caller.
*/
float* wxHarmonicsPanel::GetHarmonicLevels()
{
	float* values = new float[MAX_HARMONICS*2];
	memcpy(values, _initialHarmonicLevels, (sizeof(float) * MAX_HARMONICS ));
	// TODO: Verify this line of code.
	memcpy((values + MAX_HARMONICS), _finalHarmonicLevels, (sizeof(float) * MAX_HARMONICS ));
	return values;
}

/**
* Custom paint routine that shows harmonics on the panel.
*/
void wxHarmonicsPanel::OnPaint( wxPaintEvent& event )
{
	wxColour foregroundColour = GetForegroundColour();
	wxColour brushColour = wxColour(224,224,0);
    // Draw the image if we have one, otherwise draw a default graphic.
	wxPaintDC dc(this);
	dc.SetPen(wxPen(foregroundColour));
	dc.SetBrush(wxBrush(brushColour));
	float height = 0.0f;
	float ysize = (float)(GetSize().GetY());
	for( int i = 0; i < MAX_HARMONICS; i++ )
	{
		if( _initialVisible )
		{
			height = _initialHarmonicLevels[i] * ysize * _zoomLevel;
		}
		else
		{
			height = _finalHarmonicLevels[i] * ysize * _zoomLevel;
		}
		dc.DrawRectangle((i*5), ((int)ysize-(int)height), 5, (int)height);
	}
	event.Skip(false);
}

void wxHarmonicsPanel::SetInitialVisible(bool state)
{
	if( _initialVisible != state )
	{
		_initialVisible = state;
		RecalculateLevels();
		Refresh();
	}
}

void wxHarmonicsPanel::SetHarmonicsCallback(HarmonicsCallback* callback)
{
	_callback = callback;
}
