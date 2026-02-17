#pragma once

#include <QObject>
#include <QString>

class findDisk : public QObject
{
    Q_OBJECT
public:
    explicit findDisk(QObject *parent = nullptr);

    void ScanPartition();

private:
    QString getWinVer(const QString &devicePath);

signals:
    void sentPartition(QString name, QString type, QString label, double size, QString unit, QString Winver);
};
