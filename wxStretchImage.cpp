#include "wxStretchImage.h"

//(*InternalHeaders(wxStretchImage)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxStretchImage)
const long wxStretchImage::ID_STATICTEXT3 = wxNewId();
const long wxStretchImage::ID_TEXTCTRL1 = wxNewId();
const long wxStretchImage::ID_STATICTEXT2 = wxNewId();
const long wxStretchImage::ID_TEXTCTRL2 = wxNewId();
const long wxStretchImage::ID_STATICTEXT1 = wxNewId();
const long wxStretchImage::ID_SPINCTRL1 = wxNewId();
const long wxStretchImage::ID_STATICTEXT4 = wxNewId();
const long wxStretchImage::ID_SPINCTRL2 = wxNewId();
const long wxStretchImage::ID_RADIOBUTTON3 = wxNewId();
const long wxStretchImage::ID_RADIOBUTTON4 = wxNewId();
const long wxStretchImage::ID_STATICTEXT5 = wxNewId();
const long wxStretchImage::ID_RADIOBUTTON1 = wxNewId();
const long wxStretchImage::ID_RADIOBUTTON2 = wxNewId();
const long wxStretchImage::ID_BUTTON1 = wxNewId();
const long wxStretchImage::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxStretchImage,wxDialog)
	//(*EventTable(wxStretchImage)
	//*)
END_EVENT_TABLE()

wxStretchImage::wxStretchImage(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxStretchImage)
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, wxID_ANY, _("STRECTH IMAGE"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("OLD SIZE (pixels)"));
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Width:"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT3"));
	BoxSizer5->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer5->Add(TextCtrl1, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer5, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Height:"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	BoxSizer6->Add(TextCtrl2, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer6, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("NEW SIZE (pixels)"));
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Width:"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT1"));
	BoxSizer7->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 10000, 1, _T("ID_SPINCTRL1"));
	SpinCtrl1->SetValue(_T("1"));
	BoxSizer7->Add(SpinCtrl1, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer7, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer8->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl2 = new wxSpinCtrl(this, ID_SPINCTRL2, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 10000, 1, _T("ID_SPINCTRL2"));
	SpinCtrl2->SetValue(_T("1"));
	BoxSizer8->Add(SpinCtrl2, 2, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer8, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer2->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_TOP, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	RadioButton3 = new wxRadioButton(this, ID_RADIOBUTTON3, _("NORMAL QUALITY"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	RadioButton3->SetValue(true);
	BoxSizer10->Add(RadioButton3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	RadioButton4 = new wxRadioButton(this, ID_RADIOBUTTON4, _("HIGH QUALITY"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	BoxSizer10->Add(RadioButton4, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer9->Add(BoxSizer10, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Selecting \"HIGH QUALITY\" will interpolate the colours of the pixel.\nTherefore the Mask (transparent) information will be partially lost.\nTo avoid this problem, use the \"NORMAL QUALITY\" option."), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("ID_STATICTEXT5"));
	BoxSizer9->Add(StaticText5, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer9, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	RadioButton1 = new wxRadioButton(this, ID_RADIOBUTTON1, _("IMAGE"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	RadioButton1->SetValue(true);
	BoxSizer3->Add(RadioButton1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	RadioButton2 = new wxRadioButton(this, ID_RADIOBUTTON2, _("SELECTION"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	BoxSizer3->Add(RadioButton2, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer4->Add(Button1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer4->Add(-1,-1,1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	Button2 = new wxButton(this, ID_BUTTON2, _("CANCEL"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer4->Add(Button2, 1, wxALL|wxALIGN_RIGHT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&wxStretchImage::OnRadioButton1Select);
	Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&wxStretchImage::OnRadioButton2Select);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxStretchImage::OnOKButtonClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxStretchImage::OnCancelButtonClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxStretchImage::OnClose);
	//*)
}

wxStretchImage::~wxStretchImage()
{
	//(*Destroy(wxStretchImage)
	//*)
}


void wxStretchImage::OnCancelButtonClick(wxCommandEvent& event)
{
    EndModal(1);
}

void wxStretchImage::OnOKButtonClick(wxCommandEvent& event)
{
    EndModal(0);
}

void wxStretchImage::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}

void wxStretchImage::OnRadioButton1Select(wxCommandEvent& event)
{
    //"IMAGE" radio button selected
    TextCtrl1->SetValue(wxString::Format(_("%d"), sImage.GetWidth()));
    TextCtrl2->SetValue(wxString::Format(_("%d"), sImage.GetHeight()));
    SpinCtrl1->SetValue(sImage.GetWidth());
    SpinCtrl2->SetValue(sImage.GetHeight());
}

void wxStretchImage::OnRadioButton2Select(wxCommandEvent& event)
{
    //"SELECTION" radio button selected
    TextCtrl1->SetValue(wxString::Format(_("%d"), sSelection.GetWidth()));
    TextCtrl2->SetValue(wxString::Format(_("%d"), sSelection.GetHeight()));
    SpinCtrl1->SetValue(sSelection.GetWidth());
    SpinCtrl2->SetValue(sSelection.GetHeight());
}
