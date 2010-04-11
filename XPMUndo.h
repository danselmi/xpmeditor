/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMUndo.h
 * Purpose:   manages the Undo / Redo buffer - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPM_UNDO_H
#define XPM_UNDO_H

/** @brief This class manages the Undo / Redo buffer
  */

enum Undo_Action_Type
{
    xpm_undefined_undo=0,
    xpm_bitmap_undo=1,
    xpm_image_undo=2,
    xpm_size_undo=3
};

struct xpm_undo_action
{
    Undo_Action_Type UndoType;
    void *pData;
};

class XPMUndo
{
    public:
        XPMUndo();
        ~XPMUndo();

        //parent methods
		void SetParentPanel(XPMEditorPanel *pPanel);  ///< @brief Set the parent panel for the Undo Buffer
		XPMEditorPanel* GetParentPanel(void);         ///< @brief Get the parent panel for the Undo Buffer

        //buffer & memory functions
        void SetMaxUndoSize(long lSize);///< @brief set the maximal number of Undo actions. -1 indicates unlimited.
        long GetMaxUndoSize(void);      ///< @brief Get the maximal number of Undo actions. -1 indicates unlimited.
        bool IncreaseMemoryUndo(void);  ///< @brief Increase the size of the Undo buffer, if needed
        bool IncreaseMemoryRedo(void);  ///< @brief Increase the size of the Redo buffer, if needed
        void ClearUndoBuffer(void);     ///< @brief Delete all the Undo actions
        void ClearRedoBuffer(void);     ///< @brief Delete all the Redo actions
        void ReleaseUndoBuffer(void);   ///< @brief Delete all the Undo actions and release the memory used by the Undo buffer
        void ReleaseRedoBuffer(void);   ///< @brief Delete all the Redo actions and release the memory used by the Redo buffer

        //Undo & Redo methods
        bool CanUndo(void);                                 ///< @brief return true if it is possible to Undo something, false otherwise
        bool CanRedo(void);                                 ///< @brief return true if it is possible to Redo something, false otherwise
        bool AddUndo(Undo_Action_Type iType, void *pData,
                     bool bClearRedo = true);  ///< @brief Add 1 Undo action
        bool AddRedo(Undo_Action_Type iType, void *pData);  ///< @brief Add 1 Redo action
        void Undo(void);  ///< @brief performs 1 Undo operation
        void Redo(void);  ///< @brief performs 1 Redo operation

    private:
        xpm_undo_action *UndoBuffer;    ///< @brief The Undo Buffer
        xpm_undo_action *RedoBuffer;    ///< @brief The Redo Buffer
        long lUndoSize;                 ///< @brief The current amount of Undo actions stored
        long lUndoMax;                  ///< @brief The memory size of Undo actions array
        long lRedoSize;                 ///< @brief The current amount of Redo actions stored
        long lRedoMax;                  ///< @brief The memory size of Redo actions array

        long lMaxUndoRedo;              ///< @brief the absolute maximal size of the Undo & Redo buffers

        void ReleaseUndoRedoAction(int iUndoRedo, long lIndex); ///< @brief Release a specific Undo / Redo action

        XPMEditorPanel *parent;
        void PerformAction(XPMEditorPanel *parent, xpm_undo_action *buffer, long lIndex); ///< @brief do the Undo / Redo action


};

#endif // XPM_UNDO_H
