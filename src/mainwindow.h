#pragma once
#include <QMainWindow>
#include "diskChoosePage.h"//

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_nextBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString getWindowsIcon(const QString& winVer, int version);
    dcPage *m_dcPage;

signals: void sendPartition(QString name ,QString type,QString label,qulonglong size);
};
