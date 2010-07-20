#ifndef ADVANCEDOPTIONS_H
#define ADVANCEDOPTIONS_H

//(*Headers(AdvancedOptions)
#include <wx/dialog.h>
class wxSpinEvent;
class wxCheckBox;
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

		void EnableResolutionOption(bool bEnabled); ///< \brief activate / deactivate resolution widgets
		void EnableJPGOption(bool bEnabled);        ///< \brief activate / deactivate JPG quality widgets
		void EnablePNGFormat(bool bEnabled);        ///< \brief activate / deactivate PNG Format widgets
		void EnablePNGBitsDepth(bool bEnabled);     ///< \brief activate / deactivate PNG Bit Depth widgets

		//(*Declarations(AdvancedOptions)
		wxSlider* SliderJPGCompression;
		wxButton* OKButton;
		wxStaticText* StaticText2;
		wxSpinCtrl* SpinResolutionX;
		wxStaticText* StaticText6;
		wxChoice* ChoicePNGBitDepths;
		wxStaticText* StaticText8;
		wxCheckBox* CheckBoxJPGCompression;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxCheckBox* CheckBoxPNGFormat;
		wxCheckBox* CheckBoxResolutionOption;
		wxCheckBox* CheckBoxPNGBitDepth;
		wxSpinCtrl* SpinResolutionY;
		wxChoice* ChoiceResolutionUnit;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* StaticText4;
		wxButton* CancelButton;
		wxChoice* ChoicePNGFormat;
		//*)

	protected:

		//(*Identifiers(AdvancedOptions)
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_CHOICE1;
		static const long ID_CHECKBOX1;
		static const long ID_STATICTEXT4;
		static const long ID_SLIDER1;
		static const long ID_STATICTEXT5;
		static const long ID_CHECKBOX2;
		static const long ID_STATICTEXT6;
		static const long ID_CHOICE2;
		static const long ID_CHECKBOX3;
		static const long ID_STATICTEXT8;
		static const long ID_CHOICE5;
		static const long ID_CHECKBOX4;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

		//(*Handlers(AdvancedOptions)
		void OnClose(wxCloseEvent& event);
		void OnOKButtonClick(wxCommandEvent& event);
		void OnCancelButtonClick(wxCommandEvent& event);
		void OnCheckBoxResolutionOptionClick(wxCommandEvent& event);
		void OnCheckBoxJPGCompressionClick(wxCommandEvent& event);
		void OnCheckBoxPNGOptionsClick(wxCommandEvent& event);
		void OnCheckBoxPNGBitDepthClick(wxCommandEvent& event);
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
