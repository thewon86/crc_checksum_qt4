#include "crc12.h"

/** CRC table for the CRC-12. The poly is 0x80f (x^12 + x^11 + x^3 + x^2 + x^1 + 1) */
unsigned short const crc12_table[256] = {
    0x0000, 0x080F, 0x0811, 0x001E, 0x082D, 0x0022, 0x003C, 0x0833,
    0x0855, 0x005A, 0x0044, 0x084B, 0x0078, 0x0877, 0x0869, 0x0066,
    0x08A5, 0x00AA, 0x00B4, 0x08BB, 0x0088, 0x0887, 0x0899, 0x0096,
    0x00F0, 0x08FF, 0x08E1, 0x00EE, 0x08DD, 0x00D2, 0x00CC, 0x08C3,
    0x0945, 0x014A, 0x0154, 0x095B, 0x0168, 0x0967, 0x0979, 0x0176,
    0x0110, 0x091F, 0x0901, 0x010E, 0x093D, 0x0132, 0x012C, 0x0923,
    0x01E0, 0x09EF, 0x09F1, 0x01FE, 0x09CD, 0x01C2, 0x01DC, 0x09D3,
    0x09B5, 0x01BA, 0x01A4, 0x09AB, 0x0198, 0x0997, 0x0989, 0x0186,
    0x0A85, 0x028A, 0x0294, 0x0A9B, 0x02A8, 0x0AA7, 0x0AB9, 0x02B6,
    0x02D0, 0x0ADF, 0x0AC1, 0x02CE, 0x0AFD, 0x02F2, 0x02EC, 0x0AE3,
    0x0220, 0x0A2F, 0x0A31, 0x023E, 0x0A0D, 0x0202, 0x021C, 0x0A13,
    0x0A75, 0x027A, 0x0264, 0x0A6B, 0x0258, 0x0A57, 0x0A49, 0x0246,
    0x03C0, 0x0BCF, 0x0BD1, 0x03DE, 0x0BED, 0x03E2, 0x03FC, 0x0BF3,
    0x0B95, 0x039A, 0x0384, 0x0B8B, 0x03B8, 0x0BB7, 0x0BA9, 0x03A6,
    0x0B65, 0x036A, 0x0374, 0x0B7B, 0x0348, 0x0B47, 0x0B59, 0x0356,
    0x0330, 0x0B3F, 0x0B21, 0x032E, 0x0B1D, 0x0312, 0x030C, 0x0B03,
    0x0D05, 0x050A, 0x0514, 0x0D1B, 0x0528, 0x0D27, 0x0D39, 0x0536,
    0x0550, 0x0D5F, 0x0D41, 0x054E, 0x0D7D, 0x0572, 0x056C, 0x0D63,
    0x05A0, 0x0DAF, 0x0DB1, 0x05BE, 0x0D8D, 0x0582, 0x059C, 0x0D93,
    0x0DF5, 0x05FA, 0x05E4, 0x0DEB, 0x05D8, 0x0DD7, 0x0DC9, 0x05C6,
    0x0440, 0x0C4F, 0x0C51, 0x045E, 0x0C6D, 0x0462, 0x047C, 0x0C73,
    0x0C15, 0x041A, 0x0404, 0x0C0B, 0x0438, 0x0C37, 0x0C29, 0x0426,
    0x0CE5, 0x04EA, 0x04F4, 0x0CFB, 0x04C8, 0x0CC7, 0x0CD9, 0x04D6,
    0x04B0, 0x0CBF, 0x0CA1, 0x04AE, 0x0C9D, 0x0492, 0x048C, 0x0C83,
    0x0780, 0x0F8F, 0x0F91, 0x079E, 0x0FAD, 0x07A2, 0x07BC, 0x0FB3,
    0x0FD5, 0x07DA, 0x07C4, 0x0FCB, 0x07F8, 0x0FF7, 0x0FE9, 0x07E6,
    0x0F25, 0x072A, 0x0734, 0x0F3B, 0x0708, 0x0F07, 0x0F19, 0x0716,
    0x0770, 0x0F7F, 0x0F61, 0x076E, 0x0F5D, 0x0752, 0x074C, 0x0F43,
    0x0EC5, 0x06CA, 0x06D4, 0x0EDB, 0x06E8, 0x0EE7, 0x0EF9, 0x06F6,
    0x0690, 0x0E9F, 0x0E81, 0x068E, 0x0EBD, 0x06B2, 0x06AC, 0x0EA3,
    0x0660, 0x0E6F, 0x0E71, 0x067E, 0x0E4D, 0x0642, 0x065C, 0x0E53,
    0x0E35, 0x063A, 0x0624, 0x0E2B, 0x0618, 0x0E17, 0x0E09, 0x0606
};

CRC12::CRC12() :
    m_crc_poly(0x80F),
    m_crc_seed(0x0),
    m_crc_resu(0x0)
{
}

CRC12::CRC12(quint16 poly, quint16 seed)
{
    this->setPoly(poly);
    this->setSeed(seed);
}

QString CRC12::algorithmName() const
{
    return QString("CRC-12");
}

quint32 CRC12::CRCSum(unsigned char *data, int numb)
{
    bool lsb;
    unsigned short crc = m_crc_seed;
    unsigned short crc_poly = 0;

    if(isMsbFirst()){
        crc_poly = m_crc_poly << 4;
        crc <<= 4;
    }
    else{
        unsigned short filter = 0x0001;
        bool bit;
        for(int i=0; i < 12; i++){
            bit = m_crc_poly & filter;
            crc_poly <<= 1;
            crc_poly |= bit;
            filter <<= 1;
        }
    }

    for(int i=0; i< numb; i++){
        if(isMsbFirst()){
            crc ^= *(data + i) << 8;
        }
        else{
            crc ^= *(data + i);
        }
        for(int j=0;j<8;j++){
            if(isMsbFirst()){
                lsb = ((crc & 0x8000) == 0x8000);
                crc = crc << 1;
            }
            else{
                lsb = ((crc & 0x0001) == 0x0001);
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

quint32 CRC12::CRCSum(unsigned char *data, int numb, quint32 poly, quint32 seed)
{
    this->setPoly(poly);
    this->setSeed(seed);
    return this->CRCSum(data, numb);
}

quint32 CRC12::getCRCSum() const
{
    return this->m_crc_resu;
}

QString CRC12::getCRCSumStr(quint32 crc_sum) const
{
    QString crc_s;
    crc_s.setNum(crc_sum, 16);

    while(crc_s.length() != 4){
        crc_s.insert(0, QString("0"));
    }
    crc_s.insert((crc_s.length() - 2), QString(" "));
    return crc_s.toUpper();
}

void CRC12::setPoly(quint32 poly)
{
    this->m_crc_poly = poly & 0xFFF;
}

void CRC12::setSeed(quint32 seed)
{
    this->m_crc_seed = seed & 0xFFF;
}

quint32 CRC12::CRCTable(quint32 crc, unsigned char *data, int numb)
{
    while (numb--)
        crc = CRCByte(crc, *data++);
    return crc;
}

quint32 CRC12::CRCByte(quint32 crc, unsigned char data)
{
    return (crc >> 8) ^ ::crc12_table[(crc ^ data) & 0xff];
}

void CRC12::printCRCTable()
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

    poly_s = getPolyStr(m_crc_poly, 12);
    table << "/** " << "CRC table for the " << this->algorithmName() << ". "
          << "The poly is 0x" << QString::number(m_crc_poly, 16)
          << " (" << poly_s << ") " << "*/\n";

    table << "unsigned short const " << table_name.toLower() << "[256] = {";
    for(i=0; i < 256;i++){
        this->CRCSum((unsigned char *)(&i), 1);
//        this->CRCTable(m_crc_seed, (unsigned char *)(&i), 1);

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
