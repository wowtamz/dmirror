// app.h
#pragma once

#include <wx/wx.h>

class DMirror : public wxApp
{
    public:
        bool OnInit() override;
        void OnStartClicked(wxCommandEvent& event);
        void OnCancelClicked(wxCommandEvent& event);
    
    private:
        wxFrame* frame;
};

wxDECLARE_APP(DMirror);