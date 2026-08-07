// app.h
#pragma once

#include <filesystem>
#include <fstream>

#include <wx/wx.h>
#include <wx/filepicker.h>

#include "util/config.h"

class DMirror : public wxApp
{
    public:
        bool OnInit() override;
        void OnStartClicked(wxCommandEvent& event);
        void OnSourceDirChanged(wxFileDirPickerEvent& event);
        void OnDestinationDirChanged(wxFileDirPickerEvent& event);
        void StartCopy();
        std::optional<std::string> GetSavedSourceDir();
        std::optional<std::string> GetSavedDestinationDir();
        static std::filesystem::path GetAppDataPath();
        static std::filesystem::path GetConfigPath();
    
    private:
        void CreateAppDataDir();
        wxFrame* frame;
        wxString srcDirPath;
        wxString dstDirPath;
        Config* config;
};

wxDECLARE_APP(DMirror);