#include "AdvancedOptions.h"

//(*InternalHeaders(AdvancedOptions)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(AdvancedOptions)
const long AdvancedOptions::ID_STATICTEXT1 = wxNewId();
const long AdvancedOptions::ID_SPINCTRL1 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT2 = wxNewId();
const long AdvancedOptions::ID_SPINCTRL2 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT3 = wxNewId();
const long AdvancedOptions::ID_CHOICE1 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT4 = wxNewId();
const long AdvancedOptions::ID_SLIDER1 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT5 = wxNewId();
const long AdvancedOptions::ID_BUTTONOK = wxNewId();
const long AdvancedOptions::ID_BUTTONCANCEL = wxNewId();
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
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Advanced Image Options"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("General"));
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Resolution X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer3->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl1->SetValue(_T("0"));
	BoxSizer3->Add(SpinCtrl1, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Resolution Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer4->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl2 = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl2->SetValue(_T("0"));
	BoxSizer4->Add(SpinCtrl2, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Resolution Unit:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer5->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	BoxSizer5->Add(Choice1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticBoxSizer1->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	BoxSizer1->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("JPG Quality"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Poor Quality"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer6->Add(StaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Slider1 = new wxSlider(this, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER1"));
	BoxSizer6->Add(Slider1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Good Quality"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer6->Add(StaticText5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer6, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("PNG Format Options"));
	BoxSizer1->Add(StaticBoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	OKButton = new wxButton(this, ID_BUTTONOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	BoxSizer2->Add(OKButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	CancelButton = new wxButton(this, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer2->Add(CancelButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnOKButtonClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnCancelButtonClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&AdvancedOptions::OnClose);
	//*)
}

AdvancedOptions::~AdvancedOptions()
{
	//(*Destroy(AdvancedOptions)
	//*)
}


void AdvancedOptions::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}

void AdvancedOptions::OnOKButtonClick(wxCommandEvent& event)
{
    EndModal(0);
}

void AdvancedOptions::OnCancelButtonClick(wxCommandEvent& event)
{
    EndModal(1);
}
