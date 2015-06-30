#include "crcbase.h"

CRCBase::CRCBase(QObject *parent) :
    QObject(parent),
    m_msb(true)
{
}

quint32 CRCBase::CRCTable(quint32 crc, unsigned char *data, int numb)
{
    return 0;
}

QString CRCBase::getCRCSumStr(quint32) const
{
    return QString("");
}

void CRCBase::setMsb(bool msbFirst)
{
    this->m_msb = msbFirst;
}

bool CRCBase::isMsbFirst() const
{
    return this->m_msb;
}

void CRCBase::printCRCTable()
{

}

QString CRCBase::getPolyStr(quint32 poly, int len)
{
    QString poly_s;
    quint32 filter = 0x01;
    bool bit;
    int i;
    for(i=0; i < len; i++){
        bit = poly & filter;
        if(bit){
            if(i == 0){
                poly_s = QString("1");
            }
            else{
                poly_s = QString("x^%1 + ").arg(i) + poly_s;
            }
        }
        filter <<= 1;
    }
    poly_s = QString("x^%1 + ").arg(i) + poly_s;
    return poly_s;
}
