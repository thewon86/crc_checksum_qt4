#ifndef CRC16_H
#define CRC16_H

#include "crcbase.h"

class CRC16 : public CRCBase
{
    Q_OBJECT
public:
    explicit CRC16();
    CRC16(quint16 poly, quint16 seed);
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
    quint16 m_crc_poly;
    quint16 m_crc_seed;
    quint16 m_crc_resu;
};

#endif // CRC16_H
