/***************************************************************
 * Widgets:   wxResizeCtrl
 * Name:      wxResizeCtrl.h
 * Purpose:   Draw a sizing border around the child wxWindow, and allow the user to resize it
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-04-17
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef RESIZECONTROL_H
#define RESIZECONTROL_H

#include <wx/window.h>
#include <wx/colour.h>

/** \brief this simple class is a resizeable and moveable control: it has a child window
  * that the user can resize or move using the mouse.
  *
*/
class wxResizeCtrl : public wxWindow
{
    public:
        //constructors
        wxResizeCtrl(void); ///< \brief default constructor. call Create() after the constructor

        wxResizeCtrl(wxWindow *parent,
                     wxWindowID id,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxBORDER_SIMPLE,
                     const wxString& name = wxPanelNameStr
                    ); ///< \brief constructor

        wxResizeCtrl(wxWindow *parent,
                     wxWindowID id,
                     wxWindow *child,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxBORDER_SIMPLE,
                     const wxString& name = wxPanelNameStr
                    ); ///< \brief constructor - call SetChild()

        ~wxResizeCtrl(void); ///< \brief destructor

        //initializations methods
        void Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxBORDER_SIMPLE,
                    const wxString& name = wxPanelNameStr
                   ); ///< \brief create the window itself

        void SetChild(wxWindow *Child); ///< \brief set the child window
        wxWindow* GetChild(void);       ///< \brief get the child window
        void SetMargin(int iNewMargin); ///< \brief set the size of the resizing border, in pixel
        int GetMargin(void);            ///< \brief get the size of the resizing border, in pixel
        void SetColour(wxColour cColour); ///< \brief set the colour of the resizing border
        wxColour GetColour(void);       ///< \brief  get the colour of the resizing border

    protected:
        //Event handlers
        void OnMouseMove(wxMouseEvent& event); ///< \brief  Mouse events handler
		void OnLeftDown(wxMouseEvent& event);  ///< \brief  Mouse left button down event
		void OnLeftUp(wxMouseEvent& event);    ///< \brief  Mouse left button up event
		void OnSize(wxSizeEvent& event);       ///< \brief  Size event
		void OnPaint(wxPaintEvent& event);     ///< \brief  Paint event
		void OnCaptureLost(wxMouseCaptureLostEvent& event); ///< \brief mouse capture lost event

    private:

        void SetChildSize(wxSize s); ///< \brief compute the new size and pos of the child, and resize / move it

        wxWindow *child;     ///< \brief the child window. Can be anything, including a wxPanel
        int iMargin;         ///< \brief the size of the resizing border border
        wxColour cBoxColour; ///< \brief the colour of the resizing blocks
        bool bResizeTop;     ///< \brief true if resizing the top border
        bool bResizeLeft;    ///< \brief true if resizing the left border
        bool bResizeBottom;  ///< \brief true if resizing the bottom border
        bool bResizeRight;   ///< \brief true if resizing the right border
        bool bMoving;        ///< \brief true if moving the window
        int x;
        int y;
};

#endif
