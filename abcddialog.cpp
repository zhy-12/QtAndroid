#include "abcddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
ABCDdialog::ABCDdialog(QWidget *parent)
    : QDialog{parent}
    , checkBox_A(new QCheckBox("Line A",this))
    , checkBox_B(new QCheckBox("Line B",this))
    , checkBox_C(new QCheckBox("Line C",this))
    , checkBox_D(new QCheckBox("Line D",this))
    , checkBox_E(new QCheckBox("Line E",this))
    , checkBox_F(new QCheckBox("Line F",this))
    , checkBox_G(new QCheckBox("Line G",this))
    , checkBox_H(new QCheckBox("Line H",this))
    , applyButton(new QPushButton("Apply",this))
    , selectAllButton(new QPushButton("Select All",this))
{
    setStyleSheet("QcheckBox{border: none;color: white: }"
                  "QDialog{background-color: rgb(217,218,214);}"
                  "QCheckBox::indicator{Width: 25px;Height: 25px;}");
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
