/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxImageDataObject.h
 * Purpose:   a class for storing a wxImage in the clipboard - definition
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include <wx/image.h>
#include <wx/dataobj.h>

class wxImageDataObject : public wxDataObjectSimple
{
    public:
        // constructors
        wxImageDataObject(const wxImage& image = wxImage()); ///< \brief constructor

        //set / get the wxImage
        virtual wxImage GetImage(void) const;        ///< \brief Get the image
        virtual void SetImage(const wxImage& image); ///< \brief Set the image

        // implement base class pure virtuals
        virtual size_t GetDataSize(void) const;      ///< \brief return the size in bytes of the image
        virtual bool GetDataHere(void *buf) const;   ///< \brief copy the data in the buffer
        virtual bool SetData(size_t len, const void *buf); ///< \brief Set the data in the object

        virtual size_t GetDataSize(const wxDataFormat& WXUNUSED(format)) const; ///< \brief return the size in bytes of the image
        virtual bool GetDataHere(const wxDataFormat& WXUNUSED(format),
                                 void *buf) const;                              ///< \brief copy the data in the buffer
        virtual bool SetData(const wxDataFormat& WXUNUSED(format),
                             size_t len, const void *buf);                      ///< \brief Set the data in the object

    private:
        wxImage m_Image; ///< \brief The image itself

    DECLARE_NO_COPY_CLASS(wxImageDataObject)
};
