#include "findDisk.h"
#include <blkid/blkid.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <QDebug>//

findDisk::findDisk(QObject *parent) : QObject(parent) {}

void findDisk::ScanPartition() {
    qDebug() << "Run findDisk";
    blkid_cache cache;
    if (blkid_get_cache(&cache, NULL) < 0) {
        qDebug() << "Cache error blkid!";
        return;
    };

    blkid_probe_all(cache);
    blkid_dev_iterate iter = blkid_dev_iterate_begin(cache);
    blkid_dev dev;

    while (blkid_dev_next(iter, &dev) == 0) {
        const char *devName = blkid_dev_devname(dev);
        int fd = open(devName, O_RDONLY);
        if (fd != -1) {
            long size = blkid_get_dev_size(fd);
            close(fd);
            const char *type = blkid_get_tag_value(cache, "TYPE", devName);

            if (type && strstr(type, "ntfs")) {
                const char *label = blkid_get_tag_value(cache, "LABEL", devName);
                qDebug() << "Found: " << devName;
                emit sentPartition(QString(devName), QString(type), label ? label : "No Label", size);
            }
        }
    }
    blkid_dev_iterate_end(iter);
    blkid_put_cache(cache);
}
