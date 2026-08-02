// main_frame.h
#pragma once

class MainFrame : public wxFrame
{
    public:
        MainFrame();

    private:
        void OnStartClicked(wxCommandEvent& event);
        void OnCancelClicked(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        wxButton* m_button;
};