#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
SMActionList* MainWindow::getListWidget()
{
    return ui->listWidget;
}
QString MainWindow::saveDialog(void)
{
    return QFileDialog::getSaveFileName(0, "Save project", "", "*.smp");
}
void MainWindow::on_actionSave_triggered()
{
    emit save();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = saveDialog();
    if (!fileName.isEmpty())
        emit save(fileName);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Open project", "", "*.smp");
    if (!fileName.isEmpty())
        emit open(fileName);
}

void MainWindow::on_actionClear_triggered()
{
    int n = QMessageBox::question(0, "Clear project",
                                  "Are you realy want to clear project?",
                                  QMessageBox::Yes,
                                  QMessageBox::No);
    if (n == QMessageBox::Yes)
        emit clear();
}
void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    getListWidget()->keyPressEvent(keyEvent);
    QMainWindow::keyPressEvent(keyEvent);
}
