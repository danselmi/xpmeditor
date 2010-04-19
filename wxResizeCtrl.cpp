/***************************************************************
 * Widgets:   wxResizeCtrl
 * Name:      wxResizeCtrl.cpp
 * Purpose:   Draw a sizing border around the child wxWindow, and allow the user to resize it
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2010-04-17
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "wxResizeCtrl.h"
#include <wx/dcclient.h>
#include <wx/utils.h>

//----- CONSTRUCTORS & DESTRUCTOR ---------------

/** DEFAULT CONSTRUCTOR
  */
wxResizeCtrl::wxResizeCtrl(void) : wxWindow()
{
    Create(NULL,-1);
}

/** CONSTRUCTOR
  * @param parent: pointer to the parent window
  * @param id: id of the new window. Set to -1 for automatic generation
  * @param pos: the position of the new window
  * @param size: the size of the new window
  * @param style: the style to be apply to the window
  * @param name: the name to be given to this window
  */
wxResizeCtrl::wxResizeCtrl(wxWindow *parent,
                     wxWindowID id,
                     const wxPoint& pos ,
                     const wxSize& size ,
                     long style ,
                     const wxString& name
                    )
 : wxWindow(parent, id, pos, size, style, name)
{
    Create(parent, id, pos, size, style, name);
}

/** constructor - call SetChild()
  * @param parent: pointer to the parent window
  * @param id: id of the new window. Set to -1 for automatic generation
  * @param child: the child window
  * @param pos: the position of the new window
  * @param size: the size of the new window
  * @param style: the style to be apply to the window
  * @param name: the name to be given to this window
  */
wxResizeCtrl::wxResizeCtrl(wxWindow *parent,
                     wxWindowID id,
                     wxWindow *child,
                     const wxPoint& pos ,
                     const wxSize& size ,
                     long style ,
                     const wxString& name
                    )
  : wxWindow(parent, id, pos, size, style, name)
{
    //constructor
    Create(parent, id, pos, size, style, name);
    SetChild(child);
}

wxResizeCtrl::~wxResizeCtrl(void)
{
    //destructor
}

/** Create the window
  * @param parent: pointer to the parent window
  * @param id: id of the new window. Set to -1 for automatic generation
  * @param pos: the position of the new window
  * @param size: the size of the new window
  * @param style: the style to be apply to the window
  * @param name: the name to be given to this window
  */
void wxResizeCtrl::Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos ,
                    const wxSize& size ,
                    long style ,
                    const wxString& name
                   )
{
    long flags;
    flags = GetWindowStyle();
    flags = flags | wxBORDER_SIMPLE;
    SetWindowStyle(flags);

    child = NULL;
    iMargin = 6;
    cBoxColour = *wxBLACK;

    bResizeTop = false;
    bResizeLeft = false;
    bResizeBottom = false;
    bResizeRight = false;
    bMoving = false;

    //connect the events
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&wxResizeCtrl::OnSize);
    Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxResizeCtrl::OnPaint);
    Connect(wxEVT_MOTION,(wxObjectEventFunction)&wxResizeCtrl::OnMouseMove);
    Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&wxResizeCtrl::OnLeftDown);
    Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxResizeCtrl::OnLeftUp);
    Connect(wxEVT_MOUSE_CAPTURE_LOST,(wxObjectEventFunction)&wxResizeCtrl::OnCaptureLost);

}

/** Set the child window
  * @param Child: a pointer to the new child window
  */
void wxResizeCtrl::SetChild(wxWindow *Child)
{
    if (child) child->Destroy();
    child = Child;
    if (child) child->Reparent(this);
    SetChildSize(GetSize());
    Refresh(true,NULL);
    Update();
}

/** Get the child window
  * @return : a pointer to the child window
  */
wxWindow* wxResizeCtrl::GetChild(void)
{
    return(child);
}

/** Set the size of the resizing border, in pixel
  * @param iNewMargin: the new margin, in pixel
  */
void wxResizeCtrl::SetMargin(int iNewMargin)
{
    iMargin = iNewMargin;
    if (iMargin < 1) iMargin = 1;
    SetChildSize(GetClientSize());
    Refresh(true,NULL);
    Update();
}

/** Get the size of the resizing border, in pixel
  * @return : the margin, in pixel
  */
int wxResizeCtrl::GetMargin(void)
{
    return(iMargin);
}

/** Set the colour of the resizing border
  * @param cColour: the new olour of the resizing border
  */
void wxResizeCtrl::SetColour(wxColour cColour)
{
    cBoxColour = cColour;
    Refresh(true,NULL);
    Update();
}

/** Set the colour of the resizing border
  * @param cColour: the new olour of the resizing border
  */
wxColour wxResizeCtrl::GetColour(void)
{
    return(cBoxColour);
}

/** compute the new size and pos of the child, and resize / move it
  * @param s: the size of the wxResizeCtrl client area
  */
void wxResizeCtrl::SetChildSize(wxSize s)
{
    //compute the new size and pos of the child, and resize / move it
    int iPosX, iPosY, iWidth, iHeight;

    wxSize sBorder;
    sBorder = GetWindowBorderSize();
    iPosX = iMargin;
    iPosY = iMargin;
    iWidth = s.GetWidth() - 2 * iMargin - sBorder.x;
    iHeight = s.GetHeight() - 2 * iMargin- sBorder.y;

    if (child) child->SetSize(iPosX, iPosY, iWidth, iHeight);
}

//------------ EVENT HANDLERS ------------------

/** Size event handler
  */
void wxResizeCtrl::OnSize(wxSizeEvent& event)
{
    SetChildSize(event.GetSize());
    event.Skip();
}

/** Paint event handler
  */
void wxResizeCtrl::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    int iWidth, iHeight;

    GetClientSize(&iWidth, &iHeight);

    wxPen pen(cBoxColour, 1, wxSOLID);
    wxBrush brush(cBoxColour, wxSOLID);
    dc.SetBrush(brush);
    dc.SetPen(pen);

    //draw the 4 corner boxes
    dc.DrawRectangle(0,0,iMargin, iMargin); //top left corner box
    dc.DrawRectangle(iWidth - iMargin,0,iMargin, iMargin); //top right corner box
    dc.DrawRectangle(0,iHeight - iMargin,iMargin, iMargin); //bottom left corner box
    dc.DrawRectangle(iWidth - iMargin,iHeight - iMargin,iMargin, iMargin); //bottom right corner box

    //draw the 4 middle boxes
    dc.DrawRectangle((iWidth - iMargin) / 2,0, iMargin, iMargin); //top middle box
    dc.DrawRectangle((iWidth - iMargin) / 2,iHeight - iMargin, iMargin, iMargin); //bottom middle box
    dc.DrawRectangle(0, (iHeight - iMargin) / 2, iMargin, iMargin); //left middle box
    dc.DrawRectangle(iWidth - iMargin, (iHeight - iMargin) / 2, iMargin, iMargin); //right middle box

    //draw the rectangle
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(iMargin / 2, iMargin / 2, iWidth - iMargin, iHeight - iMargin);
}

/** Mouse events handler
  */
void wxResizeCtrl::OnMouseMove(wxMouseEvent& event)
{
    if (event.Dragging())
    {
        //Resize or move event

        if (bMoving)
        {
            //move the window
            wxPoint ScreenPos;
            wxPoint RelativePos;
            wxWindow *parent;

            //get the mouse position on screen
            ScreenPos = ::wxGetMousePosition();
            parent = GetParent();

            //the coordinate passed to Move() are relative to the parent
            //we need to convert the Screen coordinate to Client coordinate
            if (!parent) RelativePos = ScreenPos; else RelativePos = parent->ScreenToClient(ScreenPos);
            Move(RelativePos.x - x, RelativePos.y - y);
            Refresh(true, NULL);
            Update();
        }
        else if ((bResizeBottom) || (bResizeTop) || (bResizeLeft) || (bResizeRight))
        {
            //compute the new screen rectangle for the window
            wxPoint RelativePos;
            int iWidth, iHeight, iTop, iLeft;

            RelativePos = event.GetPosition();
            GetSize(&iWidth, &iHeight);
            GetPosition(&iLeft, &iTop);

            if (bResizeLeft)
            {
                iLeft = iLeft + RelativePos.x;
                iWidth = iWidth - RelativePos.x;
            }
            else if (bResizeRight)
            {
                iWidth = RelativePos.x;
            }

            if (bResizeTop)
            {
                iTop = iTop + RelativePos.y;
                iHeight = iHeight - RelativePos.y;
            }
            else if (bResizeBottom)
            {
                iHeight = RelativePos.y;
            }


            //Resize & Move the window
            Move(iLeft, iTop);
            SetSize(iWidth, iHeight);
            Refresh(true, NULL);
            Update();
        }

    }
    else
    {
        //set the cursor
        wxPoint pos;
        int iWidth, iHeight;

        pos = event.GetPosition();
        GetClientSize(&iWidth, &iHeight);
        x = pos.x;
        y = pos.y;

        if ((pos.x >= 0) && (pos.x <= iMargin))
        {
            //left border
            if ((pos.y >= 0) && (pos.y <= iMargin))
            {
                //top left corner
                SetCursor(wxCURSOR_SIZENWSE);
            }
            else if ((pos.y >= iHeight - iMargin) && (pos.y <= iHeight))
            {
                //bottom left corner
                SetCursor(wxCURSOR_SIZENESW);
            }
            else if ((pos.y >= (iHeight - iMargin) / 2) && (pos.y <= (iHeight + iMargin) / 2))
            {
                //middle of left border
                SetCursor(wxCURSOR_SIZEWE);
            }
            else
            {
                //move cursor
                SetCursor(wxCURSOR_HAND);
            }
        }
        else if ((pos.x >= (iWidth - iMargin) / 2) && (pos.x <= (iWidth + iMargin) / 2))
        {
             if ((pos.y >= 0) && (pos.y <= iMargin))
             {
                 //top middle box
                 SetCursor(wxCURSOR_SIZENS);
             }
             else if ((pos.y >= iHeight - iMargin) && (pos.y <= iHeight))
             {
                 //bottom middle box
                 SetCursor(wxCURSOR_SIZENS);
             }
             else
            {
                //move cursor
                SetCursor(wxCURSOR_HAND);
            }
        }
        else if ((pos.x >= iWidth - iMargin) && (pos.x <= iWidth))
        {
            //right border
            if ((pos.y >= 0) && (pos.y <= iMargin))
            {
                //top right corner
                SetCursor(wxCURSOR_SIZENESW);
            }
            else if ((pos.y >= iHeight - iMargin) && (pos.y <= iHeight))
            {
                //bottom right corner
                SetCursor(wxCURSOR_SIZENWSE);
            }
            else if ((pos.y >= (iHeight - iMargin) / 2) && (pos.y <= (iHeight + iMargin) / 2))
            {
                //middle of right border
                SetCursor(wxCURSOR_SIZEWE);
            }
            else
            {
                //move cursor
                SetCursor(wxCURSOR_HAND);
            }
        }
        else
        {
            //move cursor
            SetCursor(wxCURSOR_HAND);
        }
    }

    event.Skip();
}

/** Mouse left button down event
  */
void wxResizeCtrl::OnLeftDown(wxMouseEvent& event)
{

    wxPoint pos;
    int iWidth, iHeight;

    pos = event.GetPosition();
    GetClientSize(&iWidth, &iHeight);
    CaptureMouse();

    if ((pos.x >= 0) && (pos.x <= iMargin))
    {
        //left border
        if ((pos.y >= 0) && (pos.y <= iMargin))
        {
            //top left corner
            bResizeTop = true;
            bResizeLeft = true;
            bResizeBottom = false;
            bResizeRight = false;
            bMoving = false;
        }
        else if ((pos.y >= iHeight - iMargin) && (pos.y <= iHeight))
        {
            //bottom left corner
            bResizeTop = false;
            bResizeLeft = true;
            bResizeBottom = true;
            bResizeRight = false;
            bMoving = false;
        }
        else if ((pos.y >= (iHeight - iMargin) / 2) && (pos.y <= (iHeight + iMargin) / 2))
        {
            //middle of left border
            bResizeTop = false;
            bResizeLeft = true;
            bResizeBottom = false;
            bResizeRight = false;
            bMoving = false;
        }
        else
        {
            //move event
            bResizeTop = false;
            bResizeLeft = false;
            bResizeBottom = false;
            bResizeRight = false;
            bMoving = true;
        }
    }
    else if ((pos.x >= (iWidth - iMargin) / 2) && (pos.x <= (iWidth + iMargin) / 2))
    {
        if ((pos.y >= 0) && (pos.y <= iMargin))
        {
            //top middle box
            bResizeTop = true;
            bResizeLeft = false;
            bResizeBottom = false;
            bResizeRight = false;
            bMoving = false;
        }
        else if ((pos.y >= iHeight - iMargin) && (pos.y <= iHeight))
        {
            //bottom middle box
            bResizeTop = false;
            bResizeLeft = false;
            bResizeBottom = true;
            bResizeRight = false;
            bMoving = false;
        }
        else
        {
            //move event
            bResizeTop = false;
            bResizeLeft = false;
            bResizeBottom = false;
            bResizeRight = false;
            bMoving = true;
        }
    }
    else if ((pos.x >= iWidth - iMargin) && (pos.x <= iWidth))
    {
        //right border
        if ((pos.y >= 0) && (pos.y <= iMargin))
        {
            //top right corner
            bResizeTop = true;
            bResizeLeft = false;
            bResizeBottom = false;
            bResizeRight = true;
            bMoving = false;
        }
        else if ((pos.y >= iHeight - iMargin) && (pos.y <= iHeight))
        {
            //bottom right corner
            bResizeTop = false;
            bResizeLeft = false;
            bResizeBottom = true;
            bResizeRight = true;
            bMoving = false;
        }
        else if ((pos.y >= (iHeight - iMargin) / 2) && (pos.y <= (iHeight + iMargin) / 2))
        {
            //middle of right border
            bResizeTop = false;
            bResizeLeft = false;
            bResizeBottom = false;
            bResizeRight = true;
            bMoving = false;
        }
        else
        {
            //move event
            bResizeTop = false;
            bResizeLeft = false;
            bResizeBottom = false;
            bResizeRight = false;
            bMoving = true;
        }
    }
    else
    {
        //move event
        bResizeTop = false;
        bResizeLeft = false;
        bResizeBottom = false;
        bResizeRight = false;
        bMoving = true;
    }


    event.Skip();
}

/** Mouse left button up event  */
void wxResizeCtrl::OnLeftUp(wxMouseEvent& event)
{
    ReleaseMouse();

    bResizeTop = false;
    bResizeLeft = false;
    bResizeBottom = false;
    bResizeRight = false;
    bMoving = false;

    event.Skip();
}

/** mouse capture lost event
  */
void wxResizeCtrl::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    bResizeTop = false;
    bResizeLeft = false;
    bResizeBottom = false;
    bResizeRight = false;
    bMoving = false;

    event.Skip();
}
