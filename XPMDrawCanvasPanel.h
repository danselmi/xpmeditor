#ifndef XPMDRAWCANVASPANEL_H
#define XPMDRAWCANVASPANEL_H

//(*Headers(XPMDrawCanvasPanel)
#include <wx/panel.h>
class wxTextCtrl;
class wxScrolledWindow;
class wxStaticText;
class wxBoxSizer;
//*)

class XPMDrawCanvasPanel: public wxPanel
{
	public:

		XPMDrawCanvasPanel(wxWindow* parent,
                           wxWindowID id=wxID_ANY,
                           const wxPoint& pos=wxDefaultPosition,
                           const wxSize& size=wxDefaultSize,
                           const wxString &name=_("DrawCanvasPanel")
                          );
		virtual ~XPMDrawCanvasPanel();

		void ShowTextSizer(bool bShow); ///< \brief Hide or show the text controls used by the Text Tool
		void SetTextValue(wxString sText); ///< \brief change the text inside the Text Box used by the Text Tool

		//(*Declarations(XPMDrawCanvasPanel)
		wxStaticText* sCursorPos;
		wxScrolledWindow* DrawCanvas;
		wxStaticText* StaticText2;
		wxBoxSizer* CanvasSizer;
		wxTextCtrl* TextEdit;
		wxBoxSizer* sTextSizer;
		wxBoxSizer* BoxSizer1;
		wxTextCtrl* TextCtrl1;
		//*)

	protected:

		//(*Identifiers(XPMDrawCanvasPanel)
		static const long ID_SCROLLEDWINDOW1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL2;
		//*)

		//(*Handlers(XPMDrawCanvasPanel)
		void OnTextEditText(wxCommandEvent& event);
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
