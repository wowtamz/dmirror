#include <wx/wx.h>
#include <wx/mstream.h>

#include "image.h"

wxBitmap LoadBitmapFromResource(const unsigned char* data, unsigned int size)
{
    wxMemoryInputStream stream(data, size);
    wxImage image(stream, wxBITMAP_TYPE_PNG);

    if (image.IsOk())
    {
        return wxBitmap(image);
    }
    else
    {
        return wxNullBitmap;
    }
}