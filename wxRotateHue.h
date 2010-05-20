/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxRotateHue.h
 * Purpose:   A dialog box for converting the colours in the Image or the selection  - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef WXROTATEHUE_H
#define WXROTATEHUE_H

//(*Headers(wxRotateHue)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/clrpicker.h>
//*)

class wxRotateHue: public wxDialog
{
	public:

		wxRotateHue(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxRotateHue();

		//(*Declarations(wxRotateHue)
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxColourPickerCtrl* PickerTo;
		wxRadioButton* RadioButton4;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* Button2;
		wxRadioButton* RadioButton3;
		wxColourPickerCtrl* PickerFrom;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(wxRotateHue)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_CUSTOM3;
		static const long ID_STATICTEXT4;
		static const long ID_CUSTOM1;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON4;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(wxRotateHue)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
