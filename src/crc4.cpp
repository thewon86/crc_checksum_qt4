#include "crc4.h"
#include "mtypesdef.h"

/** CRC table for the CRC-4. The poly is 0x3 (x^4 + x^1 + 1) */
unsigned char const crc4_table[16] = {
    0x00, 0x0D, 0x03, 0x0E, 0x06, 0x0B, 0x05, 0x08,
    0x0C, 0x01, 0x0F, 0x02, 0x0A, 0x07, 0x09, 0x04
};

CRC4::CRC4() :
    m_crc_poly(0x03),
    m_crc_seed(0x0),
    m_crc_resu(0x0)
{
}

CRC4::CRC4(quint8 poly, quint8 seed)
{
    this->setPoly(poly);
    this->setSeed(seed);
}

QString CRC4::algorithmName() const
{
    return QString("CRC-4");
}

quint32 CRC4::CRCSum(unsigned char *data, int numb)
{
    bool lsb;
    unsigned char crc = m_crc_seed;
    unsigned char crc_poly = 0;

    if(isMsbFirst()){
        crc_poly = m_crc_poly << 4;
        crc <<= 4;
    }
    else{
        quint8 filter = 0x01;
        bool bit;
        for(int i=0; i < 4; i++){
            bit = m_crc_poly & filter;
            crc_poly <<= 1;
            crc_poly |= bit;
            filter <<= 1;
        }
    }
//    MDEBUGHEX(crc);MDEBUGHEX(crc_poly);
    for(int i=0; i< numb; i++){
        crc ^= *(data + i);
        for(int j=0;j<8;j++){
            if(isMsbFirst()){
                lsb = ((crc & 0x80) == 0x80);
                crc = crc << 1;
            }
            else{
                lsb = ((crc & 0x01) == 0x01);
                crc = crc >> 1;
            }
            if(lsb){
                crc ^= crc_poly;
            }
        }
    }

    if(isMsbFirst()){
        crc >>= 4;
    }

    m_crc_resu = crc;

    return this->getCRCSum();
}

quint32 CRC4::CRCSum(unsigned char *data, int numb, quint32 poly, quint32 seed)
{
    this->setPoly(poly);
    this->setSeed(seed);
    return this->CRCSum(data, numb);
}

quint32 CRC4::getCRCSum() const
{
    return this->m_crc_resu;
}

QString CRC4::getCRCSumStr(quint32 crc_sum) const
{
    QString crc_s;
    crc_s.setNum(crc_sum, 16);

    while(crc_s.length() != 2){
        crc_s.insert(0, QString("0"));
    }
    return crc_s.toUpper();
}

void CRC4::setPoly(quint32 poly)
{
    this->m_crc_poly = poly & 0xF;
}

void CRC4::setSeed(quint32 seed)
{
    this->m_crc_seed = seed & 0xF;
}

quint32 CRC4::CRCTable(quint32 crc, unsigned char *data, int numb)
{
    while (numb--)
        crc = CRCByte(crc, *data++);
    return crc;
}

quint32 CRC4::CRCByte(quint32 crc, unsigned char data)
{
    quint32 i;
    for(i = 0;i < 2;i ++)
    {
        crc = crc4_table[(crc ^ (data & 0x0F)) & 0x0F];
        data >>= 4;
    }
    return crc;
}

void CRC4::printCRCTable()
{
    int i,j;
    QFile table_f;
    QTextStream table;
    QString table_name;
    QString poly_s;

    table_name = this->algorithmName().remove(QChar('-')) + QString("_table");
    table_f.setFileName(table_name + QString(".h"));
    table_f.open(QFile::WriteOnly);
    table.setDevice(&table_f);

    poly_s = getPolyStr(m_crc_poly, 4);
    table << "/** " << "CRC table for the " << this->algorithmName() << ". "
          << "The poly is 0x" << QString::number(m_crc_poly, 16)
          << " (" << poly_s << ") " << "*/\n";

    table << "unsigned char const " << table_name.toLower() << "[16] = {";
    for(i=0; i < 16;i++){
        j = i << 4;
        this->CRCSum((unsigned char *)(&j), 1);
//        this->CRCTable(m_crc_seed, (unsigned char *)(&j), 1);

        if(i%8 == 0){
            table << "\n\t";
        }
        table << "0x" << this->getCRCSumStr(m_crc_resu).remove(QChar(' '));
        if(i != 15){
            table << ", ";
        }
    }
    table << "\n};\n";

    table_f.close();
}
