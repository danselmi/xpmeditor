/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      folditemex.cpp
 * Purpose:   a composite control containing a wxCaptionBarEx and a wxWindow
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-07-04
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "wx/foldpanelex/folditemex.h"

DEFINE_EVENT_TYPE(wxEVT_FOLDITEMEX)

/** Constructor
  * \param parent : a pointer to the parent window
  * \param sCaption : the caption of the bar
  * \param child    : a pointer to the child (foldable) window
  *                   this window must have been created before. It will be reparented
  * \param StyleCaption: the type of caption.
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  * \param pIconsList: the image list used for the folding icons drawing. Can be NULL
  *                    ignored for wxButton caption
  * \param bUseIcons: indicates the behaviour to have in case pIconsList is NULL.
  *                   if bUseIcons = true and pIconsList = NULL then a default image list is used
  *                   if bUseIcons = false and pIconsList = NULL then no images are used
  *                   if pIconsList != NULL then this flag is ignored
  * \param bVertical: true if the layout is vertical (default), false if it is horizontal)
  * \param id : the identifier of the new widget.
  * \param pos : the initial position of the widget
  * \param size: the initial size of the widget
  * \param style: the style of the widgets. See doc for wxCaptionBar and wxButton for more information
  * \param validator : the validator for the caption bar
  * \param name : the name of the widget
  */
wxFoldItemEx::wxFoldItemEx(wxWindow* parent,
                           wxString sCaption,
                           wxWindow* child,
                           long lButtonStyle,
                           wxImageList *pIconsList,
                           bool bUseIcons,
                           bool bVertical,
                           wxWindowID id,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style,
                           const wxString& name
                          )
    : wxPanel(parent, id, pos, size, style, name)
{
    //initialisation
    m_bVertical = bVertical;
    m_lButtonStyle = lButtonStyle;
    m_child = child;
    m_caption = NULL;
    m_images = pIconsList;
    m_bUseImages = bUseIcons;
    m_idCaption = wxNewId();

    //create the sizer and the caption bar
    if (!bVertical)
    {
        MainSizer = new wxBoxSizer(wxHORIZONTAL);
        m_caption = new wxCaptionBarEx(this,
                                       m_idCaption,
                                       sCaption,
                                       WX_CAPTION_NORMAL,
                                       pIconsList,
                                       bUseIcons,
                                       pos,
                                       size,
                                       0
                                      );
    }
    else
    {
        MainSizer = new wxBoxSizer(wxVERTICAL);
        m_caption = new wxCaptionBarEx(this,
                                       m_idCaption,
                                       sCaption,
                                       lButtonStyle,
                                       pIconsList,
                                       bUseIcons,
                                       pos,
                                       size,
                                       0
                                      );
    }

    if (m_child) m_child->Reparent(this);
    ComputeDimensions();

    if (MainSizer)
    {
        if (m_caption) MainSizer->Add(m_caption, 0, wxALL|wxEXPAND|wxALIGN_CENTER|wxALIGN_TOP, 0);
        if (m_child) MainSizer->Add(m_child, 0, wxALL|wxEXPAND|wxALIGN_CENTER|wxALIGN_BOTTOM, 0);
        SetSizer(MainSizer);
        MainSizer->SetSizeHints(this);
        MainSizer->Fit(this);
    }

    Collapse(false);

    Connect(m_idCaption, wxEVT_CAPTIONBAREX, (wxObjectEventFunction)&wxFoldItemEx::OnCaptionBarClicked);
}

/** Destructor
  */
wxFoldItemEx::~wxFoldItemEx(void)
{

}

/** Set the button style (recreate the caption if needed)
  * If the layout is horizontal, nothing is done because only the WX_CAPTION_NORMAL style
  * is allowed (no possibility to draw wxButton with vertical text label)
  * If the layout is vertical, then the caption is recreated if needed
  * \param lNewStyle : the type of caption. Can be one of the following values:
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  */
void wxFoldItemEx::SetButtonStyle(long lNewStyle)
{
    if ((lNewStyle != m_lButtonStyle) && (m_bVertical) && (m_caption))
    {
        m_caption->SetCaptionStyle(lNewStyle, 0);
        if (MainSizer) MainSizer->Layout();
    }
}

/** Get the button style (recreate the caption if needed)
  * \return : the type of caption. Can be one of the following values:
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  */
long wxFoldItemEx::GetButtonStyle(void)
{
    if (m_bVertical) return(m_lButtonStyle);
    return(WX_CAPTION_NORMAL);
}

/** Set the folding orientation (vertical or horizontal)
  * \param bVertical : if true, the layout is vertical
  *                    if false, the layout is horizontal
  */
void wxFoldItemEx::SetOrientation(bool bVertical)
{
    if (bVertical != m_bVertical)
    {
        if (bVertical)
        {
            if (MainSizer) MainSizer->SetOrientation(wxVERTICAL);
            if (m_caption) m_caption->SetVertical();
        }
        else
        {
            if (MainSizer) MainSizer->SetOrientation(wxHORIZONTAL);
            if (m_caption) m_caption->SetHorizontal();
        }
        m_bVertical = bVertical;
        FitSizer();
    }
}

/** Set the folding orientation vertical
  */
void wxFoldItemEx::SetVertical(void)
{
    SetOrientation(true);
}

/** Set the folding orientation horizontal
  */
void wxFoldItemEx::SetHorizontal(void)
{
    SetOrientation(false);
}


/** Get the folding orientation (vertical or horizontal)
  * \return if true, the layout is vertical
  *         if false, the layout is horizontal
  */
bool wxFoldItemEx::GetOrientation(void)
{
    return(m_bVertical);
}

/** Get the folding orientation (vertical or horizontal)
  * \return if true, the layout is vertical
  *         if false, the layout is horizontal
  */
bool wxFoldItemEx::IsVertical(void)
{
    return(m_bVertical);
}

/** Get the folding orientation (vertical or horizontal)
  * \return if true, the layout is horizontal
  *         if false, the layout is horizontal
  */
bool wxFoldItemEx::IsHorizontal(void)
{
    return(!m_bVertical);
}

/** Set the caption on the caption bar / button
  * \param sNewCaption : the new caption to set
  */
void wxFoldItemEx::SetCaption(wxString sNewCaption)
{
    if (m_caption)
    {
        m_caption->SetCaption(sNewCaption);
        if (MainSizer) MainSizer->Layout();
    }
}

/** Get the caption on the caption bar / button
  * \return the caption
  */
wxString wxFoldItemEx::GetCaption(void)
{
    if (m_caption) return(m_caption->GetCaption());
    return(_(""));
}

/** Get the caption bar
  * \return a pointer to the caption bar.
  *  Can be NULL if an error occured during its creation
  */
wxCaptionBarEx* wxFoldItemEx::GetCaptionBar(void)
{
    return(m_caption);
}

/** Get the child (foldable) wxWindow
  * \return a pointer to the child (foldable) wxWindow
  *  Can be NULL if an error occured during its creation
  */
wxWindow* wxFoldItemEx::GetChildWindow(void)
{
    return(m_child);
}

/** Compute and Set minimal size for the window
  */
void wxFoldItemEx::ComputeDimensions(void)
{
    wxSize sMinSizeCaption(0,0);
    wxSize sMinSizeChild(0,0);
    wxSize sGlobalMinSize(0,0);

    if (m_caption) sMinSizeCaption = m_caption->DoGetBestSize();
    if (m_child) sMinSizeChild = m_child->GetBestSize();

    if (IsVertical())
    {
        if (sMinSizeCaption.GetWidth() > sMinSizeChild.GetWidth())
        {
            sGlobalMinSize.SetWidth(sMinSizeCaption.GetWidth());
        }
        else
        {
            sGlobalMinSize.SetWidth(sMinSizeChild.GetWidth());
        }
        sGlobalMinSize.SetHeight(sMinSizeCaption.GetHeight());
        if (IsExpanded())
        {
            int iHeight;
            iHeight = sGlobalMinSize.GetHeight();
            iHeight = iHeight + sMinSizeChild.GetHeight();
            sGlobalMinSize.SetHeight(iHeight);
            MainSizer->SetItemMinSize(m_child, sMinSizeChild.GetWidth(), sMinSizeChild.GetHeight());
        }
        else
        {
            MainSizer->SetItemMinSize(m_child, 0, 0);
        }
    }
    else
    {
        if (sMinSizeCaption.GetHeight() > sMinSizeChild.GetHeight())
        {
            sGlobalMinSize.SetHeight(sMinSizeCaption.GetHeight());
        }
        else
        {
            sGlobalMinSize.SetHeight(sMinSizeChild.GetHeight());
        }
        sGlobalMinSize.SetWidth(sMinSizeCaption.GetWidth());
        if (IsExpanded())
        {
            int iWidth;
            iWidth = sGlobalMinSize.GetWidth();
            iWidth = iWidth + sMinSizeChild.GetWidth();
            sGlobalMinSize.SetWidth(iWidth);
            MainSizer->SetItemMinSize(m_child, sMinSizeChild.GetWidth(), sMinSizeChild.GetHeight());
        }
        else
        {
            MainSizer->SetItemMinSize(m_child, 0, 0);
        }
    }

    SetMinSize(sGlobalMinSize);
    if (MainSizer) MainSizer->SetMinSize(sGlobalMinSize);
}

/** Return the ideal size for the control
  * \return : the ideal size of the caption bar + the child panel
  *           orientation + expanded / collapsed state are taken into account
  */
wxSize wxFoldItemEx::DoGetBestSize(void)
{
    ComputeDimensions();
    return(GetMinSize());
}

/** collapse the caption bar
  * \param bSendEvent : if true, the event wxEVT_CAPTIONBAREX is sent
  */
void wxFoldItemEx::Collapse(bool bSendEvent)
{

    if (m_caption) m_caption->Collapse(bSendEvent);
    if (MainSizer)
    {
        MainSizer->Hide(m_child, true);
        FitSizer();
    }

}

/** expand the caption bar
  * \param bSendEvent : if true, the event wxEVT_CAPTIONBAREX is sent
  */
void wxFoldItemEx::Expand(bool bSendEvent)
{
    if (m_caption) m_caption->Expand(bSendEvent);
    if (MainSizer)
    {
        MainSizer->Show(m_child, true);
        FitSizer();
    }
}

/** Redo the sizer layout
  */
void wxFoldItemEx::FitSizer(void)
{
    ComputeDimensions();
    if (MainSizer) MainSizer->Layout();

    Refresh(false, NULL);
    Update();

}


/** return true if the caption bar is expanded
  * \return true if the caption bar is expanded
  */
bool wxFoldItemEx::IsExpanded(void)
{
    if (m_caption) return(m_caption->IsExpanded());
    return(false);
}

/** return true if the caption bar is collapsed
  * \return true if the caption bar is collapsed
  */
bool wxFoldItemEx::IsCollapsed(void)
{
    if (m_caption) return(m_caption->IsCollapsed());
    return(false);
}

/** Collapse / Expand event handler
  * This generates an event wxEVT_FOLDITEMEX
  */
void wxFoldItemEx::OnCaptionBarClicked(wxCommandEvent &event)
{
    int iCollapsed;
    bool bCollapsed;

    iCollapsed = event.GetInt();

    if (iCollapsed == 1) bCollapsed = false; else bCollapsed = true;

    SendEvent(bCollapsed);
}

/** send a wxEVT_FOLDITEMEX event
  * \param bCollapsed : true if the caption bar has been collapsed
  *                     false if the caption bar has been expanded
  */
void wxFoldItemEx::SendEvent(bool bCollapsed)
{
    wxCommandEvent event(wxEVT_FOLDITEMEX, GetId());
    event.SetEventObject(this);

    if (bCollapsed) event.SetInt(0); else event.SetInt(1);

    // Send the event
    GetEventHandler()->ProcessEvent(event);
}

