/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImageInterfacePanel.h
 * Purpose:   A panel containing image display option interface - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPMINTERFACEPANEL_H
#define XPMINTERFACEPANEL_H

//(*Headers(XPMInterfacePanel)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/bmpbuttn.h>
#include <wx/combobox.h>
#include <wx/clrpicker.h>
//*)

class XPMEditorPanel;

class XPMInterfacePanel: public wxPanel
{
	public:

		XPMInterfacePanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMInterfacePanel();

		void SetParentPanel(XPMEditorPanel *p); ///< \brief set the parent panel for this tool panel

		//(*Declarations(XPMInterfacePanel)
		wxColourPickerCtrl* GridColour;
		wxBitmapButton* BitmapButton2;
		wxStaticText* StaticText1;
		wxBoxSizer* BoxSizer2;
		wxBitmapButton* BitmapButton1;
		wxCheckBox* CheckBox1;
		wxComboBox* ZoomFactor;
		//*)

	protected:

		//(*Identifiers(XPMInterfacePanel)
		static const long ID_STATICTEXT1;
		static const long ID_COMBOBOX1;
		static const long ID_BITMAPBUTTON1;
		static const long ID_BITMAPBUTTON2;
		static const long ID_CHECKBOX1;
		static const long ID_CUSTOM3;
		//*)

		//help handlers
		void OnZoomWidgetsMouseEnter(wxMouseEvent& event);
		void OnShowGridWidgetsMouseEnter(wxMouseEvent& event);
		void OnGridColourWidgetsMouseEnter(wxMouseEvent& event);
		void OnRotateClockwiseWidgetsMouseEnter(wxMouseEvent& event);
		void OnRotateCounterClockwiseWidgetsMouseEnter(wxMouseEvent& event);
		void OnWidgetsMouseLeave(wxMouseEvent& event);

	private:

		//(*Handlers(XPMInterfacePanel)
		void OnZoomChanged(wxCommandEvent& event);
		void OnRotateCounterClockwise(wxCommandEvent& event);
		void OnRotateClockwise(wxCommandEvent& event);
		void OnShowGrid(wxCommandEvent& event);
		void OnGridColourMouseEnter(wxMouseEvent& event);
		void OnGridColourMouseLeave(wxMouseEvent& event);
		//*)

		XPMEditorPanel *m_parent;

		void OnGridColourPickerColourChanged(wxColourPickerEvent& event);    ///< \brief The colour of the grid changed

		DECLARE_EVENT_TABLE()
};

#endif
