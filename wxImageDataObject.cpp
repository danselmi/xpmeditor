/***************************************************************
 * Plugin:    XPMEditor for Code::Blocks
 * Name:      wxImageDataObject.cpp
 * Purpose:   a class for storing a wxImage in the clipboard - code
 * Author:    Seb (seb_seb0@lycos.com)
 * Created:   2009-04-23
 * Copyright: Seb ()
 * License:   GPL 3.0
 **************************************************************/

#include <wx/wx.h>
#include "wxImageDataObject.h"

/** Constructor
  * \param image: the wxImage to copy in the clipboard
  */
wxImageDataObject::wxImageDataObject(const wxImage& image)
: wxDataObjectSimple(wxDF_INVALID)
{
    wxDataFormat format;

    format.SetId(_("wxImage"));
    SetFormat(format);
    m_Image = image;
}

/** Return the wxImage currently stored in the object
  * \return : the image
  */
wxImage wxImageDataObject::GetImage(void) const
{
    return(m_Image);
}

/** Set the image
  * \param image : the new image. The old data is deleted
  *                the image passed in parameters is copied
  */
void wxImageDataObject::SetImage(const wxImage& image)
{
    m_Image = image;
}

/** return the size in bytes of the image
  * \return the size of the object
  */
size_t wxImageDataObject::GetDataSize(void) const
{
    //each pixel is stored using 3 bytes (1 for Red, 1 for Green, 1 for Blus)
    //optionally an alpha channel can be used (1 byte / pixel)

    size_t sSize;
    sSize = sizeof(wxImage);

    if (!m_Image.IsOk()) return(0);
/*
    sSize = sSize + 3 * m_Image.GetWidth() * m_Image.GetHeight();
    if (m_Image.HasAlpha()) sSize = sSize + m_Image.GetWidth() * m_Image.GetHeight();
*/
    return(sSize);
}

/** copy the data in the buffer
  * \param buf : the buffer to which the data has to be copied
  *              buf is assumed to be a pointer to a wxImage
  * \return true on success, false on failure
  */
bool wxImageDataObject::GetDataHere(void *buf) const
{
    if (!buf) return(false);

    wxImage *image;
    image = (wxImage *) buf;

    *image = m_Image;
    return(true);

/*
    //initialize the image
    int iHeight, iWidth;
    iHeight = m_Image.GetHeight();
    iWidth = m_Image.GetWidth();
    image->Create(iWidth, iHeight, true);

    //copy the pixel data
    unsigned char *SrcData, *DestData;
    SrcData = m_Image.GetData();
    if (!SrcData) return(false);
    DestData = (unsigned char*) malloc(iWidth * iHeight * 3 * sizeof(unsigned char)); //3 bytes per pixels
    if (!DestData) return(false);
    memcpy(DestData, SrcData, iWidth * iHeight * 3 * sizeof(unsigned char));
    image->SetData(DestData);

    //copy the alpha channel data if needed
    unsigned char *SrcAlpha, *DestAlpha;
    if (m_Image.HasAlpha())
    {
        SrcAlpha = m_Image.GetAlpha();
        if (!SrcAlpha) return(false);
        DestAlpha = (unsigned char*) malloc(iWidth * iHeight * sizeof(unsigned char)); //1 byte per pixels
        if (!DestAlpha) return(false);
        memcpy(DestAlpha, SrcAlpha, iWidth * iHeight * sizeof(unsigned char));
        image->SetAlpha(DestAlpha);
    }

    return(true);
*/
}

/** Set the data in the object
  * \param len : the size of the buffer buf, in bytes
  * \param buf : the buffer containing the data to copy in the object
  *              the buffer is assumed to be a pointer to a wxImage
  * \return true on success, false on failure
  */
bool wxImageDataObject::SetData(size_t WXUNUSED(len), const void *buf)
{
    if (!buf) return(false);

    wxImage *image;
    image = (wxImage *) buf;

    m_Image = *image;
    return(true);
}

/** return the size in bytes of the image
  * \return the size of the object
  */
size_t wxImageDataObject::GetDataSize(const wxDataFormat& WXUNUSED(format)) const
{
    return(GetDataSize());
}

/** copy the data in the buffer
  * \param format: the format in which the data must be copied (ignored)
  * \param buf : the buffer to which the data has to be copied
  * \return true on success, false on failure
  */
bool wxImageDataObject::GetDataHere(const wxDataFormat& WXUNUSED(format),
                             void *buf) const
{
    return(GetDataHere(buf));
}

/** Set the data in the object
  * \param format: the format in which the data must be copied (ignored)
  * \param len : the size of the buffer buf, in bytes
  * \param buf : the buffer containing the data to copy in the object
  * \return true on success, false on failure
  */
bool wxImageDataObject::SetData(const wxDataFormat& WXUNUSED(format),
                         size_t len, const void *buf)
{
    return(SetData(len, buf));
}
