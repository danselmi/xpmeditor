/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImageManipulationPanel.cpp
 * Purpose:   A panel containing tools for transforming the image or the selection - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/
#include "XPMImageManipulationPanel.h"
#include "XPMEditorPanel.h"

#define ID_FIRST_HELP 50

//(*InternalHeaders(XPMImageManipulationPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMImageManipulationPanel)
const long XPMImageManipulationPanel::ID_BUTTON1 = wxNewId();
const long XPMImageManipulationPanel::ID_BUTTON2 = wxNewId();
const long XPMImageManipulationPanel::ID_BUTTON3 = wxNewId();
const long XPMImageManipulationPanel::ID_BUTTON4 = wxNewId();
const long XPMImageManipulationPanel::ID_BUTTON5 = wxNewId();
const long XPMImageManipulationPanel::ID_BUTTON6 = wxNewId();
const long XPMImageManipulationPanel::ID_BUTTON7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMImageManipulationPanel,wxPanel)
	//(*EventTable(XPMImageManipulationPanel)
	//*)
END_EVENT_TABLE()

XPMImageManipulationPanel::XPMImageManipulationPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMImageManipulationPanel)
	wxBoxSizer* BoxSizer13;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer13 = new wxBoxSizer(wxVERTICAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("STRETCH"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer13->Add(Button1, 0, wxALL|wxEXPAND, 2);
	Button2 = new wxButton(this, ID_BUTTON2, _("MIRROR"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer13->Add(Button2, 0, wxALL|wxEXPAND, 2);
	Button3 = new wxButton(this, ID_BUTTON3, _("BLUR"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON3"));
	BoxSizer13->Add(Button3, 0, wxALL|wxEXPAND, 2);
	Button4 = new wxButton(this, ID_BUTTON4, _("ROTATE"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer13->Add(Button4, 0, wxALL|wxEXPAND, 2);
	Button5 = new wxButton(this, ID_BUTTON5, _("HUE"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON5"));
	BoxSizer13->Add(Button5, 0, wxALL|wxEXPAND, 2);
	Button6 = new wxButton(this, ID_BUTTON6, _("COLOUR DEPTH"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON6"));
	BoxSizer13->Add(Button6, 0, wxALL|wxEXPAND, 2);
	Button7 = new wxButton(this, ID_BUTTON7, _("INVERT"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON7"));
	BoxSizer13->Add(Button7, 0, wxALL|wxEXPAND, 2);
	BoxSizer13->Add(5,5,0, wxALL|wxEXPAND, 2);
	SetSizer(BoxSizer13);
	BoxSizer13->Fit(this);
	BoxSizer13->SetSizeHints(this);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonStretchClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonMirrorClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonBlurClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonRotateClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonHueClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonColourDepthClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImageManipulationPanel::OnButtonInvertClick);
	//*)

	//help handler
	Button1->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnStretchButtonMouseEnter,0,this);
	Button1->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);
	Button2->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnMirrorButtonMouseEnter,0,this);
	Button2->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);
	Button3->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnBlurButtonMouseEnter,0,this);
	Button3->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);
	Button4->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnRotateButtonMouseEnter,0,this);
	Button4->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);
	Button5->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnHueButtonMouseEnter,0,this);
	Button5->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);
	Button6->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnColourDepthButtonMouseEnter,0,this);
	Button6->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);
	Button7->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnInvertButtonMouseEnter,0,this);
	Button7->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMImageManipulationPanel::OnWidgetsMouseLeave,0,this);

	m_parent = NULL;
}

XPMImageManipulationPanel::~XPMImageManipulationPanel()
{
	//(*Destroy(XPMImageManipulationPanel)
	//*)
}

/** Set the parent panel
  * \param [in] a pointer to the new parent
  */
void XPMImageManipulationPanel::SetParentPanel(XPMEditorPanel *p)
{
    m_parent = p;
}

/** Stretch the image or the selection */
void XPMImageManipulationPanel::OnButtonStretchClick(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnStretchImage(event);
    }
}

/** Flip the image or the selection */
void XPMImageManipulationPanel::OnButtonMirrorClick(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnMirror(event);
    }
}

/** Blur the image or the selection */
void XPMImageManipulationPanel::OnButtonBlurClick(wxCommandEvent& event)
{

    if (m_parent)
    {
        m_parent->OnBlur(event);
    }
}

/** Rotate the image or the selection */
void XPMImageManipulationPanel::OnButtonRotateClick(wxCommandEvent& event)
{

    if (m_parent)
    {
        m_parent->OnRotate(event);
    }
}

/** Convert some colours from the image or the selection */
void XPMImageManipulationPanel::OnButtonHueClick(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnRotateHueClick(event);
    }
}

/** Convert to Gray Scale / Monochrome the image or the selection */
void XPMImageManipulationPanel::OnButtonColourDepthClick(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnButtonColourDepthClick(event);
    }
}

/** Invert the image or the selection */
void XPMImageManipulationPanel::OnButtonInvertClick(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnInvertImageClick(event);
    }
}

/** HELP HANDLERS ***/
void XPMImageManipulationPanel::OnWidgetsMouseLeave(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(-1);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnStretchButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnMirrorButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 1);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnBlurButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 2);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnRotateButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 3);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnHueButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 4);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnColourDepthButtonMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 5);
    }

    event.Skip();
}

void XPMImageManipulationPanel::OnInvertButtonMouseEnter(wxMouseEvent& event)

{
    if (m_parent)
    {
        m_parent->DisplayHelpText(ID_FIRST_HELP + 6);
    }

    event.Skip();
}

