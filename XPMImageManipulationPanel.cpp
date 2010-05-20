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
	BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("STRETCH"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer13->Add(Button1, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button2 = new wxButton(this, ID_BUTTON2, _("MIRROR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer13->Add(Button2, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button3 = new wxButton(this, ID_BUTTON3, _("BLUR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	BoxSizer13->Add(Button3, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button4 = new wxButton(this, ID_BUTTON4, _("ROTATE"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer13->Add(Button4, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button5 = new wxButton(this, ID_BUTTON5, _("HUE"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
	BoxSizer13->Add(Button5, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button6 = new wxButton(this, ID_BUTTON6, _("COLOUR DEPTH"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
	BoxSizer13->Add(Button6, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button7 = new wxButton(this, ID_BUTTON7, _("INVERT"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
	BoxSizer13->Add(Button7, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer13->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
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
}

XPMImageManipulationPanel::~XPMImageManipulationPanel()
{
	//(*Destroy(XPMImageManipulationPanel)
	//*)
}

/** Stretch the image or the selection */
void XPMImageManipulationPanel::OnButtonStretchClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnStretchImage(event);
    }
}

/** Flip the image or the selection */
void XPMImageManipulationPanel::OnButtonMirrorClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnMirror(event);
    }
}

/** Blur the image or the selection */
void XPMImageManipulationPanel::OnButtonBlurClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnBlur(event);
    }
}

/** Rotate the image or the selection */
void XPMImageManipulationPanel::OnButtonRotateClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnRotate(event);
    }
}

/** Convert some colours from the image or the selection */
void XPMImageManipulationPanel::OnButtonHueClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnRotateHueClick(event);
    }
}

/** Convert to Gray Scale / Monochrome the image or the selection */
void XPMImageManipulationPanel::OnButtonColourDepthClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnButtonColourDepthClick(event);
    }
}

/** Invert the image or the selection */
void XPMImageManipulationPanel::OnButtonInvertClick(wxCommandEvent& event)
{
    XPMEditorPanel *parent;

    parent = (XPMEditorPanel *) GetParent();
    if (parent)
    {
        parent->OnInvertImageClick(event);
    }
}
