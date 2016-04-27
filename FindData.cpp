#include "FindData.h"
#include "ui_FindData.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QVBoxLayout>

FindData::FindData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindData)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    queryModel = new QSqlQueryModel;
    ui->tableView->setModel(queryModel);
}

FindData::~FindData()
{
    delete queryModel;
    delete ui;
}

//查询当天的数据
void FindData::on_pushButton_clicked()
{

    QSqlQuery query;
    QDateTime todayDateTime(ui->dateEdit->date());
    QDateTime nextdayDateTime(ui->dateEdit->date().addDays(1));
    QString today = todayDateTime.toString("yyyy-MM-dd-HH:mm:ss");
    QString nextDay = nextdayDateTime.toString("yyyy-MM-dd-HH:mm:ss");

    queryModel->setQuery(QString("select * from ValueFromSerialport where receiveTime > '%1' and receiveTime < '%2' ")
                         .arg(today).arg(nextDay));
    queryModel->query();
    queryModel->setHeaderData(0, Qt::Horizontal, "时间");
    queryModel->setHeaderData(1, Qt::Horizontal, "酒精值");
}
