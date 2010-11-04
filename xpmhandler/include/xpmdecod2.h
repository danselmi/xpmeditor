/***************************************************************************/
/** file    : include/wx/wxadv/image/xpmdecod2.h                          **/
/** purpose : wxXPMDecoder2                                               **/
/**           corrects a bug: when XPM contains comments, it is not       **/
/**           recognized as an image file.                                **/
/** created : 02/11/2010 (dd/mm/yyyy)                                     **/
/** modified:                                                             **/
/** authors : Vaclav Slavik, Robert Roebling, Sebastien DUBREUIL          **/
/** licence : wxWindows licence                                           **/
/***************************************************************************/

#ifndef _WX_XPMDECOD2_H_
#define _WX_XPMDECOD2_H_

#include <wx/defs.h>

#if wxUSE_IMAGE && wxUSE_XPM

class WXDLLIMPEXP_FWD_CORE wxImage;
class WXDLLIMPEXP_FWD_BASE wxInputStream;

// --------------------------------------------------------------------------
// wxXPMDecoder class
// --------------------------------------------------------------------------

class wxXPMDecoder2
{
public:
    // constructor, destructor, etc.
    wxXPMDecoder2() {}
    ~wxXPMDecoder2() {}

#if wxUSE_STREAMS
    // Is the stream XPM file?
    // NOTE: this function modifies the current stream position
    bool CanRead(wxInputStream& stream);

    // Read XPM file from the stream, parse it and create image from it
    wxImage ReadFile(wxInputStream& stream);
#endif

    // Read directly from XPM data (as passed to wxBitmap ctor):
    wxImage ReadData(const char* const* xpm_data);

#ifdef __BORLANDC__
    // needed for Borland 5.5
    wxImage ReadData(char** xpm_data)
        { return ReadData(const_cast<const char* const*>(xpm_data)); }
#endif
};

#endif // wxUSE_IMAGE && wxUSE_XPM

#endif  // _WX_XPMDECOD2_H_
