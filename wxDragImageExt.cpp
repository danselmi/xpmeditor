/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxDragImageExt.cpp
 * Purpose:   class for generic drag image, with as few flicker as possible - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include <sdk.h>
#include "wxDragImageExt.h"

IMPLEMENT_DYNAMIC_CLASS(wxDragImageExt, wxGenericDragImage);

/** Constructor
  */
wxDragImageExt::wxDragImageExt(const wxBitmap& image, const wxCursor& cursor)
{
    m_dScale = 1;
    m_isDirty = false;
    m_isShown = false;
    m_windowDC = (wxDC*) NULL;
    m_window = (wxScrolledWindow*) NULL;
    m_backingBitmap = wxNullBitmap;


    // We don't have to combine the cursor explicitly since we simply show the cursor
    // as we drag. This currently will only work within one window.
    m_cursor = cursor;
    m_bitmap = image;
}

/** Destructor
  */
wxDragImageExt::~wxDragImageExt(void)
{
    if (m_windowDC)
    {
        delete m_windowDC;
    }
}

/** Set the destination scaling factor
  * \param dScale: the new scaling factor.
  *                nothing is done if dScale <= 0
  */
void wxDragImageExt::SetScale(double dScale)
{
    if (dScale > 0) m_dScale = dScale;
}

/** Begin drag.
  * \param hotspot: location of the drag position relative to the upper-left
  *                 corner of the image.
  * \param window: the target window. Cannot be NULL
  * \param rect : the bounding rectangle, enclosing the dragged area. It will not be possible to drag the image
  *               outside this area
  * \return true on success, false on failure
  */
bool wxDragImageExt::BeginDrag(const wxPoint& hotspot, wxWindow* window, wxRect* rect)
{
    if (!window) return(false);

    // The image should be offset by this amount
    m_offset = hotspot;
    m_window = window;

    //compute bounding rectangle
    wxSize sClientSize;
    sClientSize = window->GetClientSize();
    m_boundingRect.x = 0;
    m_boundingRect.y = 0;
    m_boundingRect.width = sClientSize.GetWidth();
    m_boundingRect.height = sClientSize.GetHeight();
    if (rect)
    {
        m_boundingRect = *rect;
        if (m_boundingRect.width > sClientSize.GetWidth()) m_boundingRect.width = sClientSize.GetWidth();
        if (m_boundingRect.height > sClientSize.GetHeight()) m_boundingRect.height = sClientSize.GetHeight();
        if (m_boundingRect.x < 0) m_boundingRect.x = 0;
        if (m_boundingRect.y < 0) m_boundingRect.y = 0;
    }

    m_backingBitmap = wxBitmap(m_boundingRect.width, m_boundingRect.height);

    m_isDirty = false;

    if (window)
    {
        window->CaptureMouse();

        if (m_cursor.Ok())
        {
            m_oldCursor = window->GetCursor();
            window->SetCursor(m_cursor);
        }
    }

    // Make a copy of the window so we can repair damage done as the image is dragged.
    wxMemoryDC memDC;
    memDC.SelectObject(m_backingBitmap);

    UpdateBackingFromWindow(*m_windowDC, memDC, m_boundingRect, wxRect(0, 0, m_boundingRect.width, m_boundingRect.height));

    memDC.SelectObject(wxNullBitmap);

    m_windowDC = new wxClientDC(window);
    if (!m_windowDC) return(false);

    return(true);
}

/** End the drag
  * \return true on success, false on failure
  */
bool wxDragImageExt::EndDrag(void)
{
    if (m_window)
    {
#ifdef __WXMSW__
        // Under Windows we can be pretty sure this test will give
        // the correct results
        if (wxWindow::GetCapture() == m_window)
#endif
            m_window->ReleaseMouse();

        if (m_cursor.Ok() && m_oldCursor.Ok())
        {
            m_window->SetCursor(m_oldCursor);
        }
    }

    if (m_windowDC)
    {
        m_windowDC->DestroyClippingRegion();
        delete m_windowDC;
        m_windowDC = (wxDC*) NULL;
    }

    m_repairBitmap = wxNullBitmap;

    return(true);
}

/** Move the image: call from OnMouseMove. Pt is in window client coordinates if window
  * is non-NULL, or in screen coordinates if NULL.
  * \param pt: the coordinates of the mouse cursor.
  *            This is NOT the coordinates of the upper left corner of the dragged image
  *            Pt is in window client coordinates
  * \return true on success, false on failure
  */
bool wxDragImageExt::Move(const wxPoint& pt)
{
   if (!m_windowDC) return(false);

    wxPoint pt2(pt);

    // Erase at old position, then show at the current position
    wxPoint oldPos = m_position;

    bool eraseOldImage = (m_isDirty && m_isShown);

    if (m_isShown)
    {
        RedrawImage(oldPos - m_offset, pt2 - m_offset, eraseOldImage, true);
    }

    m_position = pt2;

    if (m_isShown) m_isDirty = true;

    return(true);
}

/** Show the image
  * \return true on success, false on failure
  */
bool wxDragImageExt::Show(void)
{
    if (!m_windowDC) return(false);

    // Show at the current position

    if (!m_isShown)
    {
        // This is where we restore the backing bitmap, in case
        // something has changed on the window.
        wxMemoryDC memDC;
        memDC.SelectObject(m_backingBitmap);

        UpdateBackingFromWindow(*m_windowDC, memDC, m_boundingRect, wxRect(0, 0, m_boundingRect.width, m_boundingRect.height));

        memDC.SelectObject(wxNullBitmap);

        RedrawImage(m_position - m_offset, m_position - m_offset, false, true);
    }

    m_isShown = true;
    m_isDirty = true;

    return(true);
}

/** Hide the image
  * \return true on success, false on failure
  */
bool wxDragImageExt::Hide(void)
{
     if (!m_windowDC) return(false);

    // Repair the old position

    if (m_isShown && m_isDirty)
    {
        RedrawImage(m_position - m_offset, m_position - m_offset, true, false);
    }

    m_isShown = false;
    m_isDirty = false;

    return(true);
}

/** Draw the dragged bitmap, as if it was a cursor
  * Override this if you are using a virtual image (drawing your own image)
  * \param dc : the wxDC on which the bitmap has to be drawn
  * \param pos : the position of the upper left corner of the bitmap to draw
  *              If the drag is limited to a window, then the coordinates are client coordinates
  *              If whole screen is used, then they are screen coordinates
  * \return true on success, false on failure
  */
bool wxDragImageExt::DoDrawImage(wxDC& dc, const wxPoint& pos) const
{

    if (m_bitmap.Ok())
    {
        if (m_dScale == 0.0) return(false);
        wxBitmap bmp(m_bitmap);
        wxMemoryDC mdc(bmp);
        mdc.SetUserScale(1 / m_dScale, 1/ m_dScale);
        dc.SetUserScale(1, 1);
        dc.Blit(pos.x,pos.y, m_bitmap.GetWidth() * m_dScale, m_bitmap.GetHeight() * m_dScale, &mdc, 0, 0);
        return(true);
    }

    return(false);
}

/** Clip the enclosing wxRect of the dragged bitmap to the client area of the destination window
  * \param pos the position of the upper left corner of the bitmap to draw
  *            If the drag is limited to a window, then the coordinates are client coordinates
  *            If whole screen is used, then they are screen coordinates
  */
wxRect wxDragImageExt::ComputeBitmapRectangle(wxPoint& pos) const
{
    if (m_bitmap.Ok())
    {
        int x, y, w, h;
        x = pos.x;
        y = pos.y;
        w = m_bitmap.GetWidth();
        h = m_bitmap.GetHeight();

        ClipBitmap(&x, &y, &w, &h);
        x = x - pos.x;
        y = y - pos.y;

        wxRect r(x, y, w, h);
        return(r);
    }

    return(wxRect(pos.x, pos.y, 0, 0));
}

/** Compute the wxRect enclosing the dragged bitmap, as if it was a cursor
  * Override this if you are using a virtual image (drawing your own image)
  * \param pos: the position of the upper left corner of the bitmap to draw
  *              If the drag is limited to a window, then the coordinates are client coordinates
  *              If whole screen is used, then they are screen coordinates
  * \return the wxRect containing the whole bitmap to draw
  *         the value is clipped to the client area of the destination window if necessary
  *         On error, the wxRect as a width & height equal to 0
  */
wxRect wxDragImageExt::GetImageRect(const wxPoint& pos) const
{
    int x, y, w, h;

    if (m_bitmap.Ok())
    {
        //add 1 pixel on each side, to avoid round off errors
        x = pos.x - 1;
        y = pos.y - 1;
        w = m_bitmap.GetWidth() * m_dScale + 1;
        h = m_bitmap.GetHeight() * m_dScale + 1;
    }
    else
    {
        x = pos.x;
        y = pos.y;
        w = 0;
        h = 0;
    }

    //no need to have a rectangle bigger than the client size of the window
    ClipBitmap(&x, &y, &w, &h);

    wxRect rResult(x,y,w,h);

    return(rResult);

}

/** Clip the bitmap size to the visible client area of the target window
  * \param x : [in , out] the X coordinate of the upper left corner of the bitmap,
  *            in client coordinate
  * \param y : [in , out] the Y coordinate of the upper left corner of the bitmap,
  *            in client coordinate
  * \param width: [in, out] the width of the bitmap
  * \param height: [in, out] the height of the bitmap
  */
void wxDragImageExt::ClipBitmap(int *x, int *y, int *width, int *height) const
{
    if (!m_window) return;
    if ((!x) || (!y) || (!width) || (!height)) return;

    wxSize sClientSize;
    sClientSize = m_window->GetClientSize();

    if (*x < 0)
    {
        //the bitmap left edge is beyond the visible portion of the window
        *width = *width + *x; //reduce the width of the image
        if (*width < 0) *width = 0;
        *x = 0;
    }

    if (*y < 0)
    {
        //the bitmap top edge is beyond the visible portion of the window
        *height = *height + *y; //reduce the height of the image
        if (*height < 0) *height = 0;
        *y = 0;
    }

    if (sClientSize.GetWidth() < *x + *width)
    {
        //the bitmap right edge is beyond the visible portion of the window
        *width = sClientSize.GetWidth() - *x; //reduce the width of the image
        if (*width < 0) *width = 0;
    }

    if (sClientSize.GetHeight() < *y + *width)
    {
        //the bitmap right edge is beyond the visible portion of the window
        *height = sClientSize.GetHeight() - *y; //reduce the height of the image
        if (*height < 0) *height = 0;
    }

}

/** Copy the window display to a bitmap
  * Override this if you wish to draw the window contents to the backing bitmap
  * yourself. This can be desirable if you wish to avoid flicker by not having to
  * redraw the window itself before dragging in order to be graphic-minus-dragged-objects.
  * Instead, paint the drag image's backing bitmap to be correct, and leave the window
  * to be updated only when dragging the objects away (thus giving a smoother appearance).
  * \param windowDC : the device context of the window where the dragging occurs
  * \param destDC : the memory device context which will receive the backing bitmap
  * \param sourceRect: the source rectangle to copy
  * \param destRect: the destination rectangle to copy
  * \return true on success, false on failure
  */
bool wxDragImageExt::UpdateBackingFromWindow(wxDC& windowDC, wxMemoryDC& destDC,
                                             const wxRect& sourceRect,
                                             const wxRect& destRect) const
{
    return(destDC.Blit(destRect.x, destRect.y, destRect.width, destRect.height, &windowDC,
                       sourceRect.x, sourceRect.y
                      )
          );
}

/** Erase and redraw simultaneously if possible
  * \param oldPos: the old position of the upper left corner of the dragged image
  *                In client coordinate (or screen coordinate if the drag is full screen)
  * \param newPos: the new position, of the upper left corner of the dragged image
  *                In client coordinate (or screen coordinate if the drag is full screen)
  * \param eraseOld: if true, the old bitmap will be erased first
  * \param drawNew: if true, the dragged bitmap will be redrawn
  */
bool wxDragImageExt::RedrawImage(const wxPoint& oldPos, const wxPoint& newPos,
                                 bool eraseOld, bool drawNew)
{
    if (!m_windowDC) return(false);
    Manager::Get()->GetLogManager()->Log(_("Step 1b"));
    if (!m_backingBitmap.Ok()) return(false);

    wxRect oldRect(GetImageRect(oldPos));
    wxRect newRect(GetImageRect(newPos));
    wxRect fullRect;
    int iMargin;

    iMargin = -0;

    // Full rect: the combination of both rects
    if (eraseOld && drawNew)
    {
        int oldRight = oldRect.GetRight();
        int oldBottom = oldRect.GetBottom();
        int newRight = newRect.GetRight();
        int newBottom = newRect.GetBottom();

        wxPoint topLeft = wxPoint(wxMin(oldPos.x, newPos.x), wxMin(oldPos.y, newPos.y));
        wxPoint bottomRight = wxPoint(wxMax(oldRight, newRight), wxMax(oldBottom, newBottom));

        fullRect.x = topLeft.x;
        fullRect.y = topLeft.y;
        fullRect.SetRight(bottomRight.x);
        fullRect.SetBottom(bottomRight.y);
    }
    else if (eraseOld)
    {
        fullRect = oldRect;
    }
    else if (drawNew)
    {
        fullRect = newRect;
    }

    fullRect.Inflate(iMargin);

    // Make the bitmap bigger than it need be, so we don't
    // keep reallocating all the time.
    int excess = 50;

    if ((!m_repairBitmap.Ok()) || ((m_repairBitmap.GetWidth() < fullRect.GetWidth()) || (m_repairBitmap.GetHeight() < fullRect.GetHeight())))
    {
        m_repairBitmap = wxBitmap(fullRect.GetWidth() + excess, fullRect.GetHeight() + excess);
    }

    wxMemoryDC memDC;
    memDC.SelectObject(m_backingBitmap);

    wxMemoryDC memDCTemp;
    memDCTemp.SelectObject(m_repairBitmap);

    // Draw the backing bitmap onto the repair bitmap.
    memDCTemp.Blit(0, 0, fullRect.GetWidth(), fullRect.GetHeight(), &memDC, fullRect.x - m_boundingRect.x, fullRect.y - m_boundingRect.y);

    // If drawing, draw the image onto the mem DC
    if (drawNew)
    {
        wxPoint pos(newPos.x - fullRect.x, newPos.y - fullRect.y) ;
        DoDrawImage(memDCTemp, pos);
    }

    // Now blit to the window
    // Finally, blit the temp mem DC to the window.
    m_windowDC->Blit(fullRect.x, fullRect.y, fullRect.width, fullRect.height, &memDCTemp, 0, 0);

    memDCTemp.SelectObject(wxNullBitmap);
    memDC.SelectObject(wxNullBitmap);

    return(true);
}

