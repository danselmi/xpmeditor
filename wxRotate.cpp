#include "wxRotate.h"
//(*InternalHeaders(wxRotate)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxRotate)
const long wxRotate::ID_STATICTEXT1 = wxNewId();
const long wxRotate::ID_RADIOBUTTON1 = wxNewId();
const long wxRotate::ID_RADIOBUTTON2 = wxNewId();
const long wxRotate::ID_RADIOBUTTON5 = wxNewId();
const long wxRotate::ID_RADIOBUTTON6 = wxNewId();
const long wxRotate::ID_STATICTEXT2 = wxNewId();
const long wxRotate::ID_SPINCTRL1 = wxNewId();
const long wxRotate::ID_RADIOBUTTON3 = wxNewId();
const long wxRotate::ID_RADIOBUTTON4 = wxNewId();
const long wxRotate::ID_BUTTON1 = wxNewId();
const long wxRotate::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxRotate,wxDialog)
	//(*EventTable(wxRotate)
	//*)
END_EVENT_TABLE()

wxRotate::wxRotate(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	wxString s1(_("90"));
    wxString s2(_("180"));
    wxString s3(_("270"));
    s1 = s1 + wxString::From8BitData("°");
    s2 = s2 + wxString::From8BitData("°");
    s3 = s3 + wxString::From8BitData("°");

	//(*Initialize(wxRotate)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Please select the rotation direction (clockwise)."), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	RadioButton1 = new wxRadioButton(this, ID_RADIOBUTTON1, _("90"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	RadioButton1->SetValue(true);
	BoxSizer7->Add(RadioButton1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	RadioButton2 = new wxRadioButton(this, ID_RADIOBUTTON2, _("180"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	BoxSizer7->Add(RadioButton2, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer4->Add(BoxSizer7, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	RadioButton5 = new wxRadioButton(this, ID_RADIOBUTTON5, _("270"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	BoxSizer8->Add(RadioButton5, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	RadioButton6 = new wxRadioButton(this, ID_RADIOBUTTON6, _("Other:"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
	BoxSizer8->Add(RadioButton6, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer4->Add(BoxSizer8, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Angle:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText2->Disable();
	BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 360, 1, _T("ID_SPINCTRL1"));
	SpinCtrl1->SetValue(_T("1"));
	SpinCtrl1->Disable();
	BoxSizer6->Add(SpinCtrl1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer4->Add(BoxSizer6, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer3->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	RadioButton3 = new wxRadioButton(this, ID_RADIOBUTTON3, _("Image"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	RadioButton3->SetValue(true);
	BoxSizer5->Add(RadioButton3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	RadioButton4 = new wxRadioButton(this, ID_RADIOBUTTON4, _("Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	BoxSizer5->Add(RadioButton4, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer5->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button2 = new wxButton(this, ID_BUTTON2, _("CANCEL"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer2->Add(Button2, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_RADIOBUTTON6,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&wxRotate::OnRadioButton6Select);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxRotate::OnButtonOKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxRotate::OnButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxRotate::OnClose);
	//*)
	RadioButton1->SetLabel(s1);
	RadioButton2->SetLabel(s2);
	RadioButton5->SetLabel(s3);
}

wxRotate::~wxRotate()
{
	//(*Destroy(wxRotate)
	//*)
}


void wxRotate::OnRadioButton6Select(wxCommandEvent& event)
{
    StaticText2->Enable(event.IsChecked());
    SpinCtrl1->Enable(event.IsChecked());
}

void wxRotate::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(1);
}

void wxRotate::OnButtonOKClick(wxCommandEvent& event)
{
    EndModal(0);
}

void wxRotate::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}
