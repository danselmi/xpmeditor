/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditorBase.h
 * Purpose:   the base class for the Image editor - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPMEDITOR_H
#define XPMEDITOR_H

#include <editorbase.h>
#include <projectfile.h>
#include <wx/bitmap.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <vector>
#include <set>

#include "XPMEditorPanel.h"


/** \brief Base class for editors used inside XPM_BUILDER
 *
 * This class is responsible for proper binding / unbinging editor
 * from image.
 *
 * When editing image it should always be done in such form:
 *  - Call BeginChange() to put image into unstable state
 *  - Change data of image (without updating informations on screen)
 *  - Call EndChange() which should update image on screen
 *
 */

class XPMEditorPanel;
class XPMEditorBase;

class XPMEditorBase: public EditorBase
{
	public:
		//constructors and destructors
		XPMEditorBase(wxWindow* parent, const wxString& title,wxImage *img,
                        wxString sFileName);            ///< \brief Constructor
		~XPMEditorBase();                             ///< \brief Destructor
        static bool IsImgEditor(EditorBase* editor);    ///< \brief Check if given editor is an image editor
        static void CloseAllEditors(void);              ///< \brief Close all editors
        void SetPanel(wxPanel *Panel);                  ///< \brief Set the panel to be used by the editor
        wxPanel* GetPanel(void);                        ///< \brief Get the panel used by the editor

        //Save, Close, Readonly, is modified
        virtual bool GetModified(void) const;       ///< \brief Get the modification flag for the image
		virtual void SetModified(bool bModified);   ///< \brief Set the modification flag for the image
        void UpdateModified(void);                  ///< \brief Update the state of "modified" document
        void SetReadOnly(bool bReadOnly);           ///< \brief set read-only status
		bool GetReadOnly(void) const;               ///< \brief test if current image is read-only
		bool IsReadOnly(void) const;                ///< \brief test if current image is read-only
		bool Save(void);                            ///< \brief Save image
		bool SaveAs(void);                          ///< \brief Save image under a new name
		bool Close(void);                           ///< \brief Close the editor
		void SetProjectFile(ProjectFile* project_file,
                            bool preserve_modified = false); ///< \brief Set the Project file of the Image. Can be NULL
        void NotifyPlugins(wxEventType type, int intArg = 0, const wxString& strArg = wxEmptyString,
                           int xArg = 0, int yArg = 0);     ///< \brief Post application-wide events

		//undo / redo functions
		bool CanUndo(void) const;   ///< \brief Checking if can Undo
		bool CanRedo(void) const;   ///< \brief Ckeck if can Redo
		void ClearUndoBuffer(void); ///< \brief Clear the Undo Buffer
		void ClearRedoBuffer(void); ///< \brief Clear the Redo Buffer
		bool AddUndo(wxBitmap *bm); ///< \brief Add 1 UNDO operation to the buffer
        bool AddRedo(wxBitmap *bm); ///< \brief Add 1 REDO operation to the buffer
        bool AddUndo(wxImage *img); ///< \brief Add 1 UNDO operation to the buffer
        bool AddRedo(wxImage *img); ///< \brief Add 1 REDO operation to the buffer
        bool AddUndo(void);         ///< \brief Add 1 UNDO operation to the buffer
        bool AddRedo(void);         ///< \brief Add 1 REDO operation to the buffer
		void Undo(void);            ///< \brief UNDO
		void Redo(void);            ///< \brief REDO

		//clipboards operations
		bool CanCopy(void) const;        ///< \brief Check if we can copy
        bool CanCut(void) const;         ///< \brief Check if we can cut
		bool CanPaste(void) const;       ///< \brief Check if we can paste
		void Cut(void);                  ///< \brief Perform a Cut operation
		void Copy(void);                 ///< \brief perform a Copy operation
		void Paste(void);                ///< \brief perform a Paste operation
		bool HasSelection(void) const;   ///< \brief returns true if it has selection
		void ClearSelection(void);       ///< \brief clear the current selection
		void GetBoundingRect(wxRect *r); ///< \brief Get the bounding rectangle of the selection

		//image access
		wxImage GetImage(void);         ///< \brief Get current image (a copy)
		void SetImage(wxImage *img);     ///< \brief Set the current image

		//configuration
		void UpdateConfiguration(void);   ///< \brief Update the configuration of the editor

	private:
	    wxScrolledWindow* GetDrawCanvas(void);  ///< \brief return a pointer to the ScrolledWindow where Bitmaps are drawn
        bool m_bModified;                       ///< \brief indicates if an image has been modifed
        bool m_bReadOnly;                       ///< \brief indicates the image is read only
        bool m_bIsFileNameOK;                   ///< \brief indicates if the filename is correct
        XPMEditorPanel *m_DrawArea;           ///< \brief the panel with the toolbox and the DrawCanvas (wxScrolledWindow with Bitmap)
        ProjectFile* m_pProjectFile;            ///< \brief the project file to which the image belongs

        //events handlers
        void OnResize(wxSizeEvent& event);      ///< \brief max size of the Redo buffer

        //all editors set
        typedef std::set< EditorBase* > EditorsSet;     ///< \brief manage a list of all Image Editors currently opened.
        static EditorsSet m_AllEditors;         ///< \brief Set of all opened editors, used to close all editors when plugin is being unloaded
};

#endif
