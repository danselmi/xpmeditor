#include "wxRotateHue.h"

//(*InternalHeaders(wxRotateHue)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxRotateHue)
const long wxRotateHue::ID_STATICTEXT1 = wxNewId();
const long wxRotateHue::ID_STATICTEXT2 = wxNewId();
const long wxRotateHue::ID_SPINCTRL1 = wxNewId();
const long wxRotateHue::ID_RADIOBUTTON3 = wxNewId();
const long wxRotateHue::ID_RADIOBUTTON4 = wxNewId();
const long wxRotateHue::ID_BUTTON1 = wxNewId();
const long wxRotateHue::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxRotateHue,wxDialog)
	//(*EventTable(wxRotateHue)
	//*)
END_EVENT_TABLE()

wxRotateHue::wxRotateHue(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxRotateHue)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, id, _("ROTATE HUE"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Please select the \"angle\" of the Hue rotation: from -360 to 360 degrees."), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Hue rotation:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer4->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxSize(72,21), 0, -360, 360, 0, _T("ID_SPINCTRL1"));
	SpinCtrl1->SetValue(_T("0"));
	BoxSizer4->Add(SpinCtrl1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer3->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	RadioButton3 = new wxRadioButton(this, ID_RADIOBUTTON3, _("Image"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	RadioButton3->SetValue(true);
	BoxSizer5->Add(RadioButton3, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	RadioButton4 = new wxRadioButton(this, ID_RADIOBUTTON4, _("Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	BoxSizer5->Add(RadioButton4, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button2 = new wxButton(this, ID_BUTTON2, _("CANCEL"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer2->Add(Button2, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxRotateHue::OnButtonOKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxRotateHue::OnButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxRotateHue::OnClose);
	//*)
}

wxRotateHue::~wxRotateHue()
{
	//(*Destroy(wxRotateHue)
	//*)
}


void wxRotateHue::OnButtonOKClick(wxCommandEvent& event)
{
    EndModal(0);
}

void wxRotateHue::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(1);
}

void wxRotateHue::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}
