#include "abcddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
ABCDdialog::ABCDdialog(QWidget *parent)
    : QDialog{parent}
    , checkBox_A(new QCheckBox("A",this))
    , checkBox_B(new QCheckBox("B",this))
    , checkBox_C(new QCheckBox("C",this))
    , checkBox_D(new QCheckBox("D",this))
    , checkBox_E(new QCheckBox("E",this))
    , checkBox_F(new QCheckBox("F",this))
    , checkBox_G(new QCheckBox("G",this))
    , checkBox_H(new QCheckBox("H",this))
    , applyButton(new QPushButton("Apply",this))
    , selectAllButton(new QPushButton("Select All",this))
{
    setStyleSheet("QcheckBox::indicator{width:20px;height:20px}");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    layout->addWidget(checkBox_A);
    layout->addWidget(checkBox_B);
    layout->addWidget(checkBox_C);
    layout->addWidget(checkBox_D);
    layout->addWidget(checkBox_E);
    layout->addWidget(checkBox_F);
    layout->addWidget(checkBox_G);
    layout->addWidget(checkBox_H);
    btnLayout->addWidget(selectAllButton);
    btnLayout->addWidget(applyButton);
    layout->addLayout(btnLayout);
    connect(applyButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(selectAllButton,SIGNAL(clicked(bool)),this,SLOT(slot_selectAll()));
}
void ABCDdialog::slot_selectAll()
{
    checkBox_A->setChecked(true);
    checkBox_B->setChecked(true);
    checkBox_C->setChecked(true);
    checkBox_D->setChecked(true);
    checkBox_E->setChecked(true);
    checkBox_F->setChecked(true);
    checkBox_G->setChecked(true);
    checkBox_H->setChecked(true);
}
