#include "sampledialog.h"
#include <QVBoxLayout>
#include <QScreen>
sampleDialog::sampleDialog(QWidget *parent)
    : QDialog{parent}
    , h2o2Btn(new QPushButton("H₂O₂",this))
    , glucoseBtn(new QPushButton("glucose",this))

{
    h2o2Btn->resize(screen()->size().width()/4,screen()->size().width()/8);
    glucoseBtn->resize(h2o2Btn->size());
    m_sampleType = sampleType_non;
    auto layout = new QVBoxLayout(this);

    layout->addWidget(h2o2Btn);
    layout->addWidget(glucoseBtn);
    setStyleSheet("QDialog{background-color:rgb(125,127,131)}");
    connect(h2o2Btn,SIGNAL(clicked(bool)),this,SLOT(slot_processH2O2()));
    connect(glucoseBtn,SIGNAL(clicked(bool)),this,SLOT(slot_processGlucose()));
}
void sampleDialog::slot_processH2O2()
{
    m_sampleType = sampleType_h202;
    accept();
}
void sampleDialog::slot_processGlucose()
{
    m_sampleType = sampleType_glucose;
    accept();
}
