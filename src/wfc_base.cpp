#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/numdlg.h>

#include "wfc_base.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	MainFrame *win = new MainFrame(wxT("Wave function collapse"), wxPoint (100, 100),
	wxSize(450, 340));
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	
	win->SetIcon(wxICON(AppIcon));
	
	SetExitOnFrameDelete(TRUE);
	win->Show(TRUE);
	SetTopWindow(win);
	
	srand(time(NULL));
	
	return TRUE;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, ID_MAINWIN_QUIT);
	Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
	Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
	Bind(wxEVT_MOUSEWHEEL, &MainFrame::OnMouseWheel, this);
	Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);
	
	Bind(wxEVT_ERASE_BACKGROUND, &MainFrame::OnEreaseBackground, this);
	Bind(wxEVT_MENU, &MainFrame::OnOpenTemplate, this, ID_MAINWIN_OPEN_TEMPLATE);
	Bind(wxEVT_MENU, &MainFrame::OnCanvasSize, this, ID_MAINWIN_CANVAS_SIZE);
	Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, ID_MAINWIN_TIMER);

	wxMenu *FileMenu = new wxMenu;
	wxMenuBar *MenuBar = new wxMenuBar;
	
	FileMenu->Append(ID_MAINWIN_QUIT, wxT("&Beenden\tALT-F4"));
	FileMenu->Append(ID_MAINWIN_OPEN_TEMPLATE, wxT("Kachelvorlage öffnen"));
	FileMenu->AppendSeparator();
	FileMenu->Append(ID_MAINWIN_CANVAS_SIZE, wxT("Leinwandgröße ändern"));
	
	MenuBar->Append(FileMenu, wxT("&Datei"));
	SetMenuBar(MenuBar);
	
	
	canvasX = canvasY = 20;
	pixelScale = 3;
	imgCanvas = wxImage(canvasX * 3, canvasY * 3, true);
	wxColor bgCol = wxClientDC(this).GetBackground().GetColour();
	imgCanvas.SetRGB(wxRect(0, 0, imgCanvas.GetWidth(), imgCanvas.GetHeight()), bgCol.GetRed(), bgCol.GetGreen(), bgCol.GetBlue());
	
	c = NULL;
	thState.finished = 1;
	
	TIMERTICK = 200;
		
	CreateStatusBar(2);
	SetStatusText(wxT("Wave function collapse"));
	
	//Maximize(true);
}

MainFrame::~MainFrame()
{
	if (c) {
		deleteCells(&c);
	}
	if (m_tiles) {
		deleteTiles(&m_tiles);
	}
}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
	Close(TRUE);
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.SetBrush(wxBrush(dc.GetBackground()));
	dc.SetPen(wxPen(dc.GetBackground().GetColour()));
	dc.DrawRectangle(wxPoint(0, 0), dc.GetSize());
	dc.SetPen(wxPen(wxColor(0, 0, 0)));
	
	SetStatusText(wxString::Format(wxT("DC-Größe: %d, %d"), dc.GetSize().GetWidth(), dc.GetSize().GetHeight()), 1);
	
	if(imgCanvas.IsOk())
	{
		dc.DrawBitmap(wxBitmap(imgCanvas.Scale(imgCanvas.GetWidth()*pixelScale, imgCanvas.GetHeight()*pixelScale)), wxPoint(0, 0));
	}
	return;
}

void MainFrame::OnEreaseBackground(wxEraseEvent &event)
{
	return;
}

void MainFrame::OnSize(wxSizeEvent &event)
{
	Refresh();
	return;
}

void MainFrame::OnOpenTemplate(wxCommandEvent &event)
{
	if (thState.finished != 1) {
		if (wxMessageDialog(this,
						wxT("Thread ist noch aktiv!\nNeues Template öffnen?"),
						wxT("Achtung"),
						wxYES | wxNO).ShowModal() == wxID_YES) {
			thState.abort = 1;
			wxMessageDialog(this, wxT("Thread wird abgebrochen")).ShowModal();
			while(thState.finished != 1);
		} else {
			return;	
		}
	}
	wxFileDialog TexturOpener(this, wxT("Kachelvorlage öffnen"), wxT(""), wxT(""), wxT("PNG/JPEG-Dateien (*.png, *.jpg)|*png; *.jpg"),1);
	if(TexturOpener.ShowModal()==wxID_CANCEL)return;
    wxString DateiEndung, Pfad;
    DateiEndung = Pfad = TexturOpener.GetPath();
	
	/* Endung suchen */
	for(int i= DateiEndung.Len(); i>0; i--)
	{
		if(DateiEndung.GetChar(i) == '.')
		{
			DateiEndung = Pfad.Right(DateiEndung.Len()-i-1);
			break;
		}
	}
	/* ENDE Endung suchen */

	if(!DateiEndung.CmpNoCase(wxT("jpg")) || !DateiEndung.CmpNoCase(wxT("jpeg")))
	{
		imgTemplate.LoadFile(Pfad, wxBITMAP_TYPE_JPEG);
		SetStatusText(wxString::Format(wxT("JPEG geöffnet: %s"), Pfad));
	}else
	if(!DateiEndung.CmpNoCase(wxT("png")))
	{
		imgTemplate.LoadFile(Pfad, wxBITMAP_TYPE_PNG);
		SetStatusText(wxString::Format(wxT("PNG geöffnet: %s"), Pfad));
	}else{
		SetStatusText(wxT("Dateityp wird nicht unterstützt"));
		return;
	}
	
	if(imgTemplate.GetHeight() < 3)
		return;
	
	imgCanvas.Destroy();
	imgCanvas = wxImage(canvasX, canvasY, true);
	wxColor bgCol = wxClientDC(this).GetBackground().GetColour();
	imgCanvas.SetRGB(wxRect(0, 0, imgCanvas.GetWidth(), imgCanvas.GetHeight()), bgCol.GetRed(), bgCol.GetGreen(), bgCol.GetBlue());
	
	if (m_tiles) {
		deleteTiles(&m_tiles);
		m_tiles = NULL;
	}
	unsigned char* templateData = imgTemplate.GetData();
	unsigned char* canvasData = imgCanvas.GetData();
	
	initTiles(&m_tiles, &maxTiles, templateData, imgTemplate.GetWidth(), imgTemplate.GetHeight());
	if (c != NULL) {
		deleteCells(&c);
		printf("Cells = %p\n", (void*)c);
	}
	initCells(&c, canvasX, canvasY, maxTiles);
	
	thState.finished = 0;
	thState.abort = 0;
	std::thread thCollapseGrid(&collapseGrid, c, canvasX, canvasY, &m_tiles, &maxTiles, canvasData, imgCanvas.GetWidth(), imgCanvas.GetHeight(), &thState);
	thCollapseGrid.detach();
	
	timer.SetOwner(this, ID_MAINWIN_TIMER);
	timer.Start(TIMERTICK);

    return;
}

void MainFrame::OnCanvasSize(wxCommandEvent &event)
{
	wxNumberEntryDialog nmbWidth(this, wxT("Bitte geben Sie eine Ganzzahl ein!"), wxT("neue Breite"), wxT("Leinwandgröße ändern"), canvasX, 1, 300);
	nmbWidth.ShowModal();
	canvasX = nmbWidth.GetValue();
	
	
	wxNumberEntryDialog nmbHeight(this, wxT("Bitte geben Sie eine Ganzzahl ein!"), wxT("neue Höhe"), wxT("Leinwandgröße ändern"), canvasY, 1, 300);
	nmbHeight.ShowModal();
	canvasY = nmbHeight.GetValue();
	return;
}

void MainFrame::OnTimer(wxTimerEvent &event)
{
	Refresh();
	if (thState.finished == 1) {
		timer.Stop();
	}
	return;
}

void MainFrame::OnMouseWheel(wxMouseEvent &event)
{
	if (event.GetWheelRotation() < 0) {
		pixelScale--;
	}
	if (event.GetWheelRotation() > 0) {
		pixelScale++;
	}
	
	if (pixelScale < 1) {
		pixelScale = 1;
	}else if (pixelScale > MAXPIXELSCALE) {
		pixelScale = MAXPIXELSCALE;
	}
	
	SetStatusText(wxString::Format(wxT("pixel scale: %d"), pixelScale), 0);
	Refresh();
}

void MainFrame::OnKeyDown(wxKeyEvent &event)
{
	switch (event.GetKeyCode()) {
		case WXK_ESCAPE:
			if (thState.finished != 1 && wxMessageDialog(this,
													wxT("Thread läuft noch!\n Soll abgebrochen werden?"),
													wxT("Escape gedrückt"), wxYES|wxNO).ShowModal() == wxID_YES
				) {
				thState.abort = 1;
				while(thState.finished != 1);
				wxMessageDialog(this, wxT("Thread erfolgreich abgebrochen")).ShowModal();
			} else {
				SetStatusText(wxT("Esc gedrückt"), 0);
			}
			break;
	}
	return;
}