/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxDragImageExt.h
 * Purpose:   class for generic drag image, with as few flicker as possible - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef _WX_DRAG_IMAGE_EXT_H_
#define _WX_DRAG_IMAGE_EXT_H_

#include <wx/dc.h>
#include <wx/dcmemory.h>
#include <wx/generic/dragimgg.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/cursor.h>
#include <wx/log.h>

/** \class the generic drag image class from wxWidgets is reimplemented, because there are too many innacuracies in the drawing of the image
 * moreover, wxGenericDragImage is extremely inefficient for large zoom factor, because it leads to large bitmap which are not clipped
 * to the client area.
 * Full Screen is not supported, because it is useless in this configuration
 */

class wxDragImageExt: public wxObject
{
    public:
        wxDragImageExt(const wxBitmap& image = wxNullBitmap, const wxCursor& cursor = wxNullCursor); ///< \brief constructor
        virtual ~wxDragImageExt(void); ///< \brief destructor

        //wxGenericDragImage API. Only BeginDrag is modified (no fullscreen flag)
        virtual bool DoDrawImage(wxDC& dc, const wxPoint& pos) const;   ///< \brief Draw the dragged bitmap, as if it was a cursor
        virtual wxRect GetImageRect(const wxPoint& pos) const;          ///< \brief Compute the wxRect enclosing the dragged bitmap, as if it was a cursor

        virtual bool UpdateBackingFromWindow(wxDC& windowDC, wxMemoryDC& destDC,
                                             const wxRect& sourceRect,
                                             const wxRect& destRect) const; ///< \brief draw the window contents to the backing bitmap

        virtual bool RedrawImage(const wxPoint& oldPos, const wxPoint& newPos,
                                 bool eraseOld, bool drawNew);              ///< \brief Erase and redraw simultaneously if possible

        bool BeginDrag(const wxPoint& hotspot, wxWindow* window, wxRect* rect = (wxRect*) NULL); ///< \brief Begin Drag
        bool EndDrag(void);           ///< \brief End Drag
        bool Move(const wxPoint& pt); /// < \brief Move the image
        bool Show(void);              ///< \brief Show the image
        bool Hide(void);              ///< \brief Hide the image

        void SetScale(double dScale);                       ///< \brief Set the destination scaling factor
        wxRect ComputeBitmapRectangle(wxPoint& pos) const;  ///< \brief Clip the enclosing wxRect of the dragged bitmap to the client area of the destination window

    private:
        double          m_dScale;        ///< \brief the scaling factor of the target window
        wxBitmap        m_bitmap;        ///< \brief the dragged bitmap
        wxCursor        m_cursor;        ///< \brief the mouse cursor used while dragging the bitmap
        wxCursor        m_oldCursor;     ///< \brief a backup of the cursor before dragging
        wxPoint         m_offset;        ///< \brief The hostpot value passed to BeginDrag
        wxPoint         m_position;      ///< \brief the current dragging position
        bool            m_isDirty;       ///< \brief true if the window must be redrawn
        bool            m_isShown;       ///< \brief true if the dragged bitmap is currently shown
        wxWindow*       m_window;        ///< \brief the target window
        wxDC*           m_windowDC;      ///< \brief a wxDeviceContext for the target window
        wxBitmap        m_backingBitmap; ///< \brief the original window drawing (before dragging)
        wxBitmap        m_repairBitmap;  ///< \brief A temporary bitmap for repairing/redrawing
        wxRect          m_boundingRect;  ///< \brief the bounding rectangle of the window

        void ClipBitmap(int *x, int *y, int *width, int *height) const; ///< \brief Clip the bitmap size to the visible client area of the target window

        DECLARE_DYNAMIC_CLASS(wxDragImageExt)
        DECLARE_NO_COPY_CLASS(wxDragImageExt)
};

#endif //#ifndef _WX_DRAG_IMAGE_EXT_H_
