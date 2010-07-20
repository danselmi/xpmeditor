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

        //initialisation of the dialog box
        //resolution
        bool bModified, bResolution, bJPG, bPNGFormat, bPNGBitDepth;
        int iResolutionUnit, iResolutionX, iResolutionY, iJPGQuality;
        int iPNGBitDepth, iPNGFormat;
        wxString sString;
        wxString sString2;
        wxString sPNGFormat;
        wxString sPNGBitDepths;

        bModified = false;



        if ((img.HasOption(wxIMAGE_OPTION_RESOLUTIONUNIT)) || (img.HasOption(wxIMAGE_OPTION_RESOLUTIONUNIT)) || (img.HasOption(wxIMAGE_OPTION_RESOLUTIONUNIT)))
        {
            bResolution = true;

            iResolutionUnit = img.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONUNIT);
            switch(iResolutionUnit)
            {

                case wxIMAGE_RESOLUTION_INCHES : sString = _("Inch"); break;
                case wxIMAGE_RESOLUTION_CM     : sString = _("Cm"); break;
                #if wxCHECK_VERSION(2, 9, 0)
                case wxIMAGE_RESOLUTION_NONE   :
                #endif
                default                        : sString = _("Not Specified"); break;
            }

            iResolutionX = img.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
            iResolutionY = img.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);
        }
        else
        {
            bResolution = false;

            iResolutionUnit = 0;
            iResolutionX = 0;
            iResolutionY = 0;
            sString = _("Not Specified");
        }
        dao.ChoiceResolutionUnit->SetStringSelection(sString);
        dao.SpinResolutionX->SetValue(iResolutionX);
        dao.SpinResolutionY->SetValue(iResolutionY);
        dao.EnableResolutionOption(bResolution);

        //JPG quality
        if (img.HasOption(wxIMAGE_OPTION_QUALITY))
        {
            iJPGQuality = img.GetOptionInt(wxIMAGE_OPTION_QUALITY);
            bJPG = true;
        }
        else
        {
            iJPGQuality = 0;
            bJPG = false;
        }
        dao.SliderJPGCompression->SetValue(iJPGQuality);
        dao.EnableJPGOption(bJPG);

        //PNG Format
        if (img.HasOption(wxIMAGE_OPTION_PNG_FORMAT))
        {
            bPNGFormat = true;
            iPNGFormat = img.GetOptionInt(wxIMAGE_OPTION_PNG_FORMAT);

            switch(iPNGFormat)
            {
                case wxPNG_TYPE_COLOUR   : sPNGFormat = _("0 - Colour PNG"); break;
                case wxPNG_TYPE_GREY     : sPNGFormat = _("2 - Grey Scale PNG"); break;
                case wxPNG_TYPE_GREY_RED : sPNGFormat = _("3 - Grey Scale using Red as Grey"); break;
                default                  : sPNGFormat = _("0 - Colour PNG"); break;
            }


        }
        else
        {
            bPNGFormat = false;
            iPNGFormat = wxPNG_TYPE_COLOUR;
            sPNGFormat = _("0 - Colour PNG");
        }
        dao.ChoicePNGFormat->SetStringSelection(sPNGFormat);
        dao.EnablePNGFormat(bPNGFormat);

        //PNG Bit Depths
        if (img.HasOption(wxIMAGE_OPTION_PNG_BITDEPTH))
        {
            bPNGBitDepth = true;
            iPNGBitDepth = img.GetOptionInt(wxIMAGE_OPTION_PNG_BITDEPTH);
            //only 8 or 16 are valid values
            if (iPNGBitDepth < 16) iPNGBitDepth = 8;
            if (iPNGBitDepth > 16) iPNGBitDepth = 16;
            if (iPNGBitDepth == 8)  sPNGBitDepths = _("8");
            if (iPNGBitDepth == 16) sPNGBitDepths = _("16");
        }
        else
        {
            bPNGBitDepth = false;
            iPNGBitDepth = 8;
            sPNGBitDepths = _("8");
        }
        dao.ChoicePNGBitDepths->SetStringSelection(sPNGBitDepths);
        dao.EnablePNGBitsDepth(bPNGBitDepth);


        if (dao.ShowModal() == 0)
        {
            //set the resolution
            if (dao.CheckBoxResolutionOption->IsChecked())
            {
                sString2 = dao.ChoiceResolutionUnit->GetStringSelection();
                if ((sString2 == _("Inch")) && (sString != sString2)) img.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);
                if ((sString2 == _("Cm")) && (sString != sString2)) img.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_CM);
                #if wxCHECK_VERSION(2, 9, 0)
                if ((sString2 == _("Not Specified")) && (sString != sString2)) img.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_NONE);
                #else
                if ((sString2 == _("Not Specified")) && (sString != sString2)) img.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);
                #endif

                if (sString2 != sString) bModified = true;

                int iResolutionX2, iResolutionY2;
                iResolutionX2 = dao.SpinResolutionX->GetValue();
                iResolutionY2 = dao.SpinResolutionY->GetValue();

                img.SetOption(wxIMAGE_OPTION_RESOLUTIONX, iResolutionX2);
                img.SetOption(wxIMAGE_OPTION_RESOLUTIONY, iResolutionY2);

                if (iResolutionX != iResolutionX2) bModified = true;
                if (iResolutionY != iResolutionY2) bModified = true;
            }
            if (bResolution != dao.CheckBoxResolutionOption->IsChecked()) bModified = true;

            //set the JPG quality
            if (dao.CheckBoxJPGCompression->IsChecked())
            {
                int iJPGQuality2;
                iJPGQuality2 = dao.SliderJPGCompression->GetValue();

                img.SetOption(wxIMAGE_OPTION_QUALITY, iJPGQuality2);

                if (iJPGQuality != iJPGQuality2) bModified = true;
            }
            if (bJPG != dao.CheckBoxJPGCompression->IsChecked()) bModified = true;

            //set the PNG Format
            if (dao.CheckBoxPNGFormat->IsChecked())
            {
                wxString sPNGFormat2;
                sPNGFormat2 = dao.ChoicePNGFormat->GetStringSelection();
                if ((sPNGFormat != sPNGFormat2) && (sPNGFormat2 == _("0 - Colour PNG"))) img.SetOption(wxIMAGE_OPTION_PNG_FORMAT, wxPNG_TYPE_COLOUR);
                if ((sPNGFormat != sPNGFormat2) && (sPNGFormat2 == _("2 - Grey Scale PNG"))) img.SetOption(wxIMAGE_OPTION_PNG_FORMAT, wxPNG_TYPE_GREY);
                if ((sPNGFormat != sPNGFormat2) && (sPNGFormat2 == _("3 - Grey Scale using Red as Grey"))) img.SetOption(wxIMAGE_OPTION_PNG_FORMAT, wxPNG_TYPE_GREY_RED);

                if (sPNGFormat != sPNGFormat2) bModified = true;
            }

            //set the PNG Bit Depth
            if (dao.CheckBoxPNGBitDepth->IsChecked())
            {
                wxString sPNGBitDepths2;
                sPNGBitDepths2 = dao.ChoicePNGBitDepths->GetStringSelection();
                if ((sPNGBitDepths != sPNGBitDepths2) && (sPNGBitDepths2 == _("8"))) img.SetOption(wxIMAGE_OPTION_PNG_BITDEPTH , 8);
                if ((sPNGBitDepths != sPNGBitDepths2) && (sPNGBitDepths2 == _("16"))) img.SetOption(wxIMAGE_OPTION_PNG_BITDEPTH , 16);

                if (sPNGBitDepths != sPNGBitDepths2) bModified = true;
            }



            if (bModified)
            {
                m_parent->SetModified(bModified);
                m_parent->SetImage(&img);
            }
        }

    }
}

/** The type of the image has changed (BMP, PNG, JPEG, XPM, ...)
  */
void XPMImagePropertiesPanel::OnImageTypeChanged(wxCommandEvent& event)
{
    if ((m_parent) && (XPM_Plugin()))
    {
        wxBitmapType bt;
        wxString sSelection;

        sSelection = event.GetString();
        bt = XPM_Plugin()->GetFormatBitmap(sSelection);
        //Manager::Get()->GetLogManager()->Log(wxString::Format(_("bt=%d"), bt));
        m_parent->SetImageFormat(bt);
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
