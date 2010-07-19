/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImagePropertiesPanel.cpp
 * Purpose:   A panel containing the image properties: size, file format, compression, resolution - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "AdvancedOptions.h"
#include "XPMEditor.h"
#include "XPMImagePropertiesPanel.h"
#include "XPMEditorPanel.h"

//#include <sdk.h>

//(*InternalHeaders(XPMImagePropertiesPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMImagePropertiesPanel)
const long XPMImagePropertiesPanel::ID_SPINCTRL1 = wxNewId();
const long XPMImagePropertiesPanel::ID_STATICTEXT1 = wxNewId();
const long XPMImagePropertiesPanel::ID_SPINCTRL2 = wxNewId();
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

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Width x Heigth"));
	BMPWidth = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxSize(60,21), 0, 0, 10000, 0, _T("ID_SPINCTRL1"));
	BMPWidth->SetValue(_T("0"));
	StaticBoxSizer1->Add(BMPWidth, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("x"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticBoxSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BMPHeight = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxSize(60,21), 0, 0, 10000, 0, _T("ID_SPINCTRL2"));
	BMPHeight->SetValue(_T("0"));
	StaticBoxSizer1->Add(BMPHeight, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Format"));
	Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	StaticBoxSizer2->Add(Choice1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button1 = new wxButton(this, ID_BUTTON1, _("Advanced"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	StaticBoxSizer2->Add(Button1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer8->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 2);
	SetSizer(BoxSizer8);
	BoxSizer8->Fit(this);
	BoxSizer8->SetSizeHints(this);

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnImageSizeChanged);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnImageSizeChanged);
	Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnImageTypeChanged);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMImagePropertiesPanel::OnButtonAdvancedPropClick);
	//*)

    AppendFormatsToChoice();
	m_parent = NULL;
/*
    wxSize sSize;
    sSize = StaticBoxSizer1->GetMinSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("StaticBoxSizer sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));
    sSize = StaticBoxSizer2->GetMinSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("StaticBoxSizer2 sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));
*/
}

XPMImagePropertiesPanel::~XPMImagePropertiesPanel()
{
	//(*Destroy(XPMImagePropertiesPanel)
	//*)
}

/** Populate the wxChoice with the available formats
  */
void XPMImagePropertiesPanel::AppendFormatsToChoice(void)
{

    if (XPM_Plugin())
    {
        XPM_Plugin()->GetFileSavingFormat(m_sFormats);
    }
    else
    {
       m_sFormats.Clear();
    }

    if (Choice1)
    {
        size_t i, iMax;
        int iIndex;
        Choice1->Clear();
        m_iIndex.Clear();

        iMax = m_sFormats.GetCount();
        for(i=0;i<iMax;i++)
        {
            iIndex = Choice1->Append(m_sFormats.Item(i));
            m_iIndex.Add(iIndex);
        }
    }
}

/** Set the parent panel
  * \param [in] p a pointer to the new parent
  */
void XPMImagePropertiesPanel::SetParentPanel(XPMEditorPanel *p)
{
    m_parent = p;
}

/** set the image saving file format in the wxChoice
  * \param [in] bt : the wxBitmapType to use for saving the image
  *                  See wxWidgets documentation for more information (keywords wxImage, wxBitmapType)
  */
void XPMImagePropertiesPanel::SetImageFormat(wxBitmapType bt)
{

    if (!XPM_Plugin()) return; //nothing done
    wxBitmapType bt2;
    wxString sFormatString;

    XPM_Plugin()->GetFileSavingFormat(m_sFormats);

    if (bt == wxBITMAP_TYPE_ANY)
    {
        Choice1->SetStringSelection(m_sFormats.Item(0));
        return;
    }

    bt2 = XPM_Plugin()->GetCompatibleSavingFormat(bt);
    sFormatString = XPM_Plugin()->GetFormatString(bt2);
    Choice1->SetStringSelection(sFormatString);

}

/** The button "ADVANCED" has been clicked.
  */
void XPMImagePropertiesPanel::OnButtonAdvancedPropClick(wxCommandEvent& event)
{
    if (m_parent)
    {
        AdvancedOptions dao(this);
        wxImage img = m_parent->GetImage();

        if (dao.ShowModal() == 0)
        {

        }

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
