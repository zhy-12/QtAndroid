#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include "plotwidget.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QStackedWidget>
#include <sampledialog.h>

#define assertRectValid(x,y,w,h,imageW,ImageH) ((((x>=0)&&(w>=0)&&(x+w<imageW)&&(y>=0)&&(h>=0)&&(y+h<imageW))>0)?true:false)
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


//    QArrayData findCenter();
public slots:
    void slot_chooseImage();
    void slot_processBtn1();
    void slot_processBtn2();
    void slot_processBtn3();
    void slot_processBtn4();
    void slot_switchToImage();
    void slot_switchToChart();

private:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void updateLabelText(int x);
    void updateImage(cv::Mat frame);
    void process_Color(cv::Mat frame, std::vector<double> cX, std::vector<double> cY);
private:
    std::vector<double> center_y;
    std::vector<double> center_x;

    QStackedWidget *stack;
    QLabel* myLabel;
    QLabel* myImageLabel;
    QPushButton* myButton1;
    QPushButton* myButton2;
    QPushButton* myButton3;
    QPushButton* myButton4;
    QPushButton* chooseImageButton;
    QPushButton* btn_stackToImage;
    QPushButton* btn_stackToChart;
    QVBoxLayout* mainLayout;
    QStringList filename;
    plotWidget *myPlot;


    std::vector<double> average_B;
    std::vector<double> average_G;
    std::vector<double> average_R;
    std::vector<double> average_H;
    std::vector<double> average_S;
    std::vector<double> average_V;
    std::vector<double> textBuffer;
    cv::Mat text_frame;
    cv::Mat frameBtn1;
    cv::Mat frameBtn2;
    cv::Mat frameBtn3;
    cv::Mat frameBtn4;
    std::vector<double> cX;
    std::vector<double> cY;
    bool processedBtn[4];
    std::vector<QPointF> points1;
    std::vector<QPointF> points2;
    int index1;
    int index2;
    sampleType m_sampleType;
};
#endif // MAINWINDOW_H
