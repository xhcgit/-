#include "ShowData.h"
#include "ui_ShowData.h"

#include <QSqlRecord>
#include <QVBoxLayout>
#include <QDateTime>
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
