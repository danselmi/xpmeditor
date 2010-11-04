/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMHelpPanel.h
 * Purpose:   A panel containing basic help - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#ifndef XPMHELPPANEL_H
#define XPMHELPPANEL_H

//(*Headers(XPMHelpPanel)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
//*)

class XPMEditorPanel;

class XPMHelpPanel: public wxPanel
{
	public:

		XPMHelpPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMHelpPanel();
		void SetParentPanel(XPMEditorPanel *p); ///< \brief set the parent panel for this tool panel

		//(*Declarations(XPMHelpPanel)
		wxButton* Button1;
		wxTextCtrl* TextCtrl1;
		//*)

	protected:

		//(*Identifiers(XPMHelpPanel)
		static const long ID_BUTTON1;
		static const long ID_TEXTCTRL1;
		//*)

		//(*Handlers(XPMHelpPanel)
		void OnHelpButtonClick(wxCommandEvent& event);
		//*)

		//help handlers
		void OnWidgetsMouseLeave(wxMouseEvent& event);
		void OnHelpButtonMouseEnter(wxMouseEvent& event);
		void OnHelpDisplayMouseEnter(wxMouseEvent& event);

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		XPMEditorPanel *m_parent;

		DECLARE_EVENT_TABLE()
};

#endif
