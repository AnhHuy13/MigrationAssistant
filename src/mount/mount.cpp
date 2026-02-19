#include "mount.h"
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

MountDisks::MountDisks(QObject *parent) : QObject(parent) {}

bool MountDisks::isUserRoot() const {
    return geteuid() == 0;
}

bool MountDisks::isDirectoryBlacklisted(const QString &targetPath) const {
    QString absolutePath = QDir(targetPath).absolutePath();
    QStringList systemPaths = {"/", "/etc", "/bin", "/sbin", "/lib", "/usr", "/boot", "/home", "/root", "/var"};

    for (const QString &path : systemPaths) {
        if (absolutePath == path || absolutePath.startsWith(path + "/")) {
            return true;
        }
    }
    return false;
}

bool MountDisks::mountPartition(const QString &devicePath, const QString &mountPoint) {
    if (!isUserRoot()) {
        qDebug() << "!!!! Error: Application must be run with root privileges to mount partitions. !!!!";
        return false;
    }

    if (isDirectoryBlacklisted(mountPoint)) {
        qDebug() << "Error: Attempted to mount on a restricted system directory:" << mountPoint;
        return false;
    }

    QDir dir;
    if (!dir.exists(mountPoint)) {
        if (!dir.mkpath(mountPoint)) {
            qDebug() << "Error: Failed to create mount point directory.";
            return false;
        }
    }

    int result = mount(devicePath.toUtf8().constData(),mountPoint.toUtf8().constData(),"ntfs3", MS_RDONLY, NULL);

    if (result != 0) {
        qDebug() << "ntfs3 failed, attempting legacy ntfs driver...";
        result = mount(devicePath.toUtf8().constData(),mountPoint.toUtf8().constData(),"ntfs", MS_RDONLY, NULL);
    }

    if (result == 0) {
        qDebug() << "Successfully mounted" << devicePath << "to" << mountPoint;
        return true;
    } else {
        qDebug() << "Mount failed with error code:" << errno;
        return false;
    }
}
