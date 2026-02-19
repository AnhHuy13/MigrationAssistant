#pragma once
#include <QObject>
#include <QString>

class MountDisks : public QObject
{
    Q_OBJECT

public:
    explicit MountDisks(QObject *parent = nullptr);

    bool mountPartition(const QString &devicePath, const QString &mountPoint);

private:
    bool isUserRoot() const;
    bool isDirectoryBlacklisted(const QString &targetPath) const;
};
