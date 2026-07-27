// folder_list.h
#pragma once
#include <wx/listctrl.h>

class FolderList : public wxListCtrl
{
public:
    explicit FolderList(wxWindow *parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxLC_ICON, const wxValidator &validator=wxDefaultValidator, const wxString &name=wxListCtrlNameStr);

private:
    wxArrayString folders;
};