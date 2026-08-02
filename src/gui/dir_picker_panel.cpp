#include <wx/wx.h>
#include <wx/filepicker.h>

#include "dir_picker_panel.h"

DirPickerPanel::DirPickerPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{

    auto* srcPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto* srcSizer = new wxBoxSizer(wxVERTICAL);
    auto* srcPicker = new wxDirPickerCtrl(
        srcPanel,
        wxID_ANY,
        wxEmptyString,
        "Select a destination directory",
        wxDefaultPosition,
        wxDefaultSize,
        wxDIRP_DEFAULT_STYLE | wxDIRP_USE_TEXTCTRL
    );
    srcSizer->Add(new wxStaticText(srcPanel, wxID_ANY, "Source Directory"), 0, wxBOTTOM | wxCENTER, 8);
    srcSizer->Add(srcPicker, 0, wxEXPAND);
    srcPanel->SetSizer(srcSizer);

    auto* dstPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto* dstSizer = new wxBoxSizer(wxVERTICAL);
    auto* dstPicker = new wxDirPickerCtrl(
        dstPanel,
        wxID_ANY,
        wxEmptyString,
        "Select a destination directory",
        wxDefaultPosition,
        wxDefaultSize,
        wxDIRP_DEFAULT_STYLE | wxDIRP_USE_TEXTCTRL
    );
    dstSizer->Add(new wxStaticText(dstPanel, wxID_ANY, "Destination Directory"), 0, wxBOTTOM | wxCENTER, 8);
    dstSizer->Add(dstPicker, 0, wxEXPAND);
    dstPanel->SetSizer(dstSizer);

    auto* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(srcPanel, 1, wxEXPAND | wxALL, 5);
    sizer->Add(dstPanel, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(sizer);
}