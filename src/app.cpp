#include "app.h"
#include "gui/main_frame.h"
#include "gui/dialog/progress_dialog.h"
#include "lib/dmirror.h"

#include <thread>
#include <chrono>

bool DMirror::OnInit()
{
    wxInitAllImageHandlers();

    frame = new MainFrame();

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
    std::cout << "Start button clicked!" << std::endl;
    StartCopy();
}

void DMirror::StartCopy()
{
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
                [progDialog](int current, int total)
                {
                    wxGetApp().CallAfter([progDialog, current, total]()
                    {
                        progDialog->SetRange(total);
                        progDialog->SetProgress(current);
                    });
                    std::cout << "Progress: " << current << "/" << total << std::endl;
                },
                [cancelled]()
                {
                    return cancelled->load();
                }
            );

            switch(result)
            {
                case CopyResult::Success:
                    std::cout << "Successfully copied directory contents from " << srcDirPath << " to " << dstDirPath << std::endl;
                        wxMessageBox(
                            "The copy operation has completed successfully.",
                            "Copy Operation Completed",
                            wxOK | wxICON_WARNING,
                            frame
                    );
                    break;
                
                case CopyResult::Cancelled:
                    std::cout << "Cancelled copying directory contents from " << srcDirPath << " to " << dstDirPath << std::endl;
                    wxMessageBox(
                        "The copy operation has cancelled.",
                        "Information",
                        wxOK | wxICON_EXCLAMATION,
                        frame
                    );
                    break;
                
                case CopyResult::Failed:
                    std::cerr << "Failed to copy directory contents from " << srcDirPath << " to " << dstDirPath << std::endl;
                    wxMessageBox(
                        "The copy operation has failed to copy all files. Check the console output for details.",
                        "Copy Operation Failed",
                        wxOK | wxICON_EXCLAMATION,
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
    std::cout << "Source Directory changed to " << srcDirPath.ToUTF8().data() << std::endl;
}

void DMirror::OnDestinationDirChanged(wxFileDirPickerEvent& event)
{
    this->dstDirPath = event.GetPath();
    std::cout << "Destination Directory changed to " << dstDirPath.ToUTF8().data() << std::endl;
}