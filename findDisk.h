#ifndef FINDDISK_H
#define FINDDISK_H

#include <QObject>
#include <QString>

class findDisk : public QObject
{
    Q_OBJECT
public:
    explicit findDisk(QObject *parent = nullptr);

    void ScanPartition();

signals:
    void sentPartition(QString name, QString type, QString label, long size);
};

#endif
