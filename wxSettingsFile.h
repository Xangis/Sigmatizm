#ifndef _WXSETTINGSFILE_H_
#define _WXSETTINGSFILE_H_

#include "wx/wx.h"
#include "wx/xml/xml.h"

class wxSettingsFile : public wxXmlDocument
{
public:
    wxSettingsFile();
	wxString GetValue(wxString key);
	void SetValue(wxString key, wxString value);
	void SetValue(wxString key, int value);
	void SetValue(wxString key, float value);
private:

};

#endif