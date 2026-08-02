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
}

void DMirror::OnCancelClicked(wxCommandEvent& event)
{
    std::cout << "Cancel button clicked!" << std::endl;
}