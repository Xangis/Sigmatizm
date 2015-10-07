#include "wxSettingsFile.h"

wxSettingsFile::wxSettingsFile() :
		wxXmlDocument()

{
	this->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, _("Settings")));
}

wxString wxSettingsFile::GetValue(wxString key)
{
	wxXmlNode* node = this->GetRoot();
	if( !node )
	{
		return wxString(_(""));
	}

	wxXmlNode *child = node->GetChildren();
	while (child) 
	{
		if (child->GetName() == key) 
		{
			wxXmlProperty* prop = child->GetProperties();
			while( prop != NULL )
			{
				if( prop->GetName() == key )
				{
					return prop->GetValue();
				}
				prop = prop->GetNext();
			}
		}
		child = child->GetNext();
	}
	return wxString(_(""));
}

void wxSettingsFile::SetValue(wxString key, int value)
{
    SetValue(key, wxString::Format(_("%d"), value ));
}

void wxSettingsFile::SetValue(wxString key, float value)
{
    SetValue(key, wxString::Format(_("%f"), value ));
}

void wxSettingsFile::SetValue(wxString key, wxString value)
{
	wxXmlNode* node = this->GetRoot();
	if( !node )
	{
		return;
	}
	wxXmlNode *child = node->GetChildren();
	while (child) 
	{
		if (child->GetName() == key) 
		{
			child->SetContent(value);
			return;
		}
		child = child->GetNext();
	}
	wxXmlNode* newNode = new wxXmlNode(wxXML_ELEMENT_NODE, key);
	newNode->AddProperty(key, value);
	child = node->GetChildren();
	node->InsertChild(newNode, child);
}
