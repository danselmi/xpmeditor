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
#include "XPMEditorPanel.h"
#include "QuickHelpDialog.h"

#define ID_FIRST_HELP 44

//(*InternalHeaders(XPMHelpPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMHelpPanel)
const long XPMHelpPanel::ID_BUTTON1 = wxNewId();
const long XPMHelpPanel::ID_TEXTCTRL1 = wxNewId();
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
	BoxSizer1->Add(Button1, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(100,126), wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer1->Add(TextCtrl1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMHelpPanel::OnHelpButtonClick);
	//*)


	//help handlers
	Button1->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMHelpPanel::OnHelpButtonMouseEnter,0,this);
	Button1->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMHelpPanel::OnWidgetsMouseLeave,0,this);
	TextCtrl1->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMHelpPanel::OnHelpDisplayMouseEnter,0,this);
	TextCtrl1->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMHelpPanel::OnWidgetsMouseLeave,0,this);

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

/** HELP HANDLERS **/
void XPMHelpPanel::OnWidgetsMouseLeave(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(-1);
    }

    event.Skip();
}

void XPMHelpPanel::OnHelpButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP);
    }

    event.Skip();
}

void XPMHelpPanel::OnHelpDisplayMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 1);
    }

    event.Skip();
}
