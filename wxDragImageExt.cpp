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
#include <wx/scrolwin.h>
#include <wx/dcclient.h>

#include <manager.h>
#include <logmanager.h>

#include "wxDragImageExt.h"

#include <windef.h>
#include <wingdi.h>

IMPLEMENT_DYNAMIC_CLASS(wxDragImageExt, wxGenericDragImage);

/** Constructor
  */
wxDragImageExt::wxDragImageExt(const wxBitmap& image, const wxCursor& cursor)
{
    m_isDirty = false;
    m_isShown = false;
    m_windowDC = (wxDC*) NULL;
    m_window = (wxScrolledWindow*) NULL;
    m_backingBitmap = wxNullBitmap;
    m_dScale = 1.0;
    m_position.x = -1;
    m_position.y = -1;


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

    //Log(_("BeginDrag Start"));
    if (!window) return(false);
    //Log(wxString::Format(_("wxDragImageExt::BeginDrag m_offset.x=%d m_offset.y=%d"), m_offset.x, m_offset.y));

    // The image should be offset by this amount
    m_offset = hotspot;
    if (m_dScale > 0)
    {
        m_offset.x = m_offset.x * m_dScale;
        m_offset.y = m_offset.y * m_dScale;
    }
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

    //Log(_("BeginDrag OK"));

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
/** set the scale factor
  * \param dScale : the new scale factor
  */
void wxDragImageExt::SetScale(double dScale)
{
    if (dScale > 0.0)
    {
        m_offset.x = m_offset.x / m_dScale;
        m_offset.y = m_offset.y / m_dScale;
        m_dScale = dScale;
        m_offset.x = m_offset.x * m_dScale;
        m_offset.y = m_offset.y * m_dScale;
        //Log(wxString::Format(_("wxDragImageExt::SetScale m_offset.x=%d m_offset.y=%d"), m_offset.x, m_offset.y));
    }
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

    wxPoint newPos(pt);

    // Erase at old position, then show at the current position
    wxPoint oldPos = m_position;

    bool eraseOldImage = (m_isDirty && m_isShown);

    if (m_isShown)
    {
        RedrawImage(oldPos - m_offset, newPos - m_offset, eraseOldImage, true);
    }

    m_position = newPos;

    //Log(wxString::Format(_("wxDragImageExt::Move m_position.x=%d m_position.y=%d oldPos.x=%d oldPos.y=%d"), m_position.x, m_position.y, oldPos.x, oldPos.y));

    if (m_isShown) m_isDirty = true;

    return(true);
}

/** Show the image
  * \return true on success, false on failure
  */
bool wxDragImageExt::Show(void)
{
    //Log(wxString::Format(_("wxDragImageExt::Show m_position.x=%d m_position.y=%d "), m_position.x, m_position.y));
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
  * \param pos : the position of the upper left corner of the bitmap to draw, in the wxDC dc coordinates
  *              If the drag is limited to a window, then the coordinates are client coordinates
  *              If whole screen is used, then they are screen coordinates
  *              These coordinates are not direct coordinates: DoDrawImage draws the image on a memory bitmap
  *              These coordinates are therefore relative to this bitmap.
  *              The memory bitmap will be pasted on the wxWindow client area (or on the screen) at the coordinates newPos
  * \param newPos: the position of the upper left corner of the bitmap to draw, in the destination wxDC coordinates
  *                This is different than @pos, because the wxDC are not identical.
  *                If the drag is limited to a window, then the coordinates are client coordinates
  *                If whole screen is used, then they are screen coordinates
  *                These coordinates are direct coordinates.
  * \return true on success, false on failure
  */
bool wxDragImageExt::DoDrawImage(wxDC& dc, const wxPoint& pos, wxPoint newPos) const
{

    if ((m_bitmap.Ok()) && (pos.x <= m_boundingRect.GetRight()) && (pos.y <= m_boundingRect.GetBottom()))
    {
        wxBitmap bmp(m_bitmap);
        wxBitmap bmp2;
        if (!StretchBitmap(bmp, bmp2, 1.0, m_dScale, newPos)) return(false);



        wxMemoryDC mdc(bmp2);

        int x, y;
        x = pos.x;
        y = pos.y;
        if (x < 0) x = 0;
        if (y < 0) y = 0;

        dc.Blit(x, y, bmp2.GetWidth(), bmp2.GetHeight(), &mdc, 0, 0, wxCOPY, true);

        return(true);
    }

    return(false);
}

/** Compute the wxRect enclosing the dragged bitmap, as if it was a cursor
  * Override this if you are using a virtual image (drawing your own image)
  * \param pos: the position of the upper left corner of the bitmap to draw
  *              If the drag is limited to a window, then the coordinates are client coordinates
  *              If whole screen is used, then they are screen coordinates
  * \param bClipToDC : if true, the rectangle will be clipped to the destination wxDC
  *                    the rectangle will never overlaps any border from the destination wxDC
  * \return the wxRect containing the whole bitmap to draw
  *         the value is clipped to the client area of the destination window if necessary
  *         On error, the wxRect as a width & height equal to 0
  */
wxRect wxDragImageExt::GetImageRect(const wxPoint& pos, bool bClipToDC) const
{
    int x, y, w, h;

    if (m_bitmap.Ok())
    {
        x = pos.x;
        y = pos.y;
        w = m_bitmap.GetWidth() * m_dScale;
        h = m_bitmap.GetHeight() * m_dScale;
    }
    else
    {
        x = pos.x;
        y = pos.y;
        w = 0;
        h = 0;
    }

    //no need to have a rectangle bigger than the client size of the window
    if (bClipToDC) ClipBitmap(&x, &y, &w, &h);

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

    //Log(wxString::Format(_("wxDragImageExt::ClipBitmap BEFORE x=%d y=%d w=%d h=%d"), *x, *y, *width, *height));

    //Left border
    int iXRight;
    //int iXLeft;
    //iXLeft = *x;
    iXRight = *x + *width;
    *x = wxMax(*x, m_boundingRect.x);

    //right border
    iXRight = wxMin(iXRight, m_boundingRect.x + m_boundingRect.GetWidth());
    *width = iXRight - (*x);
    if (*width < 1) *width = 1;

    //top border
    //int iYTop;
    int iYBottom;
    //iYTop = *y;
    iYBottom = *y + *height ;
    *y = wxMax(*y, m_boundingRect.y);

    //right border
    iYBottom = wxMin(iYBottom, m_boundingRect.y + m_boundingRect.GetHeight());
    *height = iYBottom - (*y);
    if (*height < 1) *height = 1;

    //Log(wxString::Format(_("wxDragImageExt::ClipBitmap AFTER x=%d y=%d w=%d h=%d"), *x, *y, *width, *height));

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
    if (!m_backingBitmap.Ok()) return(false);

    //Log(wxString::Format(_("wxDragImageExt::RedrawImage old_pos.x=%d old_pos.y=%d new_pos.x=%d new_pos.y=%d"), oldPos.x, oldPos.y, newPos.x, newPos.y));

    wxRect oldRect(GetImageRect(oldPos));
    wxRect newRect(GetImageRect(newPos));
    wxRect fullRect;
    //int iMargin;

    //Log(wxString::Format(_("wxDragImageExt::RedrawImage oldRect.x=%d oldRect.y=%d oldRect.width=%d oldRect.height=%d"), oldRect.x, oldRect.y, oldRect.width, oldRect.height));
    //Log(wxString::Format(_("wxDragImageExt::RedrawImage newRect.x=%d newRect.y=%d newRect.width=%d newRect.height=%d"), newRect.x, newRect.y, newRect.width, newRect.height));

    //iMargin = -0;

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

    //fullRect.Inflate(iMargin);
    //Log(wxString::Format(_("wxDragImageExt::RedrawImage fullRect.x=%d fullRect.y=%d fullRect.width=%d fullRect.height=%d"), fullRect.x, fullRect.y, fullRect.width, fullRect.height));

    // Make the bitmap bigger than it need be, so we don't
    // keep reallocating all the time.
    int excess = 0;

    if ((!m_repairBitmap.Ok()) || ((m_repairBitmap.GetWidth() < fullRect.GetWidth()) || (m_repairBitmap.GetHeight() < fullRect.GetHeight())))
    {
        m_repairBitmap = wxBitmap(fullRect.GetWidth() + excess, fullRect.GetHeight() + excess);
    }

    wxMemoryDC memDC;
    memDC.SelectObject(m_backingBitmap);

    wxMemoryDC memDCTemp;
    memDCTemp.SelectObject(m_repairBitmap);

    // Draw the backing bitmap onto the repair bitmap.
    int x, y, x2, y2, w, h;
    x = 0; y = 0;
    if (fullRect.x < 0)
    {
        x = 0;
        x2 = -fullRect.x;
        w = fullRect.width + fullRect.x;
    }
    else
    {
        x = fullRect.x;
        x2 = 0;
        w = fullRect.width;
    }
    if (fullRect.y < 0)
    {
        y = 0;
        y2 = -fullRect.y;
        h = fullRect.height + fullRect.y;
    }
    else
    {
        y = fullRect.y;
        y2 = 0;
        h = fullRect.height;
    }

    memDCTemp.Blit(0, 0, w, h, &memDC, x, y);

    // If drawing, draw the image onto the mem DC
    if (drawNew)
    {
        wxPoint pos(newPos.x - fullRect.x, newPos.y - fullRect.y) ;
        DoDrawImage(memDCTemp, pos, newPos);
    }

    // Now blit to the window
    // Finally, blit the temp mem DC to the window.
    m_windowDC->Blit(x, y, w, h, &memDCTemp, 0, 0);

    memDCTemp.SelectObject(wxNullBitmap);
    memDC.SelectObject(wxNullBitmap);

    return(true);
}

/** Modify the scale of a bitmap
  * In addition, the destination bitmap is clipped to the bounding rectangle.
  * \param src: the source bitmap
  * \param dest: the resulting bitmap
  * \param dSrcScale: the original scale of the bitmap
  * \param dDestScale: the desired scale of the bitmap
  *        example: if dSrcScale == 1.0 and dDestScale == 2.0, then the
  *                 destination bitmap is twice the size of the original bitmap
  * \param pos : the upper left corner of the destination bitmap.
  *              these coordinates are relative to the destination (Window or Screen) DC
  *              used for clipping the bitmap to the visible portion of the screen
  * \param true on success, false on failure
  */
bool wxDragImageExt::StretchBitmap(wxBitmap src, wxBitmap &dest, double dSrcScale, double dDestScale, wxPoint pos) const
{
    if (!src.IsOk()) return(false);
    if (dSrcScale <= 0.0) return(false);
    if (dDestScale <= 0.0) return(false);

    //Step 1 : compute the dimensions of the stretched bitmap
    //Step 2 : find the bounding pixels of this stretched bitmap, to ensure it fits in the screen
    //         0,0 is the top left pixel of the stretched bitmap, which may be outside the visible part of the window
    //Step 3 : convert these bounding pixels to coordinates in the source bitmap
    //Step 4 : perform the stretch using a blit

    int iWidthSrc, iHeightSrc;     //dimensions of the Source bitmap
    int iWidthDest, iHeightDest;   //dimensions of the Destination bitmap
    int iXDestLeft, iYDestTop;     //position of the upper left corner of the destination bitmap (pixel coordinates of the bitmap)
    int iXDestRight, iYDestBottom; //position of the upper bottom right corner of the destination bitmap (pixel coordinates of the bitmap)
    int iXSrc,  iYSrc;             //position of the upper left corner of the source bitmap
    double dScaleFactor;

    //Step 1 : compute the dimensions of the stretched bitmap
    dScaleFactor = dDestScale / dSrcScale;
    iWidthDest  = src.GetWidth()  * dScaleFactor;
    iHeightDest = src.GetHeight() * dScaleFactor;

    //Step 2 : find the bounding pixels of this stretched bitmap, to ensure it fits in the screen
    //left border
    if (pos.x >= m_boundingRect.x)
    {
        iXDestLeft = 0;
    }
    else
    {
        iXDestLeft = m_boundingRect.x - pos.x;
    }
    //right border
    if (pos.x + iWidthDest < m_boundingRect.x + m_boundingRect.width + 1)
    {
        iXDestRight = iWidthDest - 1;
    }
    else
    {
        iXDestRight = m_boundingRect.x + m_boundingRect.width - pos.x;
    }
    //top border
    if (pos.y >= m_boundingRect.y)
    {
        iYDestTop = 0;
    }
    else
    {
        iYDestTop = m_boundingRect.y - pos.y;
    }
    //bottom border
    if (pos.y + iHeightDest < m_boundingRect.y + m_boundingRect.height + 1)
    {
        iYDestBottom = iHeightDest - 1;
    }
    else
    {
        iYDestBottom = m_boundingRect.y + m_boundingRect.height - pos.y;
    }
    iWidthDest = iXDestRight - iXDestLeft + 1;
    iHeightDest = iYDestBottom - iYDestTop + 1;
    //Log(wxString::Format(_("iXDestLeft=%d iXDestRight=%d iYDestTop=%d iYDestBottom=%d"), iXDestLeft, iXDestRight, iYDestTop, iYDestBottom));

    //Step 3 : convert these bounding pixels to coordinates in the source bitmap
    iXSrc = iXDestLeft / dScaleFactor;
    iYSrc = iYDestTop  / dScaleFactor;

    //Step 4 : perform the stretch using a blit
    //create the memory DC
    wxMemoryDC dcMemSrc;
    wxMemoryDC dcMemDest;
    wxBitmap destBmp(iWidthDest, iHeightDest);

    if (!destBmp.IsOk()) return(false);

    dcMemSrc.SelectObject(src);
    dcMemDest.SelectObject(destBmp);
    if (!dcMemSrc.IsOk()) return(false);
    if (!dcMemDest.IsOk()) return(false);

    SetUserScale(dcMemSrc, dSrcScale, dSrcScale);
    SetUserScale(dcMemDest, dDestScale, dDestScale);



    dcMemDest.Blit(0, 0, iWidthDest, iHeightDest, &dcMemSrc, iXSrc, iYSrc, wxCOPY, true, iXSrc, iYSrc); //WARNING: with this operation, the mask is lost !!

    dcMemDest.SelectObject(wxNullBitmap);

    //if (src.GetMask()) Log(_("src has mask.")); else Log(_("src has no mask."));
    //if (destBmp.GetMask()) Log(_("destBmp has mask.")); else Log(_("destBmp has no mask."));

    //get the mask and stretch it
    if (src.GetMask())
    {
        wxBitmap bmpMask(iWidthDest, iHeightDest);
        wxMemoryDC dcMemDestMask;

        dcMemDestMask.SelectObject(bmpMask);
        SetUserScale(dcMemDestMask, dDestScale, dDestScale);

        //set the bitmap in WHITE
        dcMemDestMask.SetBackground(*wxWHITE_BRUSH);
        ClearDC(dcMemDestMask);

        //copying with mask, and a XOR on it will make all the unmasked (==shown) pixels in BLACK
        dcMemDestMask.Blit(0, 0, iWidthDest, iHeightDest, &dcMemSrc, iXSrc, iYSrc, wxCOPY, true, iXSrc, iYSrc);
        dcMemDestMask.Blit(0, 0, iWidthDest, iHeightDest, &dcMemSrc, iXSrc, iYSrc, wxXOR, true, iXSrc, iYSrc);
        dcMemDestMask.SelectObject(wxNullBitmap);

        //now we have a bitmap where:
        //  1 - all the pixels which must be shown are BLACK
        //  2 - all the pixels which must be hidden are WHITE
        //This is therefore the REVERTED definition of a bitmap mask

        //Set the bitmap mask
        destBmp.SetMask(new wxMask(bmpMask, *wxWHITE)); //WHITE is defined as the TRANSPARENT colour - hidden pixels are TRANSPARENT.
    }


    dcMemSrc.SelectObject(wxNullBitmap);

    dest = destBmp;

    return(true);
}


/** This method is used to work around a wxWidgets bug in MSW port
  * The bug occurs when scaling a DC (wxDC::SetUserScale).
  * There is a cumulative round-off error.
  * It can be bad enough to be 4 pixels off for a 500 x 300 bitmap.
  * Another work-around is to invert the scaling factor in a memory DC, and blit
  * the results in the destination DC (with scale 1).
  * This works on Windows, but pose problems on wxGTK
  * \param dc : the wxDC for which the option must be set
  */
void wxDragImageExt::SetDCOptions(wxDC &dc) const
{
#if defined(__WXMSW__)
#ifndef __WXWINCE__

    //the error comes originally from VIEWPORT_EXTENT constant, defined if src/msw/dc.cpp at line 85
    //the original value of 1000 is too low. 10000 is much better

    int iViewPortExtent, width, height;
    int iDeviceOriginX, iDeviceOriginY, iLogicalOriginX, iLogicalOriginY;
    int iSignX, iSignY;

    iViewPortExtent = 10000;

    //dc.SetAxisOrientation(true, false);
    iSignX = 1;
    iSignY = 1;

    dc.GetDeviceOrigin(&iDeviceOriginX, &iDeviceOriginY);
    dc.GetLogicalOrigin(&iLogicalOriginX, &iLogicalOriginY);


    width  = dc.DeviceToLogicalXRel(iViewPortExtent) * iSignX,
    height = dc.DeviceToLogicalYRel(iViewPortExtent) * iSignY;

    HDC hdc;
    hdc = (HDC) dc.GetHDC();

    //this is the code that allows to work around the bug (redefinition of the reference frame
    //with a larger size. The round off errors are minimized
    ::SetMapMode(hdc, MM_ANISOTROPIC);
    ::SetViewportExtEx(hdc, iViewPortExtent, iViewPortExtent, NULL);
    ::SetWindowExtEx(hdc, width, height, NULL);
    ::SetViewportOrgEx(hdc, iDeviceOriginX, iDeviceOriginY, NULL);
    ::SetWindowOrgEx(hdc, iLogicalOriginX, iLogicalOriginY, NULL);

#endif
#endif
}

/** This is a wrapper around dc.SetUserScale()
  * This method is used to work around a wxWidgets bug in MSW port
  * The bug occurs when scaling a DC (wxDC::SetUserScale).
  * There is a cumulative round-off error.
  * It can be bad enough to be 4 pixels off for a 500 x 300 bitmap.
  * Another work-around is to invert the scaling factor in a memory DC, and blit
  * the results in the destination DC (with scale 1).
  * This works on Windows, but pose problems on wxGTK
  * \param dc : the wxDC for which the option must be set
  * \param dScale_X : the scaling factor in the X direction
  * \param dScale_Y : the scaling factor in the Y direction
  */
void wxDragImageExt::SetUserScale(wxDC &dc, double dScale_X, double dScale_Y) const
{
    dc.SetUserScale(dScale_X, dScale_Y);
#if defined(__WXMSW__)
#ifndef __WXWINCE__
    SetDCOptions(dc);
#endif
#endif
}

/** This is a wrapper around dc.Clear()
 * This method is used to work around a wxWidgets bug in MSW port
  * The bug occurs when scaling a DC (wxDC::SetUserScale).
  * There is a cumulative round-off error.
  * It can be bad enough to be 4 pixels off for a 500 x 300 bitmap.
  * Another work-around is to invert the scaling factor in a memory DC, and blit
  * the results in the destination DC (with scale 1).
  * This works on Windows, but pose problems on wxGTK
  * \param dc : the wxDC for which the option must be set
  */
void wxDragImageExt::ClearDC(wxDC &dc) const
{
    dc.Clear();
#if defined(__WXMSW__)
#ifndef __WXWINCE__
    SetDCOptions(dc);
#endif
#endif
}

/** shortcut to CB Logging methods
  * \param sLogText: the text to display in the "Code::Blocks" log tab
  */
void wxDragImageExt::Log(wxString sLogText) const
{
    Manager::Get()->GetLogManager()->Log(sLogText);
}

/** debug method - save the bitmap to a file
  * \param bmp : the bitmap to save
  * \param sPath : the full path where to save the bitmap
  */
void wxDragImageExt::Save(wxBitmap bmp, wxString sPath) const
{
#if _WX_DEBUG__WX_DRAG_IMAGE_EXT_ == 1
    wxString sFullPath;
    sFullPath = _("C:\\debug\\");
    sFullPath += sPath;
    bmp.SaveFile(sFullPath, wxBITMAP_TYPE_BMP);
#endif
}

/** debug method - save blit coordinates to a text file
  * \param xDest : the Destination device context x position
  * \param yDest : the Destination device context y position
  * \param width : Width of source area to be copied
  * \param height: height of source area to be copied
  * \param xSrc  : Source device context x position.
  * \param ySrc  : Source device context y position.
  */
void wxDragImageExt::LogToFile(int xDest, int yDest, int width, int height, int xSrc, int ySrc, wxString sPath) const
{
#if _WX_DEBUG__WX_DRAG_IMAGE_EXT_ == 1
    wxString sFullPath;
    wxString sLine;
    wxTextFile tf;

    sFullPath = _("C:\\debug\\");
    sFullPath += sPath;

    sLine = wxString::Format(_("xDest=%d yDest=%d width=%d height=%d xSrc=%d ySrc=%d"), xDest, yDest, width, height, xSrc, ySrc);

    if (!tf.Create(sFullPath))
    {
        if (!tf.Open(sFullPath)) return;
    }
    if (!tf.IsOpened()) return;

    tf.Clear();
    tf.AddLine(sLine);
    tf.Write();
#endif
}
