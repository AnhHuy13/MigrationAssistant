#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "diskChoosePage.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_dcPage = new dcPage(ui->listView, this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_nextBtn_clicked() {
    int curIndex = ui->stackedWidget->currentIndex();

    if (curIndex == 0) {
        ui->stackedWidget->setCurrentIndex(1);
        if (m_dcPage) {
            m_dcPage->ScanPartition();
        }
    }
    else if (curIndex == 1) {
        ui->stackedWidget->setCurrentIndex(2);
    }
}
