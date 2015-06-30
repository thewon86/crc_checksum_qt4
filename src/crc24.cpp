#include "crc24.h"

CRC24::CRC24() :
    m_crc_poly(0x864CFB),
    m_crc_seed(0x0),
    m_crc_resu(0x0)
{
}

CRC24::CRC24(quint32 poly, quint32 seed)
{
    this->setPoly(poly);
    this->setSeed(seed);
}

QString CRC24::algorithmName() const
{
    return QString("CRC-24");
}

quint32 CRC24::CRCSum(unsigned char *data, int numb)
{
    bool lsb;
    quint32 crc = m_crc_seed;
    quint32 crc_poly = 0;

    if(isMsbFirst()){
        crc_poly = m_crc_poly << 8;
        crc <<= 8;
    }
    else{
        quint32 filter = 0x1;
        bool bit;
        for(int i=0; i < 24; i++){
            bit = m_crc_poly & filter;
            crc_poly <<= 1;
            crc_poly |= bit;
            filter <<= 1;
        }
    }

    for(int i=0; i< numb; i++){
        if(isMsbFirst()){
            crc ^= *(data + i) << 24;
        }
        else{
            crc ^= *(data + i);
        }
        for(int j=0;j<8;j++){
            if(isMsbFirst()){
                lsb = ((crc & 0x80000000) == 0x80000000);
                crc = crc << 1;
            }
            else{
                lsb = ((crc & 0x1) == 0x1);
                crc = crc >> 1;
            }
            if(lsb){
                crc ^= crc_poly;
            }
        }
    }

    if(isMsbFirst()){
        crc >>= 8;
    }

    m_crc_resu = crc;

    return this->getCRCSum();
}

quint32 CRC24::CRCSum(unsigned char *data, int numb, quint32 poly, quint32 seed)
{
    this->setPoly(poly);
    this->setSeed(seed);
    return this->CRCSum(data, numb);
}

quint32 CRC24::getCRCSum() const
{
    return this->m_crc_resu;
}

QString CRC24::getCRCSumStr(quint32 crc_sum) const
{
    QString crc_s;
    crc_s.setNum(m_crc_resu, 16);

    while(crc_s.length() != 6){
        crc_s.insert(0, QString("0"));
    }
    crc_s.insert((crc_s.length() - 4), QString(" "));
    crc_s.insert((crc_s.length() - 2), QString(" "));
    return crc_s.toUpper();
}

void CRC24::setPoly(quint32 poly)
{
    this->m_crc_poly = poly & 0xFFFFFF;
}

void CRC24::setSeed(quint32 seed)
{
    this->m_crc_seed = seed & 0xFFFFFF;
}

quint32 CRC24::CRCTable(quint32 crc, unsigned char *data, int numb)
{
    while (numb--)
        crc = CRCByte(crc, *data++);
    return crc;
}

quint32 CRC24::CRCByte(quint32 crc, unsigned char data)
{
//    return (crc >> 8) ^ ::crc24_table[(crc ^ data) & 0xff];
}

void CRC24::printCRCTable()
{
    int i;
     QFile table_f;
     QTextStream table;
     QString table_name;
     QString poly_s;

     table_name = this->algorithmName().remove(QChar('-')) + QString("_table");
     table_f.setFileName(table_name + QString(".h"));
     table_f.open(QFile::WriteOnly);
     table.setDevice(&table_f);

     poly_s = getPolyStr(m_crc_poly, 24);
     table << "/** " << "CRC table for the " << this->algorithmName() << ". "
           << "The poly is 0x" << QString::number(m_crc_poly, 16)
           << " (" << poly_s << ") " << "*/\n";

     table << "unsigned int const " << table_name.toLower() << "[256] = {";
     for(i=0; i < 256;i++){
         this->CRCSum((unsigned char *)(&i), 1);
//         this->CRCTable(m_crc_seed, (unsigned char *)(&i), 1);

         if(i%8 == 0){
             table << "\n\t";
         }
         table << "0x" << this->getCRCSumStr(m_crc_resu).remove(QChar(' '));
         if(i != 255){
             table << ", ";
         }
     }
     table << "\n};\n";

     table_f.close();
}
