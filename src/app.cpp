#include "app.h"
#include "gui/main_frame.h"

bool DMirror::OnInit()
{
    wxInitAllImageHandlers();

    auto* frame = new MainFrame();

    frame->Show();
    return true;
}