/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMCustomZoom.h
 * Purpose:   Dialog Box for Custom zoom factor - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPM_CUSTOMZOOM_H
#define XPM_CUSTOMZOOM_H

//(*Headers(XPMCustomZoom)
#include <wx/dialog.h>
class wxTextCtrl;
class wxStaticText;
class wxBoxSizer;
class wxButton;
//*)

class XPMCustomZoom: public wxDialog
{
	public:

		XPMCustomZoom(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMCustomZoom();

		//(*Declarations(XPMCustomZoom)
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxTextCtrl* TextCtrl1;
		//*)

	protected:

		//(*Identifiers(XPMCustomZoom)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

		//(*Handlers(XPMCustomZoom)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
