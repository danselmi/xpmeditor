/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMToggleBMPButton.cpp
 * Purpose:   a bitmap toggle button - not available in wxWidgets 2.8.10 - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/tglbtn.cpp
// Purpose:     Definition of the wxToggleButton class, which implements a
//              toggle button under wxMSW.
// Author:      John Norris, minor changes by Axel Schlueter
//              and William Gallafent.
// Modified by:
// Created:     08.02.01
// RCS-ID:      $Id: tglbtn.cpp 58718 2009-02-07 18:59:25Z VZ $
// Copyright:   (c) 2000 Johnny C. Norris II
// License:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/brush.h>
    #include <wx/dcscreen.h>
    #include <wx/settings.h>

    #include <wx/log.h>
#endif // WX_PRECOMP

#include <wx/wx.h>
#include <wx/renderer.h>
#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/image.h>

#include "XPMToggleBMPButton.h"


//-----------------------------------------------------------------------------
// XPMToggleBMPButton
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(XPMToggleBMPButton, wxToggleButton)

BEGIN_EVENT_TABLE(XPMToggleBMPButton,wxToggleButton)
   EVT_PAINT(XPMToggleBMPButton::OnPaint)
   EVT_MOUSE_EVENTS(XPMToggleBMPButton::OnMouse)
   EVT_CHAR(XPMToggleBMPButton::OnChar)
   EVT_SIZE(XPMToggleBMPButton::OnSize)
END_EVENT_TABLE()

XPMToggleBMPButton::XPMToggleBMPButton(void)
{
    Init();
}

XPMToggleBMPButton::XPMToggleBMPButton(wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap& label,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   const wxValidator& validator,
                   const wxString& name)
{
    Create(parent, id, label, pos, size, style, validator, name);
}

void XPMToggleBMPButton::Init()
{
    m_depressed = false;
    m_oldValue = false;
    m_capturing = false;
}

bool XPMToggleBMPButton::Create( wxWindow *parent, wxWindowID id,
                const wxBitmap& label,const wxPoint& pos, const wxSize& size, long style,
                const wxValidator& validator, const wxString& name )
{
    Init();

    if (!wxToggleButton::Create( parent, id, _(""), pos, size, style, validator, name ))
        return false;

    m_bitmap = label;

    if (size.x == -1 || size.y == -1)
    {
        wxSize new_size = GetBestSize();
        if (size.x != -1)
            new_size.x = size.x;
        if (size.y != -1)
            new_size.y = size.y;
        SetSize( new_size );
    }

    return true;
}

void XPMToggleBMPButton::SetValue(bool state)
{
    if (m_capturing) return;

    if (state == m_depressed) return;

    m_depressed = state;
    Refresh();
}

bool XPMToggleBMPButton::GetValue() const
{
    return m_depressed;
}

void XPMToggleBMPButton::SetLabel(const wxString& label)
{
    wxControl::SetLabel(label);
}

void XPMToggleBMPButton::SetLabel(const wxBitmap& label)
{
    m_bitmap = label;
    m_disabledBitmap = wxBitmap();

    Refresh();
}

bool XPMToggleBMPButton::Enable(bool enable)
{
    if (m_capturing) return false;

    if (!wxToggleButton::Enable( enable ))
      return false;

    Refresh();

    return true;
}

void XPMToggleBMPButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxSize size = GetSize();

    wxBitmap bitmap = m_bitmap;

    wxPaintDC dc(this);

    wxRendererNative &renderer = wxRendererNative::Get();
    int flags = 0;
    if (m_depressed) flags |= wxCONTROL_PRESSED;
    wxRect rect;
    rect.SetX(0);
    rect.SetY(0);
    rect.SetWidth(size.GetWidth());
    rect.SetHeight(size.GetHeight());
    dc.Clear();
    renderer.DrawPushButton( this, dc, rect, flags );

    if (bitmap.IsOk())
    {
        if (!IsEnabled())
        {
            if (!m_disabledBitmap.IsOk())
            {
                wxImage image = m_bitmap.ConvertToImage();
                m_disabledBitmap = wxBitmap( image.ConvertToGreyscale() );
            }

            bitmap = m_disabledBitmap;
        }

        int iHeight, iWidth;
        iHeight = bitmap.GetHeight();
        iWidth = bitmap.GetWidth();

        wxSize bsize(iWidth, iHeight);
        int offset;
        if (m_depressed) offset = 1; else offset = 0;
        dc.DrawBitmap( bitmap, (size.GetWidth() - iWidth)/2 + offset, (size.GetHeight() - iHeight)/2 + offset, true );
    }
}

void XPMToggleBMPButton::OnMouse(wxMouseEvent &event)
{
    if (!IsEnabled())
        return;

    wxSize size = GetSize();
    bool mouse_in = ((event.GetX() > 0) && (event.GetX() < size.x) &&
                     (event.GetY() > 0) && (event.GetY() < size.y));

    if (m_capturing)
    {
        bool old_depressed = m_depressed;
        if (mouse_in)
            m_depressed = !m_oldValue;
        else
            m_depressed = m_oldValue;

        if (event.LeftUp())
        {
            ReleaseCapture();
            m_capturing = false;
            if (mouse_in)
            {
                wxCommandEvent event(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, m_windowId);
                event.SetInt(GetValue());
                event.SetEventObject(this);
                ProcessCommand(event);
            }
        }

        if (old_depressed != m_depressed)
           Refresh();
    }
    else
    {
        if (event.LeftDown())
        {
            m_capturing = true;
            m_oldValue = m_depressed;
            m_depressed = !m_oldValue;
            CaptureMouse();
            Refresh();
        }
    }
}

void XPMToggleBMPButton::OnChar(wxKeyEvent &event)
{
   if (event.GetKeyCode() == WXK_SPACE)
   {
       m_depressed = !m_depressed;
       Refresh();

       wxCommandEvent event(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, m_windowId);
       event.SetInt(GetValue());
       event.SetEventObject(this);
       ProcessCommand(event);
   }
}

void XPMToggleBMPButton::OnSize(wxSizeEvent &WXUNUSED(event))
{
    Refresh(false);
}

wxSize XPMToggleBMPButton::DoGetBestSize() const
{
    if (!m_bitmap.IsOk())
        return wxSize(16,16);

    int iHeight, iWidth;

    iHeight = m_bitmap.GetHeight();
    iWidth = m_bitmap.GetWidth();

    wxSize ret(iWidth + 8, iHeight + 8);
    return ret;
}



