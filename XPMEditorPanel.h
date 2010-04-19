/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      XPMEditorPanel.h
 * Purpose:   the panel containing all the drawing tools and the draw canvas - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#ifndef XPM_EDITOR_PANEL_H
#define XPM_EDITOR_PANEL_H



class wxDragImage;
//class wxBitmap;

#include <wx/image.h>
#include <wx/bitmap.h>

//(*Headers(XPMEditorPanel)
#include <wx/panel.h>
class wxSpinEvent;
class wxCheckBox;
class wxTextCtrl;
class XPMColorPicker;
class wxComboBox;
class wxCustomButton;
class wxScrolledWindow;
class wxRadioButton;
class wxStaticText;
class wxToggleButton;
class wxBitmapButton;
class wxSpinCtrl;
class wxBoxSizer;
class wxResizeCtrl;
class wxButton;
class wxGridSizer;
//*)

#define XPM_NUMBER_TOOLS 14
#define XPM_MAXPOINTS 25

enum
{
    XPM_CIRCLE_INDEX=0,
    XPM_SQUARE_INDEX=1,
    XPM_LEFT_HAIR_INDEX=2,
    XPM_RIGHT_HAIR_INDEX=3
};

enum
{
    XPM_CIRCLE_INDEX_LIST=0,
    XPM_SQUARE_INDEX_LIST=1,
    XPM_LEFT_HAIR_INDEX_LIST=2,
    XPM_RIGHT_HAIR_INDEX_LIST=3
};

enum
{
    XPM_ID_SELECT_TOOL=0,
    XPM_ID_LASSO_TOOL,
    XPM_ID_PEN_TOOL,
    XPM_ID_BRUSH_TOOL,
    XPM_ID_PIPETTE_TOOL,
    XPM_ID_FILL_TOOL,
    XPM_ID_CURVE_TOOL,
    XPM_ID_LINE_TOOL,
    XPM_ID_ERASER_TOOL,
    XPM_ID_TEXT_TOOL,
    XPM_ID_RECTANGLE_TOOL,
    XPM_ID_POLYGON_TOOL,
    XPM_ID_ELLIPSE_TOOL,
    XPM_ID_ROUNDEDRECT_TOOL,
    XPM_ID_DRAG_TOOL,
    XPM_ID_STRETCH_TOOL
};

/// @brief Structure used to store Tool specific data
struct ToolData
{
    int x1;          ///< @brief the 1st click X position
    int y1;          ///< @brief the 1st click Y position
    int x2;          ///< @brief the 2nd click X position
    int y2;          ///< @brief the 2nd click Y position
    int iNbClicks;   ///< @brief the number of times the left mouse button has been clicked with the tool
    int iStyle;      ///< @brief represent a style for the tool (like the brush style: square, circle, ...)
    int iSize;       ///< @brief represent the size to be applied to the tool (brush thickness for example)
    wxPoint pts[XPM_MAXPOINTS+1]; ///< @brief an array of points. Statically limited to 25, to simplify the plugin
    int iNbPoints;   ///< @brief how many points are in the array
    int iRadius;     ///< @brief the radius for rounded rectangle
    wxString sText;  ///< @brief the text string to be used for the tool (Text tool)
    wxFont font;     ///< @brief the font to be used for the tool (Text tool)
    int iHorizAlign; ///< @brief text horizontal alignment: wxALIGN_RIGHT, wxALIGN_LEFT, wxALIGN_CENTER
    int iVertAlign;  ///< @brief text horizontal alignment: wxALIGN_BOTTOM, wxALIGN_LEFT, wxALIGN_TOP
    int angle;       ///< @brief text angle
};

class XPMUndo;

class XPMEditorPanel: public wxPanel
{
	public:

		XPMEditorPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMEditorPanel();

        //Bitmap & draw canvas access
		wxImage GetImage(void);        ///< \brief return a copy the associated image (unscaled)
		void SetImage(wxImage *img);    ///< \brief set the current image (unscaled)
		wxScrolledWindow* GetDrawCanvas(void); ///< \brief get the DrawCanvas
		void Repaint(void);            ///< \brief update the display

		void SetModified(bool bModified); ///< \brief Set the Modification flag
		bool GetModified(void);           ///< \brief Get the Modification flag

        //sizing & scaling
        double GetScaleFactor(void); ///< \brief Get Scaling (zoom) Factor
        void SetScaleFactor(double dNewScalingFactor); ///< \brief Set Scaling (zoom) Factor
        wxSize GetDrawAreaSize(void); ///< \brief Get Bitmap Size
        void SetDrawAreaSize(wxSize sNewDrawAreaSize); ///< \brief Set Bitmap Size

        //Selection methods
        bool HasSelection(void) const;          ///< \brief returns true if it has selection
		void ClearSelection(void);              ///< \brief clear the current selection
		void GetBoundingRect(wxRect *r);        ///< \brief Get the bounding rectangle of the selection
        int IsPointInSelection(int x, int y);   ///< \brief Return if the cursor is hovering over the selection (1 for over the selection, 2 for edge, 0 otherwise)
        void ReplaceRect(const wxImage &newImage, wxRect rRect);///< \brief Replace the rectangle by a new image.
        void PasteImage(const wxImage &newImage, int x, int y); ///< \brief Paste an image at the specified location
        void ConvertSelectionToRect(void);      ///< \brief convert the selection to a rectangular selection, for simpler stretching

        //UNDO & REDO Methods
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

		//Clipboard operations
		void Cut(void);                  ///< \brief Perform a Cut operation
		void Copy(void);                 ///< \brief perform a Copy operation
		void Paste(void);                ///< \brief perform a Paste operation

		//Configuration
		void UpdateConfiguration(void); ///< @brief ask all the image editors to update their configuration

		//(*Declarations(XPMEditorPanel)
		wxBoxSizer* ToolSizer;
		wxSpinCtrl* SpinCtrl4;
		wxCustomButton* FillButton;
		wxCustomButton* EraserButton;
		wxSpinCtrl* BMPHeight;
		wxRadioButton* CenterCenter;
		wxStaticText* sCursorPos;
		wxButton* Button4;
		wxScrolledWindow* DrawCanvas;
		wxRadioButton* TopCenter;
		wxSpinCtrl* SpinCtrl1;
		wxSpinCtrl* BMPWidth;
		wxStaticText* StaticText2;
		wxBoxSizer* CanvasSizer;
		wxTextCtrl* TextEdit;
		wxRadioButton* CenterLeft;
		wxCustomButton* SquareBrushButton;
		wxButton* Button1;
		wxStaticText* StaticText6;
		wxCustomButton* BrushButton;
		wxRadioButton* BottomCenter;
		wxCustomButton* LineButton;
		wxCustomButton* PenButton;
		wxStaticText* StaticText8;
		wxBitmapButton* BitmapButton2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxBitmapButton* BitmapButton1;
		wxButton* Button2;
		wxPanel* ToolPanel;
		wxCheckBox* CheckBox1;
		wxButton* Button6;
		wxButton* FontButton;
		wxCustomButton* PipetteButton;
		wxButton* Button5;
		wxRadioButton* BottomLeft;
		wxButton* Button3;
		wxSpinCtrl* SpinCtrl3;
		wxStaticText* StaticText5;
		wxCustomButton* SelectButton;
		wxResizeCtrl* ResizeCtrl1;
		wxStaticText* StaticText7;
		wxCustomButton* EllipseButton;
		wxRadioButton* TopRight;
		wxCustomButton* RHairBrushButton;
		wxSpinCtrl* SpinCtrl2;
		wxGridSizer* AlignmentSizer;
		wxComboBox* ZoomFactor;
		XPMColorPicker* ColourPicker;
		wxBoxSizer* PanelSizer;
		wxToggleButton* BackgroundButton;
		wxCustomButton* LassoButton;
		wxCustomButton* RectangleButton;
		wxCustomButton* CircleBrushButton;
		wxRadioButton* BottomRight;
		wxRadioButton* CenterRight;
		wxCustomButton* RRectButton;
		wxBoxSizer* AngleSizer;
		wxBoxSizer* ToolPanelSizer;
		wxStaticText* StaticText4;
		wxCustomButton* LHairBrushButton;
		wxCustomButton* CurveButton;
		wxCustomButton* TextButton;
		wxRadioButton* TopLeft;
		wxCustomButton* PolygonButton;
		//*)

	protected:

		//(*Identifiers(XPMEditorPanel)
		static const long ID_STATICTEXT1;
		static const long ID_COMBOBOX1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRL2;
		static const long ID_CHECKBOX1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BITMAPBUTTON1;
		static const long ID_BITMAPBUTTON2;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_CUSTOM1;
		static const long ID_SELECT_BUTN;
		static const long ID_LASSO_BTN;
		static const long ID_PEN_BTN;
		static const long ID_BRUSH_BTN;
		static const long ID_PIPETTE_BTN;
		static const long ID_FILL_BTN;
		static const long ID_LINE_BTN;
		static const long ID_CURVE_BTN;
		static const long ID_ERASER_BTN;
		static const long ID_TEXT_BTN;
		static const long ID_RECTANGLE_BTN;
		static const long ID_POLYGON_BTN;
		static const long ID_ELLIPSE_BTN;
		static const long ID_ROUNDEDRECT_BTN;
		static const long ID_SQUARE_BRUSH;
		static const long ID_CIRCLE_BRUSH;
		static const long ID_LRHAIR_BRUSH;
		static const long ID_LHAIR_BRUSH;
		static const long ID_FONT_BUTTON;
		static const long ID_BKMODE_TOGGLEBUTTON;
		static const long ID_STATICTEXT8;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON9;
		static const long ID_RADIOBUTTON8;
		static const long ID_RADIOBUTTON7;
		static const long ID_RADIOBUTTON6;
		static const long ID_RADIOBUTTON5;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON2;
		static const long ID_STATICTEXT9;
		static const long ID_SPINCTRL6;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRL3;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_SPINCTRL4;
		static const long ID_PANEL1;
		static const long ID_TEXTCTRL1;
		static const long ID_RESIZECTRL1;
		static const long ID_SCROLLEDWINDOW1;
		static const long ID_STATICTEXT4;
		//*)

		//(*Handlers(XPMEditorPanel)
		void OnDrawCanvasPaint(wxPaintEvent& event);
		void OnBitmapSizeChanged(wxSpinEvent& event);
		void OnZoomChanged(wxCommandEvent& event);
		void OnDrawCanvasResize(wxSizeEvent& event);
		void OnShowGrid(wxCommandEvent& event);
		void OnDrawCanvasMouseMove(wxMouseEvent& event);
		void OnDrawCanvasLeftDown(wxMouseEvent& event);
		void OnDrawCanvasLeftUp(wxMouseEvent& event);
		void OnDrawCanvasMouseLeave(wxMouseEvent& event);
		void OnDrawCanvasLeftDClick(wxMouseEvent& event);
		void OnDrawCanvasEraseBackground(wxEraseEvent& event);
		void OnSpinSizeChanged(wxSpinEvent& event);
		void OnSpinRadiusChanged(wxSpinEvent& event);
		void OnEllipseButtonToggle(wxCommandEvent& event);
		void OnRRectButtonToggle(wxCommandEvent& event);
		void OnRectangleButtonToggle(wxCommandEvent& event);
		void OnPolygonButtonToggle(wxCommandEvent& event);
		void OnEraserButtonToggle(wxCommandEvent& event);
		void OnTextButtonToggle(wxCommandEvent& event);
		void OnCurveButtonToggle(wxCommandEvent& event);
		void OnLineButtonToggle(wxCommandEvent& event);
		void OnPipetteButtonToggle(wxCommandEvent& event);
		void OnFillButtonToggle(wxCommandEvent& event);
		void OnPenButtonToggle(wxCommandEvent& event);
		void OnBrushButtonToggle(wxCommandEvent& event);
		void OnLassoButtonToggle(wxCommandEvent& event);
		void OnSelectButtonToggle(wxCommandEvent& event);
		void OnSquareBrushButtonToggle(wxCommandEvent& event);
		void OnCircleBrushButtonToggle(wxCommandEvent& event);
		void OnLHairBrushButtonToggle(wxCommandEvent& event);
		void OnRHairBrushButtonToggle(wxCommandEvent& event);
		void OnDrawCanvasRightUp(wxMouseEvent& event);
		void OnFontButtonClick(wxCommandEvent& event);
		void OnBackgroundButtonToggle(wxCommandEvent& event);
		void OnTextEditText(wxCommandEvent& event);
		void OnDrawCanvasKeyDown(wxKeyEvent& event);
		void OnStretchImage(wxCommandEvent& event);
		void OnMirror(wxCommandEvent& event);
		void OnBlur(wxCommandEvent& event);
		void OnRotate(wxCommandEvent& event);
		void OnRotateCounterClockwise(wxCommandEvent& event);
		void OnRotateClockwise(wxCommandEvent& event);
		void OnRotateHueClick(wxCommandEvent& event);
		void OnButtonColourDepthClick(wxCommandEvent& event);
		void OnTopLeftSelect(wxCommandEvent& event);
		void OnTopCenterSelect(wxCommandEvent& event);
		void OnTopRightSelect(wxCommandEvent& event);
		void OnCenterLeftSelect(wxCommandEvent& event);
		void OnCenterCenterSelect(wxCommandEvent& event);
		void OnCenterRightSelect(wxCommandEvent& event);
		void OnBottomLeftSelect(wxCommandEvent& event);
		void OnBottomCenterSelect(wxCommandEvent& event);
		void OnBottomRightSelect(wxCommandEvent& event);
		void OnSpinCtrl4Change(wxSpinEvent& event);
		//*)

		void OnTransparentColorChanged(wxCommandEvent& event); ///< \brief the transparent colour in the colour picker changed
		void OnLineColorChanged(wxCommandEvent& event);        ///< \brief the Line colour in the colour picker changed
		void OnFillColorChanged(wxCommandEvent& event);        ///< \brief the Fill colour in the colour picker changed

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);
		void ToggleButtons(int iIndex, bool bClearSelection = true); ///< @brief toggle all Tools Buttons OFF, except the iIndex one.
		void SetToolCursor(void); ///< @brief Set the correct cursor, according to the currently selected tool & option
        int GetToolID(void);  ///< @brief Get the tool ID currently in use
        void SetToolID(int iTool); ///< @brief Set the tool ID currently in use
        void ProcessToolAction(int iTool, int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief draw the tool action: select box for Selection tool, rectangle for rectangle tool, ...
        void HideControls(int iIndex, bool bChecked); ///< @brief Hide or Show specific tool controls - like style listbox for Brush tool...
        void HideControlsAndDoLayout(int iIndex, bool bChecked); ///< @brief Hide or Show specific tool controls - like style listbox for Brush tool... + update the layout

        //methods for processing the tools
        void ProcessPen(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Pen tool
        void ProcessFill(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Fill tool
        void ProcessPipette(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Pipette tool
        void ProcessSelect(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Selection tool
        void ProcessLasso(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Lasso (complex selection) tool
        void ProcessRectangle(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Rectangle tool
        void ProcessLine(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Line tool
        void ProcessCurve(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Curve tool
        void ProcessRoundedRectangle(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Rounded Rectangle tool
        void ProcessEllipse(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick); ///< @brief process the Ellipse tool
        void ProcessPolygon(int x, int y,
                            bool bLeftDown, bool bLeftUp,
                            bool bPressed, bool bDClick); ///< @brief process the Polygon tool
        void ProcessEraser(int x, int y,
                            bool bLeftDown, bool bLeftUp,
                            bool bPressed, bool bDClick); ///< @brief process the Eraser tool
        void ProcessBrush(int x, int y,
                            bool bLeftDown, bool bLeftUp,
                            bool bPressed, bool bDClick); ///< @brief process the Brush tool
        void ProcessText(int x, int y,
                         bool bLeftDown, bool bLeftUp,
                         bool bPressed, bool bDClick); ///< @brief process the text tool
        void ProcessDragAction(int x, int y,
                               bool bLeftDown, bool bLeftUp,
                               bool bPressed, bool bDClick); ///< @brief process the drag & drop image tool
        void ProcessSizeAction(int x, int y,
                               bool bLeftDown, bool bLeftUp,
                               bool bPressed, bool bDClick, int iDirection); ///< @brief process the stretch selection tool

    private:
        //bitmap, images methods
        wxColour cMaskColour;           ///< \brief the current mask colour
        wxBitmap *m_Bitmap;             ///< \brief the temporary bitmap, used for drawing
        wxImage *m_Image;               ///< \brief the temporary image, used for misc functions
        void UpdateBitmap(void);        ///< \brief recreate the m_Bitmap member from the m_Image member
        void UpdateImage(void);         ///< \brief Ensure the Image is up-to-date (buffered user actions are flushed)
        wxBitmap* GetBitmap(void);      ///< \brief return the associated scaled bitmap
		void SetBitmap(wxBitmap *bm);   ///< \brief set the current unscaled bitmap

        //drag related methods & members
		wxDragImage *m_DragImage;        ///< \brief for dragging the current selection
		bool m_bDragging;                ///< \brief true if the user is currently dragging a shape, false otherwise
		wxImage m_SelectionImage;        ///< \brief for dragging selection: save the image to drag
		wxBitmap m_SelectionBitmap;      ///< \brief for drawing the selection
		bool m_bEraseSelection;          ///< \brief if true, the selection will be erased during a drag operation
        wxPoint pStartDragging;          ///< \brief position for the 1st dragging event

        //scale factor & scrollbars
        double dScale;          ///< \brief scale factor
        bool bShowGrid;         ///< \brief Grid display
        void DoSetScrollBars(void); ///< \brief Set scrollbars size

        //Sizing
        wxSize sDrawAreaSize;   ///< \brief Canvas size
        bool bCanResizeX;   ///< \brief indicate that the mouse is in a sizing area (border)
        bool bCanResizeY;   ///< \brief indicate that the mouse is in a sizing area (border)
        bool bSizingX;      ///< \brief indicate that the user is currently resizing the bitmap
        bool bSizingY;      ///< \brief indicate that the user is currently resizing the bitmapnt
        int OldX;           ///< \brief For resizing the bitmap: indicates the last x mouse position
        int OldY;           ///< \brief For resizing the bitmap: indicates the last x mouse position

        //Text tool methods & members
        int iPos;           ///< \brief The current SpinCtrl4 value.
        void DrawTextRectangle( wxDC& dc,
                                const wxString& value,
                                const wxRect& rect,
                                int horizAlign,
                                int vertAlign,
                                double textOrientation); ///< \brief draw a text in a rectangle. Copied from wxGrid
        void DrawTextRectangle(wxDC& dc,
                               const wxArrayString& lines,
                               const wxRect& rect,
                               int horizAlign,
                               int vertAlign,
                               double textOrientation); ///< \brief draw a text in a rectangle. Copied from wxGrid
        void StringToLines(const wxString& value, wxArrayString& lines); ///< \brief Split multi-line text up into an array of strings. Copied from wxGrid
        void GetTextBoxSize(const wxDC& dc,
                            const wxArrayString& lines,
                            long *width, long *height); ///< \brief Compute the ideal TextBox size for the string to draw. Copied from wxGrid

        //Selection members & methods
        wxPoint *pSelection;        ///< \brief indicates the points coordinates defining the selected region
        int NbPoints;               ///< \brief how many points are defining the region (rectangle = 4 points)
        int NbPointsMax;            ///< \brief the maximal size of the wxPoint array
        bool m_bSizing;             ///< \brief if true, the user is currently resizing the selection
        int m_iSizeAction;          ///< \brief the direction of the selection stretch / resizing (see IsPointInSelection() for a list of code)
        bool bDrawSelection;        ///< \brief true to draw the selection in OnDrawCanvasPaint
        wxPoint* CheckMemorySelection(int iNeeded); ///< \brief increase the memory if needed
        wxImage GetImageFromSelection(void); ///< \brief Return an image representing the selection
        void CutSelection(void);    ///< \brief Replace the Selection with the mask colour
        void MoveSelection(int dx, int dy); ///< \brief Move the selection
        void PasteSelection(void);  ///< \brief Paste the current selection to the current selection coordinates
        void DrawTextBitmap(void);  ///< \brief Draw the text bitmap on the selection image


        //Undo & Redo buffers
        XPMUndo *m_undo_buffer;  ///< \brief the Undo buffer

        //Drawing tools ids, & cursors
        wxCustomButton* tools[XPM_NUMBER_TOOLS]; ///< \brief the buttons associated to the tools
        int iToolUsed; ///< \brief the index of the tool currently in use
        wxCursor ToolCursor[XPM_NUMBER_TOOLS]; ///< \brief the cursors associated to the tools
        ToolData tdata; ///< \brief tool specific data
        void InitToolData(void); ///< @brief init tool data for a first use
        bool bUsingTool;  ///< @brief true if a tool is currently in use

        //standard configuration
        int iXPMDefaultWidth;  ///< @brief the default width of a new XPM
        int iXPMDefaultHeight; ///< @brief the default height of a new XPM
        wxColour cBackgroundColour; ///< @brief Background colour for the canvas


        DECLARE_EVENT_TABLE()
};

//brush style
enum
{
    XPM_BRUSH_STYLE_SQUARE = 0,
    XPM_BRUSH_STYLE_CIRCLE = 1,
    XPM_BRUSH_STYLE_LEFTHAIR = 2,
    XPM_BRUSH_STYLE_RIGHTHAIR = 3
};

#endif
