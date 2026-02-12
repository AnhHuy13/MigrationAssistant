#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QIcon>
#include <QSize>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listView->setViewMode(QListView::IconMode);
    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *item1 = new QStandardItem(QIcon::fromTheme("drive-harddisk"), "Ổ đĩa cứng");
    QStandardItem *item2 = new QStandardItem(QIcon::fromTheme("drive-removabledisk"), "USB");
    model->appendRow(item1);
    model->appendRow(item2);
    ui->listView->setModel(model);
    ui->listView->setGridSize(QSize(120,120));
    ui->listView->setIconSize(QSize(60,60));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_nextBtn_clicked()
{
    int curIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(curIndex+1);
}

