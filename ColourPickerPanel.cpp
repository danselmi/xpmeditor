#include "ColourPickerPanel.h"

//(*InternalHeaders(ColourPickerPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ColourPickerPanel)
const long ColourPickerPanel::ID_CUSTOM1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ColourPickerPanel,wxPanel)
	//(*EventTable(ColourPickerPanel)
	//*)
END_EVENT_TABLE()

ColourPickerPanel::ColourPickerPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ColourPickerPanel)
	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	ColourPicker = new XPMColorPicker(this,ID_CUSTOM1,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_CUSTOM1"));
	BoxSizer1->Add(ColourPicker, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

ColourPickerPanel::~ColourPickerPanel()
{
	//(*Destroy(ColourPickerPanel)
	//*)
}

