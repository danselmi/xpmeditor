/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMColourPickerPanel.h
 * Purpose:   A simple panel containing only a ColourPicker widgets - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#ifndef XPMCOLOURPICKERPANEL_H
#define XPMCOLOURPICKERPANEL_H

//(*Headers(XPMColourPickerPanel)
#include <wx/sizer.h>
#include <wx/panel.h>
#include "XPMColorPicker.h"
//*)

class XPMEditorPanel;

class XPMColourPickerPanel: public wxPanel
{
	public:

		XPMColourPickerPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMColourPickerPanel();

		//(*Declarations(XPMColourPickerPanel)
		XPMColorPicker* ColourPicker;
		//*)

		//interface methods
		void init_colours(void); ///< \brief init the array of colours with default values.
        void Repaint(void); ///< \brief immediate repainting of the widgets

        //colours getter / setter
        wxColour GetLineColour(void);   ///< \brief return the current line colour
        wxColour GetFillColour(void);   ///< \brief return the current fill colour
        wxColour GetUnusedColour(void); ///< \brief return the 1st unused colour
        int GetLineColourIndex(void);   ///< \brief return the current line colour index
        int GetFillColourIndex(void);   ///< \brief return the current fill colour index
        void SetLineColour(int iIndex, bool bPostEvent = true); ///< \brief set the current line colour
        void SetFillColour(int iIndex, bool bPostEvent = true); ///< \brief set the current fill colour
        int GetPaletteSize(void);       ///< \brief get the palette size
        bool SetPaletteSize(int iSize); ///< \brief set the palette size
        void SetPaletteColour(int iIndex, wxColour cColor); ///< \brief set the specific palette colour
        wxColour GetPaletteColour(int iIndex);          ///< \brief Get the specific palette colour
        wxColour GetTransparentColour(void);            ///< \brief Get the transparent colour
        void SetTransparentColour(wxColour cColour, bool bPostEvent = true);  ///< \brief Set the transparent colour

        void SetParentPanel(XPMEditorPanel *p); ///< \brief set the parent panel for this tool panel

	protected:

		//(*Identifiers(XPMColourPickerPanel)
		static const long ID_COLOURPICKER1;
		//*)

		//event handlers
		void OnTransparentColorChanged(wxCommandEvent& event); ///< \brief the transparent colour in the colour picker changed
		void OnLineColorChanged(wxCommandEvent& event);        ///< \brief the Line colour in the colour picker changed
		void OnFillColorChanged(wxCommandEvent& event);        ///< \brief the Fill colour in the colour picker changed

	private:

		//(*Handlers(XPMColourPickerPanel)
		void OnColourPickerMouseEnter(wxMouseEvent& event);
		void OnColourPickerMouseLeave(wxMouseEvent& event);
		//*)

		XPMEditorPanel *m_parent;

		DECLARE_EVENT_TABLE()
};

#endif
