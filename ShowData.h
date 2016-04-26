#ifndef SHOWDATA_H
#define SHOWDATA_H

#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

namespace Ui {
class ShowData;
}

class ShowData : public QWidget
{
    Q_OBJECT

public:
    explicit ShowData(QWidget *parent = 0);
    ~ShowData();

private slots:
    void on_ButtonCancel_clicked();

private:
    Ui::ShowData *ui;
    QSqlTableModel* model;
    QTableView* view;
    //QSqlQueryModel* queryModel;
public slots:
    void slotInsertIntoDB(int value);
};

#endif // SHOWDATA_H
