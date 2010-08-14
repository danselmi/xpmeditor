/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      QuickHelpDialog.cpp
 * Purpose:   a dialog box for displaying help - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "QuickHelpDialog.h"

//(*InternalHeaders(QuickHelpDialog)
#include <wx/sizer.h>
#include "QuickHelpPanel.h"
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(QuickHelpDialog)
const long QuickHelpDialog::ID_QUICKHELPPANEL = wxNewId();
const long QuickHelpDialog::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(QuickHelpDialog,wxScrollingDialog)
	//(*EventTable(QuickHelpDialog)
	//*)
END_EVENT_TABLE()

QuickHelpDialog::QuickHelpDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
}

void QuickHelpDialog::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(QuickHelpDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, _("Help on XPMEditor"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	QuickHelpPanel1 = new QuickHelpPanel(this);
	BoxSizer1->Add(QuickHelpPanel1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button1 = new wxButton(this, ID_BUTTON1, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&QuickHelpDialog::OnButtonCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&QuickHelpDialog::OnClose);
	//*)
}

QuickHelpDialog::~QuickHelpDialog()
{
	//(*Destroy(QuickHelpDialog)
	//*)
}


void QuickHelpDialog::OnButtonCloseClick(wxCommandEvent& event)
{
    //this is not a modal window (to allow the user to read the help while working with the editor)
    //therefore, we call Destroy(), and not EndModal()
    Destroy();
}

void QuickHelpDialog::OnClose(wxCloseEvent& event)
{
    //this is not a modal window (to allow the user to read the help while working with the editor)
    //therefore, we call Destroy(), and not EndModal()
    Destroy();
}
