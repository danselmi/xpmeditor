/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditorPanel.cpp
 * Purpose:   the panel containing all the drawing tools and the draw canvas - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include "XPMEditor.h"
#include "wxStretchImage.h"
#include "wxMirror.h"
#include "wxRotate.h"
#include "wxBlur.h"
#include "wxRotateHue.h"
#include "wxConversion.h"
#include "wxInvertDialog.h"

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/brush.h>
#include <wx/pen.h>
#include <wx/utils.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/rawbmp.h>
#include <wx/region.h>
#include <wx/graphics.h>
#include <wx/bitmap.h>
#include <wx/dragimag.h>
#include <wx/fontdlg.h>
#include <wx/textfile.h>
#include <wx/stattext.h>
#include <wx/textfile.h>
#include <math.h>

#define PI 3.14159265358979

#include "XPMToolPanel.h"
#include "XPMHelpPanel.h"
#include "XPMImagePropertiesPanel.h"
#include "XPMImageManipulationPanel.h"
#include "wxResizeCtrl.h"

//(*InternalHeaders(XPMEditorPanel)
#include "XPMInterfacePanel.h"
#include "XPMDrawCanvasPanel.h"
#include <wx/aui/aui.h>
#include "XPMColourPickerPanel.h"
#include <wx/intl.h>
#include "XPMFoldPanel.h"
#include <wx/string.h>
//*)

#define _DEBUG_XPM_EDITOR_PANEL 0


//----------- INIITIALISATION & DECLARATIONS ------------------------------

//(*IdInit(XPMEditorPanel)
const long XPMEditorPanel::ID_DRAWCANVASPANEL = wxNewId();
const long XPMEditorPanel::ID_FOLDPANEL = wxNewId();
const long XPMEditorPanel::ID_INTERFACEPANEL = wxNewId();
const long XPMEditorPanel::ID_COLOURPICKERPANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMEditorPanel,wxPanel)
	//(*EventTable(XPMEditorPanel)
	//*)
END_EVENT_TABLE()

//---------------------------- CONSTRUCTORS  ------------------------------

/** Constructor - Create a new Image Editor panel
  * \param parent: the parent window for the panel
  * \param id = wxID_ANY : the window identifier for this panel
  * \param pos = wxDefaultPosition : the desired position of the panel after creation
  * \param size = wxDefaultSize : the desired size of the panel after creation
  */
XPMEditorPanel::XPMEditorPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{

	ResizeCtrl1 = NULL; //necessary to avoid race condition, when building the panel
	TextEdit = NULL;    //necessary to avoid race condition, when building the panel
	BuildContent(parent,id,pos,size);

	m_Bitmap = NULL;
	m_Image = NULL;
	bCanResizeX = false;
	bCanResizeY = false;
	bSizingX = false;
	bSizingY = false;
	OldX = 0; OldY = 0;
	dScale = 1;

	//selection initialisation
	NbPointsMax = 100;
    NbPoints = 0;
    pSelection = (wxPoint*) malloc(NbPointsMax * sizeof(wxPoint));
    bDrawSelection = true;

    //Undo and Redo buffer
    m_undo_buffer = new XPMUndo;
    if (m_undo_buffer) m_undo_buffer->SetParentPanel(this);


    TextEdit->Hide();
    ResizeCtrl1->Hide();

    bUsingTool = false;
    m_DragImage = NULL;
    m_bDragging = false;
    m_bEraseSelection = false;
    m_SelectionImage = wxImage();
    pStartDragging = wxPoint(0,0);
    cMaskColour = *wxBLACK;
    m_iSizeAction = 0;
    m_bSizing = false;
    iPos = 0;
    iHotSpotX = -1;
    iHotSpotY = -1;
    m_bDrawToolDynamic = false;

    UpdateConfiguration();
}

/** Create all the widgets in the panel. Called by the constructor
  * \param parent: the parent window for the panel
  * \param id = wxID_ANY : the window identifier for this panel
  * \param pos = wxDefaultPosition : the desired position of the panel after creation
  * \param size = wxDefaultSize : the desired size of the panel after creation
  */
void XPMEditorPanel::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMEditorPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(199,144), wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("wxID_ANY"));
	m_AUIXPMEditor = new wxAuiManager(this, wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_DEFAULT);
	DrawCanvasPanel = new XPMDrawCanvasPanel(this);
	m_AUIXPMEditor->AddPane(DrawCanvasPanel, wxAuiPaneInfo().Name(_T("Image")).CenterPane().Caption(_("Image")));
	FoldPanel = new XPMFoldPanel(this);
	m_AUIXPMEditor->AddPane(FoldPanel, wxAuiPaneInfo().Name(_T("Tools")).DefaultPane().Caption(_("Tools")).CloseButton(false).Left());
	InterfacePanel = new XPMInterfacePanel(this);
	m_AUIXPMEditor->AddPane(InterfacePanel, wxAuiPaneInfo().Name(_T("Interface")).DefaultPane().Caption(_("Interface")).CloseButton(false).Top());
	ColourPicker = new XPMColourPickerPanel(this);
	m_AUIXPMEditor->AddPane(ColourPicker, wxAuiPaneInfo().Name(_T("Colours")).DefaultPane().Caption(_("Colours")).CloseButton(false).Top());
	m_AUIXPMEditor->Update();

	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasKeyDown);
	//*)

	//to do : register an event handler when AUI colours are updated
	//EVT_MENU(idSettingsEnvironment, MainFrame::OnSettingsEnvironment)
    UpdateMinimalSizes();
	UpdateAUIColours();

	wxString sFilePath;
	sFilePath = ConfigManager::GetExecutableFolder();
	sFilePath = sFilePath + _("\\XPMEditor_log.txt");

	LogToFile(_("Step 1"), sFilePath);

	if (DrawCanvasPanel)
	{
	    DrawCanvas  = DrawCanvasPanel->DrawCanvas;
		TextEdit    = DrawCanvasPanel->TextEdit;
		ResizeCtrl1 = DrawCanvasPanel->ResizeCtrl1;
		sCursorPos  = DrawCanvasPanel->sCursorPos;

		DrawCanvasPanel->TextEdit->Connect(wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnTextEditText);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_PAINT,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasPaint,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_ERASE_BACKGROUND,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasEraseBackground,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasKeyDown,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftDown,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftUp,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftDClick,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_RIGHT_UP,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasRightUp,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_MOTION,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasMouseMove,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasMouseLeave,0,this);
        DrawCanvasPanel->DrawCanvas->Connect(wxEVT_SIZE,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasResize,0,this);
	}
	else
	{
	    DrawCanvas  = NULL;
		TextEdit    = NULL;
		ResizeCtrl1 = NULL;
		sCursorPos  = NULL;
	}

	//init pointers to panels
	if (FoldPanel)
	{
	    ToolPanel = FoldPanel->GetToolPanel();
	    HelpPanel = FoldPanel->GetHelpPanel();
	    PropertiesPanel = FoldPanel->GetPropertiesPanel();
	    ImageManipulationPanel = FoldPanel->GetImageManipulationPanel();
	}
	else
	{
	    ToolPanel = NULL;
	    HelpPanel = NULL;
	    PropertiesPanel = NULL;
	    ImageManipulationPanel = NULL;
	}


#if __WXMSW__
	//Mouse Capture lost event
	DrawCanvas->Connect(wxEVT_MOUSE_CAPTURE_LOST ,(wxObjectEventFunction)&XPMEditorPanel::OnMouseCaptureLost,0,this);
#endif

    //set the parents of the child panels
    if (ColourPicker) ColourPicker->SetParentPanel(this);
    if (InterfacePanel) InterfacePanel->SetParentPanel(this);
    if (FoldPanel) FoldPanel->SetParentPanel(this);

    //ResizeCtrl
    ResizeCtrl1->SetChild(TextEdit);
    ToggleButtons(-1, false); //Toggle All buttons off.

	dScale = 1;
	bShowGrid = false;
	cGridColour = *wxBLACK;

}

/** Debugging function : writes a string to a text file
  * The identifer _DEBUG_XPM_EDITOR_PANEL must be defined with a value different from 0
  * see at the beginning of this file
  * \param sLogText : the string to write in the file.
  *                   a new line will be created
  * \param sFilePath: the path of the file
  *                   if the file does not exist, it will be created
  */
void XPMEditorPanel::LogToFile(wxString sLogText, wxString sFilePath)
{
    #if _DEBUG_XPM_EDITOR_PANEL != 0

        wxTextFile tf(sFilePath);

        if (tf.Exists())
        {
            if (!tf.Open()) return;
        }
        else
        {
            if (!tf.Create()) return;
        }

        tf.AddLine(sLogText);
        tf.Write();
        tf.Close();

    #endif
}


/** Set minimal sizes for the AUI Panes
  * Minimal sizes will be set for :
  *  - the tools panel (FoldPanel)
  *  - the interface panel (InterfacePanel)
  *  - the colour picker panel
  * Floating size, and minimal sizes are given
  */
void XPMEditorPanel::UpdateMinimalSizes(void)
{
    if (m_AUIXPMEditor)
    {
        wxAuiPaneInfo auiColPickerInfo;
        wxAuiPaneInfo auiInterfaceInfo;
        wxAuiPaneInfo auiFoldInfo;

        auiColPickerInfo = m_AUIXPMEditor->GetPane(ColourPicker);
        auiInterfaceInfo = m_AUIXPMEditor->GetPane(InterfacePanel);
        auiFoldInfo = m_AUIXPMEditor->GetPane(FoldPanel);

        if ((auiColPickerInfo.IsOk()) && (ColourPicker))
        {
            //Colour Picker
            wxSize sMinSize;
            wxSize sBestSize;

            if (ColourPicker->ColourPicker)
            {
                sMinSize  = ColourPicker->ColourPicker->DoGetMinSize();
                sBestSize = ColourPicker->ColourPicker->GetBestSize();
            }

            auiColPickerInfo = auiColPickerInfo.BestSize(sBestSize)
                                               //.FloatingSize(sBestSize)
                                               .MinSize(sMinSize);


            if (m_AUIXPMEditor->DetachPane(ColourPicker))
            {
                m_AUIXPMEditor->AddPane(ColourPicker, auiColPickerInfo);
            }

        }

        if ((auiInterfaceInfo.IsOk()) && (InterfacePanel))
        {
            //Interface Panel
            wxSize sMinSize;
            wxSize sBestSize;

            if (InterfacePanel->GetSizer())
            {
                sMinSize  = InterfacePanel->GetSizer()->GetMinSize();
                sBestSize =  InterfacePanel->GetBestSize();
            }


            auiInterfaceInfo = auiInterfaceInfo.BestSize(sBestSize)
                                               .FloatingSize(sBestSize)
                                               .MinSize(sMinSize);

            if (m_AUIXPMEditor->DetachPane(InterfacePanel))
            {
                m_AUIXPMEditor->AddPane(InterfacePanel, auiInterfaceInfo);
            }
        }

        if ((auiFoldInfo.IsOk()) && (FoldPanel))
        {
            //Tools panel
            wxSize sMinSize;
            wxSize sBestSize;

            if (FoldPanel->FoldPanelBar1)
            {
                sMinSize  = FoldPanel->FoldPanelBar1->GetMinSize();
                sBestSize = FoldPanel->FoldPanelBar1->GetBestSize();
            }

            auiFoldInfo = auiFoldInfo.BestSize(sBestSize)
                                     .FloatingSize(sBestSize)
                                     .MinSize(sMinSize);

            if (m_AUIXPMEditor->DetachPane(FoldPanel))
            {
                m_AUIXPMEditor->AddPane(FoldPanel, auiFoldInfo);
            }
        }

        m_AUIXPMEditor->Update();
    }
}

/** Get the same colours as codeblocks configuration
  * Code::Blocks wxAUIManager is fetched, and its colours & metrics are read
  * These colours & metrics are then applied to the current wxAUIManager
  */
void XPMEditorPanel::UpdateAUIColours(void)
{
    wxAuiManager *auiCodeBlocksManager;
    wxAuiDockArt *auiCodeBlocksDockArt;
    wxAuiDockArt *auiXPMEditorDockArt;

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("app"));

    //recover wxAUIManager from Code::Blocks main window
    auiCodeBlocksManager = wxAuiManager::GetManager(GetParent());
    if (!auiCodeBlocksManager) return;

    //recover wxAuiDockArt from Code::Blocks main window
    auiCodeBlocksDockArt = auiCodeBlocksManager->GetArtProvider();
    if (!auiCodeBlocksDockArt) return;

    //recover wxAuiDockArt from this panel
    if (!m_AUIXPMEditor) return;
    auiXPMEditorDockArt = m_AUIXPMEditor->GetArtProvider();
    if (!auiXPMEditorDockArt) return;

    //copy the colours
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR,              cfg->ReadColour(_T("/environment/aui/active_caption_colour"), auiXPMEditorDockArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR)));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR,     cfg->ReadColour(_T("/environment/aui/active_caption_gradient_colour"), auiXPMEditorDockArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR)));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR,         cfg->ReadColour(_T("/environment/aui/active_caption_text_colour"), auiXPMEditorDockArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR)));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,            cfg->ReadColour(_T("/environment/aui/inactive_caption_colour"), auiXPMEditorDockArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR)));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,   cfg->ReadColour(_T("/environment/aui/inactive_caption_gradient_colour"), auiXPMEditorDockArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR)));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR,       cfg->ReadColour(_T("/environment/aui/inactive_caption_text_colour"), auiXPMEditorDockArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR)));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_BACKGROUND_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_BACKGROUND_COLOUR));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_SASH_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_SASH_COLOUR));
    //auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR));
    //auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR));
    //auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR));
    //auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR));
    //auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR));
    //auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_BORDER_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_BORDER_COLOUR));
    auiXPMEditorDockArt->SetColour(wxAUI_DOCKART_GRIPPER_COLOUR, auiCodeBlocksDockArt->GetColour(wxAUI_DOCKART_GRIPPER_COLOUR));

    //copy the font
    auiXPMEditorDockArt->SetFont(wxAUI_DOCKART_CAPTION_FONT, auiCodeBlocksDockArt->GetFont(wxAUI_DOCKART_CAPTION_FONT));
    auiXPMEditorDockArt->SetFont(wxAUI_DOCKART_GRADIENT_TYPE, auiCodeBlocksDockArt->GetFont(wxAUI_DOCKART_GRADIENT_TYPE));

    //copy the metrics
    auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,   cfg->ReadInt(_T("/environment/aui/border_size"), auiXPMEditorDockArt->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE)));
    auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_SASH_SIZE,          cfg->ReadInt(_T("/environment/aui/sash_size"), auiXPMEditorDockArt->GetMetric(wxAUI_DOCKART_SASH_SIZE)));
    auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_CAPTION_SIZE,       cfg->ReadInt(_T("/environment/aui/caption_size"), auiXPMEditorDockArt->GetMetric(wxAUI_DOCKART_CAPTION_SIZE)));
    //auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_SASH_SIZE, auiCodeBlocksDockArt->GetMetric(wxAUI_DOCKART_SASH_SIZE));
    //auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_CAPTION_SIZE, auiCodeBlocksDockArt->GetMetric(wxAUI_DOCKART_CAPTION_SIZE));
    auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_GRIPPER_SIZE, auiCodeBlocksDockArt->GetMetric(wxAUI_DOCKART_GRIPPER_SIZE));
    //auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE, auiCodeBlocksDockArt->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE));
    auiXPMEditorDockArt->SetMetric(wxAUI_DOCKART_PANE_BUTTON_SIZE, auiCodeBlocksDockArt->GetMetric(wxAUI_DOCKART_PANE_BUTTON_SIZE));

    //apply the colours & metrics
    m_AUIXPMEditor->Update();
}

/** Destructor
  */
XPMEditorPanel::~XPMEditorPanel()
{
	//(*Destroy(XPMEditorPanel)
	//*)

    //bitmap release
	if (m_Bitmap)
	{
        delete(m_Bitmap);
        m_Bitmap = NULL;
	}

	//image release
	if (m_Image)
	{
        delete(m_Image);
        m_Image = NULL;
	}

	//selection memory release
	ClearSelection();
    free(pSelection);
    if (m_DragImage) delete m_DragImage;

    //free UNDO buffer
    if (m_undo_buffer) delete m_undo_buffer;
}

//--------------------- MODIFICATION FLAGS METHODS  -----------------------

/** Update the modification flag
  * \param bModified : the modification flag
  */
void XPMEditorPanel::SetModified(bool bModified)
{
    XPMEditorBase *editor;
    editor = (XPMEditorBase *) GetParent();
    if (editor) editor->SetModified(bModified);
}

/** Get the modification flag
  * \return : the modification flag
  */
bool XPMEditorPanel::GetModified(void)
{
    XPMEditorBase *editor;
    editor = (XPMEditorBase *) GetParent();
    if (editor) return(editor->GetModified());
    return(false);
}

//----------------- IMAGE & BITMAP MANAGEMENTS METHODS  -------------------

/** Return the scaled internal bitmap.
  * The returned pointer to the wxBitmap is NOT a copy: do NOT delete it.
  * \return a pointer to the wxBitmap used internally by the editor
  *         this wxBitmap is used for scaling and all draw modification.
  *         It is converted as an image before saving, or before any manipulation requiring a wxImage
  *         Both a wxBitmap and wxImage are stored because it is the only way to work around a wxWidgets bug:
  *         Scaling (wxDC.SetUserScale()) is innacurate.
  */
wxBitmap* XPMEditorPanel::GetBitmap(void)
{
    //return the associated bitmap
    return(m_Bitmap);
}

/** Return an image representing the selection
  */
wxImage XPMEditorPanel::GetImageFromSelection(void)
{
    //Return an image representing the selection
    if (!m_Image) return(wxImage());
    if (!HasSelection()) return(wxImage());

    //get Selection bounding rect
    wxRect rSelection;
    GetBoundingRect(&rSelection);

    //reduce width & Height by 1 because the rectangle enclose completly the selection
    //if not done, the copied image will be 1 pixel to large
    int iWidth, iHeight;
    iWidth = rSelection.GetWidth();
    iHeight = rSelection.GetHeight();
    if (iWidth > 1) iWidth--;
    if (iHeight > 1) iHeight--;
    rSelection.SetWidth(iWidth);
    rSelection.SetHeight(iHeight);

    //get sub-image from selection bounding rect
    wxImage imgCopy(*m_Image);
    imgCopy = imgCopy.GetSubImage(rSelection);

    //create a bitmap mask
    wxBitmap bmpMask(rSelection.GetWidth(), rSelection.GetHeight(), 1);
    wxMemoryDC memDC;
    memDC.SelectObject(bmpMask);
    memDC.SetBackground(*wxBLACK_BRUSH);
    memDC.Clear();
    memDC.SetPen(*wxWHITE_PEN);
    memDC.SetBrush(*wxWHITE_BRUSH);
    //draw the mask
    wxPoint *tmp;
    tmp = new wxPoint[NbPoints];
    if (tmp)
    {
        int i;
        for(i=0;i<NbPoints;i++)
        {
            tmp[i].x = pSelection[i].x - rSelection.GetLeft() ;
            tmp[i].y = pSelection[i].y - rSelection.GetTop();
        }
        memDC.DrawPolygon(NbPoints, tmp);
        delete[] tmp;
    }


    //apply the mask (convert it to wxImage first)
    wxImage imgMask = bmpMask.ConvertToImage();
    imgCopy.SetMask(true);
    imgCopy.SetMaskFromImage(imgMask,0,0,0);

    m_SelectionBitmap = wxBitmap(imgCopy);
    return(imgCopy);
}

/** Replace the Selection with the mask colour
  */
void XPMEditorPanel::CutSelection(void)
{
    if (!m_Image) return;
    if (!HasSelection()) return;

    //get Selection bounding rect
    wxRect rSelection;
    GetBoundingRect(&rSelection);

    //reduce width & Height by 1 because the rectangle enclose completly the selection
    //if not done, the copied image will be 1 pixel to large
    int iWidth, iHeight;
    iWidth = rSelection.GetWidth();
    iHeight = rSelection.GetHeight();
    if (iWidth > 1) iWidth--;
    if (iHeight > 1) iHeight--;
    rSelection.SetWidth(iWidth);
    rSelection.SetHeight(iHeight);

    //create a bitmap mask
    wxBitmap bmpMask(rSelection.GetWidth(), rSelection.GetHeight(), -1);
    wxMemoryDC memDC;
    memDC.SelectObject(bmpMask);
    memDC.SetBackground(*wxBLACK_BRUSH);
    memDC.Clear();
    memDC.SetPen(*wxWHITE_PEN);
    memDC.SetBrush(*wxWHITE_BRUSH);
    //draw the mask
    wxPoint *tmp;
    tmp = new wxPoint[NbPoints];
    if (tmp)
    {
        int i;
        for(i=0;i<NbPoints;i++)
        {
            tmp[i].x = pSelection[i].x - rSelection.GetLeft() ;
            tmp[i].y = pSelection[i].y - rSelection.GetTop();
        }
        memDC.DrawPolygon(NbPoints, tmp);
        delete[] tmp;
    }

    //replace the white area by the mask colour
    wxImage imgMask = bmpMask.ConvertToImage();
    imgMask.Replace(255, 255, 255, cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

    //Set a mask colour (Black) for this image
    imgMask.SetMaskColour(0,0,0);

    PasteImage(imgMask, rSelection.GetLeft(), rSelection.GetTop());
}

/** Move the selection by dx / dy
  * no border checks are performed.
  * if the selection is beyond the image boundaries (may happen when pasting a bigger image from clipboard,
  * or when moving the selection), then the selection image beyond the boundaries is hidden.
  * \param dx : the increment by which the selection must be moved horizontally.
                Positive value moves the selection to the right
  * \param dy : the increment by which the selection must be moved vertically.
                Positive value moves the selection to the bottom
  */
void XPMEditorPanel::MoveSelection(int dx, int dy)
{
    //Move the selection
    if (!HasSelection()) return;
    int i;

    for(i=0; i<NbPoints; i++)
    {
        pSelection[i].x = pSelection[i].x + dx;
        pSelection[i].y = pSelection[i].y + dy;
/*
        //border check
        if (pSelection[i].x < 0) pSelection[i].x = 0;
        if (pSelection[i].y < 0) pSelection[i].y = 0;
        if (pSelection[i].x > m_Image->GetWidth()) pSelection[i].x = m_Image->GetWidth();
        if (pSelection[i].y > m_Image->GetHeight()) pSelection[i].y = m_Image->GetHeight();
*/
    }
}

/** Return the current associated image
  * All changes buffered are flushed first
  * \return the wxImage stored.
  *         This is a copy of the wxImage
  */
wxImage XPMEditorPanel::GetImage(void)
{
    //return the associated image
    if (!m_Image) return(wxImage());

    wxImage img(*m_Image);
    img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

    //hot spot information
    if ((iHotSpotX >= 0) && (iHotSpotX < m_Image->GetWidth()) && (iHotSpotY >= 0) && (iHotSpotY < m_Image->GetHeight()))
    {
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, iHotSpotX);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, iHotSpotY);
    }

    return(img);
}

/** Set the image to be edited
  * the previous one is discarded and deleted
  * Scale factor is kept.
  * \param : a pointer to the new wxImage.
  *          a copy is created by the method. It is therefore safe to delete it afterwards.
  *          if NULL, a blank wxImage with default size will be created
  */
void XPMEditorPanel::SetImage(wxImage *img)
{
    //set the associated bitmap
    if (DrawCanvas)
    {
        //delete the previous bitmap first
        if (m_Image)
        {
            delete(m_Image);
        }

        if (img)
        {
            //create a copy of the bitmap
            m_Image = new wxImage(*img);

            unsigned char r, g, b;
            m_Image->GetOrFindMaskColour(&r, &g, &b);
            cMaskColour = wxColour(r, g , b);
            ColourPicker->SetTransparentColour(cMaskColour, false);
            ColourPicker->Repaint();
            if (m_Image->HasMask())
            {
                //remove the mask
                unsigned char r2, g2, b2;
                wxColour cTransparent;
                cTransparent = ColourPicker->GetTransparentColour();
                r2 = cTransparent.Red();
                g2 = cTransparent.Green();
                b2 = cTransparent.Blue();
                //wxMessageBox(wxString::Format(_("Has mask r=%d g=%d b=%d r2=%d g2=%d b2=%d"),r,g,b, r2, g2, b2));
                m_Image->SetMask(false);
            }

        }
        else
        {
            //create a blank bitmap
            m_Image = new wxImage(iXPMDefaultWidth, iXPMDefaultHeight, true);
            cMaskColour = ColourPicker->GetTransparentColour();
        }
        if (!m_Image) return;

        //check for HotSpot coordinates

        if ((m_Image->HasOption(wxIMAGE_OPTION_CUR_HOTSPOT_X)) && (m_Image->HasOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y)))
        {
            iHotSpotX = m_Image->GetOptionInt(wxIMAGE_OPTION_CUR_HOTSPOT_X);
            iHotSpotY = m_Image->GetOptionInt(wxIMAGE_OPTION_CUR_HOTSPOT_Y);
        }

        wxBitmap bm(*m_Image);
        SetBitmap(&bm);
    }
}

/** recreate the m_Bitmap member from the m_Image member
  * The Draw Canvas is not updated. It must be done manually by the caller
  * The scrollbars are updated
  */
void XPMEditorPanel::UpdateBitmap(void)
{
    //recreate the m_Bitmap member from the m_Image member
    if (m_Image)
    {
        wxBitmap bm(*m_Image);
        SetBitmap(&bm);
    }
}

/** Set the unscaled internal bitmap.
  * \param bm: a pointer to the wxBitmap to be used internally by the editor
  *         this wxBitmap is used for scaling and all draw modification.
  *         The method will create a copy of the wxBitmap passed in parameter
  *         It is therefore safe to delete it afterwards
  *         It is converted as an image before saving, or before any manipulation requiring a wxImage
  *         Both a wxBitmap and wxImage are stored because it is the only way to work around a wxWidgets bug:
  *         Scaling (wxDC.SetUserScale()) is innacurate.
  */
void XPMEditorPanel::SetBitmap(wxBitmap *bm)
{
    //set the associated bitmap
    if (DrawCanvas)
    {
        //delete the previous bitmap first
        wxBitmap *bmTemp;
        if (m_Bitmap)
        {
            bmTemp = m_Bitmap;
        }
        else
        {
            bmTemp = NULL;
        }

        if (bm)
        {
            //create a copy of the bitmap
            m_Bitmap = new wxBitmap(*bm);
        }
        else
        {
            //create a blank bitmap
            m_Bitmap = new wxBitmap(iXPMDefaultWidth, iXPMDefaultHeight);
        }
        if (bmTemp) delete(bmTemp);
        if (!m_Bitmap) return;

        //Set the scrollbars and draw area size
        sDrawAreaSize = wxSize(m_Bitmap->GetWidth(), m_Bitmap->GetHeight());
        if (PropertiesPanel)
        {
            if (PropertiesPanel->BMPWidth) PropertiesPanel->BMPWidth->SetValue(m_Bitmap->GetWidth());
            if (PropertiesPanel->BMPHeight) PropertiesPanel->BMPHeight->SetValue(m_Bitmap->GetHeight());
        }

        int iFactor;
        iFactor = dScale * 100;
        if (InterfacePanel)
        {
            if (InterfacePanel->ZoomFactor) InterfacePanel->ZoomFactor->SetValue(wxString::Format(_("%d%%"), iFactor));
        }


        DoSetScrollBars();

        m_bmDrawBitmap = wxBitmap(m_Bitmap->GetWidth(), m_Bitmap->GetHeight());

        Repaint();
    }
}


/** Ensure the Image is up-to-date (buffered user actions are flushed)
  * recreate the m_Image member from the m_Bitmap member
  * The Draw Canvas is not updated. It must be done manually by the caller
  * The scrollbars are also not updated
  */
void XPMEditorPanel::UpdateImage(void)
{
    //Ensure the Image is up-to-date (buffered user actions are flushed)
    //recreate the m_Image member from the m_Bitmap member
    if (m_Bitmap)
    {
        wxImage img;
        img = m_Bitmap->ConvertToImage();
        wxImage *img2;
        img2 = new wxImage(img);
        if (img2)
        {
            wxImage *img_temp;
            img_temp = m_Image;
            m_Image = img2;
            if(img_temp) delete(img_temp);
        }
    }
}

/** return a pointer to the wxScrolledWindow on which the Bitmap is drawn.
    \return a pointer to the wxScrolledWindow on which the Bitmap is drawn on success, NULL otherwise
  */
wxScrolledWindow* XPMEditorPanel::GetDrawCanvas(void)
{
    //get the DrawCanvas
    return(DrawCanvas);
}

//--------------------------- PAINT HANDLERS  -----------------------------
/** To avoid redrawing the window background.
  * It is taken care of manually in OnDrawCanvasPaint
  */
void XPMEditorPanel::OnDrawCanvasEraseBackground(wxEraseEvent& event)
{

}

/** update the display
  */
void XPMEditorPanel::Repaint(void)
{
    if (DrawCanvas)
    {
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
    }
}

/** The Paint Event handler
  * Will also redraw the background where needed (this solution is better while scrolling)
  */
void XPMEditorPanel::OnDrawCanvasPaint(wxPaintEvent& event)
{
    //wxPaintDC dc(DrawCanvas);
    wxBufferedPaintDC dc(DrawCanvas);

    int iMax, jMax;

    if (DrawCanvas) DrawCanvas->PrepareDC(dc);
    dc.SetUserScale(1,1);

    //create the transparent background, and draw the bitmap on it
    wxColour cTransparent;
    if (ColourPicker) cTransparent = ColourPicker->GetTransparentColour(); else cTransparent = *wxWHITE;

    wxBrush bTransparent(cTransparent, wxSOLID);
    wxPen bTransparentPen(cTransparent, 1, wxSOLID);
    dc.SetBrush(bTransparent);
    dc.SetPen(bTransparentPen);
    if ((m_Bitmap) && (dScale > 0))
    {
        wxMemoryDC memDC;
        memDC.SelectObject(*m_Bitmap);

        memDC.SetUserScale(1 / dScale, 1 / dScale);

        //main bitmap
        dc.Blit(0,0,m_Bitmap->GetWidth() * dScale, m_Bitmap->GetHeight() * dScale,&memDC,0,0, wxCOPY, false);

        //Draw the Hotspot
        if ((iHotSpotX >= 0) && (iHotSpotY >= 0) && (iHotSpotX < m_Bitmap->GetWidth()) && (iHotSpotY <= m_Bitmap->GetHeight()))
        {
            wxMemoryDC memDC_HotSpot;
            wxBitmap bmpHotSpot(1,1);
            int iSize;
            if (dScale < 1) iSize = 1; else iSize = dScale;
            memDC_HotSpot.SelectObject(bmpHotSpot);
            wxPen pHotSpotPen(cHotSpotColour, 1, wxSOLID);
            memDC_HotSpot.SetPen(pHotSpotPen);
            memDC_HotSpot.DrawPoint(0, 0);
            memDC_HotSpot.SetUserScale(1 / dScale, 1 / dScale);
            dc.Blit(iHotSpotX * dScale,iHotSpotY * dScale, iSize, iSize,&memDC_HotSpot,0,0, wxCOPY, false);
        }


        //draw selection
        wxRect rSelection;
        wxMemoryDC memDC2;
        GetBoundingRect(&rSelection);
        memDC2.SelectObject(m_SelectionBitmap);
        memDC2.SetUserScale(1 / dScale, 1 / dScale);
        if ((pSelection) && (NbPoints > 1) && (bDrawSelection) && (memDC2.IsOk()))
        {
            //Selection bitmap
            int iStartX, iStartY, iSourceX, iSourceY, iSelWidth, iSelHeight;
            iStartX = rSelection.GetLeft();
            iStartY = rSelection.GetTop();
            iSourceX = 0;
            iSourceY = 0;
            if (iStartX < 0)
            {
                iStartX = 0;
                iSourceX = -rSelection.GetLeft();
            };

            if (iStartY < 0)
            {
                iStartY = 0;
                iSourceY = -rSelection.GetTop();
            };

            iSelWidth = m_SelectionBitmap.GetWidth() - iSourceX;
            iSelHeight = m_SelectionBitmap.GetHeight() - iSourceY;

            //if (iSelWidth + iStartX > )

            dc.Blit(iStartX * dScale, iStartY * dScale,
                    iSelWidth * dScale, iSelHeight * dScale,
                    &memDC2, iSourceX * dScale, iSourceY * dScale, wxCOPY, true
                   ); //Selection

/*
            dc.Blit(rSelection.GetLeft() * dScale, rSelection.GetTop() * dScale,
                    m_SelectionBitmap.GetWidth() * dScale, m_SelectionBitmap.GetHeight() * dScale,
                    &memDC2, 0, 0, wxCOPY, true); //Selection
*/
        }
    }

    //draw the dynamic tool
    if (m_bDrawToolDynamic)
    {
        wxMemoryDC memDC3;
        memDC3.SelectObject(m_bmDrawBitmap);
        memDC3.SetUserScale(1 / dScale, 1 / dScale);
        /*
        dc.Blit(m_rClip.GetLeft() * dScale, m_rClip.GetTop() * dScale,
                m_rClip.GetWidth() * dScale, m_rClip.GetHeight() * dScale,
                &memDC3, m_rClip.GetLeft() * dScale, m_rClip.GetTop() * dScale, wxCOPY, true
               );*/
        dc.Blit(0, 0,
                m_Bitmap->GetWidth() * dScale, m_Bitmap->GetHeight() * dScale,
                &memDC3, 0, 0, wxCOPY, true
               );
    }


    //draw the grid
    if ((dScale >= 4.0) && (bShowGrid) &&(m_Bitmap))
    {
        //draw the grid
        int i, iMax, j, jMax;
        iMax = m_Bitmap->GetWidth();
        jMax = m_Bitmap->GetHeight();
        wxPen pen(cGridColour, 1);
        dc.SetPen(pen);
        for(i=0;i<iMax+1;i++)
        {
            dc.DrawLine(i * dScale,0,i * dScale,jMax * dScale);
            //dc.DrawLine((i+1) * dScale - 1,0,(i+1) * dScale - 1,jMax * dScale);
        }
        for(j=0;j<jMax+1;j++)
        {
            dc.DrawLine(0,j * dScale,iMax * dScale,j * dScale);
            //dc.DrawLine(0,(j+1) * dScale - 1,iMax * dScale,(j+1) * dScale - 1);
        }

    }

    //clear the background
    wxBrush bBackgroundBrush(cBackgroundColour);
    wxPen pBackgroundPen(cBackgroundColour);
    dc.SetBackground(bBackgroundBrush);
    dc.SetBackgroundMode(wxSOLID);
    if (m_Bitmap)
    {
        wxSize cSize, vSize;
        int xStart, yStart, iXPixelPerScrollUnit, iYPixelPerScrollUnit, iBmpWidth, iBmpHeight;
        cSize = DrawCanvas->GetClientSize();
        vSize = DrawCanvas->GetVirtualSize();
        DrawCanvas->GetViewStart(&xStart, &yStart);
        iXPixelPerScrollUnit = 0;
        iYPixelPerScrollUnit = 0;
        DrawCanvas->GetScrollPixelsPerUnit(&iXPixelPerScrollUnit, &iYPixelPerScrollUnit);
        xStart = xStart * iXPixelPerScrollUnit;
        yStart = yStart * iYPixelPerScrollUnit;
        iBmpWidth = m_Bitmap->GetWidth() * dScale;
        iBmpHeight = m_Bitmap->GetHeight() * dScale;
        dc.SetPen(pBackgroundPen);
        dc.SetBrush(bBackgroundBrush);

        if (cSize.GetWidth() + xStart > iBmpWidth )
        {
            dc.DrawRectangle(iBmpWidth, yStart, cSize.GetWidth() - iBmpWidth + xStart, vSize.GetHeight() - yStart);
            if ((cSize.GetHeight() + yStart > iBmpHeight) && (iBmpWidth + 1 > xStart))
            {
                dc.DrawRectangle(xStart, iBmpHeight, iBmpWidth - xStart + 10, cSize.GetHeight() - iBmpHeight + yStart);
            }
        }
        else if (cSize.GetHeight() + yStart > iBmpHeight)
        {
            dc.DrawRectangle(xStart, iBmpHeight, vSize.GetWidth() - xStart + 10, cSize.GetHeight() - iBmpHeight + yStart);
        }

    }

    //draw the selection border
    if ((pSelection) && (NbPoints > 1) && (bDrawSelection))
    {
        dc.SetLogicalFunction(wxINVERT);
        int iPenWidth;
        if (dScale < 1) iPenWidth = 1; else iPenWidth = 3;
        wxPen pSelectionPen(*wxBLACK, iPenWidth, wxSHORT_DASH);
        dc.SetPen(pSelectionPen);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        wxPoint *tmp;
        tmp = new wxPoint[NbPoints];
        if (tmp)
        {
            int i;
            for(i=0;i<NbPoints;i++)
            {
                tmp[i].x = (pSelection[i].x) * dScale;
                tmp[i].y = (pSelection[i].y) * dScale;
            }
            dc.DrawPolygon(NbPoints, tmp);
            delete[] tmp;
        }
        dc.SetLogicalFunction(wxCOPY);
    }

    //draw sizing border
    if (m_Bitmap)
    {
        iMax = m_Bitmap->GetWidth() * dScale;
        jMax = m_Bitmap->GetHeight() * dScale;
        wxPen pBluePen(*wxBLUE,1,wxSOLID);
        dc.SetPen(pBluePen);
        //blue border
        dc.DrawLine(0,jMax , iMax, jMax);
        dc.DrawLine(iMax, 0, iMax, jMax);

        //5 small rectangles at corners and middles of lines
        wxBrush pBlueBrush(*wxBLUE,wxSOLID);
        dc.SetBrush(pBlueBrush);
        dc.DrawRectangle(0,jMax,4,4);
        dc.DrawRectangle(iMax / 2,jMax,4,4);
        dc.DrawRectangle(iMax,jMax,4,4);
        dc.DrawRectangle(iMax,jMax / 2,4,4);
        dc.DrawRectangle(iMax,0,4,4);
    }

}

//---------------------- ZOOMING & SIZING HANDLERS  -----------------------

/** The user has changed the size of the bitmap in 1 of the 2 spinboxes.
  */
void XPMEditorPanel::OnImageSizeChanged(wxSpinEvent& event)
{

    if (PropertiesPanel)
    {
        int iWidth, iHeight;

        if (PropertiesPanel->BMPWidth) iWidth = PropertiesPanel->BMPWidth->GetValue();
        if (PropertiesPanel->BMPHeight) iHeight = PropertiesPanel->BMPHeight->GetValue();
        if (m_undo_buffer) m_undo_buffer->AddUndo(xpm_size_undo, m_Image);
        SetDrawAreaSize(wxSize(iWidth, iHeight));
    }

}


/** Return the current applied scaling factor (== zoom factor)
  * a factor of 1 indicate no zooming (100%)
  * \return the scale factor (double)
  */
double XPMEditorPanel::GetScaleFactor(void)
{
    //get scaling factor
    return(dScale);
}

/** Set the new  scaling factor (== zoom factor)
  * a factor of 1 indicate no zooming (100%)
  * \param dNewScalingFactor : the new scale factor (double)
  */
void XPMEditorPanel::SetScaleFactor(double dNewScalingFactor)
{
    //Set Scaling Factor
    dScale = dNewScalingFactor;
    wxString s = wxString::Format(_("%d%%"), dScale * 100);
    if (InterfacePanel) InterfacePanel->ZoomFactor->SetValue(s);
    DoSetScrollBars();


    if (InterfacePanel)
    {
        if (dScale >= 4.0)
        {
            InterfacePanel->CheckBox1->Enable(true);
        }
        else
        {
            InterfacePanel->CheckBox1->Disable();
        }
    }
    //UpdateBitmap(); //rescale the memory bitmap

    Repaint();
}

/** Return the size of the current draw area (== the size of the bitmap), in pixel
  * \return the current draw area, in pixel
  */
wxSize XPMEditorPanel::GetDrawAreaSize(void)
{
    //Get Size
    return(sDrawAreaSize);
}

/** Set the size of the draw area (== the size of the bitmap), in pixel
  * \param sNewDrawAreaSize the new draw area, in pixel
  */
void XPMEditorPanel::SetDrawAreaSize(wxSize sNewDrawAreaSize)
{
    //Set Size
    if (DrawCanvas)
    {
        sDrawAreaSize = sNewDrawAreaSize;

        //indicate the size of the bitmap in the spinboxes
        if (PropertiesPanel)
        {
            if (PropertiesPanel->BMPWidth) PropertiesPanel->BMPWidth->SetValue(sDrawAreaSize.GetWidth());
            if (PropertiesPanel->BMPHeight) PropertiesPanel->BMPHeight->SetValue(sDrawAreaSize.GetHeight());
        }


        //resize the image
        if (m_Image)
        {
            m_Image->Resize(sDrawAreaSize, wxPoint(0,0),
                            cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue()
                           );
        }
        UpdateBitmap();

        if (m_Bitmap)
        {
            m_bmDrawBitmap = wxBitmap(m_Bitmap->GetWidth(), m_Bitmap->GetHeight());
        }
        SetModified(true);
    }
}

void XPMEditorPanel::OnDrawCanvasResize(wxSizeEvent& event)
{
    DoSetScrollBars();
    event.Skip();
}

void XPMEditorPanel::DoSetScrollBars(void)
{
    int xStep, yStep, x, y, iWidth, iHeight;
    double dScale2;

    dScale2 = dScale;
    if (dScale2 < 0) dScale2 = - dScale2;
    if (dScale2 == 0) dScale2 = 1;

    if ((m_Bitmap) && (DrawCanvas))
    {
        if (m_Bitmap->GetWidth() < 100)
        {
            xStep = 10;
            iWidth =  m_Bitmap->GetWidth() * dScale2 / xStep;
        }
        else
        {
            xStep = m_Bitmap->GetWidth() / 10;
            iWidth =  m_Bitmap->GetWidth() * dScale2 / xStep;
        }
        if (m_Bitmap->GetHeight() < 100)
        {
            yStep = 10;
            iHeight = m_Bitmap->GetHeight() * dScale2 / yStep;
        }
        else
        {
            yStep = m_Bitmap->GetHeight() / 10;
            iHeight = m_Bitmap->GetHeight() * dScale2 / yStep;
        }

        iWidth = iWidth + 1;   //for resizing border
        iHeight = iHeight + 1; //for resizing border
        DrawCanvas->GetViewStart(&x, &y);
        DrawCanvas->SetScrollbars(  xStep, yStep,
                                    iWidth, iHeight,
                                    x, y, true
                                 );
    }
}

/**Show or hide the grid
  *\param bShow: true if the grid should be displayed, false otherwise
  */
void XPMEditorPanel::ShowGrid(bool bShow)
{
    bShowGrid = bShow;
    Repaint();
}

/** Handler for mouse movement
  */
void XPMEditorPanel::OnDrawCanvasMouseMove(wxMouseEvent& event)
{
    //test for sizing border
    int iWidth, iHeight;

    if (m_Bitmap)
    {
        iWidth = m_Bitmap->GetWidth() * dScale;
        iHeight = m_Bitmap->GetHeight() * dScale;
    }
    else
    {
        if (DrawCanvas) DrawCanvas->GetClientSize(&iWidth, &iHeight); else GetClientSize(&iWidth, &iHeight);
    }

    if ((event.Dragging()) && (bSizingX || bSizingY) && (!bUsingTool))
    {
        //draw a dynamic resize border
        int x, y, xx, yy;

        //draw the rectangle
        wxClientDC dc(DrawCanvas);
        dc.SetUserScale(1,1);
        DrawCanvas->DoPrepareDC(dc);
        wxPen pGrayPen(*wxBLACK, 1,wxSOLID);
        wxColour cBackCol;
        cBackCol = DrawCanvas->GetBackgroundColour();
        wxPen pBackPen(cBackCol, 1,wxSOLID);

        //Get mouse position
        DrawCanvas->GetViewStart(&x, &y);
        if (bSizingX) x = event.m_x; else x = m_Bitmap->GetWidth() * dScale - x;
        if (bSizingY) y = event.m_y; else y = m_Bitmap->GetHeight() * dScale - y ;

        DrawCanvas->CalcUnscrolledPosition(x, y, &xx, &yy);
        //set the text indicating the cursor position
        xx = xx / dScale;
        yy = yy / dScale;
        sCursorPos->SetLabel(wxString::Format(_("Cursor at: x=%d y=%d"),xx,yy));

        //erase the old line
        dc.SetPen(pBackPen);
        dc.DrawLine(OldX, 0, OldX, OldY);
        dc.DrawLine(0,OldY, OldX, OldY);

         //refresh the bitmap
        Repaint();

        if (PropertiesPanel)
        {
            if (PropertiesPanel->BMPWidth) PropertiesPanel->BMPWidth->SetValue(xx);
            if (PropertiesPanel->BMPHeight) PropertiesPanel->BMPHeight->SetValue(yy);
        }


        dc.SetPen(pGrayPen);
        //xx = xx / dScale;
        //yy = yy / dScale;
        dc.DrawLine(xx * dScale, 0, xx * dScale , yy * dScale);
        dc.DrawLine(0,yy * dScale, xx * dScale, yy * dScale);
        OldX = xx * dScale ; OldY = yy * dScale;
    }
    else if (event.Dragging() && (m_bDragging))
    {
        //move the selection
        int x, y;

        event.GetPosition(&x, &y);
        ProcessDragAction(x, y, false, false, true, false);

    }
    else if (event.Dragging() && (m_bSizing))
    {
        //move the selection
        int x, y;

        event.GetPosition(&x, &y);
        ProcessSizeAction(x, y, false, false, true, false, m_iSizeAction);

    }
    else
    {
        //there is no dragging event - just set the cursor and draw the tool
        int x, y, xx, yy;

        event.GetPosition(&x, &y);
        DrawCanvas->CalcUnscrolledPosition(x, y, &xx, &yy);
        //set the text indicating the cursor position
        x = xx;
        y = yy;
        xx = xx / dScale;
        yy = yy / dScale;
        sCursorPos->SetLabel(wxString::Format(_("Cursor at: x=%d y=%d"),xx, yy));

        if ( (bSizingX) && (bSizingY) )
        {
            SetCursor(wxCURSOR_SIZENWSE);
        }
        else if (bSizingX)
        {
            SetCursor(wxCURSOR_SIZEWE);
        }
        else if (bSizingY)
        {
            SetCursor(wxCURSOR_SIZENS);
        }
        else if ((x > iWidth + 10) || (y > iHeight + 10))
        {
            SetCursor(wxCURSOR_CROSS);
            bCanResizeX = false;
            bCanResizeY = false;
        }
        else if (((x >= iWidth) && (x <= iWidth + 10)) && ((y >= iHeight) && (y <= iHeight + 10)))
        {
            SetCursor(wxCURSOR_SIZENWSE);
            bCanResizeX = true;
            bCanResizeY = true;
        }
        else if ((x >= iWidth) && (x <= iWidth + 10))
        {
            SetCursor(wxCURSOR_SIZEWE);
            bCanResizeX = true;
            bCanResizeY = false;
        }
        else if ((y >= iHeight) && (y <= iHeight + 10))
        {
            SetCursor(wxCURSOR_SIZENS);
            bCanResizeX = false;
            bCanResizeY = true;
        }
        else
        {
            //else if ((x >= 0) && (y >= 0))
            bCanResizeX = false;
            bCanResizeY = false;
            int iResult, xxx, yyy;
            event.GetPosition(&xxx, &yyy);
            iResult = IsPointInSelection(xxx, yyy);
            switch(iResult)
            {
                case 1 : SetCursor(wxCURSOR_HAND  ); break;
                case 2 : SetCursor(wxCURSOR_SIZENS); break;
                case 3 : SetCursor(wxCURSOR_SIZEWE); break;
                case 4 : SetCursor(wxCURSOR_SIZENS); break;
                case 5 : SetCursor(wxCURSOR_SIZEWE); break;
                case 6 : SetCursor(wxCURSOR_SIZENWSE); break;
                case 7 : SetCursor(wxCURSOR_SIZENESW); break;
                case 8 : SetCursor(wxCURSOR_SIZENESW); break;
                case 9 : SetCursor(wxCURSOR_SIZENWSE); break;
                default: SetToolCursor();
                         break;
            }
        }

        //draw the tools in action
        int iTool;
        iTool = GetToolID();
        if (x > iWidth) x = iWidth ;
        if (y  > iHeight) y = iHeight;
        ProcessToolAction(iTool, x, y, false, false, event.LeftIsDown(), false);
    }

    event.Skip();
}

void XPMEditorPanel::OnDrawCanvasLeftDClick(wxMouseEvent& event)
{
    int x, y, iWidth, iHeight;
    wxPoint ptPosition;

    ptPosition = event.GetPosition();
    DrawCanvas->CalcUnscrolledPosition(ptPosition.x, ptPosition.y, &x, &y);
    if (m_Bitmap)
    {
        iWidth = m_Bitmap->GetWidth() * dScale;
        iHeight = m_Bitmap->GetHeight() * dScale;
    }
    else
    {
        if (DrawCanvas) DrawCanvas->GetClientSize(&iWidth, &iHeight); else GetClientSize(&iWidth, &iHeight);
    }

    int iTool;
    iTool = GetToolID();
    if (x > iWidth) x = iWidth ;
    if (y  > iHeight) y = iHeight;

    int iResult;
    iResult = IsPointInSelection(ptPosition.x, ptPosition.y);
    switch(iResult)
    {
        case 1 : if (    (!bUsingTool)
                          || ((GetToolID() != XPM_ID_SELECT_TOOL) && (GetToolID() != XPM_ID_LASSO_TOOL))
                         )
                 {
                     ProcessDragAction(x, y, false, false, false, true);
                 }
                 else
                 {
                     ProcessToolAction(iTool, x, y, false, false, false, true);
                 }
                 break;
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 : if (    (!bUsingTool)
                          || ((GetToolID() != XPM_ID_SELECT_TOOL) && (GetToolID() != XPM_ID_LASSO_TOOL))
                         )
                 {
                     ProcessSizeAction(x, y, false, false, false, true, iResult); break;
                 }
                 else
                 {
                     ProcessToolAction(iTool, x, y, false, false, false, true);
                 }
                 break;
        case 0 :
        default: ProcessToolAction(iTool, x, y, false, false, false, true); break;
    }

    event.Skip();
}


void XPMEditorPanel::OnDrawCanvasRightUp(wxMouseEvent& event)
{
    int x, y, iWidth, iHeight;
    wxPoint ptPosition;

    ptPosition = event.GetPosition();
    DrawCanvas->CalcUnscrolledPosition(ptPosition.x, ptPosition.y, &x, &y);
    if (m_Bitmap)
    {
        iWidth = m_Bitmap->GetWidth() * dScale;
        iHeight = m_Bitmap->GetHeight() * dScale;
    }
    else
    {
        if (DrawCanvas) DrawCanvas->GetClientSize(&iWidth, &iHeight); else GetClientSize(&iWidth, &iHeight);
    }

    int iTool;
    iTool = GetToolID();
    if (x > iWidth) x = iWidth ;
    if (y  > iHeight) y = iHeight;

    //simulate a double click
    ProcessToolAction(iTool, x, y, true, false, false, false);
    ProcessToolAction(iTool, x, y, false, true, false, false);
    ProcessToolAction(iTool, x, y, false, false, true, false);
    ProcessToolAction(iTool, x, y, false, false, false, true);

    event.Skip();
}


/** draw the tool action: select box for Selection tool, rectangle for rectangle tool, ...
  * \param iTool: the index of the tool used
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessToolAction(int iTool, int x, int y,
                                         bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    //draw the tool action: select box for Selection tool, rectangle for rectangle tool, ...

    switch(iTool)
    {
        case XPM_ID_SELECT_TOOL :
                    ProcessSelect(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_LASSO_TOOL  :
                    ProcessLasso(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_PEN_TOOL :
                    ProcessPen(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_BRUSH_TOOL :
                    ProcessBrush(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_PIPETTE_TOOL :
                    ProcessPipette(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_FILL_TOOL :
                    ProcessFill(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_CURVE_TOOL :
                    ProcessCurve(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_LINE_TOOL :
                    ProcessLine(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_ERASER_TOOL :
                    ProcessEraser(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_TEXT_TOOL :
                    ProcessText(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_RECTANGLE_TOOL :
                    ProcessRectangle(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_POLYGON_TOOL :
                    ProcessPolygon(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_ELLIPSE_TOOL :
                    ProcessEllipse(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_ROUNDEDRECT_TOOL :
                    ProcessRoundedRectangle(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_DRAG_TOOL:
                    ProcessDragAction(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        case XPM_ID_STRETCH_TOOL:
                    ProcessSizeAction(x, y, bLeftDown, bLeftUp, bPressed, bDClick, IsPointInSelection(x,y));
                    break;

        case XPM_ID_HOTSPOT_TOOL:
                    ProcessHotSpot(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
                    break;

        default: break;
    }

}

/** Process the Pipette tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessPipette(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {
        //get the pixel color
        if (m_Image)
        {
            int iColour;
            unsigned char iRed, iGreen, iBlue;

            bUsingTool = true;
            iRed = m_Image->GetRed(x / dScale, y / dScale);
            iGreen = m_Image->GetGreen(x / dScale, y / dScale);
            iBlue = m_Image->GetBlue(x / dScale, y / dScale);
            wxColour cColour(iRed, iGreen, iBlue);
            iColour = ColourPicker->GetLineColourIndex();
            ColourPicker->SetPaletteColour(iColour, cColour);
            ColourPicker->Refresh(false,NULL);
            ColourPicker->Update();
        }
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the HotSpot tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessHotSpot(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {
        //get the pixel color
        iHotSpotX = x / dScale;
        iHotSpotY = y / dScale;

        SetModified(true);

        Repaint();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Fill tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessFill(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {
        //Undo & modification flag
        AddUndo();
        SetModified(true);
        bUsingTool = true;

        //draw the pixel
        if (!m_Bitmap) return;
        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            wxColour cSurfaceColour;
            cColour = ColourPicker->GetFillColour();
            wxBrush brush(cColour, wxSOLID);
            mem_dc.SetBrush(brush);
            mem_dc.GetPixel(x / dScale, y / dScale, &cSurfaceColour);
            mem_dc.FloodFill(x / dScale, y / dScale,cSurfaceColour, wxFLOOD_SURFACE);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Pen tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessPen(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{

    if (bLeftDown)
    {
        //Undo & modification flag
        if (!m_Bitmap) return;
        AddUndo();
        SetModified(true);
        bUsingTool = true;

        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            cColour = ColourPicker->GetLineColour();
            wxPen pen(cColour, 1, wxSOLID);
            mem_dc.SetPen(pen);

            int xx, yy;
            xx = x / dScale; yy = y / dScale;
            mem_dc.DrawPoint(xx, yy);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }


    if (bPressed)
    {
        //draw the pixel
        if (!m_Bitmap) return;
        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            cColour = ColourPicker->GetLineColour();
            wxPen pen(cColour, 1, wxSOLID);
            mem_dc.SetPen(pen);

            int xx, yy;
            xx = x / dScale; yy = y / dScale;
            mem_dc.DrawPoint(xx, yy);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Text tool in action
  * a text control will be shown - this text control can be resized
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessText(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    //this is mainly like the rectangular selection tool
    //the idea is that the selection bitmap (m_SelectionBitmap) will be drawn directly on
    //with the text
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks < 2) ProcessSelect(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
    }

    //left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            ProcessSelect(x, y, bLeftDown, bLeftUp, bPressed, bDClick);
        }
        else if (tdata.iNbClicks == 1)
        {
            NbPoints = 4;
            pSelection = CheckMemorySelection(4);
            if (pSelection)
            {
                pSelection[0].x = tdata.x1;
                pSelection[0].y = tdata.y1;
                pSelection[1].x = x / dScale;
                pSelection[1].y = tdata.y1;
                pSelection[2].x = x / dScale;
                pSelection[2].y = y / dScale;
                pSelection[3].x = tdata.x1;
                pSelection[3].y = y / dScale;
            }

            m_SelectionBitmap = wxBitmap(x / dScale - tdata.x1 + 1, y / dScale - tdata.y1 + 1);
            tdata.sText = _("");
            TextEdit->ChangeValue(tdata.sText);
            DrawTextBitmap();

            Repaint();
            //InitToolData();
            m_bEraseSelection = false;
            tdata.iNbClicks = 2;

            TextEdit->Show(true);
            ResizeCtrl1->Show(true);
        }
        else
        {
            AddUndo();
            SetModified(true);
            ToggleButtons(-1, false);
            if (ToolPanel)  ToolPanel->HideControlsAndDoLayout(-1, false);
            Repaint();
        }
    }
}

/** Process the Selection tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessSelect(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            Repaint();

            wxClientDC dc(DrawCanvas);
            dc.SetLogicalFunction(wxINVERT);
            DrawCanvas->DoPrepareDC(dc);
            int iPenWidth;
            if (dScale < 1) iPenWidth = 1; else iPenWidth = 3;
            wxPen pSelectionPen(*wxBLACK, iPenWidth, wxSHORT_DASH);
            dc.SetPen(pSelectionPen);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            tdata.x2 = x;
            tdata.y2 = y;
            int x1,y1,x2,y2;
            SnapRectToGrid(&x1,&y1,&x2,&y2);
            dc.DrawRectangle(x1, y1,
                             (x2 - x1), (y2 - y1));
            dc.SetLogicalFunction(wxCOPY);
        }
    }

    //left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            ClearSelection();
            Repaint();
            tdata.iNbClicks = 1;
            tdata.x1 = x;
            tdata.y1 = y;
            bUsingTool = true;
        }
        else
        {
            NbPoints = 4;
            pSelection = CheckMemorySelection(4);
            tdata.x2 = x;
            tdata.y2 = y;
            int x1,y1,x2,y2;
            SnapRectToGrid(&x1,&y1,&x2,&y2);
            if (pSelection)
            {
                pSelection[0].x = x1 / dScale;
                pSelection[0].y = y1 / dScale;
                pSelection[1].x = x2 / dScale;
                pSelection[1].y = y1 / dScale;
                pSelection[2].x = x2 / dScale;
                pSelection[2].y = y2 / dScale;
                pSelection[3].x = x1 / dScale;
                pSelection[3].y = y2 / dScale;
            }
            m_SelectionImage = GetImageFromSelection();
            Repaint();
            InitToolData();
            m_bEraseSelection = true;
        }
    }
}

/** Process the Drag & Drop tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessDragAction(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {
        //Release the mouse capture to prevent weird behaviour (cursor seems blocked)
        //this is due to the fact that the mouse is captured again during the drag action
        if (DrawCanvas)
        {
            if (DrawCanvas->HasCapture()) DrawCanvas->ReleaseMouse();
        }

        //Undo & modification flag
        AddUndo();
        SetModified(true);
        bUsingTool = true;
        m_bDragging = true;

        //save the image to drag
        wxRect rSelection;
        GetBoundingRect(&rSelection);

        //get the drag image
        wxImage img(m_SelectionImage);
        img.Rescale(img.GetWidth() * dScale, img.GetHeight() * dScale);
        wxBitmap bmp(img);
        if (m_DragImage) delete(m_DragImage);
        m_DragImage = new wxDragImage(bmp, wxCursor(wxCURSOR_HAND));
        if (!m_DragImage) return;
        if (!m_Bitmap) return;

        //replace the selection by a transparent image
        if (m_bEraseSelection)
        {
            CutSelection();
        }


        //begin the drag
        //wxPoint ptHotSpot(rSelection.GetWidth() * dScale / 2, rSelection.GetHeight() * dScale / 2);

        wxPoint ptHotSpot(x - rSelection.GetLeft() * dScale, y - rSelection.GetTop() * dScale);

        pStartDragging.x = x / dScale - rSelection.GetLeft();
        pStartDragging.y = y / dScale - rSelection.GetTop() ;
        m_DragImage->BeginDrag(ptHotSpot, DrawCanvas, false, NULL);
        m_DragImage->Hide();

        //Update Image
        Repaint();
        m_DragImage->Move(wxPoint(x, y));
        m_DragImage->Show();


    }
    else if ((bPressed) && (m_bDragging))
    {

        if (!m_Bitmap) return;
        if (!m_DragImage) return;

        //m_DragImage->Hide();
        Repaint();
        m_DragImage->Move(wxPoint(x,y));
        m_DragImage->Show();

    }
    else if (bLeftUp)
    {

        //finish
        m_bDragging = false;

        if (m_DragImage)
        {
            m_DragImage->EndDrag();
            delete(m_DragImage);
            m_DragImage = NULL;

            int xx, yy;
            DrawCanvas->CalcUnscrolledPosition (x, y, &xx, &yy);

            wxRect rSelection;
            GetBoundingRect(&rSelection);

            //offset the selection
            //MoveSelection(xx / dScale - m_SelectionImage.GetWidth() / 2 - rSelection.GetLeft(),
            //              yy / dScale - m_SelectionImage.GetHeight()/ 2 - rSelection.GetTop());
            MoveSelection(xx / dScale - pStartDragging.x - rSelection.GetLeft(),
                          yy / dScale - pStartDragging.y - rSelection.GetTop());

            SetCursor(wxCURSOR_HAND);
        }
        m_bEraseSelection = false;
        Repaint();

        InitToolData();
    }
}

/** Process the stretch selection tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  * \param iDirection: the direction of the stretch
  *            2: top; 3 : left; 4: bottom; 5 : right;
  *            6:top left;      7: top right;
  *            8: bottom left;  9 bottom right
  */
void XPMEditorPanel::ProcessSizeAction(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick, int iDirection)
{
    if ((bLeftDown) || (bPressed))
    {
        if (!m_Bitmap) return;
        m_bSizing = true;
        m_iSizeAction = iDirection;

        //wxMessageBox(wxString::Format(_("iDirection=%d"), iDirection));

        ToggleButtons(XPM_ID_STRETCH_TOOL, false);
        ConvertSelectionToRect();
        wxRect rSelection;
        GetBoundingRect(&rSelection);
        m_bEraseSelection = false;

        int iLeft, iTop, iRight, iBottom, xx, yy;
        DrawCanvas->CalcUnscrolledPosition(x, y, &xx, &yy);
        iLeft = rSelection.GetLeft();
        iRight = rSelection.GetRight();
        iTop = rSelection.GetTop();
        iBottom = rSelection.GetBottom();
        if ((iDirection == 2) || (iDirection == 6) || (iDirection == 7)) iTop = yy / dScale;
        if ((iDirection == 3) || (iDirection == 6) || (iDirection == 8)) iLeft = xx / dScale;
        if ((iDirection == 4) || (iDirection == 8) || (iDirection == 9)) iBottom = yy / dScale;
        if ((iDirection == 5) || (iDirection == 7) || (iDirection == 9)) iRight = xx / dScale;

        if (m_SelectionImage.IsOk())
        {
            m_SelectionImage.Rescale(iRight - iLeft, iBottom - iTop, wxIMAGE_QUALITY_NORMAL);
            m_SelectionBitmap = wxBitmap(m_SelectionImage);

            if (NbPoints == 4)
            {
                pSelection[0].x = iLeft;
                pSelection[0].y = iTop;

                pSelection[1].x = iRight;
                pSelection[1].y = iTop;

                pSelection[2].x = iRight;
                pSelection[2].y = iBottom;

                pSelection[3].x = iLeft;
                pSelection[3].y = iBottom;
            }
        }

        Repaint();
    }

    if (bLeftUp)
    {
        //finish
        m_bSizing = false;
        InitToolData();
    }
}

/** Process the Lasso (complex selection) tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessLasso(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            bDrawSelection = false;
            Repaint();
            bDrawSelection = true;

            wxClientDC dc(DrawCanvas);
            dc.SetLogicalFunction(wxINVERT);
            DrawCanvas->DoPrepareDC(dc);
            int iPenWidth;
            if (dScale < 1) iPenWidth = 1; else iPenWidth = 3;
            wxPen pSelectionPen(*wxBLACK, iPenWidth, wxSHORT_DASH);
            dc.SetPen(pSelectionPen);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.SetLogicalFunction(wxCOPY);

            wxPoint *tmp;
            tmp = new wxPoint[NbPoints + 1];
            if (tmp)
            {
                int i;
                tmp[NbPoints].x = x;
                tmp[NbPoints].y = y;
                for(i=0;i<NbPoints;i++)
                {
                    tmp[i].x = pSelection[i].x * dScale;
                    tmp[i].y = pSelection[i].y * dScale;
                }
                dc.DrawPolygon(NbPoints + 1, tmp);
                delete[] tmp;
            }
        }
    }

    // left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            ClearSelection();
            bUsingTool = true;
            Repaint();
            tdata.iNbClicks = 1;
            tdata.x1 = x;
            tdata.y1 = y;
            NbPoints = 1;
            pSelection = CheckMemorySelection(1);
            if (pSelection)
            {
                pSelection[0].x = x / dScale;
                pSelection[0].y = y / dScale;
            }
        }
        else
        {
            NbPoints = NbPoints + 1;
            pSelection = CheckMemorySelection(NbPoints);
            if (pSelection)
            {
                pSelection[NbPoints - 1].x = x / dScale;
                pSelection[NbPoints - 1].y = y / dScale;
            }
            m_SelectionImage = GetImageFromSelection();
            Repaint();
        }
    }

    if (bDClick)
    {
        ToggleButtons(-1, false);
        bUsingTool = false;
        m_bEraseSelection = true;
    }
}

/** Process the Brush tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessBrush(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {
        //Undo & modification flag
        if (!m_Bitmap) return;
        AddUndo();
        SetModified(true);
        bUsingTool = true;

        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            cColour = ColourPicker->GetLineColour();
            wxBrush brush(cColour, wxSOLID);
            wxPen pen(cColour, 1, wxSOLID);
            mem_dc.SetBrush(brush);
            mem_dc.SetPen(pen);

            int xx, yy, i, iSize;
            xx = x / dScale; yy = y / dScale;
            iSize = tdata.iSize2;

            switch (tdata.iStyle)
            {
                case XPM_BRUSH_STYLE_CIRCLE   :
                        mem_dc.DrawCircle(xx, yy, iSize/2);
                        break;

                case XPM_BRUSH_STYLE_LEFTHAIR :
                        for(i=0; i < iSize; i++)
                        {
                            mem_dc.DrawPoint(xx - iSize / 2 + i, yy + iSize / 2 - i);
                        }
                        //mem_dc.DrawLine(xx - tdata.iSize2 /2.828 ,yy + tdata.iSize2 / 2.828, xx + tdata.iSize2 / 2.828, yy - tdata.iSize2 / 2.828);
                        break;

                case XPM_BRUSH_STYLE_RIGHTHAIR:
                        for(i=0; i < iSize; i++)
                        {
                            mem_dc.DrawPoint(xx - iSize / 2 + i, yy - iSize / 2 + i);
                        }
                        //mem_dc.DrawLine(xx - tdata.iSize2 / 2.828, yy - tdata.iSize2 /2.828, xx + tdata.iSize2 /2.828, yy + tdata.iSize2 / 2.828);
                        break;

                case XPM_BRUSH_STYLE_SQUARE   :
                default:
                        mem_dc.DrawRectangle(xx - iSize / 2,
                                             yy - iSize / 2,
                                             iSize,
                                             iSize);
                        break;
            }
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }

    if (bPressed)
    {
        if (!m_Bitmap) return;
        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            cColour = ColourPicker->GetLineColour();
            wxBrush brush(cColour, wxSOLID);
            wxPen pen(cColour, 1, wxSOLID);
            mem_dc.SetBrush(brush);
            mem_dc.SetPen(pen);

            int xx, yy, i, iSize;
            xx = x / dScale; yy = y / dScale;
            iSize = tdata.iSize2;

            switch (tdata.iStyle)
            {
                case XPM_BRUSH_STYLE_CIRCLE   :
                    mem_dc.DrawCircle(xx, yy, iSize/2);
                    break;

                case XPM_BRUSH_STYLE_LEFTHAIR :
                    for(i=0; i < iSize; i++)
                    {
                            mem_dc.DrawPoint(xx - iSize / 2 + i, yy + iSize / 2 - i);
                    }
                    //mem_dc.DrawLine(xx - tdata.iSize2 /2.828,yy + tdata.iSize2 / 2.828, xx + tdata.iSize2 / 2.828, yy - tdata.iSize2 / 2.828);
                    break;

                case XPM_BRUSH_STYLE_RIGHTHAIR:
                    for(i=0; i < iSize; i++)
                    {
                            mem_dc.DrawPoint(xx - iSize / 2 + i, yy - iSize / 2 + i);
                    }
                    //mem_dc.DrawLine(xx - tdata.iSize2 / 2.828, yy - tdata.iSize2 / 2.828, xx + tdata.iSize2 / 2.828, yy + tdata.iSize2 / 2.828);
                    break;

                case XPM_BRUSH_STYLE_SQUARE   :
                default:
                    mem_dc.DrawRectangle(xx - iSize / 2,
                                         yy - iSize / 2,
                                         iSize,
                                         iSize);
                    break;
            }
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Eraser tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessEraser(int x, int y,
                                      bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {
        //Undo & modification flag
        if (!m_Bitmap) return;
        AddUndo();
        SetModified(true);
        bUsingTool = true;

        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            cColour = ColourPicker->GetTransparentColour();
            wxBrush brush(cColour, wxSOLID);
            wxPen pen(cColour, 1, wxSOLID);
            mem_dc.SetBrush(brush);
            mem_dc.SetPen(pen);

            int xx, yy;
            xx = x / dScale; yy = y / dScale;
            mem_dc.DrawRectangle(xx - tdata.iSize2 / 2,
                                 yy - tdata.iSize2 / 2,
                                 tdata.iSize2,
                                 tdata.iSize2);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }

    if (bPressed)
    {
        if (!m_Bitmap) return;
        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cColour;
            cColour = ColourPicker->GetTransparentColour();
            wxBrush brush(cColour, wxSOLID);
            wxPen pen(cColour, 1, wxSOLID);
            mem_dc.SetBrush(brush);
            mem_dc.SetPen(pen);

            int xx, yy;
            xx = x / dScale; yy = y / dScale;

            mem_dc.DrawRectangle(xx - tdata.iSize2 / 2,
                                 yy - tdata.iSize2 / 2,
                                 tdata.iSize2,
                                 tdata.iSize2);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        Repaint();
    }
    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Polygon tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessPolygon(int x, int y,
                                      bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            //Draw the polygon
            wxMemoryDC memDC;
            if ((memDC.IsOk()) && (dScale > 0))
            {
                memDC.SelectObject(m_bmDrawBitmap);

                wxColour cLineColour, cFillColour, cBackColour;
                cFillColour = ColourPicker->GetFillColour();
                cLineColour = ColourPicker->GetLineColour();
                cBackColour = ColourPicker->GetUnusedColour();
                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                wxBrush BackBrush(cBackColour, wxSOLID);
                memDC.SetBackground(BackBrush);
                memDC.SetBrush(brush);
                memDC.SetPen(pen);

                memDC.Clear();
                wxPoint tmp[XPM_MAXPOINTS+1];
                int i;
                tmp[tdata.iNbPoints].x = x / dScale;
                tmp[tdata.iNbPoints].y = y / dScale;
                for(i=0;i<tdata.iNbPoints;i++)
                {
                    tmp[i].x = tdata.pts[i].x;
                    tmp[i].y = tdata.pts[i].y;
                }
                memDC.DrawPolygon(tdata.iNbPoints + 1, tmp);


                memDC.SelectObject(wxNullBitmap);
                wxMask *mask;
                mask = new wxMask(m_bmDrawBitmap, cBackColour);
                if (mask) m_bmDrawBitmap.SetMask(mask);

                m_bDrawToolDynamic = true;

                Repaint();
            }
        }
    }

    // left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            Repaint();
            tdata.iNbClicks = 1;
            tdata.iNbPoints = 1;
            tdata.pts[0].x = x  / dScale;
            tdata.pts[0].y = y / dScale;
            bUsingTool = true;
            m_bDrawToolDynamic = true;
        }
        else
        {
            tdata.iNbClicks = tdata.iNbClicks + 1;
            tdata.iNbPoints = tdata.iNbPoints + 1;
            if (tdata.iNbPoints >= XPM_MAXPOINTS) tdata.iNbPoints = XPM_MAXPOINTS;
            tdata.pts[tdata.iNbPoints - 1].x = x  / dScale;
            tdata.pts[tdata.iNbPoints - 1].y = y / dScale;
            m_bDrawToolDynamic = true;
        }
    }

    if (bDClick)
    {
        AddUndo();
        SetModified(true);

        if (!m_Bitmap) return;
        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cLineColour, cFillColour;
            cLineColour = ColourPicker->GetLineColour();
            cFillColour = ColourPicker->GetFillColour();

            wxPen pen(cLineColour, tdata.iSize, wxSOLID);
            wxBrush brush(cFillColour, wxSOLID);
            mem_dc.SetPen(pen);
            mem_dc.SetBrush(brush);
            mem_dc.DrawPolygon(tdata.iNbPoints, tdata.pts);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();

        m_bDrawToolDynamic = false;
        Repaint();
        InitToolData();
        ToggleButtons(-1,false);
    }
}

/** Process the Rectangle tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessRectangle(int x, int y,
                                      bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            tdata.x2 = x;
            tdata.y2 = y;
            int x1,y1,x2,y2;
            SnapRectToGrid(&x1,&y1,&x2,&y2);

            wxMemoryDC memDC;
            if ((memDC.IsOk()) && (dScale > 0))
            {
                memDC.SelectObject(m_bmDrawBitmap);

                wxColour cLineColour, cFillColour, cBackColour;
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();
                cBackColour = ColourPicker->GetUnusedColour();
                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                wxBrush BackBrush(cBackColour, wxSOLID);
                memDC.SetBackground(BackBrush);

                memDC.SetPen(pen);
                memDC.SetBrush(brush);
                x1 = x1 / dScale; y1 = y1 / dScale;
                x2 = x2 / dScale; y2 = y2 / dScale;

                memDC.Clear();
                memDC.DrawRectangle(x1 + tdata.iSize - 1, y1 + tdata.iSize - 1,
                                    (x2 - x1) - tdata.iSize + 1, (y2 - y1) - tdata.iSize + 1);
                memDC.SelectObject(wxNullBitmap);
                wxMask *mask;
                mask = new wxMask(m_bmDrawBitmap, cBackColour);
                if (mask) m_bmDrawBitmap.SetMask(mask);

                m_bDrawToolDynamic = true;

                Repaint();
            }
        }
    }

    //left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            tdata.iNbClicks = 1;
            tdata.x1 = x;
            tdata.y1 = y;
            bUsingTool = true;
            m_bDrawToolDynamic = true;
        }
        else
        {
            if (!m_Bitmap) return;
            AddUndo();
            SetModified(true);
            wxMemoryDC mem_dc(*m_Bitmap);
            if (mem_dc.IsOk())
            {
                wxColour cLineColour, cFillColour;
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();

                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                mem_dc.SetPen(pen);
                mem_dc.SetBrush(brush);
                tdata.x2 = x;
                tdata.y2 = y;
                int x1,y1,x2,y2;
                SnapRectToGrid(&x1,&y1,&x2,&y2);
                x1 = x1 / dScale; y1 = y1 / dScale;
                x2 = x2 / dScale; y2 = y2 / dScale;
                mem_dc.DrawRectangle(x1 + tdata.iSize - 1, y1  + tdata.iSize - 1,
                                     x2 - x1 - tdata.iSize + 1 , y2 - y1 - tdata.iSize + 1);

                mem_dc.SelectObject(wxNullBitmap);
            }
            UpdateImage();
            m_bDrawToolDynamic = false;
            Repaint();
            InitToolData();
        }
    }
}

/** Process the Line tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessLine(int x, int y,
                                bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            tdata.x2 = x;
            tdata.y2 = y;
            int x1,y1,x2,y2;
            //SnapRectToGrid(&x1,&y1,&x2,&y2, false);
            x1 = tdata.x1;
            y1 = tdata.y1;
            x2 = x;
            y2 = y;

            wxMemoryDC memDC;
            if ((memDC.IsOk()) && (dScale > 0))
            {
                memDC.SelectObject(m_bmDrawBitmap);

                wxColour cLineColour, cFillColour, cBackColour;
                cLineColour = ColourPicker->GetLineColour();
                cBackColour = ColourPicker->GetUnusedColour();
                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush BackBrush(cBackColour, wxSOLID);
                memDC.SetBackground(BackBrush);

                memDC.SetPen(pen);
                x1 = x1 / dScale;
                y1 = y1 / dScale;
                x2 = x2 / dScale;
                y2 = y2 / dScale;

                memDC.Clear();
                memDC.DrawLine(x1 , y1, x2, y2);
                memDC.DrawPoint(x2, y2);
                memDC.SelectObject(wxNullBitmap);
                wxMask *mask;
                mask = new wxMask(m_bmDrawBitmap, cBackColour);
                if (mask) m_bmDrawBitmap.SetMask(mask);

                m_bDrawToolDynamic = true;

                Repaint();
            }
        }
    }

    //left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            tdata.iNbClicks = 1;
            tdata.x1 = x;
            tdata.y1 = y;
            bUsingTool = true;
            m_bDrawToolDynamic = true;
        }
        else
        {
            if (!m_Bitmap) return;
            AddUndo();
            SetModified(true);
            wxMemoryDC mem_dc(*m_Bitmap);
            if (mem_dc.IsOk())
            {
                wxColour cLineColour, cFillColour;
                cLineColour = ColourPicker->GetLineColour();

                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                mem_dc.SetPen(pen);

                int x1,y1,x2,y2;
                x1 = tdata.x1;
                y1 = tdata.y1;
                x2 = x;
                y2 = y;

                x1 = x1 / dScale;
                y1 = y1 / dScale;
                x2 = x2 / dScale;
                y2 = y2 / dScale;
                mem_dc.DrawLine(x1, y1, x2, y2);
                mem_dc.DrawPoint(x2, y2);

                mem_dc.SelectObject(wxNullBitmap);
            }
            UpdateImage();
            m_bDrawToolDynamic = false;
            Repaint();
            InitToolData();
        }
    }
}

/** Process the Curve tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessCurve(int x, int y,
                                bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            //Draw the polygon
            wxMemoryDC memDC;
            if ((memDC.IsOk()) && (dScale > 0))
            {
                memDC.SelectObject(m_bmDrawBitmap);

                wxColour cLineColour, cFillColour, cBackColour;
                cFillColour = ColourPicker->GetFillColour();
                cLineColour = ColourPicker->GetLineColour();
                cBackColour = ColourPicker->GetUnusedColour();
                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                wxBrush BackBrush(cBackColour, wxSOLID);
                memDC.SetBackground(BackBrush);
                memDC.SetBrush(brush);
                memDC.SetPen(pen);

                memDC.Clear();
                wxPoint tmp[XPM_MAXPOINTS+1];
                int i;
                tmp[tdata.iNbPoints].x = x / dScale;
                tmp[tdata.iNbPoints].y = y / dScale;
                for(i=0;i<tdata.iNbPoints;i++)
                {
                    tmp[i].x = tdata.pts[i].x;
                    tmp[i].y = tdata.pts[i].y;
                }
                memDC.DrawSpline(tdata.iNbPoints + 1, tmp);

                memDC.SelectObject(wxNullBitmap);
                wxMask *mask;
                mask = new wxMask(m_bmDrawBitmap, cBackColour);
                if (mask) m_bmDrawBitmap.SetMask(mask);

                m_bDrawToolDynamic = true;

                Repaint();
            }
        }
    }

    // left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            Repaint();
            tdata.iNbClicks = 1;
            tdata.iNbPoints = 1;
            int x1, y1;
            x1 = x;
            y1 = y;
            SnapToGrid(&x1, &y1, true);
            tdata.pts[0].x = x1  / dScale;
            tdata.pts[0].y = y1 / dScale;
            bUsingTool = true;
            m_bDrawToolDynamic = true;
        }
        else
        {
            tdata.iNbClicks = tdata.iNbClicks + 1;
            tdata.iNbPoints = tdata.iNbPoints + 1;
            if (tdata.iNbPoints >= XPM_MAXPOINTS) tdata.iNbPoints = XPM_MAXPOINTS;
            int x1, y1;
            x1 = x;
            y1 = y;
            SnapToGrid(&x1, &y1, true);
            tdata.pts[tdata.iNbPoints - 1].x = x1  / dScale;
            tdata.pts[tdata.iNbPoints - 1].y = y1 / dScale;
            m_bDrawToolDynamic = true;
        }
    }

    if (bDClick)
    {
        AddUndo();
        SetModified(true);

        if (!m_Bitmap) return;
        wxMemoryDC mem_dc(*m_Bitmap);
        if (mem_dc.IsOk())
        {
            wxColour cLineColour;
            cLineColour = ColourPicker->GetLineColour();

            wxPen pen(cLineColour, tdata.iSize, wxSOLID);
            mem_dc.SetPen(pen);
            mem_dc.DrawSpline(tdata.iNbPoints, tdata.pts);
            mem_dc.SelectObject(wxNullBitmap);
        }
        UpdateImage();
        m_bDrawToolDynamic = false;

        Repaint();
        InitToolData();
        ToggleButtons(-1,false);
    }

}

/** Process the RoundedRectangle tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessRoundedRectangle(int x, int y,
                                      bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            tdata.x2 = x;
            tdata.y2 = y;
            int x1,y1,x2,y2;
            SnapRectToGrid(&x1,&y1,&x2,&y2);

            wxMemoryDC memDC;
            if ((memDC.IsOk()) && (dScale > 0))
            {
                memDC.SelectObject(m_bmDrawBitmap);

                wxColour cLineColour, cFillColour, cBackColour;
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();
                cBackColour = ColourPicker->GetUnusedColour();
                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                wxBrush BackBrush(cBackColour, wxSOLID);
                memDC.SetBackground(BackBrush);

                memDC.SetPen(pen);
                memDC.SetBrush(brush);
                x1 = x1 / dScale; y1 = y1 / dScale;
                x2 = x2 / dScale; y2 = y2 / dScale;

                memDC.Clear();
                memDC.DrawRoundedRectangle(x1 + tdata.iSize - 1, y1 + tdata.iSize - 1,
                                    (x2 - x1) - tdata.iSize + 1, (y2 - y1) - tdata.iSize + 1, tdata.iRadius);
                memDC.SelectObject(wxNullBitmap);
                wxMask *mask;
                mask = new wxMask(m_bmDrawBitmap, cBackColour);
                if (mask) m_bmDrawBitmap.SetMask(mask);

                m_bDrawToolDynamic = true;

                Repaint();
            }
        }
    }

    //left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            tdata.iNbClicks = 1;
            tdata.x1 = x;
            tdata.y1 = y;
            bUsingTool = true;
            m_bDrawToolDynamic = true;
        }
        else
        {
            if (!m_Bitmap) return;
            AddUndo();
            SetModified(true);
            wxMemoryDC mem_dc(*m_Bitmap);
            if (mem_dc.IsOk())
            {
                wxColour cLineColour, cFillColour;
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();

                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                mem_dc.SetPen(pen);
                mem_dc.SetBrush(brush);
                tdata.x2 = x;
                tdata.y2 = y;
                int x1,y1,x2,y2;
                SnapRectToGrid(&x1,&y1,&x2,&y2);
                x1 = x1 / dScale; y1 = y1 / dScale;
                x2 = x2 / dScale; y2 = y2 / dScale;
                mem_dc.DrawRoundedRectangle(x1  + tdata.iSize - 1, y1  + tdata.iSize - 1,
                                            x2 - x1 - tdata.iSize + 1 , y2 - y1 - tdata.iSize + 1,
                                            tdata.iRadius);

                mem_dc.SelectObject(wxNullBitmap);
            }
            UpdateImage();
            m_bDrawToolDynamic = false;

            Repaint();
            InitToolData();
        }
    }
}

/** Process the Ellipse tool in action
  * \param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * \param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * \param bLeftDown: true if a mouse left button DOWN event has been triggered
  * \param bLeftUp: true if a mouse left button UP event has been triggered
  * \param bPressed: true if the left mouse button is currently pressed
  * \param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessEllipse(int x, int y,
                                    bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
    {
        if (tdata.iNbClicks > 0)
        {
            tdata.x2 = x;
            tdata.y2 = y;
            int x1,y1,x2,y2;
            SnapRectToGrid(&x1,&y1,&x2,&y2);

            wxMemoryDC memDC;
            if ((memDC.IsOk()) && (dScale > 0))
            {
                memDC.SelectObject(m_bmDrawBitmap);

                wxColour cLineColour, cFillColour, cBackColour;
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();
                cBackColour = ColourPicker->GetUnusedColour();
                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                wxBrush BackBrush(cBackColour, wxSOLID);
                memDC.SetBackground(BackBrush);

                memDC.SetPen(pen);
                memDC.SetBrush(brush);
                x1 = x1 / dScale; y1 = y1 / dScale;
                x2 = x2 / dScale; y2 = y2 / dScale;

                memDC.Clear();
                memDC.DrawEllipse(x1 + tdata.iSize - 1, y1 + tdata.iSize - 1,
                                 (x2 - x1) - tdata.iSize + 1, (y2 - y1) - tdata.iSize + 1);
                memDC.SelectObject(wxNullBitmap);
                wxMask *mask;
                mask = new wxMask(m_bmDrawBitmap, cBackColour);
                if (mask) m_bmDrawBitmap.SetMask(mask);

                m_bDrawToolDynamic = true;

                Repaint();
            }
        }
    }

    //left button UP
    if (bLeftUp)
    {
        if (tdata.iNbClicks == 0)
        {
            tdata.iNbClicks = 1;
            tdata.x1 = x;
            tdata.y1 = y;
            bUsingTool = true;
            m_bDrawToolDynamic = true;
        }
        else
        {
            if (!m_Bitmap) return;
            AddUndo();
            SetModified(true);
            wxMemoryDC mem_dc(*m_Bitmap);
            if (mem_dc.IsOk())
            {
                wxColour cLineColour, cFillColour;
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();

                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                wxBrush brush(cFillColour, wxSOLID);
                mem_dc.SetPen(pen);
                mem_dc.SetBrush(brush);
                tdata.x2 = x;
                tdata.y2 = y;
                int x1,y1,x2,y2;
                SnapRectToGrid(&x1,&y1,&x2,&y2);
                x1 = x1 / dScale;
                y1 = y1 / dScale;
                x2 = x2 / dScale;
                y2 = y2 / dScale;
                mem_dc.DrawEllipse(x1 + tdata.iSize - 1, y1 + tdata.iSize - 1 ,
                                   x2 - x1 - tdata.iSize + 1 , y2 - y1 - tdata.iSize + 1);

                mem_dc.SelectObject(wxNullBitmap);
            }
            UpdateImage();
            m_bDrawToolDynamic = false;

            Repaint();
            InitToolData();
        }
    }
}

/** init tool data for a first use
  */
void XPMEditorPanel::InitToolData(void)
{
    bUsingTool = false;
    if (DrawCanvas)
    {
        if (DrawCanvas->HasCapture()) DrawCanvas->ReleaseMouse();
    }
    if (ToolPanel) ToolPanel->InitToolData();
}

/** Get the tool data (size of the cursor, radius when applicable, ...)
  * \param t: pointer to the ToolData structure receiving the information
  */
void XPMEditorPanel::GetToolData(ToolData *t)
{
    //get the tool data
    if (t)
    {
        memcpy(t, &tdata, sizeof(ToolData));
    }
}

/** Set the tool data (size of the cursor, radius when applicable, ...)
  * \param t: pointer to the ToolData structure containing the information
  */
void XPMEditorPanel::SetToolData(ToolData *t)
{
    //set the tool data
    if (t)
    {
        memcpy(&tdata, t, sizeof(ToolData));
    }
}

/** Return the ID of the tool currently in use
  * \param the tool ID, -1 if no tool are currently selected
  */
int XPMEditorPanel::GetToolID(void)
{
    if (ToolPanel)
    {
        return(ToolPanel->GetToolID());
    }
    return(-1);
}

/** Set the ID of the tool currently in use
  * Do not modify the state of the buttons
  * \param the tool ID, -1 if no tool are currently selected
  */
void XPMEditorPanel::SetToolID(int iTool)
{
    if (ToolPanel)
    {
        ToolPanel->SetToolID(iTool);
    }
}

/** Set the correct cursor, according to the currently selected tool & option
  */
void XPMEditorPanel::SetToolCursor(void)
{
    //Set the correct cursor, according to the currently selected tool & option
    if (ToolPanel)
    {
        ToolPanel->SetToolCursor();
    }
}

/** Set the cursor for the DrawCanvas
  * \param cursor: the cursor to be set
  */
void XPMEditorPanel::SetToolCursor(const wxCursor &cursor)
{
    //Set the correct cursor, according to the currently selected tool & option
    if (DrawCanvas)
    {
        DrawCanvas->SetCursor(cursor);
    }
}



/** Return if the cursor is hovering over the selection
  * \param x : x position of the mouse cursor, relative to DrawCanvas. Unscrolled, scaled
  * \param y : y position of the mouse cursor, relative to DrawCanvas. Unscrolled, scaled
  * \return  1 if the cursor is hovering over the selection
  *          2 if the cursor is hovering over the EDGE of the selection - above
  *          3 if the cursor is hovering over the EDGE of the selection - right
  *          4 if the cursor is hovering over the EDGE of the selection - under
  *          5 if the cursor is hovering over the EDGE of the selection - left
  *          6 if the cursor is hovering over the EDGE of the selection - top left corner
  *          7 if the cursor is hovering over the EDGE of the selection - top right corner
  *          8 if the cursor is hovering over the EDGE of the selection - bottom left corner
  *          9 if the cursor is hovering over the EDGE of the selection - bottom right corner
  *          0 otherwise
  */
int XPMEditorPanel::IsPointInSelection(int x, int y)
{
    if (!HasSelection()) return(0);


    //get the region containing the selection
    int i;
    wxPoint *tmp;
    tmp = new wxPoint[NbPoints];
    if (!tmp) return(0);
    for(i=0;i<NbPoints;i++)
    {
        tmp[i].x = pSelection[i].x * dScale;
        tmp[i].y = pSelection[i].y * dScale;
    }
    wxRegion region(NbPoints, tmp);
    delete[] tmp;
    int xx, yy;

    DrawCanvas->CalcUnscrolledPosition(x,y,&xx, &yy);
    wxRegionContain rResult;
    //if (dScale > 0) {xx = xx / dScale; yy = yy / dScale;}
    //to check if the cursor is over the limits, we check if a 4 pts around the cursor position is partly in the region
    //if one of them is out of the region AND one of then is in the region, then the cursor is over the regions edges
    wxPoint pt1(xx - 5, yy);
    wxPoint pt2(xx + 5, yy);
    wxPoint pt3(xx, yy - 5);
    wxPoint pt4(xx, yy + 5);
    int i1, i2, i3, i4;
    i1 = 0; i2 = 0; i3 = 0; i4 = 0;
    rResult = region.Contains(pt1);
    if ((rResult == wxInRegion) || (rResult == wxPartRegion )) i1 = 1;
    rResult = region.Contains(pt2);
    if ((rResult == wxInRegion) || (rResult == wxPartRegion )) i2 = 1;
    rResult = region.Contains(pt3);
    if ((rResult == wxInRegion) || (rResult == wxPartRegion )) i3 = 1;
    rResult = region.Contains(pt4);
    if ((rResult == wxInRegion) || (rResult == wxPartRegion )) i4 = 1;
    if ((i1 + i2 + i3 + i4 > 0) && (i1 + i2 + i3 + i4 < 4))
    {
        int iCount;

        //for now the algorithm is pretty simple.
        iCount = i1 + i2 + i3 + i4;
        if ((i4 == 1) && (iCount == 1)) return(2); //above
        if ((i2 == 1) && (iCount == 1)) return(3); //left
        if ((i3 == 1) && (iCount == 1)) return(4); //under
        if ((i1 == 1) && (iCount == 1)) return(5); //right

        if ((i2 == 1) && (i4 == 1) && (iCount == 2)) return(6); //top left corner
        if ((i1 == 1) && (i4 == 1) && (iCount == 2)) return(7); //top right corner
        if ((i3 == 1) && (i2 == 1) && (iCount == 2)) return(8); //bottom left corner
        if ((i3 == 1) && (i1 == 1) && (iCount == 2)) return(9); //bottom right corner
    }

    rResult = region.Contains(wxPoint(xx,yy));
    if ((rResult == wxInRegion) || (rResult == wxPartRegion )) return(1);

    return(0);
}

/** Handler for mouse left button down
  */
void XPMEditorPanel::OnDrawCanvasLeftDown(wxMouseEvent& event)
{
    if (DrawCanvas) DrawCanvas->SetFocus();

    if ((DrawCanvas) && ((bCanResizeX) || (bCanResizeY)) && (m_Bitmap) && (!bUsingTool))
    {

        if ((!bSizingX) && (!bSizingY))
        {
            bSizingX = bCanResizeX;
            bSizingY = bCanResizeY;
            DrawCanvas->CaptureMouse();
            return;
        }


    }
    else
    {
        bSizingX = false;
        bSizingY = false;
        int x, y, xx, yy, iTool, iWidth, iHeight;
        x = event.m_x;
        y = event.m_y;
        DrawCanvas->CalcUnscrolledPosition(x, y, &xx, &yy);
        iTool = GetToolID();
        if (m_Bitmap)
        {
            iWidth = m_Bitmap->GetWidth() * dScale;
            iHeight = m_Bitmap->GetHeight() * dScale;
        }
        else
        {
            if (DrawCanvas) DrawCanvas->GetClientSize(&iWidth, &iHeight); else GetClientSize(&iWidth, &iHeight);
        }
        if (xx > iWidth) xx = iWidth ;
        if (yy  > iHeight) yy = iHeight;

        DrawCanvas->CaptureMouse();
        int iResult;
        iResult = IsPointInSelection(x, y);
        switch(iResult)
        {
            case 1 : if (    (!bUsingTool)
                          || ((GetToolID() != XPM_ID_SELECT_TOOL) && (GetToolID() != XPM_ID_LASSO_TOOL))
                         )
                     {
                         ProcessDragAction(xx, yy, true, false, true, false);
                     }
                     else
                     {
                         ProcessToolAction(iTool, xx, yy, true, false, true, false);
                     }
                     break;
            case 2 :
            case 3 :
            case 4 :
            case 5 :
            case 6 :
            case 7 :
            case 8 :
            case 9 : if (    (!bUsingTool)
                          || ((GetToolID() != XPM_ID_SELECT_TOOL) && (GetToolID() != XPM_ID_LASSO_TOOL))
                         )
                     {
                         ProcessSizeAction(xx, yy, true, false, true, false, iResult);
                     }
                     else
                     {
                         ProcessToolAction(iTool, xx, yy, true, false, true, false);
                     }
                     break;
            case 0 :
            default: ProcessToolAction(iTool, xx, yy, true, false, true, false); break;
        }


    }
    event.Skip();
}

/** Handler for mouse left button up
  */
void XPMEditorPanel::OnDrawCanvasLeftUp(wxMouseEvent& event)
{
    if (((bSizingX) || (bSizingY)) && (m_Bitmap) && (!bUsingTool))
    {
        //resize
        if (DrawCanvas) DrawCanvas->ReleaseMouse();
        int x, y, xx, yy;
        if (bSizingX) x = event.m_x; else x = m_Bitmap->GetWidth() * dScale;
        if (bSizingY) y = event.m_y; else y = m_Bitmap->GetHeight() * dScale;
        DrawCanvas->CalcUnscrolledPosition(x, y, &xx, &yy);
        if (xx < 1) xx = 1;
        if (yy < 1) yy = 1;
        bSizingX = false;
        bSizingY = false;
        OldX = 0; OldY = 0;
        x = xx / dScale;
        y = yy / dScale;
        wxSize sNewSize(x, y);
        if (m_undo_buffer) m_undo_buffer->AddUndo(xpm_size_undo, m_Image);
        SetDrawAreaSize(sNewSize);
        Refresh(false,NULL);
        Update();
        return;

    }
    else if (m_bDragging)
    {
        int x, y;

        x = event.m_x;
        y = event.m_y;
        if (DrawCanvas) DrawCanvas->ReleaseMouse();
        ProcessDragAction(x, y, false, true, false, false);
    }
    else if ((!bSizingX) && (!bSizingY))
    {
        //process a tool click
        int x, y, xx, yy, iTool, iWidth, iHeight;

        x = event.m_x;
        y = event.m_y;
        DrawCanvas->CalcUnscrolledPosition(x, y, &xx, &yy);
        iTool = GetToolID();
        if (m_Bitmap)
        {
            iWidth = m_Bitmap->GetWidth() * dScale;
            iHeight = m_Bitmap->GetHeight() * dScale;
        }
        else
        {
            if (DrawCanvas) DrawCanvas->GetClientSize(&iWidth, &iHeight); else GetClientSize(&iWidth, &iHeight);
        }
        if (xx > iWidth) xx = iWidth ;
        if (yy  > iHeight) yy = iHeight;

        if (DrawCanvas->HasCapture()) DrawCanvas->ReleaseMouse();
        ProcessToolAction(iTool, xx, yy, false, true, false, false);

    }
    event.Skip();
}

#if __WXMSW__
/** Handler for mouse event: set the cursor to Arrow when leaving the drawing canvas
  */
void XPMEditorPanel::OnDrawCanvasMouseLeave(wxMouseEvent& event)
{
    SetCursor(wxCURSOR_ARROW);
}
#endif


//------ SELECTION METHODS --------------------------------------

/** Get the smallest bounding rectangle encompassing the selection
  * \param r the rectangle (wxRect) containing the result
  */
void XPMEditorPanel::GetBoundingRect(wxRect *r)
{
    //Get the bounding rectangle of the selection
    if ((HasSelection()) && (r))
    {
        int i;
        int iMinX, iMaxX, iMinY, iMaxY;

        iMinX = pSelection[0].x;
        iMaxX = pSelection[0].x;
        iMinY = pSelection[0].y;
        iMaxY = pSelection[0].y;

        for(i=0;i<NbPoints;i++)
        {
            if (pSelection[i].x < iMinX) iMinX = pSelection[i].x;
            if (pSelection[i].y < iMinY) iMinY = pSelection[i].y;
            if (pSelection[i].x > iMaxX) iMaxX = pSelection[i].x;
            if (pSelection[i].y > iMaxY) iMaxY = pSelection[i].y;
        }
        r->SetLeft(iMinX);
        r->SetTop(iMinY);
        r->SetRight(iMaxX);
        r->SetBottom(iMaxY);
    }
}

/** convert the selection to a rectangular selection, for simpler stretching
  */
void XPMEditorPanel::ConvertSelectionToRect(void)
{
    if (HasSelection())
    {
        wxRect rSelectionRect;
        int x1, y1, x2, y2;

        GetBoundingRect(&rSelectionRect);
        x1 = rSelectionRect.GetLeft();
        y1 = rSelectionRect.GetTop();
        x2 = rSelectionRect.GetRight();
        y2 = rSelectionRect.GetBottom();

        pSelection = CheckMemorySelection(4);
        if (!pSelection) return;
        NbPoints = 4;

        pSelection[0].x = x1;
        pSelection[0].y = y1;

        pSelection[1].x = x2;
        pSelection[1].y = y1;

        pSelection[2].x = x2;
        pSelection[2].y = y2;

        pSelection[3].x = x1;
        pSelection[3].y = y2;

    }
}

/** Replace the rectangle by a new image
  * if the new image is smaller than the rectangle, then the new image is pasted as (0,0) on the selection,
  * and the remaining space is filled with the mask (transparent) colour
  * if the new image is bigger than the rectangle, then it is resized first to fit to the rectangle
  * \param newImage: the new image which will replace the rectangle
  * \param rRect: a wxRect representing the rectangle to be replaced
  *           the rectangle will be modified to fit in the image, if necessary
  */
void XPMEditorPanel::ReplaceRect(const wxImage &newImage, wxRect rRect)
{
    if (!m_Image) return;

    wxRect r(rRect);

    if (r.GetLeft() < 0) r.SetLeft(0);
    if (r.GetTop() < 0) r.SetTop(0);
    if (r.GetWidth() > m_Image->GetWidth()) r.SetWidth(m_Image->GetWidth());
    if (r.GetHeight() > m_Image->GetHeight()) r.SetHeight(m_Image->GetHeight());


    wxImage img(r.GetWidth(), r.GetHeight(), true);
    img.SetMask(false);
    img.Replace(0,0,0, cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

    wxImage newImage2(newImage);

    if ((r.GetWidth() < newImage.GetWidth()) && (r.GetHeight() < newImage.GetHeight()))
    {
        //new image is bigger than the selection
        //resize it first
        newImage2.Resize(wxSize(r.GetWidth(), r.GetHeight()), wxPoint(0,0), 0,0,0);
    }

    //PasteImage(img, r.GetLeft(), r.GetTop());
    PasteImage(newImage2, r.GetLeft(), r.GetTop());
}

/** Paste a new image in the image, at the specified position
  * if the new image is bigger than the buffer image, then the new image is cropped.
  * if the paste position is outside the buffer image, nothing is done
  * \param newImage: the new image which will replace the rectangle
  * \param x : the X coordinate where the paste will occur
  * \param y : the Y coordinate where the paste will occur
  */
void XPMEditorPanel::PasteImage(const wxImage &newImage, int x, int y)
{
    if (!m_Bitmap) return;

    //test is the position is valid
    if ((x < 0) || (x > m_Bitmap->GetWidth())) return;
    if ((y < 0) || (y > m_Bitmap->GetHeight())) return;

    //create and resize the image to paste
    wxImage img(newImage);
    if (img.GetWidth() + x > m_Bitmap->GetWidth()) img.Resize(wxSize(m_Bitmap->GetWidth() - x, img.GetHeight()), wxPoint(0,0));
    if (img.GetHeight() + y > m_Bitmap->GetHeight()) img.Resize(wxSize( img.GetWidth(), m_Bitmap->GetHeight() - y), wxPoint(0,0));

    //draw the image on the buffer bitmap
    wxMemoryDC mem_dc;
    if (!mem_dc.IsOk()) return;
    mem_dc.SelectObject(*m_Bitmap);
    wxBitmap bmp(img);
    mem_dc.DrawBitmap(bmp, x, y, true);

    mem_dc.SelectObject(wxNullBitmap);
    UpdateImage();

    Repaint();
}

/** clear the current selection
  * \return no return value
  */
void XPMEditorPanel::ClearSelection(void)
{
    //clear the selection
    if (HasSelection())
    {
        //paste the selection at the current coordinates
        PasteSelection();

        //remove the selection
        NbPoints = 0;

        //for text tool, which use the selection
        InitToolData();
    }
}

/** Paste the content of the selection to the current selection coordinates
  * \return no return value
  */
void XPMEditorPanel::PasteSelection(void)
{
    wxRect rSelection;
    GetBoundingRect(&rSelection);

    PasteImage(m_SelectionImage, rSelection.GetLeft(), rSelection.GetTop());
}


/** Return true if there is some Selection, false otherwise
  * \return true if something is selected, false otherwise
  */
bool XPMEditorPanel::HasSelection(void) const
{
    //check if an area is selected
    if ((NbPoints > 0) && (pSelection)) return(true);
    return(false);
}

//---------- CLIPBOARD OPERATIONS -----------------
/** perform a "cut" clipbopard operation
  * \return no return value
  */
void XPMEditorPanel::Cut(void)
{
    //CUT
    if (!HasSelection()) return;

    if (!HasSelection()) return;

    if ((wxTheClipboard->Open()) && (m_SelectionBitmap.IsOk()))
    {
        //copy to the clipboard
        wxTheClipboard->SetData(new wxBitmapDataObject(m_SelectionBitmap));
        wxTheClipboard->Close();
        AddUndo();

        //cute a transparent image
        if(m_bEraseSelection) CutSelection();
        NbPoints = 0;
        SetModified(true);
        Repaint();
    }
}

/** perform a "copy" clipbopard operation
  * \return no return value
  */
void XPMEditorPanel::Copy(void)
{
    //COPY
    if (!HasSelection()) return;

    if ((wxTheClipboard->Open()) && (m_SelectionBitmap.IsOk()))
    {
        //copy to the clipboard
        wxTheClipboard->SetData(new wxBitmapDataObject(m_SelectionBitmap));
        wxTheClipboard->Close();
    }
}

/** perform a "paste" clipbopard operation
  * \return no return value
  */
void XPMEditorPanel::Paste(void)
{
    //PASTE
    if (!DrawCanvas) return;
    ClearSelection();
    bool bResult;

    //get the bitmap from the clipboard
    if (wxTheClipboard->Open())
    {
        wxBitmapDataObject bmdo;
        bResult = wxTheClipboard->IsSupported(wxDF_BITMAP);
        if (bResult)
        {
            wxTheClipboard->GetData(bmdo);
        }
        wxTheClipboard->Close();
        if (bResult)
        {
            AddUndo();
            wxBitmap bm;
            bm = bmdo.GetBitmap();
            wxBitmap *original_bitmap;
            original_bitmap = GetBitmap();

            //increase the size of the current bitmap if needed
            int iWidth, iHeight, iWidth2, iHeight2;
            iWidth = bm.GetWidth();
            iHeight = bm.GetHeight();
            if (original_bitmap)
            {
                iWidth2 = original_bitmap->GetWidth();
                iHeight2 = original_bitmap->GetHeight();
            }
            else
            {
                iWidth2 = XPM_DEFAULT_WIDTH;
                iHeight2 = XPM_DEFAULT_HEIGHT;
            }
            if ((iHeight > iHeight2) || (iWidth > iWidth2))
            {
                int iResult;
                iResult = wxMessageBox(_("Do you want to resize the Bitmap ?"), _("Confirmation required."), wxYES_NO);
                if (iResult == wxYES)
                {
                    //resize the bitmap
                    if (iHeight > iHeight2) iHeight2 = iHeight;
                    if (iWidth > iWidth2) iWidth2 = iWidth;;
                    SetDrawAreaSize(wxSize(iWidth2, iHeight2));
                }
            }

            //Create the selection
            NbPoints = 4;
            if (NbPoints > NbPointsMax)
            {
                pSelection = (wxPoint*) realloc(pSelection, (NbPointsMax + 10) * sizeof(wxPoint));
                if (pSelection) NbPointsMax = NbPointsMax + 10;
            }
            if (pSelection)
            {
                pSelection[0] = wxPoint(0,0);
                pSelection[1] = wxPoint(iWidth,0);
                pSelection[2] = wxPoint(iWidth,iHeight);
                pSelection[3] = wxPoint(0,iHeight);
            }

            //paste the Bitmap at 0,0
            m_SelectionBitmap = bm;
            m_SelectionImage = bm.ConvertToImage();
            //if (bm.GetMask()) wxMessageBox(_("mask")); else wxMessageBox(_("no mask"));
            SetModified(true);
            Repaint();
            m_bEraseSelection = false;
        }
    }
}


//---------------- UNDO & REDO ---------------------------

/** Return true if UNDO can be done, false otherwise
  * \return true if UNDO can be done, false otherwise
  */
bool XPMEditorPanel::CanUndo(void) const
{
    //check if UNDO is possible
    if (m_undo_buffer) return(m_undo_buffer->CanUndo());
    return(false);
}

/** Return true if UNDO can be done, false otherwise
  * \return true if UNDO can be done, false otherwise
  */
bool XPMEditorPanel::CanRedo(void) const
{
    //check if REDO is possible
    if (m_undo_buffer) return(m_undo_buffer->CanRedo());
    return(false);
}

/** Clear the Undo Buffer
  * \return no return value
  */
void XPMEditorPanel::ClearUndoBuffer(void)
{
    //Clear the Undo Buffer
    if (m_undo_buffer) m_undo_buffer->ClearUndoBuffer();
}

/** Clear the Redo Buffer
  * \return no return value
  */
void XPMEditorPanel::ClearRedoBuffer(void)
{
    //Clear the Redo Buffer
    if (m_undo_buffer) m_undo_buffer->ClearRedoBuffer();
}

/** Add 1 Undo operation to the undo buffer
  * \param bm a pointer to the bitmap to add to the Undo Buffer. A copy of this bitmap is done
  * \return true on success, false otherwise
  */
bool XPMEditorPanel::AddUndo(wxBitmap *bm)
{
    //add 1 UNDO operation to the buffer
    if (bm)
    {
        wxImage img = bm->ConvertToImage();
        img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

        if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_image_undo, &img));
    }
    return(false);
}

/** Add 1 Undo operation to the undo buffer
  * \param img: a pointer to the image to add to the Undo Buffer. A copy of this image is done
  * \return true on success, false otherwise
  */
bool XPMEditorPanel::AddUndo(wxImage *img)
{
    //add 1 UNDO operation to the buffer
    if (img)
    {
        wxImage img2(*img);
        img2.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

        if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_image_undo, &img2));
    }
    return(false);
}

/** Add 1 Undo operation to the undo buffer
  * \return true on success, false otherwise
  */
bool XPMEditorPanel::AddUndo(void)
{
    //add 1 Undo operation to the Buffer
    if (m_Image)
    {
        wxImage img(*m_Image);
        img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

        if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_image_undo, &img));
    }

    return(false);
}

/** Add 1 Undo operation to the redo buffer
  * \return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(void)
{
    //add 1 Redo operation to the Buffer
    if (m_Image)
    {
        wxImage img(*m_Image);
        img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

        if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_image_undo, &img));
    }

    return(false);
}

/** Add 1 Redo operation to the redo buffer
  * \param bm a pointer to the bitmap to add to the Redo Buffer. A copy of this bitmap is done
  * \return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(wxBitmap *bm)
{
    //add 1 REDO operation to the buffer
    if (bm)
    {
        wxImage img = bm->ConvertToImage();
        img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

        if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_image_undo, &img));
    }
    return(false);

}

/** Add 1 Redo operation to the redo buffer
  * \param bm a pointer to the image to add to the Redo Buffer. A copy of this image is done
  * \return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(wxImage *img)
{
    //add 1 REDO operation to the buffer
    if (img)
    {
        wxImage img2(*img);
        img2.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

        if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_image_undo, &img2));
    }
    return(false);
}

/** Performs an Undo operation
  * \return no return value
  */
void XPMEditorPanel::Undo(void)
{
    //UNDO
    if (m_undo_buffer) m_undo_buffer->Undo();
}

/** Performs a Redo operation
  * \return no return value
  */
void XPMEditorPanel::Redo(void)
{
    //REDO
    if (m_undo_buffer) m_undo_buffer->Redo();
}

//------- TRANSPARENT COLOR CHANGED --------
void XPMEditorPanel::OnTransparentColorChanged(wxCommandEvent& event)
{
    unsigned char iGreen, iRed, iBlue;
    wxColour cColour;

    UpdateImage();

    //get the new transparent colour
    if (!ColourPicker) return;
    cColour = ColourPicker->GetTransparentColour();
    iRed = cColour.Red();
    iGreen = cColour.Green();
    iBlue = cColour.Blue();

    //Replace the old mask colour by the new mask colour
    if (m_Image)
    {
        AddUndo();
        SetModified(true);
        unsigned char iGreen2, iRed2, iBlue2;
        iRed2 = cMaskColour.Red();
        iGreen2 = cMaskColour.Green();
        iBlue2 = cMaskColour.Blue();

        m_Image->Replace(iRed2, iGreen2, iBlue2, iRed, iGreen, iBlue);
        cMaskColour = cColour;

        UpdateBitmap();
    }
}

//---- HANDLERS FOR ALL THE DRAWING TOOLS  ----
/** toggle all Tools Buttons OFF, except the iIndex one.
  * \param iIndex: the button to keep ON.
  *                if the button is OFF, it will stay OFF.
  *                if iIndex < 0, then ALL buttons are turned OFF
  * \param bClearSelection: if true, the current selection will be cleared (default).
  */
void XPMEditorPanel::ToggleButtons(int iIndex, bool bClearSelection)
{
    //toggle all Tools Buttons OFF, except the iIndex one.
    if (ToolPanel)
    {
        ToolPanel->ToggleButtons(iIndex, bClearSelection);
    }
}

/** hide or show some widgets
  * \param bShow : if true, show the controls. If false, hide them
  */
void XPMEditorPanel::ShowCanvasWidgets(bool bShow)
{
    if (TextEdit) TextEdit->Show(bShow);
    if (ResizeCtrl1) ResizeCtrl1->Show(bShow);
}

//------ CONFIGURATION ---------------
/** Apply the current configuration to the editor
  */
void XPMEditorPanel::UpdateConfiguration(void)
{
    int iMaxUndoRedo;

    if (XPM_Plugin())
    {
        XPM_Plugin()->GetDefaultImageSize(&iXPMDefaultWidth, &iXPMDefaultHeight);
        iMaxUndoRedo = XPM_Plugin()->GetMaxUndoRedo();

        wxColour *array;
        wxColour cTransparent;
        int i, iNbColours;
        bool bResult;

        XPM_Plugin()->GetColourArray(NULL, &iNbColours, &cTransparent);
        array = new wxColour[iNbColours];


        if (ColourPicker)
        {
            if (array)
            {
                ColourPicker->SetPaletteSize(iNbColours);
                XPM_Plugin()->GetColourArray(array, &iNbColours, &cTransparent);
                for(i=0;i<iNbColours;i++) ColourPicker->SetPaletteColour(i, array[i]);
                delete[] array;
            }
            else
            {
                ColourPicker->init_colours();
            }
            ColourPicker->SetTransparentColour(cTransparent, false);
        }

        cBackgroundColour = XPM_Plugin()->GetBackGroundColour();

    }
    else
    {
        //the plugin could not be found. Apply default values.
        iXPMDefaultWidth = XPM_DEFAULT_WIDTH;
        iXPMDefaultHeight = XPM_DEFAULT_HEIGHT;
        iMaxUndoRedo = XPM_MAX_UNDO;

        cBackgroundColour = *wxLIGHT_GREY;
        if (ColourPicker) ColourPicker->init_colours();
    }

    if (m_undo_buffer) m_undo_buffer->SetMaxUndoSize(iMaxUndoRedo);
    Refresh(true,NULL);
    Update();
}

wxPoint* XPMEditorPanel::CheckMemorySelection(int iNeeded)
{
    //increase the selection memory if needed
    if ((!pSelection) || (iNeeded > NbPointsMax))
    {
        if (NbPointsMax < 1) NbPointsMax = 100;
        if (iNeeded > NbPointsMax) NbPointsMax = iNeeded + 10;
        pSelection = (wxPoint*) realloc(pSelection, NbPointsMax * sizeof(wxPoint));
    }
    return(pSelection);
}

/** Draw the text bitmap on the selection image
  */
void XPMEditorPanel::DrawTextBitmap(void)
{
    if (m_SelectionBitmap.IsOk())
    {
        wxColour cLineColour;
        wxColour cFillColour;
        wxColour cMaskColour2;

        wxMemoryDC memdc;
        if (memdc.IsOk())
        {
            if (ColourPicker)
            {
                cLineColour = ColourPicker->GetLineColour();
                cFillColour = ColourPicker->GetFillColour();
            }
            else
            {
                cLineColour = *wxWHITE;
                cFillColour = *wxBLACK;
            }
            cMaskColour2 = cMaskColour;
            if ((cLineColour == cMaskColour2) || (cFillColour == cMaskColour2))
            {
                cMaskColour2 = *wxRED;
            }
            //if red is already used, use white
            if ((cLineColour == cMaskColour2) || (cFillColour == cMaskColour2))
            {
                cMaskColour2 = *wxWHITE;
            }
            //if red is already used, use black
            if ((cLineColour == cMaskColour2) || (cFillColour == cMaskColour2))
            {
                cMaskColour2 = *wxBLACK;
            }

            wxBrush brush(cMaskColour2, wxSOLID);
            memdc.SelectObject(m_SelectionBitmap);
            memdc.SetBackground(brush);
            memdc.Clear();
            //memdc.SetBackgroundMode(tdata.iStyle);
            memdc.SetFont(tdata.font);

            memdc.SetTextForeground(cLineColour);
            memdc.SetBackgroundMode(wxSOLID);
            if (tdata.iStyle == wxSOLID)
            {
                memdc.SetTextBackground(cFillColour);
            }
            else
            {
                memdc.SetTextBackground(cMaskColour2);
            }

            DrawTextRectangle(memdc, tdata.sText,
                              wxRect(0,0, m_SelectionBitmap.GetWidth(), m_SelectionBitmap.GetHeight()),
                              tdata.iHorizAlign, tdata.iVertAlign, tdata.angle
                             );

            memdc.SelectObject(wxNullBitmap);

            //create the mask for the bitmap
            m_SelectionBitmap.SetMask(new wxMask(m_SelectionBitmap, cMaskColour2));
        }
        m_SelectionImage = m_SelectionBitmap.ConvertToImage();
        //m_SelectionImage.SetMaskColour(cMaskColour2.Red(),cMaskColour2.Green(),cMaskColour2.Blue());
    }
}

/** Draw a text rectangle
  * code copied from wxGrid
  * \param dc: the dc to draw on
  * \param value: the string to draw
  * \param rect: the bounding rectangle
  * \param horizAlign: the horizontal alignment wxALIGN_RIGHT, wxALIGN_LEFT, wxALIGN_CENTER
  * \param vertAlign : the vertical alignment wxALIGN_BOTTOM, wxALIGN_LEFT, wxALIGN_TOP
  * \param double: the angle / orientation of the text (in degrees, not in radians)
  */
void XPMEditorPanel::DrawTextRectangle( wxDC& dc,
                                const wxString& value,
                                const wxRect& rect,
                                int horizAlign,
                                int vertAlign,
                                double textOrientation )
{
    wxArrayString lines;

    StringToLines( value, lines );

    // Forward to new API.
    DrawTextRectangle( dc, lines, rect, horizAlign, vertAlign, textOrientation );
}

/** Draw a text rectangle
  * code copied from wxGrid
  * \param dc: the dc to draw on
  * \param value: the string to draw
  * \param rect: the bounding rectangle
  * \param horizAlign: the horizontal alignment wxALIGN_RIGHT, wxALIGN_LEFT, wxALIGN_CENTER
  * \param vertAlign : the vertical alignment wxALIGN_BOTTOM, wxALIGN_LEFT, wxALIGN_TOP
  * \param double: the angle / orientation of the text (in degrees, not in radians)
  */
void XPMEditorPanel::DrawTextRectangle(wxDC& dc,
                               const wxArrayString& lines,
                               const wxRect& rect,
                               int horizAlign,
                               int vertAlign,
                               double textOrientation)
{
    if ( lines.empty() ) return;

    wxDCClipper clip(dc, rect);

    long textWidth,
         textHeight;

    if ( textOrientation == 0 )
        GetTextBoxSize( dc, lines, &textWidth, &textHeight );
    else
        GetTextBoxSize( dc, lines, &textHeight, &textWidth );

    int x = 0,
        y = 0;
    switch ( vertAlign )
    {
        case wxALIGN_BOTTOM:
            if ( textOrientation == 0 )
                y = rect.y + (rect.height - textHeight - 1);
            else
                x = rect.x + rect.width - textWidth;
            break;

        case wxALIGN_CENTRE:
            if ( textOrientation == 0 )
                y = rect.y + ((rect.height - textHeight) / 2);
            else
                x = rect.x + ((rect.width - textWidth) / 2);
            break;

        case wxALIGN_TOP:
        default:
            if ( textOrientation == 0 )
                y = rect.y + 1;
            else
                x = rect.x + 1;
            break;
    }

    // Align each line of a multi-line label
    size_t nLines = lines.GetCount();
    for ( size_t l = 0; l < nLines; l++ )
    {
        const wxString& line = lines[l];

        if ( line.empty() )
        {
            *(textOrientation == 0 ? &y : &x) += dc.GetCharHeight();
            continue;
        }

        long lineWidth = 0,
             lineHeight = 0;
        dc.GetTextExtent(line, &lineWidth, &lineHeight);

        switch ( horizAlign )
        {
            case wxALIGN_RIGHT:
                if ( textOrientation == 0 )
                    x = rect.x + (rect.width - lineWidth - 1);
                else
                    y = rect.y + lineWidth + 1;
                break;

            case wxALIGN_CENTRE:
                if ( textOrientation == 0 )
                    x = rect.x + ((rect.width - lineWidth) / 2);
                else
                    y = rect.y + rect.height - ((rect.height - lineWidth) / 2);
                break;

            case wxALIGN_LEFT:
            default:
                if ( textOrientation == 0 )
                    x = rect.x + 1;
                else
                    y = rect.y + rect.height - 1;
                break;
        }

        if ( textOrientation == 0 )
        {
            dc.DrawText( line, x, y );
            y += lineHeight;
        }
        else
        {
            dc.DrawRotatedText( line, x, y, textOrientation );
            x += lineHeight;
        }
    }
}

/** Split multi-line text up into an array of strings.
  * Any existing contents of the string array are preserved.
  * code copied from wxGrid
  * \param value: the string to split
  * \param lines: the wxArrayString which will receive the strings - 1 per line
  */
void XPMEditorPanel::StringToLines(const wxString& value, wxArrayString& lines)
{
    int startPos = 0;
    int pos;
    wxString eol = wxTextFile::GetEOL( wxTextFileType_Unix );
    wxString tVal = wxTextFile::Translate( value, wxTextFileType_Unix );

    while ( startPos < (int)tVal.length() )
    {
        pos = tVal.Mid(startPos).Find( eol );
        if ( pos < 0 )
        {
            break;
        }
        else if ( pos == 0 )
        {
            lines.Add( wxEmptyString );
        }
        else
        {
            lines.Add( tVal.Mid(startPos, pos) );
        }

        startPos += pos + 1;
    }

    if ( startPos < (int)tVal.length() )
    {
        lines.Add( tVal.Mid( startPos ) );
    }
}

/** Compute the ideal TextBox size for the string to draw
  */
void XPMEditorPanel::GetTextBoxSize(const wxDC& dc,
                                    const wxArrayString& lines,
                                    long *width, long *height)
{
    long w = 0;
    long h = 0;
    long lineW = 0, lineH = 0;

    size_t i;
    for ( i = 0; i < lines.GetCount(); i++ )
    {
        dc.GetTextExtent( lines[i], &lineW, &lineH );
        w = wxMax( w, lineW );
        h += lineH;
    }

    *width = w;
    *height = h;
}


/** The font for the text edition has been changed
  * \param event: the wxCommandEvent associated with the event
  */
void XPMEditorPanel::OnFontButtonClick(wxCommandEvent& event)
{
    wxFontData fdata;

    fdata.SetInitialFont(tdata.font);
    if (ColourPicker) fdata.SetColour(ColourPicker->GetLineColour());
    wxFontDialog d(this, fdata);
    if (d.ShowModal() == wxID_OK)
    {
        wxFontData retData = d.GetFontData();
        tdata.font = retData.GetChosenFont();
        if (ColourPicker)
        {
            int iColour;
            iColour = ColourPicker->GetLineColourIndex();
            ColourPicker->SetPaletteColour(iColour, retData.GetColour());
            ColourPicker->Refresh(false,NULL);
            ColourPicker->Update();
        }
        DrawTextBitmap();
        Repaint();
    }
}

/** Change the background mode used by the text tool (opaque or transparent)
  * \param event: the wxCommandEvent associated with the event
  */
void XPMEditorPanel::OnBackgroundButtonToggle(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        if (ToolPanel) ToolPanel->BackgroundButton->SetLabel(_("TRANSPARENT"));
        tdata.iStyle = wxTRANSPARENT;
    }
    else
    {
        if (ToolPanel) ToolPanel->BackgroundButton->SetLabel(_("OPAQUE"));
        tdata.iStyle = wxSOLID;
    }
    DrawTextBitmap();
    Repaint();
}

void XPMEditorPanel::OnLineColorChanged(wxCommandEvent& event)
{
    wxColour cColour;

    //get the new line colour
    if (!ColourPicker) return;
    cColour = ColourPicker->GetLineColour();

    if (bUsingTool)
    {
        switch(GetToolID())
        {
            case XPM_ID_TEXT_TOOL: DrawTextBitmap();
                                   Repaint();
                                   //wxMessageBox(_("line colour changed"));
                                   break;
            default: break;
        }
    }
}

void XPMEditorPanel::OnFillColorChanged(wxCommandEvent& event)
{
    wxColour cColour;

    //get the new line colour
    if (!ColourPicker) return;
    cColour = ColourPicker->GetFillColour();

    if (bUsingTool)
    {
        switch(GetToolID())
        {
            case XPM_ID_TEXT_TOOL: DrawTextBitmap();
                                   Repaint();
                                   break;
            default: break;
        }
    }
}

/** Process char events in the text editor for the Text Tool
  * \param event: the wxCommandEvent class instance associated with the event.
  */
void XPMEditorPanel::OnTextEditText(wxCommandEvent& event)
{
    if (bUsingTool)
    {
        switch(GetToolID())
        {
            case XPM_ID_TEXT_TOOL: tdata.sText = wxString(TextEdit->GetValue());
                                   DrawTextBitmap();
                                   Repaint();
                                   break;
            default: break;
        }
    }
}

/** Handler for DEL key: erase selection
  * Handler for ARROW key: move selection
  */
void XPMEditorPanel::OnDrawCanvasKeyDown(wxKeyEvent& event)
{
    bool bUpdate, bSkip;
    int iModifiers;
    int dx;

    bUpdate = false;
    bSkip = true;
    iModifiers = event.GetModifiers();
    dx = 5;
    if (iModifiers & wxMOD_CONTROL) dx = 1;
    if (iModifiers & wxMOD_SHIFT) dx = 10;

    switch(event.GetKeyCode())
    {
        case WXK_NUMPAD_DELETE :
        case WXK_BACK :
        case WXK_DELETE :   if (HasSelection())
                            {
                                //remove the selection
                                NbPoints = 0;

                                //for text tool, which use the selection
                                InitToolData();
                                bUpdate = true;
                            }
                            break;
        case WXK_LEFT : if (HasSelection())
                        {
                            if (m_bEraseSelection) {AddUndo(); CutSelection(); m_bEraseSelection = false;};
                            MoveSelection(-dx,0);
                            SetModified(true);
                            bUpdate = true;
                            if (iModifiers & wxMOD_CONTROL) bSkip = false; //to avoid scrollbar event
                        }
                        break;
        case WXK_UP   : if (HasSelection())
                        {
                            if (m_bEraseSelection) {AddUndo(); CutSelection(); m_bEraseSelection = false;};
                            MoveSelection(0,-dx);
                            SetModified(true);
                            bUpdate = true;
                            if (iModifiers & wxMOD_CONTROL) bSkip = false; //to avoid scrollbar event
                        }
                        break;
        case WXK_RIGHT: if (HasSelection())
                        {
                            if (m_bEraseSelection) {AddUndo(); CutSelection(); m_bEraseSelection = false;};
                            MoveSelection(dx,0);
                            SetModified(true);
                            bUpdate = true;
                            if (iModifiers & wxMOD_CONTROL) bSkip = false; //to avoid scrollbar event
                        }
                        break;
        case WXK_DOWN : if (HasSelection())
                        {
                            if (m_bEraseSelection) {AddUndo(); CutSelection(); m_bEraseSelection = false;};
                            MoveSelection(0,dx);
                            SetModified(true);
                            bUpdate = true;
                            if (iModifiers & wxMOD_CONTROL) bSkip = false; //to avoid scrollbar event
                        }
                        break;

        default: break;
    }

    if (bUpdate)
    {
        Repaint();
    }

    if ((bSkip) && (!HasSelection())) event.Skip();
}

/** Handler for the "STRETCH" BUTTON
  * It will resize the image while stretching it
  */
void XPMEditorPanel::OnStretchImage(wxCommandEvent& event)
{
    wxStretchImage sd(this);

    //init the dialog box
    wxRect rSelection;
    wxSize sSelection;

    GetBoundingRect(&rSelection);
    sd.sSelection = wxSize(rSelection.GetWidth(), rSelection.GetHeight());
    sd.sImage = sDrawAreaSize;

    if (HasSelection())
    {
        //selection
        sd.RadioButton2->SetValue(true);
        sd.RadioButton2->Enable(true);
        sd.TextCtrl1->SetValue(wxString::Format(_("%d"), rSelection.GetWidth()));
        sd.TextCtrl2->SetValue(wxString::Format(_("%d"), rSelection.GetHeight()));
        sd.SpinCtrl1->SetValue(rSelection.GetWidth());
        sd.SpinCtrl2->SetValue(rSelection.GetHeight());
    }
    else
    {
        //image
        sd.RadioButton1->SetValue(true);
        sd.RadioButton2->Enable(false);
        sd.TextCtrl1->SetValue(wxString::Format(_("%d"), sDrawAreaSize.GetWidth()));
        sd.TextCtrl2->SetValue(wxString::Format(_("%d"), sDrawAreaSize.GetHeight()));
        sd.SpinCtrl1->SetValue(sDrawAreaSize.GetWidth());
        sd.SpinCtrl2->SetValue(sDrawAreaSize.GetHeight());
    }
    sd.RadioButton3->SetValue(true);

    //display the dialog box
    if (sd.ShowModal() == 0)
    {
        int iQuality, iHeight, iWidth;
        if (sd.RadioButton3->GetValue()) iQuality = wxIMAGE_QUALITY_NORMAL; else iQuality = wxIMAGE_QUALITY_HIGH;
        iWidth  = sd.SpinCtrl1->GetValue();
        iHeight = sd.SpinCtrl2->GetValue();

        if (sd.RadioButton1->GetValue())
        {
            //rescale the image
            AddUndo();
            sDrawAreaSize = wxSize(iWidth, iHeight);

            //indicate the size of the bitmap in the spinboxes
            if (PropertiesPanel)
            {
                if (PropertiesPanel->BMPWidth) PropertiesPanel->BMPWidth->SetValue(sDrawAreaSize.GetWidth());
                if (PropertiesPanel->BMPHeight) PropertiesPanel->BMPHeight->SetValue(sDrawAreaSize.GetHeight());
            }

            //resize the image
            if (m_Image)
            {
                m_Image->Rescale(iWidth, iHeight, iQuality);
            }
            UpdateBitmap();
            SetModified(true);
            Repaint();
        }
        else if (HasSelection())
        {
            //rescale the selection
            ConvertSelectionToRect();
            pSelection[1].x = pSelection[0].x + iWidth;
            pSelection[2].x = pSelection[1].x;
            pSelection[2].y = pSelection[0].y + iHeight;
            pSelection[3].y = pSelection[2].y;
            m_SelectionImage.Rescale(iWidth, iHeight, iQuality);
            m_SelectionBitmap = wxBitmap(m_SelectionImage);
            m_bEraseSelection = false;
            Repaint();
        }
    }
}

/** Mirroring of the image or the selection
  */
void XPMEditorPanel::OnMirror(wxCommandEvent& event)
{
    wxMirror md(this);

    //init the dialog box
    if (HasSelection())
    {
        md.RadioButton4->Enable(true);
        md.RadioButton4->SetValue(true);
    }
    else
    {
        md.RadioButton4->Enable(false);
        md.RadioButton3->SetValue(true);
    }
    md.RadioButton1->SetValue(true);

    //show the dialog box
    if (md.ShowModal() == 0)
    {
        bool bHorizontal;
        if (md.RadioButton1->GetValue()) bHorizontal = true; else bHorizontal = false;

        if ((md.RadioButton3->GetValue()))
        {
           //mirror image
           if (m_Image)
           {
               AddUndo();
               wxImage img;
               img = m_Image->Mirror(bHorizontal);
               SetImage(&img);
               SetModified(true);
           }
        }
        else
        {
            //mirror selection
            if (HasSelection())
            {
                wxImage img;
                img = m_SelectionImage.Mirror(bHorizontal);
                m_SelectionImage = img;
                m_SelectionBitmap = wxBitmap(m_SelectionImage);
                m_bEraseSelection = false;
                Repaint();
            }
        }
    }
}

/** Blur handler **/
void XPMEditorPanel::OnBlur(wxCommandEvent& event)
{
    wxBlur bd(this);

    //init the dialog box
    if (HasSelection())
    {
        bd.RadioButton4->Enable(true);
        bd.RadioButton4->SetValue(true);
    }
    else
    {
        bd.RadioButton4->Enable(false);
        bd.RadioButton3->SetValue(true);
    }
    bd.RadioButton1->SetValue(true);

    //show the dialog box
    if (bd.ShowModal() == 0)
    {
        int iRadius;
        bool bBoth, bHorizontal;
        if (bd.RadioButton1->GetValue()) bHorizontal = true; else bHorizontal = false;
        if (bd.RadioButton5->GetValue()) bBoth = true; else bBoth = false;
        iRadius = bd.SpinCtrl1->GetValue();

        if ((bd.RadioButton3->GetValue()) && (m_Image))
        {
           //blur image
           if (m_Image)
           {
               AddUndo();
               wxImage img;
               if (bHorizontal)
               {
                   img = m_Image->BlurHorizontal(iRadius);
               }
               else if (bBoth)
               {
                   img = m_Image->Blur(iRadius);
               }
               else
               {
                   img = m_Image->BlurVertical(iRadius);
               }
               SetImage(&img);
               SetModified(true);
           }
        }
        else
        {
            //blur selection
            if (HasSelection())
            {
                wxImage img;
                if (bHorizontal)
                {
                    img = m_SelectionImage.BlurHorizontal(iRadius);
                }
                else if (bBoth)
                {
                    img = m_SelectionImage.Blur(iRadius);
                }
                else
                {
                    img = m_SelectionImage.BlurVertical(iRadius);
                }
                m_SelectionImage = img;
                m_SelectionBitmap = wxBitmap(m_SelectionImage);
                m_bEraseSelection = false;
                Repaint();
            }
        }
    }
}

/** Rotate the image or the selection **/
void XPMEditorPanel::OnRotate(wxCommandEvent& event)
{
    wxRotate rd(this);

    //init the dialog box
    if (HasSelection())
    {
        rd.RadioButton4->Enable(true);
        rd.RadioButton4->SetValue(true);
    }
    else
    {
        rd.RadioButton4->Enable(false);
        rd.RadioButton3->SetValue(true);
    }
    rd.RadioButton1->SetValue(true);
    rd.StaticText2->Enable(false);
    rd.SpinCtrl1->Enable(false);

    //show the dialog box
    if (rd.ShowModal() == 0)
    {
        int iAngle;
        double dAngle;
        iAngle = rd.SpinCtrl1->GetValue();
        dAngle = iAngle;

        if ((rd.RadioButton3->GetValue()) && (m_Image))
        {
            //rotate image
            AddUndo();
            wxImage img;
            if (rd.RadioButton1->GetValue())
            {
                img = m_Image->Rotate90(true);
            }
            if (rd.RadioButton2->GetValue())
            {
                img = m_Image->Rotate90(true);
                img = img.Rotate90(true);
            }
            if (rd.RadioButton5->GetValue())
            {
                img = m_Image->Rotate90(false);
            }
            if (rd.RadioButton6->GetValue())
            {
                dAngle = 360 - dAngle; //to reverse the direction
                dAngle = dAngle * PI / 180; //conversion in Radians
                wxPoint ptCenter(m_Image->GetWidth() / 2, m_Image->GetHeight() / 2);

                NbPoints = 0; //Clear the selection

                img = m_Image->Rotate(dAngle, ptCenter, false, NULL);
            }
            SetImage(&img);
            SetModified(true);

        }
        else
        {
            if (HasSelection())
            {
                //rotate selection
                ConvertSelectionToRect();
                wxRect rSelection;

                GetBoundingRect(&rSelection);

                wxImage img;
                if (rd.RadioButton1->GetValue())
                {
                    img = m_SelectionImage.Rotate90(true);
                    pSelection[1].x = pSelection[0].x + img.GetWidth();
                    pSelection[2].x = pSelection[1].x;
                    pSelection[2].y = pSelection[0].y + img.GetHeight();
                    pSelection[3].y = pSelection[2].y;

                }
                if (rd.RadioButton2->GetValue())
                {
                    img = m_SelectionImage.Rotate90(true);
                    img = img.Rotate90(true);
                }
                if (rd.RadioButton5->GetValue())
                {
                    img = m_SelectionImage.Rotate90(false);
                    pSelection[1].x = pSelection[0].x + img.GetWidth();
                    pSelection[2].x = pSelection[1].x;
                    pSelection[2].y = pSelection[0].y + img.GetHeight();
                    pSelection[3].y = pSelection[2].y;
                }
                if (rd.RadioButton6->GetValue())
                {
                    dAngle = 360 - dAngle; //to reverse the direction
                    dAngle = dAngle * PI / 180; //conversion in Radians
                    wxPoint ptCenter(m_SelectionImage.GetWidth() / 2, m_SelectionImage.GetHeight() / 2);
                    wxPoint ptOffset(0,0);
                    img = m_SelectionImage;
                    img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());
                    img = img.Rotate(dAngle, ptCenter, false, &ptOffset);

                    //recompute selection
                    pSelection[1].x = pSelection[0].x + img.GetWidth() * cos(dAngle);
                    pSelection[1].y = pSelection[0].y + img.GetWidth() * sin(dAngle);
                    pSelection[2].x = pSelection[1].x + img.GetHeight() * sin(dAngle);
                    pSelection[2].y = pSelection[1].y + img.GetHeight() * cos(dAngle);
                    pSelection[3].x = pSelection[0].x + img.GetWidth() * sin(dAngle);
                    pSelection[3].y = pSelection[0].y + img.GetWidth() * cos(dAngle);


                }
                m_SelectionImage = img;
                m_SelectionBitmap = wxBitmap(m_SelectionImage);
                m_bEraseSelection = false;
                Repaint();
            }
        }
    }
}

/** Rotate 90 degree counter-clockwise handler **/
void XPMEditorPanel::RotateCounterClockwise(void)
{
    wxImage img;

    if (HasSelection())
    {
        ConvertSelectionToRect();
        img = m_SelectionImage.Rotate90(false);
        m_SelectionImage = img;
        m_SelectionBitmap = wxBitmap(m_SelectionImage);
        m_bEraseSelection = false;
        pSelection[1].x = pSelection[0].x + img.GetWidth();
        pSelection[2].x = pSelection[1].x;
        pSelection[2].y = pSelection[0].y + img.GetHeight();
        pSelection[3].y = pSelection[2].y;
        Repaint();
    }
    else
    {
        if (m_Image)
        {
            AddUndo();
            img = m_Image->Rotate90(false);
            SetImage(&img);
            SetModified(true);
        }
    }
}

/** Rotate 90 degree clockwise handler **/
void XPMEditorPanel::RotateClockwise(void)
{
    wxImage img;

    if (HasSelection())
    {
        ConvertSelectionToRect();
        img = m_SelectionImage.Rotate90(true);
        m_SelectionImage = img;
        m_SelectionBitmap = wxBitmap(m_SelectionImage);
        m_bEraseSelection = false;
        pSelection[1].x = pSelection[0].x + img.GetWidth();
        pSelection[2].x = pSelection[1].x;
        pSelection[2].y = pSelection[0].y + img.GetHeight();
        pSelection[3].y = pSelection[2].y;
        Repaint();
    }
    else
    {
        if (m_Image)
        {
            AddUndo();
            img = m_Image->Rotate90(true);
            SetImage(&img);
            SetModified(true);
        }
    }
}

/** Rotate Hue handler **/
void XPMEditorPanel::OnRotateHueClick(wxCommandEvent& event)
{
    wxRotateHue rd(this);

    /*
    wxImage::HSVValue greenHSV = wxImage::RGBtoHSV(wxImage::RGBValue(0, 255, 0));
        wxImage::HSVValue redHSV = wxImage::RGBtoHSV(wxImage::RGBValue(255, 0, 0));
        image.RotateHue(redHSV.hue - greenHSV.hue);
        colorized_horse_jpeg = wxBitmap( image );
    */

    //init the dialog box
    if (HasSelection())
    {
        rd.RadioButton4->Enable(true);
        rd.RadioButton4->SetValue(true);
    }
    else
    {
        rd.RadioButton4->Enable(false);
        rd.RadioButton3->SetValue(true);
    }

    //show the dialog box
    if (rd.ShowModal() == 0)
    {
        wxColour cFrom, cTo;
        double dAngle;
        cFrom = rd.PickerFrom->GetColour();
        cTo = rd.PickerTo->GetColour();
        wxImage::HSVValue FromHSV = wxImage::RGBtoHSV(wxImage::RGBValue(cFrom.Red(), cFrom.Green(), cFrom.Blue()));
        wxImage::HSVValue ToHSV = wxImage::RGBtoHSV(wxImage::RGBValue(cTo.Red(), cTo.Green(), cTo.Blue()));
        dAngle = ToHSV.hue - FromHSV.hue;

        if ((rd.RadioButton3->GetValue()))
        {
           //rotate hue for the image
           if (m_Image)
           {
               AddUndo();
               wxImage img;
               m_Image->RotateHue(dAngle);
               UpdateBitmap();
               SetModified(true);
           }
        }
        else
        {
            //rotate hue for the selection
            if (HasSelection())
            {
                m_SelectionImage.RotateHue(dAngle);
                m_SelectionBitmap = wxBitmap(m_SelectionImage);
                m_bEraseSelection = false;
                Repaint();
            }
        }
    }
}

/** Invert the image or the selection **/
void XPMEditorPanel::OnInvertImageClick(wxCommandEvent& event)
{
     wxInvertDialog id(this);

    //init the dialog box
    if (HasSelection())
    {
        id.RadioButton4->Enable(true);
        id.RadioButton4->SetValue(true);
    }
    else
    {
        id.RadioButton4->Enable(false);
        id.RadioButton3->SetValue(true);
    }

    //show the dialog box
    if (id.ShowModal() == 0)
    {
        if ((id.RadioButton3->GetValue()))
        {
           //invert image
           if (m_Image)
           {
               AddUndo();
               wxImage img;
               img = InvertImage(*m_Image);
               SetImage(&img);
               SetModified(true);
           }
        }
        else
        {
            //invert selection
            if (HasSelection())
            {
                wxImage img;
                img = InvertImage(m_SelectionImage);
                m_SelectionImage = img;
                m_SelectionBitmap = wxBitmap(m_SelectionImage);
                m_bEraseSelection = false;
                Repaint();
            }
        }
    }
}

/** Invert the colours of an image
  * \param img: the image to be inverted. The image is copied first - it is therefore not modified
  * \return : a new image, with the colours inverted
  */
wxImage XPMEditorPanel::InvertImage(wxImage img)
{
    //invert the colours of an image
    wxBitmap bmp(img);
    wxBitmap bmp2(img);
    wxMemoryDC dc;
    wxMemoryDC dc2;

    dc.SelectObject(bmp);
    dc2.SelectObject(bmp2);
    dc.Blit(0,0, bmp2.GetWidth(), bmp2.GetHeight(), &dc2, 0, 0, wxSRC_INVERT);
    dc.SelectObject(wxNullBitmap);
    dc2.SelectObject(wxNullBitmap);

    wxImage imgResult;
    imgResult = bmp.ConvertToImage();
    return(imgResult);
}

/** Colour Depth change handler **/
void XPMEditorPanel::OnButtonColourDepthClick(wxCommandEvent& event)
{
    wxConversion cd(this);

    //init the dialog box
    if (HasSelection())
    {
        cd.RadioButton4->Enable(true);
        cd.RadioButton4->SetValue(true);
    }
    else
    {
        cd.RadioButton4->Enable(false);
        cd.RadioButton3->SetValue(true);
    }
    cd.RadioButton1->SetValue(true);

    //show the dialog box
    if (cd.ShowModal() == 0)
    {
        bool bMonochrome;
        if (cd.RadioButton1->GetValue()) bMonochrome = true; else bMonochrome = false;

        if ((cd.RadioButton3->GetValue()))
        {
           //mirror image
           if (m_Image)
           {
               AddUndo();
               wxImage img;
               if (bMonochrome) img = m_Image->ConvertToMono(255,255,255);
               else img = m_Image->ConvertToGreyscale();
               SetImage(&img);
               SetModified(true);
           }
        }
        else
        {
            //mirror selection
            if (HasSelection())
            {
                wxImage img;
                if (bMonochrome) img = m_SelectionImage.ConvertToMono(255,255,255);
                else img = m_SelectionImage.ConvertToGreyscale();
                m_SelectionImage = img;
                m_SelectionBitmap = wxBitmap(m_SelectionImage);
                m_bEraseSelection = false;
                Repaint();
            }
        }
    }
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnTopLeftSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_LEFT;
    tdata.iVertAlign  = wxALIGN_TOP;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnTopCenterSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_CENTER;
    tdata.iVertAlign  = wxALIGN_TOP;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnTopRightSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_RIGHT;
    tdata.iVertAlign  = wxALIGN_TOP;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnCenterLeftSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_LEFT;
    tdata.iVertAlign  = wxALIGN_CENTER;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnCenterCenterSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_CENTER;
    tdata.iVertAlign  = wxALIGN_CENTER;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnCenterRightSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_RIGHT;
    tdata.iVertAlign  = wxALIGN_CENTER;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnBottomLeftSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_LEFT;
    tdata.iVertAlign  = wxALIGN_BOTTOM;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnBottomCenterSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_CENTER;
    tdata.iVertAlign  = wxALIGN_BOTTOM;
    DrawTextBitmap();
}

/** Text alignement handler - radio button **/
void XPMEditorPanel::OnBottomRightSelect(wxCommandEvent& event)
{
    tdata.iHorizAlign = wxALIGN_RIGHT;
    tdata.iVertAlign  = wxALIGN_BOTTOM;
    DrawTextBitmap();
}

/** Spin Control for text alignment - force 90, 180, 270 or 0 **/
void XPMEditorPanel::OnSpinAngleChange(wxSpinEvent& event)
{
    int iCurrentPos;
    wxSpinCtrl *SpinAngle;
    iCurrentPos = event.GetPosition();

    //wxMessageBox(wxString::Format(_("iPos=%d iCurrentPos=%d"), iPos, iCurrentPos));
    if (ToolPanel) SpinAngle = ToolPanel->SpinAngle; else SpinAngle = NULL;

    if ((iPos > iCurrentPos) && (SpinAngle))
    {
        //the user downgrades the value

        if (iCurrentPos <= 90) SpinAngle->SetValue(0);
        if ((iCurrentPos <= 180) && (iCurrentPos > 90)) SpinAngle->SetValue(90);
        if ((iCurrentPos <= 270) && (iCurrentPos > 180)) SpinAngle->SetValue(180);
        if ((iCurrentPos <= 360) && (iCurrentPos > 270)) SpinAngle->SetValue(270);
    }
    else if (SpinAngle)
    {
        //the user upgrades the value
        if (iCurrentPos > 270) SpinAngle->SetValue(0);
        if ((iCurrentPos > 180) && (iCurrentPos <= 270)) SpinAngle->SetValue(270);
        if ((iCurrentPos > 90) && (iCurrentPos <= 180)) SpinAngle->SetValue(180);
        if ((iCurrentPos > 0) && (iCurrentPos <= 90)) SpinAngle->SetValue(90);
    }
    iPos = iCurrentPos;

}

/** The Hot Spot colour must be updated
  */
void XPMEditorPanel::OnHotSpotColourPickerColourChanged(wxColourPickerEvent& event)
{
    cHotSpotColour = event.GetColour();
    Repaint();
}

/** The Grid colour must be updated
  */
void XPMEditorPanel::OnGridColourPickerColourChanged(wxColourPickerEvent& event)
{
    cGridColour = event.GetColour();
    Repaint();
}

#if __WXMSW__
/** The mouse capture has been lost
    Reinitialize the tools
  */
void XPMEditorPanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    //the mouse capture was lost
    InitToolData();
}

#endif

/** Convert the coordinates stored in tdata.x1, tdata.x2, tdata.y1 & tdata.y2
  * The coordinates are scaled, and converted to the real pixel coordinate
  * \param x1 : pointer to the resulting top left x coordinate
  * \param y1 : pointer to the resulting top left y coordinate
  * \param x2 : pointer to the resulting bottom right x coordinate
  * \param y2 : pointer to the resulting bottom right y coordinate
  * \param bInvert: if true, the coordinates will be rearranged,
  *                 so (x1,y1) is always Top Left, and (x2,y2) bottom right
  *                 if false, no coordinates are swapped, and x1,y1 is not modified
  */
void XPMEditorPanel::SnapRectToGrid(int *x1, int *y1, int *x2, int *y2, bool bInvert)
{
    //4 cases : (x1,y1) is the top left corner, or the Top right corner
    //          or the bottom left corner, or the bottom right corner

    if ((tdata.x1 < tdata.x2) && (tdata.y1 < tdata.y2))
    {
        //(x1,y1) is the top left corner
        //we round it off - down . X will be moved to A
        //  A-----+-----+
        //  |  X  |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----B
        if (bInvert)
        {
            *x1 = tdata.x1 / dScale;
            *x1 = *x1 * dScale;
            *y1 = tdata.y1 / dScale;
            *y1 = *y1 * dScale;
        }
        else
        {
            *x1 = tdata.x1;
            *y1 = tdata.y1;
        }

        //we round (x2, y2) off - up . X will be moved to B
        //  A-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |  X  |
        //  |     |     |
        //  +-----+-----B

        *x2 = tdata.x2 / dScale;
        *x2 = *x2 + 1;
        *x2 = *x2 * dScale;
        *y2 = tdata.y2 / dScale;
        *y2 = *y2 + 1;
        *y2 = *y2 * dScale;
    }
    else if ((tdata.x2 < tdata.x1) && (tdata.y1 < tdata.y2))
    {
        //(x1,y1) is the top left corner
        //we round it off - down . X will be moved to A
        //  +-----+-----A
        //  |     |  X  |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  B-----+-----+
        if (bInvert)
        {
            *x1 = tdata.x1 / dScale;
            *x1 = *x1 + 1;
            *x1 = *x1 * dScale;
            *y1 = tdata.y1 / dScale;
            *y1 = *y1 * dScale;
        }
        else
        {
            *x1 = tdata.x1;
            *y1 = tdata.y1;
        }

        //we round (x2, y2) off - up . X will be moved to B
        //  +-----+-----A
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  | X   |     |
        //  |     |     |
        //  B-----+-----+

        *x2 = tdata.x2 / dScale;
        *x2 = *x2 * dScale;
        *y2 = tdata.y2 / dScale;
        *y2 = *y2 + 1;
        *y2 = *y2 * dScale;

        //reorder the x coordinates
        if (bInvert)
        {
            int x;
            x = *x1;
            *x1 = *x2;
            *x2 = x;
        }

    }
    else if ((tdata.x2 < tdata.x1) && (tdata.y2 < tdata.y1))
    {
        //(x2,y2) is the top left corner
        //we round it off - down . X will be moved to A
        //  B-----+-----+
        //  |  X  |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----A
        *x2 = tdata.x2 / dScale;
        *x2 = *x2 * dScale;
        *y2 = tdata.y2 / dScale;
        *y2 = *y2 * dScale;

        //we round (x2, y2) off - up . X will be moved to B
        //  B-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |  X  |
        //  |     |     |
        //  +-----+-----A
        if (bInvert)
        {
            *x1 = tdata.x1 / dScale;
            *x1 = *x1 + 1;
            *x1 = *x1 * dScale;
            *y1 = tdata.y1 / dScale;
            *y1 = *y1 + 1;
            *y1 = *y1 * dScale;
        }
        else
        {
            *x1 = tdata.x1;
            *y1 = tdata.y1;
        }

        //invert the coordinates
        if (bInvert)
        {
            int x, y;
            x = *x1;
            y = *y1;
            *x1 = *x2;
            *y1 = *y2;
            *x2 = x;
            *y2 = y;
        }
    }
    else
    {
        //(x1,y1) is the top left corner
        //we round it off - down . X will be moved to A
        //  +-----+-----B
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |     |
        //  | X   |     |
        //  A-----+-----+
        if (bInvert)
        {
            *x1 = tdata.x1 / dScale;
            *x1 = *x1 * dScale;
            *y1 = tdata.y1 / dScale;
            *y1 = *y1 + 1;
            *y1 = *y1 * dScale;
        }
        else
        {
            *x1 = tdata.x1;
            *y1 = tdata.y1;
        }

        //we round (x2, y2) off - up . X will be moved to B
        //  +-----+-----B
        //  |     |  X  |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  A-----+-----+

        *x2 = tdata.x2 / dScale;
        *x2 = *x2 + 1;
        *x2 = *x2 * dScale;
        *y2 = tdata.y2 / dScale;
        *y2 = *y2 * dScale;

        //reorder the y coordinates
        if (bInvert)
        {
            int y;
            y = *y1;
            *y1 = *y2;
            *y2 = y;
        }

    }
}

/** Convert the coordinates stored in x & y to snapped to grid coordinates
  * The coordinates are scaled, and converted to the real pixel coordinate
  * \param x : pointer to the input and resulting top left x coordinate - IN & OUT
  * \param y : pointer to the input and resulting top left y coordinate - IN & OUT
  * \param bUp: if true, will snap the point up, else down
  */
void XPMEditorPanel::SnapToGrid(int *x, int *y, bool bUp)
{
    if (!x) return;
    if (!y) return;

    if (bUp)
    {
        //we round it off - down . X will be moved to A
        //  A-----+-----+
        //  |  X  |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----B
        *x = *x / dScale;
        *x = *x * dScale;
        *y = *y / dScale;
        *y = *y * dScale;
    }
    else
    {
        //we round (x, y) off - up . X will be moved to B
        //  A-----+-----+
        //  |     |     |
        //  |     |     |
        //  |     |     |
        //  +-----+-----+
        //  |     |     |
        //  |     |  X  |
        //  |     |     |
        //  +-----+-----B

        *x = *x / dScale;
        *x = *x + 1;
        *x = *x * dScale;
        *y = *y / dScale;
        *y = *y + 1;
        *y = *y * dScale;
    }
}


