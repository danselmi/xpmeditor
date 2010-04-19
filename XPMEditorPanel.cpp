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
#include <math.h>

#define PI 3.14159265358979

//(*InternalHeaders(XPMEditorPanel)
#include <wx/scrolwin.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/things/toggle.h>
#include <wx/bitmap.h>
#include "XPMColorPicker.h"
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/radiobut.h>
#include "wxResizeCtrl.h"
#include <wx/settings.h>
#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/combobox.h>
//*)

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
#include "xpm/left.xpm"
#include "xpm/right.xpm"


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
const long XPMEditorPanel::ID_BUTTON1 = wxNewId();
const long XPMEditorPanel::ID_BUTTON2 = wxNewId();
const long XPMEditorPanel::ID_BUTTON3 = wxNewId();
const long XPMEditorPanel::ID_BUTTON4 = wxNewId();
const long XPMEditorPanel::ID_BITMAPBUTTON1 = wxNewId();
const long XPMEditorPanel::ID_BITMAPBUTTON2 = wxNewId();
const long XPMEditorPanel::ID_BUTTON5 = wxNewId();
const long XPMEditorPanel::ID_BUTTON6 = wxNewId();
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
const long XPMEditorPanel::ID_SQUARE_BRUSH = wxNewId();
const long XPMEditorPanel::ID_CIRCLE_BRUSH = wxNewId();
const long XPMEditorPanel::ID_LRHAIR_BRUSH = wxNewId();
const long XPMEditorPanel::ID_LHAIR_BRUSH = wxNewId();
const long XPMEditorPanel::ID_FONT_BUTTON = wxNewId();
const long XPMEditorPanel::ID_BKMODE_TOGGLEBUTTON = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT8 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON1 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON9 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON8 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON7 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON6 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON5 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON4 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON3 = wxNewId();
const long XPMEditorPanel::ID_RADIOBUTTON2 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT9 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL6 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT5 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL3 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT7 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL5 = wxNewId();
const long XPMEditorPanel::ID_STATICTEXT6 = wxNewId();
const long XPMEditorPanel::ID_SPINCTRL4 = wxNewId();
const long XPMEditorPanel::ID_PANEL1 = wxNewId();
const long XPMEditorPanel::ID_TEXTCTRL1 = wxNewId();
const long XPMEditorPanel::ID_RESIZECTRL1 = wxNewId();
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

    SquareBrushButton->Hide();
    SquareBrushButton->SetLabel(square_xpm);
    CircleBrushButton->Hide();
    CircleBrushButton->SetLabel(circle_xpm);
    LHairBrushButton->Hide();
    LHairBrushButton->SetLabel(left_hair_xpm);
    RHairBrushButton->Hide();
    RHairBrushButton->SetLabel(right_hair_xpm);

    StaticText4->Hide();
    SpinCtrl1->Hide();
    StaticText5->Hide();
    SpinCtrl2->Hide();
    StaticText6->Hide();
    SpinCtrl3->Hide();
    FontButton->Hide();
    TextEdit->Hide();
    ResizeCtrl1->Hide();
    BackgroundButton->Hide();
    TopLeft->Hide();
    TopCenter->Hide();
    TopRight->Hide();
    CenterLeft->Hide();
    CenterCenter->Hide();
    CenterRight->Hide();
    BottomLeft->Hide();
    BottomCenter->Hide();
    BottomRight->Hide();
    StaticText7->Hide();
    StaticText8->Hide();
    SpinCtrl4->Hide();
    ToolPanelSizer->Layout();
    ToolPanelSizer->FitInside(ToolPanel);

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
	wxBoxSizer* BoxSizer13;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer1;
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
	BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this, ID_BUTTON1, _("STRETCH"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer13->Add(Button1, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button2 = new wxButton(this, ID_BUTTON2, _("MIRROR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer13->Add(Button2, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button3 = new wxButton(this, ID_BUTTON3, _("BLUR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	BoxSizer13->Add(Button3, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button4 = new wxButton(this, ID_BUTTON4, _("ROTATE"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer13->Add(Button4, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BitmapButton1 = new wxBitmapButton(this, ID_BITMAPBUTTON1, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
	BoxSizer13->Add(BitmapButton1, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BitmapButton2 = new wxBitmapButton(this, ID_BITMAPBUTTON2, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
	BoxSizer13->Add(BitmapButton2, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button5 = new wxButton(this, ID_BUTTON5, _("HUE"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
	BoxSizer13->Add(Button5, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	Button6 = new wxButton(this, ID_BUTTON6, _("COLOUR DEPTH"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
	BoxSizer13->Add(Button6, 0, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer13->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	PanelSizer->Add(BoxSizer13, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	ColourPicker = new XPMColorPicker(this,ID_CUSTOM1,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_T("ID_CUSTOM1"));
	PanelSizer->Add(ColourPicker, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ToolSizer = new wxBoxSizer(wxVERTICAL);
	ToolPanel = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	ToolPanelSizer = new wxBoxSizer(wxVERTICAL);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	SelectButton = new wxCustomButton(ToolPanel,ID_SELECT_BUTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_SELECT_BUTN"));
	SelectButton->SetBitmapDisabled(SelectButton->CreateBitmapDisabled(SelectButton->GetBitmapLabel()));
	SelectButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer6->Add(SelectButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	LassoButton = new wxCustomButton(ToolPanel,ID_LASSO_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_LASSO_BTN"));
	LassoButton->SetBitmapDisabled(LassoButton->CreateBitmapDisabled(LassoButton->GetBitmapLabel()));
	LassoButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer6->Add(LassoButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	ToolPanelSizer->Add(BoxSizer6, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	PenButton = new wxCustomButton(ToolPanel,ID_PEN_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_PEN_BTN"));
	PenButton->SetBitmapDisabled(PenButton->CreateBitmapDisabled(PenButton->GetBitmapLabel()));
	PenButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer5->Add(PenButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BrushButton = new wxCustomButton(ToolPanel,ID_BRUSH_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BRUSH_BTN"));
	BrushButton->SetBitmapDisabled(BrushButton->CreateBitmapDisabled(BrushButton->GetBitmapLabel()));
	BrushButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer5->Add(BrushButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	PipetteButton = new wxCustomButton(ToolPanel,ID_PIPETTE_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_PIPETTE_BTN"));
	PipetteButton->SetBitmapDisabled(PipetteButton->CreateBitmapDisabled(PipetteButton->GetBitmapLabel()));
	PipetteButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer5->Add(PipetteButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FillButton = new wxCustomButton(ToolPanel,ID_FILL_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_FILL_BTN"));
	FillButton->SetBitmapDisabled(FillButton->CreateBitmapDisabled(FillButton->GetBitmapLabel()));
	FillButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer5->Add(FillButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	ToolPanelSizer->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	LineButton = new wxCustomButton(ToolPanel,ID_LINE_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_LINE_BTN"));
	LineButton->SetBitmapDisabled(LineButton->CreateBitmapDisabled(LineButton->GetBitmapLabel()));
	LineButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer7->Add(LineButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	CurveButton = new wxCustomButton(ToolPanel,ID_CURVE_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_CURVE_BTN"));
	CurveButton->SetBitmapDisabled(CurveButton->CreateBitmapDisabled(CurveButton->GetBitmapLabel()));
	CurveButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer7->Add(CurveButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EraserButton = new wxCustomButton(ToolPanel,ID_ERASER_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_ERASER_BTN"));
	EraserButton->SetBitmapDisabled(EraserButton->CreateBitmapDisabled(EraserButton->GetBitmapLabel()));
	EraserButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer7->Add(EraserButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	TextButton = new wxCustomButton(ToolPanel,ID_TEXT_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_TEXT_BTN"));
	TextButton->SetBitmapDisabled(TextButton->CreateBitmapDisabled(TextButton->GetBitmapLabel()));
	TextButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer7->Add(TextButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	ToolPanelSizer->Add(BoxSizer7, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	RectangleButton = new wxCustomButton(ToolPanel,ID_RECTANGLE_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_RECTANGLE_BTN"));
	RectangleButton->SetBitmapDisabled(RectangleButton->CreateBitmapDisabled(RectangleButton->GetBitmapLabel()));
	RectangleButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer10->Add(RectangleButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	PolygonButton = new wxCustomButton(ToolPanel,ID_POLYGON_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_POLYGON_BTN"));
	PolygonButton->SetBitmapDisabled(PolygonButton->CreateBitmapDisabled(PolygonButton->GetBitmapLabel()));
	PolygonButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer10->Add(PolygonButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EllipseButton = new wxCustomButton(ToolPanel,ID_ELLIPSE_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_ELLIPSE_BTN"));
	EllipseButton->SetBitmapDisabled(EllipseButton->CreateBitmapDisabled(EllipseButton->GetBitmapLabel()));
	EllipseButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer10->Add(EllipseButton, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	RRectButton = new wxCustomButton(ToolPanel,ID_ROUNDEDRECT_BTN,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_ROUNDEDRECT_BTN"));
	RRectButton->SetBitmapDisabled(RRectButton->CreateBitmapDisabled(RRectButton->GetBitmapLabel()));
	RRectButton->SetBitmapMargin(wxSize(2,2));
	BoxSizer10->Add(RRectButton, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	ToolPanelSizer->Add(BoxSizer10, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	ToolPanelSizer->Add(BoxSizer8, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	ToolPanelSizer->Add(-1,-1,0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	SquareBrushButton = new wxCustomButton(ToolPanel,ID_SQUARE_BRUSH,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CDROM")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_SQUARE_BRUSH"));
	SquareBrushButton->SetBitmapDisabled(SquareBrushButton->CreateBitmapDisabled(SquareBrushButton->GetBitmapLabel()));
	SquareBrushButton->SetBitmapMargin(wxSize(5,5));
	BoxSizer1->Add(SquareBrushButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	CircleBrushButton = new wxCustomButton(ToolPanel,ID_CIRCLE_BRUSH,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CDROM")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_CIRCLE_BRUSH"));
	CircleBrushButton->SetBitmapDisabled(CircleBrushButton->CreateBitmapDisabled(CircleBrushButton->GetBitmapLabel()));
	CircleBrushButton->SetBitmapMargin(wxSize(5,5));
	BoxSizer1->Add(CircleBrushButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	RHairBrushButton = new wxCustomButton(ToolPanel,ID_LRHAIR_BRUSH,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CDROM")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_LRHAIR_BRUSH"));
	RHairBrushButton->SetBitmapDisabled(RHairBrushButton->CreateBitmapDisabled(RHairBrushButton->GetBitmapLabel()));
	RHairBrushButton->SetBitmapMargin(wxSize(5,5));
	BoxSizer1->Add(RHairBrushButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	LHairBrushButton = new wxCustomButton(ToolPanel,ID_LHAIR_BRUSH,wxEmptyString,wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CDROM")),wxART_BUTTON),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_LHAIR_BRUSH"));
	LHairBrushButton->SetBitmapDisabled(LHairBrushButton->CreateBitmapDisabled(LHairBrushButton->GetBitmapLabel()));
	LHairBrushButton->SetBitmapMargin(wxSize(5,5));
	BoxSizer1->Add(LHairBrushButton, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 2);
	ToolPanelSizer->Add(BoxSizer1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	FontButton = new wxButton(ToolPanel, ID_FONT_BUTTON, _("FONT"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_FONT_BUTTON"));
	ToolPanelSizer->Add(FontButton, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BackgroundButton = new wxToggleButton(ToolPanel, ID_BKMODE_TOGGLEBUTTON, _("OPAQUE"), wxDefaultPosition, wxSize(90,23), 0, wxDefaultValidator, _T("ID_BKMODE_TOGGLEBUTTON"));
	ToolPanelSizer->Add(BackgroundButton, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticText7 = new wxStaticText(ToolPanel, ID_STATICTEXT8, _("Alignment:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	ToolPanelSizer->Add(StaticText7, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	AlignmentSizer = new wxGridSizer(3, 3, 0, 0);
	TopLeft = new wxRadioButton(ToolPanel, ID_RADIOBUTTON1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	TopLeft->SetValue(true);
	AlignmentSizer->Add(TopLeft, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 1);
	TopCenter = new wxRadioButton(ToolPanel, ID_RADIOBUTTON9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON9"));
	AlignmentSizer->Add(TopCenter, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 1);
	TopRight = new wxRadioButton(ToolPanel, ID_RADIOBUTTON8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON8"));
	AlignmentSizer->Add(TopRight, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_TOP, 1);
	CenterLeft = new wxRadioButton(ToolPanel, ID_RADIOBUTTON7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON7"));
	AlignmentSizer->Add(CenterLeft, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	CenterCenter = new wxRadioButton(ToolPanel, ID_RADIOBUTTON6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
	AlignmentSizer->Add(CenterCenter, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	CenterRight = new wxRadioButton(ToolPanel, ID_RADIOBUTTON5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	AlignmentSizer->Add(CenterRight, 1, wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
	BottomLeft = new wxRadioButton(ToolPanel, ID_RADIOBUTTON4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	AlignmentSizer->Add(BottomLeft, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	BottomCenter = new wxRadioButton(ToolPanel, ID_RADIOBUTTON3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	AlignmentSizer->Add(BottomCenter, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 1);
	BottomRight = new wxRadioButton(ToolPanel, ID_RADIOBUTTON2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	AlignmentSizer->Add(BottomRight, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_BOTTOM, 1);
	ToolPanelSizer->Add(AlignmentSizer, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	AngleSizer = new wxBoxSizer(wxVERTICAL);
	StaticText8 = new wxStaticText(ToolPanel, ID_STATICTEXT9, _("Angle (deg):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	AngleSizer->Add(StaticText8, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SpinCtrl4 = new wxSpinCtrl(ToolPanel, ID_SPINCTRL6, _T("0"), wxDefaultPosition, wxSize(72,21), 0, 0, 360, 0, _T("ID_SPINCTRL6"));
	SpinCtrl4->SetValue(_T("0"));
	AngleSizer->Add(SpinCtrl4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	ToolPanelSizer->Add(AngleSizer, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	StaticText4 = new wxStaticText(ToolPanel, ID_STATICTEXT5, _("Size:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	ToolPanelSizer->Add(StaticText4, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SpinCtrl1 = new wxSpinCtrl(ToolPanel, ID_SPINCTRL3, _T("2"), wxDefaultPosition, wxSize(83,21), 0, 2, 16, 2, _T("ID_SPINCTRL3"));
	SpinCtrl1->SetValue(_T("2"));
	ToolPanelSizer->Add(SpinCtrl1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	StaticText6 = new wxStaticText(ToolPanel, ID_STATICTEXT7, _("Radius:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	ToolPanelSizer->Add(StaticText6, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	SpinCtrl3 = new wxSpinCtrl(ToolPanel, ID_SPINCTRL5, _T("1"), wxDefaultPosition, wxSize(20,21), 0, 1, 1000, 1, _T("ID_SPINCTRL5"));
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
	TextEdit = new wxTextCtrl(DrawCanvas, ID_TEXTCTRL1, _("Text"), wxPoint(57,193), wxDefaultSize, wxTE_MULTILINE|wxTE_LEFT|wxTE_CHARWRAP, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	ResizeCtrl1 = new wxResizeCtrl(DrawCanvas,ID_RESIZECTRL1,wxPoint(6,7),wxSize(119,74),0,_T("ID_RESIZECTRL1"));
	CanvasSizer->Add(DrawCanvas, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	BoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
	sCursorPos = new wxStaticText(this, ID_STATICTEXT4, _("Cursor at: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer12->Add(sCursorPos, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	CanvasSizer->Add(BoxSizer12, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	BoxSizer3->Add(CanvasSizer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	PanelSizer->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
	SetSizer(PanelSizer);
	PanelSizer->Fit(this);
	PanelSizer->SetSizeHints(this);

	Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnZoomChanged);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnBitmapSizeChanged);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnBitmapSizeChanged);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnShowGrid);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnStretchImage);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnMirror);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnBlur);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRotate);
	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRotateCounterClockwise);
	Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRotateClockwise);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRotateHueClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnButtonColourDepthClick);
	Connect(ID_SELECT_BUTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnSelectButtonToggle);
	Connect(ID_LASSO_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnLassoButtonToggle);
	Connect(ID_PEN_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnPenButtonToggle);
	Connect(ID_BRUSH_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnBrushButtonToggle);
	Connect(ID_PIPETTE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnPipetteButtonToggle);
	Connect(ID_FILL_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnFillButtonToggle);
	Connect(ID_LINE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnLineButtonToggle);
	Connect(ID_CURVE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnCurveButtonToggle);
	Connect(ID_ERASER_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnEraserButtonToggle);
	Connect(ID_TEXT_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnTextButtonToggle);
	Connect(ID_RECTANGLE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRectangleButtonToggle);
	Connect(ID_POLYGON_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnPolygonButtonToggle);
	Connect(ID_ELLIPSE_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnEllipseButtonToggle);
	Connect(ID_ROUNDEDRECT_BTN,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRRectButtonToggle);
	Connect(ID_SQUARE_BRUSH,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnSquareBrushButtonToggle);
	Connect(ID_CIRCLE_BRUSH,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnCircleBrushButtonToggle);
	Connect(ID_LRHAIR_BRUSH,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnLHairBrushButtonToggle);
	Connect(ID_LHAIR_BRUSH,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnRHairBrushButtonToggle);
	Connect(ID_FONT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnFontButtonClick);
	Connect(ID_BKMODE_TOGGLEBUTTON,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&XPMEditorPanel::OnBackgroundButtonToggle);
	Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnTopLeftSelect);
	Connect(ID_RADIOBUTTON9,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnTopCenterSelect);
	Connect(ID_RADIOBUTTON8,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnTopRightSelect);
	Connect(ID_RADIOBUTTON7,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnCenterLeftSelect);
	Connect(ID_RADIOBUTTON6,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnCenterCenterSelect);
	Connect(ID_RADIOBUTTON5,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnCenterRightSelect);
	Connect(ID_RADIOBUTTON4,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnBottomLeftSelect);
	Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnBottomCenterSelect);
	Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&XPMEditorPanel::OnBottomRightSelect);
	Connect(ID_SPINCTRL6,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinCtrl4Change);
	Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinSizeChanged);
	Connect(ID_SPINCTRL5,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinRadiusChanged);
	Connect(ID_SPINCTRL4,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnSpinSizeChanged);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&XPMEditorPanel::OnTextEditText);
	DrawCanvas->Connect(wxEVT_PAINT,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasPaint,0,this);
	DrawCanvas->Connect(wxEVT_ERASE_BACKGROUND,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasEraseBackground,0,this);
	DrawCanvas->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasKeyDown,0,this);
	DrawCanvas->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftDown,0,this);
	DrawCanvas->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftUp,0,this);
	DrawCanvas->Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasLeftDClick,0,this);
	DrawCanvas->Connect(wxEVT_RIGHT_UP,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasRightUp,0,this);
	DrawCanvas->Connect(wxEVT_MOTION,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasMouseMove,0,this);
	DrawCanvas->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasMouseLeave,0,this);
	DrawCanvas->Connect(wxEVT_SIZE,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasResize,0,this);
	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&XPMEditorPanel::OnDrawCanvasKeyDown);
	//*)

    //Bitmap Button rotate
    BitmapButton2->SetBitmapLabel(left_xpm);
    BitmapButton1->SetBitmapLabel(right_xpm);

	//transparent color changed
	Connect(ID_CUSTOM1,wxEVT_TRANSPARENT_COLOR_CHANGED,(wxObjectEventFunction)&XPMEditorPanel::OnTransparentColorChanged);
	Connect(ID_CUSTOM1,wxEVT_FILL_COLOR_CHANGED,(wxObjectEventFunction)&XPMEditorPanel::OnFillColorChanged);
	Connect(ID_CUSTOM1,wxEVT_LINE_COLOR_CHANGED,(wxObjectEventFunction)&XPMEditorPanel::OnLineColorChanged);

    //ResizeCtrl
    ResizeCtrl1->SetChild(TextEdit);

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
    if (m_DragImage) delete m_DragImage;

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
  * if some points are move beyond image border, they are put back inside borders
  * There is therefore a risk of loosing data by using this function (loosing selection information)
  * @param dx : the increment by which the selection must be moved horizontally.
                Positive value moves the selection to the right
  * @param dy : the increment by which the selection must be moved vertically.
                Positive value moves the selection to the down
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

        //border check
        if (pSelection[i].x < 0) pSelection[i].x = 0;
        if (pSelection[i].y < 0) pSelection[i].y = 0;
        if (pSelection[i].x > m_Image->GetWidth()) pSelection[i].x = m_Image->GetWidth();
        if (pSelection[i].y > m_Image->GetHeight()) pSelection[i].y = m_Image->GetHeight();
    }
}

/** Return the current associated image
  * All changes buffered are flushed first
  * @return the wxImage stored.
  *         This is a copy of the wxImage
  */
wxImage XPMEditorPanel::GetImage(void)
{
    //return the associated image
    if (!m_Image) return(wxImage());

    wxImage img(*m_Image);
    img.SetMaskColour(cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue());

    return(img);
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

        int iFactor;
        iFactor = dScale * 100;
        if (ZoomFactor) ZoomFactor->SetValue(wxString::Format(_("%d%%"), iFactor));

        DoSetScrollBars();

        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
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
        //dc.DrawRectangle(0,0,m_Bitmap->GetWidth() * dScale, m_Bitmap->GetHeight() * dScale);

        wxMemoryDC memDC;
        memDC.SelectObject(*m_Bitmap);
        memDC.SetUserScale(1 / dScale, 1 / dScale);

        //main bitmap
        dc.Blit(0,0,m_Bitmap->GetWidth() * dScale, m_Bitmap->GetHeight() * dScale,&memDC,0,0, wxCOPY, false);

        //draw selection
        wxRect rSelection;
        wxMemoryDC memDC2;
        GetBoundingRect(&rSelection);
        memDC2.SelectObject(m_SelectionBitmap);
        memDC2.SetUserScale(1 / dScale, 1 / dScale);
        if ((pSelection) && (NbPoints > 1) && (bDrawSelection) && (memDC2.IsOk()))
        {

            /*
            //draw the selection border
            wxPen pSelectionPen(*wxBLUE, 1, wxSHORT_DASH);
            memDC2.SetPen(pSelectionPen);
            memDC2.SetBrush(*wxTRANSPARENT_BRUSH);

            wxPoint *tmp;
            tmp = new wxPoint[NbPoints];
            if (tmp)
            {
                int i;
                for(i=0;i<NbPoints;i++)
                {
                    tmp[i].x = (pSelection[i].x - rSelection.GetLeft()) * dScale;
                    tmp[i].y = (pSelection[i].y - rSelection.GetTop()) * dScale;
                }
                memDC2.DrawPolygon(NbPoints, tmp);
                delete[] tmp;
            }
            */
            //Selection bitmap
            dc.Blit(rSelection.GetLeft() * dScale, rSelection.GetTop() * dScale,
                    m_SelectionBitmap.GetWidth() * dScale, m_SelectionBitmap.GetHeight() * dScale,
                    &memDC2, 0, 0, wxCOPY, true); //Selection

            //draw the selection border
            wxPen pSelectionPen(*wxBLUE, 1, wxSHORT_DASH);
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
    if (m_undo_buffer) m_undo_buffer->AddUndo(xpm_size_undo, m_Image);
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
                            cMaskColour.Red(), cMaskColour.Green(), cMaskColour.Blue()
                           );
        }
        UpdateBitmap();
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
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();

        if (BMPWidth) BMPWidth->SetValue(xx);
        if (BMPHeight) BMPHeight->SetValue(yy);
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
        else if ((x >= 0) && (y >= 0))
        {
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
        else
        {
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
        case 1 : ProcessDragAction(x, y, false, false, false, true); break;
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 : ProcessSizeAction(x, y, false, false, false, true, iResult); break;
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

        default: break;
    }

}

/** Process the Pipette tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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

/** Process the Fill tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Pen tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
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
        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Text tool in action
  * a text control will be shown - this text control can be resized
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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

            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
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
            HideControlsAndDoLayout(-1, false);
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
        }
    }
}

/** Process the Selection tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessSelect(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
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
            bUsingTool = true;
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
            m_SelectionImage = GetImageFromSelection();
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
            InitToolData();
            m_bEraseSelection = true;
        }
    }
}

/** Process the Drag & Drop tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessDragAction(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
    if (bLeftDown)
    {

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
        wxPoint ptHotSpot(rSelection.GetWidth() * dScale / 2, rSelection.GetHeight() * dScale / 2);
        pStartDragging.x = rSelection.GetLeft();
        pStartDragging.y = rSelection.GetTop();
        m_DragImage->BeginDrag(ptHotSpot, DrawCanvas, false, NULL);
        m_DragImage->Hide();

        //Update Image
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
        m_DragImage->Move(wxPoint(x, y));
        m_DragImage->Show();


    }
    else if ((bPressed) && (m_bDragging))
    {

        if (!m_Bitmap) return;
        if (!m_DragImage) return;

        //m_DragImage->Hide();
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
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
            MoveSelection(xx / dScale - m_SelectionImage.GetWidth() / 2 - rSelection.GetLeft(),
                          yy / dScale - m_SelectionImage.GetHeight()/ 2 - rSelection.GetTop());

            SetCursor(wxCURSOR_HAND);
        }
        m_bEraseSelection = false;
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();

        InitToolData();
    }
}

/** Process the stretch selection tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  * @param iDirection: the direction of the stretch
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

        int iLeft, iTop, iRight, iBottom;
        iLeft = rSelection.GetLeft();
        iRight = rSelection.GetRight();
        iTop = rSelection.GetTop();
        iBottom = rSelection.GetBottom();
        if ((iDirection == 2) || (iDirection == 6) || (iDirection == 7)) iTop = y;
        if ((iDirection == 3) || (iDirection == 6) || (iDirection == 8)) iLeft = x;
        if ((iDirection == 4) || (iDirection == 8) || (iDirection == 9)) iBottom = y;
        if ((iDirection == 5) || (iDirection == 7) || (iDirection == 9)) iRight = x;

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

        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
    }

    if (bLeftUp)
    {
        //finish
        m_bSizing = false;
        InitToolData();
    }
}

/** Process the Lasso (complex selection) tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessLasso(int x, int y,
                                  bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
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
            bUsingTool = true;
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
            tdata.iNbClicks = 1;
            tdata.x1 = x  / dScale;
            tdata.y1 = y / dScale;
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
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
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
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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
            iSize = tdata.iSize;

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
                        //mem_dc.DrawLine(xx - tdata.iSize /2.828 ,yy + tdata.iSize / 2.828, xx + tdata.iSize / 2.828, yy - tdata.iSize / 2.828);
                        break;

                case XPM_BRUSH_STYLE_RIGHTHAIR:
                        for(i=0; i < iSize; i++)
                        {
                            mem_dc.DrawPoint(xx - iSize / 2 + i, yy - iSize / 2 + i);
                        }
                        //mem_dc.DrawLine(xx - tdata.iSize / 2.828, yy - tdata.iSize /2.828, xx + tdata.iSize /2.828, yy + tdata.iSize / 2.828);
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
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
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
            iSize = tdata.iSize;

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
                    //mem_dc.DrawLine(xx - tdata.iSize /2.828,yy + tdata.iSize / 2.828, xx + tdata.iSize / 2.828, yy - tdata.iSize / 2.828);
                    break;

                case XPM_BRUSH_STYLE_RIGHTHAIR:
                    for(i=0; i < iSize; i++)
                    {
                            mem_dc.DrawPoint(xx - iSize / 2 + i, yy - iSize / 2 + i);
                    }
                    //mem_dc.DrawLine(xx - tdata.iSize / 2.828, yy - tdata.iSize / 2.828, xx + tdata.iSize / 2.828, yy + tdata.iSize / 2.828);
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
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
    }

    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Eraser tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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
    if (bLeftUp)
    {
        //finish
        InitToolData();
    }
}

/** Process the Polygon tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessPolygon(int x, int y,
                                      bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
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
            bUsingTool = true;
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

        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
        InitToolData();
        ToggleButtons(-1,false);
    }
}

/** Process the Rectangle tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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

            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();

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
            dc.DrawRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
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
            bUsingTool = true;
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
                mem_dc.DrawRectangle(tdata.x1 , tdata.y1 ,
                                     x / dScale - tdata.x1 , y / dScale - tdata.y1);

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
}

/** Process the Line tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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

            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();

            int iPenWidth;
            if (dScale < 1) iPenWidth = tdata.iSize; else iPenWidth = dScale * tdata.iSize;
            wxClientDC dc(DrawCanvas);
            DrawCanvas->DoPrepareDC(dc);

            wxColour cLineColour;
            cLineColour = ColourPicker->GetLineColour();

            wxPen pen(cLineColour, iPenWidth, wxSOLID);
            dc.SetPen(pen);
            dc.DrawLine(tdata.x1 * dScale, tdata.y1 * dScale,
                        x , y );
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
            bUsingTool = true;
        }
        else
        {
            if (!m_Bitmap) return;
            AddUndo();
            SetModified(true);
            wxMemoryDC mem_dc(*m_Bitmap);
            if (mem_dc.IsOk())
            {
                wxColour cLineColour;
                cLineColour = ColourPicker->GetLineColour();

                wxPen pen(cLineColour, tdata.iSize, wxSOLID);
                mem_dc.SetPen(pen);
                mem_dc.DrawLine(tdata.x1 , tdata.y1 ,
                                x / dScale, y / dScale);

                mem_dc.SelectObject(wxNullBitmap);
            }
            UpdateImage();

            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
            InitToolData();
        }
    }
}

/** Process the Curve tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
  */
void XPMEditorPanel::ProcessCurve(int x, int y,
                                bool bLeftDown, bool bLeftUp, bool bPressed, bool bDClick)
{
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
            bUsingTool = true;
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

        DrawCanvas->Refresh(false, NULL);
        DrawCanvas->Update();
        InitToolData();
        ToggleButtons(-1,false);
    }

}

/** Process the RoundedRectangle tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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

            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();

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
            dc.DrawRoundedRectangle(tdata.x1 * dScale, tdata.y1 * dScale,
                                    x - tdata.x1 * dScale, y - tdata.y1 * dScale, tdata.iRadius);
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
            bUsingTool = true;
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
                mem_dc.DrawRoundedRectangle(tdata.x1 , tdata.y1 ,
                                     x / dScale - tdata.x1 , y / dScale - tdata.y1, tdata.iRadius);

                mem_dc.SelectObject(wxNullBitmap);
            }
            UpdateImage();
            wxRect r(tdata.x1 * dScale - 10, tdata.y1 * dScale - 10,
                     x - tdata.x1 * dScale + 20, y - tdata.y1 * dScale + 20);

            DrawCanvas->Refresh(false, &r);
            DrawCanvas->Update();
            InitToolData();
        }
    }
}

/** Process the Ellipse tool in action
  * @param x: the new mouse X position - Scaled coordinates - clipped to bitmap
  * @param y: the new mouse Y position - Scaled coordinates - clipped to bitmap
  * @param bLeftDown: true if a mouse left button DOWN event has been triggered
  * @param bLeftUp: true if a mouse left button UP event has been triggered
  * @param bPressed: true if the left mouse button is currently pressed
  * @param bDClick: true if the a double-click event occured
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

            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();

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
            dc.DrawEllipse(tdata.x1 * dScale, tdata.y1 * dScale,
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
            bUsingTool = true;
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
                mem_dc.DrawEllipse(tdata.x1 , tdata.y1 ,
                                   x / dScale - tdata.x1 , y / dScale - tdata.y1);

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
}

/** init tool data for a first use
  */
void XPMEditorPanel::InitToolData(void)
{
    bUsingTool = false;
    tdata.x1 = -1;
    tdata.y1 = -1;
    tdata.x2 = -1;
    tdata.y2 = -1;
    tdata.iNbClicks = 0;
    tdata.iNbPoints = 0;
    if (DrawCanvas) tdata.font = DrawCanvas->GetFont(); else tdata.font = GetFont();
    tdata.sText = _("");

    int i;
    for(i=0;i < XPM_MAXPOINTS; i++)
    {
        tdata.pts[i].x = -1;
        tdata.pts[i].y = -1;
    }

    switch(iToolUsed)
    {
        case XPM_ID_ERASER_TOOL:
        case XPM_ID_BRUSH_TOOL : tdata.iSize = SpinCtrl1->GetValue();
                                 if (tdata.iSize < 2) tdata.iSize = 2;
                                 break;

        case XPM_ID_LINE_TOOL:
        case XPM_ID_CURVE_TOOL:
        case XPM_ID_RECTANGLE_TOOL:
        case XPM_ID_POLYGON_TOOL:
        case XPM_ID_ELLIPSE_TOOL: tdata.iSize = SpinCtrl2->GetValue();
                                  if (tdata.iSize < 2) tdata.iSize = 2;
                                  break;

        case XPM_ID_ROUNDEDRECT_TOOL: tdata.iSize = SpinCtrl2->GetValue();
                                      if (tdata.iSize < 2) tdata.iSize = 2;

                                      tdata.iRadius = SpinCtrl3->GetValue();
                                      if (tdata.iRadius < 2) tdata.iRadius = 2;
                                      break;

        case XPM_ID_TEXT_TOOL: if (BackgroundButton->GetValue())
                               {
                                   tdata.iStyle = wxTRANSPARENT;
                               }
                               else
                               {
                                   tdata.iStyle = wxSOLID;
                               }
                               if (TopLeft->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_LEFT;
                                   tdata.iVertAlign = wxALIGN_TOP;
                               }
                               if (TopCenter->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_CENTER;
                                   tdata.iVertAlign = wxALIGN_TOP;
                               }
                               if (TopRight->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_RIGHT;
                                   tdata.iVertAlign = wxALIGN_TOP;
                               }
                               if (CenterLeft->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_LEFT;
                                   tdata.iVertAlign = wxALIGN_CENTER;
                               }
                               if (CenterCenter->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_CENTER;
                                   tdata.iVertAlign = wxALIGN_CENTER;
                               }
                               if (CenterRight->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_RIGHT;
                                   tdata.iVertAlign = wxALIGN_CENTER;
                               }
                               if (BottomLeft->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_LEFT;
                                   tdata.iVertAlign = wxALIGN_BOTTOM;
                               }
                               if (BottomCenter->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_CENTER;
                                   tdata.iVertAlign = wxALIGN_BOTTOM;
                               }
                               if (BottomRight->GetValue())
                               {
                                   tdata.iHorizAlign = wxALIGN_RIGHT;
                                   tdata.iVertAlign = wxALIGN_BOTTOM;
                               }

                               tdata.angle = SpinCtrl4->GetValue();
                               break;

        default: break;
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
        int iSize, iSizeMin;
        double dScale2;
        if (dScale < 1) dScale2 = 1; else dScale2 = dScale;
        if (tdata.iSize < 2) tdata.iSize = 2;
        iSize = tdata.iSize * dScale2;
        if (tdata.iStyle == XPM_BRUSH_STYLE_SQUARE) iSize = iSize + 2 * dScale2;
        if ((tdata.iStyle == XPM_BRUSH_STYLE_CIRCLE) && (tdata.iSize * dScale2 <= 4)) iSize = 5;
        if (iSize < 8) iSizeMin = 8; else iSizeMin = iSize;
        wxBitmap bm(iSizeMin, iSizeMin);

        //2 - draw on the bitmap
        wxMemoryDC mem_dc;
        mem_dc.SelectObject(bm);
        mem_dc.SetBrush(*wxRED_BRUSH);
        mem_dc.SetPen(*wxRED_PEN);

        switch (tdata.iStyle)
        {
            case XPM_BRUSH_STYLE_CIRCLE   :
                mem_dc.DrawRectangle(0,0,iSizeMin, iSizeMin);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxWHITE_PEN);
                mem_dc.DrawCircle(iSizeMin/2, iSizeMin/2, iSizeMin/2);
                break;

            case XPM_BRUSH_STYLE_LEFTHAIR :
                mem_dc.DrawRectangle(0,0,iSizeMin, iSizeMin);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxBLACK_PEN);
                mem_dc.DrawLine(0,iSizeMin-1, iSizeMin-1, 0);
                break;

            case XPM_BRUSH_STYLE_RIGHTHAIR:
                mem_dc.DrawRectangle(0,0,iSizeMin, iSizeMin);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxBLACK_PEN);
                mem_dc.DrawLine(0,0, iSizeMin-1, iSizeMin-1);
                break;

            case XPM_BRUSH_STYLE_SQUARE   :
            default:
                mem_dc.DrawRectangle(0,0,iSizeMin+2, iSizeMin+2);
                mem_dc.SetBrush(*wxBLACK_BRUSH);
                mem_dc.SetPen(*wxWHITE_PEN);
                mem_dc.DrawRectangle(1,1,iSizeMin, iSizeMin);
                break;
        }
        mem_dc.SetPen(*wxRED_PEN);
        mem_dc.DrawPoint(iSizeMin / 2, iSizeMin / 2);
        mem_dc.SelectObject(wxNullBitmap);

        //3 - convert the wxBitmap to a wxImage, and set the HotSpot
        wxImage img;
        img = bm.ConvertToImage();
        img.SetMaskColour(255,0,0);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, iSizeMin / 2);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, iSizeMin / 2);

        //4 - create and set the cursor
        wxCursor cursor(img);
        SetCursor(cursor);
    }
    else if (iTool == XPM_ID_ERASER_TOOL)
    {
        //create the correct cursor
        //1 - create the bitmap
        int iSize, iSizeMin;
        double dScale2;
        if (tdata.iSize < 2) tdata.iSize = 2;
        if (dScale < 1) dScale2 = 1; else dScale2 = dScale;
        iSize = (tdata.iSize + 2) * dScale2;
        if (iSize < 8) iSizeMin = 8; else iSizeMin = iSize;
        wxBitmap bm(iSizeMin, iSizeMin);


        //2 - draw on the bitmap
        wxMemoryDC mem_dc;
        mem_dc.SelectObject(bm);
        mem_dc.SetBrush(*wxRED_BRUSH);
        mem_dc.SetPen(*wxRED_PEN);

        mem_dc.DrawRectangle(0,0,iSizeMin, iSizeMin);
        mem_dc.SetBrush(*wxBLACK_BRUSH);
        mem_dc.SetPen(*wxWHITE_PEN);
        mem_dc.DrawRectangle(1,1,iSizeMin-2, iSizeMin-2);
        mem_dc.SetPen(*wxRED_PEN);
        mem_dc.DrawPoint(iSizeMin / 2, iSizeMin / 2);
        mem_dc.SelectObject(wxNullBitmap);

        //3 - convert the wxBitmap to a wxImage, and set the HotSpot
        wxImage img;
        img = bm.ConvertToImage();
        img.SetMaskColour(255,0,0);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, iSizeMin / 2);
        img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, iSizeMin / 2);

        //4 - create and set the cursor
        wxCursor cursor(img);
        SetCursor(cursor);
    }
    else
    {
        SetCursor(ToolCursor[iTool]);
    }
}

/** Return if the cursor is hovering over the selection
  * @param x : x position of the mouse cursor, relative to DrawCanvas. Unscrolled, unscaled
  * @param y : y position of the mouse cursor, relative to DrawCanvas. Unscrolled, unscaled
  * @return  1 if the cursor is hovering over the selection
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
    wxRegion region(NbPoints, pSelection);
    int xx, yy;

    DrawCanvas->CalcUnscrolledPosition(x,y,&xx, &yy);
    wxRegionContain rResult;
    if (dScale > 0) {xx = xx / dScale; yy = yy / dScale;}
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

        int iResult;
        iResult = IsPointInSelection(x, y);
        switch(iResult)
        {
            case 1 : ProcessDragAction(xx, yy, true, false, true, false); break;
            case 2 :
            case 3 :
            case 4 :
            case 5 :
            case 6 :
            case 7 :
            case 8 :
            case 9 : ProcessSizeAction(xx, yy, true, false, true, false, iResult); break;
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
        GetBoundingRect(&rSelectionRect);

        pSelection = CheckMemorySelection(4);
        if (!pSelection) return;
        NbPoints = 4;

        pSelection[0].x = rSelectionRect.GetLeft();
        pSelection[0].y = rSelectionRect.GetTop();

        pSelection[1].x = rSelectionRect.GetRight();
        pSelection[1].y = rSelectionRect.GetTop();

        pSelection[2].x = rSelectionRect.GetRight();
        pSelection[2].y = rSelectionRect.GetBottom();

        pSelection[3].x = rSelectionRect.GetLeft();
        pSelection[3].y = rSelectionRect.GetTop();

    }
}

/** Replace the rectangle by a new image
  * if the new image is smaller than the rectangle, then the new image is pasted as (0,0) on the selection,
  * and the remaining space is filled with the mask (transparent) colour
  * if the new image is bigger than the rectangle, then it is resized first to fit to the rectangle
  * @param newImage: the new image which will replace the rectangle
  * @param rRect: a wxRect representing the rectangle to be replaced
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
  * @param newImage: the new image which will replace the rectangle
  * @param x : the X coordinate where the paste will occur
  * @param y : the Y coordinate where the paste will occur
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

    if (DrawCanvas)
    {
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
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
        //paste the selection at the current coordinates
        PasteSelection();

        //remove the selection
        NbPoints = 0;

        //for text tool, which use the selection
        InitToolData();
    }
}

/** Paste the content of the selection to the current selection coordinates
  * @return no return value
  */
void XPMEditorPanel::PasteSelection(void)
{
    wxRect rSelection;
    GetBoundingRect(&rSelection);

    PasteImage(m_SelectionImage, rSelection.GetLeft(), rSelection.GetTop());
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
        if (DrawCanvas)
        {
            DrawCanvas->Refresh(true, NULL);
            DrawCanvas->Update();
        }
    }
}

/** perform a "copy" clipbopard operation
  * @return no return value
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
            m_SelectionBitmap = bm;
            m_SelectionImage = bm.ConvertToImage();
            //if (bm.GetMask()) wxMessageBox(_("mask")); else wxMessageBox(_("no mask"));
            SetModified(true);
            DrawCanvas->Refresh(false, NULL);
            DrawCanvas->Update();
            m_bEraseSelection = false;
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
    if (m_undo_buffer) return(m_undo_buffer->AddUndo(xpm_image_undo, m_Image));
    return(false);
}

/** Add 1 Undo operation to the redo buffer
  * @return true on success, false otherwise
  */
bool XPMEditorPanel::AddRedo(void)
{
    //add 1 Redo operation to the Buffer
    if (m_undo_buffer) return(m_undo_buffer->AddRedo(xpm_image_undo, m_Image));
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
  * @param iIndex: the button to keep ON.
  *                if the button is OFF, it will stay OFF.
  *                if iIndex < 0, then ALL buttons are turned OFF
  * @param bClearSelection: if true, the current selection will be cleared (default).
  */
void XPMEditorPanel::ToggleButtons(int iIndex, bool bClearSelection)
{
    //toggle all Tools Buttons OFF, except the iIndex one.
    int i;

    if ((bClearSelection) && (HasSelection()))
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
            if (tools[i]->GetValue()) tools[i]->SetValue(false);
        }
    }
    SetToolID(-1);
    if ((iIndex >=0) && (iIndex < XPM_NUMBER_TOOLS))
    {
        if (tools[iIndex]->GetValue()) SetToolID(iIndex);
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
        SpinCtrl1->SetValue(2);

        //show tool specific control
        LHairBrushButton->Show(true);
        RHairBrushButton->Show(true);
        CircleBrushButton->Show(true);
        SquareBrushButton->Show(true);
        StaticText4->Show(true);
        SpinCtrl1->Show(true);
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
        FontButton->Hide();
        BackgroundButton->Hide();
        TextEdit->Hide();
        ResizeCtrl1->Hide();
        TopLeft->Hide();
        TopCenter->Hide();
        TopRight->Hide();
        CenterLeft->Hide();
        CenterCenter->Hide();
        CenterRight->Hide();
        BottomLeft->Hide();
        BottomCenter->Hide();
        BottomRight->Hide();
        StaticText7->Hide();
        StaticText8->Hide();
        SpinCtrl4->Hide();
    }
    else if ((iIndex == XPM_ID_ERASER_TOOL) && (bChecked))
    {
        //show brush tools
        SpinCtrl1->SetValue(2);
        tdata.iSize = 1;

        //show tool specific control
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
        StaticText4->Show(true);
        SpinCtrl1->Show(true);
        LHairBrushButton->Hide();
        RHairBrushButton->Hide();
        CircleBrushButton->Hide();
        SquareBrushButton->Hide();
        FontButton->Hide();
        BackgroundButton->Hide();
        TextEdit->Hide();
        ResizeCtrl1->Hide();
        TopLeft->Hide();
        TopCenter->Hide();
        TopRight->Hide();
        CenterLeft->Hide();
        CenterCenter->Hide();
        CenterRight->Hide();
        BottomLeft->Hide();
        BottomCenter->Hide();
        BottomRight->Hide();
        StaticText7->Hide();
        StaticText8->Hide();
        SpinCtrl4->Hide();
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
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText5->Show(true);
        SpinCtrl2->Show(true);
        StaticText6->Hide();
        SpinCtrl3->Hide();
        LHairBrushButton->Hide();
        RHairBrushButton->Hide();
        CircleBrushButton->Hide();
        SquareBrushButton->Hide();
        FontButton->Hide();
        BackgroundButton->Hide();
        TextEdit->Hide();
        ResizeCtrl1->Hide();
        TopLeft->Hide();
        TopCenter->Hide();
        TopRight->Hide();
        CenterLeft->Hide();
        CenterCenter->Hide();
        CenterRight->Hide();
        BottomLeft->Hide();
        BottomCenter->Hide();
        BottomRight->Hide();
        StaticText7->Hide();
        StaticText8->Hide();
        SpinCtrl4->Hide();
    }
    else if ((iIndex == XPM_ID_ROUNDEDRECT_TOOL) && (bChecked))
    {
        //show brush tools
        SpinCtrl2->SetValue(1);
        SpinCtrl3->SetValue(2);
        tdata.iSize = 1;
        tdata.iRadius = 2;

        //show tool specific control
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText6->Show(true);
        SpinCtrl3->Show(true);
        StaticText5->Show(true);
        SpinCtrl2->Show(true);
        LHairBrushButton->Hide();
        RHairBrushButton->Hide();
        CircleBrushButton->Hide();
        SquareBrushButton->Hide();
        FontButton->Hide();
        BackgroundButton->Hide();
        TextEdit->Hide();
        ResizeCtrl1->Hide();
        TopLeft->Hide();
        TopCenter->Hide();
        TopRight->Hide();
        CenterLeft->Hide();
        CenterCenter->Hide();
        CenterRight->Hide();
        BottomLeft->Hide();
        BottomCenter->Hide();
        BottomRight->Hide();
        StaticText7->Hide();
        StaticText8->Hide();
        SpinCtrl4->Hide();
    }
    else if (iIndex == XPM_ID_TEXT_TOOL)
    {
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
        LHairBrushButton->Hide();
        RHairBrushButton->Hide();
        CircleBrushButton->Hide();
        SquareBrushButton->Hide();
        FontButton->Show(true);
        BackgroundButton->Show(true);
        //hide for now - will show them later
        TextEdit->Hide();
        ResizeCtrl1->Hide();
        TopLeft->Show(true);
        TopCenter->Show(true);
        TopRight->Show(true);
        CenterLeft->Show(true);
        CenterCenter->Show(true);
        CenterRight->Show(true);
        BottomLeft->Show(true);
        BottomCenter->Show(true);
        BottomRight->Show(true);
        StaticText7->Show(true);
        StaticText8->Show(true);
        SpinCtrl4->Show(true);
    }
    else
    {
        //hide all tools
        StaticText4->Hide();
        SpinCtrl1->Hide();
        StaticText5->Hide();
        SpinCtrl2->Hide();
        StaticText6->Hide();
        SpinCtrl3->Hide();
        LHairBrushButton->Hide();
        RHairBrushButton->Hide();
        CircleBrushButton->Hide();
        SquareBrushButton->Hide();
        FontButton->Hide();
        BackgroundButton->Hide();
        TextEdit->Hide();
        ResizeCtrl1->Hide();
        TopLeft->Hide();
        TopCenter->Hide();
        TopRight->Hide();
        CenterLeft->Hide();
        CenterCenter->Hide();
        CenterRight->Hide();
        BottomLeft->Hide();
        BottomCenter->Hide();
        BottomRight->Hide();
        StaticText7->Hide();
        StaticText8->Hide();
        SpinCtrl4->Hide();
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

void XPMEditorPanel::OnSelectButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_SELECT_TOOL); else ToggleButtons(-1);

    //hide other controls potentially shown
    HideControlsAndDoLayout(XPM_ID_SELECT_TOOL, event.IsChecked());
}


void XPMEditorPanel::OnLassoButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_LASSO_TOOL); else ToggleButtons(-1);

    //hide other controls potentially shown
    HideControlsAndDoLayout(XPM_ID_LASSO_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnBrushButtonToggle(wxCommandEvent& event)
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

void XPMEditorPanel::OnLineButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_LINE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_LINE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnCurveButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_CURVE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_CURVE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnPenButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_PEN_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_PEN_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnEllipseButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_ELLIPSE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_ELLIPSE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnRectangleButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_RECTANGLE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_RECTANGLE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnRRectButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_ROUNDEDRECT_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_ROUNDEDRECT_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnPipetteButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_PIPETTE_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_PIPETTE_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnPolygonButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_POLYGON_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_POLYGON_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnFillButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_FILL_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_FILL_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnEraserButtonToggle(wxCommandEvent& event)
{
    //toggle all the other buttons
    if (event.IsChecked()) ToggleButtons(XPM_ID_ERASER_TOOL); else ToggleButtons(-1);
    HideControlsAndDoLayout(XPM_ID_ERASER_TOOL, event.IsChecked());
}

void XPMEditorPanel::OnTextButtonToggle(wxCommandEvent& event)
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

void XPMEditorPanel::OnSpinRadiusChanged(wxSpinEvent& event)
{
    tdata.iRadius = event.GetPosition();
}


void XPMEditorPanel::OnSquareBrushButtonToggle(wxCommandEvent& event)
{

    if (event.IsChecked())
    {
        tdata.iStyle = XPM_BRUSH_STYLE_SQUARE;
        CircleBrushButton->SetValue(false);
        LHairBrushButton->SetValue(false);
        RHairBrushButton->SetValue(false);
        //SetToolCursor();
    }
}

void XPMEditorPanel::OnCircleBrushButtonToggle(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        tdata.iStyle = XPM_BRUSH_STYLE_CIRCLE;
        SquareBrushButton->SetValue(false);
        LHairBrushButton->SetValue(false);
        RHairBrushButton->SetValue(false);
        //SetToolCursor();
    }
}

void XPMEditorPanel::OnLHairBrushButtonToggle(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        tdata.iStyle = XPM_BRUSH_STYLE_LEFTHAIR;
        SquareBrushButton->SetValue(false);
        CircleBrushButton->SetValue(false);
        RHairBrushButton->SetValue(false);
        //SetToolCursor();
    }
}

void XPMEditorPanel::OnRHairBrushButtonToggle(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        tdata.iStyle = XPM_BRUSH_STYLE_RIGHTHAIR;
        SquareBrushButton->SetValue(false);
        CircleBrushButton->SetValue(false);
        LHairBrushButton->SetValue(false);
        //SetToolCursor();
    }
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
  * @param dc: the dc to draw on
  * @param value: the string to draw
  * @param rect: the bounding rectangle
  * @param horizAlign: the horizontal alignment wxALIGN_RIGHT, wxALIGN_LEFT, wxALIGN_CENTER
  * @param vertAlign : the vertical alignment wxALIGN_BOTTOM, wxALIGN_LEFT, wxALIGN_TOP
  * @param double: the angle / orientation of the text (in degrees, not in radians)
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
  * @param dc: the dc to draw on
  * @param value: the string to draw
  * @param rect: the bounding rectangle
  * @param horizAlign: the horizontal alignment wxALIGN_RIGHT, wxALIGN_LEFT, wxALIGN_CENTER
  * @param vertAlign : the vertical alignment wxALIGN_BOTTOM, wxALIGN_LEFT, wxALIGN_TOP
  * @param double: the angle / orientation of the text (in degrees, not in radians)
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
  * @param value: the string to split
  * @param lines: the wxArrayString which will receive the strings - 1 per line
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


/** Process a char event. Here, it is mainly used for the text tool
  */


/** Change the font used by the text tool
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
        if (DrawCanvas)
        {
            DrawCanvas->Refresh(true, NULL);
            DrawCanvas->Update();
        }
    }
}

/** Change the background mode used by the text tool (opaque or transparent)
  */
void XPMEditorPanel::OnBackgroundButtonToggle(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        BackgroundButton->SetLabel(_("TRANSPARENT"));
        tdata.iStyle = wxTRANSPARENT;
    }
    else
    {
        BackgroundButton->SetLabel(_("OPAQUE"));
        tdata.iStyle = wxSOLID;
    }
    DrawTextBitmap();
    if (DrawCanvas)
    {
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
    }
}

void XPMEditorPanel::OnLineColorChanged(wxCommandEvent& event)
{
    wxColour cColour;

    //get the new line colour
    if (!ColourPicker) return;
    cColour = ColourPicker->GetLineColour();

    if (bUsingTool)
    {
        switch(iToolUsed)
        {
            case XPM_ID_TEXT_TOOL: DrawTextBitmap();
                                   if (DrawCanvas)
                                   {
                                       DrawCanvas->Refresh(true, NULL);
                                       DrawCanvas->Update();
                                   }
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
        switch(iToolUsed)
        {
            case XPM_ID_TEXT_TOOL: DrawTextBitmap();
                                   if (DrawCanvas)
                                   {
                                       DrawCanvas->Refresh(true, NULL);
                                       DrawCanvas->Update();
                                   }
                                   break;
            default: break;
        }
    }
}

void XPMEditorPanel::OnTextEditText(wxCommandEvent& event)
{
    if (bUsingTool)
    {
        switch(iToolUsed)
        {
            case XPM_ID_TEXT_TOOL: tdata.sText = wxString(TextEdit->GetValue());
                                   DrawTextBitmap();
                                   if (DrawCanvas)
                                   {
                                       DrawCanvas->Refresh(true, NULL);
                                       DrawCanvas->Update();
                                   }
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
    bool bUpdate;
    int iModifiers;
    int dx;

    bUpdate = false;
    iModifiers = event.GetModifiers();
    if (iModifiers & wxMOD_CONTROL) dx = 1; else dx = 5;

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
        case WXK_LEFT : if (HasSelection()) MoveSelection(-dx,0); bUpdate = true; break;
        case WXK_UP   : if (HasSelection()) MoveSelection(0,-dx); bUpdate = true; break;
        case WXK_RIGHT: if (HasSelection()) MoveSelection(dx,0); bUpdate = true; break;
        case WXK_DOWN : if (HasSelection()) MoveSelection(0,dx); bUpdate = true; break;

        default: break;
    }

    if ((bUpdate) && (DrawCanvas))
    {
        DrawCanvas->Refresh(true, NULL);
        DrawCanvas->Update();
    }

    event.Skip();
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
            if (BMPWidth) BMPWidth->SetValue(sDrawAreaSize.GetWidth());
            if (BMPHeight) BMPHeight->SetValue(sDrawAreaSize.GetHeight());

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
void XPMEditorPanel::OnRotateCounterClockwise(wxCommandEvent& event)
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
void XPMEditorPanel::OnRotateClockwise(wxCommandEvent& event)
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
        int iAngle;
        double dAngle;
        iAngle = rd.SpinCtrl1->GetValue();
        dAngle = iAngle / 360;

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
void XPMEditorPanel::OnSpinCtrl4Change(wxSpinEvent& event)
{
    int iCurrentPos;
    iCurrentPos = event.GetPosition();

    //wxMessageBox(wxString::Format(_("iPos=%d iCurrentPos=%d"), iPos, iCurrentPos));

    if (iPos > iCurrentPos)
    {
        //the user downgrades the value

        if (iCurrentPos <= 90) SpinCtrl4->SetValue(0);
        if ((iCurrentPos <= 180) && (iCurrentPos > 90)) SpinCtrl4->SetValue(90);
        if ((iCurrentPos <= 270) && (iCurrentPos > 180)) SpinCtrl4->SetValue(180);
        if ((iCurrentPos <= 360) && (iCurrentPos > 270)) SpinCtrl4->SetValue(270);
    }
    else
    {
        //the user upgrades the value
        if (iCurrentPos > 270) SpinCtrl4->SetValue(0);
        if ((iCurrentPos > 180) && (iCurrentPos <= 270)) SpinCtrl4->SetValue(270);
        if ((iCurrentPos > 90) && (iCurrentPos <= 180)) SpinCtrl4->SetValue(180);
        if ((iCurrentPos > 0) && (iCurrentPos <= 90)) SpinCtrl4->SetValue(90);
    }
    iPos = iCurrentPos;

}
