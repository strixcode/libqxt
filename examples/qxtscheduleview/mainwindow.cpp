#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createModel();
}

void MainWindow::createModel ()
{
    QStandardItemModel * model = new QStandardItemModel(10,1,ui->centralWidget);

    QDateTime time = QDateTime(QDate(2010,1,1),QTime(8,0,0));
    ui->centralWidget->setDateRange(time.date(),time.date().addDays(6));

    for (int row = 0; row < 100; ++row) {
        int column = 0;
        QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));

        item->setData(time.toTime_t(),Qxt::ItemStartTimeRole);
        item->setData(900*4,Qxt::ItemDurationRole);
        item->setData(QColor(qrand()%255,qrand()%255,qrand()%255),Qt::BackgroundRole);

        qDebug()<<"Data: "<<item->data(Qxt::ItemStartTimeRole);

        time = time.addSecs(900*2);

        model->setItem(row, column, item);
    }

    ui->centralWidget->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}
