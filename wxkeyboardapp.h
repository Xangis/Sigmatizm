/////////////////////////////////////////////////////////////////////////////
// Name:        wxkeyboardapp.h
// Purpose:     
// Author:      Jason Champion
// Modified by: 
// Created:     16/11/2006 15:58:39
// RCS-ID:      
// Copyright:   (c) 2006
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VST
#include "aeffect.h"
#include "aeffeditor.h"
#ifdef DLLFUNCTIONS_EXPORTS
#define DLLFUNCTIONS_API __declspec(dllexport)
#else
#define DLLFUNCTIONS_API __declspec(dllimport)
#endif
extern "C" DLLFUNCTIONS_API AEffect* VSTPluginMain (audioMasterCallback audioMaster);
#endif

/*!
 * Includes
 */

#include "wx/image.h"
#include "wxkeyboard.h"

/*!
 * Forward declarations
 */

/*!
 * Control identifiers
 */

/*!
 * wxKeyboardApp class declaration
 */

#ifdef VST
class wxKeyboardApp: public wxApp, public AEffEditor
#else
class wxKeyboardApp: public wxApp
#endif
{    
    DECLARE_CLASS( wxKeyboardApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxKeyboardApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

#ifdef VST
	virtual bool open(void* ptr);
	virtual void close();
    wxKeyboardApp(AudioEffect* pEffect);
#endif
private:
    wxKeyboard* _mainWindow;
    wxWindow* _parent;
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(wxKeyboardApp)