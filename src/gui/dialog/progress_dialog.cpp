#include <wx/wx.h>
#include <wx/gauge.h>

#include "progress_dialog.h"


ProgressDialog::ProgressDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "Progress", wxDefaultPosition, wxSize(400, 200))
{

    cancelled = std::make_shared<std::atomic<bool>>(false);

    auto* sizer = new wxBoxSizer(wxVERTICAL);

    progressLabel = new wxStaticText(this, wxID_ANY, "Copying 0/0");
    progressBar = new wxGauge(this, wxID_ANY, 100);
    auto* cancelButton = new wxButton(this, wxID_ANY, "Cancel");

    sizer->AddStretchSpacer(1);

    sizer->Add(progressLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    sizer->Add(progressBar, 0, wxEXPAND | wxALL, 5);
    sizer->Add(cancelButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    sizer->AddStretchSpacer(1);

    SetSizerAndFit(sizer);
    SetSize(400, 100);

    cancelButton->Bind(wxEVT_BUTTON, &ProgressDialog::OnCancelClicked, this);
}

void ProgressDialog::SetProgress(const int& progress)
{
    progressBar->SetValue(progress);
    progressLabel->SetLabel("Copying " + std::to_string(progress) + "/" + std::to_string(progressBar->GetRange()) + " files.");
}

void ProgressDialog::SetRange(const int& range)
{
    progressBar->SetRange(range);
}

void ProgressDialog::OnCancelClicked(wxCommandEvent& event)
{
    cancelled->store(true);
}

std::shared_ptr<std::atomic<bool>> ProgressDialog::GetCancellationFlag()
{
    return cancelled;
}