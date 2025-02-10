#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "base.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
   MainFrame *win = new MainFrame(wxT("Wave function collapse"), wxPoint (100, 100),
     wxSize(450, 340));
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
	
	wxMenu *FileMenu = new wxMenu;
    wxMenuBar *MenuBar = new wxMenuBar;

    FileMenu->Append(ID_MAINWIN_QUIT, wxT("&Beenden\tALT-F4"));

    MenuBar->Append(FileMenu, wxT("&Datei"));
    SetMenuBar(MenuBar);	

    CreateStatusBar(2);
    SetStatusText(wxT("Wave function collapse"));
	
	Maximize(true);
}

MainFrame::~MainFrame()
{
	
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
	
	SetStatusText(wxString::Format("DC-Größe: %d, %d", dc.GetSize().GetWidth(), dc.GetSize().GetHeight()), 1);
	
	wxPoint t_mitte(dc.GetSize().GetWidth() / 2, dc.GetSize().GetHeight() / 2);
	
	dc.SetBrush(wxBrush(wxColor(117,35,90)));
	dc.DrawRectangle(t_mitte, wxSize(50, 50));
	dc.DrawText("Hallo Welt!", dc.GetSize().GetWidth() / 2, dc.GetSize().GetHeight() / 2);
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