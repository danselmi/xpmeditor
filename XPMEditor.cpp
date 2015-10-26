/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditor.cpp
 * Purpose:   plugin main entry point - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include <sdk.h> // Code::Blocks SDK
#include <cbproject.h>
#include <filegroupsandmasks.h>
#include <manager.h>
#include <configmanager.h>
#include <editormanager.h>
#include <logmanager.h>
#include <projectmanager.h>
#include <configurationpanel.h>
#include <filefilters.h>
#include <cbauibook.h>

#include <wx/aui/auibook.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/xrc/xmlres.h>

#include "XPMEditor.h"
#include "XPMColorPicker.h"
#include "imagxpm2.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<XPMEditor> regMain(_T("XPMEditor"));
}


const long XPMEditor::idFileNewImage = wxNewId();
const long XPMEditor::idOpenImage = wxNewId();
const long XPMEditor::idwxSmithNewImage = wxNewId();
const long XPMEditor::idOpenImageModuleMenu = wxNewId();
const long XPMEditor::idOpenImageModuleMenu2 = wxNewId();

XPMEditor* XPMEditor::m_Singleton = 0;

/** Constructor
  */
XPMEditor::XPMEditor()
{

    m_XPMEditorLogger = NULL;
    m_Singleton = NULL;

    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("XPMEditor.zip")))
    {
        NotifyMissingFile(_T("XPMEditor.zip"));
    }

    ReadConfiguration(true);
}

/** destructor
  */
XPMEditor::~XPMEditor()
{
    if (m_Singleton == this)
    {
        if (DefaultColourArray) delete[] DefaultColourArray;
        m_Singleton = NULL;
    }

}

/** Test if a file can be opened by the plugin
  * \param filename : the path to the file to be tested
  * \return true if the file can be opened by the plugin, false otherwise
  */
bool XPMEditor::CanHandleFile(const wxString& filename) const
{
   wxString fn;

   if (!XPMEditor::Get()) return(false); //plugin not loaded

   fn = filename;
   fn.MakeUpper();

   //special handling for XPM image
   if (fn.Right(4) == _(".XPM"))
   {
       //iOpenXPM==1 : 1: open the XPM in text editor. 2: ask the user. Other values: open the XPM in Image Editor.
       if (iOpenXPM == 1) return(false);
       if (iOpenXPM == 2)
       {
           wxString sMsg;

           sMsg = _("Do you want to open this file in the Image Editor ?");
           sMsg += _("\n");
           sMsg += _("If you click NO, then the file will be opened in the text editor");
           if (::wxMessageBox(sMsg, _("Question"),wxICON_QUESTION | wxYES_NO) == wxID_NO) return(false);
       }
       return(true);
   }

   //see if an handler is available
   wxBitmapType bt;

   if (GetImageFormatFromFileName(filename, &bt)) return(true);

   //try to open it directly
   wxImage img;
   if (LoadImage(&img, filename, &bt)) return(true);

   return(false);
}

/** Open a file in the editor
  * \param filename : the path to the file to open
  * \return 0 on success, 1 on failure.
  */
int XPMEditor::OpenFile(const wxString& filename)
{
    if (!XPM_Plugin()) return(1);
    if (XPM_Plugin()->OpenInEditor(filename)) return(0);

    return(1);
}

/** return always false
  */
bool XPMEditor::HandlesEverything() const
{
    return(false);
}

/** Find the path to the plugin resource files, containing the HTLM help files
  * \return the path to the resource file on success
  *         an empty string on failure
  */
wxString XPMEditor::GetHTMLHelp(void)
{
    wxString resourceFile = ConfigManager::LocateDataFile(_T("XPMEditor.zip"), sdDataGlobal | sdDataUser);

    if(wxFile::Access(resourceFile, wxFile::read) == false) return(_(""));

    return(resourceFile);
}

/** return a pointer to the unique instance of the plugin class.
  * static method
  * \return a pointer to the unique class instance on success, NULL on failure (plugin not initialized)
  */
XPMEditor* XPMEditor::Get(void)
{
    //This method return a pointer to the the unique instance of this class
    return(m_Singleton);
}

/** Plugin initialization
  */
void XPMEditor::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...

    if (m_Singleton == NULL)
    {
        const FilesGroupsAndMasks *fm = Manager::Get()->GetProjectManager()->GetFilesGroupsAndMasks();
        wxString sMask;
        if (fm) sMask = fm->GetFileMasks(0);
        wxInitAllImageHandlers();
        wxImage::RemoveHandler(wxT("XPM file"));
        wxImage::AddHandler(new wxXPMHandler2);
        wxBitmap::InitStandardHandlers();
        wxFileSystem::AddHandler(new wxZipFSHandler);

        //this piece of code works only on Windows yet. Linux crashes for unknown reasons
        #ifdef __WXMSW__
        AddFileMasksToProjectManager();
        #endif

        m_Singleton = this;
        m_FileNameSelected = _("");

        // Create a new log window.
        if(LogManager *LogMan = Manager::Get()->GetLogManager())
        {
            m_XPMEditorLogger = new XPMEditorLogger();
            m_LogPageIndex = LogMan->SetLog(m_XPMEditorLogger);
            LogMan->Slot(m_LogPageIndex).title = wxT("XPMEditor");
            CodeBlocksLogEvent evtAdd(cbEVT_ADD_LOG_WINDOW, m_XPMEditorLogger, LogMan->Slot(m_LogPageIndex).title);
            Manager::Get()->ProcessEvent(evtAdd);
        }

    }
}

/** Plugin release
  */
void XPMEditor::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be FALSE...
    if ( !appShutDown )
    {
        CloseMyEditors();
    }

    // Remove the log window.
    if(Manager::Get()->GetLogManager())
    {
        if(m_XPMEditorLogger)
        {
            CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_XPMEditorLogger);
            Manager::Get()->ProcessEvent(evt);
        }
    }
}

/** This method adds Filemasks, such as "*.bmp" to the File Open or File Save dialog
  */
void XPMEditor::AddFileMasksToFileOpenSaveDialog(wxArrayString sFileMasks)
{
    wxString sImageMasks;
    wxString sMask;
    size_t i;

    //sImageMasks = wxImage::GetImageExtWildcard();
    sImageMasks = _("");
    for(i=0;i<sFileMasks.GetCount();i++)
    {
        sMask = sFileMasks.Item(i);
        if (sImageMasks.Len() > 0) sImageMasks += _(",");
        sImageMasks += sMask;
    }

    FileFilters::Add(_("Image Files"), sImageMasks);
}

/** This method is called by Code::Blocks core modules (EditorManager,
          * ProjectManager etc) and is used by the plugin to add any menu
          * items it needs in the module's popup menu. For example, when
          * the user right-clicks on a project file in the project tree,
          * ProjectManager prepares a popup menu to display with context
          * sensitive options for that file. Before it displays this popup
          * menu, it asks all attached plugins (by asking PluginManager to call
          * this method), if they need to add any entries
          * in that menu. This method is called.\n
          * If the plugin does not need to add items in the menu,
          * just do nothing ;)
          * \param type the module that's preparing a popup menu
          * \param menu pointer to the popup menu
          * \param data pointer to FileTreeData object (to access/modify the file tree)
          */
void XPMEditor::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    if ( !menu || !IsAttached() ) return;
    wxMenu* menu2;

    switch ( type )
    {
        case mtProjectManager:

            if ( data && data->GetKind()==FileTreeData::ftdkFile )
            {
                wxMenuItem* child = menu->FindItem( menu->FindItem( _("Open with") ) );
                if ( child && child->IsSubMenu() )
                {
                    menu2 = child->GetSubMenu();
                    if (menu2) menu = menu2;
                }

                menu->AppendSeparator();
                menu->Append(idOpenImageModuleMenu, _( "XPM Editor" ), _( "Open this file in image editor" ));
                Connect(idOpenImageModuleMenu,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&XPMEditor::OnOpenFromProjectManager);
            }
            break;

        case mtUnknown: //Assuming file explorer -- fileexplorer fills the filetreedata with ftdkFile or ftdkFolder as "kind", the file/folder selected is the "FullPath" of the entry
            if(data && data->GetKind()==FileTreeData::ftdkFile)  //right clicked on folder in file explorer
            {
                wxFileName f(data->GetFolder());
                m_FileNameSelected = f.GetFullPath();
                wxMenuItem* child = menu->FindItem( menu->FindItem( _("Open with") ) );
                if ( child && child->IsSubMenu() )
                {
                    menu2 = child->GetSubMenu();
                    if (menu2) menu = menu2;
                }
                menu->Append( idOpenImageModuleMenu2, _( "Open with XPM Editor" ), _( "Open this file in image editor" ) );
                Connect(idOpenImageModuleMenu2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&XPMEditor::OnOpenFromFileManager);
            }
            break;


        default:
            break;
    }
}

/** This method is called by Code::Blocks and is used by the plugin
  * to add any menu items it needs on Code::Blocks's menu bar.\n
  * It is a pure virtual method that needs to be implemented by all
  * plugins. If the plugin does not need to add items on the menu,
  * just do nothing ;)
  * \param menuBar the wxMenuBar to create items in
  */
void XPMEditor::BuildMenu(wxMenuBar* menuBar)
{
    int idFileNewProject = XRCID("idFileNewProject");
    int idFileOpen = XRCID("idFileOpen");

    wxMenu* popup;

    if (!menuBar) return;

    //create new item in sub menu "New"
    menuBar->FindItem(idFileNewProject, &popup);
    if (popup)
    {
        popup->Append(idFileNewImage, _("New Image"), _("Create a new blank image or icon"));
        Connect(idFileNewImage,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&XPMEditor::OnNewImage);
    }

    //create a new item in menu "File"
    menuBar->FindItem(idFileOpen, &popup);
    if (popup)
    {
        //we need to insert a menu item, so we need to know its position.
        //there are no other way than iterating the whole menu to find the insertion position
        size_t pos;
        int id;
        wxMenuItemList& list = popup->GetMenuItems();
        pos = 0;
        for ( wxMenuItemList::iterator i = list.begin(); i != list.end(); ++i)
        {
            wxMenuItem* item = *i;
            if (item)
            {
                id = item->GetId();
                if (id == idFileOpen)
                {
                    popup->Insert( pos+1, idOpenImage, _("Open with XPMEditor"), _("Open file using the image editor") );
                    Connect(idOpenImage,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&XPMEditor::OnOpenImage);
                }
            }
            pos++;
        }
    }

    //create a new menu in "wxSmith"
    int idwxSmith;
    idwxSmith = menuBar->FindMenu(_("&wxSmith"));
    if (idwxSmith != wxNOT_FOUND)
    {
        //append a separator and a menu item
        wxMenu *mSmithMenu;
        mSmithMenu = menuBar->GetMenu(idwxSmith);
        if (mSmithMenu)
        {
            mSmithMenu->AppendSeparator();
            mSmithMenu->Append(idwxSmithNewImage, _("Add Image"), _("Add a new image or icon to the project"));
            Connect(idwxSmithNewImage,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&XPMEditor::OnNewProjectImage);
        }
    }
}

/** This method adds Filemasks, such as "*.bmp" to the project manager
  * This allows the display of all images into 1 virtual folder "Images"
  */
void XPMEditor::AddFileMasksToProjectManager(void)
{
    ProjectManager *pm;
    pm = Manager::Get()->GetProjectManager();

    wxArrayString sFileMasks;

    //get the list of file extension supported by wxImage
    wxList list = wxImage::GetHandlers();
    sFileMasks.Clear();

    for(wxList::compatibility_iterator node = list.GetFirst(); node; node = node->GetNext())
    {
        wxImageHandler *handler=(wxImageHandler*) node->GetData();
        wxString sExtension = _("*.") + handler->GetExtension();
        sExtension.MakeLower();
        if ((sFileMasks.Index(sExtension, false) == wxNOT_FOUND) && (handler->GetExtension().Len() > 0)) sFileMasks.Add(sExtension);
    }

    //get the list of file extension supported by wxBitmap
    wxBitmapHandler *handler;
    wxString sExtension;
    wxBitmapType bt[32];
    int i;

    bt[0] = wxBITMAP_TYPE_BMP;
    bt[1] = wxBITMAP_TYPE_BMP_RESOURCE;
    bt[2] = wxBITMAP_TYPE_RESOURCE;
    bt[3] = wxBITMAP_TYPE_ICO;
    bt[4] = wxBITMAP_TYPE_ICO_RESOURCE;
    bt[5] = wxBITMAP_TYPE_CUR;
    bt[6] = wxBITMAP_TYPE_CUR_RESOURCE;
    bt[7] = wxBITMAP_TYPE_XBM;
    bt[8] = wxBITMAP_TYPE_XBM_DATA;
    bt[9] = wxBITMAP_TYPE_XPM;
    bt[10] = wxBITMAP_TYPE_XPM_DATA;
    bt[11] = wxBITMAP_TYPE_TIF;
    bt[12] = wxBITMAP_TYPE_TIF_RESOURCE;
    bt[13] = wxBITMAP_TYPE_GIF;
    bt[14] = wxBITMAP_TYPE_GIF_RESOURCE;
    bt[15] = wxBITMAP_TYPE_PNG;
    bt[16] = wxBITMAP_TYPE_PNG_RESOURCE;
    bt[17] = wxBITMAP_TYPE_JPEG;
    bt[18] = wxBITMAP_TYPE_JPEG_RESOURCE;
    bt[19] = wxBITMAP_TYPE_PNM;
    bt[20] = wxBITMAP_TYPE_PNM_RESOURCE;
    bt[21] = wxBITMAP_TYPE_PCX;
    bt[22] = wxBITMAP_TYPE_PCX_RESOURCE;
    bt[23] = wxBITMAP_TYPE_PICT;
    bt[24] = wxBITMAP_TYPE_PICT_RESOURCE;
    bt[25] = wxBITMAP_TYPE_ICON;
    bt[26] = wxBITMAP_TYPE_ICON_RESOURCE;
    bt[27] = wxBITMAP_TYPE_ANI;
    bt[28] = wxBITMAP_TYPE_IFF;
    bt[29] = wxBITMAP_TYPE_TGA;
    bt[30] = wxBITMAP_TYPE_MACCURSOR;
    bt[31] = wxBITMAP_TYPE_MACCURSOR_RESOURCE;


    for(i=0;i<32;i++)
    {
        handler = (wxBitmapHandler*) wxBitmap::FindHandler(bt[i]);
        if (handler)
        {
            sExtension = _("*.") + handler->GetExtension();
            if ((sFileMasks.Index(sExtension, false) == wxNOT_FOUND) && (handler->GetExtension().Len() > 0)) sFileMasks.Add(sExtension);
        }
    }



    //add additionnal exensions
    if (sFileMasks.Index(_("*.dib"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.dib"));
    if (sFileMasks.Index(_("*.xbm"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.xbm"));
    if (sFileMasks.Index(_("*.iff"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.iff"));
    if (sFileMasks.Index(_("*.pic"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.pic"));
    if (sFileMasks.Index(_("*.pict"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.pict"));
    if (sFileMasks.Index(_("*.jpe"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.jpe"));
    if (sFileMasks.Index(_("*.icon"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.icon"));
    if (sFileMasks.Index(_("*.tiff"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.tiff"));
    if (sFileMasks.Index(_("*.jpeg"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.jpeg"));
    if (sFileMasks.Index(_("*.jfif"), false) == wxNOT_FOUND) sFileMasks.Add(_("*.jfif"));

    //add these file masks to the project manager
    if (pm)
    {
        FilesGroupsAndMasks *fm;
        fm = (FilesGroupsAndMasks *) pm->GetFilesGroupsAndMasks();
        if (fm)
        {
            //test if the filemasks are already associated
            unsigned int i, iMax, iMatch;
            size_t j, jMax;
            bool bMatch;
            wxString sMask;

            bMatch = false;
            jMax = sFileMasks.GetCount();
            iMax = fm->GetGroupsCount();
            for(j=0;j<jMax;j++) //loop over all the possible image files extensions
            {
                sMask = sFileMasks.Item(j);
                for(i=0;i<iMax;i++) //loop over the groups managed by the project manager
                {
                    if (fm->MatchesMask(sMask, i))
                    {
                        bMatch= true;
                        iMatch = i;
                    }
                }
            }

            if (!bMatch)
            {
                //create a new group
                wxString sName;
                sName = _("Images");
                while (IsGroupNameExisting(sName, fm)) sName = sName + _("_");
                fm->AddGroup(sName);
                iMatch = fm->GetGroupsCount() - 1;
            }

            //add all the filemasks to the group
            sMask = fm->GetFileMasks(iMatch);
            for(j=0;j<jMax;j++) //loop over all the possible image files extensions
            {
                if (!(fm->MatchesMask(sFileMasks.Item(j), i))) //to avoid having twice the same mask
                {
                    if (sMask.Len() > 0) sMask = sMask  + _(";");
                    sMask = sMask  + sFileMasks.Item(j);
                }
            }
            fm->SetFileMasks(iMatch, sMask);

            fm->Save();
            pm->GetUI().RebuildTree();
        }
    }

    AddFileMasksToFileOpenSaveDialog(sFileMasks);
}

/** Test if a group name is already existing in the Project Manager
  * \param sName : the group name to test. Example: _("Images")
  * \param fm : the FilesGroupAndMasks to search
  * \return true if the Name already exist, false otherwise
  */
bool XPMEditor::IsGroupNameExisting(wxString sName, const FilesGroupsAndMasks *fm)
{
    if (!fm) return(false);
    unsigned int i, iMax;

    wxString sName2;

    iMax = fm->GetGroupsCount();
    for(i=0;i<iMax;i++) //loop over the groups managed by the project manager
    {
        sName2 = fm->GetGroupName(i);
        if (sName == sName2) return(true);
    }

    return(false);
}

/** Load an image from a file and open it in a new Image Editor
  * If the file is already opened in an editor, then activate this editor
  * \param FileName : the full path to the file to open
  * \return true on success, false on failure
  */
bool XPMEditor::OpenInEditor(wxString FileName)
{
    //This method opens an image file in the Image editor

    if (Manager::Get()->GetEditorManager()->IsOpen(FileName))
    {
        //file already opened - activate the editor
        if (Manager::Get()->GetEditorManager()->GetEditor(FileName))
            Manager::Get()->GetEditorManager()->GetEditor(FileName)->Activate();
        return(true);
    }
    else
    {
        //file not opened - create a new editor
        XPMEditorBase *NewEditor;
        wxString title = wxFileName(FileName).GetFullName();

        //get the bitmap type and load it
        wxBitmapType bt;
        wxImage img;

        if (!(LoadImage(&img, FileName, &bt))) return(false);

        NewEditor = new XPMEditorBase(Manager::Get()->GetEditorManager()->GetNotebook(),
                                        title,
                                        &img,
                                        FileName,
                                        wxBITMAP_TYPE_ANY   //and not bt: this ensure automatic format detection using file extension. Default behaviour
                                      );
        if (NewEditor)
        {
            ProjectFile *pf;
            pf = FindProjectFile(FileName);
            if (pf)
            {
                NewEditor->SetTitle(pf->relativeToCommonTopLevelPath);
                NewEditor->SetProjectFile(pf);
            }
            Manager::Get()->GetEditorManager()->SetActiveEditor(NewEditor);
            return(true);
        }
    }

    return(false);
}

/** This method will load an image from a file
  * It will use wxImage or wxBitmap, depending on the format
  * \param img [out] : a pointer to the wxImage loaded
  * \param sFileName [in] : the full path to the image to load
  * \param bt [out] : a pointer to the file format detected
  * \return : true on success, false on failure
  *           on failure, img is not modified, and bt = wxBITMAP_TYPE_INVALID
  */
bool XPMEditor::LoadImage(wxImage *img, wxString sFileName, wxBitmapType *bt) const
{
    //load an image
    bool bRecognized;
    wxFileName fn(sFileName);
    wxString sExt;
    wxBitmapType bt2;

    sExt = fn.GetExt();

    //get the file format
    bRecognized = GetImageFormatFromFileName(sFileName, bt);

    if (bRecognized)
    {
        //try the simplest case
        //Manager::Get()->GetLogManager()->Log(wxString::Format(_("Image format %d"), *bt));
        if (img->LoadFile(sFileName, *bt)) return(true);

        //try with auto-detection
        bt2 = wxBITMAP_TYPE_ANY;
        if (img->LoadFile(sFileName, bt2))
        {
            *bt = bt2;
            return(true);
        }
    }
    else
    {
        //try with auto-detection
        bt2 = wxBITMAP_TYPE_ANY;
        if (img->LoadFile(sFileName, bt2))
        {
            *bt = bt2;
            return(true);
        }
    }

    //try using a bitmap
    wxBitmap bmp;
    if (bmp.LoadFile(sFileName, *bt))
    {
        if (bmp.IsOk())
        {
            wxImage img2;
            img2 = bmp.ConvertToImage();
            if (img2.IsOk())
            {
                *img = img2;
                return(true);
            }
        }
    }
    if (bmp.LoadFile(sFileName, wxBITMAP_TYPE_ANY))
    {
        if (bmp.IsOk())
        {
            wxImage img2;
            img2 = bmp.ConvertToImage();
            if (img2.IsOk())
            {
                *bt = wxBITMAP_TYPE_ANY;
                *img = img2;
                return(true);
            }
        }
    }

    //try using an icon
    wxIcon ico;
    if (ico.LoadFile(sFileName, *bt))
    {
        if (ico.IsOk())
        {
            wxImage img2;
            wxBitmap bmp;
            bmp.CopyFromIcon(ico);
            if (bmp.IsOk())
            {
                img2 = bmp.ConvertToImage();
                if (img2.IsOk())
                {
                    *img = img2;
                    return(true);
                }
            }
        }
    }
    if (ico.LoadFile(sFileName, wxBITMAP_TYPE_ANY))
    {
        if (ico.IsOk())
        {
            wxImage img2;
            wxBitmap bmp;
            bmp.CopyFromIcon(ico);
            if (bmp.IsOk())
            {
                img2 = bmp.ConvertToImage();
                if (img2.IsOk())
                {
                    *img = img2;
                    return(true);
                }
            }
        }
    }

    return(false);
}

/** This method will save an image to a file
  * It will use wxImage or wxBitmap, depending on the format
  * \param img [in] : a pointer to the wxImage to save
  * \param sFileName [in/out] : the full path to the image to save
  * \param bt [int] : the file format detected
  * \param editor: a pointer to the editor for which the image must be saved
  *                Can be NULL (in which case, no notifications are sent, and the modification flag is not reseted)
  * \return : true on success, false on failure
  *           on failure, img is not modified, and bt = wxBITMAP_TYPE_INVALID
  */
bool XPMEditor::SaveImage(wxImage *img, wxString sFileName, wxBitmapType bt, XPMEditorBase *Editor)
{

    //get the file format to use for saving (bmp, jpeg, png, ...)
    bool bRecognized;
    wxBitmapType bt2;

    if (!img) return(false);
    bt2 = bt;

    bRecognized = false;
    if (bt2 == wxBITMAP_TYPE_ANY)
    {
        //get fileformat requested, based on file extension
        if (XPM_Plugin())
        {
            XPM_Plugin()->GetImageFormatFromFileName(sFileName, &bt2);
            if (XPM_Plugin()->IsFormatValidForWriting(bt2)) bRecognized = true;
        }
    }
    else
    {
        //the fileformat is requested by the user specifically
        if (XPM_Plugin())
        {
            if (XPM_Plugin()->IsFormatValidForWriting(bt2)) bRecognized = true;
        }
    }

    if (!bRecognized)
    {
        wxString sMsg;
        sMsg = _("The file format is not supported on this platform, or is not recognized. Try one of this one:");
        sMsg += _("    *.bmp - bitmap format\n");
        sMsg += _("    *.dib - bitmap format\n");
        sMsg += _("    *.xpm - pixmap XPM format\n");
        sMsg += _("    *.xbm - bitmap XBM format\n");
        sMsg += _("    *.png - bitmap PNG format\n");
        sMsg += _("    *.jpg, *.jpeg, *.jfif - bitmap JPEG format\n");
        sMsg += _("    *.tif, *.tiff - bitmap TIFF format\n");
        sMsg += _("    *.pnm - bitmap PNM format\n");
        sMsg += _("    *.pcx - PCX format\n");
        sMsg += _("    *.pict - PICT format\n");
        sMsg += _("    *.icon - icon format\n");
        sMsg += _("    *.ico - icon format - Windows only\n");
        sMsg += _("    *.cur - cursor format - Windows only\n");
        sMsg += _("    *.ani - animated cursor format - Windows only\n");
        ::wxMessageBox(sMsg, _("File saving error"), wxOK | wxICON_ERROR);
        return(false);
    }

    //check if hotspot is present
    if ((img->GetOptionInt(wxIMAGE_OPTION_CUR_HOTSPOT_X) > 0) &&
        (img->GetOptionInt(wxIMAGE_OPTION_CUR_HOTSPOT_Y) > 0) &&
        (bt2 != wxBITMAP_TYPE_CUR)
       )
    {
        //hot spot not supported in this format
        wxString sMsg;
        sMsg = _("There is a Hot Spot in this image - however, the selected format does not support ");
        sMsg += _("Hot Spot information. The information will be lost.");
        sMsg += _("The following formats support Hot Spot:\n");
        sMsg += _("*.cur - cursor format - Windows only\n");
        sMsg += _("Do you want to continue ?");
        if (::wxMessageBox(sMsg, _("File saving error"), wxYES_NO | wxICON_QUESTION) == wxNO) return(false);
    }


    //save the file
    /// \todo : the NotifyPlugins provokes a crash on save. Understand why
    //if (Editor) Editor->NotifyPlugins(cbEVT_EDITOR_BEFORE_SAVE);
    img->SaveFile(sFileName, bt2);
    if (Editor) Editor->NotifyPlugins(cbEVT_EDITOR_SAVE);

    //flags, quitting
    if (Editor) Editor->SetModified(false);
    return(true);

}

/** Return a list of file saving format supported
  * \param array : a wxString Array containing the results
  *                The array will be cleared : all previous contents will be lost
  *                The results will be in the form:
  *                 "Bitmap (*.bmp)"
  *                 one format per array item
  */
void XPMEditor::GetFileSavingFormat(wxArrayString &array)
{
    array.Clear();
    array.Add(GetFormatString(wxBITMAP_TYPE_ANY));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_BMP) || IsFormatValidForWriting(wxBITMAP_TYPE_BMP_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_BMP));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_CUR) || IsFormatValidForWriting(wxBITMAP_TYPE_CUR_RESOURCE) ||
        IsFormatValidForWriting(wxBITMAP_TYPE_MACCURSOR) || IsFormatValidForWriting(wxBITMAP_TYPE_MACCURSOR_RESOURCE)
       ) array.Add(GetFormatString(wxBITMAP_TYPE_CUR));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_ICON) || IsFormatValidForWriting(wxBITMAP_TYPE_ICON_RESOURCE) ||
        IsFormatValidForWriting(wxBITMAP_TYPE_ICO) || IsFormatValidForWriting(wxBITMAP_TYPE_ICO_RESOURCE)
       ) array.Add(GetFormatString(wxBITMAP_TYPE_ICO));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG) || IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_JPEG));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_PICT) || IsFormatValidForWriting(wxBITMAP_TYPE_PICT_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_PICT));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX) || IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_PCX));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG) || IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_PNG));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM) || IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_PNM));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF) || IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) array.Add(GetFormatString(wxBITMAP_TYPE_TIF));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_XPM) || IsFormatValidForWriting(wxBITMAP_TYPE_XPM_DATA)) array.Add(GetFormatString(wxBITMAP_TYPE_XPM));

    if (IsFormatValidForWriting(wxBITMAP_TYPE_XBM) || IsFormatValidForWriting(wxBITMAP_TYPE_XBM_DATA)) array.Add(GetFormatString(wxBITMAP_TYPE_XBM));
}

/** Return the string associated to the file format (ex: "bitmap (*.bmp)
  * \param bt : the wxBitmapType to test
  * \return : the string associated
  */
wxString XPMEditor::GetFormatString(wxBitmapType bt)
{
    wxString sResult;

    sResult = _("Automatic (*.*)");

    switch(bt)
    {
        case wxBITMAP_TYPE_XBM            :
        case wxBITMAP_TYPE_XBM_DATA       : sResult = _("X bitmap (*.xbm)");
                                            break;

        case wxBITMAP_TYPE_BMP            :
        case wxBITMAP_TYPE_BMP_RESOURCE   :
        //case wxBITMAP_TYPE_RESOURCE       :
                                            sResult = _("bitmaps (*.bmp)");
                                            break;
        //png format
        case wxBITMAP_TYPE_PNG            :
        case wxBITMAP_TYPE_PNG_RESOURCE   :
                                            sResult = _("png (*.png)");
                                            break;

        //jpeg format
        case wxBITMAP_TYPE_JPEG           :
        case wxBITMAP_TYPE_JPEG_RESOURCE  :
                                            //sResult = _("jpeg (*.jpg, *.jpeg, *.jpe)");
                                            sResult = _("jpeg (*.jpg,...)");
                                            break;

        //PCX format
        case wxBITMAP_TYPE_PCX            :
        case wxBITMAP_TYPE_PCX_RESOURCE   :
                                            sResult = _("pcx (*.pcx)");
                                            break;

        //PNM format
        case wxBITMAP_TYPE_PNM            :
        case wxBITMAP_TYPE_PNM_RESOURCE   :
                                            sResult = _("pnm (*.pnm)");
                                            break;

        //TIFF format
        case wxBITMAP_TYPE_TIF            :
        case wxBITMAP_TYPE_TIF_RESOURCE   :
                                            sResult = _("Tiff (*.tif, *.tiff)");
                                            break;

        case wxBITMAP_TYPE_ICO            :
        case wxBITMAP_TYPE_ICO_RESOURCE   :
                                            sResult = _("Icon (*.ico)");
                                            break;
        case wxBITMAP_TYPE_ICON           :
        case wxBITMAP_TYPE_ICON_RESOURCE  :
                                            sResult = _("Icon (*.icon)");
                                            break;

        //Cursor format
        //Mac Cursor Revert to CUR as default
        case wxBITMAP_TYPE_MACCURSOR      :
        case wxBITMAP_TYPE_MACCURSOR_RESOURCE :
        case wxBITMAP_TYPE_CUR            :
        case wxBITMAP_TYPE_CUR_RESOURCE   :
                                            sResult = _("Cursor (*.cur)");
                                            break;

        //XPM format - saving supported
        case wxBITMAP_TYPE_XPM            :
        case wxBITMAP_TYPE_XPM_DATA       :
                                            sResult = _("XPM (*.xpm)");
                                            break;


        //GIFF, IFF, TGA, ANI, PICT formats - saving NOT supported - revert to PNG as default
        case wxBITMAP_TYPE_ANI            : sResult = _("Animated cursor (*.ani)");
                                            break;

        case wxBITMAP_TYPE_IFF            : sResult = _("Iff (*.iff)");
                                            break;

        case wxBITMAP_TYPE_TGA            : sResult = _("tga (*.tga)");
                                            break;

        case wxBITMAP_TYPE_GIF            :
        case wxBITMAP_TYPE_GIF_RESOURCE   :
                                            sResult = _("Giff (*.gif, *.giff)");
                                            break;

        case wxBITMAP_TYPE_PICT           :
        case wxBITMAP_TYPE_PICT_RESOURCE  :
                                            sResult = _("Picture (*.pic, *.pict)");
                                            break;



        //use autodetection based on filename
        case wxBITMAP_TYPE_INVALID        :
        case wxBITMAP_TYPE_ANY            :
        default :                           sResult = _("Automatic (*.*)");
                                            break;
    }

    return(sResult);
}

/** Return the wxBitmapFormat associated to the file format (ex: "bitmap (*.bmp))"
  * \param sFormat: the format to test. It is one of the format returned by GetFormatString()
  * \return the wxBitmapFormat associated
  */
wxBitmapType XPMEditor::GetFormatBitmap(wxString sFormat)
{
    if (sFormat == _("Automatic (*.*)"))         return(wxBITMAP_TYPE_ANY);
    if (sFormat == _("Picture (*.pic, *.pict)")) return(wxBITMAP_TYPE_PICT);
    if (sFormat == _("Giff (*.gif, *.giff)"))    return(wxBITMAP_TYPE_GIF);
    if (sFormat == _("tga (*.tga)"))             return(wxBITMAP_TYPE_TGA);
    if (sFormat == _("Iff (*.iff)"))             return(wxBITMAP_TYPE_IFF);
    if (sFormat == _("Animated cursor (*.ani)")) return(wxBITMAP_TYPE_ANI);
    if (sFormat == _("XPM (*.xpm)"))             return(wxBITMAP_TYPE_XPM);
    if (sFormat == _("Cursor (*.cur)"))          return(wxBITMAP_TYPE_CUR);
    if (sFormat == _("Icon (*.icon)"))           return(wxBITMAP_TYPE_ICON);
    if (sFormat == _("Icon (*.ico)"))            return(wxBITMAP_TYPE_ICO);
    if (sFormat == _("Tiff (*.tif, *.tiff)"))    return(wxBITMAP_TYPE_TIF);
    if (sFormat == _("pnm (*.pnm)"))             return(wxBITMAP_TYPE_PNM);
    if (sFormat == _("pcx (*.pcx)"))             return(wxBITMAP_TYPE_PCX);
    if (sFormat == _("jpeg (*.jpg,...)"))        return(wxBITMAP_TYPE_JPEG);
    if (sFormat == _("png (*.png)"))             return(wxBITMAP_TYPE_PNG);
    if (sFormat == _("bitmaps (*.bmp)"))         return(wxBITMAP_TYPE_BMP);
    if (sFormat == _("X bitmap (*.xbm)"))        return(wxBITMAP_TYPE_XBM);

    return(wxBITMAP_TYPE_ANY);
}

/** This method checks if the given saving file format is available.
  * It returns a compatible file saving format available on the platform
  * \param btFormat : the wxBitmapType to test
  * \return if btFormat is available on saving, then the return value is btFormat
  *         otherwise, a format as close as possible is given
  */
wxBitmapType XPMEditor::GetCompatibleSavingFormat(wxBitmapType btFormat)
{
    //simplest case: the format is recognized
    if (IsFormatValidForWriting(btFormat)) return(btFormat);

    //find an available format
    switch(btFormat)
    {
        //bitmap format - saving supported
        case wxBITMAP_TYPE_XBM            :
        case wxBITMAP_TYPE_XBM_DATA       :
        case wxBITMAP_TYPE_BMP            :
        case wxBITMAP_TYPE_BMP_RESOURCE   :
        //case wxBITMAP_TYPE_RESOURCE       :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_XBM)) return(wxBITMAP_TYPE_XBM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_XBM_DATA)) return(wxBITMAP_TYPE_XBM_DATA);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_BMP_RESOURCE)) return(wxBITMAP_TYPE_BMP_RESOURCE);
                                            break;
        //png format
        case wxBITMAP_TYPE_PNG            :
        case wxBITMAP_TYPE_PNG_RESOURCE   :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG)) return(wxBITMAP_TYPE_PNG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) return(wxBITMAP_TYPE_PNG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG)) return(wxBITMAP_TYPE_JPEG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) return(wxBITMAP_TYPE_JPEG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX)) return(wxBITMAP_TYPE_PCX);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) return(wxBITMAP_TYPE_PCX_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM)) return(wxBITMAP_TYPE_PNM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) return(wxBITMAP_TYPE_PNM_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF)) return(wxBITMAP_TYPE_TIF);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) return(wxBITMAP_TYPE_TIF_RESOURCE);
                                            break;

        //jpeg format
        case wxBITMAP_TYPE_JPEG           :
        case wxBITMAP_TYPE_JPEG_RESOURCE  :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG)) return(wxBITMAP_TYPE_JPEG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) return(wxBITMAP_TYPE_JPEG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG)) return(wxBITMAP_TYPE_PNG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) return(wxBITMAP_TYPE_PNG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX)) return(wxBITMAP_TYPE_PCX);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) return(wxBITMAP_TYPE_PCX_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM)) return(wxBITMAP_TYPE_PNM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) return(wxBITMAP_TYPE_PNM_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF)) return(wxBITMAP_TYPE_TIF);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) return(wxBITMAP_TYPE_TIF_RESOURCE);
                                            break;

        //PCX format
        case wxBITMAP_TYPE_PCX            :
        case wxBITMAP_TYPE_PCX_RESOURCE   :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX)) return(wxBITMAP_TYPE_PCX);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) return(wxBITMAP_TYPE_PCX_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG)) return(wxBITMAP_TYPE_JPEG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) return(wxBITMAP_TYPE_JPEG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG)) return(wxBITMAP_TYPE_PNG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) return(wxBITMAP_TYPE_PNG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM)) return(wxBITMAP_TYPE_PNM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) return(wxBITMAP_TYPE_PNM_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF)) return(wxBITMAP_TYPE_TIF);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) return(wxBITMAP_TYPE_TIF_RESOURCE);
                                            break;

        //PNM format
        case wxBITMAP_TYPE_PNM            :
        case wxBITMAP_TYPE_PNM_RESOURCE   :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM)) return(wxBITMAP_TYPE_PNM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) return(wxBITMAP_TYPE_PNM_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG)) return(wxBITMAP_TYPE_JPEG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) return(wxBITMAP_TYPE_JPEG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG)) return(wxBITMAP_TYPE_PNG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) return(wxBITMAP_TYPE_PNG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX)) return(wxBITMAP_TYPE_PCX);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) return(wxBITMAP_TYPE_PCX_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF)) return(wxBITMAP_TYPE_TIF);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) return(wxBITMAP_TYPE_TIF_RESOURCE);
                                            break;

        //TIFF format
        case wxBITMAP_TYPE_TIF            :
        case wxBITMAP_TYPE_TIF_RESOURCE   :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF)) return(wxBITMAP_TYPE_TIF);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) return(wxBITMAP_TYPE_TIF_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG)) return(wxBITMAP_TYPE_JPEG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) return(wxBITMAP_TYPE_JPEG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG)) return(wxBITMAP_TYPE_PNG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) return(wxBITMAP_TYPE_PNG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM)) return(wxBITMAP_TYPE_PNM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) return(wxBITMAP_TYPE_PNM_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX)) return(wxBITMAP_TYPE_PCX);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) return(wxBITMAP_TYPE_PCX_RESOURCE);
                                            break;

        case wxBITMAP_TYPE_ICO            :
        case wxBITMAP_TYPE_ICO_RESOURCE   :
        case wxBITMAP_TYPE_ICON           :
        case wxBITMAP_TYPE_ICON_RESOURCE  :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_ICO)) return(wxBITMAP_TYPE_ICO);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_ICO_RESOURCE)) return(wxBITMAP_TYPE_ICO_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_ICON)) return(wxBITMAP_TYPE_ICON);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_ICON_RESOURCE)) return(wxBITMAP_TYPE_ICON_RESOURCE);
                                            break;

        //Cursor format
        //Mac Cursor Revert to CUR as default
        case wxBITMAP_TYPE_MACCURSOR      :
        case wxBITMAP_TYPE_MACCURSOR_RESOURCE :
        case wxBITMAP_TYPE_CUR            :
        case wxBITMAP_TYPE_CUR_RESOURCE   :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_MACCURSOR)) return(wxBITMAP_TYPE_MACCURSOR);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_MACCURSOR_RESOURCE)) return(wxBITMAP_TYPE_MACCURSOR_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_CUR)) return(wxBITMAP_TYPE_CUR);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_CUR_RESOURCE)) return(wxBITMAP_TYPE_CUR_RESOURCE);
                                            break;

        //XPM format - saving supported
        case wxBITMAP_TYPE_XPM            :
        case wxBITMAP_TYPE_XPM_DATA       :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_XPM)) return(wxBITMAP_TYPE_XPM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_XPM_DATA)) return(wxBITMAP_TYPE_XPM_DATA);
                                            break;


        //GIFF, IFF, TGA, ANI, PICT formats - saving NOT supported - revert to PNG as default
        case wxBITMAP_TYPE_ANI            :
        case wxBITMAP_TYPE_IFF            :
        case wxBITMAP_TYPE_TGA            :
        case wxBITMAP_TYPE_GIF            :
        case wxBITMAP_TYPE_GIF_RESOURCE   :
        case wxBITMAP_TYPE_PICT           :
        case wxBITMAP_TYPE_PICT_RESOURCE  :
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG)) return(wxBITMAP_TYPE_JPEG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_JPEG_RESOURCE)) return(wxBITMAP_TYPE_JPEG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG)) return(wxBITMAP_TYPE_PNG);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNG_RESOURCE)) return(wxBITMAP_TYPE_PNG_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX)) return(wxBITMAP_TYPE_PCX);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PCX_RESOURCE)) return(wxBITMAP_TYPE_PCX_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM)) return(wxBITMAP_TYPE_PNM);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PNM_RESOURCE)) return(wxBITMAP_TYPE_PNM_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF)) return(wxBITMAP_TYPE_TIF);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_TIF_RESOURCE)) return(wxBITMAP_TYPE_TIF_RESOURCE);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PICT)) return(wxBITMAP_TYPE_PICT);
                                            if (IsFormatValidForWriting(wxBITMAP_TYPE_PICT_RESOURCE)) return(wxBITMAP_TYPE_PICT_RESOURCE);
                                            break;



        //use autodetection based on filename
        case wxBITMAP_TYPE_INVALID        :
        case wxBITMAP_TYPE_ANY            :
        default :
                                            break;
    }

    return(wxBITMAP_TYPE_BMP); //available on all platforms
}

/** Autodetect the file format for the image, based on the file extension
  * \param sFileName : the full or relative path of the file to test
  * \param bt : the resulting wxBitmapType
  * \return true if recognized, false otherwise
  *         if false, bt will have the value wxBITMAP_TYPE_ANY
  */
bool XPMEditor::GetImageFormatFromFileName(wxString sFileName, wxBitmapType *bt) const
{
    bool bRecognized;
    wxFileName fn(sFileName);
    wxString sExtension;

    sExtension = fn.GetExt();
    sExtension.MakeUpper();
    if (!bt) return(false);

    //convert extension to more common extension
    if (sExtension == _("DIB")) sExtension = _("BMP");
    if (sExtension == _("PICT")) sExtension = _("PIC");
    if (sExtension == _("ICON")) sExtension = _("ICO");
    if (sExtension == _("TIFF")) sExtension = _("TIF");
    if (sExtension == _("JPEG")) sExtension = _("JPG");
    if (sExtension == _("JFIF")) sExtension = _("JPG");

    //initialisation
    bRecognized = false;
    *bt = wxBITMAP_TYPE_ANY;

    //loop over all images handlers
    wxList list = wxImage::GetHandlers();

    for(wxList::compatibility_iterator node = list.GetFirst(); node; node = node->GetNext())
    {
        wxImageHandler *handler=(wxImageHandler*) node->GetData();
        wxString sExtension2 = handler->GetExtension();
        sExtension2.MakeUpper();
        if (sExtension2 == sExtension)
        {
            *bt = (wxBitmapType) handler->GetType();
            return(true);
        }
    }

    //get the list of file extension supported by wxBitmap
    wxBitmapHandler *handler2;
    wxString sExtension2;
    wxBitmapType bt2[32];
    int i;

    bt2[0] = wxBITMAP_TYPE_BMP;
    bt2[1] = wxBITMAP_TYPE_BMP_RESOURCE;
    bt2[2] = wxBITMAP_TYPE_RESOURCE;
    bt2[3] = wxBITMAP_TYPE_ICO;
    bt2[4] = wxBITMAP_TYPE_ICO_RESOURCE;
    bt2[5] = wxBITMAP_TYPE_CUR;
    bt2[6] = wxBITMAP_TYPE_CUR_RESOURCE;
    bt2[7] = wxBITMAP_TYPE_XBM;
    bt2[8] = wxBITMAP_TYPE_XBM_DATA;
    bt2[9] = wxBITMAP_TYPE_XPM;
    bt2[10] = wxBITMAP_TYPE_XPM_DATA;
    bt2[11] = wxBITMAP_TYPE_TIF;
    bt2[12] = wxBITMAP_TYPE_TIF_RESOURCE;
    bt2[13] = wxBITMAP_TYPE_GIF;
    bt2[14] = wxBITMAP_TYPE_GIF_RESOURCE;
    bt2[15] = wxBITMAP_TYPE_PNG;
    bt2[16] = wxBITMAP_TYPE_PNG_RESOURCE;
    bt2[17] = wxBITMAP_TYPE_JPEG;
    bt2[18] = wxBITMAP_TYPE_JPEG_RESOURCE;
    bt2[19] = wxBITMAP_TYPE_PNM;
    bt2[20] = wxBITMAP_TYPE_PNM_RESOURCE;
    bt2[21] = wxBITMAP_TYPE_PCX;
    bt2[22] = wxBITMAP_TYPE_PCX_RESOURCE;
    bt2[23] = wxBITMAP_TYPE_PICT;
    bt2[24] = wxBITMAP_TYPE_PICT_RESOURCE;
    bt2[25] = wxBITMAP_TYPE_ICON;
    bt2[26] = wxBITMAP_TYPE_ICON_RESOURCE;
    bt2[27] = wxBITMAP_TYPE_ANI;
    bt2[28] = wxBITMAP_TYPE_IFF;
    bt2[29] = wxBITMAP_TYPE_TGA;
    bt2[30] = wxBITMAP_TYPE_MACCURSOR;
    bt2[31] = wxBITMAP_TYPE_MACCURSOR_RESOURCE;


    for(i=0;i<32;i++)
    {
        handler2 = (wxBitmapHandler*) wxBitmap::FindHandler(bt2[i]);
        if (handler2)
        {
            sExtension2 = handler2->GetExtension();
            if ((sExtension2 == sExtension) && (sExtension2.Len() > 0))
            {
                *bt = (wxBitmapType) handler2->GetType();
                return(true);
            }

        }
    }

    //hard coded extensions
    if (!bRecognized)
    {
        if (sExtension == _("XPM")) {*bt = wxBITMAP_TYPE_XPM; bRecognized = true; }
        if (sExtension == _("ICO")) {*bt = wxBITMAP_TYPE_ICO; bRecognized = true; }
        if (sExtension == _("CUR")) {*bt = wxBITMAP_TYPE_CUR; bRecognized = true; }
        if (sExtension == _("XBM")) {*bt = wxBITMAP_TYPE_XBM; bRecognized = true; }
        if (sExtension == _("BMP")) {*bt = wxBITMAP_TYPE_BMP; bRecognized = true; }
        if (sExtension == _("TIF")) {*bt = wxBITMAP_TYPE_TIF; bRecognized = true; }
        if (sExtension == _("JPG")) {*bt = wxBITMAP_TYPE_JPEG; bRecognized = true; }
        if (sExtension == _("JPE")) {*bt = wxBITMAP_TYPE_JPEG; bRecognized = true; }
        if (sExtension == _("DIB")) {*bt = wxBITMAP_TYPE_BMP; bRecognized = true; }
        if (sExtension == _("PNG")) {*bt = wxBITMAP_TYPE_PNG; bRecognized = true; }
        if (sExtension == _("PNM")) {*bt = wxBITMAP_TYPE_PNM; bRecognized = true; }
        if (sExtension == _("PCX")) {*bt = wxBITMAP_TYPE_PCX; bRecognized = true; }
        if (sExtension == _("GIF")) {*bt = wxBITMAP_TYPE_GIF; bRecognized = true; }
        if (sExtension == _("ANI")) {*bt = wxBITMAP_TYPE_ANI; bRecognized = true; }
        if (sExtension == _("IFF")) {*bt = wxBITMAP_TYPE_IFF; bRecognized = true; }
        if (sExtension == _("TGA")) {*bt = wxBITMAP_TYPE_TGA; bRecognized = true; }
        if (sExtension == _("PIC")) {*bt = wxBITMAP_TYPE_PICT; bRecognized = true; }
    }

    return(bRecognized);
}

/** Indicates if a format is supported or not for writing
  * \param btFormat : the format to test
  *                   see wxImage / wxBitmapType in wxWidgets doc for more information
  * \return true if supported for saving, false otherwise
  *         Note that some format are supported only for reading (GIFF for example)
  *         In this case, the method will return false
  */
bool XPMEditor::IsFormatValidForWriting(wxBitmapType btFormat)
{
    switch(btFormat)
    {
        case wxBITMAP_TYPE_INVALID : return(false);
        case wxBITMAP_TYPE_ANY     : return(true);
        default :  wxImageHandler *ih;
                   wxBitmapHandler *bh;

                   ih = wxImage::FindHandler(btFormat);
                   if (ih)
                   {
                       bool bCanWrite;

                       bCanWrite = true;

                       if (btFormat == wxBITMAP_TYPE_GIF) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_GIF_RESOURCE) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_ANI) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_IFF) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_TGA) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_PICT_RESOURCE) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_PICT) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_MACCURSOR) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_MACCURSOR_RESOURCE) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_XBM) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_XBM_DATA) bCanWrite = false;


                       if (bCanWrite) return(true);

                   }

                   bh = (wxBitmapHandler*) wxBitmap::FindHandler(btFormat);
                   if (bh)
                   {
                       bool bCanWrite;

                       bCanWrite = true;

                       if (btFormat == wxBITMAP_TYPE_GIF) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_GIF_RESOURCE) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_ANI) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_IFF) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_TGA) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_PICT_RESOURCE) bCanWrite = false;
                       if (btFormat == wxBITMAP_TYPE_PICT) bCanWrite = false;

                       if (bCanWrite) return(true);
                   }

                   break;
    }

    return(false);
}

/** Indicates if a format is supported or not for reading
  * \param btFormat : the format to test
  *                   see wxImage / wxBitmapType in wxWidgets doc for more information
  * \return true if supported for reading, false otherwise
  *         Note that some format are supported only for reading (GIFF for example)
  *         In this case, the method will return false
  */
bool XPMEditor::IsFormatValidForReading(wxBitmapType btFormat)
{
    switch(btFormat)
    {
        case wxBITMAP_TYPE_INVALID : return(false);
        case wxBITMAP_TYPE_ANY     : return(true);
        default :  wxImageHandler *ih;
                   wxBitmapHandler *bh;

                   ih = wxImage::FindHandler(btFormat);
                   if (ih) return(true);

                   bh = (wxBitmapHandler*) wxBitmap::FindHandler(btFormat);
                   if (bh) return(true);

                   break;
    }

    return(false);
}

/** Close all the Image Editor opened.
  * Called when the Plugin is unloaded
  */
void XPMEditor::CloseMyEditors(void)
{
    //Close all the Image Editor opened. Called when the Plugin is unloaded
    XPMEditorBase::CloseAllEditors();
}

/** Return the project file if the file belongs to an opened project, return NULL otherwise
  * \param fileName: the wxString with the full path of the file to search
  * \return a pointer to the ProjectFile if the file fileName belongs to an opened project.
            NULL if the file does not belong to an opened CodeBlocks project
  */
ProjectFile* XPMEditor::FindProjectFile(const wxString& fileName)
{
    //return the project file if the file belongs to an opened project, return NULL otherwise
    ProjectsArray* projects = ProjectManager::Get()->GetProjects();
    if (!projects) return(NULL);

    for(size_t i=0; i<projects->Count(); ++i)
    {
        cbProject* project = (*projects)[i];
        if (!project) continue;
        ProjectFile* file = project->GetFileByFilename(fileName, false, false);
        if (file) return(file);
    }

    return(NULL);
}

//---- CONFIGURATION METHODS ------------------------

/** Read the configuration file
  * \param bDefault: if true, will init the parameters to default values on failure
  * \return true on success, false on failure
  *         note : if bDefault = true, the method can return false, but the parameters will be initialized to default values anyway
  */
bool XPMEditor::ReadConfiguration(bool bDefault)
{
    //Read the configuration file.
    bool bSuccess;

    bSuccess = false;
    if ((bDefault) && (!bSuccess))
    {
        //set a default configuration

        iMaxUndoRedo = XPM_MAX_UNDO;
        iXPMDefaultWidth = XPM_DEFAULT_WIDTH;
        iXPMDefaultHeight = XPM_DEFAULT_HEIGHT;
        iOpenXPM = 0; //open in image editor by default.

        cBackgroundColour = *wxLIGHT_GREY;

        iNbColours = 68;
        DefaultColourArray = new wxColour[iNbColours];
        if (DefaultColourArray)
        {
            //Black
            DefaultColourArray[0] = wxTheColourDatabase->Find(_("BLACK"));

            //purple hues
            DefaultColourArray[1] = wxTheColourDatabase->Find(_("BLUE VIOLET"));
            DefaultColourArray[2] = wxTheColourDatabase->Find(_("MAGENTA"));
            DefaultColourArray[3] = wxTheColourDatabase->Find(_("VIOLET"));
            DefaultColourArray[4] = wxTheColourDatabase->Find(_("VIOLET RED"));
            DefaultColourArray[5] = wxTheColourDatabase->Find(_("MEDIUM VIOLET RED"));
            DefaultColourArray[6] = wxTheColourDatabase->Find(_("PINK"));
            DefaultColourArray[7] = wxTheColourDatabase->Find(_("PLUM"));
            DefaultColourArray[8] = wxTheColourDatabase->Find(_("PURPLE"));
            DefaultColourArray[9] = wxTheColourDatabase->Find(_("SALMON"));

            //blue hues
            DefaultColourArray[10] = wxTheColourDatabase->Find(_("BLUE"));
            DefaultColourArray[11] = wxTheColourDatabase->Find(_("AQUAMARINE"));
            DefaultColourArray[12] = wxTheColourDatabase->Find(_("CADET BLUE"));
            DefaultColourArray[13] = wxTheColourDatabase->Find(_("CORAL"));
            DefaultColourArray[14] = wxTheColourDatabase->Find(_("CORNFLOWER BLUE"));
            DefaultColourArray[15] = wxTheColourDatabase->Find(_("MEDIUM AQUAMARINE"));
            DefaultColourArray[16] = wxTheColourDatabase->Find(_("MEDIUM BLUE"));
            DefaultColourArray[17] = wxTheColourDatabase->Find(_("CYAN"));
            DefaultColourArray[18] = wxTheColourDatabase->Find(_("DARK SLATE BLUE"));
            DefaultColourArray[19] = wxTheColourDatabase->Find(_("DARK TURQUOISE"));
            DefaultColourArray[20] = wxTheColourDatabase->Find(_("LIGHT BLUE"));
            DefaultColourArray[21] = wxTheColourDatabase->Find(_("LIGHT STEEL BLUE"));
            DefaultColourArray[22] = wxTheColourDatabase->Find(_("MEDIUM SLATE BLUE"));
            DefaultColourArray[23] = wxTheColourDatabase->Find(_("MEDIUM TURQUOISE"));
            DefaultColourArray[24] = wxTheColourDatabase->Find(_("MIDNIGHT BLUE"));
            DefaultColourArray[25] = wxTheColourDatabase->Find(_("NAVY"));
            DefaultColourArray[26] = wxTheColourDatabase->Find(_("SKY BLUE"));
            DefaultColourArray[27] = wxTheColourDatabase->Find(_("SLATE BLUE"));
            DefaultColourArray[28] = wxTheColourDatabase->Find(_("STEEL BLUE"));
            DefaultColourArray[29] = wxTheColourDatabase->Find(_("TURQUOISE"));

            //green hues
            DefaultColourArray[30] = wxTheColourDatabase->Find(_("KHAKI"));
            DefaultColourArray[31] = wxTheColourDatabase->Find(_("DARK GREEN"));
            DefaultColourArray[32] = wxTheColourDatabase->Find(_("DARK OLIVE GREEN"));
            DefaultColourArray[33] = wxTheColourDatabase->Find(_("FOREST GREEN"));
            DefaultColourArray[34] = wxTheColourDatabase->Find(_("MEDIUM FOREST GREEN"));
            DefaultColourArray[35] = wxTheColourDatabase->Find(_("GREEN"));
            DefaultColourArray[36] = wxTheColourDatabase->Find(_("GREEN YELLOW"));
            DefaultColourArray[37] = wxTheColourDatabase->Find(_("LIME GREEN"));
            DefaultColourArray[38] = wxTheColourDatabase->Find(_("MEDIUM SEA GREEN"));
            DefaultColourArray[39] = wxTheColourDatabase->Find(_("MEDIUM SPRING GREEN"));
            DefaultColourArray[40] = wxTheColourDatabase->Find(_("SPRING GREEN"));
            DefaultColourArray[41] = wxTheColourDatabase->Find(_("PALE GREEN"));
            DefaultColourArray[42] = wxTheColourDatabase->Find(_("SEA GREEN"));

            //yellow / orange hues
            DefaultColourArray[43] = wxTheColourDatabase->Find(_("YELLOW"));
            DefaultColourArray[44] = wxTheColourDatabase->Find(_("YELLOW GREEN"));
            DefaultColourArray[45] = wxTheColourDatabase->Find(_("GOLD"));
            DefaultColourArray[46] = wxTheColourDatabase->Find(_("GOLDENROD"));
            DefaultColourArray[47] = wxTheColourDatabase->Find(_("MEDIUM GOLDENROD"));
            DefaultColourArray[48] = wxTheColourDatabase->Find(_("ORANGE"));
            DefaultColourArray[49] = wxTheColourDatabase->Find(_("ORANGE RED"));
            DefaultColourArray[50] = wxTheColourDatabase->Find(_("WHEAT"));

            //red hues
            DefaultColourArray[51] = wxTheColourDatabase->Find(_("RED"));
            DefaultColourArray[52] = wxTheColourDatabase->Find(_("ORCHID"));
            DefaultColourArray[53] = wxTheColourDatabase->Find(_("DARK ORCHID"));
            DefaultColourArray[54] = wxTheColourDatabase->Find(_("FIREBRICK"));
            DefaultColourArray[55] = wxTheColourDatabase->Find(_("INDIAN RED"));
            DefaultColourArray[56] = wxTheColourDatabase->Find(_("MEDIUM ORCHID"));

            //brown hues
            DefaultColourArray[57] = wxTheColourDatabase->Find(_("BROWN"));
            DefaultColourArray[58] = wxTheColourDatabase->Find(_("MAROON"));
            DefaultColourArray[59] = wxTheColourDatabase->Find(_("THISTLE"));
            DefaultColourArray[60] = wxTheColourDatabase->Find(_("SIENNA"));
            DefaultColourArray[61] = wxTheColourDatabase->Find(_("TAN"));

            //greys & white hues
            DefaultColourArray[62] = wxTheColourDatabase->Find(_("DARK GREY"));
            DefaultColourArray[63] = wxTheColourDatabase->Find(_("DARK SLATE GREY"));
            DefaultColourArray[64] = wxTheColourDatabase->Find(_("DIM GREY"));
            DefaultColourArray[65] = wxTheColourDatabase->Find(_("GREY"));
            DefaultColourArray[66] = wxTheColourDatabase->Find(_("LIGHT GREY"));
            DefaultColourArray[67] = wxTheColourDatabase->Find(_("WHITE"));

            cTransparent = *wxLIGHT_GREY;
        }
    }

    return(bSuccess);
}

/** Write the configuration file
  * \return true on success, false on failure
  */
bool XPMEditor::WriteConfiguration(void)
{
    //write the configuration file
    return(false);
}

/** set the size of the UNDO / REDO buffer
  * \param iMax: the size of the Undo Buffer
  *        the Redo buffer will automatically have the same size as the UNDO buffer
  *         a negative value indicates unlimited Undo / Redo
  */
void XPMEditor::SetMaxUndoRedo(int iMax)
{
    //set the size of the UNDO / REDO buffer
    iMaxUndoRedo = iMax;
    UpdateConfiguration();
}

/** get the size of the UNDO / REDO buffer
  * \return the size of the Undo Buffer
  *         the size of the Redo Buffer is the same
  *         a negative value indicates unlimited Undo / Redo
  */
int XPMEditor::GetMaxUndoRedo(void)
{
    //get the size of the UNDO / REDO buffer
    return(iMaxUndoRedo);
}

/** set the default width & height of a new image
  * \param iWidth: the new width, in pixel.
  * \param iHeight: the new height, in pixel.
  * if the values are <= 0, then the default value is used
  */
void XPMEditor::SetDefaultImageSize(int iWidth, int iHeight)
{
    //set the default width & height of a new image
    iXPMDefaultWidth = iWidth;
    iXPMDefaultHeight = iHeight;
    if (iXPMDefaultWidth <= 0) iXPMDefaultWidth = XPM_DEFAULT_WIDTH;
    if (iXPMDefaultHeight <= 0) iXPMDefaultHeight = XPM_DEFAULT_HEIGHT;
    UpdateConfiguration();
}

/** Get the default width & height of a new image
  * \param iWidth: a pointer to the width, in pixel.
  * \param iHeight: a pointer the height, in pixel.
  */
void XPMEditor::GetDefaultImageSize(int *iWidth, int *iHeight)
{
    //get the default width & height of a new image
    if (iWidth) *iWidth = iXPMDefaultWidth;
    if (iHeight) *iHeight = iXPMDefaultHeight;
}

/** Set the default colour array
  * \param array : the new array of colours
  * \param inbColours: the number of elements in array
  * \param cTransparentColour: the colour to be used as transparent mask
  * \return true on success, false on failure (memory allocation problem)
  * The caller is responsible for allocating & freeing the "array" array
  */
bool XPMEditor::SetColourArray(wxColour *array, int inbColours,
                            wxColour cTransparentColour)
{
    //set the default colour array
    if (!array) return(false);
    if (inbColours <= 0) return(false);
    wxColour *newArray;
    newArray = new wxColour[inbColours];
    if (!newArray) return(false);

    int i;
    iNbColours = inbColours;
    for(i=0;i<inbColours;i++) newArray[i] = array[i];
    delete[] DefaultColourArray;
    DefaultColourArray = newArray;
    cTransparent = cTransparentColour;
    return(true);
}

/** Get the default colour array
  * \param array : an array will receive the array of colours
  *                if NULL, it is ignored.
  *                if not NULL, the method will NOT allocate memory for copying the array.
  * \param inbColours: a pointer to the number of elements in array
  *                    if NULL, it is ignored.
  * \param cTransparentColour: the colour to be used as transparent mask
  *                            if NULL, it is ignored.
  * The caller is responsible for allocating and freeing the memory for the array
  * Use GetColourArray(NULL, &iNbColours, NULL) to get the size of the array to be allocated.
  */
void XPMEditor::GetColourArray(wxColour *array, int *inbColours, wxColour *cTransparentColour)
{
    //get the default colour array
    if ((array) && (DefaultColourArray))
    {
        int i;
        for(i=0;i<iNbColours;i++) array[i] = DefaultColourArray[i];
    }
    if (inbColours) *inbColours = iNbColours;
    if (cTransparentColour) *cTransparentColour = cTransparent;
}


/** Set the background colour
  * \param cBackColour: the new background colour
  */
void XPMEditor::SetBackGroundColour(wxColour cBackColour)
{
    //Set the background colour
    cBackgroundColour = cBackColour;
    UpdateConfiguration();
}

/** Get the background colour
  * \return the background colour
  */
wxColour XPMEditor::GetBackGroundColour(void)
{
    //Get the background colour
    return(cBackgroundColour);
}

/** Request all the image editors to update their configuration
  */
void XPMEditor::UpdateConfiguration(void)
{
    //ask all the image editors to update their configuration
    int i, iMax;
    EditorBase *ed;

    if (!EditorManager::Get()) return;

    iMax = EditorManager::Get()->GetEditorsCount();

    for(i=0; i<iMax; i++)
    {
        ed = EditorManager::Get()->GetEditor(i);
        if (!ed) continue;
        if (XPMEditorBase::IsImgEditor(ed)) ((XPMEditorBase *)ed)->UpdateConfiguration();
    }
}

/** menu handler for "New Image"
  */
void XPMEditor::OnNewImage(wxCommandEvent &event)
{
    //Manager::Get()->GetLogManager()->Log(_("menu new image selected"));
    wxImage img(XPM_DEFAULT_WIDTH, XPM_DEFAULT_HEIGHT, true);

    if (img.IsOk())
    {
        img.Replace(0,0,0,0xFF, 0xFF, 0xFF); //set the image to white
        img.SetMaskColour(0xFF, 0xFF, 0xFF); //mask colour is white

        XPMEditorBase *NewEditor;
        NewEditor = new XPMEditorBase(Manager::Get()->GetEditorManager()->GetNotebook(),
                                        _("Untitled.xpm"),
                                        &img,
                                        wxEmptyString,
                                        wxBITMAP_TYPE_ANY   //and not bt: this ensure automatic format detection using file extension. Default behaviour
                                      );
        if (NewEditor) NewEditor->SetModified(true);
    }
}

/** menu handler for "Add Image" (wxSmith menu)
  */
void XPMEditor::OnNewProjectImage(wxCommandEvent &event)
{
    //Manager::Get()->GetLogManager()->Log(_("menu wxSmith new image selected"));
    wxImage img(XPM_DEFAULT_WIDTH, XPM_DEFAULT_HEIGHT, true);
    if (img.IsOk())
    {
        img.Replace(0,0,0,0xFF, 0xFF, 0xFF); //set the image to white

        XPMEditorBase *NewEditor;
        NewEditor = new XPMEditorBase(Manager::Get()->GetEditorManager()->GetNotebook(),
                                        _("Untitled.xpm"),
                                        &img,
                                        wxEmptyString,
                                        wxBITMAP_TYPE_ANY   //and not bt: this ensure automatic format detection using file extension. Default behaviour
                                      );
        if (NewEditor) NewEditor->SetModified(true);

        //add the image to the project
        cbProject *activeProject = ProjectManager::Get()->GetActiveProject();
        if (activeProject)
        {
            if (NewEditor->Save())
            {
                wxString sFileName;
                sFileName = NewEditor->GetFilename();
                ProjectManager::Get()->AddFileToProject(sFileName, activeProject, -1);
                Manager::Get()->GetEditorManager()->SetActiveEditor(NewEditor);
                ProjectManager::Get()->GetUI().RebuildTree();

                if (wxMessageBox(_("Do you want to save the project ?"), _("New image added"), wxYES_NO, NewEditor) == wxYES)
                {
                    activeProject->Save();
                }
            }
        }
    }
}

/** menu handler for "Open Image"
  */
void XPMEditor::OnOpenImage(wxCommandEvent &event)
{
    //Manager::Get()->GetLogManager()->Log(_("menu open image selected"));
    wxString sFileName;
    wxString Filters = FileFilters::GetFilterString();

    int StoredIndex = FileFilters::GetIndexForFilterAll();

    FileFilters::GetFilterIndexFromName(Filters, _("Image Files"), StoredIndex);

    wxFileDialog dlg(Manager::Get()->GetAppWindow(),
                     _("Open file"),
                    wxEmptyString,
                    wxEmptyString,
                    Filters,
                    wxFD_OPEN | compatibility::wxHideReadonly);
    dlg.SetFilterIndex(StoredIndex);

    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
         sFileName = dlg.GetPath();
         OpenFile(sFileName);
    }
}

/** menu handler for "Open Image" (Project Manager context menu)
  */
void XPMEditor::OnOpenFromProjectManager(wxCommandEvent &event)
{
    wxTreeCtrl *tree = Manager::Get()->GetProjectManager()->GetUI().GetTree();

    if ( !tree )
        return;

    wxTreeItemId treeItem = Manager::Get()->GetProjectManager()->GetUI().GetTreeSelection();

    if ( !treeItem.IsOk() )
        return;

    const FileTreeData *data = static_cast<FileTreeData*>( tree->GetItemData( treeItem ) );

    if ( !data )
        return;

    if ( data->GetKind() == FileTreeData::ftdkFile )
    {

        wxString sFileName;
        sFileName = data->GetProjectFile()->file.GetFullPath();
        OpenFile(sFileName);
    }
}

/**  menu handler for "Open Image" (File Manager context menu)
  */
void XPMEditor::OnOpenFromFileManager(wxCommandEvent &event)
{
    if (m_FileNameSelected != _(""))
    {
        OpenFile(m_FileNameSelected);
    }
    m_FileNameSelected = _("");
}
