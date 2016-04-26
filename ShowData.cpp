#include "ShowData.h"
#include "ui_ShowData.h"

#include <QSqlRecord>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>


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
    model->setHeaderData(1, Qt::Horizontal, "酒精值");
    ui->tableView->setModel(model);


    /*QSqlQuery query;
    QString str = "2016-04-13";
    //QDateTime dt;
    //dt = QDateTime::fromString(str,"yyyy-MM-dd-HH:mm:ss");
    //qDebug() << dt;
    query.prepare("select * from ValueFromSerialport where receiveTime > :str");
    query.bindValue(":str",str);
    query.exec();
    while(query.next())
    {
        qDebug() << query.value(0);
    }*/
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
