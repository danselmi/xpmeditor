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
#include <configurationpanel.h>

#include <wx/aui/auibook.h>

#include "XPMEditor.h"
#include "XPMColorPicker.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<XPMEditor> regMain(_T("XPMEditor"));
}

XPMEditor* XPMEditor::m_Singleton = 0;

/** Constructor
  */
XPMEditor::XPMEditor()
{
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
  * @param filename : the path to the file to be tested
  * @return true if the file can be opened by the plugin, false otherwise
  */
bool XPMEditor::CanHandleFile(const wxString& filename) const
{
   wxString fn;

   fn = filename;
   fn.MakeUpper();

   if (fn.Right(4) == _(".XPM"))
   {
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

   if (fn.Right(4) == _(".ICO")) return(true);
   if (fn.Right(4) == _(".CUR")) return(true);
   if (fn.Right(4) == _(".XBM")) return(true);
   if (fn.Right(4) == _(".BMP")) return(true);
   if (fn.Right(4) == _(".TIF")) return(true);
   if (fn.Right(4) == _(".JPG")) return(true);
   if (fn.Right(4) == _(".JPE")) return(true);
   if (fn.Right(4) == _(".DIB")) return(true);
   if (fn.Right(4) == _(".PNG")) return(true);
   if (fn.Right(4) == _(".PNM")) return(true);
   if (fn.Right(4) == _(".PCX")) return(true);
   if (fn.Right(4) == _(".GIF")) return(true);
   if (fn.Right(5) == _(".ANI")) return(true);
   if (fn.Right(5) == _(".IFF")) return(true);
   if (fn.Right(5) == _(".TGA")) return(true);
   if (fn.Right(5) == _(".PICT")) return(true);
   if (fn.Right(5) == _(".ICON")) return(true);
   if (fn.Right(5) == _(".TIFF")) return(true);
   if (fn.Right(5) == _(".JPEG")) return(true);
   if (fn.Right(5) == _(".JFIF")) return(true);

   return(false);
}

/** Open a file in the editor
  * @param filename : the path to the file to open
  * @return 0 on success, 1 on failure.
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

/** return a pointer to the unique instance of the plugin class.
  * static method
  * @return a pointer to the unique class instance on success, NULL on failure (plugin not initialized)
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
        wxInitAllImageHandlers();
        m_Singleton = this;
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
}

/** Load an image from a file and open it in a new Image Editor
  * If the file is already opened in an editor, then activate this editor
  * @param FileName : the full path to the file to open
  * @return true on success, false on failure
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
        wxString fn;
        wxBitmapType bt;
        fn = FileName;
        fn.MakeUpper();

        if (fn.Right(4) == _(".XPM")) bt = wxBITMAP_TYPE_XPM;
        if (fn.Right(4) == _(".ICO")) bt = wxBITMAP_TYPE_ICO;
        if (fn.Right(4) == _(".CUR")) bt = wxBITMAP_TYPE_CUR;
        if (fn.Right(4) == _(".XBM")) bt = wxBITMAP_TYPE_XBM;
        if (fn.Right(4) == _(".BMP")) bt = wxBITMAP_TYPE_BMP;
        if (fn.Right(4) == _(".TIF")) bt = wxBITMAP_TYPE_TIF;
        if (fn.Right(4) == _(".JPG")) bt = wxBITMAP_TYPE_JPEG;
        if (fn.Right(4) == _(".JPE")) bt = wxBITMAP_TYPE_JPEG;
        if (fn.Right(4) == _(".DIB")) bt = wxBITMAP_TYPE_BMP;
        if (fn.Right(4) == _(".PNG")) bt = wxBITMAP_TYPE_PNG;
        if (fn.Right(4) == _(".PNM")) bt = wxBITMAP_TYPE_PNM;
        if (fn.Right(4) == _(".PCX")) bt = wxBITMAP_TYPE_PCX;
        if (fn.Right(4) == _(".GIF")) bt = wxBITMAP_TYPE_GIF;
        if (fn.Right(5) == _(".ANI")) bt = wxBITMAP_TYPE_ANI;
        if (fn.Right(5) == _(".IFF")) bt = wxBITMAP_TYPE_IFF;
        if (fn.Right(5) == _(".TGA")) bt = wxBITMAP_TYPE_TGA;
        if (fn.Right(5) == _(".PICT")) bt = wxBITMAP_TYPE_PICT;
        if (fn.Right(5) == _(".ICON")) bt = wxBITMAP_TYPE_ICON;
        if (fn.Right(5) == _(".TIFF")) bt = wxBITMAP_TYPE_TIF;
        if (fn.Right(5) == _(".JPEG")) bt = wxBITMAP_TYPE_JPEG;
        if (fn.Right(5) == _(".JFIF")) bt = wxBITMAP_TYPE_JPEG;

        wxImage img;
        if (!(img.LoadFile(FileName, bt))) return(false);
        //ensure the image has a mask
        if (!img.HasMask())
        {
            unsigned char iRed, iGreen, iBlue;
            if (img.FindFirstUnusedColour(&iRed, &iGreen, &iBlue))
            {
                img.SetMaskColour(iRed, iGreen, iBlue);
            }
            else
            {
                img.SetMask();
            }
        }


            //::wxMessageBox(_("Before NewEditor"), _("INFO"), wxOK);
        NewEditor = new XPMEditorBase(Manager::Get()->GetEditorManager()->GetNotebook(),
                                        title,
                                        &img,
                                        FileName
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

            //Set the correct mask colour as transparent colour
            unsigned char iRed, iGreen, iBlue;
            iRed = img.GetMaskRed();
            iGreen = img.GetMaskGreen();
            iBlue = img.GetMaskBlue();
            wxColour cColour(iRed, iGreen, iBlue);
            if (NewEditor->GetPanel())
            {
                XPMEditorPanel *panel;
                panel = (XPMEditorPanel *) NewEditor->GetPanel();
                if (panel->ColourPicker)
                {
                    panel->ColourPicker->SetTransparentColour(cColour, false);
                    panel->Refresh(false, NULL);
                    panel->Update();
                }
            }
        }
        return(true);
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
  * @param fileName: the wxString with the full path of the file to search
  * @return a pointer to the ProjectFile if the file fileName belongs to an opened project.
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
  * @param bDefault: if true, will init the parameters to default values on failure
  * @return true on success, false on failure
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
  * @return true on success, false on failure
  */
bool XPMEditor::WriteConfiguration(void)
{
    //write the configuration file
    return(false);
}

/** set the size of the UNDO / REDO buffer
  * @param iMax: the size of the Undo Buffer
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
  * @return the size of the Undo Buffer
  *         the size of the Redo Buffer is the same
  *         a negative value indicates unlimited Undo / Redo
  */
int XPMEditor::GetMaxUndoRedo(void)
{
    //get the size of the UNDO / REDO buffer
    return(iMaxUndoRedo);
}

/** set the default width & height of a new image
  * @param iWidth: the new width, in pixel.
  * @param iHeight: the new height, in pixel.
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
  * @param iWidth: a pointer to the width, in pixel.
  * @param iHeight: a pointer the height, in pixel.
  */
void XPMEditor::GetDefaultImageSize(int *iWidth, int *iHeight)
{
    //get the default width & height of a new image
    if (iWidth) *iWidth = iXPMDefaultWidth;
    if (iHeight) *iHeight = iXPMDefaultHeight;
}

/** Set the default colour array
  * @param array : the new array of colours
  * @param inbColours: the number of elements in array
  * @param cTransparentColour: the colour to be used as transparent mask
  * @return true on success, false on failure (memory allocation problem)
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
  * @param array : an array will receive the array of colours
  *                if NULL, it is ignored.
  *                if not NULL, the method will NOT allocate memory for copying the array.
  * @param inbColours: a pointer to the number of elements in array
  *                    if NULL, it is ignored.
  * @param cTransparentColour: the colour to be used as transparent mask
  *                            if NULL, it is ignored.
  * The caller is responsible for allocating and freeing the memory for the array
  * Use GetColourArray(NULL, &iNbColours, NULL) to get the size of the array to be allocated.
  */
void XPMEditor::GetColourArray(wxColour *array, int *inbColours, wxColour *cTransparentColour)
{
    //get the default colour array
    bool bResult;
    if ((array) && (DefaultColourArray))
    {
        int i;
        for(i=0;i<iNbColours;i++) array[i] = DefaultColourArray[i];
    }
    if (inbColours) *inbColours = iNbColours;
    if (cTransparentColour) *cTransparentColour = cTransparent;
}


/** Set the background colour
  * @param cBackColour: the new background colour
  */
void XPMEditor::SetBackGroundColour(wxColour cBackColour)
{
    //Set the background colour
    cBackgroundColour = cBackColour;
    UpdateConfiguration();
}

/** Get the background colour
  * @return the background colour
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
