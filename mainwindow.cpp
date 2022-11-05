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
#include "abcddialog.h"
void printVector(std::vector<QPointF> vector);
void test1(std::vector<QPointF> &vector,int lo,int hi);//冒泡排序


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , stack(new QStackedWidget(this))
    , myLabel(new QLabel(this))
    , myImageLabel(new QLabel(this))
    , myButton1(new QPushButton("reagent",this))
    , myButton2(new QPushButton("linear",this))
    , myButton3(new QPushButton("unused",this))
    , myButton4(new QPushButton("unused", this))
    , chooseImageButton(new QPushButton("Select",this))
    , btn_stackToImage(new QPushButton("Image",this))
    , btn_stackToChart(new QPushButton("Chart",this))
    , myPlot(new plotWidget(this,"chart"))
{
    m_sampleType = sampleType_non;

    myLabel->setAlignment(Qt::AlignCenter);
    myLabel->setWordWrap(true);
    myLabel->setMargin(0);
    QFont ft;
    ft.setPointSize(10);
    myLabel->setFont(ft);
    updateLabelText(1);
    myImageLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    myPlot->setAixs("concentration",0,1,8,"RGB",0,300,6);
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
void MainWindow::process_Color(cv::Mat frame, std::vector<double> cX, std::vector<double> cY )
{

    cv::Mat frame_temp; 
    frame.copyTo(frame_temp);
    cv::Mat cut_frame;
    cv::Scalar tempVal;
    for(int i=0;i<int(cX.size());i++)
    {
        for(int j=0;j<int(cY.size());j++)
        {
            qDebug()<<"point:"<<cX[i]-2<<","<<cY[j]-2;
            cut_frame = frame_temp(cv::Rect(cX[i]-2,cY[j]-2,4,4));
            tempVal = cv::mean(cut_frame);
            average_B.push_back(tempVal[0]);
            average_G.push_back(tempVal[1]);
            average_R.push_back(tempVal[2]);
        }
    }
    cv::cvtColor(frame_temp, frame_temp, cv::COLOR_BGR2HSV);
    for(int i=0;i<int(cX.size());i++)
    {
        for(int j=0;j<int(cY.size());j++)
        {
            cut_frame = frame_temp(cv::Rect(cX[i]-2,cY[j]-2,4,4));
            tempVal = cv::mean(cut_frame);

            average_H.push_back(tempVal[0]);
            average_S.push_back(tempVal[1]);
            average_V.push_back(tempVal[2]);
        }
    }
}

void MainWindow::slot_chooseImage()
{
    center_y={270, 500, 700, 950, 1150, 1400, 1600, 1825};
    center_x={320, 550, 770, 990, 1220, 1450, 1680, 1900, 2125, 2350, 2570, 2800};

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
        average_B.clear();
        average_G.clear();
        average_R.clear();
        average_H.clear();
        average_S.clear();
        average_V.clear();
    }
    cX.clear();
    cY.clear();
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
    img_temp = img_temp.scaled(QSize(4680,3456),Qt::IgnoreAspectRatio);
    qDebug()<<"img_temp"<<img_temp.size().width()<<" "<<img_temp.size().height();
    cv::Mat frame(img_temp.height(),img_temp.width(),CV_8UC3,(uchar*)img_temp.bits(),img_temp.bytesPerLine());
    qDebug()<<"frame1:"<<frame.cols<<" "<<frame.rows;
    cv::resize(frame,frame,cv::Size(4680,3456),0,0,cv::INTER_CUBIC);
    qDebug()<<"frame2:"<<frame.cols<<" "<<frame.rows;
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
    qDebug()<<"frame3:"<<frame.cols<<" "<<frame.rows;
    text_frame = frame;
    text_frame.copyTo(frameBtn1);
    text_frame.copyTo(frameBtn2);
    text_frame.copyTo(frameBtn3);
    text_frame.copyTo(frameBtn4);
    process_Color(text_frame, center_x, center_y );

    auto begin_ave_s = average_S.begin();
    auto end_ave_s = average_S.end();
    auto begin_ave_G = average_G.begin();
    qDebug()<<"average_S:"<<average_S;
    qDebug()<<"average_G:"<<average_G;
    textBuffer.clear();
    while(begin_ave_s!=end_ave_s)
    {

        //if((*begin_ave_s+22.1059)/286.3640 < 0.95)
        //    textBuffer.push_back(double((*begin_ave_s + 22.1059)/286.3640));
        //else
            textBuffer.push_back(double((*begin_ave_G - 219.7655)/(-28.0058)));
        begin_ave_s++;
        begin_ave_G++;
    }

    updateImage(text_frame);


    this->myPlot->myLineSeries->clear();
    this->myPlot->myScatters->clear();
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
    center_y={270, 500, 700, 950, 1150, 1400, 1600, 1825};
    center_x={320, 550, 770, 990, 1220, 1450, 1680, 1900, 2125, 2350, 2570, 2800};
    sampleDialog* dialog = new sampleDialog();
    if(dialog->exec())
    {
        m_sampleType = dialog->m_sampleType;
    }
    if(m_sampleType==sampleType_non)
        return;
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    auto begin_text = textBuffer.begin();
    auto end_text = textBuffer.end();
    std::string text;
    int i = 0;
    while(begin_text!=end_text)
    {
        if(*begin_text>0.2)
        {
            text = std::to_string(*begin_text);
            text = text.substr(0, text.find(".") + 2 + 1);
            cv::putText(frameBtn1,text,cv::Point(center_x[int(i-int(i/12)*12)], center_y[int(i/12)]),cv::FONT_HERSHEY_SIMPLEX,2,cv::Scalar(0,0,0),10);
            cv::circle(frameBtn1,cv::Point(center_x[int(i-int(i/12)*12)], center_y[int(i/12)]),15,cv::Scalar(0,0,0),-1);
        }

        i++;
        begin_text++;
    }
    updateImage(frameBtn1);
    this->slot_switchToImage();
    this->updateLabelText(1);
}
void MainWindow::slot_processBtn2()
{
    center_y.clear();
    center_x.clear();
    std::vector<int> tt;
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    ABCDdialog *dialog = new ABCDdialog(this);
    if(dialog->exec())
    {
        if(dialog->checkBox_A->isChecked())
        {
            center_x.push_back(270);
            tt.push_back(1);
        }
        if(dialog->checkBox_B->isChecked())
        {
            center_x.push_back(500);
            tt.push_back(2);
        }

        if(dialog->checkBox_C->isChecked())
        {
            center_x.push_back(700);
            tt.push_back(3);
        }

        if(dialog->checkBox_D->isChecked())
        {
            center_x.push_back(950);
            tt.push_back(4);
        }

        if(dialog->checkBox_E->isChecked())
        {
            center_x.push_back(1150);
            tt.push_back(5);
        }

        if(dialog->checkBox_F->isChecked())
        {
            center_x.push_back(1400);
            tt.push_back(6);
        }
        if(dialog->checkBox_G->isChecked())
        {
            center_x.push_back(1600);
            tt.push_back(7);
        }
        if(dialog->checkBox_H->isChecked())
        {
            center_x.push_back(1825);
            tt.push_back(8);
        }

        if(center_x.empty())
            return;
    }
    std::vector<qreal> line_ave_s;
    points1.clear();
    for(int i = 0;i<int(center_x.size());i++)
    {
        for(int j = 0;j<12;j++)
        {
            line_ave_s.push_back(average_S[(tt[i]-1)*12+j]);
        }
    }

    for(int i=0,j=0;i<int(textBuffer.size());i++)
    {

        if(textBuffer[i]>0.2)
        {
            j++;
            points1.push_back(QPointF(j,textBuffer[i]));
        }
    }
    myPlot->slot_updateChart(points1);
    this->slot_switchToChart();
    if(screen()->size().width()>=screen()->size().height())
        myPlot->resize(stack->width(),stack->height());
    else
        myPlot->resize(stack->width(),stack->height()/2.2);
    myPlot->updateGeometry();
    this->updateLabelText(2);
}
void MainWindow::slot_processBtn3()
{
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;



//    myPlot->slot_updateChart(points2);
//    if(screen()->size().width()>=screen()->size().height())
//        myPlot->resize(stack->width(),stack->height());
//    else
//        myPlot->resize(stack->width(),stack->height()/2.2);
//    myPlot->updateGeometry();
//    updateLabelText(3);
//    updateImage(frameBtn3);
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
//    if(!processedBtn[3])
//    {
//        std::string text;
//        std::vector<qreal> y_green;
//        for(int i=0;i<int(average_G.size());i++)
//        {
//            y_green.push_back(qreal((average_R[i]+average_B[i]+average_B[i])));
//            text = std::to_string(y_green.back());
//            text = text.substr(0, text.find(".") + 2 + 1);
//            cv::putText(frameBtn4,text,cv::Point(cX[i]-90,cY[i]),cv::FONT_HERSHEY_SIMPLEX,2,cv::Scalar(0,0,0),10);
//        }
//        processedBtn[2] = true;
//    }
//    updateLabelText(4);
//    updateImage(frameBtn4);

}
void MainWindow::paintEvent(QPaintEvent* e)
{

//    myPlot->resize(this->width(),myPlot->height());
    if(screen()->size().width()>=screen()->size().height())
        myPlot->resize(stack->width(),stack->height());
    else
        myPlot->resize(stack->width(),stack->height()/2.2);
    myPlot->updateGeometry();
//    QWidget::paintEvent(e);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
//    if(myPlot->myChartView->scene())
//    {
//        myPlot->myChartView->scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
//        myPlot->myChart->resize(event->size());
//    }
    //updateImage(frameBtn1);


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

