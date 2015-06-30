#ifndef CRC32_H
#define CRC32_H

#include "crcbase.h"

class CRC32 : public CRCBase
{
public:
    explicit CRC32();
    CRC32(quint32 poly, quint32 seed);
    QString algorithmName() const;
    quint32 CRCSum(unsigned char *data, int numb);
    quint32 CRCSum(unsigned char *data, int numb, quint32 poly, quint32 seed);
    quint32 CRCTable(quint32 crc, unsigned char *data, int numb);
    quint32 getCRCSum() const;
    QString getCRCSumStr(quint32 crc_sum) const;
    void setPoly(quint32 poly);
    void setSeed(quint32 seed);

    void printCRCTable();

private:
    quint32 CRCByte(quint32 crc, unsigned char data);

private:
    quint32 m_crc_poly;
    quint32 m_crc_seed;
    quint32 m_crc_resu;
};

#endif // CRC32_H
