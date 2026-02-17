#include "diskChoosePage.h"
#include <QIcon>
#include <QStandardItem>

dcPage::dcPage(QListView* view, QObject *parent) : QObject(parent), m_view(view) {
    m_model = new QStandardItemModel(this);
    m_view->setModel(m_model);
}

void dcPage::ScanPartition() {
    m_model->clear();
    findDisk *fd = new findDisk(this);
    m_view->setViewMode(QListView::IconMode);
    m_view->setModel(m_model);
    m_view->setGridSize(QSize(180,160));
    m_view->setIconSize(QSize(60,60));
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(fd, &findDisk::sentPartition, this, [this](QString name, QString type, QString label, double size, QString unit, QString Winver){
        QString showString = QString("Name: %1\nLabel: %2\nSize: %3%4\nVersion: %5")
                                 .arg(name).arg(label).arg(size).arg(unit).arg(Winver);

        QString iconPath = ":/assets/disk/drive-harddisk.png";
        if (Winver.contains("11")) iconPath = ":/assets/disk/diskWin11.png";
        else if (Winver.contains("10")) iconPath = ":/assets/disk/diskWin10.png";
        else if (Winver.contains("8.1")) iconPath = ":/assets/disk/diskWin81.png";
        else if (Winver.contains("8")) iconPath = ":/assets/disk/diskWin8.png";
        else if (Winver.contains("7")) iconPath = ":/assets/disk/diskWin7.png";

        QStandardItem *item = new QStandardItem(QIcon(iconPath), showString);
        item->setData(name, Qt::UserRole);
        m_model->appendRow(item);
    });

    fd->ScanPartition();
}
