/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxGridBitmapRenderer.h
 * Purpose:   a wxBitmap renderer for wxGrid - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "wxGridBitmapRenderer.h"
#include <wx/dc.h>

/**Default Constructor
  */
wxGridBitmapRenderer::wxGridBitmapRenderer(void) : wxGridCellRenderer()
{
    //Default Constructor
    m_Bitmap = wxNullBitmap;
}

/**Constructor
  */
wxGridBitmapRenderer::wxGridBitmapRenderer(wxBitmap bm) : wxGridCellRenderer()
{
    //constructor
    m_Bitmap = bm;
}

/** Destructor
  */
wxGridBitmapRenderer::~wxGridBitmapRenderer()
{
    //Destructor
}

/** Return the wxBitmap object belonging to the cell
  * @return the wxBitmap object currently to be drawn by the cell
  */
wxBitmap wxGridBitmapRenderer::GetBitmap(void)
{
    //return the wxBitmap object currently to be drawn by the cell
    return(m_Bitmap);
}

/** Set the wxBitmap object belonging to the cell
  * @param val: the wxBitmap object currently to be drawn by the cell
  */
void wxGridBitmapRenderer::SetBitmap(wxBitmap val)
{
    m_Bitmap = val;
}

/** Draw the cell
  * @param grid: the wxGrid to which the cell belongs
  * @param attr: the wxGridCellAttribute applicable to the cell to draw
  * @param dc: the wxDC on which to draw
  * @param rect: the clipping wxRect
  * @param row : the row of the cell to draw
  * @param col : the column of the cell to draw
  * @param iSelected: true if the cell is currently selected
  */
void wxGridBitmapRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr,
                          wxDC& dc, const wxRect& rect,
                          int row, int col, bool isSelected)
{
    //draw the cell
    wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
    if (m_Bitmap.IsOk())
    {
        int iXStart, iYStart;

        if (rect.GetWidth() - m_Bitmap.GetWidth() > 0)
        {
            iXStart = rect.GetLeft() + (rect.GetWidth() - m_Bitmap.GetWidth()) / 2;
        }
        else
        {
            iXStart = rect.GetLeft() + 1;
        }

        if (rect.GetHeight() - m_Bitmap.GetHeight() > 0)
        {
            iYStart = rect.GetTop() + (rect.GetHeight() - m_Bitmap.GetHeight()) / 2;
        }
        else
        {
            iYStart = rect.GetTop() + 1;
        }
        dc.DrawBitmap(m_Bitmap, iXStart, iYStart, true);
    }

}

/** Draw the cell
  * @param grid: the wxGrid to which the cell belongs
  * @param attr: the wxGridCellAttribute applicable to the cell to evaluate
  * @param dc: the wxDC which will be used to draw
  * @param row : the row of the cell to evaluate
  * @param col : the column of the cell to evaluate
  * @return the ideal size of the cell (wxSize)
  */
wxSize wxGridBitmapRenderer::GetBestSize(wxGrid& grid, wxGridCellAttr& attr,
                                   wxDC& dc, int row, int col)
{
    //return the ideal size for the cell
    wxSize sRet;

    if (m_Bitmap.IsOk())
    {
        sRet.SetWidth(m_Bitmap.GetWidth() + 2);
        sRet.SetHeight(m_Bitmap.GetHeight() + 2);
    }
    else
    {
        sRet.SetWidth(10);
        sRet.SetHeight(10);
    }

    return(sRet);
}

/** Return a copy of the renderer
  * @return a wxGridCellRenderer - it is a copy of the renderer
  */
wxGridCellRenderer* wxGridBitmapRenderer::Clone(void) const
{
    //clone the renderer
    wxGridBitmapRenderer *renderer = new wxGridBitmapRenderer;
    if (renderer)
    {
        renderer->SetBitmap(m_Bitmap);
    }

    return renderer;
}
