/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxBlur.cpp
 * Purpose:   A dialog box for blurring the Image or the selection - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "wxBlur.h"

//(*InternalHeaders(wxBlur)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxBlur)
const long wxBlur::ID_STATICTEXT1 = wxNewId();
const long wxBlur::ID_RADIOBUTTON1 = wxNewId();
const long wxBlur::ID_RADIOBUTTON2 = wxNewId();
const long wxBlur::ID_RADIOBUTTON5 = wxNewId();
const long wxBlur::ID_STATICTEXT2 = wxNewId();
const long wxBlur::ID_SPINCTRL1 = wxNewId();
const long wxBlur::ID_RADIOBUTTON3 = wxNewId();
const long wxBlur::ID_RADIOBUTTON4 = wxNewId();
const long wxBlur::ID_BUTTON1 = wxNewId();
const long wxBlur::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxBlur,wxDialog)
	//(*EventTable(wxBlur)
	//*)
END_EVENT_TABLE()

wxBlur::wxBlur(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxBlur)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, wxID_ANY, _("BLUR IMAGE"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Please select the blur orientation: horizontal or vertical, or both.\nWarning: blurring will modifythe pixels colour. \nTherefore, Mask (transparency) information may be partially lost."), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	RadioButton1 = new wxRadioButton(this, ID_RADIOBUTTON1, _("Horizontal Blur"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	RadioButton1->SetValue(true);
	BoxSizer4->Add(RadioButton1, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	RadioButton2 = new wxRadioButton(this, ID_RADIOBUTTON2, _("Vertical Blur"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	BoxSizer4->Add(RadioButton2, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	RadioButton5 = new wxRadioButton(this, ID_RADIOBUTTON5, _("Both direction"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	BoxSizer4->Add(RadioButton5, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Radius:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxDefaultPosition, wxSize(73,21), 0, 1, 10000, 1, _T("ID_SPINCTRL1"));
	SpinCtrl1->SetValue(_T("1"));
	BoxSizer6->Add(SpinCtrl1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer4->Add(BoxSizer6, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer3->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	RadioButton3 = new wxRadioButton(this, ID_RADIOBUTTON3, _("Image"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	RadioButton3->SetValue(true);
	BoxSizer5->Add(RadioButton3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	RadioButton4 = new wxRadioButton(this, ID_RADIOBUTTON4, _("Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	BoxSizer5->Add(RadioButton4, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer5->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
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
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxBlur::OnButtonOKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxBlur::OnButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxBlur::OnClose);
	//*)
}

wxBlur::~wxBlur()
{
	//(*Destroy(wxBlur)
	//*)
}


void wxBlur::OnButtonOKClick(wxCommandEvent& event)
{
    EndModal(0);
}

void wxBlur::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(1);
}

void wxBlur::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}
