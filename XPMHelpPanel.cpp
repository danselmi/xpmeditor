/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMHelpPanel.cpp
 * Purpose:   A panel containing basic help - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMHelpPanel.h"
#include "QuickHelpDialog.h"

//(*InternalHeaders(XPMHelpPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMHelpPanel)
const long XPMHelpPanel::ID_BUTTON1 = wxNewId();
const long XPMHelpPanel::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMHelpPanel,wxPanel)
	//(*EventTable(XPMHelpPanel)
	//*)
END_EVENT_TABLE()

XPMHelpPanel::XPMHelpPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
}

void XPMHelpPanel::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMHelpPanel)
	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("HELP"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer1->Add(Button1, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("..."), wxDefaultPosition, wxSize(60,199), wxST_NO_AUTORESIZE|wxSUNKEN_BORDER, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMHelpPanel::OnHelpButtonClick);
	//*)

	m_parent = NULL;
}

XPMHelpPanel::~XPMHelpPanel()
{
	//(*Destroy(XPMHelpPanel)
	//*)
}

/** Set the parent panel
  * \param [in] a pointer to the new parent
  */
void XPMHelpPanel::SetParentPanel(XPMEditorPanel *p)
{
    m_parent = p;
}


void XPMHelpPanel::OnHelpButtonClick(wxCommandEvent& event)
{
    QuickHelpDialog *qhd;

    qhd = new QuickHelpDialog((wxWindow *) m_parent);
    if (qhd) qhd->Show(true);
}
