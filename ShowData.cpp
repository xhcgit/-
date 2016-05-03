#include "ShowData.h"
#include "ui_ShowData.h"

#include <QSqlRecord>
#include <QVBoxLayout>
#include <QDateTime>
#include <QSqlQuery>
#include <QDebug>


ShowData::ShowData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowData)
{
    ui->setupUi(this);
    model = new QSqlTableModel;
    model->setTable("ValueFromSerialport");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, "时间");
    model->setHeaderData(1, Qt::Horizontal, "酒精浓度值 mg/100ml");
    ui->tableView->setModel(model);
}

ShowData::~ShowData()
{
    delete ui;
}

void ShowData::on_ButtonCancel_clicked()
{
    hide();
}

void ShowData::slotInsertIntoDB(int value)
{
    //插入数据
    QSqlRecord record = model->record();
    record.setValue("receiveTime", QVariant(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH:mm:ss")));
    record.setValue("alcoholValue", value);
    model->insertRecord(-1, record);  //-1表示最后一行
    if(!model->submitAll())
        qDebug() << "tablemodel submit error!";
    else
        qDebug() << "insert success!";
}
