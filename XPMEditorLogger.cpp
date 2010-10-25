/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditorLogger.cpp
 * Purpose:   a fake logger for catching Settings update events - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-10-25
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMEditorLogger.h"
#include "XPMEditor.h"
#include <sdk.h>

/** Constructor. Does nothing specific
  */
XPMEditorLogger::XPMEditorLogger(bool fixedPitchFont)
{

}

/** Destructor. Does nothing specific
  */
XPMEditorLogger::~XPMEditorLogger()
{

}

/** This is the method used to catch settings change
  */
void XPMEditorLogger::UpdateSettings()
{
    if (XPMEditor::Get()) XPMEditor::Get()->UpdateConfiguration();
}

/** This method does nothing. It is here only to avoid pure virtual method
  */
void XPMEditorLogger::Append(const wxString& msg, Logger::level lv)
{

}

/** This method does nothing. It is here only to avoid pure virtual method
  */
void XPMEditorLogger::CopyContentsToClipboard(bool selectionOnly)
{

}

/** This method does nothing. It is here only to avoid pure virtual method
  */
void XPMEditorLogger::Clear()
{

}

/** This method does nothing. It is here only to avoid pure virtual method
  */
//wxWindow* XPMEditorLogger::CreateControl(wxWindow* parent)
//{
//
//}
