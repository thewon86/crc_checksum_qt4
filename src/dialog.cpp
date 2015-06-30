#include "dialog.h"
#include "ui_dialog.h"
#include "mtypesdef.h"
#include <QString>
#include "crcbase.h"
#include "crc4.h"
#include "crc7.h"
#include "crc8.h"
#include "crc12.h"
#include "crc16.h"
#include "crc24.h"
#include "crc32.h"

unsigned short crc16(unsigned short crc, unsigned char const *buffer, size_t len);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_curr_algo(NULL)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowIcon(QIcon(":/icon"));

    QString algo("CRC-4;CRC-7;CRC-8;CRC-12;CRC-16;CRC-24;CRC-32");
    ui->comboBox_algos->addItems(algo.split(";"));
    ui->lineEdit_srcdat->setText(QString("01 03 02 13 88"));

    change_algo(QString("CRC-16"));

    connect(ui->comboBox_algos, SIGNAL(currentIndexChanged(QString)), this, SLOT(change_algo(QString)));
    connect(ui->lineEdit_srcdat, SIGNAL(textEdited(QString)), this, SLOT(check_srcdata_format(QString)));
    connect(ui->checkBox_msb, SIGNAL(stateChanged(int)), this, SLOT(change_msb_text(int)));

    ui->comboBox_algos->setCurrentIndex(4);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::change_algo(const QString &algo)
{
    if(m_curr_algo != NULL){
        if(m_curr_algo->algorithmName() == algo){
            return;
        }
        delete m_curr_algo;
        m_curr_algo = NULL;
    }

    QString poly, seed;
    ui->comboBox_poly->clear();
    ui->comboBox_seed->clear();
    ui->comboBox_xor->clear();
    if(algo == QString("CRC-4")){
        m_curr_algo = new CRC4;
        poly = QString("03");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("0;F");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else if(algo == QString("CRC-7")){
        m_curr_algo = new CRC7;
        poly = QString("09");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("00;7F");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else if(algo == QString("CRC-8")){
        m_curr_algo = new CRC8;
        poly = QString("07;31");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("00;FF");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else if(algo == QString("CRC-12")){
        m_curr_algo = new CRC12;
        poly = QString("80F");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("000;FFF");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else if(algo == QString("CRC-16")){
        m_curr_algo = new CRC16;
        poly = QString("8005;1201;0589;3D65");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("0000;FFFF");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else if(algo == QString("CRC-24")){
        m_curr_algo = new CRC24;
        poly = QString("864CFB");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("000000;FFFFFF");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else if(algo == QString("CRC-32")){
        m_curr_algo = new CRC32;
        poly = QString("04C11DB7;1EDC6F41;A833982B");
        ui->comboBox_poly->addItems(poly.split(";"));
        seed = QString("00000000;FFFFFFFF");
        ui->comboBox_seed->addItems(seed.split(";"));
        ui->comboBox_xor->addItems(seed.split((";")));
    }
    else{
        return;
    }

    ui->checkBox_msb->setChecked(m_curr_algo->isMsbFirst());
}

void Dialog::on_pushButton_calc_clicked()
{
    QString poly_s, src_s, res_s;
    QStringList src_l;
    unsigned char src_dat[1024];
    int numb = 0;
    bool ok = 0;

    if(ui->comboBox_poly->currentText().isEmpty() || ui->lineEdit_srcdat->text().isEmpty()){
        return;
    }
    poly_s = ui->comboBox_poly->currentText().toUpper();
    src_s = ui->lineEdit_srcdat->text();

    src_l = src_s.split(QString(" "));
    foreach (QString ii, src_l) {
        int ret = ii.toUInt(&ok, 16);
        if(ok != 0 && numb < 1024){
            src_dat[numb] = ret;
            numb ++;
        }
    }

    m_curr_algo->setMsb(ui->checkBox_msb->isChecked());
    m_curr_algo->setPoly(ui->comboBox_poly->currentText().toUInt(&ok, 16));
    m_curr_algo->setSeed(ui->comboBox_seed->currentText().toUInt(&ok, 16));

    quint32 crcsum;
    crcsum = m_curr_algo->CRCSum(src_dat, numb);
//    MDEBUGHEX(crcsum);
//    crcsum = m_curr_algo->CRCTable(0, src_dat, numb);
//    MDEBUGHEX(crcsum);
//    return;

    if(ui->checkBox_xor->isChecked()){
        crcsum ^= ui->comboBox_xor->currentText().toUInt(&ok, 16);
    }

    res_s = m_curr_algo->getCRCSumStr(crcsum);
    ui->lineEdit_result->setText(res_s);
    res_s = ui->lineEdit_srcdat->text() + QString(" ") + res_s;

    if(ui->radioButton_hex->isChecked()){
        res_s.replace(QString(" "), QString(" 0x"));
        res_s = QString("0x") + res_s;
    }
    else if(ui->radioButton_asc->isChecked()){
        res_s.replace(QString(" "), QString("\\x"));
        res_s = QString("\"\\x") + res_s + QString("\"");
    }
    ui->lineEdit_dstdat->setText(res_s);
}

void Dialog::check_srcdata_format(const QString &str)
{
    QString src_s = str.toUpper();
    if(src_s.isEmpty()){
        return;
    }
    int len = src_s.length();
    QChar last_ch = src_s.at(len - 1);
    if(!((('0' <= last_ch) && (last_ch <= '9')) || (('A' <= last_ch) && (last_ch <= 'F')))){
        src_s.remove(len - 1, 1);
        ui->lineEdit_srcdat->setText(src_s);
        return;
    }
    if(len > 2){
        if(!((src_s.at(len - 2) == ' ') || (src_s.at(len - 3) == ' '))){
            src_s.insert(len - 1, ' ');
        }
    }
//    MDEBUG(src_s);
    ui->lineEdit_srcdat->setText(src_s.toUpper());
}

void Dialog::change_msb_text(int checked)
{
    m_curr_algo->setMsb(checked);
    if(checked){
        ui->checkBox_msb->setText(QString("MSB First"));
    }
    else{
        ui->checkBox_msb->setText(QString("LSB First"));
    }
}

void Dialog::on_pushButton_printcrctable_clicked()
{
    m_curr_algo->printCRCTable();
}
