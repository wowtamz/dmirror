#include "app.h"
#include "gui/main_frame.h"
#include "lib/dmirror.h"

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
    bool success = dmirror_copy_dir(srcDirPath.ToUTF8().data(), dstDirPath.ToUTF8().data());

    if (!success) {
        std::cerr << "Failed to copy directory contents from " << srcDirPath << " to " << dstDirPath << std::endl;
        wxMessageBox(
            "The copy operation has failed to copy all files. Check the console output for details.",
            "Copy Operation Failed",
            wxOK | wxICON_EXCLAMATION,
            frame
        );
        return;
    } else {
        std::cout << "Successfully copied directory contents from " << srcDirPath << " to " << dstDirPath << std::endl;
    }

    wxMessageBox(
            "The copy operation has completed successfully.",
            "Copy Operation Completed",
            wxOK | wxICON_WARNING,
            frame
    );
}

void DMirror::OnCancelClicked(wxCommandEvent& event)
{
    std::cout << "Cancel button clicked!" << std::endl;
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