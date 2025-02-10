#ifndef __BASE_H
#define __BASE_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>

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
		
	private:
		
};

enum
{
   ID_MAINWIN_QUIT = wxID_HIGHEST+1
};


#endif
