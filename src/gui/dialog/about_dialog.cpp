#include <wx/wx.h>
#include "about_dialog.h"
#include "icon.h"
#include "util/image.h"

AboutDialog::AboutDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "About", wxDefaultPosition, wxSize(400, 200))
{

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxBitmap bitmap = LoadBitmapFromResource(icon_png, icon_png_len);

    sizer->Add(new wxStaticBitmap(this, wxID_ANY, bitmap), 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticText* title = new wxStaticText(this, wxID_ANY, "DMirror");
    wxFont titleFont = title->GetFont();
    titleFont.SetPointSize(18);
    title->SetFont(titleFont);
    title->Wrap(300);
    sizer->Add(title, 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticText* author = new wxStaticText(this, wxID_ANY, "by Tamas Henninger-Varadi");
    sizer->Add(author, 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticText* description = new wxStaticText(this, wxID_ANY, "DMirror is a simple data backup tool that allows you to easily copy files and folders from one location to another.");
    description->Wrap(300); // Wrap the text to fit within 300 pixels
    sizer->Add(description, 0, wxALIGN_CENTER | wxALL, 10);

    sizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALIGN_CENTER | wxALL, 10);

    this->SetSizerAndFit(sizer);
}