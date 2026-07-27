#include <wx/wx.h>
#include <wx/arrstr.h>

#include "folder_list.h"

FolderList::FolderList(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name)
    : wxListCtrl(parent, id, pos, size, style, validator, name)
{
    // Set up the list control here (e.g., add columns, set styles)
    this->InsertColumn(0, "Id");
    this->InsertColumn(1, "Folder Path");

}