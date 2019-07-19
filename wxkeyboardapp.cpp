/////////////////////////////////////////////////////////////////////////////
// Name:        wxkeyboardapp.cpp
// Purpose:     
// Author:      Jason Champion
// Modified by: 
// Created:     16/11/2006 15:58:39
// RCS-ID:      
// Copyright:   (c) 2006
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxkeyboardapp.h"

/*!
 * Application instance implementation
 */
#ifdef VST
IMPLEMENT_APP_NO_MAIN( wxKeyboardApp )
#else
IMPLEMENT_APP( wxKeyboardApp )
#endif

/*!
 * wxKeyboardApp type definition
 */
IMPLEMENT_CLASS( wxKeyboardApp, wxApp )

/*!
 * wxKeyboardApp event table definition
 */
BEGIN_EVENT_TABLE( wxKeyboardApp, wxApp )
END_EVENT_TABLE()

/*!
 * Constructor for wxKeyboardApp
 */
wxKeyboardApp::wxKeyboardApp()
{
    _mainWindow = NULL;
}

/*!
 * Initialisation for wxKeyboardApp
 */
bool wxKeyboardApp::OnInit()
{    
#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif

#ifndef VST
    _mainWindow = new Sigmatizm(NULL, ID_KEYBOARD_DLG );
    _mainWindow->Show(true);
#else
    _mainWindow = new Sigmatizm(NULL, NULL, ID_KEYBOARD_DLG );
#endif

    return true;
}

/*!
 * Cleanup for wxKeyboardApp
 */
int wxKeyboardApp::OnExit()
{    
    return wxApp::OnExit();
}

#ifdef VST
wxKeyboardApp::wxKeyboardApp (AudioEffect *pEffect) : AEffEditor (pEffect)
{
    _mainWindow = new Sigmatizm(NULL, NULL, ID_KEYBOARD_DLG );
    //rect.left   = 0;
    //rect.top    = 0;
    pEffect->setEditor (this);
    systemWindow = 0;
} 

/**
* DLL Stuff for VST
*/

//------------------------------------------------------------------------
/** Must be implemented externally. */
extern AudioEffect* createEffectInstance (audioMasterCallback audioMaster);

extern "C" {

#if defined (__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define VST_EXPORT	__attribute__ ((visibility ("default")))
#else
	#define VST_EXPORT
#endif

//------------------------------------------------------------------------
/** Prototype of the export function main */
//------------------------------------------------------------------------
VST_EXPORT AEffect* VSTPluginMain (audioMasterCallback audioMaster)
{
    if(NULL == wxTheApp)
    {
        ::wxInitialize();
    }

	// Get VST Version of the Host
	if (!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
		return 0;  // old version

	// Create the AudioEffect
	AudioEffect* effect = createEffectInstance (audioMaster);
	if (!effect)
		return 0;

	// Return the VST AEffect structur
	return effect->getAeffect ();
}
//
//extern "C" __declspec(dllexport) void DLLFunction(HWND handle)
//{
//#if wxUSE_XPM
//    wxImage::AddHandler(new wxXPMHandler);
//#endif
//#if wxUSE_LIBPNG
//    wxImage::AddHandler(new wxPNGHandler);
//#endif
//#if wxUSE_LIBJPEG
//    wxImage::AddHandler(new wxJPEGHandler);
//#endif
//#if wxUSE_GIF
//    wxImage::AddHandler(new wxGIFHandler);
//#endif
//    wxKeyboard* mainWindow = new wxKeyboard(NULL, NULL, ID_KEYBOARD_DLG );
//    mainWindow->SetHWND((WXHWND)handle);
//    mainWindow->Show(true);
//} 

// support for old hosts not looking for VSTPluginMain
#if (TARGET_API_MAC_CARBON && __ppc__)
VST_EXPORT int main_macho (audioMasterCallback audioMaster) { return VSTPluginMain (audioMaster); }
#elif WIN32
VST_EXPORT int MAIN (audioMasterCallback audioMaster) { return (int)VSTPluginMain (audioMaster); }
#elif BEOS
VST_EXPORT int main_plugin (audioMasterCallback audioMaster) { return VSTPluginMain (audioMaster); }
#endif

} // extern "C"

bool wxKeyboardApp::open(void *ptr)
{
    // add this to update the value the first time
    //postUpdate ();

    // create a parent wxWindow from the host provided window
    _parent = new wxWindow();
    _parent->SetEventHandler(_parent);
    _parent->SetHWND((HWND) ptr);
    _parent->SetId(wxGetWindowId((HWND) ptr)); 

    //systemWindow = ptr;
    if( _mainWindow == NULL )
    {
        _mainWindow = new wxKeyboard(NULL, NULL, ID_KEYBOARD_DLG );
    }
    _mainWindow->Show(true);
    //_mainWindow->SetEventHandler(_mainWindow);
    //_mainWindow->SetHWND((HWND)ptr);
    //_mainWindow->SetId(wxGetWindowId((HWND)ptr));
    return AEffEditor::open(ptr); 
}

void wxKeyboardApp::close()
{
    _parent->SetHWND(NULL);
    delete _parent;
    _parent = NULL;
    //_mainWindow = NULL;

    AEffEditor::close();

    //_mainWindow->Show(false);
    //systemWindow = 0; 
}
#endif
