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

class wxDragImageExt: public wxGenericDragImage
{
    public:
        wxDragImageExt(const wxBitmap& image = wxNullBitmap, const wxCursor& cursor = wxNullCursor); ///< \brief constructor
        virtual ~wxDragImageExt(void); ///< \brief destructor

        // Override this if you are using a virtual image (drawing your own image)
        virtual bool DoDrawImage(wxDC& dc, const wxPoint& pos) const;
        virtual wxRect GetImageRect(const wxPoint& pos) const;
        void SetScale(double dScale);  ///< \brief Set the destination scaling factor

    private:
        double m_dScale;

        DECLARE_DYNAMIC_CLASS(wxDragImageExt)
        DECLARE_NO_COPY_CLASS(wxDragImageExt)
};

#endif //#ifndef _WX_DRAG_IMAGE_EXT_H_
