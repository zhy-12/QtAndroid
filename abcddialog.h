#ifndef ABCDDIALOG_H
#define ABCDDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>

class ABCDdialog : public QDialog
{
    Q_OBJECT
public:
    explicit ABCDdialog(QWidget *parent = nullptr);
public:
    QCheckBox *checkBox_A;
    QCheckBox *checkBox_B;
    QCheckBox *checkBox_C;
    QCheckBox *checkBox_D;
    QCheckBox *checkBox_E;
    QCheckBox *checkBox_F;
    QCheckBox *checkBox_G;
    QCheckBox *checkBox_H;

    QPushButton *applyButton;
    QPushButton *selectAllButton;

signals:
public slots:
    void slot_selectAll();
};

#endif // ABCDDIALOG_H
