#pragma once

#include <QObject>
#include <QListView>
#include <QStandardItemModel>
#include <QListView>

class diskChoosePage : public QObject {
    Q_OBJECT
public:
    explicit diskChoosePage(QListView* view, QObject *parent = nullptr);
    void ScanPartition();
signals:
    void partitionClicked(QString devicePath);
private:
    QListView* m_view;
    diskChoosePage* m_diskChoosePage;
    QStandardItemModel* m_model;
};
