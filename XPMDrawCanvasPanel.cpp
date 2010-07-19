#include "XPMDrawCanvasPanel.h"

//(*InternalHeaders(XPMDrawCanvasPanel)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include "wxResizeCtrl.h"
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMDrawCanvasPanel)
const long XPMDrawCanvasPanel::ID_TEXTCTRL1 = wxNewId();
const long XPMDrawCanvasPanel::ID_RESIZECTRL1 = wxNewId();
const long XPMDrawCanvasPanel::ID_SCROLLEDWINDOW1 = wxNewId();
const long XPMDrawCanvasPanel::ID_STATICTEXT4 = wxNewId();
const long XPMDrawCanvasPanel::ID_TEXTCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMDrawCanvasPanel,wxPanel)
	//(*EventTable(XPMDrawCanvasPanel)
	//*)
END_EVENT_TABLE()

XPMDrawCanvasPanel::XPMDrawCanvasPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, const wxString &name)
{
	BuildContent(parent,id,pos,size);
}

void XPMDrawCanvasPanel::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMDrawCanvasPanel)
	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	CanvasSizer = new wxBoxSizer(wxVERTICAL);
	DrawCanvas = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxSUNKEN_BORDER, _T("ID_SCROLLEDWINDOW1"));
	DrawCanvas->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	TextEdit = new wxTextCtrl(DrawCanvas, ID_TEXTCTRL1, _("Text"), wxPoint(57,126), wxDefaultSize, wxTE_MULTILINE|wxTE_LEFT|wxTE_CHARWRAP, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	ResizeCtrl1 = new wxResizeCtrl(DrawCanvas,ID_RESIZECTRL1,wxPoint(6,7),wxSize(119,74),0,_T("ID_RESIZECTRL1"));
	CanvasSizer->Add(DrawCanvas, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	sCursorPos = new wxStaticText(this, ID_STATICTEXT4, _("Cursor at: x=00000 y=00000"), wxDefaultPosition, wxSize(170,21), wxST_NO_AUTORESIZE|wxSUNKEN_BORDER, _T("ID_STATICTEXT4"));
	BoxSizer1->Add(sCursorPos, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	TextCtrl1->Disable();
	BoxSizer1->Add(TextCtrl1, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	CanvasSizer->Add(BoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SetSizer(CanvasSizer);
	CanvasSizer->Fit(this);
	CanvasSizer->SetSizeHints(this);
	//*)
}

XPMDrawCanvasPanel::~XPMDrawCanvasPanel()
{
	//(*Destroy(XPMDrawCanvasPanel)
	//*)
}

