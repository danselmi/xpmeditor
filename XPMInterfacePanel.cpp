/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMImageInterfacePanel.cpp
 * Purpose:   A panel containing image display option interface - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "XPMInterfacePanel.h"
#include "XPMEditorPanel.h"
#include "XPMCustomZoom.h"

//(*InternalHeaders(XPMInterfacePanel)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//button bitmaps
#include "xpm/left.xpm"
#include "xpm/right.xpm"

//(*IdInit(XPMInterfacePanel)
const long XPMInterfacePanel::ID_STATICTEXT1 = wxNewId();
const long XPMInterfacePanel::ID_COMBOBOX1 = wxNewId();
const long XPMInterfacePanel::ID_BITMAPBUTTON1 = wxNewId();
const long XPMInterfacePanel::ID_BITMAPBUTTON2 = wxNewId();
const long XPMInterfacePanel::ID_CHECKBOX1 = wxNewId();
const long XPMInterfacePanel::ID_CUSTOM3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMInterfacePanel,wxPanel)
	//(*EventTable(XPMInterfacePanel)
	//*)
END_EVENT_TABLE()

XPMInterfacePanel::XPMInterfacePanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMInterfacePanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("ZOOM:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ZoomFactor = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOX1"));
	ZoomFactor->Append(_("10%"));
	ZoomFactor->Append(_("25%"));
	ZoomFactor->Append(_("50%"));
	ZoomFactor->Append(_("75%"));
	ZoomFactor->Append(_("100%"));
	ZoomFactor->Append(_("125%"));
	ZoomFactor->Append(_("150%"));
	ZoomFactor->Append(_("175%"));
	ZoomFactor->Append(_("200%"));
	ZoomFactor->Append(_("400%"));
	ZoomFactor->Append(_("800%"));
	ZoomFactor->Append(_("1600%"));
	ZoomFactor->Append(_("Custom"));
	ZoomFactor->SetToolTip(_("Zoom selection"));
	BoxSizer2->Add(ZoomFactor, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton1 = new wxBitmapButton(this, ID_BITMAPBUTTON1, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
	BitmapButton1->SetToolTip(_("Rotate the Image 90 degrees counter-clockwise."));
	BoxSizer2->Add(BitmapButton1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton2 = new wxBitmapButton(this, ID_BITMAPBUTTON2, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
	BitmapButton2->SetToolTip(_("Rotate the Image 90 degrees clockwise."));
	BoxSizer2->Add(BitmapButton2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox1 = new wxCheckBox(this, ID_CHECKBOX1, _("Show Grid"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox1->SetValue(false);
	CheckBox1->Disable();
	CheckBox1->SetToolTip(_("Show / Hide the grid. A minimal Zoom of 400% is needed."));
	BoxSizer2->Add(CheckBox1, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	GridColour = new wxColourPickerCtrl(this,ID_CUSTOM3,*wxBLACK,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_CUSTOM3"));
	BoxSizer2->Add(GridColour, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer2);
	BoxSizer2->Fit(this);
	BoxSizer2->SetSizeHints(this);
	
	Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&XPMInterfacePanel::OnZoomChanged);
	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMInterfacePanel::OnRotateCounterClockwise);
	Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMInterfacePanel::OnRotateClockwise);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&XPMInterfacePanel::OnShowGrid);
	//*)

	m_parent = NULL;

	GridColour->Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&XPMInterfacePanel::OnGridColourPickerColourChanged,0,this);

	 //Bitmap Button rotate
    BitmapButton2->SetBitmapLabel(left_xpm);
    BitmapButton1->SetBitmapLabel(right_xpm);
}

XPMInterfacePanel::~XPMInterfacePanel()
{
	//(*Destroy(XPMInterfacePanel)
	//*)
}

/** Set the parent panel
  * \param [in] a pointer to the new parent
  */
void XPMInterfacePanel::SetParentPanel(XPMEditorPanel *p)
{
    m_parent = p;
}

/** The user has changed the Zoom factor in the ComboBox "Zoom"
  */
void XPMInterfacePanel::OnZoomChanged(wxCommandEvent& event)
{
    if (m_parent)
    {
        wxString s;
        double dScale2;

        s = ZoomFactor->GetValue();
        s.Replace(_("%"), _(""));
        if (s.ToDouble(&dScale2))
        {
            if (dScale2 < 0) dScale2 = - dScale2;
            if (dScale2 == 0.0) return;
            dScale2 = dScale2 / 100;
            m_parent->SetScaleFactor(dScale2);
        }
        else if (s.Find(_("Custom")) >= 0)
        {
            XPMCustomZoom cz(m_parent);
            long lValue;
            double dScale;
            dScale = m_parent->GetScaleFactor();
            lValue = dScale * 100;
            wxString sCurrentZoom = wxString::Format(_("%d"), lValue);
            cz.TextCtrl1->SetValue(sCurrentZoom);
            if (cz.ShowModal())
            {
                ZoomFactor->SetValue(wxString::Format(_("%d%%"), dScale * 100));
            }
        }
    }
}

/** Rotate 90 degree counter-clockwise handler **/
void XPMInterfacePanel::OnRotateCounterClockwise(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->RotateCounterClockwise();
    }
}

/** Rotate 90 degree clockwise handler **/
void XPMInterfacePanel::OnRotateClockwise(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->RotateClockwise();
    }
}

/** The Grid must be displayed **/
void XPMInterfacePanel::OnShowGrid(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->ShowGrid(CheckBox1->IsChecked());
    }
}

/** The Grid colour must be updated
  */
void XPMInterfacePanel::OnGridColourPickerColourChanged(wxColourPickerEvent& event)
{
    if (m_parent)
    {
        m_parent->OnGridColourPickerColourChanged(event);
    }

}
