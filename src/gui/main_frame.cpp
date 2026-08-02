#include <wx/wx.h>
#include <wx/filepicker.h>
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

    auto* srcPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto* srcSizer = new wxBoxSizer(wxVERTICAL);
    srcSizer->Add(new wxStaticText(srcPanel, wxID_ANY, "Source Directory"), 0, wxBOTTOM | wxCENTER, 8);
    srcSizer->Add(
        new wxDirPickerCtrl(
            srcPanel,
            wxID_ANY,
            wxEmptyString,
            "Select a source directory",
            wxDefaultPosition,
            wxDefaultSize,
            wxDIRP_DEFAULT_STYLE | wxDIRP_USE_TEXTCTRL
        ),
        0,
        wxEXPAND
    );
    srcPanel->SetSizer(srcSizer);

    auto* dstPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto* dstSizer = new wxBoxSizer(wxVERTICAL);
    dstSizer->Add(new wxStaticText(dstPanel, wxID_ANY, "Destination Directory"), 0, wxBOTTOM | wxCENTER, 8);
    dstSizer->Add(
        new wxDirPickerCtrl(
            dstPanel,
            wxID_ANY,
            wxEmptyString,
            "Select a destination directory",
            wxDefaultPosition,
            wxDefaultSize,
            wxDIRP_DEFAULT_STYLE | wxDIRP_USE_TEXTCTRL
        ),
        0,
        wxEXPAND
    );
    dstPanel->SetSizer(dstSizer);

    auto* ctrlPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto* ctrlSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* startButton = new wxButton(ctrlPanel, wxID_ANY, "Start");
    auto* cancelButton = new wxButton(ctrlPanel, wxID_ANY, "Cancel");
    ctrlSizer->Add(startButton, 0, wxBOTTOM | wxCENTER, 8);
    ctrlSizer->Add(cancelButton, 0, wxBOTTOM | wxCENTER, 8);
    ctrlPanel->SetSizer(ctrlSizer);

    auto* sizerSelect = new wxBoxSizer(wxHORIZONTAL);
    sizerSelect->Add(srcPanel, 1, wxEXPAND | wxALL, 10);
    sizerSelect->Add(dstPanel, 1, wxEXPAND | wxALL, 10);

    auto* sizerMain = new wxBoxSizer(wxVERTICAL);
    sizerMain->Add(sizerSelect, 1, wxEXPAND | wxALL, 10);
    sizerMain->Add(ctrlPanel, 0, wxEXPAND | wxALL, 10);

    panel->SetSizer(sizerMain);

    startButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartClicked, this);
    cancelButton->Bind(wxEVT_BUTTON, &MainFrame::OnCancelClicked, this);
}

void MainFrame::OnStartClicked(wxCommandEvent& event)
{
    std::cout << "Start button clicked!" << std::endl;
}

void MainFrame::OnCancelClicked(wxCommandEvent& event)
{
    std::cout << "Cancel button clicked!" << std::endl;
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