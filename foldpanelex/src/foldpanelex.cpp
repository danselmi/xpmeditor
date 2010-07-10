/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      foldpanelex.cpp
 * Purpose:   a panel which allows folding of sub-panels
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-07-04
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/


#ifdef WXMAKINGDLL_FOLDPANELEX
    #define WXDLLIMPEXP_FOLDPANELEX WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_FOLDPANELEX WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_FOLDPANELEX
#endif

#include "wx/foldpanelex/foldpanelex.h"
#include "icon_collapsed.xpm"
#include "icon_expanded.xpm"

DEFINE_EVENT_TYPE(wxEVT_FOLDPANELEX)

/** Constructor
  * By default, the layout is vertical.
  * Use the SetOrientation method to change it.
  * \param parent : a pointer to the parent window
  * \param id : the identifier of the new widgets.
  * \param pos : the initial position of the widget
  * \param size: the initial size of the widget
  * \param style: the style of the window
  * \param validator : the validator for the caption bar
  * \param name : the name of the widget
  */
wxFoldPanelEx::wxFoldPanelEx(wxWindow* parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxString& name
                            )
    : wxScrolledWindow(parent, id, pos, size, style, name)
{
    //initialisations
    m_bVertical = true;
    m_array.Empty();

    // create the fold icons to be used in the captions
    m_images = new wxImageList(16, 16);

    wxBitmap *bmp = new wxBitmap(icon_expanded);
    if ((m_images) && (bmp)) m_images->Add(*bmp);
    delete(bmp);

    bmp = new wxBitmap(icon_collapsed);
    if ((m_images) && (bmp)) m_images->Add(*bmp);
    delete(bmp);

    //Veto function
    m_veto = (wxVetoFunction) &wxFoldPanelEx::VetoStd;

    MainSizer = new wxBoxSizer(wxVERTICAL);
    if (MainSizer)
    {
        SetSizer(MainSizer);
        MainSizer->Fit(this);
        MainSizer->SetSizeHints(this);
    }
    //SetAutoLayout(false);

    Connect(wxEVT_SIZE,(wxObjectEventFunction)&wxFoldPanelEx::OnResize);
}

/** Destructor
  */
wxFoldPanelEx::~wxFoldPanelEx(void)
{
    if (m_images) delete(m_images);
}

/** Size event handler
  * will adjust the scrollbars, if needed
  */
void wxFoldPanelEx::OnResize(wxSizeEvent& event)
{
    FitSizer();
    event.Skip();
}

/** update the scroll increments
  * It will automatically hide / show the scrollbars
  */
void wxFoldPanelEx::UpdateScrollBars(void)
{
    SetScrollRate(10,10);
}

/** Set the folding orientation (vertical or horizontal)
  * All the child wxFoldPanelEx will be recursively set with the new style
  * If needed, the caption bar is recreated
  * \param bVertical : if true, the new layout is vertical
  *                    if false, the layout is horizontal
  */
void wxFoldPanelEx::SetOrientation(bool bVertical)
{

    size_t i, iMax;
    wxFoldItemEx *f;

    if (bVertical != m_bVertical)
    {
        m_bVertical = bVertical;

        iMax = GetCount();
        for(i=0;i<iMax;i++)
        {
            f = GetFoldItem(i);
            if (f) f->SetOrientation(bVertical);
        }
    }
}

/** Get the folding orientation (vertical or horizontal)
  * \return true if the orientation is vertical, false if horizontal
  */
bool wxFoldPanelEx::GetOrientation(void)
{
    return(m_bVertical);
}

/** Return true if the orientation is vertical, false if horizontal
  * \return true if the orientation is vertical, false if horizontal
  */
bool wxFoldPanelEx::IsVertical(void)
{
    return(m_bVertical);
}

/** Return true if the orientation is horizontal, false if vertical
  * \return true if the orientation is horizontal, false if vertical
  */
bool wxFoldPanelEx::IsHorizontal(void)
{
    return(!m_bVertical);
}

/** Return the given wxFoldItemEx (composite control: caption + wxWindow)
  * \param lIndex : the 0-based index of the wxFoldItemEx to recover
  *                 index corresponds to the order of addition of the panels
  * \return a pointer to the requested wxFoldItemEx,
  *         NULL if an error occured or is not found
  */
wxFoldItemEx* wxFoldPanelEx::GetFoldItem(size_t lIndex)
{
    if (lIndex >= GetCount()) return(NULL);
    return(m_array.Item(lIndex));
}

/** Return the given wxFoldItemEx (composite control: caption + wxWindow)
  * \param sCaption : the wxString displayed in the caption bar / button used
  *                   to collapse / expand the panel
  *                   WARNING: if 2 fold items have the same caption, the 1st one found
  *                            will be returned
  * \return a pointer to the requested wxFoldItemEx,
  *         or NULL if an error occured or is not found
  */
wxFoldItemEx* wxFoldPanelEx::GetFoldItem(wxString sCaption)
{
    size_t i, iMax;
    wxFoldItemEx *f;
    wxString sCaption2;

    iMax = GetCount();
    for(i=0;i<iMax;i++)
    {
        f = m_array.Item(i);
        if (f)
        {
            sCaption2 = f->GetCaption();
            if (sCaption2 == sCaption) return(f);
        }
    }

    return(NULL);
}

/** Return the index of a wxFoldItemEx (composite control: caption + wxWindow)
  * \param sCaption : the wxString displayed in the caption bar / button used
  *                   to collapse / expand the panel
  *                   WARNING: if 2 fold items have the same caption, the 1st one found
  *                            will be returned
  * \return the index of a wxFoldItemEx if found
  *         an index superior to GetCount() on error / if not found
  *         Use the test if (retValue < wxFoldPanelEx::GetCount()) to test for success/failure
  *         Example:
  *         @verbatim
  *             wxFoldPanelEx fp();
  *             size_t iIndex;
  *
  *              ...
  *
  *             iIndex = fp.GetFoldItemIndex(_("caption text");
  *             if (iIndex < fp.GetCount())
  *             {
  *                  //success - normal code here
  *             }
  *             else
  *             {
  *                  //failure - error handling code here
  *             }
  *         @endverbatim
  */
size_t wxFoldPanelEx::GetFoldItemIndex(wxString sCaption)
{
    size_t i, iMax;
    wxFoldItemEx *f;
    wxString sCaption2;

    iMax = GetCount();
    for(i=0;i<iMax;i++)
    {
        f = m_array.Item(i);
        if (f)
        {
            sCaption2 = f->GetCaption();
            if (sCaption2 == sCaption) return(i);
        }
    }

    return(GetCount() + 1);
}

/** Return the index of a wxFoldItemEx (composite control: caption + wxWindow)
  * \param pPanel: the foldable panel to delete.
  *                this pointer has been returned by the AddPanel or InsertPanel method
  * \return the index of a wxFoldItemEx if found
  *         an index superior to GetCount() on error / if not found
  *         Use the test if (retValue < wxFoldPanelEx::GetCount()) to test for success/failure
  *         Example:
  *         @verbatim
  *             wxFoldPanelEx fp();
  *             size_t iIndex;
  *
  *              ...
  *
  *             iIndex = fp.GetFoldItemIndex(_("caption text");
  *             if (iIndex < fp.GetCount())
  *             {
  *                  //success - normal code here
  *             }
  *             else
  *             {
  *                  //failure - error handling code here
  *             }
  *         @endverbatim
  */
size_t wxFoldPanelEx::GetFoldItemIndex(wxFoldItemEx* pPanel)
{
    size_t i, iMax;
    wxFoldItemEx *f;
    wxString sCaption2;

    iMax = GetCount();
    for(i=0;i<iMax;i++)
    {
        f = m_array.Item(i);
        if (f == pPanel)
        {
            return(i);
        }
    }

    return(GetCount() + 1);
}

/** Return the amount of fold panels
  * \return the amount of fold panels
  */
size_t wxFoldPanelEx::GetCount(void)
{
    return(m_array.GetCount());
}

/** Append a new panel (it will be displayed after all the others)
  * \param sCaption : the caption to use
  * \param item : the wxWindow to add. This window will be foldable
  *               It must be created before calling this method
  *               The window will be reparented (wxWindow::Reparent() will be called)
  * \param lCaptionStyle: the type of caption.
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  *                for horizontal layout, this parameter has no influence
  * \return a pointer to the wxFoldItemEx created on success
  *         NULL on failure
  */
wxFoldItemEx*  wxFoldPanelEx::AddFoldPanel(wxString sCaption, wxWindow *item, long lCaptionStyle)
{
    if (!MainSizer) return(NULL);

    wxFoldItemEx *f = NULL;
    wxWindowID id = wxNewId();
    f = new wxFoldItemEx(this,
                         sCaption,
                         item,
                         lCaptionStyle,
                         m_images,
                         true,
                         m_bVertical,
                         id
                        );
    if (!f) return(NULL);

    m_array.Add(f);

    MainSizer->Add(f, 0, wxALL|wxEXPAND|wxALIGN_CENTER|wxALIGN_BOTTOM, 1);

    FitSizer();

    Connect(id, wxEVT_FOLDITEMEX, (wxObjectEventFunction)&wxFoldPanelEx::OnCaptionBarClicked);

    return(f);
}

/** Insert a new panel (it will be displayed after the panel lIndex)
  * \param sCaption : the caption to use
  * \param item : the wxWindow to add. This window will be foldable
  *               It must be created before calling this method
  *               The window will be reparented (wxWindow::Reparent() will be called)
  * \param lIndex : the 0-based index of the panel immediately AFTER the new panel
  * \param lCaptionStyle: the type of caption.
  *                WX_CAPTION_FLATBUTTON : a flat button style caption
  *                WX_CAPTION_NORMAL : a normal style caption
  *                WX_CAPTION_BUTTON : a normal button style caption
  *                for horizontal layout, this parameter has no influence
  * \return a pointer to the wxFoldItemEx created on success
  *         NULL on failure
  */
wxFoldItemEx*  wxFoldPanelEx::InsertFoldPanel(wxString sCaption, wxWindow *item, size_t lIndex, long lCaptionStyle)
{
    if (lIndex >= GetCount()) return(NULL);
    if (!MainSizer) return(NULL);

    wxFoldItemEx *f = NULL;
    wxWindowID id = wxNewId();
    f = new wxFoldItemEx(this,
                         sCaption,
                         item,
                         lCaptionStyle,
                         m_images,
                         true,
                         m_bVertical,
                         id
                        );
    if (!f) return(NULL);

    m_array.Insert(f, lIndex);

    MainSizer->Insert(lIndex, f, 0, wxALL|wxEXPAND|wxALIGN_CENTER|wxALIGN_BOTTOM, 1);

    FitSizer();

    Connect(id, wxEVT_FOLDITEMEX, (wxObjectEventFunction)&wxFoldPanelEx::OnCaptionBarClicked);

    return(f);
}

/** Delete a panel
  * \param lIndex : the 0-based index of the panel to delete
  *                 if the index is invalid, nothing is done
  * \return true on success, false on failure
  */
bool wxFoldPanelEx::DeleteFoldPanel(size_t lIndex)
{
    if (lIndex >= GetCount()) return(false);
    if (!MainSizer) return(false);

    wxFoldItemEx *f = NULL;
    wxWindowID id;

    f = GetFoldItem(lIndex);
    if (!f) return(false);
    id = f->GetId();

    Disconnect(id, wxEVT_FOLDITEMEX, (wxObjectEventFunction)&wxFoldPanelEx::OnCaptionBarClicked);
    m_array.RemoveAt(lIndex);

    MainSizer->Detach(lIndex);
    f->Destroy();

    FitSizer();

    return(true);
}

/** delete the fold panel corresponding to the given pointer
  * \param pPanel: the foldable panel to delete.
  *                this pointer has been returned by the AddPanel or InsertPanel method
  *                @see also the following methods to find a panel:
  *                @GetFoldItem(), @GetFoldItemIndex()
  * \return true on success, false on failure
  */
bool wxFoldPanelEx::DeleteFoldPanel(wxFoldItemEx*  pPanel)
{
    size_t iIndex;

    iIndex = GetFoldItemIndex(pPanel);
    if (iIndex < GetCount()) return(DeleteFoldPanel(iIndex));
    return(false);
}

/** Collapse / Expand event handler
  */
void wxFoldPanelEx::OnCaptionBarClicked(wxCommandEvent &event)
{
    wxFoldItemEx *f = NULL;
    wxWindowID id;

    id = event.GetId();
    f = (wxFoldItemEx *) FindWindow(id);

    if (event.GetInt() == 0)
    {
        Collapse(f, false);
    }
    else
    {
        Expand(f, false);
    }
}

/** Compute and Set minimal size for the window
  */
void wxFoldPanelEx::ComputeDimensions(void)
{
    size_t i, iMax;
    wxFoldItemEx *f;
    wxSize sMinSize(0,0);
    wxSize sSize(0,0);

    iMax = GetCount();
    for(i=0;i<iMax;i++)
    {
        f = GetFoldItem(i);
        if (f)
        {
            sSize = f->GetBestSize();
            if (MainSizer) MainSizer->SetItemMinSize(f, sSize.GetWidth(), sSize.GetHeight());
        }
        else
        {
            sSize.SetWidth(0);
            sSize.SetHeight(0);
        }

        if (m_bVertical)
        {
            sMinSize.SetHeight(sMinSize.GetHeight() + sSize.GetHeight());
            if (sSize.GetWidth() > sMinSize.GetWidth()) sMinSize.SetWidth(sSize.GetWidth());
        }
        else
        {
            sMinSize.SetWidth(sMinSize.GetWidth() + sSize.GetWidth());
            if (sSize.GetHeight() > sMinSize.GetHeight()) sMinSize.SetHeight(sSize.GetHeight());
        }
    }

    SetMinSize(sMinSize);
    if (MainSizer) MainSizer->SetMinSize(sMinSize);
}

/** Return the ideal size for the control
  * \return : the ideal size of the caption bar + the child panel
  *           orientation + expanded / collapsed state are taken into account
  */
wxSize wxFoldPanelEx::DoGetBestSize(void)
{
    ComputeDimensions();
    return(GetMinSize());
}

/** Redo the sizer layout
  */
void wxFoldPanelEx::FitSizer(void)
{
    ComputeDimensions();
    if (MainSizer)
    {
        MainSizer->FitInside(this);
        MainSizer->Layout();
    }
    UpdateScrollBars();
}

/** Collapse the fold item at the given index
  * If the panel is already collapsed, nothing happens, and no
  * events are sent, regardless of the value of bSendEvent
  * \param iIndex : the index of the panel to fold
  * \param bSendEvent: if true, an event wxEVT_FOLDPANELEX is sent
  * \return true on success, false on failure (vetoed)
  */
bool wxFoldPanelEx::Collapse(size_t iIndex, bool bSendEvent)
{
    wxFoldItemEx *f;

    f = GetFoldItem(iIndex);
    return(Collapse(f, bSendEvent));
}

/** Collapse the fold item at the given index
  * If the panel is already collapsed, nothing happens, and no
  * events are sent, regardless of the value of bSendEvent
  * \param f : the wxFoldItemEx to collapse
  * \param bSendEvent: if true, an event wxEVT_FOLDPANELEX is sent
  * \return true on success, false on failure (vetoed)
  */
bool wxFoldPanelEx::Collapse(wxFoldItemEx *f, bool bSendEvent)
{
    if (!f) return(false);

    //vetoing
    if (m_veto)
    {
        if (! (this ->* ((wxFoldPanelEx*)this)->wxFoldPanelEx::m_veto)(f, true)) return(false);
    }

    f->Collapse(false);
    if (bSendEvent) SendEvent(f, true);

    FitSizer();

    return(true);
}

/** Expand the fold item at the given index
  * If the panel is already expanded, nothing happens, and no
  * events are sent, regardless of the value of bSendEvent
  * \param iIndex : the index of the panel to fold
  * \param bSendEvent: if true, an event wxEVT_FOLDPANELEX is sent
  * \return true on success, false on failure (vetoed)
  */
bool wxFoldPanelEx::Expand(size_t iIndex, bool bSendEvent)
{
    wxFoldItemEx *f;

    f = GetFoldItem(iIndex);
    return(Expand(f, bSendEvent));
}


/** Expand the fold item at the given index
  * If the panel is already expanded, nothing happens, and no
  * events are sent, regardless of the value of bSendEvent
  * \param f : the wxFoldItemEx to expand
  * \param bSendEvent: if true, an event wxEVT_FOLDPANELEX is sent
  * \return true on success, false on failure (vetoed)
  */
bool wxFoldPanelEx::Expand(wxFoldItemEx *f, bool bSendEvent)
{
    if (!f) return(false);

    //vetoing
    if (m_veto)
    {
        if (! (this ->* ((wxFoldPanelEx*)this)->wxFoldPanelEx::m_veto)(f, true)) return(false);
    }

    f->Expand(false);
    if (bSendEvent) SendEvent(f, false);

    FitSizer();

    return(true);
}

/** send a wxEVT_FOLDPANELEX event
  * \param f : a pointer to  the wxFoldItemEx for which the event applies
  * \param bCollapsed : true if the caption bar has been collapsed
  *                     false if the caption bar has been expanded
  */
void wxFoldPanelEx::SendEvent(wxFoldItemEx *f, bool bCollapsed)
{
    wxCommandEvent event(wxEVT_FOLDPANELEX, GetId());

    //set event properties
    event.SetEventObject(this);
    event.SetClientData(f);
    if (bCollapsed) event.SetInt(0); else event.SetInt(1);

    // Send the event
    GetEventHandler()->ProcessEvent(event);
}

/** the default veto function. Accepts everything
  * \param f : the wxFoldItemEx to which the event occurs
  * \param bCollapsed : if true, the panel is being collapsed.
  *                     If false, the panel is being expanded
  * \return true if the event is allowed to occur, false otherwise
  */
bool wxFoldPanelEx::VetoStd(wxFoldItemEx *f, bool bCollapsed)
{
    return(true);
}

/** Add a veto callback function
  * \param f : a pointer to the callback function
  *            The function must have the following prototype:
  *            @verbatim
  *             bool VetoCallbackFunction(wxFoldItemEx *f, bool bCollapsed);
  *            @endverbatim
  *            with : param f: the wxFoldItemEx to which the event occurs
  *                   param bCollapsed: if true, the panel is being collapsed.
  *                                     If false, the panel is being expanded
  *                   return value: if the function returns true, the event is allowed to occurs
  *                                 if it returns false, nothing happens
  * \return true on success, false on failure
  */
bool wxFoldPanelEx::SetVetoCallback(wxVetoFunction f)
{
    if (f) return(false);
    m_veto = f;
    return(true);
}

/** remove the Veto callback function
  */
void wxFoldPanelEx::DeleteVetoCallback(void)
{
    m_veto = (wxVetoFunction) &wxFoldPanelEx::VetoStd;
}
