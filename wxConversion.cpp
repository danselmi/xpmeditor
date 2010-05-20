/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxConvertion.cpp
 * Purpose:   A dialog box for converting the Image or the selection to GrayScale or Monochrome - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "wxConversion.h"

//(*InternalHeaders(wxConversion)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxConversion)
const long wxConversion::ID_STATICTEXT1 = wxNewId();
const long wxConversion::ID_RADIOBUTTON1 = wxNewId();
const long wxConversion::ID_RADIOBUTTON2 = wxNewId();
const long wxConversion::ID_RADIOBUTTON3 = wxNewId();
const long wxConversion::ID_RADIOBUTTON4 = wxNewId();
const long wxConversion::ID_BUTTON1 = wxNewId();
const long wxConversion::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxConversion,wxDialog)
	//(*EventTable(wxConversion)
	//*)
END_EVENT_TABLE()

wxConversion::wxConversion(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxConversion)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, id, _("COLOUR CONVERSION"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Select the type of colour conversion: \nto monochrome or to greyscale."), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	RadioButton1 = new wxRadioButton(this, ID_RADIOBUTTON1, _("Monochrome"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	RadioButton1->SetValue(true);
	BoxSizer4->Add(RadioButton1, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	RadioButton2 = new wxRadioButton(this, ID_RADIOBUTTON2, _("Grey Scale"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	BoxSizer4->Add(RadioButton2, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
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
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxConversion::OnButtonOKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxConversion::OnButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxConversion::OnClose);
	//*)
}

wxConversion::~wxConversion()
{
	//(*Destroy(wxConversion)
	//*)
}


void wxConversion::OnButtonOKClick(wxCommandEvent& event)
{
    EndModal(0);
}

void wxConversion::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(1);
}

void wxConversion::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}
