/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      QuickHelpPanel.cpp
 * Purpose:   A panel containing a simple manual for the plugin  - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMEditor.h"
#include "QuickHelpPanel.h"

//(*InternalHeaders(QuickHelpPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(QuickHelpPanel)
const long QuickHelpPanel::ID_HTMLWINDOW1 = wxNewId();
const long QuickHelpPanel::ID_PANEL1 = wxNewId();
const long QuickHelpPanel::ID_HTMLWINDOW6 = wxNewId();
const long QuickHelpPanel::ID_PANEL7 = wxNewId();
const long QuickHelpPanel::ID_HTMLWINDOW5 = wxNewId();
const long QuickHelpPanel::ID_PANEL2 = wxNewId();
const long QuickHelpPanel::ID_HTMLWINDOW4 = wxNewId();
const long QuickHelpPanel::ID_PANEL3 = wxNewId();
const long QuickHelpPanel::ID_HTMLWINDOW3 = wxNewId();
const long QuickHelpPanel::ID_PANEL4 = wxNewId();
const long QuickHelpPanel::ID_HTMLWINDOW2 = wxNewId();
const long QuickHelpPanel::ID_PANEL5 = wxNewId();
const long QuickHelpPanel::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(QuickHelpPanel,wxPanel)
	//(*EventTable(QuickHelpPanel)
	//*)
END_EVENT_TABLE()

QuickHelpPanel::QuickHelpPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(QuickHelpPanel)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(642,410), 0, _T("ID_NOTEBOOK1"));
	Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxSize(425,298), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	HtmlWindowIntro = new wxHtmlWindow(Panel1, ID_HTMLWINDOW1, wxDefaultPosition, wxSize(560,406), wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER, _T("ID_HTMLWINDOW1"));
	BoxSizer2->Add(HtmlWindowIntro, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Panel1->SetSizer(BoxSizer2);
	BoxSizer2->SetSizeHints(Panel1);
	Panel7 = new wxPanel(Notebook1, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	HtmlWindowInterface = new wxHtmlWindow(Panel7, ID_HTMLWINDOW6, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER, _T("ID_HTMLWINDOW6"));
	BoxSizer3->Add(HtmlWindowInterface, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Panel7->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel7);
	BoxSizer3->SetSizeHints(Panel7);
	Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	HtmlWindowImage = new wxHtmlWindow(Panel2, ID_HTMLWINDOW5, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER, _T("ID_HTMLWINDOW5"));
	BoxSizer4->Add(HtmlWindowImage, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Panel2->SetSizer(BoxSizer4);
	BoxSizer4->Fit(Panel2);
	BoxSizer4->SetSizeHints(Panel2);
	Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	HtmlWindowColour = new wxHtmlWindow(Panel3, ID_HTMLWINDOW4, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER, _T("ID_HTMLWINDOW4"));
	BoxSizer5->Add(HtmlWindowColour, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Panel3->SetSizer(BoxSizer5);
	BoxSizer5->Fit(Panel3);
	BoxSizer5->SetSizeHints(Panel3);
	Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	HtmlWindowTools = new wxHtmlWindow(Panel4, ID_HTMLWINDOW3, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER, _T("ID_HTMLWINDOW3"));
	BoxSizer6->Add(HtmlWindowTools, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Panel4->SetSizer(BoxSizer6);
	BoxSizer6->Fit(Panel4);
	BoxSizer6->SetSizeHints(Panel4);
	Panel5 = new wxPanel(Notebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	HtmlWindowEffects = new wxHtmlWindow(Panel5, ID_HTMLWINDOW2, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER, _T("ID_HTMLWINDOW2"));
	BoxSizer7->Add(HtmlWindowEffects, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Panel5->SetSizer(BoxSizer7);
	BoxSizer7->Fit(Panel5);
	BoxSizer7->SetSizeHints(Panel5);
	Notebook1->AddPage(Panel1, _("Introduction"), false);
	Notebook1->AddPage(Panel7, _("Interface"), false);
	Notebook1->AddPage(Panel2, _("Image"), false);
	Notebook1->AddPage(Panel3, _("Colour"), false);
	Notebook1->AddPage(Panel4, _("Tools"), false);
	Notebook1->AddPage(Panel5, _("Effects"), false);
	BoxSizer1->Add(Notebook1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)

	//load the HTML files
	wxString sResourcesPath;
	if (XPM_Plugin())
	{
	    sResourcesPath = XPM_Plugin()->GetHTMLHelp();
	    if (sResourcesPath.Len() > 0)
	    {
            //pages.zip#zip:test.htm
            if (HtmlWindowIntro) HtmlWindowIntro->LoadPage(sResourcesPath + _("#zip:doc/intro.htm"));
            if (HtmlWindowInterface) HtmlWindowInterface->LoadPage(sResourcesPath + _("#zip:doc/interface.htm"));
            if (HtmlWindowImage) HtmlWindowImage->LoadPage(sResourcesPath + _("#zip:doc/image.htm"));
            if (HtmlWindowColour) HtmlWindowColour->LoadPage(sResourcesPath + _("#zip:doc/colours.htm"));
            if (HtmlWindowTools) HtmlWindowTools->LoadPage(sResourcesPath + _("#zip:doc/tools.htm"));
            if (HtmlWindowEffects) HtmlWindowEffects->LoadPage(sResourcesPath + _("#zip:doc/effects.htm"));
	    }
	}
}

QuickHelpPanel::~QuickHelpPanel()
{
	//(*Destroy(QuickHelpPanel)
	//*)
}

