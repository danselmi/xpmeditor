/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      QuickHelpPanel.h
 * Purpose:   A panel containing a simple manual for the plugin  - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef QUICKHELPPANEL_H
#define QUICKHELPPANEL_H

//(*Headers(QuickHelpPanel)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/panel.h>
//*)

class QuickHelpPanel: public wxPanel
{
	public:

		QuickHelpPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~QuickHelpPanel();

		//(*Declarations(QuickHelpPanel)
		wxPanel* Panel5;
		wxNotebook* Notebook1;
		wxPanel* Panel4;
		wxPanel* Panel1;
		wxPanel* Panel6;
		wxPanel* Panel3;
		wxPanel* Panel7;
		wxPanel* Panel2;
		//*)

	protected:

		//(*Identifiers(QuickHelpPanel)
		static const long ID_PANEL1;
		static const long ID_PANEL7;
		static const long ID_PANEL2;
		static const long ID_PANEL3;
		static const long ID_PANEL4;
		static const long ID_PANEL5;
		static const long ID_PANEL6;
		static const long ID_NOTEBOOK1;
		//*)

	private:

		//(*Handlers(QuickHelpPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
