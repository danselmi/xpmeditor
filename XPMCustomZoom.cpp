/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMCustomZoom.cpp
 * Purpose:   Dialog Box for Custom zoom factor - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "XPMCustomZoom.h"

//(*InternalHeaders(XPMCustomZoom)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//#include <wx/msgdlg.h>
#include <wx/valtext.h>
#include "XPMEditorPanel.h"

//(*IdInit(XPMCustomZoom)
const long XPMCustomZoom::ID_STATICTEXT1 = wxNewId();
const long XPMCustomZoom::ID_TEXTCTRL1 = wxNewId();
const long XPMCustomZoom::ID_STATICTEXT2 = wxNewId();
const long XPMCustomZoom::ID_BUTTON1 = wxNewId();
const long XPMCustomZoom::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMCustomZoom,wxDialog)
	//(*EventTable(XPMCustomZoom)
	//*)
END_EVENT_TABLE()

XPMCustomZoom::XPMCustomZoom(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
}

void XPMCustomZoom::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMCustomZoom)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, id, _("Custom Zoom Factor"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Zoom Factor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer2->Add(TextCtrl1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("%"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer2->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer3->Add(Button1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button2 = new wxButton(this, ID_BUTTON2, _("CANCEL"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer3->Add(Button2, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMCustomZoom::OnButtonOKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMCustomZoom::OnButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&XPMCustomZoom::OnClose);
	//*)

	wxTextValidator tvOnlyNumeric(wxFILTER_NUMERIC);
	TextCtrl1->SetValidator(tvOnlyNumeric);
}

XPMCustomZoom::~XPMCustomZoom()
{
	//(*Destroy(XPMCustomZoom)
	//*)
}


void XPMCustomZoom::OnButtonOKClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;
    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        wxString s;
        double lValue;
        double dValue;

        //wxMessageBox(_("parent OK"), _("INFO"), wxOK);

        s = TextCtrl1->GetValue();
        if (s.ToDouble(&lValue))
        {
            dValue = lValue / 100;
            if (dValue < 0) dValue = -dValue;
            //wxMessageBox(wxString::Format(_("value=%f"), dValue), _("INFO"), wxOK);
            parent->SetScaleFactor(dValue);
            EndModal(1);
        }
    }
    EndModal(0);
}

void XPMCustomZoom::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(0);
}

void XPMCustomZoom::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}
