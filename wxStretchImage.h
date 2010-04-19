#ifndef WXSTRETCHIMAGE_H
#define WXSTRETCHIMAGE_H

//(*Headers(wxStretchImage)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class wxStretchImage: public wxDialog
{
	public:

		wxStretchImage(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxStretchImage();

		wxSize sSelection;
        wxSize sImage;

		//(*Declarations(wxStretchImage)
		wxRadioButton* RadioButton1;
		wxSpinCtrl* SpinCtrl1;
		wxStaticText* StaticText2;
		wxRadioButton* RadioButton2;
		wxButton* Button1;
		wxRadioButton* RadioButton4;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* Button2;
		wxRadioButton* RadioButton3;
		wxStaticText* StaticText5;
		wxSpinCtrl* SpinCtrl2;
		wxTextCtrl* TextCtrl2;
		wxTextCtrl* TextCtrl1;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(wxStretchImage)
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT4;
		static const long ID_SPINCTRL2;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON4;
		static const long ID_STATICTEXT5;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(wxStretchImage)
		void OnCancelButtonClick(wxCommandEvent& event);
		void OnOKButtonClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnRadioButton1Select(wxCommandEvent& event);
		void OnRadioButton2Select(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
