/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      captionbarex.h
 * Purpose:   wrapper around wxCode captionbar - definition
 *            This wrapper allows the alternative use of a flat button
 *            for portability purpose
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-07-04
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

//#if USE_CAPTIONBAREX

#ifndef _CAPTIONBAREX_H_
#define _CAPTIONBAREX_H_

#include <wx/window.h>
#include <wx/button.h>
#include <wx/imaglist.h>
#include <wx/dcclient.h>
#include <wx/event.h>

#include <wx/foldpanelex/captionbar.h>

#define WX_CAPTION_FLATBUTTON 0
#define WX_CAPTION_NORMAL     1
#define WX_CAPTION_BUTTON     2

#define WX_CAPTION_MAX_WIDTH 5000
#define WX_CAPTION_MAX_HEIGHT 5000

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_CAPTIONBAREX, -1)
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
    DECLARE_EVENT_TYPE(wxEVT_CAPTIONBAREX, -1)
END_DECLARE_EVENT_TYPES()
*/




class wxCaptionBarEx : public wxWindow
{
    public:
        wxCaptionBarEx(wxWindow *parent,
                       wxWindowID id,
                       const wxString &caption,
                       long StyleCaption = WX_CAPTION_NORMAL,
                       wxImageList *images = NULL,
                       bool bUseImages = true,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = 0,
                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = _("wxCaptionBarEx"));  ///< \brief constructor
        ~wxCaptionBarEx(void); ///< \brief destructor

        //properties accessors
        long GetCaptionStyle(void);                          ///< \brief get the caption style: normal, button or flat button
        void SetCaptionStyle(long lNewStyle, long lStyle);   ///< \brief set the caption style: normal, button or flat button
        wxCaptionBar* GetCaptionBar(void);                   ///< \brief get a pointer to the normal caption widget, or NULL if not applicable
        wxButton* GetButton(void);                           ///< \brief get a pointer to the button caption widget, or NULL if not applicable

        //size methods
        virtual wxSize DoGetBestSize(void) const; ///< \brief Return the ideal size for the control
        void ComputeDimensions(void);             ///< \brief Compute minimal and maximal sizes

        //caption methods
        wxString GetCaption(void);             ///< \brief Get the caption of the bar / button
        void SetCaption(wxString sNewCaption); ///< \brief Set the caption of the bar / button

        //collapse / expand methods
        void Expand(bool bSendEvent);          ///< \brief expand the caption bar
        void Collapse(bool bSendEvent);        ///< \brief collapse the caption bar
        bool IsExpanded(void);                 ///< \brief return true if the caption bar is expanded
        bool IsCollapsed(void);                ///< \brief return true if the caption bar is collapsed

        //orientation methods
        void SetVertical(void);        ///< \brief Set the folding direction vertical
        void SetHorizontal(void);      ///< \brief Set the folding direction horizontal
        bool IsVertical(void);         ///< \brief Return true if the folding direction is vertical, false if horizontal
        bool IsHorizontal(void);       ///< \brief Return true if the folding direction is horizontal, false if vertical

    protected:
        bool m_bCollapsed;     ///< \brief true if the bar is collapsed, false otherwise
        wxCaptionBar *cbar;    ///< \brief the captionbar widget used
        wxButton *button;      ///< \brief the button (flat or not) used
        long m_CaptionStyle;   ///< \brief the type of caption (normal, button, flat button)
        wxImageList *m_images; ///< \brief the image list of icons
        wxWindowID idBar;      ///< \brief the id of the wxCaptionBar
        wxWindowID ibButton;   ///< \brief the id of the wxButton
        bool m_bVertical;      ///< \brief true if the caption is vertical (folding direction. default), false if horizontal
        long m_buttonstyle;    ///< \brief save the initial button style
        long m_captionbarstyle;///< \brief save the initial caption bar style

        //event handler
        void SendEvent(bool bCollapsed); ///< \brief send a wxEVT_CAPTIONBAREX event
        void OnCaptionBarClicked(wxCommandEvent &event); ///< \brief wxCaptionBar handler
        void OnButtonClicked(wxCommandEvent &event); ///< \brief wxButton clicked handler
        void OnSizeEvent(wxSizeEvent &event); ///< \brief size event handler

        void CreateCaptionBar(wxWindow *parent, wxWindowID id,
                              const wxString &caption,
                              wxImageList *images,
                              const wxPoint& pos,
                              const wxSize& size,
                              long style
                             ); ///< \brief create the caption bar
        void CreateButtonBar(wxWindow *parent, wxWindowID id,
                             const wxString &caption,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxValidator& validator = wxDefaultValidator
                            ); ///< \brief create the button bar
        void CreateFlatButtonBar(wxWindow *parent,
                                 wxWindowID id,
                                 const wxString &caption,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxValidator& validator = wxDefaultValidator
                                ); ///< \brief create the flat button bar
        void DestroyButtonBar(void);  ///< \brief destroy the button bar if it already exists
        void DestroyCaptionBar(void); ///< \brief destroy the caption bar if it already exists
        void DisconnectEvents(void);  ///< \brief remove events handlers before destruction
};


#endif //#define _CAPTIONBAREX_H_

//#endif //#if USE_CAPTIONBAREX
