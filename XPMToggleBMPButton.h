/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMToggleBMPButton.h
 * Purpose:   a bitmap toggle button - not available in wxWidgets 2.8.10 - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef _XPM_BMP_TOGGLEBUTTON_H_
#define _XPM_BMP_TOGGLEBUTTON_H_

#include <wx/bitmap.h>
#include <wx/tglbtn.h>

//-----------------------------------------------------------------------------
// wxBitmapToggleButton
//-----------------------------------------------------------------------------


class XPMToggleBMPButton: public wxToggleButton
{
public:
    // construction/destruction
    XPMToggleBMPButton(void);

    XPMToggleBMPButton(wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = _("bmp_toggle_button"));

    // Create the control
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxBitmap& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = _("bmp_toggle_button"));

    // Get/set the value
    void SetValue(bool state);
    bool GetValue() const;

    // Set the label
    virtual void SetLabel(const wxString& label);
    virtual void SetLabel(const wxBitmap& label);
    bool Enable(bool enable = TRUE);

protected:
    void Init();

    wxBitmap  m_bitmap;
    wxBitmap  m_disabledBitmap;
    bool      m_capturing;
    bool      m_depressed,m_oldValue;

    void OnPaint(wxPaintEvent &event);
    void OnMouse(wxMouseEvent &event);
    void OnChar(wxKeyEvent &event);
    void OnSize(wxSizeEvent &event);

    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(XPMToggleBMPButton)
};

#endif
