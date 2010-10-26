/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMColourPickerPanel.cpp
 * Purpose:   A simple panel containing only a ColourPicker widgets - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-05-19
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "XPMColourPickerPanel.h"
#include "XPMEditorPanel.h"

//(*InternalHeaders(XPMColourPickerPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(XPMColourPickerPanel)
const long XPMColourPickerPanel::ID_COLOURPICKER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMColourPickerPanel,wxPanel)
	//(*EventTable(XPMColourPickerPanel)
	//*)
END_EVENT_TABLE()

XPMColourPickerPanel::XPMColourPickerPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMColourPickerPanel)
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	ColourPicker = new XPMColorPicker(this,ID_COLOURPICKER1,wxDefaultPosition,wxDefaultSize,0,_T("ID_COLOURPICKER1"));
	BoxSizer1->Add(ColourPicker, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	ColourPicker->Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMColourPickerPanel::OnColourPickerMouseEnter,0,this);
	//*)

	Connect(ID_COLOURPICKER1,wxEVT_TRANSPARENT_COLOR_CHANGED,(wxObjectEventFunction)&XPMColourPickerPanel::OnTransparentColorChanged);
	Connect(ID_COLOURPICKER1,wxEVT_FILL_COLOR_CHANGED,(wxObjectEventFunction)&XPMColourPickerPanel::OnFillColorChanged);
	Connect(ID_COLOURPICKER1,wxEVT_LINE_COLOR_CHANGED,(wxObjectEventFunction)&XPMColourPickerPanel::OnLineColorChanged);

	m_parent = NULL;
}

XPMColourPickerPanel::~XPMColourPickerPanel()
{
	//(*Destroy(XPMColourPickerPanel)
	//*)
}

//-------- INTERFACE METHODS ----------------
/** Set the parent panel
  * \param [in] a pointer to the new parent
  */
void XPMColourPickerPanel::SetParentPanel(XPMEditorPanel *p)
{
    m_parent = p;
}

/** The transparent colour changed
  */
void XPMColourPickerPanel::OnTransparentColorChanged(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnTransparentColorChanged(event);
    }
}

/** The line colour changed
  */
void XPMColourPickerPanel::OnLineColorChanged(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnLineColorChanged(event);
    }
}

/** The fill colour changed
  */
void XPMColourPickerPanel::OnFillColorChanged(wxCommandEvent& event)
{
    if (m_parent)
    {
        m_parent->OnFillColorChanged(event);
    }
}

/** Initialize default colour values
  */
void XPMColourPickerPanel::init_colours(void)
{
    if (ColourPicker) ColourPicker->init_colours();
}

/** immediate repainting of the widgets
  */
void XPMColourPickerPanel::Repaint(void)
{
    if (ColourPicker) ColourPicker->Repaint();
}


/** Return the current line colour
  * If an error occur, the colour returned is BLACK by default
  * \return a wxColour representing the current line colour
  */
wxColour XPMColourPickerPanel::GetLineColour(void)
{
    if (ColourPicker) return(ColourPicker->GetLineColour());
    return(wxColour(0,0,0));
}

/** Return the current fill colour
  * If an error occur, the colour returned is WHITE by default
  * \return a wxColour representing the current fill colour
  */
wxColour XPMColourPickerPanel::GetFillColour(void)
{
    if (ColourPicker) return(ColourPicker->GetFillColour());
    return(wxColour(255,255,255));
}

/** return the 1st unused colour
  * Unused colour are: not Fill Colour, not Line Colour, not transparent colour
  * This method will return a colour outside the palette if there are 3 colours or less in the current palette
  */
wxColour XPMColourPickerPanel::GetUnusedColour(void)
{
    if (ColourPicker) return(ColourPicker->GetUnusedColour());
    return(wxColour(0,0,0));
}

/** Return the current line colour index
  * \return the index representing the current line colour
  */
int XPMColourPickerPanel::GetLineColourIndex(void)
{
    if (ColourPicker) return(ColourPicker->GetLineColourIndex());
    return(-1);
}

/** Return the current fill colour index
  * \return the index representing the current fill colour
  */
int XPMColourPickerPanel::GetFillColourIndex(void)
{
     if (ColourPicker) return(ColourPicker->GetFillColourIndex());
    return(-1);
}

/** Set the current line colour
  * If an error occur, the colour set is the 1st colour of the palette by default
  * \param iIndex: the index in the wxColourArray representing the new line colour
  * \param bPostEvent: if true, a wxEVT_LINE_COLOR_CHANGED is generated (default)
  */
void XPMColourPickerPanel::SetLineColour(int iIndex, bool bPostEvent)
{
    if (ColourPicker) ColourPicker->SetLineColour(iIndex, bPostEvent);
}

/** Set the current fill colour
  * If an error occur, the colour set is the 1st colour of the palette by default
  * \param iIndex: the index in the wxColourArray representing the new fill colour
  * \param bPostEvent: if true, a wxEVT_FILL_COLOR_CHANGED is generated (default)
  */
void XPMColourPickerPanel::SetFillColour(int iIndex, bool bPostEvent)
{
    if (ColourPicker) ColourPicker->SetFillColour(iIndex, bPostEvent);
}

/** get the palette size
  * \return the amount of colours in the palette
  */
int XPMColourPickerPanel::GetPaletteSize(void)
{
    if (ColourPicker) return(ColourPicker->GetPaletteSize());
    return(-1);
}

/** set the palette size
  * if the new palette is larger, then all the new colours are initialized to blakc
  * \param iSize: the new amount of colours in the palette
  * \return true on success, false on failure
  */
bool XPMColourPickerPanel::SetPaletteSize(int iSize)
{
    if (ColourPicker) return(ColourPicker->SetPaletteSize(iSize));
    return(false);
}

/** set the specific palette colour
  * \param iIndex: the index of the colour to be changed. If not valid,nothing is done
  * \param cColor: the new colour
  */
void XPMColourPickerPanel::SetPaletteColour(int iIndex, wxColour cColor)
{
    if (ColourPicker) ColourPicker->SetPaletteColour(iIndex, cColor);
}

/** get the specific palette colour
  * \param iIndex: the index of the colour to be changed. If not valid, wxBLACK is returned
  * \return  the colour
  */
wxColour XPMColourPickerPanel::GetPaletteColour(int iIndex)
{
    if (ColourPicker) return(ColourPicker->GetPaletteColour(iIndex));
    return(wxColour(0,0,0));
}

/** Get the transparent colour
  * \return : the transparent colour
  */
wxColour XPMColourPickerPanel::GetTransparentColour(void)
{
    if (ColourPicker) return(ColourPicker->GetTransparentColour());
    return(wxColour(0,0,0));
}

/** Set the transparent colour
  * \param cColour: the new transparent colour
  * \param bPostEvent: if true, a wxEVT_TRANSPARENT_COLOR_CHANGED is generated (default)
  */
void XPMColourPickerPanel::SetTransparentColour(wxColour cColour, bool bPostEvent)
{
    if (ColourPicker) ColourPicker->SetTransparentColour(cColour, bPostEvent);
}


void XPMColourPickerPanel::OnColourPickerMouseEnter(wxMouseEvent& event)
{
    if (m_parent)
    {
        m_parent->DisplayHelpText(0);
    }

    event.Skip();
}
