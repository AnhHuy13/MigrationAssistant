#pragma once

#include <QObject>
#include <QListView>
#include <QStandardItemModel>
#include <QListView>
#include "findDisk.h">

class dcPage : public QObject {
    Q_OBJECT
public:
    explicit dcPage(QListView* view, QObject *parent = nullptr);

    void ScanPartition();
private:
    QListView* m_view;
    dcPage *m_dcPage;
    QStandardItemModel* m_model;
};
