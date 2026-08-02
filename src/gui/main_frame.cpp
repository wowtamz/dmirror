#include <wx/wx.h>
#include <wx/mstream.h>

#include "main_frame.h"
#include "menu_bar.h"

#include "app.h"
#include "icon.h"
#include "util/image.h"
#include "dialog/about_dialog.h"
#include "gui/dir_picker_panel.h"

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

    auto* dirPickerPanel = new DirPickerPanel(panel);

    auto* ctrlPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto* ctrlSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* startButton = new wxButton(ctrlPanel, wxID_ANY, "Start");
    auto* cancelButton = new wxButton(ctrlPanel, wxID_ANY, "Cancel");
    ctrlSizer->AddStretchSpacer();
    ctrlSizer->Add(startButton, 0, wxBOTTOM | wxCENTER, 8);
    ctrlSizer->Add(cancelButton, 0, wxBOTTOM | wxCENTER, 8);
    ctrlSizer->AddStretchSpacer();
    ctrlPanel->SetSizer(ctrlSizer);

    auto* sizerMain = new wxBoxSizer(wxVERTICAL);
    sizerMain->Add(dirPickerPanel, 1, wxEXPAND | wxALL, 10);
    sizerMain->Add(ctrlPanel, 0, wxEXPAND | wxALL, 10);

    panel->SetSizer(sizerMain);

    auto* app = &wxGetApp();
    startButton->Bind(wxEVT_BUTTON, &DMirror::OnStartClicked, app);
    cancelButton->Bind(wxEVT_BUTTON, &DMirror::OnCancelClicked, app);
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