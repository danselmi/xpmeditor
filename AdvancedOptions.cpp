/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      AdvancedOptions.cpp
 * Purpose:   a dialog box for editing compressions options of the image - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "AdvancedOptions.h"

//(*InternalHeaders(AdvancedOptions)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(AdvancedOptions)
const long AdvancedOptions::ID_STATICTEXT7 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT1 = wxNewId();
const long AdvancedOptions::ID_SPINCTRL1 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT2 = wxNewId();
const long AdvancedOptions::ID_SPINCTRL2 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT3 = wxNewId();
const long AdvancedOptions::ID_CHOICE1 = wxNewId();
const long AdvancedOptions::ID_CHECKBOX1 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT4 = wxNewId();
const long AdvancedOptions::ID_SLIDER1 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT5 = wxNewId();
const long AdvancedOptions::ID_CHECKBOX2 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT6 = wxNewId();
const long AdvancedOptions::ID_CHOICE2 = wxNewId();
const long AdvancedOptions::ID_CHECKBOX3 = wxNewId();
const long AdvancedOptions::ID_STATICTEXT8 = wxNewId();
const long AdvancedOptions::ID_CHOICE5 = wxNewId();
const long AdvancedOptions::ID_CHECKBOX4 = wxNewId();
const long AdvancedOptions::ID_BUTTONOK = wxNewId();
const long AdvancedOptions::ID_BUTTONCANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(AdvancedOptions,wxDialog)
	//(*EventTable(AdvancedOptions)
	//*)
END_EVENT_TABLE()

AdvancedOptions::AdvancedOptions(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
}

void AdvancedOptions::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AdvancedOptions)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, id, _("Advanced Image Options"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("These options will take effect only after saving the image and reloading it."), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("ID_STATICTEXT7"));
	BoxSizer1->Add(StaticText7, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("General"));
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Resolution X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer3->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinResolutionX = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 10000, 0, _T("ID_SPINCTRL1"));
	SpinResolutionX->SetValue(_T("0"));
	BoxSizer3->Add(SpinResolutionX, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Resolution Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer3->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinResolutionY = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 10000, 0, _T("ID_SPINCTRL2"));
	SpinResolutionY->SetValue(_T("0"));
	BoxSizer3->Add(SpinResolutionY, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Resolution Unit:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer5->Add(StaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ChoiceResolutionUnit = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	ChoiceResolutionUnit->Append(_("Not Specified"));
	ChoiceResolutionUnit->Append(_("Inch"));
	ChoiceResolutionUnit->Append(_("Cm"));
	BoxSizer5->Add(ChoiceResolutionUnit, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	CheckBoxResolutionOption = new wxCheckBox(this, ID_CHECKBOX1, _("Has Resolution option"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBoxResolutionOption->SetValue(false);
	BoxSizer5->Add(CheckBoxResolutionOption, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	BoxSizer1->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("JPG Quality"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Poor Quality + small size "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer6->Add(StaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SliderJPGCompression = new wxSlider(this, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER1"));
	BoxSizer6->Add(SliderJPGCompression, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Good Quality + large size"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer6->Add(StaticText5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer6, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	CheckBoxJPGCompression = new wxCheckBox(this, ID_CHECKBOX2, _("Has JPG Compression option"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBoxJPGCompression->SetValue(false);
	StaticBoxSizer2->Add(CheckBoxJPGCompression, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("PNG Format Options"));
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("PNG Format:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer7->Add(StaticText6, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ChoicePNGFormat = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	ChoicePNGFormat->Append(_("0 - Colour PNG"));
	ChoicePNGFormat->Append(_("2 - Grey Scale PNG"));
	ChoicePNGFormat->Append(_("3 - Grey Scale using Red as Grey"));
	BoxSizer7->Add(ChoicePNGFormat, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	CheckBoxPNGFormat = new wxCheckBox(this, ID_CHECKBOX3, _("Has PNG Format"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBoxPNGFormat->SetValue(false);
	BoxSizer7->Add(CheckBoxPNGFormat, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(BoxSizer7, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Bit Depth for R/G/B/A:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	BoxSizer8->Add(StaticText8, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ChoicePNGBitDepths = new wxChoice(this, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE5"));
	ChoicePNGBitDepths->Append(_("8"));
	ChoicePNGBitDepths->Append(_("16"));
	BoxSizer8->Add(ChoicePNGBitDepths, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBoxPNGBitDepth = new wxCheckBox(this, ID_CHECKBOX4, _("Has Bit Depth"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	CheckBoxPNGBitDepth->SetValue(false);
	BoxSizer8->Add(CheckBoxPNGBitDepth, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(BoxSizer8, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	OKButton = new wxButton(this, ID_BUTTONOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	BoxSizer2->Add(OKButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	CancelButton = new wxButton(this, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer2->Add(CancelButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnCheckBoxResolutionOptionClick);
	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnCheckBoxJPGCompressionClick);
	Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnCheckBoxPNGOptionsClick);
	Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnCheckBoxPNGBitDepthClick);
	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnOKButtonClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AdvancedOptions::OnCancelButtonClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&AdvancedOptions::OnClose);
	//*)
}

AdvancedOptions::~AdvancedOptions()
{
	//(*Destroy(AdvancedOptions)
	//*)
}


void AdvancedOptions::OnClose(wxCloseEvent& event)
{
    EndModal(1);
}

void AdvancedOptions::OnOKButtonClick(wxCommandEvent& event)
{
    EndModal(0);
}

void AdvancedOptions::OnCancelButtonClick(wxCommandEvent& event)
{
    EndModal(1);
}

/** activate / deactivate resolution widgets
  * \param bEnabled: if true, it will enable the widgets
  *                  if false, it will disable them
  */
void AdvancedOptions::EnableResolutionOption(bool bEnabled)
{
    ChoiceResolutionUnit->Enable(bEnabled);
    SpinResolutionX->Enable(bEnabled);
    SpinResolutionY->Enable(bEnabled);
    CheckBoxResolutionOption->SetValue(bEnabled);
}

/** activate / deactivate JPG quality widgets
  * \param bEnabled: if true, it will enable the widgets
  *                  if false, it will disable them
  */
void AdvancedOptions::EnableJPGOption(bool bEnabled)
{
    SliderJPGCompression->Enable(bEnabled);
    CheckBoxJPGCompression->SetValue(bEnabled);
}

/** activate / deactivate PNG Format widgets
  * \param bEnabled: if true, it will enable the widgets
  *                  if false, it will disable them
  */
void AdvancedOptions::EnablePNGFormat(bool bEnabled)
{
    ChoicePNGFormat->Enable(bEnabled);
    CheckBoxPNGFormat->SetValue(bEnabled);
}

/** activate / deactivate PNG Bit Depth widgets
  * \param bEnabled: if true, it will enable the widgets
  *                  if false, it will disable them
  */
void AdvancedOptions::EnablePNGBitsDepth(bool bEnabled)
{
    ChoicePNGBitDepths->Enable(bEnabled);
    CheckBoxPNGBitDepth->SetValue(bEnabled);
}

void AdvancedOptions::OnCheckBoxResolutionOptionClick(wxCommandEvent& event)
{
    EnableResolutionOption(event.IsChecked());
}

void AdvancedOptions::OnCheckBoxJPGCompressionClick(wxCommandEvent& event)
{
    EnableJPGOption(event.IsChecked());
}

void AdvancedOptions::OnCheckBoxPNGOptionsClick(wxCommandEvent& event)
{
    EnablePNGFormat(event.IsChecked());
}

void AdvancedOptions::OnCheckBoxPNGBitDepthClick(wxCommandEvent& event)
{
    EnablePNGBitsDepth(event.IsChecked());
}
