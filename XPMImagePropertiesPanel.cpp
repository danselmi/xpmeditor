/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImagePropertiesPanel.cpp
 * Purpose:   A panel containing the image properties: size, file format, compression, resolution - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMEditor.h"
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

    AppendFormatsToChoice();
	m_parent = NULL;
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
    m_sFormats.Clear();
    m_sFormats.Add(_("Automatic"));
    m_sFormats.Add(_("bitmap (*.bmp)"));
    m_sFormats.Add(_("cursor (*.cur)"));
    m_sFormats.Add(_("icon (*.icon)"));
    m_sFormats.Add(_("jpeg (*.jpg)"));
    m_sFormats.Add(_("pcx (*.pcx)"));
    m_sFormats.Add(_("png (*.png)"));
    m_sFormats.Add(_("pnm (*.pnm)"));
    m_sFormats.Add(_("tiff (*tif, *.tiff)"));
    m_sFormats.Add(_("xpm (*.xpm)"));

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
    if (!XPM_Plugin()->IsFormatValidForWriting(bt)) return; //file format not supported for writing. Nothing is done
    if (!Choice1) return;

    switch(bt)
    {
        //bitmap format - saving supported
        case wxBITMAP_TYPE_BMP            :
        case wxBITMAP_TYPE_BMP_RESOURCE   :
        //case wxBITMAP_TYPE_RESOURCE       :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_BMP));
                                            break;
        //png format - saving supported
        case wxBITMAP_TYPE_PNG            :
        case wxBITMAP_TYPE_PNG_RESOURCE   :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_PNG));
                                            break;

        //jpeg format - saving supported
        case wxBITMAP_TYPE_JPEG           :
        case wxBITMAP_TYPE_JPEG_RESOURCE  :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_JPG));
                                            break;

        //PCX format - saving supported
        case wxBITMAP_TYPE_PCX            :
        case wxBITMAP_TYPE_PCX_RESOURCE   :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_PCX));
                                            break;

        //PNM format - saving supported
        case wxBITMAP_TYPE_PNM            :
        case wxBITMAP_TYPE_PNM_RESOURCE   :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_PNM));
                                            break;

        //TIFF format - saving supported
        case wxBITMAP_TYPE_TIF            :
        case wxBITMAP_TYPE_TIF_RESOURCE   :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_TIF));
                                            break;

        case wxBITMAP_TYPE_ICO            :
        case wxBITMAP_TYPE_ICO_RESOURCE   :
        case wxBITMAP_TYPE_ICON           :
        case wxBITMAP_TYPE_ICON_RESOURCE  :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_ICO));
                                            break;

        //Cursor format - saving supported
        case wxBITMAP_TYPE_CUR            :
        case wxBITMAP_TYPE_CUR_RESOURCE   :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_CUR));
                                            break;

        //XPM format - saving supported
        case wxBITMAP_TYPE_XPM            :
        case wxBITMAP_TYPE_XPM_DATA       :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_XPM));
                                            break;


        //GIFF, IFF, TGA, ANI, PICT formats - saving NOT supported - revert to PNG as default
        case wxBITMAP_TYPE_ANI            :
        case wxBITMAP_TYPE_IFF            :
        case wxBITMAP_TYPE_TGA            :
        case wxBITMAP_TYPE_GIF            :
        case wxBITMAP_TYPE_GIF_RESOURCE   :
        case wxBITMAP_TYPE_PICT           :
        case wxBITMAP_TYPE_PICT_RESOURCE  :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_PNG));
                                            break;

        //Mac Cursor saving not supported. Revert to CUR as default
        case wxBITMAP_TYPE_MACCURSOR      :
        case wxBITMAP_TYPE_MACCURSOR_RESOURCE :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_CUR));
                                            break;


        //XBM saving not supported - revert to XPM as default
        case wxBITMAP_TYPE_XBM            :
        case wxBITMAP_TYPE_XBM_DATA       :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_XPM));
                                            break;

        //use autodetection based on filename
        case wxBITMAP_TYPE_INVALID        :
        case wxBITMAP_TYPE_ANY            :
        default :
                                            Choice1->SetSelection(m_iIndex.Item(XPM_FORMAT_INDEX_AUTO));
                                            break;
    }
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
