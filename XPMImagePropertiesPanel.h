/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImagePropertiesPanel.h
 * Purpose:   A panel containing the image properties: size, file format, compression, resolution - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPMIMAGEPROPERTIESPANEL_H
#define XPMIMAGEPROPERTIESPANEL_H

//(*Headers(XPMImagePropertiesPanel)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
//*)

class XPMEditorPanel;

class XPMImagePropertiesPanel: public wxPanel
{
	public:

		XPMImagePropertiesPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMImagePropertiesPanel();

		void SetParentPanel(XPMEditorPanel *p); ///< \brief set the parent panel for this tool panel

		//(*Declarations(XPMImagePropertiesPanel)
		wxSpinCtrl* BMPHeight;
		wxSpinCtrl* BMPWidth;
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxStaticText* StaticText3;
		wxStaticText* StaticText12;
		wxChoice* Choice1;
		//*)

	protected:

		//(*Identifiers(XPMImagePropertiesPanel)
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRL2;
		static const long ID_STATICTEXT13;
		static const long ID_CHOICE1;
		static const long ID_BUTTON1;
		//*)

	private:

		//(*Handlers(XPMImagePropertiesPanel)
		void OnButtonAdvancedPropClick(wxCommandEvent& event);
		void OnImageTypeChanged(wxCommandEvent& event);
		void OnImageSizeChanged(wxSpinEvent& event);
		//*)

		XPMEditorPanel *m_parent;

		DECLARE_EVENT_TABLE()
};

#endif
