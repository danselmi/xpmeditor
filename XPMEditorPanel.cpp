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

//(*InternalHeaders(XPMEditorPanel)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include "XPMColorPicker.h"
#include <wx/spinctrl.h>
#include <wx/settings.h>
#include <wx/grid.h>
#include <wx/intl.h>
#include "XPMToggleBMPButton.h"
#include <wx/string.h>
#include <wx/combobox.h>
//*)

#include "wxGridBitmapRenderer.h"

//button icons
#include "xpm/brush.xpm"
#include "xpm/curve.xpm"
#include "xpm/ellipse.xpm"
#include "xpm/eraser.xpm"
#include "xpm/fill.xpm"
#include "xpm/lasso.xpm"
#include "xpm/line.xpm"
#include "xpm/pen.xpm"
#include "xpm/pipette.xpm"
#include "xpm/polygon.xpm"
#include "xpm/rectangle.xpm"
#include "xpm/roundedrect.xpm"
#include "xpm/selection.xpm"
#include "xpm/text.xpm"

//cursors
#include "xpm/cross_cursor.xpm"
#include "xpm/pipette_cursor.xpm"
#include "xpm/fill_cursor.xpm"
#include "xpm/pen_cursor.xpm"

//additional tools icons
#include "xpm/circle.xpm"
#include "xpm/square.xpm"
#include "xpm/left_hair.xpm"
#include "xpm/right_hair.xpm"

//----------- INIITIALISATION & DECLARATIONS ------------------------------

//(*IdInit(XPMEditorPanel)
const long XPMEditorPanel::ID_STATICTEXT1 = wxNewId();
const long XPMEditorPanel::ID_COMBOBOX1 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT2 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL1 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT3 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL2 = wxNewId();
const long XPMEditorPanel::ID_CHECKBOX1 = wxNewId();
const long XPMEditorPanel::ID_CUSTOM1 = wxNewId();
const long XPMEditorPanel::ID_SELECT_BUTN = wxNewId();
const long XPMEditorPanel::ID_LASSO_BTN = wxNewId();
const long XPMEditorPanel::ID_PEN_BTN = wxNewId();
const long XPMEditorPanel::ID_BRUSH_BTN = wxNewId();
const long XPMEditorPanel::ID_PIPETTE_BTN = wxNewId();
const long XPMEditorPanel::ID_FILL_BTN = wxNewId();
const long XPMEditorPanel::ID_LINE_BTN = wxNewId();
const long XPMEditorPanel::ID_CURVE_BTN = wxNewId();
const long XPMEditorPanel::ID_ERASER_BTN = wxNewId();
const long XPMEditorPanel::ID_TEXT_BTN = wxNewId();
const long XPMEditorPanel::ID_RECTANGLE_BTN = wxNewId();
const long XPMEditorPanel::ID_POLYGON_BTN = wxNewId();
const long XPMEditorPanel::ID_ELLIPSE_BTN = wxNewId();
const long XPMEditorPanel::ID_ROUNDEDRECT_BTN = wxNewId();
const long XPMEditorPanel::ID_GRID1 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT5 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL3 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT7 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL5 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT6 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL4 = wxNewId();
const long XPMEditorPanel::ID_PANEL1 = wxNewId();
const long XPMEditorPanel::ID_SCROLLEDWINDOW1 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(XPMEditorPanel,wxPanel)
	//(*EventTable(XPMEditorPanel)
	//*)
END_EVENT_TABLE()

//---------------------------- CONSTRUCTORS  ------------------------------

/** Constructor - Create a new Image Editor panel
  * @param parent: the parent window for the panel
  * @param id = wxID_ANY : the window identifier for this panel
  * @param pos = wxDefaultPosition : the desired position of the panel after creation
  * @param size = wxDefaultSize : the desired size of the panel after creation
  */
XPMEditorPanel::XPMEditorPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	BuildContent(parent,id,pos,size);
	m_Bitmap = NULL;
	m_Image = NULL;
	bCanResizeX = false;
	bCanResizeY = false;
	bSizingX = false;
	bSizingY = false;
	OldX = 0; OldY = 0;
	dScale = 1;

	//set the bitmaps on the button
	wxBitmap bBrush(brush_xpm);
	BrushButton->SetLabel(bBrush);

	wxBitmap bCurve(CURVE_xpm);
	CurveButton->SetLabel(bCurve);

	wxBitmap bEllipse(ELLIPSE_xpm);
	EllipseButton->SetLabel(bEllipse);

	wxBitmap bEraser(ERASER_xpm);
	EraserButton->SetLabel(bEraser);

	wxBitmap bFill(FILL_xpm);
	FillButton->SetLabel(bFill);

	wxBitmap bLasso(LASSO_xpm);
	LassoButton->SetLabel(bLasso);

	wxBitmap bLine(LINE_xpm);
	LineButton->SetLabel(bLine);

	wxBitmap bPen(PEN_xpm);
	PenButton->SetLabel(bPen);

	wxBitmap bPipette(PIPETTE_xpm);
	PipetteButton->SetLabel(bPipette);

	wxBitmap bPolygon(POLYGON_xpm);
	PolygonButton->SetLabel(bPolygon);

	wxBitmap bRectangle(RECT_xpm);
	RectangleButton->SetLabel(bRectangle);

	wxBitmap bRoundedRectangle(RRECT_xpm);
	RRectButton->SetLabel(bRoundedRectangle);

	wxBitmap bSelection(SEL_xpm);
	SelectButton->SetLabel(bSelection);

	wxBitmap bText(TEXT_xpm);
    TextButton->SetLabel(bText);

	//selection initialisation
	NbPointsMax = 100;
    NbPoints = 0;
    pSelection = (wxPoint*) malloc(NbPointsMax * sizeof(wxPoint));
    bDrawSelection = true;

    //Undo and Redo buffer
    m_undo_buffer = new XPMUndo;
    if (m_undo_buffer) m_undo_buffer->SetParentPanel(this);

    Grid1->SetColLabelSize(0); //hide col labels
    Grid1->SetRowLabelSize(0); //hide row labels
    Grid1->SetDefaultColSize(10);
    Grid1->SetDefaultRenderer(new wxGridBitmapRenderer);
    Grid1->SetCellRenderer(0,0, new wxGridBitmapRenderer);
    Grid1->SetCellRenderer(0,1, new wxGridBitmapRenderer);
    Grid1->SetCellRenderer(1,0, new wxGridBitmapRenderer);
    Grid1->SetCellRenderer(1,1, new wxGridBitmapRenderer);
    Grid1->Hide();
    StaticText4->Hide();
    SpinCtrl1->Hide();
    StaticText5->Hide();
    SpinCtrl2->Hide();
    StaticText6->Hide();
    SpinCtrl3->Hide();
    ToolPanelSizer->Layout();
    ToolPanelSizer->FitInside(ToolPanel);

    UpdateConfiguration();
}

/** Create all the widgets in the panel. Called by the constructor
  * @param parent: the parent window for the panel
  * @param id = wxID_ANY : the window identifier for this panel
  * @param pos = wxDefaultPosition : the desired position of the panel after creation
  * @param size = wxDefaultSize : the desired size of the panel after creation
  */
void XPMEditorPanel::BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(XPMEditorPanel)
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer9;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("wxID_ANY"));
	PanelSizer = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("ZOOM:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ZoomFactor = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
	ZoomFactor->Append(_("10%"));
	ZoomFactor->Append(_("25%"));
	ZoomFactor->Append(_("50%"));
	ZoomFactor->Append(_("75%"));
	ZoomFactor->Append(_("100%"));
	ZoomFactor->Append(_("125%"));
	ZoomFactor->Append(_("150%"));
	ZoomFactor->Append(_("175%"));
	ZoomFactor->Append(_("200%"));
	ZoomFactor->Append(_("400%"));
	ZoomFactor->Append(_("800%"));
	ZoomFactor->Append(_("1600%"));
	ZoomFactor->Append(_("Custom"));
	BoxSizer2->Add(ZoomFactor, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("SIZE:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer2->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BMPWidth = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxSize(71,21), 0, 0, 10000, 0, _T("ID_SPINCTRL1"));
	BMPWidth->SetValue(_T("0"));
	BoxSizer2->Add(BMPWidth, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("x"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer2->Add(StaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BMPHeight = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxSize(71,21), 0, 0, 10000, 0, _T("ID_SPINCTRL2"));
	BMPHeight->SetValue(_T("0"));
	BoxSizer2->Add(BMPHeight, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	CheckBox1 = new wxCheckBox(this, ID_CHECKBOX1, _("Show Grid"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox1->SetValue(false);
	CheckBox1->Disable();
	BoxSizer2->Add(CheckBox1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	PanelSizer->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	ColourPicker = new XPMColorPicker(this,ID_CUSTOM1,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_CUSTOM1"));
	PanelSizer->Add(ColourPicker, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ToolSizer = new wxBoxSizer(wxVERTICAL);
	ToolPanel = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	ToolPanelSizer = new wxBoxSizer(wxVERTICAL);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	SelectButton = new XPMToggleBMPButton(ToolPanel,ID_SELECT_BUTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_SELECT_BUTN"));
	BoxSizer6->Add(SelectButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	LassoButton = new XPMToggleBMPButton(ToolPanel,ID_LASSO_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_LASSO_BTN"));
	BoxSizer6->Add(LassoButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer6, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	PenButton = new XPMToggleBMPButton(ToolPanel,ID_PEN_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_PEN_BTN"));
	BoxSizer5->Add(PenButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	BrushButton = new XPMToggleBMPButton(ToolPanel,ID_BRUSH_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_BRUSH_BTN"));
	BoxSizer5->Add(BrushButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	PipetteButton = new XPMToggleBMPButton(ToolPanel,ID_PIPETTE_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_PIPETTE_BTN"));
	BoxSizer7->Add(PipetteButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	FillButton = new XPMToggleBMPButton(ToolPanel,ID_FILL_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_FILL_BTN"));
	BoxSizer7->Add(FillButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer7, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	LineButton = new XPMToggleBMPButton(ToolPanel,ID_LINE_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_LINE_BTN"));
	BoxSizer10->Add(LineButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	CurveButton = new XPMToggleBMPButton(ToolPanel,ID_CURVE_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_CURVE_BTN"));
	BoxSizer10->Add(CurveButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer10, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	EraserButton = new XPMToggleBMPButton(ToolPanel,ID_ERASER_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_ERASER_BTN"));
	BoxSizer9->Add(EraserButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	TextButton = new XPMToggleBMPButton(ToolPanel,ID_TEXT_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_TEXT_BTN"));
	BoxSizer9->Add(TextButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer9, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	RectangleButton = new XPMToggleBMPButton(ToolPanel,ID_RECTANGLE_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_RECTANGLE_BTN"));
	BoxSizer11->Add(RectangleButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	PolygonButton = new XPMToggleBMPButton(ToolPanel,ID_POLYGON_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_POLYGON_BTN"));
	BoxSizer11->Add(PolygonButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer11, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	EllipseButton = new XPMToggleBMPButton(ToolPanel,ID_ELLIPSE_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_ELLIPSE_BTN"));
	BoxSizer8->Add(EllipseButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	RRectButton = new XPMToggleBMPButton(ToolPanel,ID_ROUNDEDRECT_BTN, wxNullBitmap,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_ROUNDEDRECT_BTN"));
	BoxSizer8->Add(RRectButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	ToolPanelSizer->Add(BoxSizer8, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	Grid1 = new wxGrid(ToolPanel, ID_GRID1, wxDefaultPosition, wxSize(16,100), wxSUNKEN_BORDER, _T("ID_GRID1"));
	Grid1->CreateGrid(2,2);
	Grid1->SetMinSize(wxSize(16,100));
	Grid1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	Grid1->EnableEditing(false);
	Grid1->EnableGridLines(false);
	Grid1->SetDefaultCellFont( Grid1->GetFont() );
	Grid1->SetDefaultCellTextColour( Grid1->GetForegroundColour() );
	ToolPanelSizer->Add(Grid1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText4 = new wxStaticText(ToolPanel, ID_STATICTEXT5, _("Size:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	ToolPanelSizer->Add(StaticText4, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SpinCtrl1 = new wxSpinCtrl(ToolPanel, ID_SPINCTRL3, _T("2"), wxDefaultPosition, wxSize(20,21), 0, 2, 16, 2, _T("ID_SPINCTRL3"));
	SpinCtrl1->SetValue(_T("2"));
	ToolPanelSizer->Add(SpinCtrl1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText6 = new wxStaticText(ToolPanel, ID_STATICTEXT7, _("Radius:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	ToolPanelSizer->Add(StaticText6, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SpinCtrl3 = new wxSpinCtrl(ToolPanel, ID_SPINCTRL5, _T("1"), wxDefaultPosition, wxSize(20,21), 0, 1, 16, 1, _T("ID_SPINCTRL5"));
	SpinCtrl3->SetValue(_T("1"));
	ToolPanelSizer->Add(SpinCtrl3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText5 = new wxStaticText(ToolPanel, ID_STATICTEXT6, _("Thickness:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	ToolPanelSizer->Add(StaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SpinCtrl2 = new wxSpinCtrl(ToolPanel, ID_SPINCTRL4, _T("1"), wxDefaultPosition, wxSize(20,21), 0, 1, 16, 1, _T("ID_SPINCTRL4"));
	SpinCtrl2->SetValue(_T("1"));
	ToolPanelSizer->Add(SpinCtrl2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	ToolPanelSizer->Add(62,-1,1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	ToolPanel->SetSizer(ToolPanelSizer);
	ToolPanelSizer->Fit(ToolPanel);
	ToolPanelSizer->SetSizeHints(ToolPanel);
	ToolSizer->Add(ToolPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer3->Add(ToolSizer, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	CanvasSizer = new wxBoxSizer(wxVERTICAL);
	DrawCanvas = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxSUNKEN_BORDER, _T("ID_SCROLLEDWINDOW1"));
	DrawCanvas->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	CanvasSizer->Add(DrawCanvas, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	sCursorPos = new wxStaticText(this, ID_STATICTEXT4, _("Cursor at: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	CanvasSizer->Add(sCursorPos, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3->Add(CanvasSizer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	PanelSizer->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(PanelSizer);
	PanelSizer->Fit(this);
	PanelSizer->SetSizeHints(this);

	Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnZoomChanged);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnBitmapSizeChanged);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnBitmapSizeChanged);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnShowGrid);
	Connect(ID_GRID1,wxEVT_GRID_CELL_LEFT_CLICK,(wxObjectEventFunction)&XPMEditorPanel::OnBrushStyleChanged);
	Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinSizeChanged);
	Connect(ID_SPINCTRL5,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinRadiusChanged);
	Connect(ID_SPINCTRL4,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinSizeChanged);
	DrawCanvas->Connect(wxEVT_PAINT,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasPaint,0,this);
	DrawCanvas->Connect(wxEVT_ERASE_BACKGROUND,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasEraseBackground,0,this);
	DrawCanvas->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftDown,0,this);
	DrawCanvas->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftUp,0,this);
	DrawCanvas->Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftDClick,0,this);
	DrawCanvas->Connect(wxEVT_MOTION,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasMouseMove,0,this);
	DrawCanvas->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasMouseLeave,0,this);
	DrawCanvas->Connect(wxEVT_SIZE,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasResize,0,this);
	//*)

    //Tool box events
	Connect(ID_SELECT_BUTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleSelectionTool);
	Connect(ID_LASSO_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleLassoTool);
	Connect(ID_BRUSH_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleBrushTool);
	Connect(ID_LINE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleLineTool);
	Connect(ID_CURVE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleCurveTool);
	Connect(ID_PEN_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnTogglePenTool);
	Connect(ID_ELLIPSE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleEllipseTool);
	Connect(ID_RECTANGLE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleRectangleTool);
	Connect(ID_ROUNDEDRECT_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleRoundedRectTool);
	Connect(ID_PIPETTE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnTogglePipetteTool);
	Connect(ID_POLYGON_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnTogglePolygonTool);
	Connect(ID_FILL_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleFillTool);
	Connect(ID_ERASER_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleEraserTool);
	Connect(ID_TEXT_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnToggleTextTool);

	//transparent color changed
	Connect(ID_CUSTOM1,wxEVT_TRANSPARENT_COLOR_CHANGED,(wxObjectEventFunction)&XPMEditorPanel::OnTransparentColorChanged);

    //tools IDs
	tools[XPM_ID_SELECT_TOOL] = SelectButton;
	tools[XPM_ID_LASSO_TOOL] = LassoButton;
	tools[XPM_ID_PEN_TOOL] = PenButton;
	tools[XPM_ID_BRUSH_TOOL] = BrushButton;
    tools[XPM_ID_PIPETTE_TOOL] = PipetteButton;
    tools[XPM_ID_FILL_TOOL] = FillButton;
    tools[XPM_ID_CURVE_TOOL] = CurveButton;
    tools[XPM_ID_LINE_TOOL] = LineButton;
    tools[XPM_ID_ERASER_TOOL] = EraserButton;
    tools[XPM_ID_TEXT_TOOL] = TextButton;
    tools[XPM_ID_RECTANGLE_TOOL]= RectangleButton;
    tools[XPM_ID_POLYGON_TOOL]= PolygonButton;
    tools[XPM_ID_ELLIPSE_TOOL]= EllipseButton;
    tools[XPM_ID_ROUNDEDRECT_TOOL] = RRectButton;

    //tools cursors
    wxImage ImgCrossCursor(cross_cursor_xpm);
    ImgCrossCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
    ImgCrossCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
    wxImage ImgPenCursor(pen_cursor_xpm);
    ImgPenCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 14);
    ImgPenCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 25);
    wxImage ImgPipetteCursor(pipette_cursor_xpm);
    ImgPipetteCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 8);
    ImgPipetteCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 23);
    wxImage ImgFillCursor(fill_cursor_xpm);
    ImgFillCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 8);
    ImgFillCursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 22);


    ToolCursor[XPM_ID_SELECT_TOOL] = wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_LASSO_TOOL] = wxCursor(ImgCrossCursor);
	ToolCursor[XPM_ID_PEN_TOOL] = wxCursor(ImgPenCursor);
	ToolCursor[XPM_ID_BRUSH_TOOL] = wxCursor(wxCURSOR_PAINT_BRUSH);
    ToolCursor[XPM_ID_PIPETTE_TOOL] = wxCursor(ImgPipetteCursor);
    ToolCursor[XPM_ID_FILL_TOOL] = wxCursor(ImgFillCursor);
    ToolCursor[XPM_ID_CURVE_TOOL] = wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_LINE_TOOL] = wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_ERASER_TOOL] = wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_TEXT_TOOL] = wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_RECTANGLE_TOOL]= wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_POLYGON_TOOL]= wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_ELLIPSE_TOOL]= wxCursor(ImgCrossCursor);
    ToolCursor[XPM_ID_ROUNDEDRECT_TOOL] = wxCursor(ImgCrossCursor);

    ToggleButtons(-1, false); //Toggle All buttons off.

	dScale = 1;
	bShowGrid = false;
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

    //free UNDO buffer
    if (m_undo_buffer) delete m_undo_buffer;
}

//--------------------- MODIFICATION FLAGS METHODS  -----------------------

/** Update the modification flag
  * @param bModified : the modification flag
  */
void XPMEditorPanel::SetModified(bool bModified)
{
    XPMEditorBase *editor;
    editor = (XPMEditorBase *) GetParent();
    if (editor) editor->SetModified(bModified);
}

/** Get the modification flag
  * @return : the modification flag
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
  * @return a pointer to the wxBitmap used internally by the editor
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

/** Set the scaled internal bitmap.
  * @param bm: a pointer to the wxBitmap to be used internally by the editor
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
        if (BMPWidth) BMPWidth->SetValue(m_Bitmap->GetWidth());
        if (BMPHeight) BMPHeight->SetValue(m_Bitmap->GetHeight());
        if (ZoomFactor) ZoomFactor->SetValue(_("100%"));

        DoSetScrollBars();

        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }
}

/** Return an image representing the selection
  */
wxImage XPMEditorPanel::GetImageFromSelection(void)
{
    //Return an image representing the selection
    if (!m_Image) return(wxImage());

    wxImage imgCopy(*m_Image);

    //get the region containing the selection
    wxRegion region(NbPoints, pSelection);

    //convert the region to a mask
    wxBitmap bmMask = region.ConvertToBitmap();
    wxImage imgMask = bmMask.ConvertToImage();

    wxRect r;
    r = region.GetBox();

    imgCopy = imgCopy.GetSubImage(r);
    imgMask = imgMask.GetSubImage(r);
    imgCopy.SetMask(true);
    imgCopy.SetMaskFromImage(imgMask,0,0,0);

    return(imgCopy);
}

/** Return the current associated image
  * All changes buffered are flushed first
  * @return a pointer to the wxImage stored.
  *         The caller must NOT delete the wxImage: it is still owned by the panel,
  *         and it will be deleted by the destructor.
  *         This is not a copy of the wxImage
  */
wxImage* XPMEditorPanel::GetImage(void)
{
    //return the associated image
    UpdateImage();
    return(m_Image);
}

/** Set the image to be edited
  * the previous one is discarded and deleted
  * Scale factor is kept.
  * @param : a pointer to the new wxImage.
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
        }
        else
        {
            //create a blank bitmap
            m_Image = new wxImage(iXPMDefaultWidth, iXPMDefaultHeight, true);
        }
        if (!m_Image) return;

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

/** recreate the m_Bitmap member from the m_Image member
  * The Draw Canvas is not updated. It must be done manually by the caller
  * The scrollbars are also not updated
  */
void XPMEditorPanel::FastUpdateBitmap(void)
{
    //recreate the m_Bitmap member from the m_Image member
    if (m_Image)
    {
        wxBitmap *bm;
        bm = new wxBitmap(*m_Image);
        if (!bm) return;

        wxBitmap *m_temp;
        m_temp = m_Bitmap;
        m_Bitmap = bm;
        if (m_temp) delete m_temp;
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
    @return a pointer to the wxScrolledWindow on which the Bitmap is drawn on success, NULL otherwise
  */
wxScrolledWindow* XPMEditorPanel::GetDrawCanvas(void)
{
    //get the DrawCanvas
    return(DrawCanvas);
}

//--------------------------- PAINT HANDLERS  -----------------------------
/** To avoid redrawing the window background.
  * It is taken care of it manually in OnDrawCanvasPaint
  */
void XPMEditorPanel::OnDrawCanvasEraseBackground(wxEraseEvent& event)
{

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


    //create the transparent background, and draw the bitmap on it
    wxColour cTransparent;
    if (ColourPicker) cTransparent = ColourPicker->GetTransparentColour(); else cTransparent = *wxWHITE;

    wxBrush bTransparent(cTransparent, wxSOLID);
    wxPen bTransparentPen(cTransparent, 1, wxSOLID);
    dc.SetBrush(bTransparent);
    dc.SetPen(bTransparentPen);
    if ((m_Bitmap) && (dScale > 0))
    {
        dc.DrawRectangle(0,0,m_Bitmap->GetWidth() * dScale, m_Bitmap->GetHeight() * dScale);

        wxMemoryDC memDC;
        memDC.SelectObject(*m_Bitmap);
        memDC.SetUserScale(1 / dScale, 1 / dScale);
        dc.Blit(0,0,m_Bitmap->GetWidth() * dScale, m_Bitmap->GetHeight() * dScale,&memDC,0,0, wxCOPY, true);
    }


    //draw selection
    if ((pSelection) && (NbPoints > 1) && (dScale > 0) &&(bDrawSelection))
    {
        int iPenWidth;

        if (dScale < 1) iPenWidth = 1; else iPenWidth = dScale;

        wxPen pSelectionPen(*wxBLUE, iPenWidth, wxSHORT_DASH);
        dc.SetPen(pSelectionPen);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);

        wxPoint *tmp;
        tmp = new wxPoint[NbPoints];
        if (tmp)
        {
            int i;
            for(i=0;i<NbPoints;i++)
            {
                tmp[i].x = pSelection[i].x * dScale;
                tmp[i].y = pSelection[i].y * dScale;
            }
            dc.DrawPolygon(NbPoints, tmp);
            delete[] tmp;
        }

    }

    //draw the grid
    if ((dScale >= 4.0) && (bShowGrid) &&(m_Bitmap))
    {
        //draw the grid
        int i, iMax, j, jMax;
        iMax = m_Bitmap->GetWidth();
        jMax = m_Bitmap->GetHeight();
        wxPen pen(*wxBLACK, 1);
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
void XPMEditorPanel::OnBitmapSizeChanged(wxSpinEvent& event)
{
    int iWidth, iHeight;

    if (BMPWidth) iWidth = BMPWidth->GetValue();
    if (BMPHeight) iHeight = BMPHeight->GetValue();
    if (m_undo_buffer) m_undo_buffer->AddUndo(xpm_size_undo, GetImage());
    SetDrawAreaSize(wxSize(iWidth, iHeight));
}

/** The user has changed the Zoom factor in the ComboBox "Zoom"
  */
void XPMEditorPanel::OnZoomChanged(wxCommandEvent& event)
{
    wxString s;
    double dScale2;

    s = ZoomFactor->GetValue();
    s.Replace(_("%"), _(""));
    if (s.ToDouble(&dScale2))
    {
        if (dScale2 < 0) dScale2 = - dScale2;
        if (dScale2 == 0.0) return;
        dScale2 = dScale2 / 100;
        SetScaleFactor(dScale2);
    }
    else if (s.Find(_("Custom")) >= 0)
    {
        XPMCustomZoom cz(this);
        long lValue;
        lValue = dScale * 100;
        wxString sCurrentZoom = wxString::Format(_("%d"), lValue);
        cz.TextCtrl1->SetValue(sCurrentZoom);
        if (cz.ShowModal())
        {
            ZoomFactor->SetValue(wxString::Format(_("%d%%"), dScale * 100));
        }
    }

}

/** Return the current applied scaling factor (== zoom factor)
  * a factor of 1 indicate no zooming (100%)
  * @return the scale factor (double)
  */
double XPMEditorPanel::GetScaleFactor(void)
{
    //get scaling factor
    return(dScale);
}

/** Set the new  scaling factor (== zoom factor)
  * a factor of 1 indicate no zooming (100%)
  * @param dNewScalingFactor : the new scale factor (double)
  */
void XPMEditorPanel::SetScaleFactor(double dNewScalingFactor)
{
    //Set Scaling Factor
    dScale = dNewScalingFactor;
    wxString s = wxString::Format(_("%d%%"), dScale * 100);
    ZoomFactor->SetValue(s);
    DoSetScrollBars();

    if (dScale >= 4.0) CheckBox1->Enable(true); else CheckBox1->Disable();
    //UpdateBitmap(); //rescale the memory bitmap

    if (DrawCanvas)
    {
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }
}

/** Return the size of the current draw area (== the size of the bitmap), in pixel
  * @return the current draw area, in pixel
  */
wxSize XPMEditorPanel::GetDrawAreaSize(void)
{
    //Get Size
    return(sDrawAreaSize);
}

/** Set the size of the draw area (== the size of the bitmap), in pixel
  * @param sNewDrawAreaSize the new draw area, in pixel
  */
void XPMEditorPanel::SetDrawAreaSize(wxSize sNewDrawAreaSize)
{
    //Set Size
    if (DrawCanvas)
    {
        sDrawAreaSize = sNewDrawAreaSize;

        //indicate the size of the bitmap in the spinboxes
        if (BMPWidth) BMPWidth->SetValue(sDrawAreaSize.GetWidth());
        if (BMPHeight) BMPHeight->SetValue(sDrawAreaSize.GetHeight());

        //resize the image
        if (m_Image)
        {
            m_Image->Resize(sDrawAreaSize, wxPoint(0,0),
                            m_Image->GetMaskRed(), m_Image->GetMaskGreen(), m_Image->GetMaskBlue()
                           );
        }
        UpdateBitmap();

        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
        SetModified(true);
        DoSetScrollBars();
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

void XPMEditorPanel::OnShowGrid(wxCommandEvent& event)
{
    bShowGrid = CheckBox1->IsChecked();
    if (DrawCanvas)
    {
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }
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

    if ((event.Dragging()) && (bSizingX || bSizingY))
    {
        //draw a dynamic resize border
        int x, y, xx, yy;

        //draw the rectangle
        wxClientDC dc(DrawCanvas);
        DrawCanvas->DoPrepareDC(dc);
        dc.SetUserScale(1,1);
        wxPen pGrayPen(*wxBLACK, 1,wxSOLID);
        wxColour cBackCol;
        cBackCol = DrawCanvas->GetBackgroundColour();
        wxPen pBackPen(cBackCol, 1,wxSOLID);

        //Get mouse position
        if (bSizingX) x = event.m_x; else x = m_Bitmap->GetWidth() * dScale;
        if (bSizingY) y = event.m_y; else y = m_Bitmap->GetHeight() * dScale;

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
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();

        //Manager::Get()->GetLogManager()->DebugLog(wxString::Format(_("x = %d y = %d"), xx, yy));
        if (BMPWidth) BMPWidth->SetValue(xx);
        if (BMPHeight) BMPHeight->SetValue(yy);
        dc.SetPen(pGrayPen);
        //xx = xx / dScale;
        //yy = yy / dScale;
        dc.DrawLine(xx * dScale, 0, xx * dScale , yy * dScale);
        dc.DrawLine(0,yy * dScale, xx * dScale, yy * dScale);
        OldX = xx * dScale ; OldY = yy * dScale;
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
        else if ((x >= 0) && (y >= 0))
        {
            bCanResizeX = false;
            bCanResizeY = false;
            SetToolCursor();
        }
        else
        {
            bCanResizeX = false;
            bCanResizeY = false;
            SetToolCursor();
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

    DrawCanvas->CalcUnscrolledPosition(event.m_x, event.m_y, &x, &y);
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

    ProcessToolAction(iTool, x, y, false, false, false, true);
    event.Skip();
}

/** draw the tool action: select box for Selection tool, rectangle for rectangle tool, ...
  * @param iTool: the index of the tool used
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessToolAction(int iTool, int x, int y,
                                         bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    //draw the tool action: select box for Selection tool, rectangle for rectangle tool, ...

    switch(iTool)
    {
        case XPM_ID_SELECT_TOOL :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            tdata.x2 = x;
                            tdata.y2 = y;
                            DrawCanvas->Refresh(false,NULL);
                            DrawCanvas->Update();

                            int iPenWidth;
                            if (dScale < 1) iPenWidth = 1; else iPenWidth = dScale;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);
                            wxPen pSelectionPen(*wxBLUE, iPenWidth, wxSHORT_DASH);
                            dc.SetPen(pSelectionPen);
                            dc.SetBrush(*wxTRANSPARENT_BRUSH);
                            dc.DrawRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale);
                        }
                    }

                    //left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            ClearSelection();
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                            tdata.iNbClicks = 1;
                            tdata.x1 = x  / dScale;
                            tdata.y1 = y / dScale;
                        }
                        else
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
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                            InitToolData();
                        }
                    }

                    break;

        case XPM_ID_LASSO_TOOL  :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            bDrawSelection = false;
                            tdata.x2 = x;
                            tdata.y2 = y;
                            DrawCanvas->Refresh(false,NULL);
                            DrawCanvas->Update();
                            bDrawSelection = true;

                            int iPenWidth;
                            if (dScale < 1) iPenWidth = 1; else iPenWidth = dScale;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);
                            wxPen pSelectionPen(*wxBLUE, iPenWidth, wxSHORT_DASH);
                            dc.SetPen(pSelectionPen);
                            dc.SetBrush(*wxTRANSPARENT_BRUSH);

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
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                            tdata.iNbClicks = 1;
                            tdata.x1 = x  / dScale;
                            tdata.y1 = y / dScale;
                            NbPoints = 1;
                            pSelection = CheckMemorySelection(1);
                            if (pSelection)
                            {
                                pSelection[0].x = x;
                                pSelection[0].y = y;
                            }
                        }
                        else
                        {
                            NbPoints = NbPoints + 1;
                            pSelection = CheckMemorySelection(NbPoints);
                            if (pSelection)
                            {
                                pSelection[NbPoints - 1].x = x;
                                pSelection[NbPoints - 1].y = y;
                            }
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                        }
                    }

                    if (bDClick)
                    {
                        ToggleButtons(-1, false);
                    }

                    break;

        case XPM_ID_PEN_TOOL :
                    if (bLeftDown)
                    {
                        //Undo & modification flag
                        AddUndo();
                        SetModified(true);

                        if ((m_Image) && (DrawCanvas))
                        {
                            wxColour cColour;
                            cColour = ColourPicker->GetLineColour();
                            int xx, yy;
                            xx = x / dScale; yy = y / dScale;
                            wxRect r(xx, yy, 1, 1);

                            m_Image->SetRGB(r,
                                            cColour.Red(), cColour.Green(), cColour.Blue());
                            FastUpdateBitmap();
/*
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);
                            wxPen pen(cColour, 1, wxSOLID);
                            wxBrush brush(cColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);
                            dc.DrawRectangle(x,y, dScale, dScale);
*/
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                        }
                    }
                    if (bPressed)
                    {
                        //draw the pixel
                        if ((m_Image) && (DrawCanvas))
                        {
                            wxColour cColour;
                            cColour = ColourPicker->GetLineColour();
                            int xx, yy;
                            xx = x / dScale; yy = y / dScale;
                            wxRect r(xx, yy, 1, 1);
                            m_Image->SetRGB(r,
                                            cColour.Red(), cColour.Green(), cColour.Blue());
                            FastUpdateBitmap();
/*
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);
                            wxPen pen(cColour, 1, wxSOLID);
                            wxBrush brush(cColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);
                            dc.DrawRectangle(x,y, dScale, dScale);
*/
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                        }
                    }
                    if (bLeftUp)
                    {
                        //finish
                        InitToolData();
                    }
                    break;

        case XPM_ID_BRUSH_TOOL :
                    if (bLeftDown)
                    {
                        //Undo & modification flag
                        AddUndo();
                        SetModified(true);

                        wxMemoryDC mem_dc(*m_Bitmap);
                        if (mem_dc.IsOk())
                        {
                            wxColour cColour;
                            cColour = ColourPicker->GetLineColour();
                            wxBrush brush(cColour, wxSOLID);
                            wxPen pen(cColour, 1, wxSOLID);
                            mem_dc.SetBrush(brush);
                            mem_dc.SetPen(pen);

                            int xx, yy;
                            xx = x / dScale; yy = y / dScale;

                            switch (tdata.iStyle)
                            {
                                case XPM_BRUSH_STYLE_CIRCLE   :
                                    mem_dc.DrawCircle(xx, yy, tdata.iSize/2);
                                    break;

                                case XPM_BRUSH_STYLE_LEFTHAIR :
                                    mem_dc.DrawLine(xx,yy + tdata.iSize, xx + tdata.iSize, yy);
                                    break;

                                case XPM_BRUSH_STYLE_RIGHTHAIR:
                                    mem_dc.DrawLine(xx,yy, xx + tdata.iSize, yy + tdata.iSize);
                                    break;

                                case XPM_BRUSH_STYLE_SQUARE   :
                                default:
                                    mem_dc.DrawRectangle(xx - tdata.iSize / 2,
                                                         yy - tdata.iSize / 2,
                                                         tdata.iSize,
                                                         tdata.iSize);
                                    break;
                            }
                            mem_dc.SelectObject(wxNullBitmap);
                        }
                        UpdateImage();
                        DrawCanvas->Refresh(false, NULL);
                        DrawCanvas->Update();
                    }
                    if (bPressed)
                    {
                        wxMemoryDC mem_dc(*m_Bitmap);
                        if (mem_dc.IsOk())
                        {
                            wxColour cColour;
                            cColour = ColourPicker->GetLineColour();
                            wxBrush brush(cColour, wxSOLID);
                            wxPen pen(cColour, 1, wxSOLID);
                            mem_dc.SetBrush(brush);
                            mem_dc.SetPen(pen);

                            int xx, yy;
                            xx = x / dScale; yy = y / dScale;

                            switch (tdata.iStyle)
                            {
                                case XPM_BRUSH_STYLE_CIRCLE   :
                                    mem_dc.DrawCircle(xx, yy, tdata.iSize/2);
                                    break;

                                case XPM_BRUSH_STYLE_LEFTHAIR :
                                    mem_dc.DrawLine(xx,yy + tdata.iSize, xx + tdata.iSize, yy);
                                    break;

                                case XPM_BRUSH_STYLE_RIGHTHAIR:
                                    mem_dc.DrawLine(xx,yy, xx + tdata.iSize, yy + tdata.iSize);
                                    break;

                                case XPM_BRUSH_STYLE_SQUARE   :
                                default:
                                    mem_dc.DrawRectangle(xx - tdata.iSize / 2,
                                                         yy - tdata.iSize / 2,
                                                         tdata.iSize,
                                                         tdata.iSize);
                                    break;
                            }
                            mem_dc.SelectObject(wxNullBitmap);
                        }
                        UpdateImage();
                        wxRect r(x / dScale - tdata.iSize / 2, y / dScale - tdata.iSize / 2, tdata.iSize, tdata.iSize);
                        DrawCanvas->Refresh(false, &r);
                        DrawCanvas->Update();
                    }
                    if (bLeftUp)
                    {
                        //finish
                        InitToolData();
                    }

                    break;

        case XPM_ID_PIPETTE_TOOL :
                    if (bLeftDown)
                    {
                        //get the pixel color
                        if (m_Image)
                        {
                            int iColour;
                            unsigned char iRed, iGreen, iBlue;
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
                    break;

        case XPM_ID_FILL_TOOL :
                    if (bLeftDown)
                    {
                        //Undo & modification flag
                        AddUndo();
                        SetModified(true);

                        //draw the pixel
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
                        DrawCanvas->Refresh(false, NULL);
                        DrawCanvas->Update();
                    }
                    if (bLeftUp)
                    {
                        //finish
                        InitToolData();
                    }
                    break;

        case XPM_ID_CURVE_TOOL :

                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            DrawCanvas->Refresh(false,NULL);
                            DrawCanvas->Update();

                            tdata.iNbClicks = tdata.iNbClicks + 1;
                            if (tdata.iNbPoints >= XPM_MAXPOINTS) tdata.iNbPoints = XPM_MAXPOINTS;

                            //Draw the curve
                            int iPenWidth;
                            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);

                            wxColour cLineColour;
                            cLineColour = ColourPicker->GetLineColour();
                            wxPen pen(cLineColour, iPenWidth, wxSOLID);
                            dc.SetPen(pen);

                            wxPoint tmp[XPM_MAXPOINTS+1];
                            int i;
                            tmp[tdata.iNbPoints].x = x;
                            tmp[tdata.iNbPoints].y = y;
                            for(i=0;i<tdata.iNbPoints ;i++)
                            {
                                tmp[i].x = tdata.pts[i].x * dScale;
                                tmp[i].y = tdata.pts[i].y * dScale;
                            }
                            dc.DrawSpline(tdata.iNbPoints + 1, tmp);
                        }
                    }

                    // left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                            tdata.iNbClicks = 1;
                            tdata.iNbPoints = 1;
                            tdata.pts[0].x = x  / dScale;
                            tdata.pts[0].y = y / dScale;
                        }
                        else
                        {
                            tdata.iNbClicks = tdata.iNbClicks + 1;
                            tdata.iNbPoints = tdata.iNbPoints + 1;
                            if (tdata.iNbPoints >= XPM_MAXPOINTS) tdata.iNbPoints = XPM_MAXPOINTS;
                            tdata.pts[tdata.iNbPoints - 1].x = x  / dScale;
                            tdata.pts[tdata.iNbPoints - 1].y = y / dScale;
                        }
                    }

                    if (bDClick)
                    {
                        AddUndo();
                        SetModified(true);

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

                        DrawCanvas->Refresh(false, NULL);
                        DrawCanvas->Update();
                        InitToolData();
                        ToggleButtons(-1,false);
                    }

                    break;

        case XPM_ID_LINE_TOOL :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            tdata.x2 = x;
                            tdata.y2 = y;

                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();

                            int iPenWidth;
                            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);

                            wxColour cLineColour, cFillColour;
                            cLineColour = ColourPicker->GetLineColour();
                            cFillColour = ColourPicker->GetFillColour();

                            wxPen pen(cLineColour, 1, wxSOLID);
                            wxBrush brush(cFillColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);
                            dc.DrawLine(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x, y);

                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);
                        }
                    }

                    //left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            tdata.iNbClicks = 1;
                            tdata.x1 = x  / dScale;
                            tdata.y1 = y / dScale;
                        }
                        else
                        {
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
                                mem_dc.DrawLine(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x, y);

                                mem_dc.SelectObject(wxNullBitmap);
                            }
                            UpdateImage();
                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                            DrawCanvas->Refresh(false, &r);
                            DrawCanvas->Update();
                            InitToolData();
                        }
                    }
                    break;

        case XPM_ID_ERASER_TOOL :
                    if (bLeftDown)
                    {
                        //Undo & modification flag
                        AddUndo();
                        SetModified(true);

                        wxMemoryDC mem_dc(*m_Bitmap);
                        if (mem_dc.IsOk())
                        {
                            wxColour cColour;
                            cColour = ColourPicker->GetLineColour();
                            wxBrush brush(cColour, wxSOLID);
                            wxPen pen(cColour, 1, wxSOLID);
                            mem_dc.SetBrush(brush);
                            mem_dc.SetPen(pen);

                            int xx, yy;
                            xx = x / dScale; yy = y / dScale;


                            mem_dc.DrawRectangle(xx - tdata.iSize / 2,
                                                         yy - tdata.iSize / 2,
                                                         tdata.iSize,
                                                         tdata.iSize);
                            mem_dc.SelectObject(wxNullBitmap);
                        }
                        UpdateImage();
                        DrawCanvas->Refresh(false, NULL);
                        DrawCanvas->Update();
                    }
                    if (bPressed)
                    {
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

                            mem_dc.DrawRectangle(xx - tdata.iSize / 2,
                                                         yy - tdata.iSize / 2,
                                                         tdata.iSize,
                                                         tdata.iSize);
                            mem_dc.SelectObject(wxNullBitmap);
                        }
                        UpdateImage();
                        wxRect r(x / dScale - tdata.iSize / 2, y / dScale - tdata.iSize / 2, tdata.iSize, tdata.iSize);
                        DrawCanvas->Refresh(false, &r);
                        DrawCanvas->Update();
                    }
                    if (bLeftUp)
                    {
                        //finish
                        InitToolData();
                    }
                    break;

        case XPM_ID_TEXT_TOOL :
                    break;

        case XPM_ID_RECTANGLE_TOOL :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            tdata.x2 = x;
                            tdata.y2 = y;

                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();

                            int iPenWidth;
                            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);

                            wxColour cLineColour, cFillColour;
                            cLineColour = ColourPicker->GetLineColour();
                            cFillColour = ColourPicker->GetFillColour();

                            wxPen pen(cLineColour, 1, wxSOLID);
                            wxBrush brush(cFillColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);
                            dc.DrawRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);
                        }
                    }

                    //left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            tdata.iNbClicks = 1;
                            tdata.x1 = x  / dScale;
                            tdata.y1 = y / dScale;
                        }
                        else
                        {
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
                                mem_dc.DrawRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                                mem_dc.SelectObject(wxNullBitmap);
                            }
                            UpdateImage();
                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                            DrawCanvas->Refresh(false, &r);
                            DrawCanvas->Update();
                            InitToolData();
                        }
                    }

                    break;

        case XPM_ID_POLYGON_TOOL :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            DrawCanvas->Refresh(false,NULL);
                            DrawCanvas->Update();

                            tdata.iNbClicks = tdata.iNbClicks + 1;
                            if (tdata.iNbPoints >= XPM_MAXPOINTS) tdata.iNbPoints = XPM_MAXPOINTS;

                            //Draw the polygon
                            int iPenWidth;
                            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);

                            wxColour cLineColour, cFillColour;
                            cLineColour = ColourPicker->GetLineColour();
                            cFillColour = ColourPicker->GetFillColour();
                            wxPen pen(cLineColour, iPenWidth, wxSOLID);
                            wxBrush brush(cFillColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);

                            wxPoint tmp[XPM_MAXPOINTS+1];
                            int i;
                            tmp[tdata.iNbPoints].x = x;
                            tmp[tdata.iNbPoints].y = y;
                            for(i=0;i<tdata.iNbPoints;i++)
                            {
                                tmp[i].x = tdata.pts[i].x * dScale;
                                tmp[i].y = tdata.pts[i].y * dScale;
                            }
                            dc.DrawPolygon(tdata.iNbPoints + 1, tmp);
                        }
                    }

                    // left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();
                            tdata.iNbClicks = 1;
                            tdata.iNbPoints = 1;
                            tdata.pts[0].x = x  / dScale;
                            tdata.pts[0].y = y / dScale;
                        }
                        else
                        {
                            tdata.iNbClicks = tdata.iNbClicks + 1;
                            tdata.iNbPoints = tdata.iNbPoints + 1;
                            if (tdata.iNbPoints >= XPM_MAXPOINTS) tdata.iNbPoints = XPM_MAXPOINTS;
                            tdata.pts[tdata.iNbPoints - 1].x = x  / dScale;
                            tdata.pts[tdata.iNbPoints - 1].y = y / dScale;
                        }
                    }

                    if (bDClick)
                    {
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
                            mem_dc.DrawPolygon(tdata.iNbPoints, tdata.pts);
                            mem_dc.SelectObject(wxNullBitmap);
                        }
                        UpdateImage();

                        DrawCanvas->Refresh(false, NULL);
                        DrawCanvas->Update();
                        InitToolData();
                        ToggleButtons(-1,false);
                    }

                    break;

        case XPM_ID_ELLIPSE_TOOL :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            tdata.x2 = x;
                            tdata.y2 = y;

                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();

                            int iPenWidth;
                            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);

                            wxColour cLineColour, cFillColour;
                            cLineColour = ColourPicker->GetLineColour();
                            cFillColour = ColourPicker->GetFillColour();

                            wxPen pen(cLineColour, 1, wxSOLID);
                            wxBrush brush(cFillColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);
                            dc.DrawEllipse(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);
                        }
                    }

                    //left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            tdata.iNbClicks = 1;
                            tdata.x1 = x  / dScale;
                            tdata.y1 = y / dScale;
                        }
                        else
                        {
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
                                mem_dc.DrawEllipse(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                                mem_dc.SelectObject(wxNullBitmap);
                            }
                            UpdateImage();
                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                            DrawCanvas->Refresh(false, &r);
                            DrawCanvas->Update();
                            InitToolData();
                        }
                    }
                    break;

        case XPM_ID_ROUNDEDRECT_TOOL :
                    if ((!bLeftDown) && (!bLeftUp) && (!bPressed) && (!bDClick))
                    {
                        if (tdata.iNbClicks > 0)
                        {
                            tdata.x2 = x;
                            tdata.y2 = y;

                            DrawCanvas->Refresh(false, NULL);
                            DrawCanvas->Update();

                            int iPenWidth;
                            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
                            wxClientDC dc(DrawCanvas);
                            DrawCanvas->DoPrepareDC(dc);

                            wxColour cLineColour, cFillColour;
                            cLineColour = ColourPicker->GetLineColour();
                            cFillColour = ColourPicker->GetFillColour();

                            wxPen pen(cLineColour, 1, wxSOLID);
                            wxBrush brush(cFillColour, wxSOLID);
                            dc.SetPen(pen);
                            dc.SetBrush(brush);
                            dc.DrawRoundedRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale, tdata.iRadius);

                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);
                        }
                    }

                    //left button UP
                    if (bLeftUp)
                    {
                        if (tdata.iNbClicks == 0)
                        {
                            tdata.iNbClicks = 1;
                            tdata.x1 = x  / dScale;
                            tdata.y1 = y / dScale;
                        }
                        else
                        {
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
                                mem_dc.DrawRoundedRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
                                             x - tdata.x1 * dScale, y - tdata.y1 * dScale, tdata.iRadius);

                                mem_dc.SelectObject(wxNullBitmap);
                            }
                            UpdateImage();
                            wxRect r(tdata.x1 * dScale, tdata.y1 * dScale,
                                     x - tdata.x1 * dScale, y - tdata.y1 * dScale);

                            DrawCanvas->Refresh(false, &r);
                            DrawCanvas->Update();
                            InitToolData();
                        }
                    }
                    break;

        default: break;
    }

}



/** init tool data for a first use
  */
void XPMEditorPanel::InitToolData(void)
{
    tdata.x1 = -1;
    tdata.y1 = -1;
    tdata.x2 = -1;
    tdata.y2 = -1;
    tdata.iNbClicks = 0;
    tdata.iNbPoints = 0;

    int i;
    for(i=0;i < XPM_MAXPOINTS; i++)
    {
        tdata.pts[i].x = -1;
        tdata.pts[i].y = -1;
    }
}

/** Return the ID of the tool currently in use
  * @param the tool ID, -1 if no tool are currently selected
  */
int XPMEditorPanel::GetToolID(void)
{
    return(iToolUsed);
}

/** Set the ID of the tool currently in use
  * Do not modify the state of the buttons
  * @param the tool ID, -1 if no tool are currently selected
  */
void XPMEditorPanel::SetToolID(int iTool)
{
    iToolUsed = iTool;
}

/** Set the correct cursor, according to the currently selected tool & option
  */
void XPMEditorPanel::SetToolCursor(void)
{
    //Set the correct cursor, according to the currently selected tool & option
    int iTool;
    iTool = GetToolID();

    if ((iTool < 0) || (iTool >= XPM_NUMBER_TOOLS))
    {
        SetCursor(wxCURSOR_ARROW);
    }
    else if (iTool == XPM_ID_BRUSH_TOOL)
    {
        //create the correct cursor
        //1 - create the bitmap
        int iSize;
        iSize = tdata.iSize;
        if (tdata.iStyle == XPM_BRUSH_STYLE_SQUARE) iSize += 2;
        if ((tdata.iStyle == XPM_BRUSH_STYLE_CIRCLE) && (tdata.iSize == 1)) iSize = 2;
        wxBitmap bm(iSize, iSize);

        //2 - draw on the bitmap
        wxMemoryDC mem_dc;
        mem_dc.SelectObject(bm);
        mem_dc.SetBrush(*wxRED_BRUSH);
        mem_dc.SetPen(*wxRED_PEN);

        switch (tdata.iStyle)
        {
            case XPM_BRUSH_STYLE_CIRCLE   :
                mem_dc.DrawRectangle(0,0,tdata.iSize, tdata.iSize);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxWHITE_PEN);
                mem_dc.DrawCircle(tdata.iSize/2, tdata.iSize/2, tdata.iSize/2);
                break;

            case XPM_BRUSH_STYLE_LEFTHAIR :
                mem_dc.DrawRectangle(0,0,tdata.iSize, tdata.iSize);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxBLACK_PEN);
                mem_dc.DrawLine(0,tdata.iSize-1, tdata.iSize-1, 0);
                break;

            case XPM_BRUSH_STYLE_RIGHTHAIR:
                mem_dc.DrawRectangle(0,0,tdata.iSize, tdata.iSize);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxBLACK_PEN);
                mem_dc.DrawLine(0,0, tdata.iSize-1, tdata.iSize-1);
                break;

            case XPM_BRUSH_STYLE_SQUARE   :
            default:
                mem_dc.DrawRectangle(0,0,tdata.iSize+2, tdata.iSize+2);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxWHITE_PEN);
                mem_dc.DrawRectangle(1,1,tdata.iSize, tdata.iSize);
                break;
        }
        mem_dc.SelectObject(wxNullBitmap);

        //3 - convert the wxBitmap to a wxImage, and set the HotSpot
        wxImage img;
        img = bm.ConvertToImage();
        img.SetMaskColour(255,0,0);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, iSize / 2);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, iSize / 2);

        //4 - create and set the cursor
        wxCursor cursor(img);
        SetCursor(cursor);
    }
    else if (iTool == XPM_ID_ERASER_TOOL)
    {
        //create the correct cursor
        //1 - create the bitmap
        int iSize;
        iSize = tdata.iSize;
        wxBitmap bm(iSize+2, iSize+2);

        //2 - draw on the bitmap
        wxMemoryDC mem_dc;
        mem_dc.SelectObject(bm);
        mem_dc.SetBrush(*wxRED_BRUSH);
        mem_dc.SetPen(*wxRED_PEN);

        mem_dc.DrawRectangle(0,0,tdata.iSize+2, tdata.iSize+2);
        mem_dc.SetBrush(*wxBLACK_BRUSH);
        mem_dc.SetPen(*wxWHITE_PEN);
        mem_dc.DrawRectangle(1,1,tdata.iSize, tdata.iSize);
        mem_dc.SelectObject(wxNullBitmap);

        //3 - convert the wxBitmap to a wxImage, and set the HotSpot
        wxImage img;
        img = bm.ConvertToImage();
        img.SetMaskColour(255,0,0);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, iSize / 2);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, iSize / 2);

        //4 - create and set the cursor
        wxCursor cursor(img);
        SetCursor(cursor);
    }
    else
    {
        SetCursor(ToolCursor[iTool]);
    }
}


/** Handler for mouse left button down
  */
void XPMEditorPanel::OnDrawCanvasLeftDown(wxMouseEvent& event)
{
    if ((DrawCanvas) && ((bCanResizeX) || (bCanResizeY)) && (m_Bitmap))
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
        ProcessToolAction(iTool, xx, yy, true, false, true, false);
    }
    event.Skip();
}

/** Handler for mouse left button up
  */
void XPMEditorPanel::OnDrawCanvasLeftUp(wxMouseEvent& event)
{
    if (((bSizingX) || (bSizingY)) && (m_Bitmap))
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
        if (m_undo_buffer) m_undo_buffer->AddUndo(xpm_size_undo, GetImage());
        SetDrawAreaSize(sNewSize);
        Refresh(false,NULL);
        Update();
        return;

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
        ProcessToolAction(iTool, xx, yy, false, true, false, false);
    }
    event.Skip();
}

/** Handler for mouse event: set the cursor to Arrow when leaving the drawing canvas
  */
void XPMEditorPanel::OnDrawCanvasMouseLeave(wxMouseEvent& event)
{
    SetCursor(wxCURSOR_ARROW);
}



//------ SELECTION METHODS --------------------------------------

/** Get the smallest bounding rectangle encompassing the selection
  * @param r the rectangle (wxRect) containing the result
  */
void XPMEditorPanel::GetBoundingRect(wxRect *r)
{
    //Get the bounding rectangle of the selection
    if ((HasSelection()) && (r))
    {
        int i;

        r->SetWidth(0);
        r->SetHeight(0);
        r->SetX(pSelection[0].x);
        r->SetY(pSelection[0].y);
        for(i=1;i<NbPoints;i++)
        {
            if (pSelection[i].x < r->x) r->x = pSelection[i].x;
            if (pSelection[i].y < r->y) r->x = pSelection[i].y;
            if (pSelection[i].x - r->x > r->width) r->width = pSelection[i].x - r->x;
            if (pSelection[i].y - r->y > r->height) r->height = pSelection[i].y - r->y;
        }
    }
}

/** clear the current selection
  * @return no return value
  */
void XPMEditorPanel::ClearSelection(void)
{
    //clear the selection
    if (HasSelection())
    {
        int i;
        NbPoints = 0;
    }
}

/** Return true if there is some Selection, false otherwise
  * @return true if something is selected, false otherwise
  */
bool XPMEditorPanel::HasSelection(void) const
{
    //check if an area is selected
    if ((NbPoints > 0) && (pSelection)) return(true);
    return(false);
}

//---------- CLIPBOARD OPERATIONS -----------------
/** perform a "cut" clipbopard operation
  * @return no return value
  */
void XPMEditorPanel::Cut(void)
{
    //CUT
    if (!HasSelection()) return;
    if (!m_Image) return;
    if (!DrawCanvas) return;

    wxRect r;
    GetBoundingRect(&r);

    if (wxTheClipboard->Open())
    {
        wxImage imgCopy;
        imgCopy = GetImageFromSelection();

        //copy to the clipboard
        wxTheClipboard->SetData(new wxBitmapDataObject(wxBitmap(imgCopy)));
        wxTheClipboard->Close();
        AddUndo();

        //paste a transparent image
        wxImage imgPaste(imgCopy.GetWidth(), imgCopy.GetHeight(), true); //black image
        if (m_Image->HasMask())
        {
            imgPaste.Replace(0,0,0, m_Image->GetMaskRed(), m_Image->GetMaskGreen(), m_Image->GetMaskBlue());
        }
        m_Image->Paste(imgPaste, r.GetLeft(), r.GetTop());
        FastUpdateBitmap();
        SetModified(true);
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }
}

/** perform a "copy" clipbopard operation
  * @return no return value
  */
void XPMEditorPanel::Copy(void)
{
    //COPY
    if (!HasSelection()) return;
    if (!m_Image) return;

    if (wxTheClipboard->Open())
    {
        wxImage imgCopy;
        imgCopy = GetImageFromSelection();

        //copy to the clipboard
        wxTheClipboard->SetData(new wxBitmapDataObject(wxBitmap(imgCopy)));
        wxTheClipboard->Close();
    }
}

/** perform a "paste" clipbopard operation
  * @return no return value
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
            wxMemoryDC mem_dc;
            mem_dc.SelectObject(*original_bitmap);
            mem_dc.DrawBitmap(bm,0,0,true);
            mem_dc.SelectObject(wxNullBitmap);
            UpdateImage();
            SetModified(true);
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
        }
    }
}


//---------------- UNDO & REDO ---------------------------

/** Return true if UNDO can be done, false otherwise
  * @return true if UNDO can be done, false otherwise
  */
bool XPMEditorPanel::CanUndo(void) const
{
    //check if UNDO is possible
    if (m_undo_buffer) return(m_undo_buffer->CanUndo());
    return(false);
}

/** Return true if UNDO can be done, false otherwise
  * @return true if UNDO can be done, false otherwise
  */
bool XPMEditorPanel::CanRedo(void) const
{
    //check if REDO is possible
    if (m_undo_buffer) return(m_undo_buffer->CanRedo());
    return(false);
}

/** Clear the Undo Buffer
  * @return no return value
  */
void XPMEditorPanel::ClearUndoBuffer(void)
{
    //Clear the Undo Buffer
    if (m_undo_buffer) m_undo_buffer->ClearUndoBuffer();
}

/** Clear the Redo Buffer
  * @return no return value
  */
void XPMEditorPanel::ClearRedoBuffer(void)
{
    //Clear the Redo Buffer
    if (m_undo_buffer) m_undo_buffer->ClearRedoBuffer();
}

/** Add 1 Undo operation to the undo buffer
  * @param bm a pointer to the bitmap to add to the Undo Buffer. A copy of this bitmap is done
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddUndo(wxBitmap *bm)
{
    //add 1 UNDO operation to the buffer
    if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_bitmap_undo, bm));
    return(false);
}

/** Add 1 Undo operation to the undo buffer
  * @param img: a pointer to the image to add to the Undo Buffer. A copy of this image is done
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddUndo(wxImage *img)
{
    //add 1 UNDO operation to the buffer
    if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_image_undo, img));
    return(false);
}

/** Add 1 Undo operation to the undo buffer
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddUndo(void)
{
    //add 1 Undo operation to the Buffer
    wxImage *img;
    img = GetImage();
    if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_image_undo, img));
    return(false);
}

/** Add 1 Undo operation to the redo buffer
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(void)
{
    //add 1 Redo operation to the Buffer
    wxImage *img;
    img = GetImage();
    if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_image_undo, img));
    return(false);
}

/** Add 1 Redo operation to the redo buffer
  * @param bm a pointer to the bitmap to add to the Redo Buffer. A copy of this bitmap is done
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(wxBitmap *bm)
{
    //add 1 REDO operation to the buffer
    if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_bitmap_undo, bm));
    return(false);
}

/** Add 1 Redo operation to the redo buffer
  * @param bm a pointer to the image to add to the Redo Buffer. A copy of this image is done
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(wxImage *img)
{
    //add 1 REDO operation to the buffer
    if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_image_undo, img));
    return(false);
}

/** Performs an Undo operation
  * @return no return value
  */
void XPMEditorPanel::Undo(void)
{
    //UNDO
    if (m_undo_buffer) m_undo_buffer->Undo();
}

/** Performs a Redo operation
  * @return no return value
  */
void XPMEditorPanel::Redo(void)
{
    //REDO
    if (m_undo_buffer) m_undo_buffer->Redo();
}

//------- TRANSPARENT COLOR CHANGED --------
void XPMEditorPanel::OnTransparentColorChanged(wxCommandEvent& event)
{
    //::wxMessageBox(_("event transparent color"), _("info"), wxOK);
    unsigned char iGreen, iRed, iBlue;
    wxColour cColour;

    //get the transparent colour
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
        iRed2 = m_Image->GetMaskRed();
        iGreen2 = m_Image->GetMaskGreen();
        iBlue2 = m_Image->GetMaskBlue();
        //wxMessageBox(wxString::Format(_("New mask colour Red=%d Green=%d Blue=%d\nOld Mask Colour Red=%d Green=%d Blue=%d"), iRed, iGreen, iBlue, iRed2, iGreen2, iBlue2), _("New mask colour"));
        m_Image->Replace(iRed2, iGreen2, iBlue2, iRed, iGreen, iBlue);
        m_Image->SetMaskColour(iGreen, iRed, iBlue);
        m_Image->SetMask(true);
        UpdateBitmap();
    }

    //update the display
    if (DrawCanvas)
    {
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }
}

//---- HANDLERS FOR ALL THE DRAWING TOOLS  ----
/** toggle all Tools Buttons OFF, except the iIndex one.
  * @param iIndex: the button to keep ON.
  *                if the button is OFF, it will stay OFF.
  *                if iIndex < 0, then ALL buttons are turned OFF
  * @param bClearSelection: if true, the current selection will be cleared (default).
  */
void XPMEditorPanel::ToggleButtons(int iIndex, bool bClearSelection)
{
    //toggle all Tools Buttons OFF, except the iIndex one.
    int i;

    if (bClearSelection)
    {
        ClearSelection();
        if (DrawCanvas)
        {
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
        }
    }

    for(i = 0;i<XPM_NUMBER_TOOLS;i++)
    {
        if ((i != iIndex) &&  (tools[i]))
        {
            tools[i]->SetValue(false);
        }
    }
    if ((iIndex >=0) && (iIndex < XPM_NUMBER_TOOLS))
    {
        if (tools[iIndex]->GetValue()) SetToolID(iIndex); else SetToolID(-1);
    }
    else
    {
        SetToolID(-1);
    }
    InitToolData();
}

/** Hide or Show specific tool controls - like style listbox for Brush tool...
  * @param iIndex: the tool selected
  * @param bChecked: if true, the tool is Activated
  */
void XPMEditorPanel::HideControls(int iIndex, bool bChecked)
{
    //hide other controls potentially shown
    if ((iIndex == XPM_ID_BRUSH_TOOL) && (bChecked))
    {
        //show brush tools

        wxGridBitmapRenderer *renderer;
        renderer = (wxGridBitmapRenderer*) Grid1->GetCellRenderer(0,0);
        if (renderer) renderer->SetBitmap(wxBitmap(circle_xpm));
        renderer = (wxGridBitmapRenderer*) Grid1->GetCellRenderer(0,1);
        if (renderer) renderer->SetBitmap(wxBitmap(square_xpm));
        renderer = (wxGridBitmapRenderer*) Grid1->GetCellRenderer(1,0);
        if (renderer) renderer->SetBitmap(wxBitmap(right_hair_xpm));
        renderer = (wxGridBitmapRenderer*) Grid1->GetCellRenderer(1,1);
        if (renderer) renderer->SetBitmap(wxBitmap(left_hair_xpm));
        SpinCtrl1->SetValue(2);

        //show tool specific control
        //Grid1->SetColSize(0, 12);
        //Grid1->SetColSize(1, 12);
        Grid1->SetRowSize(0, 12);
        Grid1->SetRowSize(1, 12);
        Grid1->Show(true);
        StaticText4->Show(true);
        SpinCtrl1->Show(true);
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
    }
    else if ((iIndex == XPM_ID_ERASER_TOOL) && (bChecked))
    {
        //show brush tools
        SpinCtrl1->SetValue(2);
        tdata.iSize = 1;

        //show tool specific control
        //Grid1->SetColSize(0, 12);
        //Grid1->SetColSize(1, 12);
        Grid1->Hide();
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
        StaticText4->Show(true);
        SpinCtrl1->Show(true);
    }
    else if (    ((iIndex == XPM_ID_LINE_TOOL) && (bChecked))
              || ((iIndex == XPM_ID_CURVE_TOOL) && (bChecked))
              || ((iIndex == XPM_ID_RECTANGLE_TOOL) && (bChecked))
              || ((iIndex == XPM_ID_ELLIPSE_TOOL) && (bChecked))
              || ((iIndex == XPM_ID_POLYGON_TOOL) && (bChecked))
            )
    {
        //show brush tools
        SpinCtrl2->SetValue(1);
        tdata.iSize = 1;

        //show tool specific control
        Grid1->Hide();
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText5->Show(true);
        SpinCtrl2->Show(true);
        StaticText6->Hide();
        SpinCtrl3->Hide();
    }
    else if ((iIndex == XPM_ID_ROUNDEDRECT_TOOL) && (bChecked))
    {
        //show brush tools
        SpinCtrl2->SetValue(1);
        SpinCtrl3->SetValue(2);
        tdata.iSize = 1;
        tdata.iRadius = 2;

        //show tool specific control
        Grid1->Hide();
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText6->Show(true);
        SpinCtrl3->Show(true);
        StaticText5->Show(true);
        SpinCtrl2->Show(true);
    }
    else
    {
        //hide brush tools
        Grid1->Hide();
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
    }
}

/** Hide or Show specific tool controls - like style listbox for Brush tool...
  * In addition, the panel layout (positionning of the controls on the panel) will be updated
  * @param iIndex: the tool selected
  * @param bChecked: if true, the tool is Activated
  */
void XPMEditorPanel::HideControlsAndDoLayout(int iIndex, bool bChecked)
{
    //hide other controls potentially shown and redo the layout
    HideControls(iIndex, bChecked);
    ToolPanelSizer->Layout();
    ToolPanelSizer->FitInside(ToolPanel);
}

void XPMEditorPanel::OnToggleSelectionTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_SELECT_TOOL); else ToggleButtons(-1);

    //hide other controls potentially shown
    HideControlsAndDoLayout(XPM_ID_SELECT_TOOL, event.IsChecked());
}


void XPMEditorPanel::OnToggleLassoTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_LASSO_TOOL); else ToggleButtons(-1);

    //hide other controls potentially shown
    HideControlsAndDoLayout(XPM_ID_LASSO_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleBrushTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked())
    {
        ToggleButtons(XPM_ID_BRUSH_TOOL);
        HideControlsAndDoLayout(XPM_ID_BRUSH_TOOL, true);
    }
    else
    {
        ToggleButtons(-1);
        //hide other controls potentially shown
        HideControlsAndDoLayout(XPM_ID_BRUSH_TOOL, false);
    }

}

void XPMEditorPanel::OnToggleLineTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_LINE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_LINE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleCurveTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_CURVE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_CURVE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnTogglePenTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_PEN_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_PEN_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleEllipseTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_ELLIPSE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_ELLIPSE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleRectangleTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_RECTANGLE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_RECTANGLE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleRoundedRectTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_ROUNDEDRECT_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_ROUNDEDRECT_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnTogglePipetteTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_PIPETTE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_PIPETTE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnTogglePolygonTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_POLYGON_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_POLYGON_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleFillTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_FILL_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_FILL_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleEraserTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_ERASER_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_ERASER_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnToggleTextTool(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_TEXT_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_TEXT_TOOL, event.IsChecked());
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


/** Event handler for Spin Control: the new value will update the size of the drawing tool
  */
void XPMEditorPanel::OnSpinSizeChanged(wxSpinEvent& event)
{
    tdata.iSize = event.GetPosition();
    SetToolCursor();
}

/** Event handler for Grid Control: the new value will update the brush style
  */
void XPMEditorPanel::OnBrushStyleChanged(wxGridEvent& event)
{
    int iRow, iCol;

    iRow = event.GetRow();
    iCol = event.GetCol();

    tdata.iStyle = XPM_BRUSH_STYLE_SQUARE; //default
    if (iRow == 0)
    {
        if (iCol == 0)  tdata.iStyle = XPM_BRUSH_STYLE_CIRCLE; else  tdata.iStyle = XPM_BRUSH_STYLE_SQUARE;
    }
    else if (iRow == 1)
    {
        if (iCol == 0)  tdata.iStyle = XPM_BRUSH_STYLE_LEFTHAIR; else  tdata.iStyle = XPM_BRUSH_STYLE_RIGHTHAIR;
    }
    SetToolCursor();
    event.Skip();
}

void XPMEditorPanel::OnSpinRadiusChanged(wxSpinEvent& event)
{
    tdata.iRadius = event.GetPosition();
}
