#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QHBoxLayout>
#include <QLabel>
#include <QScatterSeries>
#include <QLegend>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>

class Callout;
class plotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit plotWidget(QWidget *parent = nullptr, QString chartName = "plot");
    ~plotWidget(){}
    void setAixs(QString axisName_X, qreal min_X, qreal max_X, int tickCount_X, \
                 QString axisName_Y, qreal min_Y, qreal max_Y, int tickCount_Y);
    void initChart();
    void paintEvent(QPaintEvent* ) override;
    std::vector<QPointF> calculate(std::vector<QPointF> points);
    void resizeEvent(QResizeEvent*) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void clear();


public:
    QChart *myChart;
    QChartView *myChartView;
    QLineSeries *myLineSeries;
    QScatterSeries* myScatters;

    QHBoxLayout *layout;
    QValueAxis *axis_X;
    QValueAxis *axis_Y;

    QString chartName;
    QString axisName_X;
    QString axisName_Y;
    qreal min_X;
    qreal max_X;
    qreal min_Y;
    qreal max_Y;
    int tickCount_X;
    int tickCount_Y;
    qreal slop;
    qreal Intercept;
    qreal R2;
    QGraphicsSimpleTextItem* text;

private:
//    QGraphicsSimpleTextItem *m_coordX;
//    QGraphicsSimpleTextItem *m_coordY;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;

signals:

public slots:
    void slot_updateChart(std::vector<QPointF> points);
    void keepCallout(QPointF point);
    void tooltip(QPointF point, bool state);

};

#endif // PLOTWIDGET_H
