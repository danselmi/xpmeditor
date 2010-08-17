/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxInvertDialog.h
 * Purpose:   A dialog box for inverting the colours the Image or the selection  - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef WXINVERTDIALOG_H
#define WXINVERTDIALOG_H

//(*Headers(wxInvertDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class wxInvertDialog: public wxDialog
{
	public:

		wxInvertDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxInvertDialog();

		//(*Declarations(wxInvertDialog)
		wxButton* Button1;
		wxRadioButton* RadioButton4;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxRadioButton* RadioButton3;
		//*)

	protected:

		//(*Identifiers(wxInvertDialog)
		static const long ID_STATICTEXT1;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON3;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(wxInvertDialog)
		void OnClose(wxCloseEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnButtonOKClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
