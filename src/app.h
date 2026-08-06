// app.h
#pragma once

#include <filesystem>
#include <fstream>

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
        static std::filesystem::path GetAppDataPath();
    
    private:
        void CreateAppDataDir();
        wxFrame* frame;
        wxString srcDirPath;
        wxString dstDirPath;
};

wxDECLARE_APP(DMirror);