#ifndef WXROTATEHUE_H
#define WXROTATEHUE_H

//(*Headers(wxRotateHue)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class wxRotateHue: public wxDialog
{
	public:

		wxRotateHue(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxRotateHue();

		//(*Declarations(wxRotateHue)
		wxSpinCtrl* SpinCtrl1;
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxRadioButton* RadioButton4;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxRadioButton* RadioButton3;
		//*)

	protected:

		//(*Identifiers(wxRotateHue)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL1;
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
