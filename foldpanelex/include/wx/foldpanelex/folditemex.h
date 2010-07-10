/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      folditemex.h
 * Purpose:   a composite control containing a wxCaptionBarEx and a wxWindow
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-07-04
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef _H_WX_FOLDITEMEX_H_
#define _H_WX_FOLDITEMEX_H_

#include "wx/foldpanelex/captionbarex.h"

#include <wx/panel.h>
#include <wx/sizer.h>

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_FOLDITEMEX, -1)
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
    DECLARE_EVENT_TYPE(wxEVT_FOLDITEMEX, -1)
END_DECLARE_EVENT_TYPES()
*/

class wxFoldItemEx : public wxPanel
{
    public:
        //constructor and destructor
        wxFoldItemEx(wxWindow* parent,
                     wxString sCaption,
                     wxWindow* child,
                     long StyleCaption,
                     wxImageList *pIconsList,
                     bool bUseIcons,
                     bool bVertical = true,
                     wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxTAB_TRAVERSAL,
                     const wxString& name = _("FoldItemEx")
                    ); ///< \brief Constructor
        ~wxFoldItemEx(void); ///< \brief destructor

        //button style
        void SetButtonStyle(long lNewStyle); ///< \brief Set the button style (recreate the caption if needed)
        long GetButtonStyle(void);           ///< \brief Get the button style

        //orientation
        void SetOrientation(bool bVertical); ///< \brief Set the folding orientation (vertical or horizontal)
        void SetVertical(void);              ///< \brief Set the folding orientation vertical
        void SetHorizontal(void);            ///< \brief Set the folding orientation horizontal
        bool GetOrientation(void);           ///< \brief Get the folding direction
        bool IsVertical(void);               ///< \brief Return true if the layout is vertical
        bool IsHorizontal(void);             ///< \brief Return true if the layout is vertical

        //caption
        void SetCaption(wxString sNewCaption); ///< \brief Set the caption
        wxString GetCaption(void);             ///< \brief Get the caption

         //collapse / expand methods
        void Expand(bool bSendEvent);          ///< \brief expand the caption bar
        void Collapse(bool bSendEvent);        ///< \brief collapse the caption bar
        bool IsExpanded(void);                 ///< \brief return true if the caption bar is expanded
        bool IsCollapsed(void);                ///< \brief return true if the caption bar is collapsed

        //accessors
        wxCaptionBarEx* GetCaptionBar(void);   ///< \brief Get the caption bar
        wxWindow* GetChildWindow(void);        ///< \brief Get the child (foldable) window

    protected:
        wxBoxSizer *MainSizer;     ///< \brief the main sizer used to do the layout
        long m_lButtonStyle;       ///< \brief the style of caption: caption bar, button, flat button
        wxWindow *m_child;         ///< \brief the child window to fold / unfold
        wxCaptionBarEx *m_caption; ///< \brief the caption bar
        bool m_bVertical;          ///< \brief true if the layout is vertical, false if it is horizontal
        bool m_bUseImages;         ///< \brief used to save initial creation style
        wxImageList *m_images;     ///< \brief used to save initial creation style
        wxWindowID m_idCaption;    ///< \brief the id of the caption bar

        void OnCaptionBarClicked(wxCommandEvent &event); ///< \brief the caption bar is expanded or collapsed
        void SendEvent(bool bCollapsed); ///< \brief send a wxEVT_FOLDITEMEX event
        void FitSizer(void);           ///< \brief redo the sizer layout
        void ComputeDimensions(void);  ///< \brief Compute and Set minimal size for the window
        virtual wxSize DoGetBestSize(void); ///< \brief Return the ideal size for the control

};


#include <wx/dynarray.h>
WX_DEFINE_ARRAY_PTR(wxFoldItemEx *, wxFoldItemExArray);

#endif //#ifndef _H_WX_FOLDITEMEX_H_
