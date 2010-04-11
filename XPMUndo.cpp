/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMUndo.cpp
 * Purpose:   manages the Undo / Redo buffer - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMEditor.h"
#include <wx/gdicmn.h>

/** Constructor
  */
XPMUndo::XPMUndo()
{
    //ctor
    lMaxUndoRedo = 100;
    UndoBuffer = NULL;
    RedoBuffer = NULL;

    lUndoSize = 0; //The current amount of Undo actions stored
    lUndoMax = 20; //The memory size of Undo actions array
    lRedoSize = 0; //The current amount of Redo actions stored
    lRedoMax = 20; //The memory size of Redo actions array

    //memory allocation
    UndoBuffer = (xpm_undo_action*) malloc(lUndoMax * sizeof(xpm_undo_action));
    RedoBuffer = (xpm_undo_action*) malloc(lRedoMax * sizeof(xpm_undo_action));
    if (!UndoBuffer) lUndoMax = 0;
    if (!RedoBuffer) lRedoMax = 0;

    parent = NULL;

}

/** Destructor
  */
XPMUndo::~XPMUndo()
{
    //dtor
    ReleaseUndoBuffer();
    ReleaseRedoBuffer();
}

//---- Parent panel -----
/** Set the parent panel for the Undo Buffer
  * @param pPanel: a pointer to the new parent
  */
void XPMUndo::SetParentPanel(XPMEditorPanel *pPanel)
{
   //Set the parent panel for the Undo Buffer
    parent = pPanel;
}


/** Get the parent panel for the Undo Buffer
  * @return: a pointer to the parent
  */
XPMEditorPanel* XPMUndo::GetParentPanel(void)
{
    //Get the parent panel for the Undo Buffer
    return(parent);
}

//--- Memory management methods ------

/** Set the maximal number of Undo & Redo actions
  * a negative value indicates an unlimited Undo Redo buffer
  * Previous Undo / Redo actions are not deleted.
  * @param iSize : the new limit
  */
void XPMUndo::SetMaxUndoSize(long lSize)
{
    //set the maximal number of Undo actions. -1 indicates unlimited.
    lMaxUndoRedo = lSize;
}

/** Get the maximal number of Undo & Redo actions
  * a negative value indicates an unlimited Undo Redo buffer
  * @return : the limit
  */
long XPMUndo::GetMaxUndoSize(void)
{
    //Get the maximal number of Undo actions. -1 indicates unlimited.
    return(lMaxUndoRedo);
}

/** Increase the size of the Undo buffer, if needed
  * If memory needs to be increased, memory allocation occurs
  * If the buffer has already reached the maximum size set by SetMaxUndoSize(), then
  * the oldest Undo action is released.
  * @return true on success, false on failure
  */
bool XPMUndo::IncreaseMemoryUndo(void)
{
    //Increase the size of the Undo buffer, if needed
    if (!UndoBuffer)
    {
        lUndoSize = 0;
        UndoBuffer = (xpm_undo_action *) malloc(20 * sizeof(xpm_undo_action));
        if (!UndoBuffer)
        {
            lUndoMax = 0;
            return(false);
        }
        lUndoMax = 20;
        return(true);
    }

    if (lUndoSize + 1 < lUndoMax) return(true); //no need to allocate more memory

    //new memory needs to be allocated.
    //2 cases: the buffer has already reached the maximal size
    //         or the max size has not be already reached
    if ((lUndoMax >= lMaxUndoRedo) && (lMaxUndoRedo > 0))
    {
        //the buffer has already reached the maximal size
        //release the 1st Undo action
        ReleaseUndoRedoAction(0, 0);
        //shift the buffer to the left
        long i;
        for(i=0;i<lUndoSize-1;i++) UndoBuffer[i] = UndoBuffer[i+1];
        lUndoSize = lUndoSize - 1;
        return(true);
    }
    else
    {
        //the max size has not be already reached
        //increase the memory
        long lMax;
        lMax = lUndoMax + 20;
        if ((lMax > lMaxUndoRedo) && (lMaxUndoRedo > 0)) lMax = lMaxUndoRedo;
        UndoBuffer = (xpm_undo_action *) realloc(UndoBuffer, lMax * sizeof(xpm_undo_action));
        if (!UndoBuffer)
        {
            lUndoMax = 0;
            return(false);
        }
        lUndoMax = lMax;
        return(true);
    }

    return(false);
}

/** Increase the size of the Redo buffer, if needed
  * If memory needs to be increased, memory allocation occurs
  * If the buffer has already reached the maximum size set by SetMaxUndoSize(), then
  * the oldest Redo action is released.
  * @return true on success, false on failure
  */
bool XPMUndo::IncreaseMemoryRedo(void)
{
    //Increase the size of the Redo buffer, if needed
    if (!RedoBuffer)
    {
        lRedoSize = 0;
        RedoBuffer = (xpm_undo_action *) malloc(20 * sizeof(xpm_undo_action));
        if (!RedoBuffer)
        {
            lRedoMax = 0;
            return(false);
        }
        lRedoMax = 20;
        return(true);
    }

    if (lRedoSize + 1 < lRedoMax) return(true); //no need to allocate more memory

    //new memory needs to be allocated.
    //2 cases: the buffer has already reached the maximal size
    //         or the max size has not be already reached
    if ((lRedoMax >= lMaxUndoRedo) && (lMaxUndoRedo > 0))
    {
        //the buffer has already reached the maximal size
        //release the 1st Redo action
        ReleaseUndoRedoAction(1, 0);
        //shift the buffer to the left
        long i;
        for(i=0;i<lRedoSize-1;i++) RedoBuffer[i] = RedoBuffer[i+1];
        lRedoSize = lRedoSize - 1;
        return(true);
    }
    else
    {
        //the max size has not be already reached
        //increase the memory
        long lMax;
        lMax = lRedoMax + 20;
        if ((lMax > lMaxUndoRedo) && (lMaxUndoRedo > 0)) lMax = lMaxUndoRedo;
        RedoBuffer = (xpm_undo_action *) realloc(RedoBuffer, lMax * sizeof(xpm_undo_action));
        if (!RedoBuffer)
        {
            lRedoMax = 0;
            return(false);
        }
        lRedoMax = lMax;
        return(true);
    }

    return(false);
}

/** Delete all the Undo actions
  * The memory buffer is preserved
  */
void XPMUndo::ClearUndoBuffer(void)
{
    //Delete all the Undo actions
    long n;
    for(n=0;n<lUndoSize;n++)
    {
        ReleaseUndoRedoAction(0, n);
    }
    lUndoSize = 0;
}

/** Delete all the Redo actions
  * The memory buffer is preserved
  */
void XPMUndo::ClearRedoBuffer(void)
{
    //Delete all the Redo actions
    long n;
    for(n=0;n<lRedoSize;n++)
    {
        ReleaseUndoRedoAction(1, n);
    }
    lRedoSize = 0;
}

/** Release a specific Undo / Redo action
  * @param iUndoRedo: 0 for release an Undo action, 1 for releasing a Redo action
  * @param lIndex: the index of the action to be released
  */
void XPMUndo::ReleaseUndoRedoAction(int iUndoRedo, long lIndex)
{
    //Release a specific Undo / Redo action
    xpm_undo_action *buffer;
    long lMax, lSize;

    buffer = NULL;
    lMax = 0;
    lSize = 0;

    if (iUndoRedo == 0)
    {
        buffer = UndoBuffer;
        lMax = lUndoMax;
        lSize = lUndoSize;
    }
    else if (iUndoRedo == 1)
    {
        buffer = RedoBuffer;
        lMax = lRedoMax;
        lSize = lRedoSize;
    }

    if (!buffer) return;
    if (lIndex < 0) return;
    if (lIndex >= lSize) return;


    switch(buffer[lIndex].UndoType)
    {
        case xpm_bitmap_undo : if (buffer[lIndex].pData) free(buffer[lIndex].pData);
                               break;
        case xpm_image_undo :  if (buffer[lIndex].pData) free(buffer[lIndex].pData);
                               break;
        case xpm_size_undo :   if (buffer[lIndex].pData) free(buffer[lIndex].pData);
                               break;
        default: break;
    }
}

/** Delete all the Undo actions
  * Then release the memory used by the Undo buffer
  */
void XPMUndo::ReleaseUndoBuffer(void)
{
    //Delete all the Undo actions and release the memory used by the Undo buffer
    ClearUndoBuffer();
    if (UndoBuffer) free(UndoBuffer);
}

/** Delete all the Redo actions
  * Then release the memory used by the Redo buffer
  */
void XPMUndo::ReleaseRedoBuffer(void)
{
    //Delete all the Redo actions and release the memory used by the Redo buffer
    ClearRedoBuffer();
    if (RedoBuffer) free(RedoBuffer);
}

//----- Undo & Redo methods --------------------
/** return true if it is possible to Undo something, false otherwise
  * @return true if Undo is possible, false otherwise
  */
bool XPMUndo::CanUndo(void)
{
    //return true if it is possible to Undo something, false otherwise
    if ((UndoBuffer) &&(lUndoSize > 0)) return(true);
    return(false);
}

/** return true if it is possible to Redo something, false otherwise
  * @return true if Redo is possible, false otherwise
  */
bool XPMUndo::CanRedo(void)
{
    //return true if it is possible to Redo something, false otherwise
    if ((RedoBuffer) &&(lRedoSize > 0)) return(true);
    return(false);
}

/** Add 1 Undo action to the buffer
  * @param iType: the type of Undo action
  * @param pData: the data associated with the Undo action
  * @param bClearRedo : true if the RedoBuffer should be cleared
  * @return true on success, false on failure
  */
bool XPMUndo::AddUndo(Undo_Action_Type iType, void *pData, bool bClearRedo)
{
    //Add 1 Undo action
    if (!IncreaseMemoryUndo()) return(false);
    if (bClearRedo) ClearRedoBuffer(); //if a new action needs to be stored in UNDO, all REDOs actions are irrelevant.

    long lIndex;
    wxRect r;
    wxBitmap *OldBitmap;
    wxBitmap *pStoredBitmap;
    wxBitmap NewBitmap;
    wxImage *OldImage;
    wxImage NewImage;
    wxImage *pStoredImage;

    lIndex = lUndoSize;

    switch(iType)
    {
        case xpm_bitmap_undo:   if (!pData) return(false);
                                UndoBuffer[lIndex].UndoType = iType;

                                OldBitmap = (wxBitmap *) pData;
                                r.SetX(0);
                                r.SetY(0);
                                r.SetWidth(OldBitmap->GetWidth());
                                r.SetHeight(OldBitmap->GetHeight());
                                NewBitmap = OldBitmap->GetSubBitmap(r);

                                pStoredBitmap = new wxBitmap(NewBitmap);
                                if (!pStoredBitmap)
                                {
                                    UndoBuffer[lIndex].UndoType = xpm_undefined_undo;
                                    UndoBuffer[lIndex].pData = NULL;
                                    return(false);
                                }
                                UndoBuffer[lIndex].pData = pStoredBitmap;

                                lUndoSize++;
                                break;

        case xpm_size_undo:
        case xpm_image_undo:    if (!pData) return(false);
                                UndoBuffer[lIndex].UndoType = iType;

                                OldImage = (wxImage *) pData;
                                if (!OldImage->IsOk()) return(false);
                                NewImage = OldImage->Copy();

                                pStoredImage = new wxImage(NewImage);
                                if (!pStoredImage)
                                {
                                    UndoBuffer[lIndex].UndoType = xpm_undefined_undo;
                                    UndoBuffer[lIndex].pData = NULL;
                                    return(false);
                                }
                                UndoBuffer[lIndex].pData = pStoredImage;

                                lUndoSize++;
                                break;

        default:    UndoBuffer[lIndex].UndoType = xpm_undefined_undo;
                    UndoBuffer[lIndex].pData = NULL;
                    break;
    }

    return(true);
}

/** Add 1 Redo action to the buffer
  * @param iType: the type of Redo action
  * @param pData: the data associated with the Undo action
  * @return true on success, false on failure
  */
bool XPMUndo::AddRedo(Undo_Action_Type iType, void *pData)
{
    //Add 1 Redo action
    if (!IncreaseMemoryRedo()) return(false);

    long lIndex;
    wxRect r;
    wxBitmap *OldBitmap;
    wxBitmap *pStoredBitmap;
    wxBitmap NewBitmap;
    wxImage *OldImage;
    wxImage NewImage;
    wxImage *pStoredImage;

    lIndex = lRedoSize;

    switch(iType)
    {
        case xpm_bitmap_undo:   if (!pData) return(false);
                                RedoBuffer[lIndex].UndoType = iType;

                                OldBitmap = (wxBitmap *) pData;
                                r.SetX(0);
                                r.SetY(0);
                                r.SetWidth(OldBitmap->GetWidth());
                                r.SetHeight(OldBitmap->GetHeight());
                                NewBitmap = OldBitmap->GetSubBitmap(r);

                                pStoredBitmap = new wxBitmap(NewBitmap);
                                if (!pStoredBitmap)
                                {
                                    RedoBuffer[lIndex].UndoType = xpm_undefined_undo;
                                    RedoBuffer[lIndex].pData = NULL;
                                    return(false);
                                }
                                RedoBuffer[lIndex].pData = pStoredBitmap;

                                lRedoSize++;
                                break;

        case xpm_size_undo:
        case xpm_image_undo:    if (!pData) return(false);
                                RedoBuffer[lIndex].UndoType = iType;

                                OldImage = (wxImage *) pData;
                                if (!OldImage->IsOk()) return(false);
                                NewImage = OldImage->Copy();

                                pStoredImage = new wxImage(NewImage);
                                if (!pStoredImage)
                                {
                                    RedoBuffer[lIndex].UndoType = xpm_undefined_undo;
                                    RedoBuffer[lIndex].pData = NULL;
                                    return(false);
                                }
                                RedoBuffer[lIndex].pData = pStoredImage;

                                lRedoSize++;
                                break;

        default:    RedoBuffer[lIndex].UndoType = xpm_undefined_undo;
                    RedoBuffer[lIndex].pData = NULL;
                    break;
    }

    return(true);
}

/** Performs an Undo operation
  * @return no return value
  */
void XPMUndo::Undo(void)
{
    //performs 1 Undo operation

    if (!CanUndo()) return;

    //add 1 REDO operation
    parent = GetParentPanel();
    if (!parent) return;
    wxImage *imgCurrent;
    imgCurrent = parent->GetImage();
    if (!AddRedo(xpm_image_undo, imgCurrent)) return;

    //performs the UNDO action
    PerformAction(parent, UndoBuffer, lUndoSize - 1);
    ReleaseUndoRedoAction(0, lUndoSize - 1);
    if (lUndoSize > 0) lUndoSize--;

}

/** Performs an Redo operation
  * @return no return value
  */
void XPMUndo::Redo(void)
{
    //performs 1 Redo operation
    if (!CanRedo()) return;


    //add 1 UNDO operation
    XPMEditorPanel *parent;
    parent = GetParentPanel();
    if (!parent) return;
    wxImage *imgCurrent;
    imgCurrent = parent->GetImage();
    if (!AddUndo(xpm_image_undo, imgCurrent, false)) return;

    //performs the REDO action
    PerformAction(parent, RedoBuffer, lRedoSize - 1);
    ReleaseUndoRedoAction(1, lRedoSize - 1);
    if (lRedoSize > 0) lRedoSize--;

}

/** Do the Undo / Redo action
  * @param parent: the XPMEditorPanel on which the action is performed
  * @param buffer: the Undo / Redo buffer
  * @param lIndex: the index of the action to perform
  */
void XPMUndo::PerformAction(XPMEditorPanel *parent, xpm_undo_action *buffer, long lIndex)
{
    //perform the Undo / Redo action
    if (!parent) {return;}
    if (!buffer) {return;}
    if (lIndex < 0) {return;}

    switch (buffer[lIndex].UndoType)
    {
        case xpm_bitmap_undo :  if (buffer[lIndex].pData)
                                {
                                    wxBitmap *bm;
                                    bm = (wxBitmap *) buffer[lIndex].pData;
                                    if (!bm) break;
                                    wxImage img;
                                    img = bm->ConvertToImage();
                                    parent->SetImage(&img);
                                }
                                break;

        case xpm_size_undo :
        case xpm_image_undo:
                                if (buffer[lIndex].pData)
                                {
                                    wxImage *img;
                                    img = (wxImage *) buffer[lIndex].pData;
                                    parent->SetImage(img);
                                }
                                break;

        default:                break;
    }
}
