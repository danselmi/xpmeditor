/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditorLogger.h
 * Purpose:   a fake logger for catching Settings update events - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-10-25
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef _XPM_EDITOR_LOGGER_H
#define _XPM_EDITOR_LOGGER_H

#include <loggers.h> //from CodeBlocks SDK

/** \class this Class has only 1 purpose: to catch CodeBlocks settings events;
  * This is a hack : there is currently no ways to know when the user has modified the settings
  * using the menu "Settings" / "Environments"
  * We are using this notification update in Codeblocks/src/main.cpp void MainFrame::OnSettingsEnvironment():
  *        Manager::Get()->GetLogManager()->NotifyUpdate();
  */

class XPMEditorLogger : public Logger
{
    public:
        XPMEditorLogger(bool fixedPitchFont = false);
        ~XPMEditorLogger();

        virtual void UpdateSettings();
        virtual void Append(const wxString& msg, Logger::level lv = info);
        virtual void CopyContentsToClipboard(bool selectionOnly = false);
        virtual void Clear();
        //virtual wxWindow* CreateControl(wxWindow* parent);
};

#endif
