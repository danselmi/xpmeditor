#ifndef ADVANCEDOPTIONS_H
#define ADVANCEDOPTIONS_H

//(*Headers(AdvancedOptions)
#include <wx/dialog.h>
class wxSpinEvent;
class wxStaticText;
class wxSlider;
class wxSpinCtrl;
class wxBoxSizer;
class wxButton;
class wxStaticBoxSizer;
class wxChoice;
//*)

/*
#define PNG_NO_FILTERS     0x00
#define PNG_FILTER_NONE    0x08
#define PNG_FILTER_SUB     0x10
#define PNG_FILTER_UP      0x20
#define PNG_FILTER_AVG     0x40
#define PNG_FILTER_PAETH   0x80
#define PNG_ALL_FILTERS (PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_UP | \
                         PNG_FILTER_AVG | PNG_FILTER_PAETH)
*/

class AdvancedOptions: public wxDialog
{
	public:

		AdvancedOptions(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AdvancedOptions();

		//(*Declarations(AdvancedOptions)
		wxSlider* Slider1;
		wxButton* OKButton;
		wxSpinCtrl* SpinCtrl1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxStaticText* StaticText5;
		wxSpinCtrl* SpinCtrl2;
		wxStaticText* StaticText4;
		wxButton* CancelButton;
		wxChoice* Choice1;
		//*)

	protected:

		//(*Identifiers(AdvancedOptions)
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_CHOICE1;
		static const long ID_STATICTEXT4;
		static const long ID_SLIDER1;
		static const long ID_STATICTEXT5;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

		//(*Handlers(AdvancedOptions)
		void OnClose(wxCloseEvent& event);
		void OnOKButtonClick(wxCommandEvent& event);
		void OnCancelButtonClick(wxCommandEvent& event);
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
