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
 : wxGenericDragImage(image, cursor)
{
    m_dScale = 1;
}

/** Destructor
  */
wxDragImageExt::~wxDragImageExt(void)
{

}

/** Set the destination scaling factor
  * \param dScale: the new scaling factor.
  *                nothing is done if dScale <= 0
  */
void wxDragImageExt::SetScale(double dScale)
{
    if (dScale > 0) m_dScale = dScale;
}

// Override this if you are using a virtual image (drawing your own image)
bool wxDragImageExt::DoDrawImage(wxDC& dc, const wxPoint& pos) const
{
    double dOldScaleX, dOldScaleY;
    dc.GetUserScale(&dOldScaleX, &dOldScaleY);

    Manager::Get()->GetLogManager()->Log(_("DoDrawImage"));

    if (m_bitmap.Ok())
    {
        Manager::Get()->GetLogManager()->Log(_("m_bitmap OK"));
        //m_position
        //m_window

        wxBitmap bmp(m_bitmap);
        wxMemoryDC memDC(bmp);
        memDC.SetUserScale(1, 1 );

        dc.SetUserScale(m_dScale, m_dScale);

        dc.Blit(pos.x, pos.y, bmp.GetWidth(), bmp.GetHeight(), &memDC, 0, 0);
        dc.SetUserScale(dOldScaleX, dOldScaleY);

        //dc.DrawBitmap(m_bitmap, x, y, (m_bitmap.GetMask() != 0));

        return true;
    }
    else if (m_icon.Ok())
    {
        dc.SetUserScale(m_dScale, m_dScale);
        dc.DrawIcon(m_icon, pos.x, pos.y);
        dc.SetUserScale(dOldScaleX, dOldScaleY);
        return true;
    }
    else
        return false;
}

// Override this if you are using a virtual image (drawing your own image)
wxRect wxDragImageExt::GetImageRect(const wxPoint& pos) const
{
    wxRect rResult(0,0,0,0);
    if (m_bitmap.Ok())
    {
        rResult = wxRect(pos.x, pos.y, m_bitmap.GetWidth() * m_dScale, m_bitmap.GetHeight() * m_dScale);
    }
    else if (m_icon.Ok())
    {
        rResult =  wxRect(pos.x, pos.y, m_icon.GetWidth() * m_dScale, m_icon.GetHeight() * m_dScale);
    }
    else
    {
        rResult =  wxRect(pos.x, pos.y, 0, 0);
    }

    if (m_window)
    {
        //no need to have a rectangle bigger than the client size of the window
        wxSize sClientSize = m_window->GetClientSize();
        if (sClientSize.GetWidth() > rResult.GetWidth()) rResult.SetWidth(sClientSize.GetWidth());
        if (sClientSize.GetHeight() > rResult.GetHeight()) rResult.SetHeight(sClientSize.GetHeight());
    }

    return(rResult);
}

