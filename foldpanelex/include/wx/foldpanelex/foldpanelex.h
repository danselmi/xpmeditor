/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      foldpanelex.h
 * Purpose:   a panel which allows folding of sub-panels
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-07-04
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef _H_WX_FOLDPANELEX_H_
#define _H_WX_FOLDPANELEX_H_

#include "wx/foldpanelex/folditemex.h"

#include <wx/scrolwin.h>
#include <wx/imaglist.h>
#include <wx/sizer.h>

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_FOLDPANELEX, -1)
END_DECLARE_EVENT_TYPES()

/* The following does not work when compiling a plugin (dll), but works when compiling
   a standalone program: a link error is given, saying that _imp_wxEVT_TRANSPARENT_COLOR_CHANGED
   symbol is not found

   a dll import attribute is probably needed, and the use of the macro DECLARE_EXPORTED_EVENT_TYPE
   probably as well

   However, looking at the event.h file shows these definitions:

   #define DECLARE_LOCAL_EVENT_TYPE(name, value) \
    DECLARE_EXPORTED_EVENT_TYPE(wxEMPTY_PARAMETER_VALUE, name, value)

    So it may make no differences after all.

    TO DO: check the wxWidgets Doc and understand all these events macros.
    No need to get the old compatibility behaviour AFAIK

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_FOLDPANELEX, -1)
END_DECLARE_EVENT_TYPES()
*/

class wxFoldPanelEx;

typedef bool (wxFoldPanelEx::*wxVetoFunction)(wxFoldItemEx*, bool);

class wxFoldPanelEx : public wxScrolledWindow
{
    public:
        wxFoldPanelEx(wxWindow* parent,
                      wxWindowID id = -1,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxHSCROLL | wxVSCROLL,
                      const wxString& name = _("FoldPanelEx")
                     ); ///< \brief Constructor
        ~wxFoldPanelEx(void); ///< \brief destructor

        //orientation
        void SetOrientation(bool bVertical); ///< \brief Set the folding orientation (vertical or horizontal)
        bool GetOrientation(void);           ///< \brief Get the folding orientation (vertical or horizontal)
        bool IsVertical(void);               ///< \brief Return true if the orientation is vertical, false if horizontal
        bool IsHorizontal(void);             ///< \brief Return true if the orientation is horizontal, false if vertical

        //item access
        wxFoldItemEx* GetFoldItem(size_t lIndex);      ///< \brief Return the given wxFoldItemEx (composite control: caption + wxWindow)
        wxFoldItemEx* GetFoldItem(wxString sCaption);  ///< \brief Return the given wxFoldItemEx (composite control: caption + wxWindow)
        size_t GetFoldItemIndex(wxString sCaption);    ///< \brief Return the index of a wxFoldItemEx
        size_t GetFoldItemIndex(wxFoldItemEx* pPanel); ///< \brief Return the index of a wxFoldItemEx
        size_t GetCount(void);                         ///< \brief Return the amount of fold panels

        //add or delete foldable items
        wxFoldItemEx* AddFoldPanel(wxString sCaption, wxWindow *item,
                                   long lCaptionStyle); ///< \brief append a new panel
        wxFoldItemEx* InsertFoldPanel(wxString sCaption, wxWindow *item, size_t lIndex,
                                      long lCaptionStyle); ///< \brief insert a new panel
        bool DeleteFoldPanel(size_t lIndex);         ///< \brief delete the fold panel at index lIndex
        bool DeleteFoldPanel(wxFoldItemEx*  pPanel); ///< \brief delete the fold panel corresponding to the given pointer

        //collapse / expand a panel
        bool Collapse(size_t iIndex, bool bSendEvent); ///< \brief Collapse a panel
        bool Collapse(wxFoldItemEx *f, bool bSendEvent); ///< \brief Collapse a panel
        bool Expand(size_t iIndex, bool bSendEvent);   ///< \brief Expand a panel
        bool Expand(wxFoldItemEx *f, bool bSendEvent);   ///< \brief Expand a panel

        //vetoing the collapse / expand
        bool SetVetoCallback(wxVetoFunction f);  ///< \brief Add a veto callback function
        void DeleteVetoCallback(void);           ///< \brief Remove the veto callback function

    protected:
        wxImageList *m_images;     ///< \brief the image list of icons
        bool m_bVertical;          ///< \brief true if the folding direction is vertical (the default)
        wxBoxSizer *MainSizer;     ///< \brief the main sizer
        wxFoldItemExArray m_array; ///< \brief the list of wxFoldItemEx belonging to this panel
        wxVetoFunction m_veto;     ///< \brief the standard veto function

        void OnResize(wxSizeEvent& event); ///< \brief size event handler

        bool VetoStd(wxFoldItemEx *f, bool bCollapsed); ///< \brief the default veto function. Accepts everything

        void ComputeDimensions(void);  ///< \brief Compute and Set minimal size for the window
        virtual wxSize DoGetBestSize(void); ///< \brief Return the ideal size for the control
        void FitSizer(void); ///< \brief redo the sizer layout
        void UpdateScrollBars(void);  ///< \brief update the scroll increments - it will automatically hide / show the scrollbars
        void OnCaptionBarClicked(wxCommandEvent &event); ///< \brief a fold item has been clicked for folding / unfolding
        void SendEvent(wxFoldItemEx *f, bool bCollapsed);  ///< \brief send a collapsed / expanded event
};



#endif
