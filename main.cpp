#include <wx/app.h>
#include <wx/image.h>


#include "dbg_src/dbg_wx/debugger.h"

class MyApp : public wxApp
{
	public:
		virtual bool OnInit();
};

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    debugger* Frame = new debugger(0);
    Frame->Show();
    SetTopWindow(Frame);

    return true;
}

