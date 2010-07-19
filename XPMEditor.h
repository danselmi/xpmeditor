/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditor.h
 * Purpose:   plugin main entry point - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPM_EDITOR_H_INCLUDED
#define XPM_EDITOR_H_INCLUDED

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <sdk.h>
#include <cbplugin.h> // for "class cbPlugin"

#define XPM_DEFAULT_WIDTH   100
#define XPM_DEFAULT_HEIGHT  100
#define XPM_MAX_UNDO        100

#include <wx/msgdlg.h>
#include <wx/list.h>

#include "XPMEditorPanel.h"
#include "XPMEditorBase.h"
#include "XPMCustomZoom.h"
#include "XPMUndo.h"

class XPMEditor : public cbMimePlugin
{
    public:
        /** Constructor. */
        XPMEditor();
        /** Destructor. */
        virtual ~XPMEditor();


        /** This method is called by Code::Blocks and is used by the plugin
          * to add any menu items it needs on Code::Blocks's menu bar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the menu,
          * just do nothing ;)
          * \param menuBar the wxMenuBar to create items in
          */
        virtual void BuildMenu(wxMenuBar* menuBar){}

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
        virtual void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0){}

        /** This method is called by Code::Blocks and is used by the plugin
          * to add any toolbar items it needs on Code::Blocks's toolbar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the toolbar,
          * just do nothing ;)
          * \param toolBar the wxToolBar to create items on
          * \return The plugin should return true if it needed the toolbar, false if not
          */
        virtual bool BuildToolBar(wxToolBar* toolBar){ return false; }
        /** This method return a pointer to the the unique instance of this class
          */
        static XPMEditor* Get(void);

        /** This method opens an image file in the Draw editor
        **/
        bool OpenInEditor(wxString Filename);

        /** \brief Can a file be handled by this plugin?
          *
          * \param filename The file in question.
          * \return The plugin should return true if it can handle this file,
          * false if not.
          */
        virtual bool CanHandleFile(const wxString& filename) const;

        /** \brief Open the file.
          *
          * \param filename The file to open.
          * \return The plugin should return zero on success, other value on error.
          */
        virtual int OpenFile(const wxString& filename);

        /** \brief Is this a default handler?
          *
          * This is a flag notifying the main app that this plugin can handle
          * every file passed to it. Usually you 'll want to return false in
          * this function, because you usually create specialized handler
          * plugins (for specific MIME types)...
          *
          * \return True if this plugin can handle every possible MIME type,
          * false if not.
          */
        virtual bool HandlesEverything() const;

        /** \brief remove 1 editor from the array
          * Search the editor in the array. If found, remove it from the array
          * If the editor is opened, it will NOT be closed.
          * \param editor : the editor to delete from the array
          * \return true on success, false on failure
          */
        bool DeleteEditor(XPMEditorBase *editor);

        //save and load images
        bool LoadImage(wxImage *img, wxString sFileName, wxBitmapType *bt); ///< \brief This method will load an image from a file
        bool SaveImage(wxImage *img, wxString sFileName, wxBitmapType bt, XPMEditorBase *Editor);  ///< \brief This method will save an image to a file

        //for setting & getting standard configuration
        void SetMaxUndoRedo(int iMax);  ///< \brief set the size of the UNDO / REDO buffer
        int GetMaxUndoRedo(void);       ///< \brief get the size of the UNDO / REDO buffer
        void SetDefaultImageSize(int iWidth, int iHeight);   ///< \brief set the default width & height of a new image
        void GetDefaultImageSize(int *iWidth, int *iHeight); ///< \brief get the default width & height of a new image
        bool SetColourArray(wxColour *array, int inbColours,
                            wxColour cTransparentColour);    ///< \brief set the default colour array
        void GetColourArray(wxColour *array, int *inbColours,
                            wxColour *cTransparentColour);   ///< \brief get the default colour array
        wxColour GetBackGroundColour(void);                 ///< \brief Get the background colour
        void SetBackGroundColour(wxColour cBackColour);    ///< \brief Set the background colour

        //configuration file
        bool ReadConfiguration(bool bDefault); ///< \brief Read the configuration file.
        bool WriteConfiguration(void); ///< \brief write the configuration file
        void UpdateConfiguration(void); ///< \brief ask all the image editors to update their configuration

        //image format supported
        bool IsFormatValidForReading(wxBitmapType btFormat); ///< \brief Indicates if a format is supported or not for reading
        bool IsFormatValidForWriting(wxBitmapType btFormat); ///< \brief Indicates if a format is supported or not for writing
        bool GetImageFormatFromFileName(wxString sFileName, wxBitmapType *bt) const;  ///< \brief Autodetect the file format for the image, based on the file extension
        void AddFileMasksToProjectManager(void);                                ///< \brief This method adds Filemasks, such as "*.bmp" to the project manager
        bool IsGroupNameExisting(wxString sName, const FilesGroupsAndMasks *fm);///< \brief Test if a group name is already existing in the Project Manager
        void GetFileSavingFormat(wxArrayString &array);                         ///< \brief Return a list of file saving format supported
        wxBitmapType GetCompatibleSavingFormat(wxBitmapType btFormat);          ///< \brief This method checks if the given saving file format is available.
        wxString GetFormatString(wxBitmapType bt);                             ///< \brief Return the string associated to the file format (ex: "bitmap (*.bmp)"

        //help files
        wxString GetHTMLHelp(void); ///< \brief  Find the path to the plugin resource files, containing the HTLM help files

    protected:
        /** Any descendent plugin should override this virtual method and
          * perform any necessary initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded and should attach in Code::Blocks. When Code::Blocks
          * starts up, it finds and <em>loads</em> all plugins but <em>does
          * not</em> activate (attaches) them. It then activates all plugins
          * that the user has selected to be activated on start-up.\n
          * This means that a plugin might be loaded but <b>not</b> activated...\n
          * Think of this method as the actual constructor...
          */
        virtual void OnAttach();

        /** Any descendent plugin should override this virtual method and
          * perform any necessary de-initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded, attached and should de-attach from Code::Blocks.\n
          * Think of this method as the actual destructor...
          * \param appShutDown If true, the application is shutting down. In this
          *         case *don't* use Manager::Get()->Get...() functions or the
          *         behaviour is undefined...
          */
        virtual void OnRelease(bool appShutDown);

    private:
        static XPMEditor* m_Singleton;      ///< \brief Singleton object
        void CloseMyEditors(void);           ///< \brief To close all image editor at once.
        ProjectFile* FindProjectFile(const wxString& FileName); ///< \brief return the project file if the file belongs to an opened project, return NULL otherwise

        //standard configuration
        int iMaxUndoRedo;      ///< \brief the size of the Undo / Redo buffer. A negative size indicates unlimited Undo / Redo
        int iXPMDefaultWidth;  ///< \brief the default width of a new XPM
        int iXPMDefaultHeight; ///< \brief the default height of a new XPM
        int iOpenXPM;          ///< \brief 1: open the XPM in text editor. 2: ask the user. Other values: open the XPM in Image Editor.
        wxColour *DefaultColourArray; ///< \brief the colour array with standard colors
        wxColour cTransparent; ///< \brief the transparent colour
        int iNbColours;        ///< \brief the amount of colours in DefaultColourArray
        wxColour cBackgroundColour; ///< \brief the background colour
};

/** \brief Helper function to easily access xpm_builder plugin */
inline XPMEditor* XPM_Plugin() { return XPMEditor::Get(); }

#endif // XPM_EDITOR_H_INCLUDED
