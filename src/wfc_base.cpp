#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "wfc_base.h"

#define GRIDSIZE 10

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
	
	return TRUE;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, ID_MAINWIN_QUIT);
	Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
	Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &MainFrame::OnEreaseBackground, this);
	Bind(wxEVT_MENU, &MainFrame::OnOpenTemplate, this, ID_MAINWIN_OPEN_TEMPLATE);

	wxMenu *FileMenu = new wxMenu;
	wxMenuBar *MenuBar = new wxMenuBar;
	
	FileMenu->Append(ID_MAINWIN_QUIT, wxT("&Beenden\tALT-F4"));
	FileMenu->Append(ID_MAINWIN_OPEN_TEMPLATE, wxT("Kachelvorlage öffnen"));
	
	MenuBar->Append(FileMenu, wxT("&Datei"));
	SetMenuBar(MenuBar);
	
	imgCanvas = wxImage(GRIDSIZE * 3, GRIDSIZE * 3, true);
	
	c = new cell[GRIDSIZE * GRIDSIZE];
	
	srand(time(NULL));
	
	CreateStatusBar(2);
	SetStatusText(wxT("Wave function collapse"));
	
	//Maximize(true);
}

MainFrame::~MainFrame()
{
	delete []c;
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
		dc.DrawBitmap(wxBitmap(imgCanvas.Scale(imgCanvas.GetWidth()*5, imgCanvas.GetHeight()*5)), wxPoint(0, 0));
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
	imgCanvas = wxImage(GRIDSIZE * 3, GRIDSIZE * 3, true);
	
	int imgWidth = imgTemplate.GetWidth();
	maxTiles = imgWidth / 3;
	if (m_tiles) {
		deleteTiles(&m_tiles);
	}
	initTiles(&m_tiles, maxTiles);
	printf("m_tiles: %p\n", (void*)m_tiles);
	
	unsigned char* templateData = imgTemplate.GetData();
	for (int i = 0; i < maxTiles; i++) {
		for (int w = 0; w < 3; w++) {
			for (int h = 0; h < 3; h++) {
				m_tiles[i].data[(w + h * 3) * 3 + 0] = templateData[((i * 3 + w) + h * imgWidth) * 3 + 0];
				m_tiles[i].data[(w + h * 3) * 3 + 1] = templateData[((i * 3 + w) + h * imgWidth) * 3 + 1];
				m_tiles[i].data[(w + h * 3) * 3 + 2] = templateData[((i * 3 + w) + h * imgWidth) * 3 + 2];
			}
		}
	}
	
	tile *curTile = &m_tiles[rand() % maxTiles];
	
	pos p;
	p.x = rand() % GRIDSIZE;
	p.y = rand() % GRIDSIZE;
	
	collapseCell(&c[p.x + p.y * GRIDSIZE], curTile, p);
	drawCell(&c[p.x + p.y * GRIDSIZE], imgCanvas.GetData(), imgCanvas.GetWidth(), imgCanvas.GetHeight());

    Refresh();
    return;
}