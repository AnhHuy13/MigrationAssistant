#include "findDisk.h"
#include <QRegularExpression>
#include <QProcess>
#include <QString>
#include <cstring>
#include <QDebug>
#include <cmath>

findDisk::findDisk(QObject *parent) : QObject(parent) {}

void findDisk::ScanPartition() {
    qDebug() << "Run findDisk";
    QProcess process;
    QString output;
    process.start("lsblk", QStringList() << "-nlb" << "-o" << "NAME,FSTYPE,LABEL,SIZE"); //lsblk --nlb -o NAME,FSTYPE,LABEL,SIZE
    if (!process.waitForFinished()) {
        qDebug() << "Process failed to finish:" << process.errorString();
    }
    QByteArray standardOutput = process.readAllStandardOutput();
    output = QString(standardOutput);
    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        qDebug() << "Process exited with errors";
    }
    QStringList lines = output.split(QRegularExpression("[\r\n]"), QString::SkipEmptyParts);
    for (const QString &line : lines){
        if (line.isEmpty()){
            continue;
        }
        if (line.contains("ntfs")){
            qDebug() << "Has found the one";
            line.split(QRegularExpression("\\s+"));
            qDebug() << line;
            QStringList parts = line.split(QRegularExpression("\\s+"),Qt::SkipEmptyParts);
            qDebug() << parts;

            QString labelShow;
            qulonglong sizeShow;

            if (parts.size() == 4) {
                labelShow = parts.at(2);
                sizeShow = parts.at(3).toULongLong();
            } else {
                labelShow = "No label";
                sizeShow = parts.at(2).toULongLong();
            }

            double sizeRounding;
            QString unit;

            qulonglong tb = 1099511627776;
            unsigned long gb = 1073741824;
            float mb = 1048576;
            float kb = 1024;
            double size = static_cast<double>(sizeShow);

            if (sizeShow >= tb) {
                sizeRounding = std::round(size / tb * 100.0) / 100.0;
                unit = " TB";
            }
            else if (sizeShow >= gb) {
                sizeRounding = std::round(size / gb * 100.0) / 100.0;
                unit = " GB";
            }
            else if (sizeShow >= mb) {
                sizeRounding = std::round(size / mb * 100.0) / 100.0;
                unit = " MB";
            }
            else if (sizeShow >= kb) {
                sizeRounding = std::round(size / kb * 100.0) / 100.0;
                unit = " KB";
            }
            else {
                sizeRounding = (sizeShow);
                unit = " B";
            }

            emit sentPartition("/dev/"+QString(parts.at(0)),QString(parts.at(1)),labelShow,sizeRounding,unit);
        }
    }
}
