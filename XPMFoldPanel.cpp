/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMFoldPanel.cpp
 * Purpose:   A panel containing iall the other tool panels, collapsible - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMEditorPanel.h"

#include "XPMFoldPanel.h"
#include "XPMToolPanel.h"
#include "XPMImageManipulationPanel.h"
#include "XPMImagePropertiesPanel.h"
#include "XPMHelpPanel.h"

//#include <sdk.h>
#include <wx/scrolbar.h>
#include <wx/settings.h>

//#include <sdk.h> // Code::Blocks SDK

//(*InternalHeaders(XPMFoldPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMFoldPanel)
const long XPMFoldPanel::ID_FOLDPANELBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMFoldPanel,wxPanel)
	//(*EventTable(XPMFoldPanel)
	//*)
END_EVENT_TABLE()

XPMFoldPanel::XPMFoldPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
	CreateSubPanels();
	//expand some panels
    FoldPanelBar1->Expand((size_t) 0, false); //expand tools panel
    FoldPanelBar1->Expand((size_t) 3, false); //expand help panel
    if (MainSizer) MainSizer->Layout();
}

void XPMFoldPanel::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMFoldPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	MainSizer = new wxBoxSizer(wxHORIZONTAL);
	FoldPanelBar1 = new wxFoldPanelEx(this);
	MainSizer->Add(FoldPanelBar1, 1, wxALL|wxEXPAND, 0);
	SetSizer(MainSizer);
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);
	//*)

	ToolPanel = NULL;
    ManipulationPanel = NULL;
    PropertiesPanel = NULL;
    HelpPanel = NULL;

}

XPMFoldPanel::~XPMFoldPanel()
{
	//(*Destroy(XPMFoldPanel)
	//*)
}

/** Create the various collapsible panels
  */
void XPMFoldPanel::CreateSubPanels(void)
{
    //create the various collapsible panels

    if (!FoldPanelBar1) return;

    //create all the sub-panels first
    ToolPanel = new XPMToolPanel(this);                      //Main tool buttons
    ManipulationPanel = new XPMImageManipulationPanel(this); //Image Manipulation panel
    PropertiesPanel = new XPMImagePropertiesPanel(this);     //Image Properties panel
    HelpPanel = new XPMHelpPanel(this);                      //Help panel

    //add the panels to the foldbar
    FoldPanelBar1->AddFoldPanel(_("Tools"), ToolPanel, WX_CAPTION_NORMAL);                      //Main tool buttons
    FoldPanelBar1->AddFoldPanel(_("Image Modification"), ManipulationPanel, WX_CAPTION_NORMAL); //Image Manipulation panel
    FoldPanelBar1->AddFoldPanel(_("Image Properties"), PropertiesPanel, WX_CAPTION_NORMAL);     //Image Properties panel
    FoldPanelBar1->AddFoldPanel(_("Help"), HelpPanel, WX_CAPTION_NORMAL);                       //Help panel

    ComputeDimensions();

    if (MainSizer)
    {
        MainSizer->Layout();
        MainSizer->Fit(this);
    }

}

/** Return the best size of the panel
  * \return : the ideal size of the caption bar + the child panel
  *           orientation + expanded / collapsed state are taken into account
  */
wxSize XPMFoldPanel::DoGetBestSize(void)
{
    ComputeDimensions();
    //Manager::Get()->GetLogManager()->Log(_("XPMFoldPanel::DoGetBestSize"));
    return(m_sBestSize);
}

/** Return the minimal size of the panel
  * \return : the ideal size of the caption bar + the child panel
  *           orientation + expanded / collapsed state are taken into account
  */
wxSize XPMFoldPanel::DoGetMinSize(void)
{
    ComputeDimensions();
    //Manager::Get()->GetLogManager()->Log(_("XPMFoldPanel::DoGetMinSize"));
    return(m_sMinSize);
}

/** Return the absolute minimal size of the panel (only the caption bars + 10)
  * \return : the ideal size of the caption bar + the child panel
  *           orientation + expanded / collapsed state are taken into account
  */
wxSize XPMFoldPanel::DoGetAbsoluteMinimalSize(void)
{
    ComputeDimensions();
    //Manager::Get()->GetLogManager()->Log(_("XPMFoldPanel::DoGetMinSize"));
    return(m_sAbsMinSize);
}

/** Return the maximal size of the panel
  * \return : the ideal size of the caption bar + the child panel
  *           orientation + expanded / collapsed state are taken into account
  */
wxSize XPMFoldPanel::DoGetMaxSize(void)
{
    ComputeDimensions();
    //Manager::Get()->GetLogManager()->Log(_("XPMFoldPanel::DoGetMaxSize"));
    return(m_sMaxSize);
}

void XPMFoldPanel::ComputeDimensions(void)
{
    wxSize sCaptionBarSize(0,0);
    wxSize sToolPanelSize(0,0);
    wxSize sImagePropertiesPanelSize(0,0);
    wxSize sImageManipulationPanelSize(0,0);
    wxSize sHelpPanelSize(0,0);
    wxFoldItemEx *f;
    int iWidth, iHeight;
    int iWidthMax, iHeightMax;
    int iWidthAbs, iHeightAbs;


    //default size
    iWidth = 50;
    iHeight = 50;
    iWidthMax = 100;
    iHeightMax = 100;
    iWidthAbs = 10;
    iHeightAbs = 10;

    if (FoldPanelBar1)
    {
        //get the size of a caption bar
        f = FoldPanelBar1->GetFoldItem(0);
        if (f)
        {
            wxCaptionBarEx *c;
            c = f->GetCaptionBar();
            if (c) sCaptionBarSize = c->GetBestSize();
        }

        //get the size of all fold panels
        //tool panel
        if (f)
        {
            wxWindow *p;
            p = f->GetChildWindow();
            if (p) sToolPanelSize = p->GetBestSize();
        }

        //image manipulation panel
        f = FoldPanelBar1->GetFoldItem(1);
        if (f)
        {
            wxWindow *p;
            p = f->GetChildWindow();
            if (p) sImageManipulationPanelSize = p->GetBestSize();
        }

        //image properties panel
        f = FoldPanelBar1->GetFoldItem(2);
        if (f)
        {
            wxWindow *p;
            p = f->GetChildWindow();
            if (p) sImagePropertiesPanelSize = p->GetBestSize();
        }

        //help panel
        f = FoldPanelBar1->GetFoldItem(3);
        if (f)
        {
            wxWindow *p;
            p = f->GetChildWindow();
            if (p) sHelpPanelSize = p->GetBestSize();
        }

/*
        //log
        iHeight = GetHorizontalScrollBarHeight();
        iWidth  = GetVerticalScrollBarWidth();
        Manager::Get()->GetLogManager()->Log(_("-----------------------------------------------------------------"));
        Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions Caption Bar w=%d h=%d"), sCaptionBarSize.GetWidth(), sCaptionBarSize.GetHeight()));
        Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions Tool Panel w=%d h=%d"), sToolPanelSize.GetWidth(), sToolPanelSize.GetHeight()));
        Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions Manipulation  w=%d h=%d"), sImageManipulationPanelSize.GetWidth(), sImageManipulationPanelSize.GetHeight()));
        Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions Properties w=%d h=%d"), sImagePropertiesPanelSize.GetWidth(), sImagePropertiesPanelSize.GetHeight()));
        Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions Help Panel w=%d h=%d"), sHelpPanelSize.GetWidth(), sHelpPanelSize.GetHeight()));
        Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions Scrollbars w=%d h=%d"), iWidth, iHeight));
        Manager::Get()->GetLogManager()->Log(_("-----------------------------------------------------------------"));
*/

        //compute the default size for each
        if (FoldPanelBar1->IsVertical())
        {
            //vertical panel
            //width is the greatest of all widths.
            //height is the sum of all Caption bars height, + the height of the biggest panel
            int iw;

            //width calculation
            iWidth = sCaptionBarSize.GetWidth();
            if (sToolPanelSize.GetWidth() > iWidth) iWidth = sToolPanelSize.GetWidth();
            if (sImageManipulationPanelSize.GetWidth() > iWidth) iWidth = sImageManipulationPanelSize.GetWidth();
            if (sImagePropertiesPanelSize.GetWidth() > iWidth) iWidth = sImagePropertiesPanelSize.GetWidth();
            if (sHelpPanelSize.GetWidth() > iWidth) iWidth = sHelpPanelSize.GetWidth();
            //add the width of a vertical scrollbar
            iw = GetVerticalScrollBarWidth();
            if (iw > 0) iWidth = iWidth + iw;
            iWidthMax = iWidth;
            iWidthAbs = iWidth;

            //height calculation
            iHeight = sToolPanelSize.GetHeight();
            if (sImageManipulationPanelSize.GetHeight() > iHeight) iHeight = sImageManipulationPanelSize.GetHeight();
            if (sImagePropertiesPanelSize.GetHeight() > iHeight) iHeight = sImagePropertiesPanelSize.GetHeight();
            if (sHelpPanelSize.GetHeight() > iHeight) iHeight = sHelpPanelSize.GetHeight();
            iHeight = iHeight + 4 * sCaptionBarSize.GetHeight();
            iHeightMax = 4 * sCaptionBarSize.GetHeight();
            iHeightMax += sToolPanelSize.GetHeight();
            iHeightMax += sImageManipulationPanelSize.GetHeight();
            iHeightMax += sImagePropertiesPanelSize.GetHeight();
            iHeightMax += sHelpPanelSize.GetHeight();
            iHeightAbs = 10 + 4 * sCaptionBarSize.GetHeight();


        }
        else
        {
            //horizontal panel
            //width is the sum of all Caption bars width, + the width of the biggest panel
            //height is the greatest of all height.
            int ih;

            //width calculation
            iWidth = sToolPanelSize.GetWidth();
            if (sImageManipulationPanelSize.GetWidth() > iWidth) iWidth = sImageManipulationPanelSize.GetWidth();
            if (sImagePropertiesPanelSize.GetWidth() > iWidth) iWidth = sImagePropertiesPanelSize.GetWidth();
            if (sHelpPanelSize.GetWidth() > iWidth) iWidth = sHelpPanelSize.GetWidth();
            iWidth = iWidth + 4 * sCaptionBarSize.GetWidth();
            iWidthMax = 4 * sCaptionBarSize.GetWidth();
            iWidthMax += sToolPanelSize.GetWidth();
            iWidthMax += sImageManipulationPanelSize.GetWidth();
            iWidthMax += sImagePropertiesPanelSize.GetWidth();
            iWidthMax += sHelpPanelSize.GetWidth();
            iWidthAbs = 10 + 4 * sCaptionBarSize.GetWidth();

            //height calculation
            iHeight = sCaptionBarSize.GetHeight();
            if (sToolPanelSize.GetHeight() > iHeight) iHeight = sToolPanelSize.GetHeight();
            if (sImageManipulationPanelSize.GetHeight() > iHeight) iHeight = sImageManipulationPanelSize.GetHeight();
            if (sImagePropertiesPanelSize.GetHeight() > iHeight) iHeight = sImagePropertiesPanelSize.GetHeight();
            if (sHelpPanelSize.GetHeight() > iHeight) iHeight = sHelpPanelSize.GetHeight();
            //add the height of a horizontal scrollbar
            ih = GetHorizontalScrollBarHeight();
            if (ih > 0) iHeight = iHeight + ih;
            iHeightMax = iHeight;
            iHeightAbs = iHeight;
        }
    }

/*
    Manager::Get()->GetLogManager()->Log(_("-----------------------------------------------------------------"));
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions TOTAL w=%d h=%d"), iWidth, iHeight));
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("XPMFoldPanel::ComputeDimensions TOTAL MAX w=%d h=%d"), iWidthMax, iHeightMax));
    Manager::Get()->GetLogManager()->Log(_("-----------------------------------------------------------------"));
*/

    SetMinSize(wxSize(iWidth, iHeight));
    SetMaxSize(wxSize(iWidthMax, iHeightMax));

    m_sMinSize = wxSize(iWidth, iHeight);
    m_sBestSize = wxSize(iWidth, iHeight);
    m_sMaxSize = wxSize(iWidthMax, iHeightMax);
    m_sAbsMinSize = wxSize(iWidthAbs, iHeightAbs);
}

/** Set the parent of the panels
  * \param parent : the functionnal parent of the panel
  */
void XPMFoldPanel::SetParentPanel(XPMEditorPanel *parent)
{

    if (ToolPanel) ToolPanel->SetParentPanel(parent);
    if (ManipulationPanel) ManipulationPanel->SetParentPanel(parent);
    if (PropertiesPanel) PropertiesPanel->SetParentPanel(parent);
    if (HelpPanel) HelpPanel->SetParentPanel(parent);

    ComputeDimensions();
    if (MainSizer)
    {
        MainSizer->Layout();
        MainSizer->Fit(this);
    }

}

//---- GET POINTERS TO CHILD PANELS ------------------//
/**return a pointer to the Tool Panel
  *\return : a pointer to the wxPanel on success, NULL otherwise
  */
XPMToolPanel* XPMFoldPanel::GetToolPanel(void)
{
    //return a pointer to the Tool Panel
    return(ToolPanel);
}

/**return a pointer to the Tool Panel
  *\return : a pointer to the wxPanel on success, NULL otherwise
  */
XPMHelpPanel* XPMFoldPanel::GetHelpPanel(void)
{
    //return a pointer to the Help Panel
    return(HelpPanel);
}

/**return a pointer to the Image Properties Panel
  *\return : a pointer to the wxPanel on success, NULL otherwise
  */
XPMImagePropertiesPanel* XPMFoldPanel::GetPropertiesPanel(void)
{
    //return a pointer to the Image Properties Panel
    return(PropertiesPanel);
}

/**return a pointer to the Image Manipulation Panel
  *\return : a pointer to the wxPanel on success, NULL otherwise
  */
XPMImageManipulationPanel* XPMFoldPanel::GetImageManipulationPanel(void)
{
    //return a pointer to the Image Manipulation Panel
    return(ManipulationPanel);
}

/** return the width of a standard vertical scrollbar
  * \return the width of a standard vertical scrollbar
  */
int XPMFoldPanel::GetVerticalScrollBarWidth(void)
{
    int iResult1, iResult2, iResult;
    wxSize sSize;

    iResult1 = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
    iResult2 = wxSystemSettings::GetMetric(wxSYS_VSCROLL_ARROW_X);

    iResult = iResult1;
    if (iResult2 > iResult) iResult = iResult2;

    if (iResult < 0)
    {
        //create a scrollbar and destroy it
        wxScrollBar *sb;
        sb = new wxScrollBar(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL);

        if (sb)
        {
            sSize = sb->GetBestSize();
            iResult = sSize.GetWidth();
            sb->Destroy();
        }
    }

    return(iResult);
}

/** return the height of a standard horizontal scrollbar
  * \return the height of a standard horizontal scrollbar
  */
int XPMFoldPanel::GetHorizontalScrollBarHeight(void)
{
    int iResult1, iResult2, iResult;
    wxSize sSize;

    iResult1 = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
    iResult2 = wxSystemSettings::GetMetric(wxSYS_HSCROLL_ARROW_Y);

    iResult = iResult1;
    if (iResult2 > iResult) iResult = iResult2;

    if (iResult < 0)
    {
        //create a scrollbar and destroy it
        wxScrollBar *sb;
        sb = new wxScrollBar(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);

        if (sb)
        {
            sSize = sb->GetBestSize();
            iResult = sSize.GetHeight();
            sb->Destroy();
        }
    }

    return(iResult);
}
