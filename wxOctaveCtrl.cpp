#include "wxOctaveCtrl.h"

BEGIN_EVENT_TABLE(wxOctaveCtrl,wxControl)
    EVT_LEFT_DOWN(wxOctaveCtrl::OnOctaveClick)
    EVT_LEFT_UP(wxOctaveCtrl::OnOctaveRelease)
    EVT_PAINT(wxOctaveCtrl::OnPaint)
    EVT_RIGHT_DOWN(wxOctaveCtrl::OnRightClick)
END_EVENT_TABLE()

wxOctaveCtrl::wxOctaveCtrl( wxWindow* parent, wxWindowID id, wxBitmap bitmap, int baseNote, OctaveCallback* callback,
		const wxPoint &pos, const wxSize &size, long style )
{
    Create( parent, id, bitmap, baseNote, callback, pos, size, style );
}

wxOctaveCtrl::wxOctaveCtrl() : wxControl()
{
	memset( _playing, 0, (sizeof(bool) * 12));
	memset( _indicator, 0, (sizeof(bool) * 12));
	_parent = NULL;
	_smallKeyIndicator = NULL;
	_largeKeyIndicator = NULL;
}

void wxOctaveCtrl::SetIndicators(bool* indicators)
{
	memcpy( _indicator, indicators, (sizeof(bool) * 12));
	Refresh();
}

void wxOctaveCtrl::ClearIndicators()
{
	memset( _indicator, 0, (sizeof(bool) * 12 ));
	Refresh();
}

void wxOctaveCtrl::OnOctaveClick( wxMouseEvent& event )
{
    wxPoint location = event.GetPosition();
    int note = GetNoteFromLocation( location );
	NoteOn(note);
	Refresh();
    return;
}

void wxOctaveCtrl::OnOctaveRelease( wxMouseEvent& )
{
	TurnOffAll();
    // All notes off on mouse release.
    _parent->AllNotesOff();
    return;
}

void wxOctaveCtrl::TurnOffAll()
{
	for( int i = 0; i < 12; i++ )
	{
		_playing[i] = false;
	}
	//Refresh();
}

void wxOctaveCtrl::Create (wxWindow* parent, wxWindowID id, wxBitmap bitmap, int baseNote, OctaveCallback* callback,
                     const wxPoint &pos, const wxSize &size, long style)
{
	memset( _playing, 0, (sizeof(bool) * 12));
	memset( _indicator, 0, (sizeof(bool) * 12));
    _bitmap = bitmap;
    _baseNote = baseNote;
	_parent = callback;
	_smallKeyIndicator = NULL;
	_largeKeyIndicator = NULL;
    wxControl::Create (parent, id, pos, size, style);
}

void wxOctaveCtrl::SetBitmaps( wxBitmap* smallKeyBitmap, wxBitmap* largeKeyBitmap )
{
	_smallKeyIndicator = smallKeyBitmap;
	_largeKeyIndicator = largeKeyBitmap;
}

void wxOctaveCtrl::OnPaint(wxPaintEvent&)
{
    wxPaintDC dc(this);
	dc.DrawBitmap( _bitmap, 0, 0, true );
	// TODO: Draw something special on each key if a specific note is playing.
	dc.SetBrush( *wxRED_BRUSH );
	dc.SetPen( *wxBLACK_PEN );
	if( _smallKeyIndicator == NULL || _largeKeyIndicator == NULL )
	{
		if( _playing[0] == true ) dc.DrawCircle( 11, 75, 7 );
		if( _playing[1] == true ) dc.DrawCircle( 19, 45, 5 );
		if( _playing[2] == true ) dc.DrawCircle( 30, 75, 7 );
		if( _playing[3] == true ) dc.DrawCircle( 42, 45, 5 );
		if( _playing[4] == true ) dc.DrawCircle( 50, 75, 7 );
		if( _playing[5] == true ) dc.DrawCircle( 69, 75, 7 );
		if( _playing[6] == true ) dc.DrawCircle( 77, 45, 5 );
		if( _playing[7] == true ) dc.DrawCircle( 89, 75, 7 );
		if( _playing[8] == true ) dc.DrawCircle( 98, 45, 5 );
		if( _playing[9] == true ) dc.DrawCircle( 108, 75, 7 );
		if( _playing[10] == true ) dc.DrawCircle( 120, 45, 5 );
		if( _playing[11] == true ) dc.DrawCircle( 128, 75, 7 );
	}
	else
	{
		if( _playing[0] == true ) dc.DrawBitmap(*_largeKeyIndicator, 3, 69, true );
		if( _playing[1] == true ) dc.DrawBitmap(*_smallKeyIndicator, 13, 40, true );
		if( _playing[2] == true ) dc.DrawBitmap(*_largeKeyIndicator, 23, 69, true );
		if( _playing[3] == true ) dc.DrawBitmap(*_smallKeyIndicator, 36, 40, true );
		if( _playing[4] == true ) dc.DrawBitmap(*_largeKeyIndicator, 43, 69, true );
		if( _playing[5] == true ) dc.DrawBitmap(*_largeKeyIndicator, 62, 69, true );
		if( _playing[6] == true ) dc.DrawBitmap(*_smallKeyIndicator, 71, 40, true );
		if( _playing[7] == true ) dc.DrawBitmap(*_largeKeyIndicator, 82, 69, true );
		if( _playing[8] == true ) dc.DrawBitmap(*_smallKeyIndicator, 92, 40, true );
		if( _playing[9] == true ) dc.DrawBitmap(*_largeKeyIndicator, 101, 69 , true);
		if( _playing[10] == true ) dc.DrawBitmap(*_smallKeyIndicator, 114, 40, true );
		if( _playing[11] == true ) dc.DrawBitmap(*_largeKeyIndicator, 121, 69, true );
	}
	dc.SetBrush( *wxBLUE_BRUSH );
	if( _indicator[0] == true ) dc.DrawCircle( 11, 75, 7 );
	if( _indicator[1] == true ) dc.DrawCircle( 19, 45, 5 );
	if( _indicator[2] == true ) dc.DrawCircle( 30, 75, 7 );
	if( _indicator[3] == true ) dc.DrawCircle( 42, 45, 5 );
	if( _indicator[4] == true ) dc.DrawCircle( 50, 75, 7 );
	if( _indicator[5] == true ) dc.DrawCircle( 69, 75, 7 );
	if( _indicator[6] == true ) dc.DrawCircle( 77, 45, 5 );
	if( _indicator[7] == true ) dc.DrawCircle( 89, 75, 7 );
	if( _indicator[8] == true ) dc.DrawCircle( 98, 45, 5 );
	if( _indicator[9] == true ) dc.DrawCircle( 108, 75, 7 );
	if( _indicator[10] == true ) dc.DrawCircle( 120, 45, 5 );
	if( _indicator[11] == true ) dc.DrawCircle( 128, 75, 7 );
}

void wxOctaveCtrl::NoteOn( int note )
{
	if( note < 0 || note > 11 )
	{
		return;
	}
    _parent->PlayNote(_baseNote + note);
	// Not necessary: Parent turns on indicator.
	/*NoteIndicatorOn( note );*/
}

void wxOctaveCtrl::NoteIndicatorOn( int note )
{
	_playing[note] = true;
	Refresh();
}

void wxOctaveCtrl::NoteOff( int note )
{
	if( note < 0 || note > 11 )
	{
		return;
	}
	_parent->StopNote( _baseNote + note );
	NoteIndicatorOff( note );
}

void wxOctaveCtrl::NoteIndicatorOff( int note )
{
	_playing[note] = false;
	Refresh();
}

void wxOctaveCtrl::OnRightClick( wxMouseEvent& event )
{
    wxPoint location = event.GetPosition();
    int note = GetNoteFromLocation( location );
	_parent->OnRightClick( note + _baseNote );
    event.Skip();
}

int wxOctaveCtrl::GetNoteFromLocation( wxPoint& location )
{
    int note = 0;
    if( location.y > 55 )
    {
        // We're on the bottom part of the keyboard, black notes not possible.
        if( location.x < 20 )
        {
            note = 0; // C
        }
        else if( location.x < 39 )
        {
            note = 2; // D
        }
        else if( location.x < 59 )
        {
            note = 4; // E
        }
        else if( location.x < 78 )
        {
            note = 5; // F
        }
        else if( location.x < 98 )
        {
            note = 7; // G
        }
        else if( location.x < 117 )
        {
            note = 9; // A
        }
        else
        {
            note = 11; // B
        }
    }
    else
    {
        // We're on the top part of the keyboard, black notes are possible.
        if( location.x < 12 )
        {
            note = 0; // C
        }
        else if( location.x < 25 )
        {
            note = 1; // C#
        }
        else if( location.x < 36 )
        {
            note = 2; // D
        }
        else if( location.x < 48 )
        {
            note = 3; // D#
        }
        else if( location.x < 59 )
        {
            note = 4; // E
        }
        else if( location.x < 70 )
        {
            note = 5; // F
        }
        else if( location.x < 83 )
        {
            note = 6; // F#
        }
        else if( location.x < 92 )
        {
            note = 7; // G
        }
        else if( location.x < 104 )
        {
            note = 8; // G#
        }
        else if( location.x < 114 )
        {
            note = 9; // A
        }
        else if( location.x < 126 )
        {
            note = 10; // A#
        }
        else
        {
            note = 11; // B
        }
    }
    return note;
}

wxSize wxOctaveCtrl::DoGetBestSize () const
{
	return wxSize(137, 99);
}
