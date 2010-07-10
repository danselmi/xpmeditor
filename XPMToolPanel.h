/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMToolPanel.h
 * Purpose:   A panel containing all the drawing tools - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#ifndef XPMTOOLPANEL_H
#define XPMTOOLPANEL_H

#include "XPMEditorPanel.h"

//(*Headers(XPMToolPanel)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/things/toggle.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/clrpicker.h>
//*)

class XPMEditorPanel;

class XPMToolPanel: public wxPanel
{
	public:

		XPMToolPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, wxColour cHotSpotColour = *wxBLACK);
		virtual ~XPMToolPanel();

		void SetParentPanel(XPMEditorPanel *p); ///< \brief set the parent panel for this tool panel

		void ToggleButtons(int iIndex, bool bClearSelection = true); ///< \brief toggle all Tools Buttons OFF, except the iIndex one.
		void SetToolCursor(void); ///< \brief Set the correct cursor, according to the currently selected tool & option
		void InitToolData(void);     ///< \brief init tool data for a first use
        int GetToolID(void);  ///< \brief Get the tool ID currently in use
        void SetToolID(int iTool); ///< \brief Set the tool ID currently in use

        void HideControls(int iIndex, bool bChecked); ///< \brief Hide or Show specific tool controls - like style listbox for Brush tool...
        void HideControlsAndDoLayout(int iIndex, bool bChecked); ///< \brief Hide or Show specific tool controls - like style listbox for Brush tool... + update the layout

		//(*Declarations(XPMToolPanel)
		wxCustomButton* HotSpotButton;
		wxCustomButton* FillButton;
		wxCustomButton* EraserButton;
		wxRadioButton* CenterCenter;
		wxRadioButton* TopCenter;
		wxSpinCtrl* SpinCtrl1;
		wxSpinCtrl* SpinAngle;
		wxRadioButton* CenterLeft;
		wxCustomButton* SquareBrushButton;
		wxStaticText* StaticText6;
		wxCustomButton* BrushButton;
		wxRadioButton* BottomCenter;
		wxCustomButton* LineButton;
		wxCustomButton* PenButton;
		wxStaticText* StaticText8;
		wxPanel* AlignmentPanel;
		wxPanel* ToolPanel;
		wxButton* FontButton;
		wxCustomButton* PipetteButton;
		wxRadioButton* BottomLeft;
		wxColourPickerCtrl* HotSpotColourPicker;
		wxSpinCtrl* SpinCtrl3;
		wxStaticText* StaticText5;
		wxCustomButton* SelectButton;
		wxStaticText* StaticText7;
		wxCustomButton* EllipseButton;
		wxRadioButton* TopRight;
		wxCustomButton* RHairBrushButton;
		wxSpinCtrl* SpinCtrl2;
		wxToggleButton* BackgroundButton;
		wxCustomButton* LassoButton;
		wxCustomButton* RectangleButton;
		wxCustomButton* CircleBrushButton;
		wxPanel* Panel2;
		wxRadioButton* BottomRight;
		wxRadioButton* CenterRight;
		wxCustomButton* RRectButton;
		wxBoxSizer* AngleSizer;
		wxBoxSizer* ToolPanelSizer;
		wxStaticText* StaticText4;
		wxCustomButton* LHairBrushButton;
		wxCustomButton* CurveButton;
		wxCustomButton* TextButton;
		wxRadioButton* TopLeft;
		wxCustomButton* PolygonButton;
		//*)

	protected:

		//(*Identifiers(XPMToolPanel)
		static const long ID_SELECT_BUTN;
		static const long ID_LASSO_BTN;
		static const long ID_HOTSPOT_BTN;
		static const long ID_PEN_BTN;
		static const long ID_BRUSH_BTN;
		static const long ID_PIPETTE_BTN;
		static const long ID_FILL_BTN;
		static const long ID_LINE_BTN;
		static const long ID_CURVE_BTN;
		static const long ID_ERASER_BTN;
		static const long ID_TEXT_BTN;
		static const long ID_RECTANGLE_BTN;
		static const long ID_POLYGON_BTN;
		static const long ID_ELLIPSE_BTN;
		static const long ID_ROUNDEDRECT_BTN;
		static const long ID_SQUARE_BRUSH;
		static const long ID_CIRCLE_BRUSH;
		static const long ID_LRHAIR_BRUSH;
		static const long ID_LHAIR_BRUSH;
		static const long ID_CUSTOM2;
		static const long ID_FONT_BUTTON;
		static const long ID_BKMODE_TOGGLEBUTTON;
		static const long ID_STATICTEXT8;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON9;
		static const long ID_RADIOBUTTON8;
		static const long ID_RADIOBUTTON7;
		static const long ID_RADIOBUTTON6;
		static const long ID_RADIOBUTTON5;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON2;
		static const long ID_PANEL2;
		static const long ID_STATICTEXT9;
		static const long ID_SPINCTRL6;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRL3;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_SPINCTRL4;
		static const long ID_PANEL3;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(XPMToolPanel)
		void OnSelectButtonToggle(wxCommandEvent& event);
		void OnLassoButtonToggle(wxCommandEvent& event);
		void OnHotSpotButtonToggle(wxCommandEvent& event);
		void OnPenButtonToggle(wxCommandEvent& event);
		void OnBrushButtonToggle(wxCommandEvent& event);
		void OnPipetteButtonToggle(wxCommandEvent& event);
		void OnFillButtonToggle(wxCommandEvent& event);
		void OnLineButtonToggle(wxCommandEvent& event);
		void OnCurveButtonToggle(wxCommandEvent& event);
		void OnEraserButtonToggle(wxCommandEvent& event);
		void OnTextButtonToggle(wxCommandEvent& event);
		void OnRectangleButtonToggle(wxCommandEvent& event);
		void OnPolygonButtonToggle(wxCommandEvent& event);
		void OnEllipseButtonToggle(wxCommandEvent& event);
		void OnRRectButtonToggle(wxCommandEvent& event);
		void OnSquareBrushButtonToggle(wxCommandEvent& event);
		void OnCircleBrushButtonToggle(wxCommandEvent& event);
		void OnRHairBrushButtonToggle(wxCommandEvent& event);
		void OnLHairBrushButtonToggle(wxCommandEvent& event);
		void OnFontButtonClick(wxCommandEvent& event);
		void OnBackgroundButtonToggle(wxCommandEvent& event);
		void OnTopLeftSelect(wxCommandEvent& event);
		void OnTopCenterSelect(wxCommandEvent& event);
		void OnTopRightSelect(wxCommandEvent& event);
		void OnCenterLeftSelect(wxCommandEvent& event);
		void OnCenterCenterSelect(wxCommandEvent& event);
		void OnCenterRightSelect(wxCommandEvent& event);
		void OnBottomLeftSelect(wxCommandEvent& event);
		void OnBottomCenterSelect(wxCommandEvent& event);
		void OnBottomRightSelect(wxCommandEvent& event);
		void OnSpinAngleChange(wxSpinEvent& event);
		void OnSpinSizeChanged(wxSpinEvent& event);
		void OnSpinThicknessChanged(wxSpinEvent& event);
		void OnSpinRadiusChanged(wxSpinEvent& event);
		//*)

		XPMEditorPanel *m_parent; ///< \brief the parent panel
		int iToolUsed;           ///< \brief the index of the tool currently in use
		wxCustomButton* tools[XPM_NUMBER_TOOLS]; ///< \brief the buttons associated to the tools
		wxCursor ToolCursor[XPM_NUMBER_TOOLS]; ///< \brief the cursors associated to the tools

		DECLARE_EVENT_TABLE()
};

#endif
