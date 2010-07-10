#ifndef XPMDRAWCANVASPANEL_H
#define XPMDRAWCANVASPANEL_H

//(*Headers(XPMDrawCanvasPanel)
#include <wx/panel.h>
class wxTextCtrl;
class wxScrolledWindow;
class wxStaticText;
class wxBoxSizer;
class wxResizeCtrl;
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

		//(*Declarations(XPMDrawCanvasPanel)
		wxStaticText* sCursorPos;
		wxScrolledWindow* DrawCanvas;
		wxBoxSizer* CanvasSizer;
		wxTextCtrl* TextEdit;
		wxResizeCtrl* ResizeCtrl1;
		//*)

	protected:

		//(*Identifiers(XPMDrawCanvasPanel)
		static const long ID_TEXTCTRL1;
		static const long ID_RESIZECTRL1;
		static const long ID_SCROLLEDWINDOW1;
		static const long ID_STATICTEXT4;
		//*)

		//(*Handlers(XPMDrawCanvasPanel)
		//*)

	protected:

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);

		DECLARE_EVENT_TABLE()
};

#endif
