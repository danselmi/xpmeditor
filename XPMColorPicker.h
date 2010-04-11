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

#include <wx/scrolwin.h>
#include <wx/colour.h>
#include <wx/control.h>

#define XPM_MAX_COLOR 68
#define XPM_IDEAL_SIZE 16

class wxToggleButton;

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_TRANSPARENT_COLOR_CHANGED, -1)
END_DECLARE_EVENT_TYPES()

class XPMColorPicker : public wxPanel
{
    public:
        XPMColorPicker(wxWindow* parent,
                         wxWindowID id=wxID_ANY,
                         const wxPoint& pos=wxDefaultPosition,
                         const wxSize& size=wxDefaultSize,
                         int style = 0,
                         const wxValidator &validator = wxDefaultValidator,
                         const wxString& name = _T("XPMColorPicker")
                        );
        ~XPMColorPicker();
        void init_colours(void); ///< @brief init the array of colours with default values.

        //colours getter / setter
        wxColour GetLineColour(void);   ///< @brief return the current line colour
        wxColour GetFillColour(void);   ///< @brief return the current fill colour
        int GetLineColourIndex(void);   ///< @brief return the current line colour index
        int GetFillColourIndex(void);   ///< @brief return the current fill colour index
        void SetLineColour(int iIndex); ///< @brief set the current line colour
        void SetFillColour(int iIndex); ///< @brief set the current fill colour
        int GetPaletteSize(void);       ///< @brief get the palette size
        bool SetPaletteSize(int iSize); ///< @brief set the palette size
        void SetPaletteColour(int iIndex, wxColour cColor); ///< @brief set the specific palette colour
        wxColour GetPaletteColour(int iIndex);          ///< @brief Get the specific palette colour
        wxColour GetTransparentColour(void);            ///< @brief Get the transparent colour
        void SetTransparentColour(wxColour cColour, bool bPostEvent = true);  ///< @brief Set the transparent colour

        virtual wxSize DoGetBestSize(void) const;  ///< @brief to get the best size for the control


    protected:
        //event handlers
        void OnEventPaint(wxPaintEvent& event);  ///< @brief Paint event handler
        void OnEventSize(wxSizeEvent& event);    ///< @brief Size event handler
        void OnToggleButton1Toggle(wxCommandEvent& event);  ///< @brief Toggle Fill / Line colour

        void OnMouseEnter(wxMouseEvent& event);     ///< @brief The Mouse enter the window. Set the cursor
        void OnMouseLeave(wxMouseEvent& event);     ///< @brief The Mouse leaves the window. Set the cursor
        void OnSetCursor(wxSetCursorEvent& event);  ///< @brief Set the cursor
        void OnMouseDClick(wxMouseEvent& event);    ///< @brief Process a double click event
        void OnMouseClick(wxMouseEvent& event);     ///< @brief Process a left click event
        void OnMouseRClick(wxMouseEvent& event);     ///< @brief Process a right click event
        int GetColourIndex(int x, int y);           ///< @brief Return the index of the colour clicked

        wxToggleButton* ToggleButton1;
        static const long ID_TOGGLEBUTTON1;

    private:
        //palette
        wxColour *ColourArray; ///< @brief The current colour array
        int iPaletteSize;      ///< @brief the palette size (number of colours)

        int iColourWidth;   ///< @brief the width of a square for colour display
        int iColourHeight;  ///< @brief the Height of a square for colour display
        int iHeight;        ///< @brief available height for drawing
        int iWidth;         ///< @brief available width for drawing
        int iNbRows;        ///< @brief the number of square rows
        void ComputeDimensions(wxSize s); ///< @brief Compute the dimensions for the colour squares

        //current selection
        int iLineColor; ///< @brief the index of the current line color
        int iFillColor; ///< @brief the index of the current fill color
        wxColour cTransparent;  ///< @brief the transparent colour
        bool bLineOn; ///< @brief true if Line colour mode is selected, false otherwise

};

#endif // XPMColorPicker_H
