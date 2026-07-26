#include <wx/wx.h>
#include <wx/mstream.h>

#include "main_frame.h"
#include "menu_bar.h"

#include "icon.h"
#include "dialog/about_dialog.h"
#include "../util/image.h"

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "DMirror", wxDefaultPosition, wxSize(512, 256))
{    

    wxBitmap bitmap = LoadBitmapFromResource(icon_png, icon_png_len);

    if (bitmap.IsOk()) {
        wxIcon icon;
        icon.CopyFromBitmap(bitmap);
        SetIcon(icon);
    }
    else {
        wxLogError("Failed to load icon from resource.");
    }

    SetFrameMainMenuBar(this);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);

    auto* panel = new wxPanel(this);

    m_button = new wxButton(panel, wxID_ANY, "Click Me!");

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_button, 0, wxALL | wxCENTER, 60);

    panel->SetSizer(sizer);

    m_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
}

void MainFrame::OnButtonClicked(wxCommandEvent& event)
{
    wxMessageBox(
            "You clicked the button!",
            "Hello World!",
            wxOK | wxICON_INFORMATION,
            this
    );
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    AboutDialog dialog(this);
    dialog.ShowModal();
}