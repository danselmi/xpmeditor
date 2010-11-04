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

		virtual wxSize DoGetBestSize(void); ///< \brief get the best size of the panel, according to the orientation (vertical or horizontal)
		virtual wxSize DoGetMinSize(void);  ///< \brief get the min size for the panel, according to the orientation (vertical or horizontal)
		virtual wxSize DoGetAbsoluteMinimalSize(void);  ///< \brief get the absolute min size for the panel, according to the orientation (vertical or horizontal)
        virtual wxSize DoGetMaxSize(void);  ///< \brief get the max size for the panel, according to the orientation (vertical or horizontal)

	protected:

		//(*Identifiers(XPMFoldPanel)
		static const long ID_FOLDPANELBAR1;
		//*)

		//(*Handlers(XPMFoldPanel)
		//*)
		void ComputeDimensions(void); ///< \brief compute the actual dimensions of the panel. Will set best size, min size and max size

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);
		int GetVerticalScrollBarWidth(void);    ///< \brief return the width of a standard vertical scrollbar
		int GetHorizontalScrollBarHeight(void); ///< \brief return the height of a standard horizontal scrollbar

		XPMToolPanel *ToolPanel;
		XPMImageManipulationPanel *ManipulationPanel;
		XPMImagePropertiesPanel *PropertiesPanel;
		XPMHelpPanel *HelpPanel;
		wxSize m_sBestSize;     ///< \brief the best size of the panel
		wxSize m_sMinSize;      ///< \brief the minimal size of the panel
		wxSize m_sAbsMinSize;   ///< \brief the absolute minimal size of the panel: only the caption bars + 10
		wxSize m_sMaxSize;      ///< \brief the maximal size of the panel

		DECLARE_EVENT_TABLE()
};

#endif
