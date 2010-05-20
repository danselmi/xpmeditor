/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxRotate.h
 * Purpose:   A dialog box for rotating the Image or the selection  - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/
#ifndef WXROTATE_H
#define WXROTATE_H

//(*Headers(wxRotate)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class wxRotate: public wxDialog
{
	public:

		wxRotate(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxRotate();

		//(*Declarations(wxRotate)
		wxRadioButton* RadioButton1;
		wxSpinCtrl* SpinCtrl1;
		wxStaticText* StaticText2;
		wxRadioButton* RadioButton2;
		wxRadioButton* RadioButton6;
		wxRadioButton* RadioButton5;
		wxButton* Button1;
		wxRadioButton* RadioButton4;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxRadioButton* RadioButton3;
		//*)

	protected:

		//(*Identifiers(wxRotate)
		static const long ID_STATICTEXT1;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_RADIOBUTTON5;
		static const long ID_RADIOBUTTON6;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL1;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON4;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(wxRotate)
		void OnRadioButton6Select(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnButtonOKClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
