// main_frame.h
#pragma once

class MainFrame : public wxFrame
{
    public:
        MainFrame();

    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        wxButton* m_button;
};