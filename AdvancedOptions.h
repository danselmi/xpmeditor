#ifndef ADVANCEDOPTIONS_H
#define ADVANCEDOPTIONS_H

//(*Headers(AdvancedOptions)
#include <wx/dialog.h>
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
		//*)

	protected:

		//(*Identifiers(AdvancedOptions)
		//*)

		//(*Handlers(AdvancedOptions)
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
