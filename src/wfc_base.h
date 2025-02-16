#ifndef __BASE_H
#define __BASE_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "wfc\wfc-core.h"

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
		void OnQuit(wxCommandEvent &event);
		void OnSize(wxSizeEvent &event);
		void OnPaint(wxPaintEvent &event);
		void OnEreaseBackground(wxEraseEvent &event);
		void OnOpenTemplate(wxCommandEvent &event);
		
	private:
		wxImage imgTemplate, imgCanvas;
		tile *m_tiles;
		int maxTiles;
		cell *c;
};

enum
{
	ID_MAINWIN_QUIT = wxID_HIGHEST+1,
    ID_MAINWIN_OPEN_TEMPLATE
};


#endif
