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
	MainSizer->Add(FoldPanelBar1, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
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

    if (MainSizer)
    {
        MainSizer->Layout();
        MainSizer->Fit(this);
    }
/*
    wxSize sSize; //GetFoldItem
    wxFoldItemEx *f;

    sSize = FoldPanelBar1->GetBestSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("FoldPanel sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));

    f = FoldPanelBar1->GetFoldItem(0);
    sSize = f->GetBestSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("ToolPanel sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));

    f = FoldPanelBar1->GetFoldItem(1);
    sSize = f->GetBestSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("Image Modifications sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));

    f = FoldPanelBar1->GetFoldItem(2);
    sSize = f->GetBestSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("Image Properties sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));

    f = FoldPanelBar1->GetFoldItem(3);
    sSize = f->GetBestSize();
    Manager::Get()->GetLogManager()->Log(wxString::Format(_("Help sBestSize w=%d h=%d"), sSize.GetWidth(), sSize.GetHeight()));
*/
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
