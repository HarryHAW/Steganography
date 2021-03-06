#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include "qimage.h"
class Steganography
{
public:
    Steganography(QString file);

    int insertText_1BitPerPixel(QString*,int);

    bool imageIsNull();

    QString* getHiddenText();

    bool saveImage(QString file);

    int insertBitstream(QString*);
    int insertBitstream(QList<uint>* list);
    int insertBitstream_3BitsPerPixel(QList<uint>* l);
    int insertBitstream_3BitsPerPixel(QString* s);
    int insertBitstream_6BitsPerPixel(QString* s);
    int insertBitstream_6BitsPerPixel(QList<uint>* s);


    QString* getBitStream();
    QList<uint>* getBitStreamAsIntList();
    QList<uint>* getBitstreamAsIntList_3BitsPerPixel();
    QList<uint>* getBitstreamAsIntList_6BitsPerPixel();
    QString* getBitstream_3BitsPerPixel();
    QString* getBitstream_6BitsPerPixel();

    int getIntFromHeader(int from);

    int getSizeFromHeader();
    int getFormatFromHeader();
    int getFirstAttributeFromHeader();
    int getSecondAttributeFromHeader();
    int getBitsPerPixelFromHeader();
    int getSequenceNoFromHeader();
    int getRealSizeFromHeader();

    int insertIntInHeader(int, int);

    int insertSizeInHeader(int);
    int insertTBFieldInHeader(int format);
    int insertFirstAttribute(int);
    int insertSecondAttribute(int);
    int insertBitsPerPixelInHeader(int bitsPerPixel);
    int insertSequenceNoInHeader(int sequenceNo);
    int insertRealSizeInHeader(int realSize);



    int insertPictureHeader(int, int, int); // rausnehmen??
    int insertTextHeader(int, int);         // rausnehmen??

    QChar lastBit(int i);

    int calcInkrement(int pixels, int bits, int bitsPerPixel);




private:
    QImage image;
};

#endif // STEGANOGRAPHY_H
