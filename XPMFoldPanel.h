/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMFoldPanel.h
 * Purpose:   A panel containing iall the other tool panels, collapsible - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPMFOLDPANEL_H
#define XPMFOLDPANEL_H

#include <wx/dcclient.h>

//(*Headers(XPMFoldPanel)
#include <wx/sizer.h>
#include <wx/panel.h>
#include "wx/foldpanelex/foldpanelex.h"
//*)

class XPMToolPanel;
class XPMImageManipulationPanel;
class XPMImagePropertiesPanel;
class XPMHelpPanel;
class XPMEditorPanel;

class XPMFoldPanel: public wxPanel
{
	public:

		XPMFoldPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMFoldPanel();

		void CreateSubPanels(void); ///< \brief Create the various collapsible panels
		void SetParentPanel(XPMEditorPanel *parent); ///< \brief Set the parent of the panels

        //get pointers to child panels.
		XPMToolPanel* GetToolPanel(void);   ///< \brief return a pointer to the Tool Panel
	    XPMHelpPanel* GetHelpPanel(void);   ///< \brief return a pointer to the Help Panel
	    XPMImagePropertiesPanel* GetPropertiesPanel(void); ///< \brief return a pointer to the Image Properties Panel
	    XPMImageManipulationPanel* GetImageManipulationPanel(void); ///< \brief return a pointer to the Image Manipulation Panel

		//(*Declarations(XPMFoldPanel)
		wxBoxSizer* MainSizer;
		wxFoldPanelEx* FoldPanelBar1;
		//*)

	protected:

		//(*Identifiers(XPMFoldPanel)
		static const long ID_FOLDPANELBAR1;
		//*)

		//(*Handlers(XPMFoldPanel)
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		XPMToolPanel *ToolPanel;
		XPMImageManipulationPanel *ManipulationPanel;
		XPMImagePropertiesPanel *PropertiesPanel;
		XPMHelpPanel *HelpPanel;

		DECLARE_EVENT_TABLE()
};

#endif
