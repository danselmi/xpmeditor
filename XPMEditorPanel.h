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

#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>

//(*Headers(XPMEditorPanel)
#include <wx/panel.h>
class XPMColourPickerPanel;
class XPMInterfacePanel;
class wxAuiManager;
class XPMDrawCanvasPanel;
class XPMFoldPanel;
class wxAuiManagerEvent;
//*)

class wxScrolledWindow;

class XPMToolPanel;
class XPMHelpPanel;
class XPMImagePropertiesPanel;
class XPMImageManipulationPanel;
class XPMUndo;
class wxResizeCtrl;
class wxStaticText;

#define XPM_NUMBER_TOOLS 17
#define XPM_MAXPOINTS 200

/// \brief Structure used to store Tool specific data
struct ToolData
{
    int x1;                 ///< \brief the 1st click X position
    int y1;                 ///< \brief the 1st click Y position
    int x2;                 ///< \brief the 2nd click X position
    int y2;                 ///< \brief the 2nd click Y position
    int iNbClicks;          ///< \brief the number of times the left mouse button has been clicked with the tool
    int iStyle;             ///< \brief represent a style for the tool (like the brush style: square, circle, ...)
    int iSize;              ///< \brief represent the thickness to be applied to the tool (line thickness for example)
    int iSize2;             ///< \brief represent the size to be applied to the tool (brush size for example)
    wxPoint pts[XPM_MAXPOINTS+1]; ///< \brief an array of points. Statically limited to 25, to simplify the plugin
    int iNbPoints;          ///< \brief how many points are in the array
    int iRadius;            ///< \brief the radius for rounded rectangle
    wxString sText;         ///< \brief the text string to be used for the tool (Text tool)
    wxFont font;            ///< \brief the font to be used for the tool (Text tool)
    int iHorizAlign;        ///< \brief text horizontal alignment: wxALIGN_RIGHT, wxALIGN_LEFT, wxALIGN_CENTER
    int iVertAlign;         ///< \brief text horizontal alignment: wxALIGN_BOTTOM, wxALIGN_LEFT, wxALIGN_TOP
    int angle;              ///< \brief text angle
    int iPenStyle;          ///< \brief the line style to use (see doc wxPen for more information)
    int iBrushStyle;        ///< \brief the brush style to use (see doc wxBrush for more info)
    int iGradient;          ///< \brief 0: Linear gradient. Other: Concentric gradient
    int iGradientDirection; ///< \brief 0: To the top; 1: to the bottom; 2: to the left; 3: to the right
};



class XPMEditorPanel: public wxPanel
{
	public:
        //constructors, destructor and initialisation
		XPMEditorPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~XPMEditorPanel();

		//image format methods
		bool IsValidFormat(wxBitmapType btFormat);           ///< \brief Return true if the format is supported as a saving format.
		wxBitmapType GetImageFormat(void);                   ///< \brief Gets the format used to save the image.
		void SetImageFormat(wxBitmapType btFormat);          ///< \brief Sets the format used to save the image. If the format is not supported, nothing is done
		bool SetImageFormatFromFilename(wxString sFilename); ///< \brief Autodetect the file format for the image, based on the file extension

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
		void UpdateConfiguration(void); ///< \brief ask all the image editors to update their configuration

		//Image & Selection manipulation
		void RotateCounterClockwise(void); ///< \brief Rotate the image or the selection 90° Clockwise
		void RotateClockwise(void);        ///< \brief Rotate the image or the selection 90° CounterClockwise
		wxImage InvertImage(wxImage img);  ///< \brief Invert the colours of an image.

		void ShowGrid(bool bShow); ///< Show or hide the grid

		//tools data
		void InitToolData(void);     ///< \brief init tool data for a first use
		void GetToolData(ToolData *t);  ///< \brief get the tool data
		void SetToolData(ToolData *t);  ///< \brief set the tool data
		void SetToolCursor(const wxCursor &cursor);  ///< \brief Set the cursor for the DrawCanvas

		void ShowCanvasWidgets(bool bShow); ///< \brief hide or show some widgets

		//handlers for events forwarded from child panels
		void OnTransparentColorChanged(wxCommandEvent& event); ///< \brief the transparent colour in the colour picker changed
		void OnLineColorChanged(wxCommandEvent& event);        ///< \brief the Line colour in the colour picker changed
		void OnFillColorChanged(wxCommandEvent& event);        ///< \brief the Fill colour in the colour picker changed
		void OnGridColourPickerColourChanged(wxColourPickerEvent& event);    ///< \brief The colour of the grid changed
		void OnImageSizeChanged(wxSpinEvent& event);           ///< \brief The size of the image changed
		void OnStretchImage(wxCommandEvent& event);            ///< \brief Stretch the image or the selection
		void OnMirror(wxCommandEvent& event);                  ///< \brief Flip the image or the selection
		void OnBlur(wxCommandEvent& event);                    ///< \brief Blur the image or the selection
		void OnRotate(wxCommandEvent& event);                  ///< \brief Rotate the image or the selection
		void OnRotateHueClick(wxCommandEvent& event);          ///< \brief Convert some colours from the image or the selection
		void OnButtonColourDepthClick(wxCommandEvent& event);  ///< \brief Convert to Gray Scale / Monochrome the image or the selection
		void OnInvertImageClick(wxCommandEvent& event);        ///< \brief Invert the image or the selection
		void OnFontButtonClick(wxCommandEvent& event);         ///< \brief The font for the text edition has been changed
		void OnBackgroundButtonToggle(wxCommandEvent& event);  ///< \brief The background mode for the text edition has been changed
		void OnTopLeftSelect(wxCommandEvent& event);           ///< \brief The Text alignment changed to top-left
		void OnTopCenterSelect(wxCommandEvent& event);         ///< \brief The Text alignment changed to top-center
		void OnTopRightSelect(wxCommandEvent& event);          ///< \brief The Text alignment changed to top-right
		void OnCenterLeftSelect(wxCommandEvent& event);        ///< \brief The Text alignment changed to center-left
		void OnCenterCenterSelect(wxCommandEvent& event);      ///< \brief The Text alignment changed to center-center
		void OnCenterRightSelect(wxCommandEvent& event);       ///< \brief The Text alignment changed to center-right
		void OnBottomLeftSelect(wxCommandEvent& event);        ///< \brief The Text alignment changed to bottom-left
		void OnBottomCenterSelect(wxCommandEvent& event);      ///< \brief The Text alignment changed to bottom-center
		void OnBottomRightSelect(wxCommandEvent& event);       ///< \brief The Text alignment changed to bottom-right
		void OnSpinAngleChange(wxSpinEvent& event);            ///< \brief The Text orientation angle changed.

		void SetHotSpotColour(wxColour cNewColour);             ///< \brief The HotSpot Tool Button has been toggled

		//(*Declarations(XPMEditorPanel)
		XPMFoldPanel* FoldPanel;
		wxAuiManager* m_AUIXPMEditor;
		XPMDrawCanvasPanel* DrawCanvasPanel;
		XPMColourPickerPanel* ColourPicker;
		XPMInterfacePanel* InterfacePanel;
		//*)

	protected:

		//(*Identifiers(XPMEditorPanel)
		static const long ID_DRAWCANVASPANEL;
		static const long ID_FOLDPANEL;
		static const long ID_COLOURPICKERPANEL;
		static const long ID_INTERFACEPANEL;
		//*)

		//(*Handlers(XPMEditorPanel)
		void OnDrawCanvasPaint(wxPaintEvent& event);
		void OnDrawCanvasResize(wxSizeEvent& event);
		void OnDrawCanvasMouseMove(wxMouseEvent& event);
		void OnDrawCanvasLeftDown(wxMouseEvent& event);
		void OnDrawCanvasLeftUp(wxMouseEvent& event);
		void OnDrawCanvasMouseLeave(wxMouseEvent& event);
		void OnDrawCanvasLeftDClick(wxMouseEvent& event);
		void OnDrawCanvasEraseBackground(wxEraseEvent& event);
		void OnDrawCanvasRightUp(wxMouseEvent& event);
		void OnTextEditText(wxCommandEvent& event);
		void OnDrawCanvasKeyDown(wxKeyEvent& event);
		//*)

#if __WXMSW__
		void OnMouseCaptureLost(wxMouseCaptureLostEvent& event); ///< \brief the mouse capture was lost
#endif

		void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);
		void ToggleButtons(int iIndex, bool bClearSelection = true); ///< \brief toggle all Tools Buttons OFF, except the iIndex one.
		void SetToolCursor(void); ///< \brief Set the correct cursor, according to the currently selected tool & option
        int GetToolID(void);  ///< \brief Get the tool ID currently in use
        void SetToolID(int iTool); ///< \brief Set the tool ID currently in use
        void ProcessToolAction(int iTool, int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief draw the tool action: select box for Selection tool, rectangle for rectangle tool, ...

        //methods for processing the tools
        void ProcessPen(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Pen tool
        void ProcessFill(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Fill tool
        void ProcessPipette(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Pipette tool
        void ProcessSelect(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Selection tool
        void ProcessLasso(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Lasso (complex selection) tool
        void ProcessRectangle(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Rectangle tool
        void ProcessLine(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Line tool
        void ProcessCurve(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Curve tool
        void ProcessRoundedRectangle(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Rounded Rectangle tool
        void ProcessEllipse(int x, int y,
                              bool bLeftDown, bool bLeftUp,
                              bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Ellipse tool
        void ProcessPolygon(int x, int y,
                            bool bLeftDown, bool bLeftUp,
                            bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Polygon tool
        void ProcessEraser(int x, int y,
                            bool bLeftDown, bool bLeftUp,
                            bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Eraser tool
        void ProcessBrush(int x, int y,
                            bool bLeftDown, bool bLeftUp,
                            bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the Brush tool
        void ProcessText(int x, int y,
                         bool bLeftDown, bool bLeftUp,
                         bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the text tool
        void ProcessDragAction(int x, int y,
                               bool bLeftDown, bool bLeftUp,
                               bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the drag & drop image tool
        void ProcessSizeAction(int x, int y,
                               bool bLeftDown, bool bLeftUp,
                               bool bPressed, bool bDClick,
                              bool bShiftDown, int iDirection); ///< \brief process the stretch selection tool
        void ProcessHotSpot(int x, int y,
                               bool bLeftDown, bool bLeftUp,
                               bool bPressed, bool bDClick,
                              bool bShiftDown); ///< \brief process the hot spot tool
        void ProcessGradient(int x, int y,
                             bool bLeftDown, bool bLeftUp,
                             bool bPressed, bool bDClick,
                             bool bShiftDown); ///< \brief process the hot spot tool
        void ProcessSprayCan(int x, int y,
                             bool bLeftDown, bool bLeftUp,
                             bool bPressed, bool bDClick,
                             bool bShiftDown); ///< \brief process the hot spot tool

        void UpdateAUIColours(void);   ///< \brief Get the same colours as codeblocks configuration
        void UpdateMinimalSizes(void); ///< \brief set minimal sizes for the AUI Panes

    private:
        //debugging function
        void LogToFile(wxString sLogText, wxString sFilePath); ///< \brief Debugging function : writes a string to a text file

        //child panels and widgets
        XPMToolPanel *ToolPanel;    ///< \brief The panel containing all the tools
        XPMHelpPanel *HelpPanel;    ///< \brief The panel containing some help info
        XPMImagePropertiesPanel *PropertiesPanel;  ///< \brief The panel containing the image properties (size, compression, ...)
        XPMImageManipulationPanel *ImageManipulationPanel;  ///< \brief The panel containing image manipulation tools (invert colours, rotate, ...)

		wxScrolledWindow* DrawCanvas;          ///< \brief A scrolling window containing the image to edit
		wxTextCtrl* TextEdit;                  ///< \brief A Text Editor belonging to DrawCanvas, for adding text to image
		wxResizeCtrl *ResizeCtrl1;             ///< \brief A widget belonging to DrawCanvas, allowing to resize the text editor
		wxStaticText *sCursorPos;              ///< \brief A text belonging to DrawCanvas, displaying cursor position information

        //bitmap, images methods
        wxColour cMaskColour;           ///< \brief the current mask colour
        wxBitmap *m_Bitmap;             ///< \brief the temporary bitmap, used for drawing
        wxImage *m_Image;               ///< \brief the temporary image, used for misc functions
        wxBitmapType m_ImageFormat;     ///< \brief the format of the image file (JPEG, BMP, XPM, ...)
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
        wxColour cGridColour; ///< \brief the grid colour
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
        ToolData tdata;          ///< \brief tool specific data
        bool bUsingTool;         ///< \brief true if a tool is currently in use
        void TransformToSquare(int *x1, int *y1,
                               int *x2, int *y2); ///< \brief make a square from a rectangle
        void MakeStandardOrientation(int *x1, int *y1,
                                     int *x2, int *y2); ///< \brief ensure a line is vertical or horizontal
        void SnapRectToGrid(int *x1, int *y1,
                            int *x2, int *y2, bool bInvert = true); ///< \brief snap coordinates to grid
        double Round(double d);          ///< \brief Round off a decimal value to an integer value
        void SnapToGrid(int *x, int *y, bool bUp); ///< \brief snap coordinates to grid
        wxBitmap m_bmDrawBitmap;  ///< \brief the bitmap which will be used to draw the tool effect
        bool m_bDrawToolDynamic;  ///< \brief true if m_bmDrawBitmap must be blited during paint

        //hotspot position
        int iHotSpotX;              ///< \brief HotSpot X coordinate. A negative value means there are no HotSpot
        int iHotSpotY;              ///< \brief HotSpot Y coordinate. A negative value means there are no HotSpot
        wxColour cHotSpotColour;    ///< \brief HotSpot colour

        //standard configuration
        int iXPMDefaultWidth;  ///< \brief the default width of a new XPM
        int iXPMDefaultHeight; ///< \brief the default height of a new XPM
        wxColour cBackgroundColour; ///< \brief Background colour for the canvas

        //interpolation of coordinates
        wxArrayInt m_pt_x;   ///< \brief array of interpolated points - X coordinates
        wxArrayInt m_pt_y;   ///< \brief array of interpolated points - X coordinates
        void Interpolate(int xStart, int yStart, int xEnd, int yEnd); ///< \brief interpolate

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
    XPM_ID_HOTSPOT_TOOL,
    XPM_ID_SPRAYCAN_TOOL,
    XPM_ID_GRADIENT_TOOL,
    XPM_ID_DRAG_TOOL,
    XPM_ID_STRETCH_TOOL
};

#endif
