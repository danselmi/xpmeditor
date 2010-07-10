/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      QuickHelpPanel.cpp
 * Purpose:   A panel containing a simple manual for the plugin  - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "QuickHelpPanel.h"

//(*InternalHeaders(QuickHelpPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(QuickHelpPanel)
const long QuickHelpPanel::ID_PANEL1 = wxNewId();
const long QuickHelpPanel::ID_PANEL7 = wxNewId();
const long QuickHelpPanel::ID_PANEL2 = wxNewId();
const long QuickHelpPanel::ID_PANEL3 = wxNewId();
const long QuickHelpPanel::ID_PANEL4 = wxNewId();
const long QuickHelpPanel::ID_PANEL5 = wxNewId();
const long QuickHelpPanel::ID_PANEL6 = wxNewId();
const long QuickHelpPanel::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(QuickHelpPanel,wxPanel)
	//(*EventTable(QuickHelpPanel)
	//*)
END_EVENT_TABLE()

QuickHelpPanel::QuickHelpPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(QuickHelpPanel)
	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(364,242), 0, _T("ID_NOTEBOOK1"));
	Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel7 = new wxPanel(Notebook1, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
	Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	Panel5 = new wxPanel(Notebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	Panel6 = new wxPanel(Notebook1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	Notebook1->AddPage(Panel1, _("Introduction"), false);
	Notebook1->AddPage(Panel7, _("Interface"), false);
	Notebook1->AddPage(Panel2, _("Image"), false);
	Notebook1->AddPage(Panel3, _("Colour"), false);
	Notebook1->AddPage(Panel4, _("Tools"), false);
	Notebook1->AddPage(Panel5, _("Effects"), false);
	Notebook1->AddPage(Panel6, _("Colour"), false);
	BoxSizer1->Add(Notebook1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

QuickHelpPanel::~QuickHelpPanel()
{
	//(*Destroy(QuickHelpPanel)
	//*)
}

