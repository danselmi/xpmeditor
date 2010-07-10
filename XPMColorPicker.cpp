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

#include <wx/gdicmn.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/brush.h>
#include <wx/pen.h>
#include <wx/msgdlg.h>
#include <wx/colordlg.h>
#include <wx/renderer.h>
#include <wx/settings.h>

DEFINE_EVENT_TYPE(wxEVT_TRANSPARENT_COLOR_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_LINE_COLOR_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_FILL_COLOR_CHANGED)

const long XPMColorPicker::ID_SCROLLBAR_HORIZ = wxNewId();
const long XPMColorPicker::ID_SCROLLBAR_VERT  = wxNewId();

/** CONSTRUCTOR
  */
XPMColorPicker::XPMColorPicker(wxWindow* parent,
                                   wxWindowID id,
                                   const wxPoint& pos,
                                   const wxSize& size,
                                   int style,
                                   const wxString& name
                                  )
{
    //constructor
    bComputed = false;
    Create(parent, id, pos, size, style, name);
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    sbHorizScrollBar = new wxScrollBar(this, ID_SCROLLBAR_HORIZ,
                                       wxDefaultPosition, wxDefaultSize,
                                       wxSB_HORIZONTAL, wxDefaultValidator,
                                       _T("ID_SCROLLBAR_HORIZ")
                                      );
    sbVertScrollBar = new wxScrollBar(this, ID_SCROLLBAR_VERT,
                                      wxDefaultPosition, wxDefaultSize,
                                      wxSB_VERTICAL, wxDefaultValidator,
                                      _T("ID_SCROLLBAR_VERT")
                                     );

    //initialize the colour array
    init_colours();

    //initialize sizing parameters
    wxFont font;
    font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    SetFont(font);
    iOrientation = XPM_AUTO_ORIENTATION;
    iIdealNbRowsOrCols = 2;
    bScrollbars = false;
    iUnitX = -1;
    iUnitY = -1;
    bSquare = false;
    iMinimalSize = XPM_MINIMAL_SIZE;

    //initialize the Best size information
    wxSize sBestSize;
    sBestSize = DoGetBestSize();
    SetMinSize(sBestSize);

    bLineOn = true;

    //initializations
    iScrollBarPos = 0;
    wxSize sClientSize;
    sClientSize = GetClientSize();
    ComputeDimensions(sClientSize);
    iLineColor = 0;  //black
    iFillColor = XPM_MAX_COLOR - 1; //white

    //event handlers
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&XPMColorPicker::OnEventSize);
    Connect(wxEVT_PAINT,(wxObjectEventFunction)&XPMColorPicker::OnEventPaint);
    Connect(wxEVT_ERASE_BACKGROUND, (wxObjectEventFunction)&XPMColorPicker::OnEraseBackground);
    //scrollbar events
    if (sbHorizScrollBar)
    {
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_TOP, (wxObjectEventFunction)&XPMColorPicker::OnScrollTop);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_BOTTOM, (wxObjectEventFunction)&XPMColorPicker::OnScrollBottom);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_LINEUP, (wxObjectEventFunction)&XPMColorPicker::OnScrollLineUp);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_LINEDOWN, (wxObjectEventFunction)&XPMColorPicker::OnScrollLineDown);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_PAGEUP, (wxObjectEventFunction)&XPMColorPicker::OnScrollPageUp);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_PAGEDOWN, (wxObjectEventFunction)&XPMColorPicker::OnScrollPageDown);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_THUMBTRACK, (wxObjectEventFunction)&XPMColorPicker::OnScrollThumbTrack);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_THUMBRELEASE, (wxObjectEventFunction)&XPMColorPicker::OnScrollThumbRelease);
        Connect(ID_SCROLLBAR_HORIZ, wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&XPMColorPicker::OnScrollChanged);
    }
    if (sbVertScrollBar)
    {
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_TOP, (wxObjectEventFunction)&XPMColorPicker::OnScrollTop);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_BOTTOM, (wxObjectEventFunction)&XPMColorPicker::OnScrollBottom);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_LINEUP, (wxObjectEventFunction)&XPMColorPicker::OnScrollLineUp);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_LINEDOWN, (wxObjectEventFunction)&XPMColorPicker::OnScrollLineDown);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_PAGEUP, (wxObjectEventFunction)&XPMColorPicker::OnScrollPageUp);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_PAGEDOWN, (wxObjectEventFunction)&XPMColorPicker::OnScrollPageDown);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_THUMBTRACK, (wxObjectEventFunction)&XPMColorPicker::OnScrollThumbTrack);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_THUMBRELEASE, (wxObjectEventFunction)&XPMColorPicker::OnScrollThumbRelease);
        Connect(ID_SCROLLBAR_VERT, wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&XPMColorPicker::OnScrollChanged);
    }

    //mouse events handlers
    Connect(wxEVT_MOTION, (wxObjectEventFunction)&XPMColorPicker::OnMouseMove);
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

//scroll event handlers
/** Scrollbar event wxEVT_SCROLL_LINEUP
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollLineUp(wxScrollEvent& event)
{
    iScrollBarPos = event.GetPosition();
    Repaint();

    event.Skip();
}

/** Scrollbar event wxEVT_SCROLL_LINEDOWN
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollLineDown(wxScrollEvent& event)
{
    iScrollBarPos = event.GetPosition();
    Repaint();

    event.Skip();
}

/** Scrollbar event wxEVT_SCROLL_PAGEUP
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollPageUp(wxScrollEvent& event)
{
    int iOrientation2;
    int iPageSize;

    iOrientation2 = event.GetOrientation();

    if ((sbHorizScrollBar) && (iOrientation2 == wxHORIZONTAL))
    {
        iPageSize = sbHorizScrollBar->GetPageSize();
        iScrollBarPos = sbHorizScrollBar->GetThumbPosition() - iPageSize;
        if (iScrollBarPos < 0) iScrollBarPos = 0;
        sbHorizScrollBar->SetThumbPosition(iScrollBarPos);
    }
    if ((sbVertScrollBar) && (iOrientation2 == wxVERTICAL))
    {
        iPageSize = sbVertScrollBar->GetPageSize();
        iScrollBarPos = sbVertScrollBar->GetThumbPosition() - iPageSize;
        if (iScrollBarPos < 0) iScrollBarPos = 0;
        sbVertScrollBar->SetThumbPosition(iScrollBarPos);
    }

    Repaint();

    event.Skip();
}


/** Scrollbar event wxEVT_SCROLL_PAGEDOWN
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollPageDown(wxScrollEvent& event)
{
    int iPageSize, iScrollBarMaxPos;
    int iOrientation2;

    iOrientation2 = event.GetOrientation();

    if ((sbHorizScrollBar) && (iOrientation2 == wxHORIZONTAL))
    {
        iPageSize = sbHorizScrollBar->GetPageSize();
        iScrollBarMaxPos = sbHorizScrollBar->GetRange() - sbHorizScrollBar->GetThumbSize();
        iScrollBarPos = sbHorizScrollBar->GetThumbPosition() + iPageSize;
        if (iScrollBarPos > iScrollBarMaxPos) iScrollBarPos = iScrollBarMaxPos;
        sbHorizScrollBar->SetThumbPosition(iScrollBarPos);
    }
    if ((sbVertScrollBar) && (iOrientation2 == wxVERTICAL))
    {
        iPageSize = sbVertScrollBar->GetPageSize();
        iScrollBarMaxPos = sbVertScrollBar->GetRange() - sbVertScrollBar->GetThumbSize();
        iScrollBarPos = sbVertScrollBar->GetThumbPosition() + iPageSize;
        if (iScrollBarPos > iScrollBarMaxPos) iScrollBarPos = iScrollBarMaxPos;
        sbVertScrollBar->SetThumbPosition(iScrollBarPos);
    }

    Repaint();

    event.Skip();
}


/** Scrollbar event wxEVT_SCROLL_TOP
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollTop(wxScrollEvent& event)
{
    int iOrientation2;

    iOrientation2 = event.GetOrientation();
    iScrollBarPos = 0;
    if ((sbHorizScrollBar) && (iOrientation2 == wxHORIZONTAL)) sbHorizScrollBar->SetThumbPosition(0);
    if ((sbVertScrollBar) && (iOrientation2 == wxVERTICAL)) sbVertScrollBar->SetThumbPosition(0);

    Repaint();

    event.Skip();
}


/** Scrollbar event wxEVT_SCROLL_BOTTOM
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollBottom(wxScrollEvent& event)
{
    int iOrientation2;

    iOrientation2 = event.GetOrientation();

    if ((sbHorizScrollBar) && (iOrientation2 == wxHORIZONTAL))
    {
        iScrollBarPos = sbHorizScrollBar->GetRange() - sbHorizScrollBar->GetThumbSize();
        sbHorizScrollBar->SetThumbPosition(iScrollBarPos);
    }
    if ((sbVertScrollBar) && (iOrientation2 == wxVERTICAL))
    {
        iScrollBarPos = sbVertScrollBar->GetRange() - sbVertScrollBar->GetThumbSize();
        sbVertScrollBar->SetThumbPosition(iScrollBarPos);
    }

    Repaint();

    event.Skip();
}


/** Scrollbar event wxEVT_SCROLL_THUMBTRACK
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollThumbTrack(wxScrollEvent& event)
{
    iScrollBarPos = event.GetPosition();
    Repaint();

    event.Skip();
}


/** Scrollbar event wxEVT_SCROLL_THUMBRELEASE
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollThumbRelease(wxScrollEvent& event)
{
    iScrollBarPos = event.GetPosition();
    Repaint();

    event.Skip();
}


/** Scrollbar event wxEVT_SCROLL_CHANGED
  * \param event : the event descriptor
  */
void XPMColorPicker::OnScrollChanged(wxScrollEvent& event)
{
    event.Skip(); //do nothing to prevent double handling
}

/** Override background events - to avoid flicker
  */
void XPMColorPicker::OnEraseBackground(wxEraseEvent)
{

}

/** Handles paint event
  */
void XPMColorPicker::OnEventPaint(wxPaintEvent& event)
{
    //Paint event handler
    wxBufferedPaintDC dc(this);
    Paint(dc);
}

/** Repaint the window
  * \param dc : a pointer to the DC to use
  */
void XPMColorPicker::Paint(wxDC &dc)
{
    int iWidth, iHeight, iOrientation2;
    wxSize sClientSize;
    sClientSize = GetClientSize();
    iWidth  = sClientSize.GetWidth();
    iHeight = sClientSize.GetHeight();

    iOrientation2 = GetActualOrientation();

    if (!bComputed)
    {
        ComputeDimensions(sClientSize);
    }

    //draw the double square containing the current selection
    PaintDisplaySquare(dc, wxRect(0,0, iDisplayWidth, iDisplayHeight));

    if (iOrientation2 == XPM_HORIZONTAL_ORIENTATION)
    {
        //horizontal layout
        PaintTransparentColor(dc, wxRect(iDisplayWidth, 0, iDisplayWidth, iDisplayHeight));
        PaintColorSquares(dc, wxRect(2*iDisplayWidth, 0, iWidth - 2 * iDisplayWidth, iHeight));
    }
    else
    {
        //vertical layout
        PaintTransparentColor(dc, wxRect(0, iDisplayHeight, iDisplayWidth, iDisplayHeight));
        PaintColorSquares(dc, wxRect(0, 2 * iDisplayHeight, iWidth, iHeight - 2 * iDisplayHeight));
    }

    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);
}

/** Paint the display square : the fill colour + the line colour
  * \param dc   : the wxDC on which to draw
  * \param rect : the bounding rect on which to draw
  */
void XPMColorPicker::PaintDisplaySquare(wxDC &dc, wxRect rect)
{
    //paint the display square
    wxPen pBorderPen(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVEBORDER), 1, wxSOLID);
    wxBrush bBackBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE), wxSOLID);
    dc.SetPen(pBorderPen);
    dc.SetBrush(bBackBrush);
    dc.DrawRectangle(rect.GetLeft(),rect.GetTop(), rect.GetWidth(), rect.GetHeight());

    //draw in this square the rectangle with the current outline colour and filling colour
    wxColour cFillColour;
    wxColour cLineColour;
    cFillColour = GetFillColour();
    cLineColour = GetLineColour();
    wxPen pOutlinePen(cLineColour, 2, wxSOLID);
    wxBrush bFillBrush(cFillColour, wxSOLID);
    dc.SetPen(pOutlinePen);
    dc.SetBrush(bFillBrush);
    dc.DrawRectangle(rect.GetLeft() + 4, rect.GetTop() + 4,
                     rect.GetWidth() - 7, rect.GetHeight() - 7
                     );

    //draw the sunken border
    PaintSunkenBorder(dc, rect);
    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);
}

/** Paint the rectangle which include the transparent colour and the push button
  * \param dc   : the wxDC on which to draw
  * \param rect : the bounding rect on which to draw
  */
void XPMColorPicker::PaintTransparentColor(wxDC &dc, wxRect rect)
{

    //paint the 2 frames (one for the transparent colour, one for the push button
    wxPen pBorderPen(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVEBORDER), 1, wxSOLID);
    wxBrush bBackBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE), wxSOLID);
    dc.SetPen(pBorderPen);
    dc.SetBrush(bBackBrush);
    dc.DrawRectangle(rect.GetLeft(),rect.GetTop(), rect.GetWidth(), rect.GetHeight() / 2);
    PaintSunkenBorder(dc, wxRect(rect.GetLeft(),rect.GetTop(), rect.GetWidth(), rect.GetHeight() / 2));
    dc.SetBrush(wxNullBrush);
    dc.DrawRectangle(rect.GetLeft(),rect.GetTop() + rect.GetHeight() / 2,
                     rect.GetWidth() , rect.GetHeight() - rect.GetHeight() / 2
                    );
    PaintSunkenBorder(dc, wxRect(rect.GetLeft(),rect.GetTop() + rect.GetHeight() / 2,
                                 rect.GetWidth() , rect.GetHeight() - rect.GetHeight() / 2
                                )
                     );

    //paint the transparent colour
    wxBrush bTransparentBrush(cTransparent, wxSOLID);
    wxPen pBackPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE), 1, wxSOLID);
    dc.SetPen(pBackPen);
    dc.SetBrush(bTransparentBrush);
    dc.DrawRectangle(rect.GetLeft() + 2, rect.GetTop() + 2,
                     rect.GetWidth() - 4, rect.GetHeight() / 2 - 4);

    //paint the push button
    int flags;
    wxString sText;
    wxCoord x, y, x2, y2;
    wxFont font;
    if (!bLineOn)
    {
        flags = wxCONTROL_PRESSED;
        sText = _("Fill");
    }
    else
    {
        flags = 0;
        sText = _("Line");
    }
    wxRendererNative::Get().DrawPushButton(this, dc,
                                           wxRect(rect.GetLeft() + 1,rect.GetTop() + rect.GetHeight() / 2 + 1,
                                                  rect.GetWidth() - 2 , rect.GetHeight() - rect.GetHeight() / 2 - 2),
                                           flags
                                          );

    font = GetFont();
    dc.SetFont(font);
    dc.GetTextExtent(sText,&x,&y);
    dc.GetTextExtent(_("FillLine"), &x2, &y2);
    dc.DrawText(sText, rect.GetLeft() + 1 + (rect.GetWidth() - 2 - x) / 2 ,
                       rect.GetTop() + rect.GetHeight() / 2 + 1 + (rect.GetHeight() - rect.GetHeight() / 2 - 2 - y2) / 2);

    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);
}

void XPMColorPicker::PaintColorSquares(wxDC &dc, wxRect rect)
{
    //paint all the colour squares
    wxBrush bBackBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    wxPen pBackPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE), 1, wxSOLID);

    int i, j, iIndex, iStart, jStart;
    iIndex = 0;

    if (bScrollbars)
    {
        int iOrientation2;
        iOrientation2 = GetActualOrientation();
        if (iOrientation2 == XPM_VERTICAL_ORIENTATION)
        {
            iStart = iScrollBarPos;
            jStart = 0;
        }
        else
        {
            iStart = 0;
            jStart = iScrollBarPos;
        }
    }
    else
    {
        iStart = 0;
        jStart = 0;
    }

    for(i=iStart;i<iNbRows;i++)
    {
        for(j=jStart;j<iNbColumns;j++)
        {
            iIndex = i * iNbColumns + j;
            if (iIndex >= iPaletteSize)
            {
                dc.SetPen(pBackPen);
                dc.SetBrush(bBackBrush);
                dc.DrawRectangle(rect.GetLeft() + (j-jStart) * iColourWidth , (i-iStart) * iColourHeight + 1 + rect.GetTop(),
                                 iColourWidth, iColourHeight
                                );
            }
            else
            {
                wxColour cColor;
                cColor = GetPaletteColour(iIndex);
                wxBrush bFBrush(cColor, wxSOLID);
                wxPen pFPen(cColor, 1,  wxSOLID);
                dc.SetBrush(bFBrush);
                dc.SetPen(pFPen);
                dc.DrawRectangle(rect.GetLeft() + (j-jStart) * iColourWidth , (i-iStart) * iColourHeight + 1 + rect.GetTop(),
                                 iColourWidth, iColourHeight
                                );
                PaintSunkenBorder(dc, wxRect(rect.GetLeft() + (j-jStart) * iColourWidth , (i-iStart) * iColourHeight + 1 + rect.GetTop(),
                                             iColourWidth, iColourHeight
                                            )
                                 );
            }
            dc.SetBrush(wxNullBrush);
            dc.SetPen(wxNullPen);
        }
        //paint the end of the row with the background colour
        dc.SetPen(pBackPen);
        dc.SetBrush(bBackBrush);
        dc.DrawRectangle(rect.GetLeft() + (iNbColumns - jStart) * iColourWidth , (i-iStart) * iColourHeight + 1 + rect.GetTop(),
                         rect.GetWidth() - (iNbColumns - jStart) * iColourWidth, iColourHeight
                        );
    }
    //paint the bottom of the control with the background colour
    dc.SetPen(pBackPen);
    dc.SetBrush(bBackBrush);
    dc.DrawRectangle(rect.GetLeft() , (iNbRows - iStart) * iColourHeight + 1 + rect.GetTop(),
                     rect.GetWidth(), rect.GetHeight() - (iNbRows - iStart) * iColourHeight
                    );
    PaintSunkenBorder(dc, rect);
    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);
}

/** Paint a sunken border
  * \param dc   : the wxDC on which to draw
  * \param rect : the bounding rect which must appear sunk.
  *               The border will be drawn on the edge and 1 pixel inside the rect
  *               (the border has a thickness of up 2 pixels)
  */
void XPMColorPicker::PaintSunkenBorder(wxDC &dc, wxRect rect)
{
    //paint a sunken border
    wxPen pDarkPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW), 1, wxSOLID);
    wxPen pLightPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT), 1, wxSOLID);

    dc.SetPen(pDarkPen);
    dc.DrawLine(rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetTop());
    dc.DrawLine(rect.GetLeft(), rect.GetTop(), rect.GetLeft(), rect.GetBottom());
    dc.SetPen(pLightPen);
    dc.DrawLine(rect.GetLeft(), rect.GetBottom(), rect.GetRight()+1, rect.GetBottom());
    dc.DrawLine(rect.GetRight(), rect.GetTop(), rect.GetRight(), rect.GetBottom()+1);

    dc.SetPen(wxNullPen);
}



/** immediate repainting of the widgets
  */
void XPMColorPicker::Repaint(void)
{
    //immediate repainting of the widgets
    wxClientDC dcclient(this);
    wxBufferedDC dc(&dcclient);
    Paint(dc);
}

/** Handles a size event
  */
void XPMColorPicker::OnEventSize(wxSizeEvent& event)
{
    //Size event handler
    wxSize s;

    bComputed = false;
    s = GetClientSize();
    ComputeDimensions(s);

    Refresh(false, NULL);
    Update();
    event.Skip();
}

/** Set the orientation of the picker: horizontal or vertical
  * \param iNewOrientation : one of the following value:
  *        @XPM_HORIZONTAL_ORIENTATION : horizontal layout
  *        @XPM_VERTICAL_ORIENTATION   : vertical layout
  *        @XPM_AUTO_ORIENTATION       : let the control choose the best solution
  *        Default value is : @XPM_AUTO_ORIENTATION
  */
void XPMColorPicker::SetOrientation(int iNewOrientation)
{
    if (   (iOrientation != iNewOrientation)
        && (  (iNewOrientation == XPM_HORIZONTAL_ORIENTATION)
            ||(iNewOrientation == XPM_VERTICAL_ORIENTATION)
           )
       )
    {
        wxSize sClientSize;
        iOrientation = iNewOrientation;
        sClientSize = GetClientSize();
        ComputeDimensions(sClientSize);
        Refresh(false, NULL);
        Update();
    }
}

/** Get the orientation of the picker: horizontal or vertical
  * \return iNewOrientation : one of the following value:
  *        @XPM_HORIZONTAL_ORIENTATION : horizontal layout
  *        @XPM_VERTICAL_ORIENTATION   : vertical layout
  *        @XPM_AUTO_ORIENTATION       : let the control choose the best solution
  */
int XPMColorPicker::GetOrientation(void)
{
    return(iOrientation);
}

/** Get the orientation of the picker: horizontal or vertical
  * If the picker is in automatic mode, it will compute the best solution
  * \return iNewOrientation : one of the following value:
  *        @XPM_HORIZONTAL_ORIENTATION : horizontal layout
  *        @XPM_VERTICAL_ORIENTATION   : vertical layout
  */
int XPMColorPicker::GetActualOrientation(void) const
{
    int iOrientation2;
    wxSize sClientSize;

    iOrientation2 = XPM_HORIZONTAL_ORIENTATION;
    sClientSize = GetClientSize();
    switch(iOrientation)
    {
        case XPM_HORIZONTAL_ORIENTATION:
        case XPM_VERTICAL_ORIENTATION  : iOrientation2 = iOrientation;
                                         break;
        default: if (sClientSize.GetHeight() > sClientSize.GetWidth())
                 {
                     iOrientation2 = XPM_VERTICAL_ORIENTATION;
                 }
                 else
                 {
                     iOrientation2 = XPM_HORIZONTAL_ORIENTATION;
                 }
                 break;
    }
    return(iOrientation2);
}

/** Sets the ideal number of rows or columns the colour picker should display
  * See @SetOrientation to indicates if this value correspond to row number or column number
  * If they are enough place, the picker will draw the colours using the specified number of
  * rows or columns.
  * Default value is 2.
  * When the size reduces, the picker will first try to reduce the size of the squares, and
  * when the squares reach the minimal size (3 x 3 pixels), it will use the available space.
  * If no more space is available, scrollbars will be set
  * \param iNewValue : the new value
  */
void XPMColorPicker::SetIdealNbLines(int iNewValue)
{
    if (iNewValue > 0)
    {
        wxSize sClientSize;
        iIdealNbRowsOrCols = iNewValue;
        sClientSize = GetClientSize();
        ComputeDimensions(sClientSize);
        Refresh(false, NULL);
        Update();
    }
}

/** Gets the ideal number of rows or columns the colour picker should display
  * See @GetOrientation to know if this value correspond to row number or column number
  * If they are enough place, the picker will draw the colours using the specified number of
  * rows or columns.
  * When the size reduces, the picker will first try to reduce the size of the squares, and
  * when the squares reach the minimal size (3 x 3 pixels), it will use the available space.
  * If no more space is available, scrollbars will be set
  * \return  the value
  */
int XPMColorPicker::GetIdealNbLines(void)
{
    return(iIdealNbRowsOrCols);
}

/** Set the minimal size for a colour square
  * \param iNewValue : the new minimal size, in pixel
  */
void XPMColorPicker::SetMinimalSize(int iNewValue)
{
    if (iNewValue > 2)
    {
        wxSize sClientSize;
        iMinimalSize = iNewValue;
        sClientSize = GetClientSize();
        ComputeDimensions(sClientSize);
        Refresh(false, NULL);
        Update();
    }
}

/** Get the minimal size for a colour square
  * \return the minimal size for a colour square
  */
int XPMColorPicker::GetMinimalSize(void)
{
    return(iMinimalSize);
}

/** Set if the picker should draw exact squares or rectangles
  * \param bNewValue : if true, the control will draw exact squares for the colours
  *                    if false, the control will draw rectangles if needed
  */
void XPMColorPicker::SetSquareDisplay(bool bNewValue)
{
    if (bNewValue != bSquare)
    {
        wxSize sClientSize;
        bSquare = bNewValue;
        sClientSize = GetClientSize();
        ComputeDimensions(sClientSize);
        Refresh(false, NULL);
        Update();
    }
}

/** Get if the picker should draw exact squares or rectangles
  * \return : if true, the control will draw exact squares for the colours
  *           if false, the control will draw rectangles if needed
  */
bool XPMColorPicker::GetSquareDisplay(void)
{
    return(bSquare);
}


/** Compute the ideal dimensions of the colour squares
  * The following parameters will be computed:
  *  1 - the minimal size of the control. SetMinSize will be called
  *  2 - the maximal size of the control. SetMaxSize will be called
  *  3 - the number of rows and columns necessary to draw everything.
  *      Each colour square will be at least 3 pixels wide and tall
  *  4 - The width and height of each colour squares
  *  5 - The need of scrollbar or not.
  *  6 - The width and height of the Display square
  *  7 - the width and height of the Transparent square
  *  8 - the width and height of the push button
  *
  * These values will be stored respectively in :
  *  1 - Internal data of wxWindow. GetMinSize return it
  *  2 - Internal data of wxWindow. GetMaxSize return it
  *  3 - iNbRows and iNbColumns
  *  4 - iColourWidth and iColourHeight respectively
  *  5 - bScrollbars
  *  6 - iDisplayWidth and iDisplayHeight respectively
  *  7 - iDisplayWidth and iDisplayHeight / 2
  *  8 - iDisplayWidth and iDisplayHeight / 2
  *
  * This method takes in parameter a client area size. It will try in this order:
  *  1 - to use the available space + the ideal number of rows / columns
  *  2 - to use the available space + any number of rows/columns
  *  3 - to use as many rows or columns with a minimal size as long as space is available
  *  4 - to reduce the size of the squares until they fit or they are too small (iMinimalSize x iMinimalSize pixels)
  *  5 - to add scrollbars
  *
  * This function will have an impact on:
  *  1 - painting
  *  2 - hit test
  *  3 - sizer fitting
  *
  * \param s : the size of the client  area available for drawing
  */
void XPMColorPicker::ComputeDimensions(wxSize sClientSize)
{
    int iOrientation2;
    int iNbSquaresX, iNbSquaresY, iNbSquares;
    int iHeight, iWidth, iComputedRowsOrCols, iSize;
    bool bFinished;

    //get the orientation of the control: vertical or horizontal
    iOrientation2 = GetActualOrientation();

    //Compute the dimensions units
    GetLengthUnit();
    iHeight = sClientSize.GetHeight();
    iWidth  = sClientSize.GetWidth();

    //get the amount of rows and cols which fit in the current client area.
    bFinished = false;
    if (iOrientation2 == XPM_HORIZONTAL_ORIENTATION)
    {
        //horizontal layout
        //1 - to use the available space for the ideal number of rows/columns
        if (iMinimalSize < 1) iMinimalSize = 1;
        if (iIdealNbRowsOrCols < 1) iIdealNbRowsOrCols = 1;
        iColourHeight = iHeight / iIdealNbRowsOrCols; //the height of a square
        iNbSquaresX  = (iWidth - 2 * iDisplayWidth) / iMinimalSize; //how many squares fit on each row
        iNbSquaresY  = iIdealNbRowsOrCols;
        if ((iColourHeight >= iMinimalSize) && (iNbSquaresX * iNbSquaresY >= iPaletteSize))
        {
            //there are enough place for rows.
            //everything fit. finish the computation
            bFinished     = true;
            bScrollbars   = false;
            iNbRows       = iIdealNbRowsOrCols;
            iNbColumns    = iPaletteSize / iIdealNbRowsOrCols;
            if (iNbColumns * iNbRows < iPaletteSize) iNbColumns++;
            if (iNbColumns < 1) iNbColumns = 1;
            if (iNbRows < 1) iNbRows = 1;
            iColourWidth  = (iWidth - 2 * iDisplayWidth) / iNbColumns;
            iColourHeight = iHeight / iNbRows;
            iDisplayWidth  = iUnitX * 2;
            iDisplayHeight = iHeight;
        }

        if (!bFinished)
        {
            //2 - to use the available space for any number of rows/columns
            iComputedRowsOrCols = iHeight / iUnitY;              //how many rows of ideal size fits in this
            iNbSquaresX = (iWidth - 2 * iDisplayWidth) / iUnitX; //how many squares per row
            if ((iComputedRowsOrCols >= iIdealNbRowsOrCols) && (iNbSquaresX * iComputedRowsOrCols >= iPaletteSize))
            {
                //there are enough place for rows.
                //everything fit. finish the computation
                bFinished     = true;
                bScrollbars   = false;
                iNbRows       = iComputedRowsOrCols;
                iNbColumns    = iNbSquaresX;
                if (iNbColumns < 1) iNbColumns = 1;
                if (iNbRows < 1) iNbRows = 1;
                iColourWidth  = (iWidth - 2 * iDisplayWidth) / iNbColumns;
                iColourHeight = iHeight / iNbRows;
                iDisplayWidth  = iUnitX * 2;
                iDisplayHeight = iHeight;

            }
        }

        if (!bFinished)
        {
            //3 - to use as many rows or columns with a minimal size as long as space is available
            //compute how many squares can fit :
            iNbSquaresX = (iWidth - 2 * iDisplayWidth) / iUnitX; //how many squares per row
            iNbSquaresY = iHeight / iUnitY;                      //how many rows fits
            iNbSquares = iNbSquaresX * iNbSquaresY;
            if (iNbSquares >= iPaletteSize)
            {
                //everything fit. finish the computation
                bFinished     = true;
                bScrollbars   = false;
                iNbRows       = iNbSquaresY;
                iNbColumns    = iNbSquaresX;
                if (iNbColumns < 1) iNbColumns = 1;
                if (iNbRows < 1) iNbRows = 1;
                iColourWidth  = (iWidth - 2 * iDisplayWidth) / iNbColumns;
                iColourHeight = iHeight / iNbRows;
                iDisplayWidth  = iUnitX * 2;
                iDisplayHeight = iHeight;
            }
        }

        if (!bFinished)
        {
            //4 - to reduce the size of the squares until they fit or they are too small (iMinimalSize x iMinimalSize pixels)
            for(iSize = iUnitX; iSize >= iMinimalSize; iSize--)
            {
                iNbSquaresX = (iWidth - 2 * iDisplayWidth) / iSize; //how many squares per row
                iNbSquaresY = iHeight / iSize;                      //how many rows fits
                iNbSquares = iNbSquaresX * iNbSquaresY;
                if (iNbSquares >= iPaletteSize)
                {
                    //everything fit. finish the computation
                    bFinished     = true;
                    bScrollbars   = false;
                    iNbRows       = iNbSquaresY;
                    iNbColumns    = iNbSquaresX;
                    if (iNbColumns < 1) iNbColumns = 1;
                    if (iNbRows < 1) iNbRows = 1;
                    iColourWidth  = (iWidth - 2 * iDisplayWidth) / iNbColumns;
                    iColourHeight = iHeight / iNbRows;
                    iDisplayWidth  = iUnitX * 2;
                    iDisplayHeight = iHeight;
                    break;
                }
            }

            if (iDisplayHeight < iHeight) iDisplayHeight = iHeight;
        }

        if (!bFinished)
        {
            //5 - to add scrollbars
            int iHeightScrollBar;
            iHeightScrollBar = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y, this);

            bFinished     = true;
            bScrollbars   = true;
            iNbRows       = (iHeight - iHeightScrollBar) / XPM_IDEAL_SIZE;
            if (iNbRows < 1) iNbRows = 1;
            iNbColumns    = iPaletteSize / iNbRows;
            if (iNbRows * iNbColumns < iPaletteSize) iNbColumns++; //for avoiding round off errors.
            if (iNbColumns < 1) iNbColumns = 1;
            if (iNbRows < 1) iNbRows = 1;
            iColourWidth  = iUnitX;
            iColourHeight = (iHeight - iHeightScrollBar) / iNbRows;
            iDisplayWidth  = iUnitX * 2;
            iDisplayHeight = iHeight;
        }

        //corrections to avoid round off errors
        if (iColourWidth > iWidth) iColourWidth = iWidth;

    }
    else
    {
        //vertical layout
        //1 - to use the available space for the ideal number of rows/columns
        if (iMinimalSize < 1) iMinimalSize = 1;
        if (iIdealNbRowsOrCols < 1) iIdealNbRowsOrCols = 1;
        iColourWidth = iWidth / iIdealNbRowsOrCols; //the width of a square
        iNbSquaresY  = (iHeight - 2 * iDisplayHeight) / iMinimalSize; //how many squares fit on each column
        iNbSquaresX  = iIdealNbRowsOrCols;
        if ((iColourWidth >= iMinimalSize) && (iNbSquaresX * iNbSquaresY >= iPaletteSize))
        {
            //there are enough place for rows.
            //everything fit. finish the computation
            bFinished     = true;
            bScrollbars   = false;
            iNbColumns    = iIdealNbRowsOrCols;
            iNbRows       = iPaletteSize / iIdealNbRowsOrCols;
            if (iNbColumns * iNbRows < iPaletteSize) iNbRows++;
            if (iNbColumns < 1) iNbColumns = 1;
            if (iNbRows < 1) iNbRows = 1;
            iColourWidth  = iWidth / iNbColumns;
            iColourHeight = (iHeight - 2 * iDisplayHeight) / iNbRows;
            iDisplayWidth  = iWidth;
            iDisplayHeight = iUnitY * 2;
        }

        if (!bFinished)
        {
            //2 - to use the available space for any number of rows/columns
            iComputedRowsOrCols = iWidth / iUnitX;              //how many columns of ideal size fits in this
            iNbSquaresY = (iHeight - 2 * iDisplayHeight) / iUnitY; //how many squares per column
            if ((iComputedRowsOrCols >= iIdealNbRowsOrCols) && (iNbSquaresY * iComputedRowsOrCols >= iPaletteSize))
            {
                //there are enough place for rows.
                //everything fit. finish the computation
                bFinished     = true;
                bScrollbars   = false;
                iNbRows       = iNbSquaresY;
                iNbColumns    = iComputedRowsOrCols;
                if (iNbColumns < 1) iNbColumns = 1;
                if (iNbRows < 1) iNbRows = 1;
                iColourWidth  = iWidth / iNbColumns;
                iColourHeight = (iHeight - 2 * iDisplayHeight) / iNbRows;
                iDisplayWidth  = iWidth;
                iDisplayHeight = iUnitY * 2;
            }
        }


        if (!bFinished)
        {
            //3 - to use as many rows or columns with a minimal size as long as space is available
            //compute how many squares can fit :
            iNbSquaresX = iWidth / iUnitX;                         //how many columns
            iNbSquaresY = (iHeight - 2 * iDisplayHeight) / iUnitY; //how many squares per columns
            iNbSquares = iNbSquaresX * iNbSquaresY;
            if (iNbSquares >= iPaletteSize)
            {
                //everything fit. finish the computation
                bFinished     = true;
                bScrollbars   = false;
                iNbRows       = iNbSquaresY;
                iNbColumns    = iNbSquaresX;
                if (iNbColumns < 1) iNbColumns = 1;
                if (iNbRows < 1) iNbRows = 1;
                iColourWidth  = iWidth / iNbColumns;
                iColourHeight = (iHeight - 2 * iDisplayHeight) / iNbRows;
                iDisplayWidth  = iWidth;
                iDisplayHeight = iUnitY * 2;
            }
        }

        if (!bFinished)
        {
            //4 - to reduce the size of the squares until they fit or they are too small (iMinimalSize x iMinimalSize pixels)
            for(iSize = iUnitY; iSize >= iMinimalSize; iSize--)
            {
                iNbSquaresX = iWidth / iSize;                         //how many columns
                iNbSquaresY = (iHeight - 2 * iDisplayHeight) / iSize; //how many squares per columns
                iNbSquares = iNbSquaresX * iNbSquaresY;
                if (iNbSquares >= iPaletteSize)
                {
                    //everything fit. finish the computation
                    bFinished     = true;
                    bScrollbars   = false;
                    iNbRows       = iNbSquaresY;
                    iNbColumns    = iNbSquaresX;
                    if (iNbColumns < 1) iNbColumns = 1;
                    if (iNbRows < 1) iNbRows = 1;
                    iColourWidth  = iWidth / iNbColumns;
                    iColourHeight = (iHeight - 2 * iDisplayHeight) / iNbRows;
                    iDisplayWidth  = iWidth;
                    iDisplayHeight = iUnitY * 2;
                    break;
                }
            }
        }

        if (!bFinished)
        {
            //5 - to add scrollbars
            int iWidthScrollBar;
            iWidthScrollBar  = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, this);

            bFinished     = true;
            bScrollbars   = true;
            iNbColumns    = (iWidth - iWidthScrollBar) / XPM_IDEAL_SIZE;
            if (iNbColumns < 1) iNbColumns = 1;
            iNbRows       = iPaletteSize / iNbColumns;
            if (iNbRows * iNbColumns < iPaletteSize) iNbRows++; //for avoiding round off errors.
            if (iNbColumns < 1) iNbColumns = 1;
            if (iNbRows < 1) iNbRows = 1;
            iColourWidth  = (iWidth - iWidthScrollBar) / iNbColumns;
            iColourHeight = iUnitY;
            iDisplayWidth  = iWidth;
            iDisplayHeight = iUnitY * 2;
        }
        //corrections to avoid roundoff errors
        if (iColourHeight > iHeight) iColourHeight = iHeight;
    }

    //set minimal values to avoid arithmetic errors (division by 0)
    if (iColourHeight < 1) iColourHeight = 1;
    if (iColourWidth < 1) iColourWidth = 1;
    if (iDisplayWidth < 1) iDisplayWidth = 1;
    if (iDisplayHeight < 1) iDisplayHeight = 1;

    //set minimum & maximum size
    wxSize sMinSize;
    wxSize sMaxSize;
    sMinSize = DoGetMinSize2();
    sMaxSize = DoGetMaxSize2();
    SetMinSize(sMinSize);
    SetMaxSize(sMaxSize);

    UpdateScrollbars();

    bComputed = true;
}

/** resize the scrollbars to match the current palette and control size
  */
void XPMColorPicker::UpdateScrollbars(void)
{
    if (!bScrollbars)
    {
        //hide the scrollbars
       if (sbHorizScrollBar) sbHorizScrollBar->Hide();
       if (sbVertScrollBar) sbVertScrollBar->Hide();
    }
    else
    {
        //show the scrollbars
        int iPageSize, iOrientation2;
        wxSize sClientSize;
        sClientSize = GetClientSize();

        iOrientation2 = GetActualOrientation();

        //horizontal scrollbar
        iPageSize = (sClientSize.GetWidth() - 2 * iDisplayWidth) / iColourWidth;
        if (iScrollBarPos > iNbColumns - iPageSize) iScrollBarPos = 0;
        if ((sbHorizScrollBar) && (iOrientation2 == XPM_HORIZONTAL_ORIENTATION))
        {
            sbHorizScrollBar->SetScrollbar(iScrollBarPos, iPageSize, iNbColumns, true);
            sbHorizScrollBar->SetSize(2*iDisplayWidth + 1, sClientSize.GetHeight() - iColourHeight + 1,
                                      sClientSize.GetWidth() - 2*iDisplayWidth - 2, iColourHeight - 2
                                     );
            sbHorizScrollBar->Show();
            if (sbVertScrollBar) sbVertScrollBar->Hide();
        }

        //vertical scrollbar
        iPageSize = (sClientSize.GetHeight() - 2 * iDisplayHeight) / iColourHeight;
        if (iScrollBarPos > iNbRows - iPageSize) iScrollBarPos = 0;
        if ((sbVertScrollBar) && (iOrientation2 == XPM_VERTICAL_ORIENTATION))
        {
            sbVertScrollBar->SetScrollbar(iScrollBarPos, iPageSize, iNbRows, true);
            sbVertScrollBar->SetSize(sClientSize.GetWidth() - iColourWidth + 1, 2 * iDisplayHeight + 1,
                                     iColourWidth - 2, sClientSize.GetHeight() - 2 * iDisplayHeight - 2
                                    );
            sbVertScrollBar->Show();
            if (sbHorizScrollBar) sbHorizScrollBar->Hide();
        }
    }
}

/** Compute the minimal size of the push button,
  * and use it to deduce the sizes for the other components
  * The computed values are stored in iUnitX and iUnitY
  */
void XPMColorPicker::GetLengthUnit(void)
{
    wxFont font;
    wxSize s1, s2;
    wxClientDC dc(this);

    font = GetFont();
    dc.SetFont(font);
    s1 = dc.GetTextExtent(_("Fill"));
    s2 = dc.GetTextExtent(_("Line"));

    iUnitX = s1.GetWidth();
    if (s2.GetWidth() > iUnitX) iUnitX = s2.GetWidth();
    iUnitX = iUnitX / 2 + 1;
    if (iUnitX < XPM_IDEAL_SIZE) iUnitX = XPM_IDEAL_SIZE;
    if (iUnitX < iMinimalSize) iUnitX = iMinimalSize;

    iUnitY = s1.GetHeight();
    if (s2.GetHeight() > iUnitY) iUnitY = s2.GetHeight();
    iUnitY = iUnitY / 2 + 1;
    if (iUnitY < XPM_IDEAL_SIZE) iUnitY = XPM_IDEAL_SIZE;
    if (iUnitY < iMinimalSize) iUnitY = iMinimalSize;

    //make the biggest square possible
    if (bSquare)
    {
        if (iUnitX > iUnitY) iUnitY = iUnitX;
        if (iUnitY > iUnitX) iUnitX = iUnitY;
    }

    //avoid arithmetic errors (division by 0)
    if (iUnitX < 1) iUnitX = 1;
    if (iUnitY < 1) iUnitY = 1;
}

/** Get the maximal size of the control (borders included)
  * The maximal size will consider that all colours can fit on 2 rows
  * or 2 columns, depending on the orientation
  * The lenght units are recomputed
  * \return the computed maximal size
  */
wxSize XPMColorPicker::DoGetMaxSize(void)
{
    GetLengthUnit();
    return(DoGetMaxSize2());
}

/** Get the maximal size of the control (borders included)
  * The maximal size will consider that all colours can fit on 2 rows
  * or 2 columns, depending on the orientation
  * The lenght units are not recomputed
  * \return the computed maximal size
  */
wxSize XPMColorPicker::DoGetMaxSize2(void)
{
    int iBestWidth, iBestHeight;
    int iOrientation2;
    wxSize sResult;

    iOrientation2 = GetActualOrientation();

    if (iOrientation2 == XPM_HORIZONTAL_ORIENTATION)
    {
        iBestWidth  = iUnitX * ((iPaletteSize / 2) + 4) + 2;
        iBestHeight = iUnitY * 2 + 2;
    }
    else
    {
        iBestHeight = iUnitY * ((iPaletteSize / 2) + 4) + 2;
        iBestWidth  = iUnitX * 2 + 2;
    }


    //include the border size
    wxSize sBorderSize;
    sBorderSize = GetWindowBorderSize();
    iBestWidth = iBestWidth + sBorderSize.GetWidth() * 2 + 2;
    iBestHeight = iBestHeight + sBorderSize.GetHeight() * 2 + 2;

    sResult.SetWidth(iBestWidth);
    sResult.SetHeight(iBestHeight);
    return(sResult);
}

/** Get the minimal size of the control (borders included)
  * The minimal size will consider that only 2 colours will be drawn,
  * with the current colour and transparent colour as well.
  * Scrollbar sizes are taken into account
  * The lenght units are recomputed
  * \return the computed minimal size
  */
wxSize XPMColorPicker::DoGetMinSize(void)
{
    GetLengthUnit();
    return(DoGetMinSize2());
}

/** Get the minimal size of the control (borders included)
  * The minimal size will consider that only 2 colours will be drawn,
  * with the current colour and transparent colour as well.
  * Scrollbar sizes are taken into account
  * The lenght units are not recomputed
  * \return the computed minimal size
  */
wxSize XPMColorPicker::DoGetMinSize2(void)
{
    int iBestWidth, iBestHeight;
    int iOrientation2;
    int iHeightScrollBar, iWidthScrollBar;
    wxSize sResult;

    iOrientation2 = GetActualOrientation();

    //include size of scrollbars if needed
    iHeightScrollBar = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y, this);
    iWidthScrollBar  = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, this);
    if (iPaletteSize > 2)
    {
        if (iOrientation2 == XPM_HORIZONTAL_ORIENTATION)
        {
            iBestWidth  = iUnitX * 6 + 2;
            if (iUnitY < iHeightScrollBar) iUnitY = iHeightScrollBar;
            iBestHeight = iUnitY * 2 + 2;
        }
        else
        {
            iBestHeight = iUnitY * 6 + 2;
            if (iUnitX < iWidthScrollBar) iUnitX = iWidthScrollBar;
            iBestWidth  = iUnitX * 2 + 2;
        }
    }
    else
    {
        if (iOrientation == XPM_HORIZONTAL_ORIENTATION)
        {
            iBestWidth  = iUnitX * 6 + 2;
            iBestHeight = iUnitY * 2 + 2;
        }
        else
        {
            iBestHeight = iUnitY * 6 + 2;
            iBestWidth  = iUnitX * 2 + 2;
        }
    }


    //include the border size
    wxSize sBorderSize;
    sBorderSize = GetWindowBorderSize();
    iBestWidth = iBestWidth + sBorderSize.GetWidth() * 2;
    iBestHeight = iBestHeight + sBorderSize.GetHeight() * 2;

    sResult.SetWidth(iBestWidth);
    sResult.SetHeight(iBestHeight);

    return(sResult);
}

/** to get the best size for the control (borders included)
  * Best size is the control on 2 rows
  * The lenght units are recomputed
  * \return the best computed size
  */
wxSize XPMColorPicker::DoGetBestSize(void)
{
    GetLengthUnit();
    return(DoGetBestSize2());
}


/** to get the best size for the control (borders included)
  * Best size is the control on 2 rows
  * The lenght units are not recomputed
  * \return the best computed size
  */
wxSize XPMColorPicker::DoGetBestSize2(void)
{
    int iBestWidth, iBestHeight;
    int iOrientation2;
    wxSize sResult;

    iOrientation2 = GetActualOrientation();

    if (iOrientation2 == XPM_HORIZONTAL_ORIENTATION)
    {
        iBestWidth = iUnitX * ((iPaletteSize / iIdealNbRowsOrCols) + 4) + 2;
        iBestHeight = iUnitX * 2 + 2;
    }
    else
    {
        iBestHeight = iUnitX * ((iPaletteSize / iIdealNbRowsOrCols) + 4) + 2;
        iBestWidth = iUnitX * 2 + 2;
    }


    //include the border size
    wxSize sBorderSize;
    sBorderSize = GetWindowBorderSize();
    iBestWidth = iBestWidth + sBorderSize.GetWidth() * 2;
    iBestHeight = iBestHeight + sBorderSize.GetHeight() * 2;

    sResult.SetWidth(iBestWidth);
    sResult.SetHeight(iBestHeight);
    return(sResult);
}

//----- Get and Set current colours  -----
/** Return the current line colour
  * If an error occur, the colour returned is BLACK by default
  * \return a wxColour representing the current line colour
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
  * \return a wxColour representing the current fill colour
  */
wxColour XPMColorPicker::GetFillColour(void)
{
    //return the current fill colour
    if (iFillColor < -1) return(*wxWHITE);
    if (iFillColor >= XPM_MAX_COLOR) return(*wxWHITE);
    if (iFillColor == -1) return(cTransparent);
    return(ColourArray[iFillColor]);
}

/** return the 1st unused colour
  * Unused colour are: not Fill Colour, not Line Colour, not transparent colour
  * This method will return a colour outside the palette if there are 3 colours or less in the current palette
  */
wxColour XPMColorPicker::GetUnusedColour(void)
{
    //return the 1st unused colour, or black if none is found

    if (iPaletteSize < 4)
    {
        //palette is too small
        wxColour colour1(255,0,0); //red
        wxColour colour2(0,255,0); //green
        wxColour colour3(0,0,255); //blue
        wxColour colour4(0,0,0);   //black
        wxColour colour5(0,0,0);   //white
        wxColour cFillColour;
        wxColour cLineColour;
        cFillColour = GetFillColour();
        cLineColour = GetLineColour();


        //try with the 5 colours
        if ((colour1 != cFillColour) && (colour1 != cLineColour) && (colour1 != cTransparent)) return(colour1);
        if ((colour2 != cFillColour) && (colour2 != cLineColour) && (colour2 != cTransparent)) return(colour2);
        if ((colour3 != cFillColour) && (colour3 != cLineColour) && (colour3 != cTransparent)) return(colour3);
        if ((colour4 != cFillColour) && (colour4 != cLineColour) && (colour4 != cTransparent)) return(colour4);
        if ((colour5 != cFillColour) && (colour5 != cLineColour) && (colour5 != cTransparent)) return(colour5);
        return(colour5);
    }

    int i;
    wxColour cColour;
    for(i=0;i<iPaletteSize;i++)
    {
        cColour = GetPaletteColour(i);
        if ((i != iLineColor) && (i != iFillColor) && (cColour != cTransparent)) return(cColour);
    }

    return(*wxBLACK);
}

/** Return the current line colour index
  * If an error occur, the colour returned is BLACK by default
  * \return the index representing the current line colour
  */
int XPMColorPicker::GetLineColourIndex(void)
{
    //return the current line colour
    return(iLineColor);
}

/** Return the current fill colour index
  * \return the index representing the current fill colour
  */
int XPMColorPicker::GetFillColourIndex(void)
{
    //return the current fill colour
    return(iFillColor);
}

/** Set the current line colour
  * If an error occur, the colour set is the 1st colour of the palette by default
  * \param iIndex: the index in the wxColourArray representing the new line colour
  * \param bPostEvent: if true, a wxEVT_LINE_COLOR_CHANGED is generated (default)
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
        GetEventHandler()->ProcessEvent(eventColorChanged);
    }

    Refresh(false, NULL);
    Update();
}

/** Set the current fill colour
  * If an error occur, the colour set is the 1st colour of the palette by default
  * \param iIndex: the index in the wxColourArray representing the new fill colour
  * \param bPostEvent: if true, a wxEVT_FILL_COLOR_CHANGED is generated (default)
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
        GetEventHandler()->ProcessEvent(eventColorChanged);
    }

    Refresh(false, NULL);
    Update();
}

/** get the palette size
  * \return the amount of colours in the palette
  */
int XPMColorPicker::GetPaletteSize(void)
{
    //get the palette size
    return(iPaletteSize);
}

/** set the palette size
  * if the new palette is larger, then all the new colours are initialized to blakc
  * \param iSize: the new amount of colours in the palette
  * \return true on success, false on failure
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
    iScrollBarPos = 0;
    ComputeDimensions(GetClientSize());
    Refresh(false, NULL);
    Update();
    return(true);
}

/** set the specific palette colour
  * \param iIndex: the index of the colour to be changed. If not valid,nothing is done
  * \param cColor: the new colour
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
  * \param iIndex: the index of the colour to be changed. If not valid, wxBLACK is returned
  * \return  the colour
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
  * \return : the transparent colour
  */
wxColour XPMColorPicker::GetTransparentColour(void)
{
    //Get the transparent colour
    return(cTransparent);
}

/** Set the transparent colour
  * \param cColour: the new transparent colour
  * \param bPostEvent: if true, a wxEVT_TRANSPARENT_COLOR_CHANGED is generated (default)
  */
void XPMColorPicker::SetTransparentColour(wxColour cColour, bool bPostEvent)
{
    //Set the transparent colour
    cTransparent = cColour;

    if (bPostEvent)
    {
        wxCommandEvent eventColorChanged(wxEVT_TRANSPARENT_COLOR_CHANGED, GetId());
        eventColorChanged.SetEventObject(this);
        GetEventHandler()->ProcessEvent(eventColorChanged);
    }
}

//--- MOUSE EVENTS HANDLERS -----
/** The Mouse enters the window. Set the cursor
  * \param event: the event descriptor
  */
void XPMColorPicker::OnMouseEnter(wxMouseEvent& event)
{
    //The Mouse enters the window. Set the cursor
    SetCursor(*wxCROSS_CURSOR);
    event.Skip();
}

/** The Mouse leaves the window. Set the cursor
  * \param event: the event descriptor
  */
void XPMColorPicker::OnMouseLeave(wxMouseEvent& event)
{
    //The Mouse leaves the window. Set the cursor
    SetCursor(*wxSTANDARD_CURSOR);
    event.Skip();
}

/** Mouse motion event handler
  * \param event: the event descriptor
  */
void XPMColorPicker::OnMouseMove(wxMouseEvent& event)
{
    //Set the cursor
    int iResult;

    iResult = GetColourIndex(event.GetX(), event.GetY());
    if ((iResult == -2) || (iResult == -3)) SetCursor(*wxSTANDARD_CURSOR); else SetCursor(*wxCROSS_CURSOR);
    event.Skip();
}

/** Set the cursor event
  * \param event: the event descriptor
  */
void XPMColorPicker::OnSetCursor(wxSetCursorEvent& event)
{
    //Set the cursor
    int iResult;

    iResult = GetColourIndex(event.GetX(), event.GetY());
    if ((iResult == -2) || (iResult == -3)) event.SetCursor(*wxSTANDARD_CURSOR); else event.SetCursor(*wxCROSS_CURSOR);
    event.Skip();
}

/** Process a double click event
  * \param event: the event descriptor
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
  * \param event: the event descriptor
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
    else if ((iColourIndex == -2) && (!(event.ButtonDClick(wxMOUSE_BTN_RIGHT))))
    {
        bLineOn = !bLineOn;
        Refresh(false, NULL);
        Update();
        return;
    }
    event.Skip();
}

/** Process a right click event
  * \param event: the event descriptor
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
    else if ((iColourIndex == -2) && (!(event.ButtonDClick(wxMOUSE_BTN_RIGHT))))
    {
        bLineOn = !bLineOn;
        Refresh(false, NULL);
        Update();
        return;
    }
    event.Skip();
}

/** Return the index of the colour clicked
  * \return : -4 : unknown area or no colours area
  *           -3 : scrollbar has been clicked
  *           -2 : the toggle button has been clicked
  *           -1 : transparent colour clicked
  *           <-2: no colour clicked
  *           >=0: the index of the colour clicked
  */
int XPMColorPicker::GetColourIndex(int x, int y)
{
    //Return the index of the colour clicked
    wxSize sWindowSize;
    int iRow, iCol, iIndex, iNbSquaresPerRow;
    int iOrientation2;

    iOrientation2 = GetActualOrientation();

    sWindowSize = GetSize();

    if (iOrientation2 == XPM_HORIZONTAL_ORIENTATION)
    {
        //horizontal layout

        //in the display square
        if (x < iDisplayWidth) return(-4);
        //transparent colour or toggle button
        if (x < 2 * iDisplayWidth)
        {
            if (y <= iDisplayHeight / 2) return(-1); //transparent colour clicked
            return(-2); //toggle button clicked
        }

        //colour area: can be a colour square, the scrollbar (-3), no colours (-4)
        iRow = y / iColourHeight;
        iCol = (x - iDisplayWidth * 2) / iColourWidth;
        iNbSquaresPerRow = iPaletteSize / iNbRows;
        if (iNbSquaresPerRow * iNbRows < iPaletteSize) iNbSquaresPerRow++; //for uneven iPaletteSize
        if (bScrollbars)
        {
            iCol = iCol + iScrollBarPos;
            if (y >= sWindowSize.GetHeight() - iColourHeight) return(-3); //in the scrollbar area
        }
        iIndex = iRow * iNbSquaresPerRow + iCol;
        if (iIndex < iPaletteSize) return(iIndex);
    }
    else
    {
        //vertical layout

        //in the display square
        if (y < iDisplayHeight) return(-4);
        //transparent colour or toggle button
        if (y < 2 * iDisplayHeight)
        {
            if (y <= 3 * iDisplayHeight / 4) return(-1); //transparent colour clicked
            return(-2); //toggle button clicked
        }

        //colour area: can be a colour square, the scrollbar (-3), no colours (-4)
        iRow = (y - iDisplayHeight * 2) / iColourHeight;
        iCol = x / iColourWidth;
        iNbSquaresPerRow = iPaletteSize / iNbRows;
        if (iNbSquaresPerRow * iNbRows < iPaletteSize) iNbSquaresPerRow++; //for uneven iPaletteSize
        if (bScrollbars)
        {
            iRow = iRow + iScrollBarPos;
            if (x >= sWindowSize.GetWidth() - iColourWidth) return(-3); //in the scrollbar area
        }
        iIndex = iRow * iNbSquaresPerRow + iCol;
        if (iIndex < iPaletteSize) return(iIndex);
    }

    return(-4);
}

