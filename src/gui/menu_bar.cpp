#include <wx/wx.h>
#include "main_frame.h"

void SetFrameMainMenuBar(wxFrame* parent)
{
    wxMenu* fileMenu = new wxMenu;
    wxMenu* helpMenu = new wxMenu;

    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "About this program");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    parent->SetMenuBar(menuBar);
}
