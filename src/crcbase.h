#ifndef CRCBASE_H
#define CRCBASE_H

#include <QObject>
#include <QFile>
#include <QChar>
#include <QTextStream>
#include "mtypesdef.h"

class CRCBase : public QObject
{
    Q_OBJECT
public:
    explicit CRCBase(QObject *parent = 0);
    virtual QString algorithmName() const=0;
    virtual quint32 CRCSum(unsigned char *data, int numb)=0;
    virtual quint32 CRCSum(unsigned char *data, int numb, quint32 poly, quint32 seed)=0;
    virtual quint32 CRCTable(quint32 crc, unsigned char *data, int numb);
    virtual quint32 getCRCSum() const=0;
    virtual QString getCRCSumStr(quint32) const;
    virtual void setPoly(quint32 poly)=0;
    virtual void setSeed(quint32 seed)=0;
    void setMsb(bool msbFirst);
    bool isMsbFirst() const;

    virtual void printCRCTable();
    QString getPolyStr(quint32 poly, int len);

signals:

public slots:

private:
    bool m_msb;
};
// CRC[X]=CRC[行,列]=CRC[行,0]^CRC[0,列]
#endif // CRCBASE_H
