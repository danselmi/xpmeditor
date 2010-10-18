/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      QuickHelpDialog.h
 * Purpose:   a dialog box for displaying help - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#ifndef QUICKHELPDIALOG_H
#define QUICKHELPDIALOG_H

//(*Headers(QuickHelpDialog)
#include "scrollingdialog.h"
class QuickHelpPanel;
class wxBoxSizer;
class wxButton;
//*)

class QuickHelpDialog: public wxScrollingDialog
{
	public:

		QuickHelpDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~QuickHelpDialog();

		//(*Declarations(QuickHelpDialog)
		wxButton* Button1;
		QuickHelpPanel* QuickHelpPanel1;
		//*)

	protected:

		//(*Identifiers(QuickHelpDialog)
		static const long ID_QUICKHELPPANEL;
		static const long ID_BUTTON1;
		//*)

		//(*Handlers(QuickHelpDialog)
		void OnButtonCloseClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
