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
#include <wx/html/htmlwin.h>
//*)

class QuickHelpPanel: public wxPanel
{
	public:

		QuickHelpPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~QuickHelpPanel();

		//(*Declarations(QuickHelpPanel)
		wxPanel* Panel5;
		wxHtmlWindow* HtmlWindowImage;
		wxNotebook* Notebook1;
		wxPanel* Panel4;
		wxHtmlWindow* HtmlWindowEffects;
		wxPanel* Panel1;
		wxHtmlWindow* HtmlWindowColour;
		wxPanel* Panel3;
		wxHtmlWindow* HtmlWindowTools;
		wxPanel* Panel7;
		wxHtmlWindow* HtmlWindowInterface;
		wxPanel* Panel2;
		wxHtmlWindow* HtmlWindowIntro;
		//*)

	protected:

		//(*Identifiers(QuickHelpPanel)
		static const long ID_HTMLWINDOW1;
		static const long ID_PANEL1;
		static const long ID_HTMLWINDOW6;
		static const long ID_PANEL7;
		static const long ID_HTMLWINDOW5;
		static const long ID_PANEL2;
		static const long ID_HTMLWINDOW4;
		static const long ID_PANEL3;
		static const long ID_HTMLWINDOW3;
		static const long ID_PANEL4;
		static const long ID_HTMLWINDOW2;
		static const long ID_PANEL5;
		static const long ID_NOTEBOOK1;
		//*)

	private:

		//(*Handlers(QuickHelpPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
