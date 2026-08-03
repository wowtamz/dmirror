#include "app.h"
#include "gui/main_frame.h"

bool DMirror::OnInit()
{
    wxInitAllImageHandlers();

    frame = new MainFrame();

    frame->Show();
    return true;
}

void DMirror::OnStartClicked(wxCommandEvent& event)
{
    wxMessageBox(
            "You clicked the button!",
            "Hello World!",
            wxOK | wxICON_INFORMATION,
            frame
    );
    std::cout << "Start button clicked!" << std::endl;
    StartCopy();
}

void DMirror::StartCopy()
{
    std::cout << "Starting copy from " << srcDirPath.ToUTF8().data() << " to " << dstDirPath.ToUTF8().data() << std::endl;
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