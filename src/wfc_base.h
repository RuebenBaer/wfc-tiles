#ifndef __BASE_H
#define __BASE_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <thread>

extern "C" {
	#include "wfc\wfc-core.h"
	#include "queue.h"
}

#define MAXPIXELSCALE 10

class MainApp: public wxApp
{
public:
	virtual bool OnInit();
};

class MainFrame: public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	~MainFrame();
	
private:
	void OnQuit(wxCommandEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnEreaseBackground(wxEraseEvent &event);
	void OnOpenTemplate(wxCommandEvent &event);
	void OnCanvasSize(wxCommandEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnMouseWheel(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseLeftDown(wxMouseEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	
	void CollapsWaveFunction(void);

	wxImage imgTemplate, imgCanvas;
	int canvasX, canvasY;
	int pixelScale;
	int dcOffsetX, dcOffsetY;
	wxPoint mousePos;
	tile *m_tiles;
	int maxTiles;
	cell *c;

	wxTimer timer;
	int TIMERTICK;
	threadState thState;
};

enum
{
	ID_MAINWIN_QUIT = wxID_HIGHEST+1,
    ID_MAINWIN_OPEN_TEMPLATE, ID_MAINWIN_CANVAS_SIZE, ID_MAINWIN_TIMER
};


#endif
