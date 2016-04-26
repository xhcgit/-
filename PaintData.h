#ifndef PAINTDATA_H
#define PAINTDATA_H

#include <QWidget>
#include <qwt_plot_curve.h>

namespace Ui {
class PaintData;
}

class PaintData : public QWidget
{
    Q_OBJECT

public:
    explicit PaintData(QWidget *parent = 0);
    ~PaintData();
signals:
    void queryEnd();

private slots:
    void on_pushButton_clicked();
    void beginPaintData();

private:
    Ui::PaintData *ui;
    QList<double> timeList;
    QList<double> valueList;
    QwtPlotCurve* curve;
};

#endif // PAINTDATA_H
