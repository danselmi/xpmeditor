/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMColorPicker.cpp
 * Purpose:   a simple wxWidget control presenting a panel of colours - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#include "XPMColorPicker.h"

//#include <sdk.h> // Code::Blocks SDK
#include <wx/gdicmn.h>
#include <wx/dcclient.h>
#include <wx/brush.h>
#include <wx/pen.h>
#include <wx/tglbtn.h>
#include <wx/msgdlg.h>
#include <wx/colordlg.h>

DEFINE_EVENT_TYPE(wxEVT_TRANSPARENT_COLOR_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_LINE_COLOR_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_FILL_COLOR_CHANGED)

/** CONSTRUCTOR
  */
const long XPMColorPicker::ID_TOGGLEBUTTON1 = wxNewId();
XPMColorPicker::XPMColorPicker(wxWindow* parent,
                                   wxWindowID id,
                                   const wxPoint& pos,
                                   const wxSize& size,
                                   int style,
                                   const wxValidator &validator,
                                   const wxString& name
                                  )
{
    //ctor

    //initialize the colour array
    init_colours();

    //initialize the Best size information
    wxSize sBestSize;
    sBestSize = DoGetBestSize();
    SetMinSize(sBestSize);

    //::wxMessageBox(wxString::Format(_("width=%d heigth=%d"), sBestSize.GetWidth(), sBestSize.GetHeight()), _("INFO"), wxOK);

    Create(parent, id, pos, sBestSize, wxTAB_TRAVERSAL, _T("id"));
    //CreateControl(parent, id, pos, sBestSize, style, validator, name);
    ToggleButton1 = new wxToggleButton(this, ID_TOGGLEBUTTON1, _(" LINE "), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON1"));

    bLineOn = true;

    //initializations
    ComputeDimensions(sBestSize);
    if (iHeight > sBestSize.GetHeight()) iHeight = sBestSize.GetHeight();
    iLineColor = 0;  //black
    iFillColor = XPM_MAX_COLOR - 1; //white



    //event handlers
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&XPMColorPicker::OnEventSize);
    Connect(wxEVT_PAINT,(wxObjectEventFunction)&XPMColorPicker::OnEventPaint);
    Connect(ID_TOGGLEBUTTON1,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMColorPicker::OnToggleButton1Toggle);
    //mouse events handlers
    Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&XPMColorPicker::OnMouseEnter);
    Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMColorPicker::OnMouseLeave);
    Connect(wxEVT_SET_CURSOR,(wxObjectEventFunction)&XPMColorPicker::OnSetCursor);
    Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&XPMColorPicker::OnMouseClick);
    Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&XPMColorPicker::OnMouseRClick);
    Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&XPMColorPicker::OnMouseDClick);
}

/** Initialize default colour values
  */
void XPMColorPicker::init_colours(void)
{
    //init the array of colours

    iPaletteSize = XPM_MAX_COLOR;
    ColourArray = new wxColour[iPaletteSize];
    if (!ColourArray) return;

    //Black
    ColourArray[0] = wxTheColourDatabase->Find(_("BLACK"));

    //purple hues
    ColourArray[1] = wxTheColourDatabase->Find(_("BLUE VIOLET"));
    ColourArray[2] = wxTheColourDatabase->Find(_("MAGENTA"));
    ColourArray[3] = wxTheColourDatabase->Find(_("VIOLET"));
    ColourArray[4] = wxTheColourDatabase->Find(_("VIOLET RED"));
    ColourArray[5] = wxTheColourDatabase->Find(_("MEDIUM VIOLET RED"));
    ColourArray[6] = wxTheColourDatabase->Find(_("PINK"));
    ColourArray[7] = wxTheColourDatabase->Find(_("PLUM"));
    ColourArray[8] = wxTheColourDatabase->Find(_("PURPLE"));
    ColourArray[9] = wxTheColourDatabase->Find(_("SALMON"));

    //blue hues
    ColourArray[10] = wxTheColourDatabase->Find(_("BLUE"));
    ColourArray[11] = wxTheColourDatabase->Find(_("AQUAMARINE"));
    ColourArray[12] = wxTheColourDatabase->Find(_("CADET BLUE"));
    ColourArray[13] = wxTheColourDatabase->Find(_("CORAL"));
    ColourArray[14] = wxTheColourDatabase->Find(_("CORNFLOWER BLUE"));
    ColourArray[15] = wxTheColourDatabase->Find(_("MEDIUM AQUAMARINE"));
    ColourArray[16] = wxTheColourDatabase->Find(_("MEDIUM BLUE"));
    ColourArray[17] = wxTheColourDatabase->Find(_("CYAN"));
    ColourArray[18] = wxTheColourDatabase->Find(_("DARK SLATE BLUE"));
    ColourArray[19] = wxTheColourDatabase->Find(_("DARK TURQUOISE"));
    ColourArray[20] = wxTheColourDatabase->Find(_("LIGHT BLUE"));
    ColourArray[21] = wxTheColourDatabase->Find(_("LIGHT STEEL BLUE"));
    ColourArray[22] = wxTheColourDatabase->Find(_("MEDIUM SLATE BLUE"));
    ColourArray[23] = wxTheColourDatabase->Find(_("MEDIUM TURQUOISE"));
    ColourArray[24] = wxTheColourDatabase->Find(_("MIDNIGHT BLUE"));
    ColourArray[25] = wxTheColourDatabase->Find(_("NAVY"));
    ColourArray[26] = wxTheColourDatabase->Find(_("SKY BLUE"));
    ColourArray[27] = wxTheColourDatabase->Find(_("SLATE BLUE"));
    ColourArray[28] = wxTheColourDatabase->Find(_("STEEL BLUE"));
    ColourArray[29] = wxTheColourDatabase->Find(_("TURQUOISE"));

    //green hues
    ColourArray[30] = wxTheColourDatabase->Find(_("KHAKI"));
    ColourArray[31] = wxTheColourDatabase->Find(_("DARK GREEN"));
    ColourArray[32] = wxTheColourDatabase->Find(_("DARK OLIVE GREEN"));
    ColourArray[33] = wxTheColourDatabase->Find(_("FOREST GREEN"));
    ColourArray[34] = wxTheColourDatabase->Find(_("MEDIUM FOREST GREEN"));
    ColourArray[35] = wxTheColourDatabase->Find(_("GREEN"));
    ColourArray[36] = wxTheColourDatabase->Find(_("GREEN YELLOW"));
    ColourArray[37] = wxTheColourDatabase->Find(_("LIME GREEN"));
    ColourArray[38] = wxTheColourDatabase->Find(_("MEDIUM SEA GREEN"));
    ColourArray[39] = wxTheColourDatabase->Find(_("MEDIUM SPRING GREEN"));
    ColourArray[40] = wxTheColourDatabase->Find(_("SPRING GREEN"));
    ColourArray[41] = wxTheColourDatabase->Find(_("PALE GREEN"));
    ColourArray[42] = wxTheColourDatabase->Find(_("SEA GREEN"));

    //yellow / orange hues
    ColourArray[43] = wxTheColourDatabase->Find(_("YELLOW"));
    ColourArray[44] = wxTheColourDatabase->Find(_("YELLOW GREEN"));
    ColourArray[45] = wxTheColourDatabase->Find(_("GOLD"));
    ColourArray[46] = wxTheColourDatabase->Find(_("GOLDENROD"));
    ColourArray[47] = wxTheColourDatabase->Find(_("MEDIUM GOLDENROD"));
    ColourArray[48] = wxTheColourDatabase->Find(_("ORANGE"));
    ColourArray[49] = wxTheColourDatabase->Find(_("ORANGE RED"));
    ColourArray[50] = wxTheColourDatabase->Find(_("WHEAT"));

    //red hues
    ColourArray[51] = wxTheColourDatabase->Find(_("RED"));
    ColourArray[52] = wxTheColourDatabase->Find(_("ORCHID"));
    ColourArray[53] = wxTheColourDatabase->Find(_("DARK ORCHID"));
    ColourArray[54] = wxTheColourDatabase->Find(_("FIREBRICK"));
    ColourArray[55] = wxTheColourDatabase->Find(_("INDIAN RED"));
    ColourArray[56] = wxTheColourDatabase->Find(_("MEDIUM ORCHID"));

    //brown hues
    ColourArray[57] = wxTheColourDatabase->Find(_("BROWN"));
    ColourArray[58] = wxTheColourDatabase->Find(_("MAROON"));
    ColourArray[59] = wxTheColourDatabase->Find(_("THISTLE"));
    ColourArray[60] = wxTheColourDatabase->Find(_("SIENNA"));
    ColourArray[61] = wxTheColourDatabase->Find(_("TAN"));

    //greys & white hues
    ColourArray[62] = wxTheColourDatabase->Find(_("DARK GREY"));
    ColourArray[63] = wxTheColourDatabase->Find(_("DARK SLATE GREY"));
    ColourArray[64] = wxTheColourDatabase->Find(_("DIM GREY"));
    ColourArray[65] = wxTheColourDatabase->Find(_("GREY"));
    ColourArray[66] = wxTheColourDatabase->Find(_("LIGHT GREY"));
    ColourArray[67] = wxTheColourDatabase->Find(_("WHITE"));

    cTransparent = *wxLIGHT_GREY;
}

/** DESTRUCTOR
  */
XPMColorPicker::~XPMColorPicker()
{
    //dtor
    if (ColourArray) delete[] ColourArray;
}

//------ EVENTS HANDLERS -------------
/** Handles paint event
  */
void XPMColorPicker::OnEventPaint(wxPaintEvent& event)
{
    //Paint event handler
    wxPaintDC dc(this);
    Paint(&dc);
}

/** Repaint the window
  * @param dc : a pointer to the DC to use
  */
void XPMColorPicker::Paint(wxDC *dc)
{
    if (!dc) return;
    ClearBackground();

    wxPen pBlackPen(*wxBLACK, 1, wxSOLID);
    wxPen pGreyPen(*wxLIGHT_GREY, 1, wxSOLID);
    wxPen pWhitePen(*wxWHITE, 1, wxSOLID);
    wxPen pBlue(*wxBLUE, 1, wxSOLID);
    wxBrush bWhiteBrush(*wxWHITE, wxSOLID);

    //draw the double square containing the current selection
    dc->SetPen(pBlackPen);
    dc->SetBrush(bWhiteBrush);
    dc->DrawRectangle(0,0, iColourWidth * 2, iHeight);
    dc->SetPen(pGreyPen);
    dc->DrawLine(1,1,iColourWidth * 2 -1, 1);
    dc->DrawLine(1,1,1, iHeight - 1);
    dc->SetPen(pWhitePen);
    dc->DrawLine(iColourWidth * 2 + 1,iHeight - 2,iColourWidth * 2 -2, 2);
    dc->DrawLine(iColourWidth * 2 + 1,iHeight - 2,2, iHeight - 2);
    //draw in this square the rectangle with the current outline colour and filling colour
    wxColour cFillColour;
    wxColour cLineColour;
    cFillColour = GetFillColour();
    cLineColour = GetLineColour();
    wxPen pOutlinePen(cLineColour, 2, wxSOLID);
    wxBrush bFillBrush(cFillColour, wxSOLID);
    dc->SetPen(pOutlinePen);
    dc->SetBrush(bFillBrush);
    dc->DrawRectangle(4,4, iColourWidth * 2 - 7, iHeight - 7);

    //draw the transparent colour button
    dc->SetPen(pBlackPen);
    dc->SetBrush(bWhiteBrush);
    dc->DrawRectangle(iColourWidth * 2 - 1,0, iColourWidth * 2, iHeight / 2);
    dc->SetBrush(wxNullBrush);
    dc->DrawRectangle(iColourWidth * 2 - 1,iHeight / 2 - 1, iColourWidth * 2 , iHeight / 2 + 1);
    wxBrush bTransparentBrush(cTransparent, wxSOLID);
    dc->SetPen(pWhitePen);
    dc->SetBrush(bTransparentBrush);
    dc->DrawRectangle(iColourWidth * 2 + 1,2, iColourWidth * 2 - 4, iHeight / 2 - 4);

    //draw the squares
    long i, j, jMax, iIndex;
    jMax = iPaletteSize / iNbRows;
    if (iPaletteSize % iNbRows > 0) jMax++; //for uneven iPaletteSize
    iIndex = -1;
    for(i=0;i<iNbRows;i++)
    {
        for(j=0;j<jMax;j++)
        {
            iIndex++;
            if (iIndex >= iPaletteSize) break;
            wxColour cColor;
            cColor = GetPaletteColour(iIndex);
            wxBrush bFBrush(cColor, wxSOLID);
            dc->SetBrush(bFBrush);
            dc->SetPen(pBlackPen);
            dc->DrawRectangle(iColourWidth * 4 + j * iColourWidth - 1 , i * iColourHeight,
                              iColourWidth, iColourHeight
                             );
            dc->SetBrush(wxNullBrush);

        }
        if (iIndex >= iPaletteSize) break;
    }
}

/** immediate repainting of the widgets
  */
void XPMColorPicker::Repaint(void)
{
    //immediate repainting of the widgets
    wxClientDC dc(this);
    Paint(&dc);
}

/** Handles a size event
  */
void XPMColorPicker::OnEventSize(wxSizeEvent& event)
{
    //Size event handler
    wxSize s;

    s = event.GetSize();
    ComputeDimensions(s);

    ToggleButton1->SetSize(iColourWidth * 2, iHeight / 2, iColourWidth * 2 - 2, iHeight / 2 - 1);
    Refresh(false, NULL);
    Update();
    event.Skip();
}

/** Compute the ideal dimensions of the colour squares
  * @param s : the size of the client area available for drawing
  */
void XPMColorPicker::ComputeDimensions(wxSize s)
{
    //Compute the dimensions for the colour squares
    int iNbSquares;
    int iIdealSize;
    int iStartRow;

    iWidth = s.GetWidth();
    iHeight = s.GetHeight();
    iStartRow = 2;

    //compute number of rows
    iIdealSize = XPM_IDEAL_SIZE;
    for (iNbRows = iStartRow; iNbRows < iPaletteSize / 2; iNbRows++)
    {
        iNbSquares = iPaletteSize / iNbRows; //how many squares per row
        if (iPaletteSize % iNbRows > 0) iNbSquares++; //for uneven iPaletteSize
        iColourWidth = iWidth / (iNbSquares + 4); //width of each square. The 4 is for 2 special double-width squares
        iColourHeight = iColourWidth;
        iHeight = iColourHeight * iNbRows;
        if (iColourWidth >= iIdealSize)
        {
            iColourWidth = iIdealSize;
            iColourHeight = iColourWidth;
            iHeight = iColourHeight * iNbRows;
            break;
        }
    }

    //set minimum & maximum size
    wxSize sMinSize;
    wxSize sMaxSize;
    wxSize sDefaultSize;
    int iMinHeight, iMaxHeight;

    //minimal size

    sMinSize = ToggleButton1->GetMinSize();
    iMinHeight = sMinSize.GetHeight();
    if (iMinHeight < XPM_IDEAL_SIZE) iMinHeight = XPM_IDEAL_SIZE;
    if (iMinHeight < iColourHeight) iMinHeight = iColourHeight;
    iMinHeight = iMinHeight * 2;
    /*if (iHeight > iMinHeight) sMinSize.SetHeight(iHeight); else sMinSize.SetHeight(iMinHeight);
    if (sMinSize.GetWidth() < XPM_IDEAL_SIZE * 6) sMinSize.SetWidth(XPM_IDEAL_SIZE * 6);
    */
    sMinSize.SetWidth(XPM_IDEAL_SIZE * ((iPaletteSize / iNbRows) + 4));
    sMinSize.SetHeight(XPM_IDEAL_SIZE * iNbRows);
    SetMinSize(sMinSize);

    //maximal size
    sMaxSize = ToggleButton1->GetMaxSize();
    iMaxHeight = sMaxSize.GetHeight();
    if (iMaxHeight < iMinHeight) iMaxHeight = iMinHeight;
    sMaxSize.SetHeight(iMaxHeight * iPaletteSize / 2);
    if (sMaxSize.GetWidth() < XPM_IDEAL_SIZE * (4 + iPaletteSize)) sMaxSize.SetWidth(XPM_IDEAL_SIZE * (4 + iPaletteSize));
    SetMaxSize(sMaxSize);
    //Manager::Get()->GetLogManager()->DebugLog(wxString::Format(_("Max Width = %d Max Height = %d"),
    //                                                           sMaxSize.GetWidth(), sMaxSize.GetHeight())
    //                                         );

}

/** to get the best size for the control
  * Best size is the control on 2 rows
  * @return the best computed size
  */
wxSize XPMColorPicker::DoGetBestSize(void) const
{
    //to get the best size for the control
    int iBestWidth, iBestHeight;
    wxSize sResult;

    iBestWidth = XPM_IDEAL_SIZE * ((iPaletteSize / 2) + 4);
    iBestHeight = XPM_IDEAL_SIZE * 2;
    sResult.SetWidth(iBestWidth);
    sResult.SetHeight(iBestHeight);
    return(sResult);
}

//----- Get and Set current colours  -----
/** Return the current line colour
  * If an error occur, the colour returned is BLACK by default
  * @return a wxColour representing the current line colour
  */
wxColour XPMColorPicker::GetLineColour(void)
{
    //return the current line colour
    if (iLineColor < -1) return(*wxBLACK);
    if (iLineColor >= XPM_MAX_COLOR) return(*wxBLACK);
    if (iLineColor == -1) return(cTransparent);
    return(ColourArray[iLineColor]);
}

/** Return the current fill colour
  * If an error occur, the colour returned is WHITE by default
  * @return a wxColour representing the current fill colour
  */
wxColour XPMColorPicker::GetFillColour(void)
{
    //return the current fill colour
    if (iFillColor < -1) return(*wxWHITE);
    if (iFillColor >= XPM_MAX_COLOR) return(*wxWHITE);
    if (iFillColor == -1) return(cTransparent);
    return(ColourArray[iFillColor]);
}

/** Return the current line colour index
  * If an error occur, the colour returned is BLACK by default
  * @return the index representing the current line colour
  */
int XPMColorPicker::GetLineColourIndex(void)
{
    //return the current line colour
    return(iLineColor);
}

/** Return the current fill colour index
  * @return the index representing the current fill colour
  */
int XPMColorPicker::GetFillColourIndex(void)
{
    //return the current fill colour
    return(iFillColor);
}

/** Set the current line colour
  * If an error occur, the colour set is the 1st colour of the palette by default
  * @param iIndex: the index in the wxColourArray representing the new line colour
  * @param bPostEvent: if true, a wxEVT_LINE_COLOR_CHANGED is generated (default)
  */
void XPMColorPicker::SetLineColour(int iIndex, bool bPostEvent)
{
    //set the current line colour
    int iIndex2;
    iIndex2 = iIndex;
    if (iIndex2 < -1) iIndex2 = 0;
    if (iIndex2 >= XPM_MAX_COLOR) iIndex2 = 0;
    iLineColor = iIndex2;

    if (bPostEvent)
    {
        wxCommandEvent eventColorChanged(wxEVT_LINE_COLOR_CHANGED, GetId());
        eventColorChanged.SetEventObject(this);
        wxPostEvent(this, eventColorChanged);
    }

    Refresh(false, NULL);
    Update();
}

/** Set the current fill colour
  * If an error occur, the colour set is the 1st colour of the palette by default
  * @param iIndex: the index in the wxColourArray representing the new fill colour
  * @param bPostEvent: if true, a wxEVT_FILL_COLOR_CHANGED is generated (default)
  */
void XPMColorPicker::SetFillColour(int iIndex, bool bPostEvent)
{
    //set the current fill colour
    int iIndex2;
    iIndex2 = iIndex;
    if (iIndex2 < -1) iIndex2 = 0;
    if (iIndex2 >= XPM_MAX_COLOR) iIndex2 = 0;
    iFillColor = iIndex2;

    if (bPostEvent)
    {
        wxCommandEvent eventColorChanged(wxEVT_FILL_COLOR_CHANGED, GetId());
        eventColorChanged.SetEventObject(this);
        wxPostEvent(this, eventColorChanged);
    }

    Refresh(false, NULL);
    Update();
}

/** get the palette size
  * @return the amount of colours in the palette
  */
int XPMColorPicker::GetPaletteSize(void)
{
    //get the palette size
    return(iPaletteSize);
}

/** set the palette size
  * if the new palette is larger, then all the new colours are initialized to blakc
  * @param iSize: the new amount of colours in the palette
  * @return true on success, false on failure
  */
bool XPMColorPicker::SetPaletteSize(int iSize)
{
    //set the palette size
    wxColour *NewArray;
    NewArray = new wxColour[iSize];
    if (!NewArray) return(false);

    //copy the old colours to the new palette
    int i;
    if (iSize >= iPaletteSize)
    {
        //the new palette is bigger or same size than the old one
        for(i=0; i < iPaletteSize; i++) NewArray[i] = ColourArray[i];
        for(i=iPaletteSize; i < iSize; i++) NewArray[i] = *wxBLACK;
    }
    else
    {
        //the new palette is smaller than the old one
        for(i=0; i < iSize; i++) NewArray[i] = ColourArray[i];
    }


    delete[] ColourArray;
    ColourArray = NewArray;
    iPaletteSize = iSize;
    Refresh(false, NULL);
    Update();
    return(true);
}

/** set the specific palette colour
  * @param iIndex: the index of the colour to be changed. If not valid,nothing is done
  * @param cColor: the new colour
  */
void XPMColorPicker::SetPaletteColour(int iIndex, wxColour cColor)
{
    //set the specific palette colour
    if (iIndex < 0) return;
    if (iIndex >= iPaletteSize) return;
    if (!ColourArray) return;
    ColourArray[iIndex] = cColor;
}

/** get the specific palette colour
  * @param iIndex: the index of the colour to be changed. If not valid, wxBLACK is returned
  * @return  the colour
  */
wxColour XPMColorPicker::GetPaletteColour(int iIndex)
{
    //Get the specific palette colour
    if (iIndex < 0) return(*wxBLACK);
    if (iIndex >= iPaletteSize) return(*wxBLACK);
    if (!ColourArray) return(*wxBLACK);
    return(ColourArray[iIndex]);
}

/** Get the transparent colour
  * @return : the transparent colour
  */
wxColour XPMColorPicker::GetTransparentColour(void)
{
    //Get the transparent colour
    return(cTransparent);
}

/** Set the transparent colour
  * @param cColour: the new transparent colour
  * @param bPostEvent: if true, a wxEVT_TRANSPARENT_COLOR_CHANGED is generated (default)
  */
void XPMColorPicker::SetTransparentColour(wxColour cColour, bool bPostEvent)
{
    //Set the transparent colour
    cTransparent = cColour;

    if (bPostEvent)
    {
        wxCommandEvent eventColorChanged(wxEVT_TRANSPARENT_COLOR_CHANGED, GetId());
        eventColorChanged.SetEventObject(this);
        wxPostEvent(this, eventColorChanged);
    }
}

//--- TOGGLE FILL / LINE COLOUR ---
void XPMColorPicker::OnToggleButton1Toggle(wxCommandEvent& event)
{
    bLineOn = !bLineOn;
    if (bLineOn) ToggleButton1->SetLabel(_(" LINE ")); else ToggleButton1->SetLabel(_(" FILL "));
}

//--- MOUSE EVENTS HANDLERS -----
/** The Mouse enters the window. Set the cursor
  * @param event: the event descriptor
  */
void XPMColorPicker::OnMouseEnter(wxMouseEvent& event)
{
    //The Mouse enters the window. Set the cursor
    SetCursor(*wxCROSS_CURSOR);
    event.Skip();
}

/** The Mouse leaves the window. Set the cursor
  * @param event: the event descriptor
  */
void XPMColorPicker::OnMouseLeave(wxMouseEvent& event)
{
    //The Mouse leaves the window. Set the cursor
    SetCursor(*wxCROSS_CURSOR);
    event.Skip();
}

/** Set the cursor event
  * @param event: the event descriptor
  */
void XPMColorPicker::OnSetCursor(wxSetCursorEvent& event)
{
    //Set the cursor
    event.SetCursor(*wxCROSS_CURSOR);
    event.Skip();
}

/** Process a double click event
  * @param event: the event descriptor
  */
void XPMColorPicker::OnMouseDClick(wxMouseEvent& event)
{
    //Process a double click event
    int iColourIndex;

    iColourIndex = GetColourIndex(event.m_x, event.m_y);

    if (iColourIndex == -1)
    {
        //set the transparent colour
        wxColourDialog cd(this);

        if (cd.ShowModal() == wxID_OK)
        {
            wxColourData cColourData;
            wxColour cColour;
            cColourData = cd.GetColourData();
            cColour = cColourData.GetColour();
            SetTransparentColour(cColour);
            Refresh(false, NULL);
            Update();
        }


    }
    else if ((iColourIndex > -1) &&(iColourIndex < iPaletteSize))
    {
        //set the specific palette colour
        wxColourDialog cd(this);

        if (cd.ShowModal() == wxID_OK)
        {
            wxColourData cColourData;
            wxColour cColour;
            cColourData = cd.GetColourData();
            cColour = cColourData.GetColour();
            SetPaletteColour(iColourIndex, cColour);
            Refresh(false, NULL);
            Update();
        }
    }

    event.Skip();
}

/** Process a left click event
  * @param event: the event descriptor
  */
void XPMColorPicker::OnMouseClick(wxMouseEvent& event)
{
    //Process a click event
    int iColourIndex;
    if (event.LeftDClick())
    {
        event.Skip();
        return;
    }

    iColourIndex = GetColourIndex(event.m_x, event.m_y);

    if ((iColourIndex > -1) &&(iColourIndex < iPaletteSize) && (!(event.ButtonDClick(wxMOUSE_BTN_LEFT))))
    {
        if (bLineOn) iLineColor = iColourIndex; else iFillColor = iColourIndex;
        Refresh(false, NULL);
        Update();
        if (bLineOn)
        {
            wxCommandEvent eventColorChanged(wxEVT_LINE_COLOR_CHANGED, GetId());
            eventColorChanged.SetEventObject(this);
            wxPostEvent(this, eventColorChanged);
        }
        else
        {
            wxCommandEvent eventColorChanged(wxEVT_FILL_COLOR_CHANGED, GetId());
            eventColorChanged.SetEventObject(this);
            wxPostEvent(this, eventColorChanged);
        }
        return;
    }
    else if ((iColourIndex == -1) && (!(event.ButtonDClick(wxMOUSE_BTN_LEFT))))
    {
        if (bLineOn) iLineColor = -1; else iFillColor = -1;
        Refresh(false, NULL);
        Update();
        return;
    }
    event.Skip();
}

/** Process a right click event
  * @param event: the event descriptor
  */
void XPMColorPicker::OnMouseRClick(wxMouseEvent& event)
{
    //Process a click event
    int iColourIndex;
    if (event.LeftDClick())
    {
        event.Skip();
        return;
    }

    iColourIndex = GetColourIndex(event.m_x, event.m_y);

    if ((iColourIndex > -1) &&(iColourIndex < iPaletteSize) && (!(event.ButtonDClick(wxMOUSE_BTN_RIGHT))))
    {
        if (!bLineOn) iLineColor = iColourIndex; else iFillColor = iColourIndex;
        Refresh(false, NULL);
        Update();
        if (!bLineOn)
        {
            wxCommandEvent eventColorChanged(wxEVT_LINE_COLOR_CHANGED, GetId());
            eventColorChanged.SetEventObject(this);
            wxPostEvent(this, eventColorChanged);
        }
        else
        {
            wxCommandEvent eventColorChanged(wxEVT_FILL_COLOR_CHANGED, GetId());
            eventColorChanged.SetEventObject(this);
            wxPostEvent(this, eventColorChanged);
        }
        return;
    }
    else if ((iColourIndex == -1) && (!(event.ButtonDClick(wxMOUSE_BTN_RIGHT))))
    {
        if (!bLineOn) iLineColor = -1; else iFillColor = -1;
        Refresh(false, NULL);
        Update();
        return;
    }
    event.Skip();
}

/** Return the index of the colour clicked
  * @return : -1 : transparent colour clicked
  *           <-1: no colour clicked
  *           >=0: the index of the colour clicked
  */
int XPMColorPicker::GetColourIndex(int x, int y)
{
    //Return the index of the colour clicked
    wxSize sWindowSize;
    int iRow, iCol, iIndex, iNbSquaresPerRow;

    sWindowSize = GetSize();

    if (x < iColourWidth * 2) return(-2);
    if (x > sWindowSize.GetWidth()) return(-2);
    if (y < 0) return(-2);
    if (y > sWindowSize.GetHeight()) return(-2);

    //is transparent colour clicked ?
    if (x < iColourWidth * 4)
    {
        if (y < sWindowSize.GetHeight() / 2) return(-1);
        return(-2);
    }

    iRow = y / iColourHeight;
    iCol = (x - iColourWidth * 4) / iColourWidth;
    iNbSquaresPerRow = iPaletteSize / iNbRows;
    if (iPaletteSize % iNbRows > 0) iNbSquaresPerRow++; //for uneven iPaletteSize
    iIndex = iRow * iNbSquaresPerRow + iCol;
    if (iIndex < iPaletteSize) return(iIndex);

    return(-2);
}
