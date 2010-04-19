#ifndef WXMIRROR_H
#define WXMIRROR_H

//(*Headers(wxMirror)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class wxMirror: public wxDialog
{
	public:

		wxMirror(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxMirror();

		//(*Declarations(wxMirror)
		wxRadioButton* RadioButton1;
		wxRadioButton* RadioButton2;
		wxButton* Button1;
		wxRadioButton* RadioButton4;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxRadioButton* RadioButton3;
		//*)

	protected:

		//(*Identifiers(wxMirror)
		static const long ID_STATICTEXT1;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON4;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(wxMirror)
		void OnCancelButton(wxCommandEvent& event);
		void OnButtonOKClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
