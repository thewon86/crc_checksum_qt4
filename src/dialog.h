#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class CRCBase;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void change_algo(const QString &algo);
    void on_pushButton_calc_clicked();
    void check_srcdata_format(const QString &str);
    void change_msb_text(int checked);

    void on_pushButton_printcrctable_clicked();

private:
    Ui::Dialog *ui;
    CRCBase *m_curr_algo;
};

#endif // DIALOG_H
