#ifndef FINDDATA_H
#define FINDDATA_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>

namespace Ui {
class FindData;
}

class FindData : public QWidget
{
    Q_OBJECT

public:
    explicit FindData(QWidget *parent = 0);
    ~FindData();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FindData *ui;
    QSqlQueryModel* queryModel;

};

#endif // FINDDATA_H
