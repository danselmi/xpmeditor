#ifndef COLOURPICKERPANEL_H
#define COLOURPICKERPANEL_H

//(*Headers(ColourPickerPanel)
#include <wx/sizer.h>
#include "XPMColorPicker.h"
#include <wx/panel.h>
//*)

class ColourPickerPanel: public wxPanel
{
	public:

		ColourPickerPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ColourPickerPanel();

		//(*Declarations(ColourPickerPanel)
		XPMColorPicker* ColourPicker;
		//*)

	protected:

		//(*Identifiers(ColourPickerPanel)
		static const long ID_CUSTOM1;
		//*)

	private:

		//(*Handlers(ColourPickerPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
