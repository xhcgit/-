#include "PaintData.h"
#include "ui_PaintData.h"
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>

#include <QSqlQuery>
#include <QDebug>

PaintData::PaintData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintData)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    //鼠标滚轮放大缩小
    QwtPlotMagnifier *PM = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    //鼠标左键选择区域放大,右键还原
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 100, 300);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 86400);
    curve = new QwtPlotCurve();
    //X轴
    /*double time[10] = {67384, 67385, 67386, 67387, 67388, 67389, 67390, 67391, 67392, 67393};
    //Y轴
    double val[10] = {3, 5, 8, 7, 2, 0, 7, 9, 1};
    //加载数据
    curve->setSamples(time, val, 10);
    //加到plot，plot由IDE创建
    curve->attach(ui->qwtPlot);*/

}

PaintData::~PaintData()
{
    delete ui;
}

void PaintData::on_pushButton_clicked()
{
    QSqlQuery query;
    QDateTime dateTime;
    QTime time;
    QString today = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString nextDay = ui->dateEdit->date().addDays(1).toString();
    int second;
    query.prepare("select * from ValueFromSerialport where receiveTime > :Today and receiveTime < :nextDay");
    query.bindValue(":Today", today);
    query.bindValue(":nextDay", nextDay);
    query.exec();
    while(query.next())
    {
        dateTime = QDateTime::fromString(query.value(0).toString(),"yyyy-MM-dd-HH:mm:ss");
        time = dateTime.time();
        second = (time.hour() * 60 + time.minute()) * 60 + time.second();
        timeList.append((double)second);
        valueList.append(query.value(1).toDouble());
    }
    beginPaintData();
}

void PaintData::beginPaintData()
{
    curve = new QwtPlotCurve();
    //加载数据
    curve->setSamples(timeList.toVector(), valueList.toVector());
    //加到plot，plot由IDE创建
    curve->attach(ui->qwtPlot);
    qDebug() << "i am here";
}
