#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include "plotwidget.h"
#include "imagelabel.h"
#include "datatabel.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QStackedWidget>
#include <sampledialog.h>
#include <QTableWidget>

#define assertRectValid(x,y,w,h,imageW,ImageH) ((((x)>=0)&&((w)>=0)&&(((x)+(w))<(imageW))&&((y)>=0)&&((h)>=0)&&(((y)+(h))<(imageW))))
struct object_rect {
    int x;
    int y;
    int width;
    int height;
};
class MainWindow : public QMainWindow
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
    void slot_switchToImage();
    void slot_switchToChart();
    void slot_switchToTable();

private:
    void initDataTable();
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void updateLabelText(int x, QString str);
    void updateImage(cv::Mat frame);
    void process_Color(cv::Mat frame, std::vector<std::vector<cv::Point2f>>);
    void saveImage(cv::Mat temp_frame);
private:
    std::vector<std::vector<cv::Point2f>> center;
    QStackedWidget *stack;
    QLabel* myLabel;
    imageLabel* myImageLabel;
    QPushButton* myButton1;
    QPushButton* myButton2;
    QPushButton* chooseImageButton;
    QPushButton* btn_stackToImage;
    QPushButton* btn_stackToChart;
    QPushButton* btn_stackToTable;
    QVBoxLayout* mainLayout;
    QStringList filename;
    plotWidget *myPlot;
    QTableWidget *myTable;


    std::vector<std::vector<double>> average_B;
    std::vector<std::vector<double>> average_G;
    std::vector<std::vector<double>> average_R;
    std::vector<std::vector<double>> average_H;
    std::vector<std::vector<double>> average_S;
    std::vector<std::vector<double>> average_V;
    std::vector<std::vector<double>> textBuffer;
    std::vector<std::vector<double>> textH2O2;
    std::vector<std::vector<double>> textGlucose;
    std::vector<std::vector<double>> textAbsorbance;
    std::vector<std::vector<bool>> mask;
    cv::Mat text_frame;
    cv::Mat frameBtn1;
    cv::Mat frameBtn2;
    std::vector<double> cX;
    std::vector<double> cY;
    bool processedBtn[4];
    std::vector<QPointF> points1;
    int index1;
    int index2;
    int index3;
    sampleType m_sampleType;
};
#endif // MAINWINDOW_H
