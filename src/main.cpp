// Third-party headers
#include <wx/wx.h>
#include <wx/mstream.h>

// Local headers
#include "resources/icon.h"

wxBitmap LoadBitmapFromResource(const unsigned char* data, unsigned int size)
{
    wxMemoryInputStream stream(data, size);
    wxImage image(stream, wxBITMAP_TYPE_PNG);

    if (image.IsOk())
    {
        return wxBitmap(image);
    }
    else
    {
        return wxNullBitmap;
    }
}

class MyFrame : public wxFrame
{
    public:
        MyFrame();

    private:
        void CreateMenuBar();
        void OnButtonClicked(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        wxButton* m_button;
};

class DMirror : public wxApp
{
    public:
        bool OnInit() override;
};

MyFrame::MyFrame()
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

    CreateMenuBar();

    auto* panel = new wxPanel(this);

    m_button = new wxButton(panel, wxID_ANY, "Click Me!");

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_button, 0, wxALL | wxCENTER, 60);

    panel->SetSizer(sizer);

    m_button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
}

wxIMPLEMENT_APP(DMirror);

bool DMirror::OnInit()
{
    wxInitAllImageHandlers();

    auto* frame = new MyFrame();

    frame->Show();
    return true;
}

void MyFrame::CreateMenuBar()
{
    // Create menus
    wxMenu* fileMenu = new wxMenu;
    wxMenu* helpMenu = new wxMenu;

    // Add menu items
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");


    helpMenu->Append(wxID_ABOUT, "&About\tF1", "About this program");

    // Create menu bar
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    
    // Attack it to the window
    SetMenuBar(menuBar);

    // Connect events
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
}

void MyFrame::OnButtonClicked(wxCommandEvent& event)
{
    wxMessageBox(
            "You clicked the button!",
            "Hello World!",
            wxOK | wxICON_INFORMATION,
            this
    );
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxDialog dialog(this, wxID_ANY, "About", wxDefaultPosition, wxSize(400, 200));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxBitmap bitmap = LoadBitmapFromResource(icon_png, icon_png_len);

    sizer->Add(new wxStaticBitmap(&dialog, wxID_ANY, bitmap), 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticText* title = new wxStaticText(&dialog, wxID_ANY, "DMirror");
    wxFont titleFont = title->GetFont();
    titleFont.SetPointSize(18);
    title->SetFont(titleFont);
    title->Wrap(300);
    sizer->Add(title, 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticText* author = new wxStaticText(&dialog, wxID_ANY, "by Tamas Henninger-Varadi");
    sizer->Add(author, 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticText* description = new wxStaticText(&dialog, wxID_ANY, "DMirror is a simple data backup tool that allows you to easily copy files and folders from one location to another.");
    description->Wrap(300); // Wrap the text to fit within 300 pixels
    sizer->Add(description, 0, wxALIGN_CENTER | wxALL, 10);

    sizer->Add(new wxButton(&dialog, wxID_OK, "OK"), 0, wxALIGN_CENTER | wxALL, 10);

    dialog.SetSizerAndFit(sizer);
    dialog.ShowModal();
}