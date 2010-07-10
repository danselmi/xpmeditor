/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImagePropertiesPanel.cpp
 * Purpose:   A panel containing the image properties: size, file format, compression, resolution - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMImagePropertiesPanel.h"
#include "XPMEditorPanel.h"

//(*InternalHeaders(XPMImagePropertiesPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMImagePropertiesPanel)
const long XPMImagePropertiesPanel::ID_STATICTEXT2 = wxNewId();
const long XPMImagePropertiesPanel::ID_SPINCTRL1 = wxNewId();
const long XPMImagePropertiesPanel::ID_STATICTEXT3 = wxNewId();
const long XPMImagePropertiesPanel::ID_SPINCTRL2 = wxNewId();
const long XPMImagePropertiesPanel::ID_STATICTEXT13 = wxNewId();
const long XPMImagePropertiesPanel::ID_CHOICE1 = wxNewId();
const long XPMImagePropertiesPanel::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMImagePropertiesPanel,wxPanel)
	//(*EventTable(XPMImagePropertiesPanel)
	//*)
END_EVENT_TABLE()

XPMImagePropertiesPanel::XPMImagePropertiesPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMImagePropertiesPanel)
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer18;
	wxBoxSizer* BoxSizer17;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Size in Pixels"));
	BoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Width:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer17->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BMPWidth = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxSize(71,21), 0, 0, 10000, 0, _T("ID_SPINCTRL1"));
	BMPWidth->SetValue(_T("0"));
	BoxSizer17->Add(BMPWidth, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer17, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer1->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BMPHeight = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxSize(71,21), 0, 0, 10000, 0, _T("ID_SPINCTRL2"));
	BMPHeight->SetValue(_T("0"));
	BoxSizer1->Add(BMPHeight, 0, wxALL|wxALIGN_RIGHT|wxALIGN_BOTTOM, 5);
	StaticBoxSizer1->Add(BoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer8->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer18 = new wxBoxSizer(wxVERTICAL);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT13, _("Type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	BoxSizer18->Add(StaticText12, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	Choice1->Append(_("Automatic"));
	Choice1->Append(_("bitmap (*.bmp)"));
	Choice1->Append(_("cursor (*.cur)"));
	Choice1->Append(_("icon (*.icon)"));
	Choice1->Append(_("jpeg (*.jpg)"));
	Choice1->Append(_("pcx (*.pcx)"));
	Choice1->Append(_("png (*.png)"));
	Choice1->Append(_("pnm (*.pnm)"));
	Choice1->Append(_("tiff (*tif, *.tiff)"));
	Choice1->Append(_("xpm (*.xpm)"));
	BoxSizer18->Add(Choice1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer8->Add(BoxSizer18, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button1 = new wxButton(this, ID_BUTTON1, _("Advanced"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer8->Add(Button1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SetSizer(BoxSizer8);
	BoxSizer8->Fit(this);
	BoxSizer8->SetSizeHints(this);
	
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnImageSizeChanged);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnImageSizeChanged);
	Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnImageTypeChanged);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnButtonAdvancedPropClick);
	//*)

	m_parent = NULL;
}

XPMImagePropertiesPanel::~XPMImagePropertiesPanel()
{
	//(*Destroy(XPMImagePropertiesPanel)
	//*)
}

/** Set the parent panel
  * \param [in] a pointer to the new parent
  */
void XPMImagePropertiesPanel::SetParentPanel(XPMEditorPanel *p)
{
    m_parent = p;
}

/** The button "ADVANCED" has been clicked.
  */
void XPMImagePropertiesPanel::OnButtonAdvancedPropClick(wxCommandEvent& event)
{
    if (m_parent)
    {

    }
}

/** The type of the image has changed (BMP, PNG, JPEG, XPM, ...)
  */
void XPMImagePropertiesPanel::OnImageTypeChanged(wxCommandEvent& event)
{
    if (m_parent)
    {

    }
}


/** The width or the height of the Image changed
  */
void XPMImagePropertiesPanel::OnImageSizeChanged(wxSpinEvent& event)
{
    if (m_parent)
    {
        m_parent->OnImageSizeChanged(event);
    }
}
