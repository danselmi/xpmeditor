#include "AdvancedOptions.h"

//(*InternalHeaders(AdvancedOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AdvancedOptions)
//*)

BEGIN_EVENT_TABLE(AdvancedOptions,wxDialog)
	//(*EventTable(AdvancedOptions)
	//*)
END_EVENT_TABLE()

AdvancedOptions::AdvancedOptions(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
}

void AdvancedOptions::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AdvancedOptions)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
}

AdvancedOptions::~AdvancedOptions()
{
	//(*Destroy(AdvancedOptions)
	//*)
}

