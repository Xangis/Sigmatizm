#ifndef _WXOCTAVECTRL_H_
#define _WXOCTAVECTRL_H_

#include "wx/wx.h"
#include "wx/bitmap.h"
#include "OctaveCallback.h"
#include <vector>

class wxOctaveCtrl : public wxControl
{
public:
    wxOctaveCtrl();
    wxOctaveCtrl( wxWindow* parent, wxWindowID id, wxBitmap bitmap, int baseNote, OctaveCallback* callback,
		const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxNO_BORDER );
    void Create( wxWindow* parent, wxWindowID id, wxBitmap bitmap, int baseNote, OctaveCallback* parentDlg,
        const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxNO_BORDER );
    void OnOctaveClick( wxMouseEvent &event );
    void OnOctaveRelease( wxMouseEvent &event );
    void OnRightClick( wxMouseEvent& event );
    int GetNoteFromLocation( wxPoint& location );
	void NoteIndicatorOn( int note );
	void NoteIndicatorOff( int note );
	void SetBitmaps( wxBitmap* smallKeyBitmap, wxBitmap* largeKeyBitmap );
	void TurnOffAll(void);
	void SetIndicators(bool * indicators);
	void ClearIndicators();
private:
	void NoteOn( int note );
	void NoteOff( int note );
	bool _playing[12];
	bool _indicator[12];
    wxBitmap _bitmap;
	OctaveCallback* _parent;
    int _baseNote;
	void OnPaint( wxPaintEvent &event );
	wxSize DoGetBestSize() const;
    DECLARE_EVENT_TABLE()
	wxBitmap* _smallKeyIndicator;
	wxBitmap* _largeKeyIndicator;
};

#endif
