/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Console.h"
#include "SettingsDialog.h"
#include "ShowData.h"
#include "FindData.h"
#include "PaintData.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);
    setCentralWidget(console);

    serial = new QSerialPort(this);

    settings = new SettingsDialog;

    showData = new ShowData;

    findData = new FindData;

    paintData = new PaintData;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));


    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));

    connect(this, SIGNAL(insertIntoDB(int)), showData, SLOT(slotInsertIntoDB(int)));

    //QDateTime dateTime = QDateTime::currentDateTime();
   // QString str = dateTime.toString();
    //插入数据
    //db.exec("insert into ValueFromSerialport(receiveTime, alcoholValue) values('2016-4-12 11:19:25', '213')");

    //数据库查询
    /*QSqlTableModel model;
    model.setTable("ValueFromSerialport");
    //设置select条件，字符串加单引号
    //model.setFilter();
    model.select();  // exec query
    int ret = model.rowCount();
    // read data from database
    for(int i=0; i<ret; ++i)
    {
       QSqlRecord record = model.record(i);
       for(int j=0; j<record.count(); j++)
       {
           qDebug() << record.value(j);
       }
    }
    //更改数据
    //model.setData(model.index(0,1), 500);
    //model.submitAll();

    //插入数据
    QString strDate = QDateTime::currentDateTime().toString();
    QSqlRecord record = model.record();
    record.setValue("receiveTime", QVariant(QDateTime::currentDateTime()));
    record.setValue("alcoholValue", 300);
    model.insertRecord(-1, record);  //-1表示最后一行
    if(!model.submitAll())
        qDebug() << "tablemodel submit error!";*/

}


MainWindow::~MainWindow()
{
    delete settings;
    delete showData;
    delete findData;
    delete paintData;
    delete ui;
}
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite))
    {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();   
    if(data.startsWith("\r\n+IPD"))
    {
        if(data.length() > 16)
            return;
        QList<QByteArray> list = data.split(':');
        int value = list.at(1).toInt();
        if(list.at(0).split(',').at(2).toInt() > 0 &&
                list.at(0).split(',').at(2).toInt() < 5)
        {
            emit insertIntoDB(value);
        }
        else
            return;
    }
    console->putData(data);
}



void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::creatServer()
{
    QByteArray CIPMUX = "AT+CIPMUX=1\r\n";
    QByteArray CIPSERVER = "AT+CIPSERVER=1\r\n";
    serial->write(CIPMUX);
    //延时
    QEventLoop eventloop;
    QTimer::singleShot(1000, &eventloop, SLOT(quit()));
    eventloop.exec();
    serial->write(CIPSERVER);
}


void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionServer, SIGNAL(triggered()), this, SLOT(creatServer()));
    //connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}


void MainWindow::on_actionShowData_triggered()
{
    showData->show();
}

void MainWindow::on_actionFindData_triggered()
{
    findData->show();
}

void MainWindow::on_actionPaintData_triggered()
{
    paintData->show();
}
