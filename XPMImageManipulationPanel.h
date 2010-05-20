/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImageManipulationPanel.h
 * Purpose:   A panel containing tools for transforming the image or the selection - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#ifndef XPMIMAGEMANIPULATIONPANEL_H
#define XPMIMAGEMANIPULATIONPANEL_H

//(*Headers(XPMImageManipulationPanel)
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
//*)

class XPMImageManipulationPanel: public wxPanel
{
	public:

		XPMImageManipulationPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMImageManipulationPanel();

		//(*Declarations(XPMImageManipulationPanel)
		wxButton* Button4;
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button6;
		wxButton* Button5;
		wxButton* Button3;
		wxButton* Button7;
		//*)

	protected:

		//(*Identifiers(XPMImageManipulationPanel)
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON7;
		//*)

	private:

		//(*Handlers(XPMImageManipulationPanel)
		void OnButtonStretchClick(wxCommandEvent& event);
		void OnButtonMirrorClick(wxCommandEvent& event);
		void OnButtonBlurClick(wxCommandEvent& event);
		void OnButtonRotateClick(wxCommandEvent& event);
		void OnButtonHueClick(wxCommandEvent& event);
		void OnButtonColourDepthClick(wxCommandEvent& event);
		void OnButtonInvertClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
