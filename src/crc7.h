#ifndef CRC7_H
#define CRC7_H

#include "crcbase.h"

class CRC7 : public CRCBase
{
    Q_OBJECT
public:
    explicit CRC7();
    CRC7(quint8 poly, quint8 seed);
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
    quint8 m_crc_poly;
    quint8 m_crc_seed;
    quint8 m_crc_resu;
};

#endif // CRC7_H
