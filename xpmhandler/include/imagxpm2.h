/***************************************************************************/
/** file    : include/wx/wxadv/image/imagxpm2.h                           **/
/** purpose : wxXPMHandler                                                **/
/**           corrects a bug: when XPM contains comments, it is not       **/
/**           recognized as an image file.                                **/
/** created : 02/11/2010 (dd/mm/yyyy)                                     **/
/** modified:                                                             **/
/** authors : Vaclav Slavik, Robert Roebling, Sebastien DUBREUIL          **/
/** licence : wxWindows licence                                           **/
/***************************************************************************/

#ifndef _WX_IMAGXPM2_H_
#define _WX_IMAGXPM2_H_

#include <wx/image.h>

#if wxUSE_XPM

/** \class wxXPMHandler2 : will read an image from an XPM file.
  * This class is improved from the wxWidgets one
  * If some comments are present in the XPM file, then these comments will be ignored
  */

class wxXPMHandler2 : public wxImageHandler
{
public:
    /** \brief constructor
      * Constructor. It does not take any parameters
      */
    inline wxXPMHandler2()
    {
        m_name = wxT("XPM file");
        m_extension = wxT("xpm");
        m_type = wxBITMAP_TYPE_XPM;
        m_mime = wxT("image/xpm2");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 ); ///< \brief Load an image saved in a file in XPM format
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true );              ///< \brief Save an image to a file in XPM format
protected:
    virtual bool DoCanRead( wxInputStream& stream );                                                 ///< \brief Test if the file is actually an image file in XPM format.
#endif

private:
    //DECLARE_DYNAMIC_CLASS(wxXPMHandler2)
};

#endif // wxUSE_XPM

#endif // _WX_IMAGXPM2_H_
