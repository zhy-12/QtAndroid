#include "mainwindow.h"
#include "QHBoxLayout"
#include "QPixmap"
#include <QFileDialog>
#include <QGuiApplication>
#include <QScreen>
#include <QString>
#include <QBitmap>
#include <qtextformat.h>
#include <QTransform>
#include "sampledialog.h"
void printVector(std::vector<QPointF> vector);
void test1(std::vector<QPointF> &vector,int lo,int hi);//冒泡排序

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , stack(new QStackedWidget(this))
    , myLabel(new QLabel(this))
    , myImageLabel(new QLabel(this))
    , myButton1(new QPushButton("color",this))
    , myButton2(new QPushButton("Glucose",this))
    , myButton3(new QPushButton(this))
    , myButton4(new QPushButton("new color", this))
    , chooseImageButton(new QPushButton("Select",this))
    , btn_stackToImage(new QPushButton("Image",this))
    , btn_stackToChart(new QPushButton("Chart",this))
    , myPlot(new plotWidget(this,"chart"))
{
    m_sampleType = sampleType_non;
    myButton3->setText("H₂O₂");
    myLabel->setAlignment(Qt::AlignCenter);
    myLabel->setWordWrap(true);
    myLabel->setMargin(0);
    QFont ft;
    ft.setPointSize(10);
    myLabel->setFont(ft);
    updateLabelText(1);
    myImageLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    myPlot->setAixs("concentration",0,1,8,"RGB",0,255,6);
    myPlot->initChart();
    myPlot->hide();
//    myPlot->set
    index1 = stack->addWidget(myImageLabel);
    index2 = stack->addWidget(myPlot);
    stack->setCurrentWidget(myImageLabel);
    //stack->setContentsMargins(0,0,0,0);

    chooseImageButton->setObjectName("chooseImage");
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QHBoxLayout* stackBtnLayout = new QHBoxLayout();
    stackBtnLayout->addWidget(btn_stackToImage);
    stackBtnLayout->addWidget(btn_stackToChart);

    buttonLayout->addWidget(chooseImageButton);
    buttonLayout->addWidget(myButton1);
    buttonLayout->addWidget(myButton2);
    buttonLayout->addWidget(myButton3);
    buttonLayout->addWidget(myButton4);
    mainLayout->addWidget(myLabel);
    mainLayout->addLayout(stackBtnLayout);
    mainLayout->addWidget(stack);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);
    this->showFullScreen();
    this->setFixedSize(this->screen()->size());
    myButton1->connect(myButton1, SIGNAL(pressed()),this,SLOT(slot_processBtn1()));
    myButton2->connect(myButton2, SIGNAL(pressed()),this,SLOT(slot_processBtn2()));
    myButton3->connect(myButton3, SIGNAL(pressed()),this,SLOT(slot_processBtn3()));
    myButton4->connect(myButton4, SIGNAL(pressed()),this,SLOT(slot_processBtn4()));
    chooseImageButton->connect(chooseImageButton,SIGNAL(pressed()),this,SLOT(slot_chooseImage()));
    btn_stackToImage->connect(btn_stackToImage,SIGNAL(clicked()),this,SLOT(slot_switchToImage()));
    btn_stackToChart->connect(btn_stackToChart,SIGNAL(clicked()),this,SLOT(slot_switchToChart()));
}

MainWindow::~MainWindow(){}
void MainWindow::slot_switchToImage()
{
    myPlot->hide();
    myImageLabel->show();
    this->stack->setCurrentIndex(index1);
}
void MainWindow::slot_switchToChart()
{
    myPlot->show();
    myImageLabel->hide();
    this->stack->setCurrentIndex(index2);
}
void MainWindow::updateLabelText(int x)
{
    //QRect m_Rect = QGuiApplication::primaryScreen()->geometry();
    QSize size = this->stack->size();
    QString info;
    info.append("width:");
    info.append(QString::number(size.width()));
    info.append(" height:");
    info.append(QString::number(size.height()));
    info.append("current action:");
    switch (x)
    {
    case 1:
        info.append(myButton1->text());
        break;
    case 2:
        info.append(myButton2->text());
        break;
    case 3:
        info.append(myButton3->text());
        break;
    case 4:
        info.append(myButton4->text());
        break;
    default:
        break;
    }
    this->myLabel->setText(info);
}
void MainWindow::slot_chooseImage()
{
    int center_y[] = {320, 550, 770, 990, 1220, 1450, 1680, 1900,\
                    2125, 2350, 2570, 2800};
    int center_x[] = {270, 500, 700, 950, 1150, 1400, 1600, 1825};
    sampleDialog* dialog = new sampleDialog();
    if(dialog->exec())
    {
        m_sampleType = dialog->m_sampleType;
    }
    if(m_sampleType==sampleType_non)
        return;
//    QString sender1 = sender()->objectName();
    this->setFocus();
    QFileDialog* filechoose = new QFileDialog(this);
    filechoose->setWindowTitle("choose image");
    filechoose->setNameFilter(tr("File(*.jpg* *.png*)"));
    filechoose->setFileMode(QFileDialog::ExistingFile);
    if(filechoose->exec())
    {
        this->filename = filechoose->selectedFiles();
        processedBtn[0]=false;
        processedBtn[1]=false;
        processedBtn[2]=false;
        processedBtn[3]=false;
    }
    cX.clear();
    cY.clear();
    average_blue.clear();
    average_green.clear();
    average_red.clear();
    points1.clear();
    points2.clear();
//    myPlot->initChart();
//    myImageLabel->clear();
    myPlot->text->setText(" ");
    qDebug()<<"path:"<<filename[0];
    cv::Mat imgage_hsv;
    cv::Mat mask0;
    QImage img_temp(filename[0]);
    if(img_temp.width()<img_temp.height())
    {
        QTransform matrix;
        matrix.rotate(90);
        img_temp = img_temp.transformed(matrix);
    }
    img_temp = img_temp.convertToFormat(QImage::Format_BGR888);
    cv::Mat frame(img_temp.height(),img_temp.width(),CV_8UC3,(uchar*)img_temp.bits(),img_temp.bytesPerLine());
    cv::resize(frame,frame,cv::Size(4680,3456),0,0,cv::INTER_CUBIC);
    int rows = frame.rows;
    int colums = frame.cols;
    cv::cvtColor(frame,imgage_hsv,cv::COLOR_BGR2HSV);
    std::vector<int> lower_black = {0,0,0};
    std::vector<int> upper_black = {180, 255, 46};
    cv::inRange(imgage_hsv,lower_black,upper_black,mask0);
    int rows_m = int(rows/2);
    int colums_m = int(colums/2);
    int x_top = 0;
    int x_bottom = 0;
    int y_top = 0;
    int y_bottom = 0;
    for(int i=0;i<rows;++i){
        if(mask0.at<uchar>(rows_m-i,colums_m) == 255){
            x_top = rows_m-i;
            break;
        }
    }
    for(int i=0;i<rows;++i){
        if(mask0.at<uchar>(rows_m+i,colums_m) == 255){
            x_bottom = rows_m+i;
            break;
        }
    }
    for(int i=0;i<rows;++i){
        if(mask0.at<uchar>(rows_m,colums_m-i) == 255){
            y_top = colums_m-i;
            break;
        }
    }
    for(int i=0;i<rows;++i){
        if(mask0.at<uchar>(rows_m,colums_m+i) == 255){
            y_bottom = colums_m+i;
            break;
        }
    }
    frame = frame(cv::Rect(y_top,x_top,y_bottom-y_top,x_bottom-x_top));
    text_frame = frame;
    text_frame.copyTo(frameBtn1);
    text_frame.copyTo(frameBtn2);
    text_frame.copyTo(frameBtn3);
    text_frame.copyTo(frameBtn4);
    std::vector<int> lower_blue = {78, 30, 46};
    std::vector<int> upper_blue = {100, 255, 255};
    cv::cvtColor(frame,imgage_hsv,cv::COLOR_BGR2HSV);
    cv::inRange(imgage_hsv,lower_blue,upper_blue,mask0);
    cv::medianBlur(mask0,mask0,25);
    /////////////////////////////////////center///////////////////////////////////
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask0,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    auto begin = contours.begin();
    auto end = contours.end();
    double area = 0;
    cv::Moments M;
    while(begin!=end)
    {
        area = cv::contourArea(*begin);
        if(area > 12000)
        {
            qDebug()<<"contourArea:"<<area;
            M = cv::moments(*begin);
            cX.push_back(int(M.m10 / (M.m00+1)));
            cY.push_back(int(M.m01 / (M.m00+1)));
            qDebug()<<"M10:"<<M.m10<<"(cX,cY):["<<cX.back()<<","<<cY.back()<<"]";
        }
//        if(cX.back()>mask0.cols||cY.back()>mask0.rows)
//        {
//            cX.pop_back();
//            cY.pop_back()
//        }
        ++begin;
    }
    auto begin1 = cX.begin();
    auto begin1_1 = cY.begin();
    auto end1 = cX.end();
    cv::Mat cut_frame;
    cv::Scalar tempVal;
    while(begin1!=end1)
    {
        cut_frame = frame(cv::Rect(*begin1-5,*begin1_1-5,10,10));
        tempVal = cv::mean(cut_frame);
        average_blue.push_back(tempVal[0]);
        average_green.push_back(tempVal[1]);
        average_red.push_back(tempVal[2]);
        begin1++;
        begin1_1++;
    }
    this->myPlot->myLineSeries->clear();
    this->myPlot->myScatters->clear();
//    if(sender1 == "chooseImage")
//        emit this->myButton1->pressed();
}
void MainWindow::updateImage(cv::Mat frame)
{
    if(!frame.empty())
    {
        this->myImageLabel->hide();
        QImage img((const uchar*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        QPixmap pixImage = QPixmap::fromImage(img.rgbSwapped());
        pixImage = pixImage.scaled(this->stack->size(),Qt::KeepAspectRatio);
        myImageLabel->resize(pixImage.size());
        myImageLabel->setPixmap(pixImage);
        myImageLabel->show();
        update();
    }

}

void MainWindow::slot_processBtn1()
{
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    if(!processedBtn[0])
    {
        std::string text;
        std::vector<qreal> y_green;
        for(int i=0;i<int(average_green.size());i++)
        {
            text.append("red:");
            text.append(std::to_string((average_red[i])));
            text.append(",green:");
            text.append(std::to_string(average_green[i]));
            text.append(",blue:");
            text.append(std::to_string(average_blue[i]));
            cv::putText(frameBtn1,text,cv::Point(100,(i+1)*80),cv::FONT_HERSHEY_SIMPLEX,3,cv::Scalar(0,0,0),10);
            cv::circle(frameBtn1,cv::Point(cX[i],cY[i]),20,cv::Scalar(0,0,0),-1);
            text.clear();
        }
        processedBtn[0] = true;
    }
    updateLabelText(1);
    updateImage(frameBtn1);
}
void MainWindow::slot_processBtn2()
{
//    while(this->filename.isEmpty() == true&&this->text_frame.empty()==true){
//        while(this->filename.isEmpty() == true){
//            slot_chooseImage();
//        }
//    }
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    if(!processedBtn[1])
    {
        std::string text;
        std::vector<qreal> y_green;
        for(int i=0;i<int(average_green.size());i++)
        {
            if(average_green[i]<217.4)
                y_green.push_back(qreal((average_green[i]-217.4079)/(-25.3986)));
            else
                y_green.push_back(qreal(i/255*3));
            text = std::to_string(y_green.back());
            text = text.substr(0, text.find(".") + 2 + 1);
            points1.push_back(QPointF(qreal(y_green.back()),qreal(average_green[i])));
            cv::putText(frameBtn2,text,cv::Point(cX[i]-30,cY[i]),cv::FONT_HERSHEY_SIMPLEX,3,cv::Scalar(0,0,0),10);
        }
        processedBtn[1] = true;
    }
    myPlot->slot_updateChart(points1);
    if(screen()->size().width()>=screen()->size().height())
        myPlot->resize(screen()->size().width(),screen()->size().height());
    else
        myPlot->resize(screen()->size().width(),screen()->size().height()/2.2);
    myPlot->updateGeometry();
    updateLabelText(2);
    updateImage(frameBtn2);
}
void MainWindow::slot_processBtn3()
{
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    if(!processedBtn[2])
    {
        std::string text;
        std::vector<qreal> y_green;
        for(int i=0;i<int(average_green.size());i++)
        {
            if(average_green[i]<216)
                y_green.push_back(qreal((average_green[i]-224)/(-12)));
            else
                y_green.push_back(qreal(i/255*4));
            text = std::to_string(y_green.back());
            text = text.substr(0, text.find(".") + 2 + 1);
            points2.push_back(QPointF(qreal(y_green.back()),qreal(average_green[i])));
            cv::putText(frameBtn3,text,cv::Point(cX[i]-30,cY[i]),cv::FONT_HERSHEY_SIMPLEX,3,cv::Scalar(0,0,0),10);
        }
        processedBtn[2] = true;
    }
    myPlot->slot_updateChart(points2);
    if(screen()->size().width()>=screen()->size().height())
        myPlot->resize(stack->width(),stack->height());
    else
        myPlot->resize(stack->width(),stack->height()/2.2);
    myPlot->updateGeometry();
    updateLabelText(3);
    updateImage(frameBtn3);
}
void MainWindow::slot_processBtn4()
{
//    while(this->filename.isEmpty() == true&&this->text_frame.empty()==true){
//        while(this->filename.isEmpty() == true){
//            slot_chooseImage();
//        }
//    }
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    if(!processedBtn[3])
    {
        std::string text;
        std::vector<qreal> y_green;
        for(int i=0;i<int(average_green.size());i++)
        {
            y_green.push_back(qreal((average_red[i]+average_blue[i]+average_blue[i])));
            text = std::to_string(y_green.back());
            text = text.substr(0, text.find(".") + 2 + 1);
            cv::putText(frameBtn4,text,cv::Point(cX[i]-90,cY[i]),cv::FONT_HERSHEY_SIMPLEX,2,cv::Scalar(0,0,0),10);
        }
        processedBtn[2] = true;
    }
    updateLabelText(4);
    updateImage(frameBtn4);

}
void MainWindow::paintEvent(QPaintEvent* e)
{

    myPlot->resize(this->width(),myPlot->height());
    //this->update();
    //myImageLabel->resize(this->width(),myImageLabel->height());
//    myButton->resize(myButton->width(),myButton->height());
//    this->setLayout(mainLayout);
//    QWidget::paintEvent(e);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(myPlot->myChartView->scene())
    {
        myPlot->myChartView->scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        myPlot->myChart->resize(event->size());
    }
    updateImage(frameBtn1);


    updateLabelText(1);
    QWidget::resizeEvent(event);
}
//bool request_Android_Permission(const QString &str_permission)
//{
//#ifdef Q_OS_ANDROID
//    QtAndroid::PermissionResult r = QtAndroid::checkPermission(str_permission);
//    if(r == QtAndroid::PermissionResult::Denied)
//    {
//        QtAndroid::requestPermissionsSync( QStringList() << str_permission );
//        r = QtAndroid::checkPermission(str_permission);
//        if(r == QtAndroid::PermissionResult::Denied)
//        {
//            return false;
//        }
//    }
//    return true;
//#else
//    return false;
//#endif
//}
void printVector(std::vector<QPointF> vector)
{
    auto begin = vector.begin();
    auto end =vector.end();
    while(begin!=end)
    {
        qDebug()<<"vector"<<begin->x();
        begin++;
    }

}

