/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditorBase.cpp
 * Purpose:   the base class for the Image editor - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include <sdk.h> // Code::Blocks SDK
#include <filefilters.h>
#include <wx/msgdlg.h>
#include <wx/gdicmn.h>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/clipbrd.h>
#include <wx/dcclient.h>
#include <wx/aui/auibook.h>
#include <wx/dcclient.h>

#include "XPMEditor.h"

//---------------- CONSTRUCTORS AND DESTRUCTORS ---------------------------

XPMEditorBase::EditorsSet XPMEditorBase::m_AllEditors;

/** Constructor
  * \param parent    a pointer to the parent window
  * \param title     the title which will appear in the tab
  * \param bitmap    a pointer to the Bitmap to display. Can be NULL
  * \param sFileName the filename associated to the editor. Can be empty
  */
XPMEditorBase::XPMEditorBase(wxWindow* parent, const wxString& title,wxImage *img, wxString sFileName, wxBitmapType bt)
    :EditorBase(parent,title)
{
    //constructor
    Freeze();

    //initialisation code
    m_bModified = false;
    m_bReadOnly = false;
    SetFilename(sFileName);
    if (m_Filename == wxEmptyString) m_bIsFileNameOK = false; else m_bIsFileNameOK = true;

    m_DrawArea = new XPMEditorPanel(this);

    //project file
    m_pProjectFile = NULL;

    //sizing & refreshing
    wxSize s;
    s = GetClientSize();
    m_DrawArea->SetSize(0,0,s.GetWidth(),s.GetHeight(), 0);
    Refresh();
    Update();

    Connect(wxEVT_SIZE,(wxObjectEventFunction)&XPMEditorBase::OnResize);

    //editors set
    m_AllEditors.insert( this );

    //get the configuration
    UpdateConfiguration();
    if (m_DrawArea)
    {
        m_DrawArea->SetImage(img);
        m_DrawArea->SetImageFormat(bt); //default behaviour : autodetection using file extension
    }

    Thaw();

}

/** Destructor
  */
XPMEditorBase::~XPMEditorBase()
{
    //destructor
    m_AllEditors.erase(this);
    //m_ImageFileName = wxString("",0);

}

/** Static method. Indicate if the editor opened is an image editor
  * \param a pointer to the editor to test
  * \return true if the editor is an Image Editor, false otherwise
  */
bool XPMEditorBase::IsImgEditor(EditorBase* editor)
{
    return m_AllEditors.find( editor ) != m_AllEditors.end();
}

/** Static method. Close all the opened Image Editors.
  */
void XPMEditorBase::CloseAllEditors(void)
{
    EditorsSet s = m_AllEditors;
    for ( EditorsSet::iterator i = s.begin(); i != s.end(); ++i )
    {
        if (*i)
        {
            EditorManager::Get()->QueryClose( *i );
            (*i)->Close();
        }
    }
    assert( m_AllEditors.empty() );
}

/** Set the panel to be used by the editor
  * \param Panel: a pointer to the wxPanel to use for this editor
  */
void XPMEditorBase::SetPanel(wxPanel *Panel)
{
    //Set the panel to be used by the editor
    if (Panel) m_DrawArea = (XPMEditorPanel*) Panel;
}

/** Get the panel used by the editor
  * \return Panel: a pointer to the wxPanel used by editor
  */
wxPanel* XPMEditorBase::GetPanel(void)
{
    //Get the panel used by the editor
    return(m_DrawArea);
}

//---------------- Save, Close, Readonly, is modified ---------------------------
/** Return true if the image was modified, false otherwise
  * \return true if the image was modified, false otherwise
  */

bool XPMEditorBase::GetModified(void) const
{
    //Returns true if image is modified, false otherwise
    return(m_bModified);
}

/** Set the modification flag
  * \return no return value
  */
void XPMEditorBase::SetModified(bool bModified)
{
    //set modification flag
    m_bModified = bModified;
    if (m_pProjectFile)
    {
        if (m_bReadOnly)
        {
            m_pProjectFile->SetFileState(fvsReadOnly);
        }
        else if (m_bModified)
        {
            m_pProjectFile->SetFileState(fvsModified);
            UpdateModified();
        }
        else
        {
            m_pProjectFile->SetFileState(fvsNormal);
            UpdateModified();
        }

   }
   else
   {
       UpdateModified();
   }
}

/** Update the Window title with a '*' char if the file is modified
  */
void XPMEditorBase::UpdateModified(void)
{
    if (GetModified())
    {
        SetTitle(_T("*") + GetShortName());
    }
    else
    {
        SetTitle(GetShortName());
    }
}

/** Post application wide events
  * \return true if the image was ReadOnly, false otherwise
  */
void XPMEditorBase::NotifyPlugins(wxEventType type, int intArg, const wxString& strArg, int xArg, int yArg)
{
    //post application wide events
    if (!Manager::Get()->GetPluginManager())
        return; // no plugin manager! app shuting down?
    CodeBlocksEvent event(type);
    event.SetEditor(this);
    event.SetInt(intArg);
    event.SetString(strArg);
    event.SetX(xArg);
    event.SetY(yArg);
    //wxPostEvent(Manager::Get()->GetAppWindow(), event);
    Manager::Get()->GetPluginManager()->NotifyPlugins(event);
}

/** Return true if the image is ReadOnly, false otherwise
  * \return true if the image was ReadOnly, false otherwise
  */
bool XPMEditorBase::IsReadOnly(void) const
{
    //indicates if the image is read-only
    return(m_bReadOnly);
}

/** Return true if the image is ReadOnly, false otherwise
  * \return true if the image was ReadOnly, false otherwise
  */
bool XPMEditorBase::GetReadOnly(void) const
{
    //indicates if the image is read-only
    return(m_bReadOnly);
}

/** Set the modification flag
  * \return no return value
  */
void XPMEditorBase::SetReadOnly(bool bReadOnly)
{
    //set read-only status
    m_bReadOnly = bReadOnly;
}

/** Save the image under a new name
  * Set the modification flag
  * \return true on success, false on failure
  */
bool XPMEditorBase::SaveAs(void)
{
    wxFileName fname;
    fname.Assign(m_Filename);
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("app"));
    int StoredIndex = 0;
    wxString Filters = FileFilters::GetFilterString();
    wxString Path = fname.GetPath();
    wxString Extension = fname.GetExt();
    wxString Filter;
    if (!Extension.IsEmpty())
    {    // use the current extension as the filter
        // Select filter belonging to this file type:
        Extension.Prepend(_T("."));
        Filter = FileFilters::GetFilterString(Extension);
    }
    else if(mgr)
    {
        // File type is unknown. Select the last used filter:
        Filter = mgr->Read(_T("/file_dialogs/save_file_as/filter"), _T("bitmap files"));
    }
    if(!Filter.IsEmpty())
    {
        // We found a filter, look up its index:
        int sep = Filter.find(_T("|"));
        if (sep != wxNOT_FOUND)
        {
            Filter.Truncate(sep);
        }
        if (!Filter.IsEmpty())
        {
            FileFilters::GetFilterIndexFromName(Filters, Filter, StoredIndex);
        }
    }
    if(mgr && Path.IsEmpty())
    {
        Path = mgr->Read(_T("/file_dialogs/save_file_as/directory"), Path);
    }
    wxFileDialog dlg(Manager::Get()->GetAppWindow(),
                                         _("Save file"),
                                         Path,
                                         fname.GetFullName(),
                                         Filters,
                                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dlg.SetFilterIndex(StoredIndex);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
    {  // cancelled out
        return(false);
    }
    m_Filename = dlg.GetPath();
    Manager::Get()->GetLogManager()->Log(m_Filename);
    fname.Assign(m_Filename);
    m_Shortname = fname.GetFullName();
    SetTitle(m_Shortname);
    // invalidate m_pProjectFile, because if kept, it would point to the ProjectFile with old name and
    // cause ProjectManager::RemoveFileFromProject called via context menu to crash
    if (m_pProjectFile) m_pProjectFile->SetFileState(fvsNormal); //so the original project file will be shown as unmodified.
    UpdateModified();
    SetProjectFile(0);
    //Manager::Get()->GetLogManager()->Log(mltDevDebug, "Filename=%s\nShort=%s", m_Filename.c_str(), m_Shortname.c_str());
    m_bIsFileNameOK = true;
    SetModified(true);
    // store the last used filter and directory
    if(mgr)
    {
        int Index = dlg.GetFilterIndex();
        wxString Filter;
        if(FileFilters::GetFilterNameFromIndex(Filters, Index, Filter))
        {
            mgr->Write(_T("/file_dialogs/save_file_as/filter"), Filter);
        }
        wxString Test = dlg.GetDirectory();
        mgr->Write(_T("/file_dialogs/save_file_as/directory"), dlg.GetDirectory());
    }
    return(Save());
}

/** Save the image
  * Set the modification flag
  * \return true on success, false on failure
  */
bool XPMEditorBase::Save(void)
{
    //save the image
    if (!GetModified()) return(true); //nothing to save


    wxImage img;
    img = GetImage();
    if (!img.IsOk()) return(false); //no bitmap to save

    wxBitmapType bt;
    if (!m_DrawArea) return(false);
    bt = m_DrawArea->GetImageFormat();

    if (!m_bIsFileNameOK) return(SaveAs());

    return(XPM_Plugin()->SaveImage(&img, m_Filename, bt, this));
}

/** Close the image
  * \return true on success, false on failure
  */
bool XPMEditorBase::Close(void)
{
    //Close the image
    Destroy();
    return(true);
}

/** Set the Project file of the Image. Can be NULL
  * \param project_file pointer to the Project File. Can be NULL
  * \param preserve_modified boolean indicating if file modification flag should be preserved
  * \return no return value
  */
void XPMEditorBase::SetProjectFile(ProjectFile* project_file, bool preserve_modified)
{
    if (m_pProjectFile == project_file) return;

    bool wasmodified = false;
    if(preserve_modified) wasmodified = GetModified();

    m_pProjectFile = project_file;
    if (m_pProjectFile)
    {
        // update our filename
        m_Filename = UnixFilename(project_file->file.GetFullPath());
        m_pProjectFile->editorOpen = true;

        if (Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/tab_text_relative"), true))
            m_Shortname = m_pProjectFile->relativeToCommonTopLevelPath;
        else
            m_Shortname = m_pProjectFile->file.GetFullName();
        SetTitle(m_Shortname);

        if (!wxFileExists(m_Filename)) m_pProjectFile->SetFileState(fvsMissing);
        else if (!wxFile::Access(m_Filename.c_str(), wxFile::write)) // readonly
            m_pProjectFile->SetFileState(fvsReadOnly);
    }

    if(preserve_modified) SetModified(wasmodified);
}

//---------------- UNDO & REDO ---------------------------

/** Return true if UNDO can be done, false otherwise
  * \return true if UNDO can be done, false otherwise
  */
bool XPMEditorBase::CanUndo(void) const
{
    //check if UNDO is possible
    if (!m_DrawArea) return(false);
    return(m_DrawArea->CanUndo());
}

/** Return true if UNDO can be done, false otherwise
  * \return true if UNDO can be done, false otherwise
  */
bool XPMEditorBase::CanRedo(void) const
{
    //check if REDO is possible
    if (!m_DrawArea) return(false);
    return(m_DrawArea->CanRedo());
}

/** Clear the Undo Buffer
  * \return no return value
  */
void XPMEditorBase::ClearUndoBuffer(void)
{
    //Clear the Undo Buffer
    if (!m_DrawArea) return;
    m_DrawArea->ClearUndoBuffer();
}

/** Clear the Redo Buffer
  * \return no return value
  */
void XPMEditorBase::ClearRedoBuffer(void)
{
    //Clear the Redo Buffer
    if (!m_DrawArea) return;
    m_DrawArea->ClearRedoBuffer();
}

/** Add 1 Undo operation to the undo buffer
  * \param bm a pointer to the bitmap to add to the Undo Buffer. A copy of this bitmap is done
  * \return true on success, false otherwise
  */
bool XPMEditorBase::AddUndo(wxBitmap *bm)
{
    //add 1 UNDO operation to the buffer
    if (!m_DrawArea) return(false);
    return(m_DrawArea->AddUndo(bm));
}

/** Add 1 Undo operation to the undo buffer
  * \param img: a pointer to the image to add to the Undo Buffer. A copy of this image is done
  * \return true on success, false otherwise
  */
bool XPMEditorBase::AddUndo(wxImage *img)
{
    //add 1 UNDO operation to the buffer
    if (!m_DrawArea) return(false);
    return(m_DrawArea->AddUndo(img));
}

/** Add 1 Undo operation to the undo buffer
  * \return true on success, false otherwise
  */
bool XPMEditorBase::AddUndo(void)
{
    //add 1 Undo operation to the Buffer
    if (!m_DrawArea) return(false);
    return(m_DrawArea->AddUndo());
}

/** Add 1 Undo operation to the redo buffer
  * \return true on success, false otherwise
  */
bool XPMEditorBase::AddRedo(void)
{
    //add 1 Redo operation to the Buffer
    if (!m_DrawArea) return(false);
    return(m_DrawArea->AddRedo());
}

/** Add 1 Redo operation to the redo buffer
  * \param bm a pointer to the bitmap to add to the Redo Buffer. A copy of this bitmap is done
  * \return true on success, false otherwise
  */
bool XPMEditorBase::AddRedo(wxBitmap *bm)
{
    //add 1 REDO operation to the buffer
    if (!m_DrawArea) return(false);
    return(m_DrawArea->AddRedo(bm));
}

/** Add 1 Redo operation to the redo buffer
  * \param img: a pointer to the image to add to the Redo Buffer. A copy of this image is done
  * \return true on success, false otherwise
  */
bool XPMEditorBase::AddRedo(wxImage *img)
{
    //add 1 REDO operation to the buffer
    if (!m_DrawArea) return(false);
    return(m_DrawArea->AddRedo(img));
}

/** Performs an Undo operation
  * \return no return value
  */
void XPMEditorBase::Undo(void)
{
    //UNDO
    if (!m_DrawArea) return;
    m_DrawArea->Undo();
}

/** Performs a Redo operation
  * \return no return value
  */
void XPMEditorBase::Redo(void)
{
    //REDO
    if (!m_DrawArea) return;
    m_DrawArea->Redo();
}

//---------------- Clipboard operations ---------------------------
/** Return true if there is some Selection, false otherwise
  * \return true if something is selected, false otherwise
  */
bool XPMEditorBase::HasSelection(void) const
{
    //check if an area is selected
    if (!m_DrawArea) return(false);
    return(m_DrawArea->HasSelection());
}

/** Return true if copy is possible, false otherwise
  * \return true if copy is possible, false otherwise
  */
bool XPMEditorBase::CanCopy(void) const
{
    //indicates if Copy is possible
    return(HasSelection());
}

/** Return true if cut is possible, false otherwise
  * \return true if cut is possible, false otherwise
  */
bool XPMEditorBase::CanCut(void) const
{
    //indicates if Cut is possible
    return(HasSelection());
}

/** Return true if paste is possible, false otherwise
  * \return true if paste is possible, false otherwise
  */
bool XPMEditorBase::CanPaste(void) const
{
    //indicates if Paste is possible
    bool bResult;

    if (!(wxTheClipboard->Open())) return(false);
    bResult = wxTheClipboard->IsSupported(wxDF_BITMAP);
    wxTheClipboard->Close();

    return(bResult);
}

/** perform a "cut" clipbopard operation
  * \return no return value
  */
void XPMEditorBase::Cut(void)
{
    //CUT
    if (!m_DrawArea) return;
    m_DrawArea->Cut();
}

/** perform a "copy" clipbopard operation
  * \return no return value
  */
void XPMEditorBase::Copy(void)
{
    //COPY
    if (!m_DrawArea) return;
    m_DrawArea->Copy();
}

/** perform a "paste" clipbopard operation
  * \return no return value
  */
void XPMEditorBase::Paste(void)
{
    //PASTE
    if (!m_DrawArea) return;
    m_DrawArea->Paste();
}

/** clear the current selection
  * \return no return value
  */
void XPMEditorBase::ClearSelection(void)
{
    //clear the selection
    if (!m_DrawArea) return;
    m_DrawArea->ClearSelection();
}

/** Get the smallest bounding rectangle encompassing the selection
  * \param r the rectangle (wxRect) containing the result
  */
void XPMEditorBase::GetBoundingRect(wxRect *r)
{
    //Get the bounding rectangle of the selection
    if (!m_DrawArea) return;
    m_DrawArea->GetBoundingRect(r);
}

/** Can the editor select everything?
  *
  * \return True if the editor can select all content, false if not.
  */
bool XPMEditorBase::CanSelectAll(void) const
{
    return(true);
}

/** Select everything in the editor
  */
void XPMEditorBase::SelectAll(void)
{
    if (m_DrawArea) m_DrawArea->SelectAll();
}

/** Return a copy of the current Image
  * \return a pointer if succesfull, NULL otherwise
  */
wxImage XPMEditorBase::GetImage(void)
{
    //Getting current image
    if (!m_DrawArea) return(wxImage());
    return(m_DrawArea->GetImage());
}

/** Set the current visible Image
  * \param a pointer to the Image to set. If NULL, a default blank bitmap is created
  * \return a pointer if succesfull, NULL otherwise
  */
void XPMEditorBase::SetImage(wxImage *img)
{
    //Set current image
    if (!m_DrawArea) return;
    m_DrawArea->SetImage(img);
}

/** Get the wxScrolledWindow used to draw the bitmap
  * \return a pointer to the wxScrolledWindow if succesfull, NULL otherwise
  */
wxScrolledWindow* XPMEditorBase::GetDrawCanvas(void)
{
    //return a pointer to the ScrolledWindow where Bitmaps are drawn
    if (m_DrawArea) return(m_DrawArea->GetDrawCanvas());
    return(NULL);
}

/** wxEVT_SIZE event handler (resize event)
  * \param event: the wxSizeEvent describing the event.
  */
void XPMEditorBase::OnResize(wxSizeEvent& event)
{
    wxSize s;
    s = event.GetSize();
    m_DrawArea->SetSize(0,0,s.GetWidth(),s.GetHeight(), 0);
    event.Skip();
}

/** Update the configuration of the editor
  */
void XPMEditorBase::UpdateConfiguration(void)
{
    //Update the configuration of the editor
    if (m_DrawArea) m_DrawArea->UpdateConfiguration();
}
