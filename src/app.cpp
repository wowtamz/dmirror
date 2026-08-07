#include "app.h"
#include "gui/main_frame.h"
#include "gui/dialog/progress_dialog.h"
#include "lib/dmirror.h"

#include <thread>
#include <chrono>
#include <wx/stdpaths.h>

const std::string CONFIG_FILE = "preferences.conf";

bool DMirror::OnInit()
{
    wxInitAllImageHandlers();
    CreateAppDataDir();

    config = new Config();

    if (!config->read(DMirror::GetConfigPath().string())) {
        config->add("src_dir", "");
        config->add("dst_dir", "");
        config->save(DMirror::GetConfigPath().string());
    }

    frame = new MainFrame();

    auto srcDir = config->get("src_dir");
    auto dstDir = config->get("dst_dir");

    if (srcDir) {
        srcDirPath = srcDir.value();
    }

    if (dstDir) {
        dstDirPath = dstDir.value();
    }

    frame->Show();
    return true;
}

void DMirror::OnStartClicked(wxCommandEvent& event)
{
    if (srcDirPath.IsEmpty() || dstDirPath.IsEmpty()) {
        wxMessageBox(
            "Please specify a source and destination directory before starting the copy operation.",
            "Warning: Directory not specified.",
            wxOK | wxICON_WARNING,
            frame
        );
        return;
    }

    if (srcDirPath == dstDirPath) {
        wxMessageBox(
            "Source and destionation directories cannot be the same. Please specify separate source and destination directory before starting the copy operation.",
            "Warning: Invalid directory.",
            wxOK | wxICON_WARNING,
            frame
        );
        return;
    }

    wxMessageBox(
        "Are you sure you want to copy the contents of the source directory to the destination directory?",
        "Confirm: Copy Operation",
        wxYES_NO | wxICON_INFORMATION,
        frame
    );

    // Store current source and destination paths
    config->update("src_dir", srcDirPath.ToStdString());
    config->update("dst_dir", dstDirPath.ToStdString());
    config->save(DMirror::GetConfigPath().string());

    StartCopy();
}

void DMirror::StartCopy()
{
    wxLogDebug("Starting copy operation from '%s' to '%s'", srcDirPath, dstDirPath);

    auto* progDialog = new ProgressDialog(frame);
    
    progDialog->Bind(wxEVT_SHOW, [&](wxShowEvent& event)
    {

        // Ignore the event if dialog is being hidden
        if (!event.IsShown()) {
            event.Skip();
            return;
        }
        
        std::string src = srcDirPath.ToStdString();
        std::string dst = dstDirPath.ToStdString();

        auto cancelled = progDialog->GetCancellationFlag();

        std::thread([this, progDialog, cancelled, src, dst]()
        {
            CopyResult result = dmirror_copy_dir(src, dst,
                [progDialog](int current, int total, int scanned)
                {
                    wxGetApp().CallAfter([progDialog, current, total, scanned]()
                    {
                        progDialog->SetRange(total);
                        progDialog->SetProgress(current);
                        progDialog->SetTotal(scanned);
                    });
                    wxLogDebug("Copy progress: %d/%d", current, total);
                },
                [cancelled]()
                {
                    return cancelled->load();
                }
            );

            switch(result)
            {
                case CopyResult::Success:
                    wxLogDebug("Successfully copied directory contents from '%s' to '%s'", src, dst);
                    wxMessageBox(
                        "The copy operation has completed successfully.",
                        "Copy Operation Completed",
                        wxOK | wxICON_WARNING,
                        frame
                    );
                    break;
                
                case CopyResult::Cancelled:
                    wxLogDebug("Cancelled copying directory contents from '%s' to '%s'", src, dst);
                    wxMessageBox(
                        "The copy operation has cancelled.",
                        "Information",
                        wxOK | wxICON_EXCLAMATION,
                        frame
                    );
                    break;
                
                case CopyResult::Failed:
                    wxLogDebug("Failed to copy directory contents from '%s' to '%s'", src, dst);
                    wxMessageBox(
                        "The copy operation has failed to copy all files. Check the console output for details.",
                        "Copy Operation Failed",
                        wxOK | wxICON_ERROR,
                        frame
                    );
                    break;
            }

            wxGetApp().CallAfter([progDialog]()
            {
                progDialog->EndModal(0);
            });

        }).detach();

        event.Skip();        
    });

    progDialog->ShowModal();
}

void DMirror::OnSourceDirChanged(wxFileDirPickerEvent& event)
{
    srcDirPath = event.GetPath();
    wxLogDebug("Source Directory changed to %s", srcDirPath.ToStdString());
}

void DMirror::OnDestinationDirChanged(wxFileDirPickerEvent& event)
{
    this->dstDirPath = event.GetPath();
    wxLogDebug("Destination Directory changed to %s", dstDirPath.ToStdString());
}

std::optional<std::string> DMirror::GetSavedSourceDir()
{
    return config->get("src_dir");
}

std::optional<std::string> DMirror::GetSavedDestinationDir()
{
    return config->get("dst_dir");
}

void DMirror::CreateAppDataDir()
{
    std::error_code ec;
    std::filesystem::create_directories(DMirror::GetAppDataPath(), ec);

    if (ec)
    {
        wxLogError("Failed to create app data directory: %s", ec.message());
    }
}

std::filesystem::path DMirror::GetAppDataPath()
{
    return std::filesystem::path(
        wxStandardPaths::Get().GetUserDataDir().ToStdString()
    );
}

std::filesystem::path DMirror::GetConfigPath()
{
    return DMirror::GetAppDataPath() / CONFIG_FILE;
}