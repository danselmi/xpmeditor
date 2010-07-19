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

#include <wx/dynarray.h>
#include <wx/arrstr.h>

#define XPM_FORMAT_INDEX_AUTO    0
#define XPM_FORMAT_INDEX_BMP     1
#define XPM_FORMAT_INDEX_CUR     2
#define XPM_FORMAT_INDEX_ICO     3
#define XPM_FORMAT_INDEX_JPG     4
#define XPM_FORMAT_INDEX_PCX     5
#define XPM_FORMAT_INDEX_PNG     6
#define XPM_FORMAT_INDEX_PNM     7
#define XPM_FORMAT_INDEX_TIF     8
#define XPM_FORMAT_INDEX_XPM     9


class XPMEditorPanel;

class XPMImagePropertiesPanel: public wxPanel
{
	public:

		XPMImagePropertiesPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMImagePropertiesPanel();

		void SetParentPanel(XPMEditorPanel *p); ///< \brief set the parent panel for this tool panel
		void SetImageFormat(wxBitmapType bt);   /// \brief set the image saving file format in the wxChoice

		//(*Declarations(XPMImagePropertiesPanel)
		wxStaticBoxSizer* StaticBoxSizer2;
		wxSpinCtrl* BMPHeight;
		wxSpinCtrl* BMPWidth;
		wxButton* Button1;
		wxStaticText* StaticText1;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxChoice* Choice1;
		//*)

	protected:

		//(*Identifiers(XPMImagePropertiesPanel)
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL2;
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
		wxArrayString m_sFormats;
		wxArrayInt m_iIndex;

		void AppendFormatsToChoice(void); ///< \brief Populate the wxChoice with the available formats

		DECLARE_EVENT_TABLE()
};

#endif
