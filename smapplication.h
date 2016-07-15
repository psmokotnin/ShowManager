#ifndef SMAPPLICATION_H
#define SMAPPLICATION_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QtWidgets>
#include <QtXml>

class MainWindow;
class SMView;

class SMApplication : public QObject
{
    Q_OBJECT
public:
    explicit SMApplication(QObject *parent = 0);
    void showWindow(void);
    virtual ~SMApplication();
    QList<SMView*> viewList;
    SMView* getView(int i);
    int getViewCount();

private:
    MainWindow* w;
    QSettings* settings;

    QString projectFile;
    void setProjectFile(QString name);
    void readActionsNode(QDomNode node);
    void readActionNode(QDomNode node);

signals:
    void projectFileChanged(QString filename);

public slots:
    void saveProject(QString str = "");
    void openProject(QString str);
    void clearProject(void);
    void exit(void);
    void slotKeyPressed(QKeyEvent *keyEvent);
};

#endif // SMAPPLICATION_H
