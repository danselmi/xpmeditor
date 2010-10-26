/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMDrawCanvasPanel.cpp
 * Purpose:   wxPanel containing the status bar, and the area where the image is drawn - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "XPMDrawCanvasPanel.h"
#include "XPMEditorPanel.h"

//(*InternalHeaders(XPMDrawCanvasPanel)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMDrawCanvasPanel)
const long XPMDrawCanvasPanel::ID_SCROLLEDWINDOW1 = wxNewId();
const long XPMDrawCanvasPanel::ID_STATICTEXT2 = wxNewId();
const long XPMDrawCanvasPanel::ID_TEXTCTRL1 = wxNewId();
const long XPMDrawCanvasPanel::ID_STATICTEXT4 = wxNewId();
const long XPMDrawCanvasPanel::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMDrawCanvasPanel,wxPanel)
	//(*EventTable(XPMDrawCanvasPanel)
	//*)
END_EVENT_TABLE()

XPMDrawCanvasPanel::XPMDrawCanvasPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, const wxString &name)
{
	sTextSizer = NULL; //to avoid a race condition when building the panel. The parent call XPMDrawCanvasPanel::ShowTextSizer
	                   //in its constructor. The pointer is then not initialized correctly: leads to crash
	BuildContent(parent,id,pos,size);
	if (sTextSizer) sTextSizer->Show(false);
	Layout();
}

void XPMDrawCanvasPanel::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMDrawCanvasPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	CanvasSizer = new wxBoxSizer(wxVERTICAL);
	DrawCanvas = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxSUNKEN_BORDER, _T("ID_SCROLLEDWINDOW1"));
	DrawCanvas->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	CanvasSizer->Add(DrawCanvas, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	sTextSizer = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Text:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	sTextSizer->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	TextEdit = new wxTextCtrl(this, ID_TEXTCTRL1, _("Text"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_LEFT|wxTE_CHARWRAP, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	sTextSizer->Add(TextEdit, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CanvasSizer->Add(sTextSizer, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	sCursorPos = new wxStaticText(this, ID_STATICTEXT4, _("Cursor at: x=00000 y=00000"), wxDefaultPosition, wxSize(170,21), wxST_NO_AUTORESIZE|wxSUNKEN_BORDER, _T("ID_STATICTEXT4"));
	BoxSizer1->Add(sCursorPos, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_LEFT|wxSUNKEN_BORDER, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	CanvasSizer->Add(BoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(CanvasSizer);
	CanvasSizer->Fit(this);
	CanvasSizer->SetSizeHints(this);
	
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&XPMDrawCanvasPanel::OnTextEditText);
	//*)
}

XPMDrawCanvasPanel::~XPMDrawCanvasPanel()
{
	//(*Destroy(XPMDrawCanvasPanel)
	//*)
}


void XPMDrawCanvasPanel::OnTextEditText(wxCommandEvent& event)
{
    XPMEditorPanel *parent;
    parent = (XPMEditorPanel *) GetParent();
    if (parent) parent->OnTextEditText(event.GetString());
}

/** Hide or show the text controls used by the Text Tool
  * \param bShow : true to show the tool, false to hide them
  */
void XPMDrawCanvasPanel::ShowTextSizer(bool bShow)
{
    if (sTextSizer)
    {
        sTextSizer->Show(bShow);
        Layout();
    }
}

/** change the text inside the Text Box used by the Text Tool
  * \param bShow : true to show the tool, false to hide them
  */
void XPMDrawCanvasPanel::SetTextValue(wxString sText)
{
    if (TextEdit)
    {
        TextEdit->ChangeValue(sText);
    }
}
