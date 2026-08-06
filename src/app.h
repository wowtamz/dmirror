// app.h
#pragma once

#include <wx/wx.h>
#include <wx/filepicker.h>

class DMirror : public wxApp
{
    public:
        bool OnInit() override;
        void OnStartClicked(wxCommandEvent& event);
        void OnSourceDirChanged(wxFileDirPickerEvent& event);
        void OnDestinationDirChanged(wxFileDirPickerEvent& event);
        void StartCopy();
    
    private:
        wxFrame* frame;
        wxString srcDirPath;
        wxString dstDirPath;
};

wxDECLARE_APP(DMirror);