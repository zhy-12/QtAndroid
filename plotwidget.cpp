#include "plotwidget.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QGraphicsSimpleTextItem>

void test1(std::vector<QPointF> &vector,int lo,int hi);//冒泡排序
plotWidget::plotWidget(QWidget *parent, QString chartName)
    : QWidget{parent}
    , myChart(new QChart)
    , myChartView(new QChartView(myChart))
    , myLineSeries(new QLineSeries(this))
    , myScatters(new QScatterSeries(this))
    , layout(new QHBoxLayout(this))
    , axis_X(new QValueAxis(this))
    , axis_Y(new QValueAxis(this))
    , chartName(chartName)
    , text(new QGraphicsSimpleTextItem(myChart))
{

    myChartView->setRenderHint(QPainter::Antialiasing);
    myChartView->setScreen(parent->screen());
    myChartView->setBaseSize(parent->width(),parent->width());
    myChartView->setContentsMargins(0,0,0,0);
    myChart->setMargins(QMargins(0,0,0,0));
//    myChartView
//    myChart->setPlotArea(QRectF(50,80,310,150));
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(myChartView);

    this->setLayout(layout);
    this->setContentsMargins(0,0,0,0);

}
void plotWidget::setAixs(QString axisName_X, qreal min_X, qreal max_X, int tickCount_X, \
                         QString axisName_Y, qreal min_Y, qreal max_Y, int tickCount_Y)
{
    this->axisName_X = axisName_X;
    this->min_X = min_X;
    this->max_X = max_X;
    this->tickCount_X = tickCount_X;
    this->axisName_Y = axisName_Y;
    this->min_Y = min_Y;
    this->max_Y = max_Y;
    this->tickCount_Y = tickCount_Y;

    QFont labelsFont;
    QPen pen;
    pen.setWidth(2);
    labelsFont.setPixelSize(12);
    axis_X->setRange(this->min_X,this->max_X);
    axis_X->setLabelFormat("%.3f");//%u:无符号十进制数
    axis_X->setGridLineVisible(false);
    axis_X->setTickCount(tickCount_X);
    axis_X->setTitleText(axisName_X);
    axis_X->setLabelsFont(labelsFont);
    axis_X->setLinePen(pen);
    axis_X->setMinorGridLineVisible(false);

    axis_Y->setRange(this->min_Y,this->max_Y);
    axis_Y->setLabelFormat("%.3f");//%u:无符号十进制数
    axis_Y->setGridLineVisible(false);
    axis_Y->setMinorTickCount(1);
    axis_Y->setTickCount(tickCount_Y);
    axis_Y->setTitleText(axisName_Y);
    axis_Y->setLabelsFont(labelsFont);
    axis_Y->setLinePen(pen);
    axis_Y->setMinorGridLineVisible(false);


    myChart->addAxis(axis_X,Qt::AlignBottom);
    myChart->addAxis(axis_Y,Qt::AlignLeft);

    QFont font;
    font.setPixelSize(12);
    text->setFont(font);
    text->setPen(QPen(QColor(0,0,0)));
}
void plotWidget::initChart()
{
    myLineSeries->setPen(QPen(Qt::red,2,Qt::SolidLine));
    myLineSeries->clear();
    myScatters->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    myScatters->setMarkerSize(8);
    myScatters->setBorderColor(QColor(0,0,0));
    myScatters->setColor(QColor(0,0,0));
    std::vector<QPointF> points1 = {QPointF(0,1), QPointF(10,2), QPointF(20,4), QPointF(30,8), QPointF(40,16), \
                                   QPointF(50,16), QPointF(60,8), QPointF(70,4), QPointF(80,2), QPointF(90,1)};

    for(int i=0 ;i<int(points1.size()) ;i++)
    {
        myLineSeries->append(points1.at(i).x(),points1.at(i).y());
        myScatters->append(points1.at(i).x(),points1.at(i).y());
    }
    //myChart->setTitle(this->chartName);
    myChart->setAnimationOptions(QChart::SeriesAnimations);
    myChart->legend()->hide();
    myChart->setAcceptHoverEvents(true);
    myChart->setAcceptTouchEvents(true);

    myChart->addSeries(myLineSeries);
    myChart->addSeries(myScatters);
    myChart->setAxisX(axis_X,myLineSeries);
    myChart->setAxisY(axis_Y,myLineSeries);
    myChart->setAxisX(axis_X,myScatters);
    myChart->setAxisY(axis_Y,myScatters);


}
void plotWidget::slot_updateChart(std::vector<QPointF> points)
{
    test1(points,0,points.size());
    axis_X->setMax(points.back().x()*1.2);
    axis_X->setMin((points.front().x()/1.2/axis_X->max())>0.4?points.front().x()/1.2:0);
    axis_Y->setMax(points.back().y()*1.2);
    std::vector<QPointF> pointsFit = calculate(points);
    myLineSeries->clear();
    myScatters->clear();
    if(slop!=NAN&&Intercept!=NAN)
    {
        QString str;
        char* charCode = new char[20];
        str.append("y=");
        sprintf(charCode,"%.2f",slop);
        str.append(QString(charCode));
        str.append("x+");
        sprintf(charCode,"%.2f",Intercept);
        str.append(QString(charCode));
        text->setText(str);
        text->setPos(myChart->mapToPosition(QPointF(this->axis_X->max()*0.7,this->axis_Y->max()*0.94)));
        delete[] charCode;
    }

    for(int i=0 ;i<int(points.size()) ;i++)
    {
        myLineSeries->append(pointsFit.at(i).x(),pointsFit.at(i).y());
        myScatters->append(points.at(i).x(),points.at(i).y());
    }
}
std::vector<QPointF> plotWidget::calculate(std::vector<QPointF> points )
{
    std::vector<QPointF> buff;

    qreal fXY = 0, fXX = 0, fXA = 0, fYA = 0, fYY = 0;
    //得到平均值
    int LEN_MAX = points.size();

    for(int i = 0;i < LEN_MAX;i++)
    {
        fXA += points[i].x()/LEN_MAX;
        fYA += points[i].y()/LEN_MAX;
    }
    //得到拟合所必需的数据
    for(int i = 0;i < LEN_MAX;i++)
    {
        fXY  += (points[i].x() - fXA)*(points[i].y() - fYA);
        fXX += (points[i].x() - fXA)*(points[i].x() - fXA);
        fYY += (points[i].y() - fYA)*(points[i].y() - fYA);
    }
    slop = fXY / fXX;
    Intercept = fYA - ((slop) * fXA);
    buff.clear();
    //y = slop*x+intercept
    for(int i = 0;i < LEN_MAX;i++)
    {
       buff.push_back(QPointF(points[i].x() ,points[i].x()*slop+Intercept));
    }
    return buff;

}

void plotWidget::paintEvent(QPaintEvent* e)
{
    //this->resize()
}


void test1(std::vector<QPointF> &vector,int lo,int hi)//冒泡排序
{
    QPointF A;
    for (int i = 0; i < hi; i++)
    {
        for (int j = 0; j < hi - i - 1; j++)
        {
            if (vector[j].x() > vector[j + 1].x())
            {
                A = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = A;
            }
        }
    }
}






