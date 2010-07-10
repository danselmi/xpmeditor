/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMColorPicker.h
 * Purpose:   a simple wxWidget control presenting a panel of colours - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPMColorPicker_H
#define XPMColorPicker_H

#include <wx/colour.h>
#include <wx/control.h>
#include <wx/scrolbar.h>

#define XPM_MAX_COLOR  68
#define XPM_IDEAL_SIZE 16
#define XPM_MINIMAL_SIZE 12
#define XPM_HORIZONTAL_ORIENTATION 0
#define XPM_VERTICAL_ORIENTATION   1
#define XPM_AUTO_ORIENTATION       2

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_TRANSPARENT_COLOR_CHANGED, -1)
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_LINE_COLOR_CHANGED, -1)
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_FILL_COLOR_CHANGED, -1)
END_DECLARE_EVENT_TYPES()

/* The following does not work when compiling a plugin (dll), but works when compiling
   a standalone program: a link error is given, saying that _imp_wxEVT_TRANSPARENT_COLOR_CHANGED
   symbol is not found

   a dll import attribute is probably needed, and the use of the macro DECLARE_EXPORTED_EVENT_TYPE
   probably as well

   However, looking at the event.h file shows these definitions:

   #define DECLARE_LOCAL_EVENT_TYPE(name, value) \
    DECLARE_EXPORTED_EVENT_TYPE(wxEMPTY_PARAMETER_VALUE, name, value)

    So it may make no differences after all.

    TO DO: check the wxWidgets Doc and understand all these events macros.
    No need to get the old compatibility behaviour AFAIK

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_TRANSPARENT_COLOR_CHANGED, -1)
    DECLARE_EVENT_TYPE(wxEVT_LINE_COLOR_CHANGED, -1)
    DECLARE_EVENT_TYPE(wxEVT_FILL_COLOR_CHANGED, -1)
END_DECLARE_EVENT_TYPES()
*/

class XPMColorPicker : public wxWindow
{
    public:
        XPMColorPicker(wxWindow* parent,
                         wxWindowID id=wxID_ANY,
                         const wxPoint& pos=wxDefaultPosition,
                         const wxSize& size=wxDefaultSize,
                         int style = 0,
                         const wxString& name = _T("XPMColorPicker")
                        );
        ~XPMColorPicker();
        void init_colours(void); ///< \brief init the array of colours with default values.
        void Repaint(void);     ///< \brief immediate repainting of the widgets

        //colours getter / setter
        wxColour GetLineColour(void);   ///< \brief return the current line colour
        wxColour GetFillColour(void);   ///< \brief return the current fill colour
        wxColour GetUnusedColour(void); ///< \brief return the 1st unused colour
        int GetLineColourIndex(void);   ///< \brief return the current line colour index
        int GetFillColourIndex(void);   ///< \brief return the current fill colour index
        void SetLineColour(int iIndex, bool bPostEvent = true); ///< \brief set the current line colour
        void SetFillColour(int iIndex, bool bPostEvent = true); ///< \brief set the current fill colour
        int GetPaletteSize(void);       ///< \brief get the palette size
        bool SetPaletteSize(int iSize); ///< \brief set the palette size
        void SetPaletteColour(int iIndex, wxColour cColor); ///< \brief set the specific palette colour
        wxColour GetPaletteColour(int iIndex);          ///< \brief Get the specific palette colour
        wxColour GetTransparentColour(void);            ///< \brief Get the transparent colour
        void SetTransparentColour(wxColour cColour, bool bPostEvent = true);  ///< \brief Set the transparent colour

        //sizing methods
        virtual wxSize DoGetBestSize(void);        ///< \brief to get the best size for the control, according to the orientation (vertical or horizontal)
        virtual wxSize DoGetMinSize(void);         ///< \brief to get the min size for the control, according to the orientation (vertical or horizontal)
        virtual wxSize DoGetMaxSize(void);         ///< \brief to get the max size for the control, according to the orientation (vertical or horizontal)
        void SetOrientation(int iNewOrientation);  ///< \brief Set the orientation of the picker: horizontal or vertical or automatic
        int GetOrientation(void);                  ///< \brief Get the orientation of the picker: horizontal or vertical or automatic
        int GetActualOrientation(void) const;      ///< \brief Get the computed orientation of the picker if it is in automatic mode.
        void SetIdealNbLines(int iNewValue);       ///< \brief Set the ideal number of rows or columns for the picker
        int GetIdealNbLines(void);                 ///< \brief Get the ideal number of rows or columns for the picker
        void SetMinimalSize(int iNewValue);        ///< \brief Set the minimal size for a colour square
        int GetMinimalSize(void);                  ///< \brief Get the minimal size for a colour square
        void SetSquareDisplay(bool bNewValue);     ///< \brief Set if the picker should draw exact squares or rectangles
        bool GetSquareDisplay(void);               ///< \brief Get if the picker should draw exact squares or rectangles

    protected:
        //event handlers
        void OnEventPaint(wxPaintEvent& event);    ///< \brief Paint event handler
        void OnEraseBackground(wxEraseEvent);      ///< \brief Override background events - to avoid flicker
        void OnEventSize(wxSizeEvent& event);      ///< \brief Size event handler
        //scroll events handlers
        void OnScrollLineUp(wxScrollEvent& event);       ///< \brief Scrollbar event wxEVT_SCROLL_LINEUP
        void OnScrollLineDown(wxScrollEvent& event);     ///< \brief Scrollbar event wxEVT_SCROLL_LINEDOWN
        void OnScrollPageUp(wxScrollEvent& event);       ///< \brief Scrollbar event wxEVT_SCROLL_PAGEUP
        void OnScrollPageDown(wxScrollEvent& event);     ///< \brief Scrollbar event wxEVT_SCROLL_PAGEDOWN
        void OnScrollTop(wxScrollEvent& event);          ///< \brief Scrollbar event wxEVT_SCROLL_TOP
        void OnScrollBottom(wxScrollEvent& event);       ///< \brief Scrollbar event wxEVT_SCROLL_BOTTOM
        void OnScrollThumbTrack(wxScrollEvent& event);   ///< \brief Scrollbar event wxEVT_SCROLL_THUMBTRACK
        void OnScrollThumbRelease(wxScrollEvent& event); ///< \brief Scrollbar event wxEVT_SCROLL_THUMBRELEASE
        void OnScrollChanged(wxScrollEvent& event);      ///< \brief Scrollbar event wxEVT_SCROLL_CHANGED
        //mouse events handlers
        void OnMouseMove(wxMouseEvent& event);      ///< \brief The Mouse cursor is moved over the window. Set the cursor
        void OnMouseEnter(wxMouseEvent& event);     ///< \brief The Mouse enter the window. Set the cursor
        void OnMouseLeave(wxMouseEvent& event);     ///< \brief The Mouse leaves the window. Set the cursor
        void OnSetCursor(wxSetCursorEvent& event);  ///< \brief Set the cursor
        void OnMouseDClick(wxMouseEvent& event);    ///< \brief Process a double click event
        void OnMouseClick(wxMouseEvent& event);     ///< \brief Process a left click event
        void OnMouseRClick(wxMouseEvent& event);    ///< \brief Process a right click event
        int GetColourIndex(int x, int y);           ///< \brief Return the index of the colour clicked

        static const long ID_SCROLLBAR_HORIZ;       ///< \brief Identifier for the horizontal scrollbar
        static const long ID_SCROLLBAR_VERT;        ///< \brief Identifier for the vertical scrollbar

    private:
        //palette
        wxColour *ColourArray; ///< \brief The current colour array
        int iPaletteSize;      ///< \brief the palette size (number of colours)

        int iMinimalSize;   ///< \brief the minimal size a colour square must have
        int iColourWidth;   ///< \brief the width of a square for colour display
        int iColourHeight;  ///< \brief the Height of a square for colour display
        int iDisplayWidth;  ///< \brief the width of the Display square
        int iDisplayHeight; ///< \brief the height of the Display square
        int iNbRows;        ///< \brief the number of square rows
        int iNbColumns;     ///< \brief the number of square columns
        int iIdealNbRowsOrCols; ///< \brief the ideal number of square rows or columns (depending on orientation)
        int iOrientation;   ///< \brief indicates how the colours are listed: vertically or horizontally
        bool bScrollbars;   ///< \brief true if scrollbars are needed
        bool bComputed;     ///< \brief true if size calculations have been done
        int iUnitX;         ///< \brief the horizontal length unit, in pixel
        int iUnitY;         ///< \brief the vertical length unit, in pixel
        bool bSquare;       ///< \brief if true, force the picker to draw squares, and not rectangles.

        //current selection
        int iLineColor; ///< \brief the index of the current line color
        int iFillColor; ///< \brief the index of the current fill color
        wxColour cTransparent;  ///< \brief the transparent colour
        bool bLineOn; ///< \brief true if Line colour mode is selected, false otherwise

        //Paint methods
        void Paint(wxDC &dc);                              ///< \brief Paint the window
        void PaintDisplaySquare(wxDC &dc, wxRect rect);    ///< \brief Paint the square representing the selected colours
        void PaintTransparentColor(wxDC &dc, wxRect rect); ///< \brief Paint the square representing the transparent colour
        void PaintColorSquares(wxDC &dc, wxRect rect);     ///< \brief Paint all the colour squares
        void PaintSunkenBorder(wxDC &dc, wxRect rect);     ///< \brief Paint a sunken border.

        //size methods
        void GetLengthUnit(void);         ///< \brief Compute the length units, used to deduce all the other sizes
        void ComputeDimensions(wxSize s); ///< \brief Compute the dimensions for the colour squares
        wxSize DoGetBestSize2(void);      ///< \brief to get the best size for the control, according to the orientation (vertical or horizontal)
        wxSize DoGetMinSize2(void);       ///< \brief to get the min size for the control, according to the orientation (vertical or horizontal)
        wxSize DoGetMaxSize2(void);       ///< \brief to get the max size for the control, according to the orientation (vertical or horizontal)

        //scrollbars methods
        wxScrollBar *sbVertScrollBar;     ///< \brief the vertical scrollbar
        wxScrollBar *sbHorizScrollBar;    ///< \brief the horizontal scrollbar
        void UpdateScrollbars(void);      ///< \brief resize the scrollbars to match the current palette and control size
        int iScrollBarPos;                ///< \brief the current scroll position
};

#endif // XPMColorPicker_H
