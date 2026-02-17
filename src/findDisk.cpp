#include "findDisk.h"
#include <QRegularExpression>
#include <QProcess>
#include <QString>
#include <cstring>
#include <QDebug>
#include <cmath>
#include <QFile>
#include <QDir>

findDisk::findDisk(QObject *parent) : QObject(parent) {}

QString findDisk::getWinVer(const QString &devicePath) {
    QString winVer = "Windows (NTFS)";

    QProcess mountProc;
    mountProc.start("udisksctl", QStringList() << "mount" << "-b" << devicePath);
    mountProc.waitForFinished(3000);

    QProcess lsblk;
    lsblk.start("lsblk", QStringList() << "-no" << "MOUNTPOINT" << devicePath);
    lsblk.waitForFinished(1000);
    QString mountPath = QString::fromUtf8(lsblk.readAllStandardOutput()).trimmed();

    if (mountPath.isEmpty()) {
        qDebug() << "--- [LỖI]: Không lấy được Mount Path cho" << devicePath;
        return winVer;
    }
    QString winDir = mountPath + "/Windows";
    if (!QDir(winDir).exists()) winDir = mountPath + "/windows";

    QString kernelPath = winDir + "/System32/ntoskrnl.exe";
    if (!QFile::exists(kernelPath)) kernelPath = winDir + "/system32/ntoskrnl.exe";

    if (QFile::exists(kernelPath)) {
        qDebug() << "Found ntoskrnl.exe at " << kernelPath;

        QProcess stringsProc;

        QString cmd = QString("strings -e l '%1' | grep -E \"^10\\.0\\.[0-9]+\" | head -n 1").arg(kernelPath);

        stringsProc.start("sh", QStringList() << "-c" << cmd);
        stringsProc.waitForFinished(2000);

        QString output = QString::fromUtf8(stringsProc.readAllStandardOutput()).trimmed();
        qDebug() << "String output raw:" << output;

        if (output.startsWith("10.0.2")) winVer = "Windows 11";
        else if (output.startsWith("10.0.1")) winVer = "Windows 10";
        else if (output.startsWith("6.1")) winVer = "Windows 7";
        else if (!output.isEmpty()) winVer = "Windows (Build " + output + ")";
    } else {
        qDebug() << "Not found ntoskrnl.exe at " << kernelPath;
    }

    return winVer;
}
void findDisk::ScanPartition() {
    qDebug() << "Run findDisk";
    QProcess process;
    QString output;
    process.start("lsblk", QStringList() << "-nlb" << "-o" << "NAME,FSTYPE,LABEL,SIZE"); //lsblk -nlb -o NAME,FSTYPE,LABEL,SIZE
    if (!process.waitForFinished()) {
        qDebug() << "Process lsblk failed to finish:" << process.errorString();
    }
    QByteArray standardOutput = process.readAllStandardOutput();
    output = QString(standardOutput);
    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        qDebug() << "Process lsblk exited with errors";
    }
    QStringList lines = output.split(QRegularExpression("[\r\n]"), QString::SkipEmptyParts);
    for (const QString &line : lines){
        if (line.isEmpty()){
            continue;
        }
        if (line.contains("ntfs")){
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
            QString devPath = "/dev/" + parts.at(0);
            QString winVer = getWinVer(devPath);

            emit sentPartition(devPath,parts.at(1),labelShow,sizeRounding,unit,winVer);
        }
    }
}
