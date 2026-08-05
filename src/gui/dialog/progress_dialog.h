// progress_dialog.h
#pragma once

#include <atomic>
#include <memory>

class ProgressDialog : public wxDialog
{
    public:
        explicit ProgressDialog(wxWindow* parent);
        void SetProgress(const int& progress);
        void SetRange(const int& range);
        void OnCancelClicked(wxCommandEvent& event);
        std::shared_ptr<std::atomic<bool>> GetCancellationFlag();
    
    private:
        wxGauge* progressBar;
        wxStaticText* progressLabel;
        std::shared_ptr<std::atomic<bool>> cancelled;
};