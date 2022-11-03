#ifndef SAMPLEDIALOG_H
#define SAMPLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
typedef enum {sampleType_h202=1,sampleType_glucose,sampleType_non}sampleType;
class sampleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit sampleDialog(QWidget *parent = nullptr);

public:
    QPushButton* h2o2Btn;
    QPushButton* glucoseBtn;
    sampleType m_sampleType;
public slots:
    void slot_processH2O2(void);
    void slot_processGlucose(void);

signals:

};

#endif // SAMPLEDIALOG_H
