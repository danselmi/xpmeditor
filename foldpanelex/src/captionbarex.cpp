/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      captionbarex.cpp
 * Purpose:   wrapper around wxCode captionbar - code
 *            This wrapper allows the alternative use of a flat button
 *            for portability purpose
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-07-04
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

//#if USE_CAPTIONBAREX

#include "wx/foldpanelex/captionbarex.h"
#include "icon_collapsed.xpm"
#include "icon_expanded.xpm"

DEFINE_EVENT_TYPE(wxEVT_CAPTIONBAREX)

/** Constructor
  * \param parent : a pointer to the parent window
  * \param id : the identifier of the new widgets.
  * \param caption : the caption of the bar
  * \param StyleCaption: the type of caption.
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  * \param images: the image list used for the folding icons drawing. Can be NULL
  *                ignored for wxButton caption
  * \param bUseImages: indicates the behaviour to have in case images is NULL.
  *                    if bUseImages = true and images = NULL then a default image list is used
  *                    if bUseImages = false and images = NULL then no images are used
  *                    if images != NULL then this flag is ignored
  * \param pos : the initial position of the widget
  * \param size: the initial size of the widget
  * \param style: the style of the widgets. See doc for wxCaptionBar and wxButton for more information
  * \param validator : the validator for the caption bar
  * \param name : the name of the widget
  */
wxCaptionBarEx::wxCaptionBarEx(wxWindow *parent,
                               wxWindowID id,
                               const wxString &caption,
                               long StyleCaption,
                               wxImageList *images,
                               bool bUseImages,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               const wxValidator& validator,
                               const wxString& name
                              )
    :wxWindow(parent, id, pos, size, style, name)
{
    //initialisation
    m_bCollapsed = true;
    cbar = NULL;
    button = NULL;
    m_CaptionStyle = StyleCaption;
    m_bVertical = true;
    m_captionbarstyle = wxNO_BORDER;
    m_buttonstyle = wxBU_LEFT | wxNO_BORDER;

    //create a default image list
    // create the fold icons to be used in the captions
    m_images = new wxImageList(16, 16);

    wxBitmap *bmp = new wxBitmap(icon_expanded);
    if ((m_images) && (bmp)) m_images->Add(*bmp);
    delete(bmp);

    bmp = new wxBitmap(icon_collapsed);
    if ((m_images) && (bmp)) m_images->Add(*bmp);
    delete(bmp);

    if (m_CaptionStyle == WX_CAPTION_BUTTON)
    {
        CreateButtonBar(this, id, caption, pos, size, style, validator);
    }
    else if (m_CaptionStyle == WX_CAPTION_FLATBUTTON)
    {
        CreateFlatButtonBar(this, id, caption, pos, size, style, validator);
    }
    else // (m_CaptionStyle == WX_CAPTION_NORMAL)
    {
        wxImageList *pList;

        pList = NULL;
        if (images)
        {
            pList = images;
        }
        else
        {
            if (bUseImages) pList = m_images;
        }

        CreateCaptionBar(this, id, caption, pList, pos, size, style);
    }

    ComputeDimensions();

    Connect(wxEVT_SIZE, (wxObjectEventFunction)&wxCaptionBarEx::OnSizeEvent);

}

/** Destructor
  */
wxCaptionBarEx::~wxCaptionBarEx(void)
{
    if (m_images) delete(m_images);
}

/** create the caption bar
  * \param parent : a pointer to the parent window
  * \param id : the identifier of the new widgets.
  * \param caption : the caption of the bar
  * \param images: the image list used for the folding icons drawing. Can be NULL
  *                ignored for wxButton caption
  * \param pos : the initial position of the widget
  * \param size: the initial size of the widget
  * \param style: the style of the widgets. See doc for wxCaptionBar for more information
  * \param validator : the validator for the caption bar
  * \param name : the name of the widget
  */
void wxCaptionBarEx::CreateCaptionBar(wxWindow *parent,
                                      wxWindowID id,
                                      const wxString &caption,
                                      wxImageList *images,
                                      const wxPoint& pos,
                                      const wxSize& size,
                                      long style
                                     )
{
    DestroyCaptionBar();
    cbar = new wxCaptionBar(parent,
                            caption,
                            images,
                            id,
                            wxEmptyCaptionBarStyle,
                            pos,
                            size,
                            style
                           );
    idBar = id;

    m_captionbarstyle = style;

    if (cbar)
    {
        Connect(id, wxEVT_CAPTIONBAR, (wxObjectEventFunction)&wxCaptionBarEx::OnCaptionBarClicked);
    }
}

/** destroy the caption bar if it already exists
  */
void wxCaptionBarEx::DestroyCaptionBar(void)
{
    if (cbar)
    {
        DisconnectEvents();
        cbar->Hide();
        cbar->Reparent(NULL);
        cbar->Destroy();
        cbar = NULL;
    }
}

/** remove events handlers before destruction
  */
void wxCaptionBarEx::DisconnectEvents(void)
{
    if (cbar)
    {
        Disconnect(idBar, wxEVT_CAPTIONBAR, (wxObjectEventFunction)&wxCaptionBarEx::OnCaptionBarClicked);
    }
    if (button)
    {
        Disconnect(ibButton, wxEVT_COMMAND_BUTTON_CLICKED , (wxObjectEventFunction)&wxCaptionBarEx::OnButtonClicked);
    }
}

/** create the caption bar
  * \param parent : a pointer to the parent window
  * \param id : the identifier of the new widgets.
  * \param caption : the caption of the bar
  * \param images: the image list used for the folding icons drawing. Can be NULL
  *                ignored for wxButton caption
  * \param pos : the initial position of the widget
  * \param size: the initial size of the widget
  * \param style: the style of the widgets. See doc for wxCaptionBar for more information
  * \param validator : the validator for the caption bar
  * \param name : the name of the widget
  */
void wxCaptionBarEx::CreateButtonBar(wxWindow *parent,
                                     wxWindowID id,
                                     const wxString &caption,
                                     const wxPoint& pos,
                                     const wxSize& size,
                                     long style,
                                     const wxValidator& validator
                                    )
{
    long lStyle;

    DestroyButtonBar();

    wxString sCaption;
    if (m_bCollapsed) sCaption = _("+ "); else sCaption = _("- ");
    sCaption = sCaption + caption;

    lStyle = style | wxBU_LEFT ;
    m_buttonstyle = lStyle;
    button = new wxButton(parent, id, sCaption, pos, size, lStyle, validator);
    ibButton = id;

    if (button)
    {
        Connect(id, wxEVT_COMMAND_BUTTON_CLICKED , (wxObjectEventFunction)&wxCaptionBarEx::OnButtonClicked);
    }
}

/** destroy the button bar if it already exists
  */
void wxCaptionBarEx::DestroyButtonBar(void)
{
    if (button)
    {
        DisconnectEvents();
        button->Hide();
        button->Reparent(NULL);
        button->Destroy();
        button = NULL;
    }
}

void wxCaptionBarEx::CreateFlatButtonBar(wxWindow *parent,
                                         wxWindowID id,
                                         const wxString &caption,
                                         const wxPoint& pos,
                                         const wxSize& size,
                                         long style,
                                         const wxValidator& validator
                                        )
{
    CreateButtonBar(parent, id, caption, pos, size, style | wxNO_BORDER, validator);
}

//properties accessors
/** get the caption style: normal, button or flat button
  * \return one of the following value:
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  */
long wxCaptionBarEx::GetCaptionStyle(void)
{
    return(m_CaptionStyle);
}

/** Set the caption style: normal, button or flat button
  * The old control will be hidden
  * \param one of the following value:
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  * \param lStyle : the window style to use for the new caption bar.
  *                 see wxButton or wxCaptionBar for more information
  */
void wxCaptionBarEx::SetCaptionStyle(long lNewStyle, long lStyle)
{
    if (lNewStyle == m_CaptionStyle) return;

    wxSize size;
    wxPoint pos;
    wxString caption;
    wxWindow *parent;

    //destroy the old window first
    parent = NULL;
    if ((m_CaptionStyle == WX_CAPTION_NORMAL) && (cbar))
    {
        //get position and size, then destroy it
        size = cbar->GetSize();
        pos = cbar->GetPosition();
        parent = cbar->GetParent();
        caption = cbar->GetCaption();

        if (parent) parent->Freeze();
        DestroyCaptionBar();
    }
    else if (button)
    {
        //get position and size, then destroy it
        size = button->GetSize();
        pos = button->GetPosition();
        parent = button->GetParent();
        caption = button->GetLabel();

        if (parent) parent->Freeze();
        DestroyButtonBar();
    }

    m_CaptionStyle = lNewStyle;
    if (m_CaptionStyle == WX_CAPTION_BUTTON)
    {
        CreateButtonBar(this, wxNewId(), caption, pos, size, lStyle);
    }
    else if (m_CaptionStyle == WX_CAPTION_FLATBUTTON)
    {
        CreateFlatButtonBar(this, wxNewId(), caption, pos, size, lStyle);
    }
    else // (m_CaptionStyle == WX_CAPTION_NORMAL)
    {
        CreateCaptionBar(this, wxNewId(), caption, m_images, pos, size, lStyle);
    }

    if (parent) parent->Thaw();
    ComputeDimensions();
}

/** get a pointer to the normal caption widget, or NULL if not applicable
  * \return : a pointer to the normal caption widget, or NULL if not applicable
  */
wxCaptionBar* wxCaptionBarEx::GetCaptionBar(void)
{
    return(cbar);
}

/** get a pointer to the button caption widget, or NULL if not applicable
  * \return : a pointer to the button caption widget, or NULL if not applicable
  */
wxButton* wxCaptionBarEx::GetButton(void)
{
    return(button);
}


/** send a wxEVT_CAPTIONBAREX event
  * \param bCollapsed : true if the caption bar has been collapsed
  *                     false if the caption bar has been expanded
  */
void wxCaptionBarEx::SendEvent(bool bCollapsed)
{
    wxCommandEvent event(wxEVT_CAPTIONBAREX, GetId());
    event.SetEventObject(this);

    if (bCollapsed) event.SetInt(0); else event.SetInt(1);

    // Send the event
    GetEventHandler()->ProcessEvent(event);

}

/** wxCaptionBar handler
  */
void wxCaptionBarEx::OnCaptionBarClicked(wxCommandEvent &event)
{
    if (event.GetInt() == 0)
    {
        m_bCollapsed = true;
        if (cbar) cbar->Collapse();
        SendEvent(true);
    }
    else
    {
        m_bCollapsed = false;
        if (cbar) cbar->Expand();
        SendEvent(false);
    }

}

/** wxButton clicked handler
  */
void wxCaptionBarEx::OnButtonClicked(wxCommandEvent &event)
{
    wxString sCaption;
    wxString sCaption2;

    if (button)
    {
        sCaption = button->GetLabel();
        sCaption2 = sCaption.Right(sCaption.Len() - 2);

        if (sCaption.Left(1) == _("+"))
        {
            m_bCollapsed = false;
            SendEvent(false);
            sCaption = _("- ");
            sCaption = sCaption + sCaption2;
        }
        else
        {
            m_bCollapsed = true;
            SendEvent(true);
            sCaption = _("+ ");
            sCaption = sCaption + sCaption2;
        }
        button->SetLabel(sCaption);
    }
}

/** size event handler
  */
void wxCaptionBarEx::OnSizeEvent(wxSizeEvent &event)
{
    wxSize size;
    size = event.GetSize();

    if (cbar)
    {
        cbar->SetSize(0,0,size.GetWidth(), size.GetHeight());
    }
    else if (button)
    {
        button->SetSize(0,0,size.GetWidth(), size.GetHeight());
    }

    event.Skip();
}

/** Compute minimal and maximal dimensions
  * It will call SetMinSize and SetMaxSize
  */
void wxCaptionBarEx::ComputeDimensions(void)
{
    wxSize sMaxSize(0,0);
    wxSize sMinSize(0,0);

    if (cbar)
    {
        int x, y, h, w;
        wxFont font;
        wxString sCaption;
        font = cbar->GetFont();
        sCaption = GetCaption();

        if (m_bVertical)
        {
            GetTextExtent(sCaption, &x, &y);
            cbar->GetIconSize(&h, &w);

            if (h < y) h = y;
            sMinSize.SetWidth(x + 2*w);
            sMinSize.SetHeight(h);

            sMaxSize.SetWidth(x + WX_CAPTION_MAX_WIDTH + w);
            sMaxSize.SetHeight(WX_CAPTION_MAX_HEIGHT);
        }
        else
        {
            GetTextExtent(sCaption, &y, &x);
            cbar->GetIconSize(&h, &w);

            if (w < x) w = x;
            sMinSize.SetWidth(w);
            sMinSize.SetHeight(y + 2*h);

            sMaxSize.SetWidth(WX_CAPTION_MAX_WIDTH);
            sMaxSize.SetHeight(y + WX_CAPTION_MAX_HEIGHT + h);
        }



    }
    else if (button)
    {
        sMinSize = button->GetBestSize();
        sMaxSize = button->GetMaxSize();
    }

    if (sMaxSize.GetWidth() < sMinSize.GetWidth()) sMaxSize.SetWidth(sMinSize.GetWidth());
    if (sMaxSize.GetHeight() < sMinSize.GetHeight()) sMaxSize.SetHeight(sMinSize.GetHeight());
    SetMinSize(sMinSize);
    SetMaxSize(sMaxSize);
}

/** Return the ideal size for the control
  * \return the ideal size
  */
wxSize wxCaptionBarEx::DoGetBestSize(void) const
{
    if (cbar)
    {
        return(cbar->DoGetBestSize());
    }
    else if (button)
    {
        return(button->GetBestSize());
    }

    return(wxSize(0,0));
}

/** Get the caption of the bar / button
  * \return the caption text
  */
wxString wxCaptionBarEx::GetCaption(void)
{
    if (cbar)
    {
        return(cbar->GetCaption());
    }
    else if (button)
    {
        wxString sCaption;
        sCaption = button->GetLabel();
        return(sCaption.Right(sCaption.Len() - 2));
    }

    return(_(""));
}

/** Set the caption of the bar / button
  * \param sNewCaption:  the caption text
  */
void wxCaptionBarEx::SetCaption(wxString sNewCaption)
{
    if (cbar)
    {
        cbar->SetCaption(sNewCaption);
    }
    else if (button)
    {
        button->SetLabel(sNewCaption);
    }

    Refresh(true, NULL);
    Update();
    ComputeDimensions();
}

/** expand the caption bar
  * If the caption bar is already expanded, nothing is done
  * and no events are sent, regardless of the value of bSendEvent
  * \param bSendEvent : if true, the event wxEVT_CAPTIONBAREX is sent
  */
void wxCaptionBarEx::Expand(bool bSendEvent)
{
    if (!m_bCollapsed) return; //does nothing
    if (cbar)
    {
        cbar->Expand();

    }
    else if (button)
    {
        wxString sCaption;
        wxString sCaption2;

        sCaption = button->GetLabel();
        sCaption2 = sCaption.Right(sCaption.Len() - 2);
        sCaption = _("- ");
        sCaption = sCaption + sCaption2;
        button->SetLabel(sCaption);
    }
    m_bCollapsed = false;
    if (bSendEvent) SendEvent(false);
}

/** collapse the caption bar
  * If the caption bar is already expanded, nothing is done
  * and no events are sent, regardless of the value of bSendEvent
  * \param bSendEvent : if true, the event wxEVT_CAPTIONBAREX is sent
  */
void wxCaptionBarEx::Collapse(bool bSendEvent)
{
    if (m_bCollapsed) return; //does nothing
    if (cbar)
    {
        cbar->Collapse();
    }
    else if (button)
    {
        wxString sCaption;
        wxString sCaption2;

        sCaption = button->GetLabel();
        sCaption2 = sCaption.Right(sCaption.Len() - 2);
        sCaption = _("+ ");
        sCaption = sCaption + sCaption2;
        button->SetLabel(sCaption);
    }
    m_bCollapsed = true;
    if (bSendEvent) SendEvent(true);
}

/** return true if the caption bar is expanded
  * \return true if the caption bar is expanded
  */
bool wxCaptionBarEx::IsExpanded(void)
{
    return(!m_bCollapsed);
}

/** return true if the caption bar is collapsed
  * \return true if the caption bar is collapsed
  */
bool wxCaptionBarEx::IsCollapsed(void)
{
    return(m_bCollapsed);
}

/** Set the folding direction vertical
  */
void wxCaptionBarEx::SetVertical(void)
{
    if (!m_bVertical)
    {
        m_bVertical = true;

        if (m_CaptionStyle == WX_CAPTION_NORMAL)
        {
            if (cbar) cbar->SetVertical(true);
        }
        else
        {
            //destroy the current wxCaptionBar and create the wxButton (flat or not) instead
            //get position and size, then destroy it
            wxWindow *parent;
            wxSize size;
            wxPoint pos;
            wxString caption;

            if (cbar)
            {
                size = cbar->GetSize();
                pos = cbar->GetPosition();
                parent = cbar->GetParent();
                caption = cbar->GetCaption();
            }

            if (parent) parent->Freeze();
            DestroyCaptionBar();

            if (m_CaptionStyle == WX_CAPTION_BUTTON)
            {
                CreateButtonBar(this, wxNewId(), caption, pos, size, m_buttonstyle);
            }
            else if (m_CaptionStyle == WX_CAPTION_FLATBUTTON)
            {
                CreateFlatButtonBar(this, wxNewId(), caption, pos, size, m_buttonstyle);
            }

            if (parent) parent->Thaw();
        }
    }

    ComputeDimensions();
}

/** Set the folding direction horizontal
  */
void wxCaptionBarEx::SetHorizontal(void)
{
    if (m_bVertical)
    {
        m_bVertical = false;

        if (m_CaptionStyle == WX_CAPTION_NORMAL)
        {
            if (cbar) cbar->SetVertical(false);
        }
        else
        {
            //destroy the current wxButton and create a wxCaptionBar instead
            //get position and size, then destroy it
            wxWindow *parent;
            wxSize size;
            wxPoint pos;
            wxString caption;

            if (button)
            {
                size = button->GetSize();
                pos = button->GetPosition();
                parent = button->GetParent();
                caption = button->GetLabel();
            }

            if (parent) parent->Freeze();
            DestroyButtonBar();

            CreateCaptionBar(this, wxNewId(), caption, m_images, pos, size, m_captionbarstyle);
            if (cbar) cbar->SetVertical(false);

            if (parent) parent->Thaw();
        }
    }

    ComputeDimensions();
}

/** Return true if the folding direction is vertical, false if horizontal
  * \return true if the folding direction is vertical, false if horizontal
  */
bool wxCaptionBarEx::IsVertical(void)
{
    return(m_bVertical);
}

/** Return true if the folding direction is horizontal, false if vertical
  * \return true if the folding direction is horizontal, false if vertical
  */
bool wxCaptionBarEx::IsHorizontal(void)
{
    return(!m_bVertical);
}
//#endif //#if USE_CAPTIONBAREX
