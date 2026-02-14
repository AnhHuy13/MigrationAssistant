#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QIcon>
#include <QSize>
#include <QStackedWidget>
#include "findDisk.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listView->setViewMode(QListView::IconMode);
    QStandardItemModel *model = new QStandardItemModel(this);
    /*QStandardItem *item1 = new QStandardItem(QIcon::fromTheme("drive-harddisk"), "Ổ đĩa cứng");
    QStandardItem *item2 = new QStandardItem(QIcon::fromTheme("drive-removabledisk"), "USB");
    model->appendRow(item1);
    model->appendRow(item2);*/
    ui->listView->setModel(model);
    ui->listView->setGridSize(QSize(180,160));
    ui->listView->setIconSize(QSize(60,60));
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    findDisk *fd = new findDisk(this);
    connect(fd, &findDisk::sentPartition, this, [this, model](QString name, QString type, QString label, double size, QString unit, QString Winver){
        qDebug() << "Slot received (name,type,label,unit,winver):" << name << type << label << unit << Winver;

        QString showString = QString("Name: %1\nLabel: %2\nSize: %3%4\nVersion: %5")
                                 .arg(name).arg(label).arg(size).arg(unit).arg(Winver);

        QString iconPath = ":/assets/disk/drive-harddisk.png";

        if (Winver.contains("11")){
            iconPath = ":/assets/disk/diskWin11.png";
        }
        else if (Winver.contains("10")){
            iconPath = ":/assets/disk/diskWin10.png";
        }
        else if (Winver.contains("8.1")){
            iconPath = ":/assets/disk/diskWin81.png";
        }
        else if (Winver.contains("8")){
            iconPath = ":/assets/disk/diskWin8.png";
        }
        else if (Winver.contains("7")){
            iconPath = ":/assets/disk/diskWin7.png";
        }
        else {
            iconPath = ":/assets/disk/drive-harddisk.png";
        }

        QIcon icon(iconPath);

        if (icon.isNull()) {
            qDebug() << "Resource fail:" << iconPath;
            icon = QIcon::fromTheme("drive-harddisk");
        }

        QStandardItem *item = new QStandardItem(icon, showString);

        item->setData(name, Qt::UserRole);

        model->appendRow(item);
    });
    fd->findDisk::ScanPartition();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_nextBtn_clicked()
{
    int curIndex = ui->stackedWidget->currentIndex();
    if (curIndex == 1) {
        QModelIndex curIdxOfList = ui->listView->currentIndex();
        if (!curIdxOfList.isValid()) {
                QMessageBox errorNChoseADisk;
                errorNChoseADisk.setIcon(QMessageBox::Warning);
                errorNChoseADisk.setText("Warning!");
                errorNChoseADisk.setInformativeText("Please choose a disk.");
                errorNChoseADisk.exec();
        }
        QString selectedDisk = curIdxOfList.data(Qt::UserRole).toString();
        qDebug() << "selection Disk:" << selectedDisk;

        int curIndex = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(curIndex + 1);
    }
    ui->stackedWidget->setCurrentIndex(curIndex+1);
}

