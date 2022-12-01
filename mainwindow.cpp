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
#include <QMessageBox>
#include <QStringList>
#include <QHeaderView>
#include <QScroller>
#include <QListWidget>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <rotateimagedialog.h>
#include <QStandardPaths>
void printVector(std::vector<QPointF> vector);
void test1(std::vector<QPointF> &vector,int lo,int hi);//冒泡排序
int resize_uniform(cv::Mat &src, cv::Mat &dst, cv::Size dst_size, object_rect &effect_area);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stack(new QStackedWidget(this))
    , myLabel(new QLabel(this))
    , myImageLabel(new imageLabel(this))
    , myButton1(new QPushButton("Model",this))
    , myButton2(new QPushButton("Fit",this))
    , chooseImageButton(new QPushButton("Select",this))
    , btn_stackToImage(new QPushButton("Image",this))
    , btn_stackToChart(new QPushButton("Chart",this))
    , btn_stackToTable(new QPushButton("RGB",this))
    , myPlot(new plotWidget(this,"chart"))
    , myTable(new QTableWidget(8,12,this))
{
    setStyleSheet("QMainWindow{background-color:white;}");
    m_sampleType = sampleType_non;

    myLabel->setAlignment(Qt::AlignCenter);
    myLabel->setWordWrap(true);
    myLabel->setMargin(0);
    QFont ft;
    ft.setPointSize(10);
    myLabel->setFont(ft);
    updateLabelText(1,"");
//    myImageLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    myPlot->setAixs("position",0,12,13,"G",0,300,6);
    myPlot->initChart();
    myPlot->hide();
//    myPlot->set
    initDataTable();
    index1 = stack->addWidget(myImageLabel);
    index2 = stack->addWidget(myPlot);
    index3 = stack->addWidget(myTable);
    stack->setCurrentWidget(myImageLabel);
//    stack->setStyleSheet("QStackedWidget{background-color:white;}")
    //stack->setContentsMargins(0,0,0,0);

    chooseImageButton->setObjectName("chooseImage");
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QHBoxLayout* stackBtnLayout = new QHBoxLayout();
    stackBtnLayout->addWidget(btn_stackToImage);
    stackBtnLayout->addWidget(btn_stackToChart);
    stackBtnLayout->addWidget(btn_stackToTable);

    buttonLayout->addWidget(chooseImageButton);
    buttonLayout->addWidget(myButton1);
    buttonLayout->addWidget(myButton2);
    buttonLayout->setAlignment(Qt::AlignBottom);
//    mainLayout->addWidget(myLabel);
    myLabel->hide();
    mainLayout->addLayout(stackBtnLayout);
    //myImageLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(stack);
    mainLayout->addLayout(buttonLayout);
    QWidget * centerWidget = new QWidget(this);
    centerWidget->setLayout(mainLayout);
    this->setCentralWidget(centerWidget);
    //this->showFullScreen();
    //this->setFixedSize(this->screen()->size());
    myButton1->connect(myButton1, SIGNAL(pressed()),this,SLOT(slot_processBtn1()));
    myButton2->connect(myButton2, SIGNAL(pressed()),this,SLOT(slot_processBtn2()));
    chooseImageButton->connect(chooseImageButton,SIGNAL(pressed()),this,SLOT(slot_chooseImage()));
    btn_stackToImage->connect(btn_stackToImage,SIGNAL(clicked()),this,SLOT(slot_switchToImage()));
    btn_stackToChart->connect(btn_stackToChart,SIGNAL(clicked()),this,SLOT(slot_switchToChart()));
    btn_stackToTable->connect(btn_stackToTable,SIGNAL(clicked()),this,SLOT(slot_switchToTable()));
}

MainWindow::~MainWindow(){}
void MainWindow::slot_switchToImage()
{
    myPlot->hide();
    myTable->hide();
    myImageLabel->show();
    this->stack->setCurrentIndex(index1);
}
void MainWindow::slot_switchToChart()
{
    myPlot->show();
    myImageLabel->hide();
    myTable->hide();
    this->stack->setCurrentIndex(index2);
}
void MainWindow::slot_switchToTable()
{
    myTable->show();
    myPlot->hide();
    myImageLabel->hide();
    this->stack->setCurrentIndex(index3);
}
void MainWindow::updateLabelText(int x,QString str)
{
    QRect m_Rect = QGuiApplication::primaryScreen()->geometry();
    QSize size = m_Rect.size();
    //QSize size = this->stack->size();
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
    default:
        break;
    }
    info.append(" ");
    info.append(str);
    this->myLabel->setText(info);
}
void MainWindow::process_Color(cv::Mat frame, std::vector<std::vector<cv::Point2f>> center )
{
    cv::Mat frame_temp; 
    frame.copyTo(frame_temp);
    cv::Mat cut_frame;
    cv::Scalar tempVal;
    for(int i=0;i<8;i++)//8行
    {
        std::vector<double> average_B_temp;
        std::vector<double> average_G_temp;
        std::vector<double> average_R_temp;
        for(int j=0;j<12;j++)//12列
        {
            cut_frame = frame_temp(cv::Rect(center[i][j].x-2,center[i][j].y-2,4,4));
            tempVal = cv::mean(cut_frame);
            average_B_temp.push_back(tempVal[0]);
            average_G_temp.push_back(tempVal[1]);
            average_R_temp.push_back(tempVal[2]);
        }
        average_B.push_back(average_B_temp);
        average_G.push_back(average_G_temp);
        average_R.push_back(average_R_temp);
    }
    cv::cvtColor(frame_temp, frame_temp, cv::COLOR_BGR2HSV);
    for(int i=0;i<8;i++)
    {
        std::vector<double> average_H_temp;
        std::vector<double> average_S_temp;
        std::vector<double> average_V_temp;
        for(int j=0;j<12;j++)
        {
            cut_frame = frame_temp(cv::Rect(center[i][j].x-2,center[i][j].y-2,4,4));
            tempVal = cv::mean(cut_frame);
            average_H_temp.push_back(tempVal[0]);
            average_S_temp.push_back(tempVal[1]);
            average_V_temp.push_back(tempVal[2]);
        }
        average_H.push_back(average_H_temp);
        average_S.push_back(average_S_temp);
        average_V.push_back(average_V_temp);
    }
}

void MainWindow::slot_chooseImage()
{
    QString filePath;
    double center_y[]={270, 500, 700, 950, 1150, 1400, 1600, 1825};
//    double center_x[]={320, 550, 770, 990, 1220, 1450, 1680, 1900+10, 2125+33, 2350+33, 2570+33, 2800+33};
    double center_x[]={320, 550, 770, 990, 1220, 1450, 1680, 1900, 2125, 2350, 2570, 2800};
    std::vector<cv::Point2f> center_temp;
    center.clear();
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<12;j++)
            center_temp.push_back(cv::Point(center_x[j],center_y[i]));
        center.push_back(center_temp);
        center_temp.clear();

    }

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
        this->myTable->clearContents();
    }
    cX.clear();
    cY.clear();
    points1.clear();
    myPlot->text->setText(" ");
    qDebug()<<"path:"<<filename[0];
    cv::Mat imgage_hsv;
    cv::Mat mask0;
    QImage img_temp(filename[0]);



    rotateImageDialog *rotateDialog = new rotateImageDialog(this);
    rotateDialog->setPicture(&img_temp);
    if(rotateDialog->exec())
    {
        //img_temp = rotateDialog->getPicture().copy(rotateDialog->getPicture().rect());
    }
    qDebug()<<"\033[31m"<<"旋转完后图片尺寸 width:"<<img_temp.width()<<"height:"<<img_temp.height();
//    if(img_temp.width()<img_temp.height())
//    {
//        QTransform matrix;
//        matrix.rotate(90);
//        img_temp = img_temp.transformed(matrix);
//    }

    img_temp = img_temp.convertToFormat(QImage::Format_BGR888);
    img_temp = img_temp.scaled(QSize(4608,3456),Qt::IgnoreAspectRatio);
    qDebug()<<"img_temp"<<img_temp.size().width()<<" "<<img_temp.size().height();
    cv::Mat frame(img_temp.height(),img_temp.width(),CV_8UC3,(uchar*)img_temp.bits(),img_temp.bytesPerLine());
    qDebug()<<"frame1:"<<frame.cols<<" "<<frame.rows;
    cv::resize(frame,frame,cv::Size(4608,3456),0,0,cv::INTER_CUBIC);
    qDebug()<<"frame2:"<<frame.cols<<" "<<frame.rows;
    int rows = frame.rows;
    int colums = frame.cols;
    cv::cvtColor(frame,imgage_hsv,cv::COLOR_BGR2HSV);
    std::vector<int> lower_black = {0,0,0};
    std::vector<int> upper_black = {180, 255, 46};
    cv::inRange(imgage_hsv,lower_black,upper_black,mask0);
    int rows_m = int(rows/2);
    int colums_m = int(colums/2);
    int top_Y = 0;
    int bottom_Y = 0;
    int top_X = 0;
    int bottom_X = 0;
    cv::Scalar temp_sum = 0;
    cv::Mat temp_mask;
//    cv::cvtColor(imgage_hsv,temp_mask,cv::COLOR_GRAY2RGB);
//    saveImage(imgage_hsv);
    for(int i=0;i<rows;++i){
        temp_sum = cv::sum(mask0(cv::Rect(colums_m,rows_m-i-10,1,10)));
        if((mask0.at<uchar>(rows_m-i,colums_m) == 255)&&temp_sum[0]>=2550){

            top_Y = rows_m-i;
            break;
        }
    }
    for(int i=0;i<rows;++i){
        if(mask0.at<uchar>(rows_m+i,colums_m) == 255){
            bottom_Y = rows_m+i;
            break;
        }
    }
    for(int i=0;i<rows;++i){
        if(mask0.at<uchar>(rows_m,colums_m-i) == 255){
            top_X = colums_m-i;
            break;
        }
    }
    for(int i=0;i<rows;++i){
        temp_sum = cv::sum(mask0(cv::Rect(colums_m+i,rows_m,10,1)));
//        qDebug()<<"temp_sum:"<<temp_sum[0];
        if((mask0.at<uchar>(rows_m,colums_m+i) == 255)&&temp_sum[0]>=2550){

            bottom_X = colums_m+i;
            break;
        }
    }

    if(!assertRectValid(top_X,top_Y,bottom_X-top_X,bottom_Y-top_Y,frame.cols,frame.rows))
    {
        QMessageBox::warning(this,"choose Image error","please retry",QMessageBox::Close);
        return;
    }
    frame = frame(cv::Rect(top_X,top_Y,bottom_X-top_X,bottom_Y-top_Y));
//    for(int i=0;i<8;i++)
//    {
//        for(int j=0;j<12;j++)
//        {
//            cv::circle(frame,center[i][j],10,cv::Scalar(0,0,0),-1);
//            qDebug()<<i<<","<<j<<":("<<center[i][j].x<<","<<center[i][j].y<<")";
//        }

//    }
//    cv::circle(frame,cv::Point(0,0),20,cv::Scalar(0,0,221),-1);
    qDebug()<<"\033[31m"<<"裁剪后大小:width:"<<frame.cols<<"height:"<<frame.rows;//3154
    qDebug()<<"裁剪位置 top_X(865):"<<top_X<<"top_Y:"<<top_Y<<"bottom_X(4019):"<<bottom_X<<"bottom_Y"<<bottom_Y;
    qDebug()<<"temp_sum"<<temp_sum[0]<<" "<<temp_sum[1]<<" "<<temp_sum[2];
    frame.copyTo(text_frame);

    cv::imwrite(filename[0].toStdString()+"temp.jpg",frame);
    text_frame.copyTo(frameBtn1);
    text_frame.copyTo(frameBtn2);
    process_Color(text_frame, center);


    //qDebug()<<"average_S:"<<average_S;
    //qDebug()<<"average_G:"<<average_G;
    textBuffer.clear();
    textH2O2.clear();
    textGlucose.clear();
    textAbsorbance.clear();
    double temp;
    std::vector<double> temp_H2O2;
    std::vector<double> temp_Glucose;
    std::vector<double> temp_Absorbance;
    double tempToPush;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<12;j++)
        {
            if((average_S[i][j]+35.7905)/219.2612 < 0.52)
            {
                temp = double((average_S[i][j] + 10.2910)/170.3633);
                tempToPush =0.1683*temp-0.0342;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_H2O2.push_back(0);
                else
                    temp_H2O2.push_back(tempToPush);

                tempToPush = 1.4914*temp-0.4627;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_Glucose.push_back(0);
                else
                    temp_Glucose.push_back(tempToPush);

                temp_Absorbance.push_back(temp);
            }
            else if(((average_S[i][j] + 35.7905)/(219.2612) < 1) && ((average_S[i][j] + 35.7905)/(219.2612) > 0.52))
            {
                temp = double((average_S[i][j] + 35.7905)/219.2612);
                tempToPush =0.1683*temp-0.0342;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_H2O2.push_back(0);
                else
                    temp_H2O2.push_back(tempToPush);

                tempToPush = 1.4914*temp-0.4627;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_Glucose.push_back(0);
                else
                    temp_Glucose.push_back(tempToPush);
                temp_Absorbance.push_back(temp);
            }
            else if(((average_G[i][j] - 219.7655)/(-28.0058)) < 1.84)
            {
                temp = double((average_G[i][j] - 219.7655)/(-28.0058));
                tempToPush =0.1683*temp-0.0342;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_H2O2.push_back(0);
                else
                    temp_H2O2.push_back(tempToPush);

                tempToPush = 1.4914*temp-0.4627;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_Glucose.push_back(0);
                else
                    temp_Glucose.push_back(tempToPush);
                temp_Absorbance.push_back(temp);
            }
            else
            {
                temp = double((average_G[i][j] - 203.63)/(-19.243));
                tempToPush =0.1683*temp-0.0342;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_H2O2.push_back(0);
                else
                    temp_H2O2.push_back(tempToPush);

                tempToPush = 1.4914*temp-0.4627;
                if(tempToPush>-0.3&&tempToPush<0)
                    temp_Glucose.push_back(0);
                else
                    temp_Glucose.push_back(tempToPush);
                temp_Absorbance.push_back(temp);
            }
        }
        //qDebug()<<"temp_H2O2:"<<temp_H2O2;
        textH2O2.push_back(temp_H2O2);
        textGlucose.push_back(temp_Glucose);
        textAbsorbance.push_back(temp_Absorbance);
        temp_H2O2.clear();
        temp_Glucose.clear();
        temp_Absorbance.clear();
    }
    updateImage(text_frame);
    qDebug()<<"average_G[D]:"<<average_G[3];
    qDebug()<<"average_G[E]:"<<average_G[4];
    this->myPlot->myLineSeries->clear();
    this->myPlot->myScatters->clear();
    slot_switchToImage();
}
void MainWindow::updateImage(cv::Mat frame)
{
    if(!frame.empty())
    {
        this->myImageLabel->hide();
        QImage img((const uchar*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        myImageLabel->setImage(img.rgbSwapped());
//        QPixmap pixImage = QPixmap::fromImage(img.rgbSwapped());
//        pixImage = pixImage.scaled(this->stack->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        myImageLabel->resize(pixImage.size());
//        myImageLabel->setPixmap(pixImage);
        myImageLabel->show();

        update();

    }

}
void MainWindow::saveImage(cv::Mat temp_frame)
{
    QImage img((const uchar*)(temp_frame.data),temp_frame.cols,temp_frame.rows,temp_frame.step,QImage::Format_RGB888);
    QString path = QFileDialog::getSaveFileName(this,"保存文件",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    img = img.rgbSwapped();
    img.save(path );
    qDebug()<<path;
}
void MainWindow::slot_processBtn1()
{
    double threshold;
    sampleDialog* dialog = new sampleDialog();
    if(dialog->exec())
    {
        m_sampleType = dialog->m_sampleType;
    }
    switch (m_sampleType) {
    case sampleType_h202:
        textBuffer = textH2O2;
        threshold = -0.1;
        updateLabelText(1,"H2O2");
        break;
    case sampleType_glucose:
        textBuffer = textGlucose;
        threshold = -0.1;
        updateLabelText(1,"Glucose");
        break;
    case sampleType_absorbance:
        textBuffer = textAbsorbance;
        threshold = -0.1;
        updateLabelText(1,"Absorbance");
        break;
    case sampleType_non:
        threshold = -0.1;
        return;
    default:
        threshold = -0.1;
        break;
    }
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    std::vector<int> tt;
    ABCDdialog *dialog_selcetLine = new ABCDdialog(this);
    if(dialog_selcetLine->exec())
    {
        if(dialog_selcetLine->checkBox_A->isChecked()){
            tt.push_back(0);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_B->isChecked()){
            tt.push_back(1);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_C->isChecked()){
            tt.push_back(2);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_D->isChecked()){
            tt.push_back(3);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_E->isChecked()){
            tt.push_back(4);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_F->isChecked()){
            tt.push_back(5);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_G->isChecked()){
            tt.push_back(6);
        }else{tt.push_back(-1);}
        if(dialog_selcetLine->checkBox_H->isChecked()){
            tt.push_back(7);
        }else{tt.push_back(-1);}
        if(tt.empty())
            return;
    }
    bool isEmpty = true;
    foreach (auto item, tt)
    {
        if(item !=-1)
            isEmpty = false;
    }
    if(isEmpty==true)
    {
        QMessageBox::warning(this,"WARNING","Select one or more lines",QMessageBox::Close);
        return;
    }
    myTable->clearContents();
    std::string text;
    cv::Mat temp_frame;
    frameBtn1.copyTo(temp_frame);
    qDebug()<<"图片大小"<<"width:"<<temp_frame.cols<<"height:"<<temp_frame.rows;
    mask.clear();
    std::vector<bool> mask_lineTemp;
    for(int i=0;i<8;i++)
    {
        if(tt[i] == i)
        {
            for(int j=2;j<10;j++)
            {
                if(textBuffer[i][j]>threshold)
                {
                    mask_lineTemp.push_back(1);
                    //write the textbuffer value to the corresponding position on the image
                    text = std::to_string(textBuffer[i][j]);
                    text = text.substr(0, text.find(".") + 2 + 1);
                    int baseline;
                    cv::Point temp_point;
                    int thickness = 8;
                    double font_scale = 2.8;
                    cv::Size text_Size = cv::getTextSize(text,cv::FONT_HERSHEY_SIMPLEX,font_scale,thickness,&baseline);
                    temp_point.x = center[i][j].x - text_Size.width/2*0.8;
                    temp_point.y = center[i][j].y + text_Size.height/2;
                    cv::putText(temp_frame,text,temp_point,cv::FONT_HERSHEY_SIMPLEX,font_scale,cv::Scalar(0,0,0),thickness);
                    //cv::circle(temp_frame,center[i][j],15,cv::Scalar(0,0,0),-1);
                    //update table items with the corresponding RGB
                    QTableWidgetItem *item = new QTableWidgetItem;
                    item->setBackground(QBrush(QColor(0,0,0)));
                    item->setForeground(QColor(125,182,191));
                    QString str =QString("R%1\nG%2\nB%3").arg(QString::number(int(average_R[i][j])),QString::number(int(average_G[i][j])),QString::number(int(average_B[i][j])));
                    item->setText(str);
                    myTable->setItem(i,j,item);
                }
                else
                {
                    mask_lineTemp.push_back(0);
                }
            }
        } else
        {
            for(int j=0;j<12;j++)
                mask_lineTemp.push_back(0);
        }
        mask.push_back(mask_lineTemp);
        mask_lineTemp.clear();
    }
//    saveImage(temp_frame);
    updateImage(temp_frame);
    this->slot_switchToImage();
}
void MainWindow::slot_processBtn2()
{
    if(textBuffer.empty())
    {
        QMessageBox::warning(this,"WARNING","Please click button Model first",QMessageBox::Close);
        return;
    }
    std::vector<int> tt;
    if(this->filename.isEmpty() == true&&this->text_frame.empty())
        return;
    ABCDdialog *dialog = new ABCDdialog(this);
    if(dialog->exec())
    {
        if(dialog->checkBox_A->isChecked()){
            tt.push_back(1);
        }
        if(dialog->checkBox_B->isChecked()){
            tt.push_back(2);
        }
        if(dialog->checkBox_C->isChecked()){
            tt.push_back(3);
        }
        if(dialog->checkBox_D->isChecked()){
            tt.push_back(4);
        }
        if(dialog->checkBox_E->isChecked()){
            tt.push_back(5);
        }
        if(dialog->checkBox_F->isChecked()){
            tt.push_back(6);
        }
        if(dialog->checkBox_G->isChecked()){
            tt.push_back(7);
        }
        if(dialog->checkBox_H->isChecked()){
            tt.push_back(8);
        }
        if(tt.empty())
            return;
    }
    points1.clear();
    auto colorBuffer = average_G;
    if(tt.size()>1||tt.size()==0)
    {
        QMessageBox::warning(this,"WARNING","Select one line",QMessageBox::Close);
        return;
    }

    for(int j=0;j<12;j++)
    {
        if(mask[tt.back()-1][j]==true)
        {
            points1.push_back(QPointF(j+1,colorBuffer[tt.back()-1][j]));
            qDebug()<<points1.back();
        }
    }
    if(!points1.empty())
        myPlot->slot_updateChart(points1);
    else
        myPlot->clear();
    this->slot_switchToChart();
    this->updateLabelText(2,"");
}
void MainWindow::initDataTable()
{
    QFont font;
    QString qssTV = "QTableWidget::item:hover{background-color:rgb(92,188,227,200)}"
                        "QTableWidget::item:selected{background-color:#1B89A1}"
                        "QHeaderView::section,QTableCornerButton:section{ \
                padding:3px; margin:0px; color:#DCDCDC;  border:1px solid #242424; \
        border-left-width:0px; border-right-width:1px; border-top-width:0px; border-bottom-width:1px; \
    background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252); }"
    "QTableWidget{background-color:white;border:none;}";

    myTable->setStyleSheet(qssTV);
    myTable->setFont(QFont("song", 12, QFont::Bold));
    myTable->setHorizontalHeaderLabels(QStringList()<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12");
    myTable->setVerticalHeaderLabels(QStringList()<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H");
    myTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    myTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QScroller *pScroller = QScroller::scroller(myTable);
    pScroller->grabGesture(myTable,QScroller::LeftMouseButtonGesture);
    myTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    myTable->horizontalScrollBar()->hide();
    myTable->verticalScrollBar()->hide();
    myTable->resizeRowsToContents();
    myTable->resizeColumnsToContents();
}
void MainWindow::paintEvent(QPaintEvent* e)
{
    myTable->resizeRowsToContents();
    QWidget::paintEvent(e);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(screen()->size().width()>=screen()->size().height())
    {
        myPlot->resize(stack->width(),stack->height());
    }
    else
    {
        myPlot->resize(stack->width(),stack->height()/2.2);
    }
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


int resize_uniform(cv::Mat &src, cv::Mat &dst, cv::Size dst_size, object_rect &effect_area)
{
    int w = src.cols;
    int h = src.rows;
    int dst_w = dst_size.width;
    int dst_h = dst_size.height;
    qDebug() << "src: (" << h << ", " << w << ")" ;
    dst = cv::Mat(cv::Size(dst_w, dst_h), CV_8UC3, cv::Scalar(0));

    float ratio_src = w*1.0 / h;
    float ratio_dst = dst_w*1.0 / dst_h;

    int tmp_w=0;
    int tmp_h=0;
    if (ratio_src > ratio_dst) {
        tmp_w = dst_w;
        tmp_h = floor((dst_w*1.0 / w) * h);
    } else if (ratio_src < ratio_dst){
        tmp_h = dst_h;
        tmp_w = floor((dst_h*1.0 / h) * w);
    } else {
        resize(src, dst, dst_size);
        effect_area.x = 0;
        effect_area.y = 0;
        effect_area.width = dst_w;
        effect_area.height = dst_h;
        return 0;
    }

    qDebug() << "tmp: (" << tmp_h << ", " << tmp_w << ")";
    cv::Mat tmp;
    resize(src, tmp, cv::Size(tmp_w, tmp_h));

    if (tmp_w != dst_w) { //高对齐，宽没对齐
        int index_w = floor((dst_w - tmp_w) / 2.0);
        qDebug() << "index_w: " << index_w ;
        for (int i=0; i<dst_h; i++) {
            memcpy(dst.data+i*dst_w*3 + index_w*3, tmp.data+i*tmp_w*3, tmp_w*3);
        }
        effect_area.x = index_w;
        effect_area.y = 0;
        effect_area.width = tmp_w;
        effect_area.height = tmp_h;
    } else if (tmp_h != dst_h) { //宽对齐， 高没有对齐
        int index_h = floor((dst_h - tmp_h) / 2.0);
        qDebug() << "index_h: " << index_h ;
        memcpy(dst.data+index_h*dst_w*3, tmp.data, tmp_w*tmp_h*3);
        effect_area.x = 0;
        effect_area.y = index_h;
        effect_area.width = tmp_w;
        effect_area.height = tmp_h;
    } else {
        printf("error\n");
    }
    return 0;
}
