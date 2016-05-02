#include "PaintData.h"
#include "ui_PaintData.h"
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>

#include <QSqlQuery>

PaintData::PaintData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintData)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 2200);   //数据最大值为2200
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 86400);//一天一共86400秒
    //鼠标滚轮放大缩小
    //QwtPlotMagnifier *PM = new QwtPlotMagnifier( ui->qwtPlot->canvas());

    //鼠标左键选择区域放大,右键还原
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas());

    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    curve = new QwtPlotCurve();
}

PaintData::~PaintData()
{
    delete ui;
}


void PaintData::on_showAllDay_clicked()
{
    QSqlQuery query;
    QDateTime dateTime;
    QTime time;
    QString today = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString nextDay = ui->dateEdit->date().addDays(1).toString();
    int second;
    timeList.clear();
    valueList.clear();
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
    //加载数据
    curve->setSamples(timeList.toVector(), valueList.toVector());
    //加到plot，plot由IDE创建
    curve->attach(ui->qwtPlot);

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 2200);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 86400);
    ui->qwtPlot->replot();
}

void PaintData::on_showTime_clicked()
{
    QSqlQuery query;
    QDateTime dateTime;
    QTime time;
    QString today = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString nextDay = ui->dateEdit->date().addDays(1).toString();
    int second;
    timeList.clear();
    valueList.clear();
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


    QTime time2 = ui->timeEdit->time();
    int hour = time2.hour();
    int minute = time2.minute();
    int xAxis = (hour * 60 + minute) * 60;

    //加载数据
    curve->setSamples(timeList.toVector(), valueList.toVector());
    //加到plot，plot由IDE创建
    curve->attach(ui->qwtPlot);

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 2200);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, xAxis, xAxis + 60);
    ui->qwtPlot->replot();
}

void PaintData::on_showHour_clicked()
{
    QSqlQuery query;
    QDateTime dateTime;
    QTime time;
    QString today = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString nextDay = ui->dateEdit->date().addDays(1).toString();
    int second;
    timeList.clear();
    valueList.clear();
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


    QTime time2 = ui->timeEdit->time();
    int hour = time2.hour();
    int xAxis = hour * 60 * 60;

    //加载数据
    curve->setSamples(timeList.toVector(), valueList.toVector());
    //加到plot，plot由IDE创建
    curve->attach(ui->qwtPlot);

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 2200);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, xAxis, xAxis + 3600);
    ui->qwtPlot->replot();
}
