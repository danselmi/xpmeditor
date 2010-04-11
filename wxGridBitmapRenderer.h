/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxGridBitmapRenderer.h
 * Purpose:   a wxBitmap renderer for wxGrid - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef WXGRIDBITMAPRENDERER_H
#define WXGRIDBITMAPRENDERER_H

#include <wx/grid.h>
#include <wx/bitmap.h>

class wxGridBitmapRenderer : public wxGridCellRenderer
{
    public:
        //Constructors & destructors
        wxGridBitmapRenderer(void);  ///< @brief default constructor
        wxGridBitmapRenderer(wxBitmap bm);  ///< @brief constructor
        ~wxGridBitmapRenderer(void); ///< @brief destructor

        //wxGridCellRenderer methods implementation
        virtual void Draw(wxGrid& grid, wxGridCellAttr& attr,
                          wxDC& dc, const wxRect& rect,
                          int row, int col, bool isSelected); ///< @brief draw the cell

        virtual wxSize GetBestSize(wxGrid& grid, wxGridCellAttr& attr,
                                   wxDC& dc, int row, int col); ///< @brief return the ideal size for the cell

        virtual wxGridCellRenderer* Clone(void) const; ///< @brief clone the renderer

        //Get / Set the bitmap
        wxBitmap GetBitmap(void);        ///< @brief Get the bitmap
        void SetBitmap(wxBitmap val);    ///< @brief Set the bitmap

    private:
        wxBitmap m_Bitmap; ///< @brief The wxBitmap to draw
};

#endif // WXGRIDBITMAPRENDERER_H
