#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "Action/smactionlist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SMActionList* getListWidget();
    QString saveDialog(void);

protected:
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();

    void on_actionClear_triggered();

private:
    Ui::MainWindow *ui;

signals:
    void save(QString str = "");
    void open(QString str);
    void clear(void);
};

#endif // MAINWINDOW_H
