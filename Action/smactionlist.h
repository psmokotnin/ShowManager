#ifndef SMACTIONMANAGER_H
#define SMACTIONMANAGER_H

#include <QObject>
#include <QDebug>
#include <QListWidget>

#include "Action/smaction.h"
#include "Action/smactionvideo.h"
#include "Action/smactionempty.h"
#include "Action/smactionaudio.h"
#include "Action/smactionplay.h"
#include "Action/smactionstop.h"
#include "Action/smactionopacity.h"
#include "Action/smactionwait.h"


class SMActionList : public QListWidget
{
    Q_OBJECT

    SMApplication* application;
    QTimer* _keyTimer;

    bool _busy;

public:
    explicit SMActionList(QWidget *parent = 0);
    ~SMActionList();
    SMAction* getActionItem(int i);

    void addAction(SMAction* action);
    void removeActions();

    void setApplication(SMApplication* app){application = app;}
    SMApplication* getApplication(){return application;}

    SMAction* getActionById(int id);
    int calcNewId(void);
    int getMaxId(void);
    void setBusy(bool busy){_busy = busy;}
    bool isBusy(){return _busy;}


public:
    virtual void keyPressEvent(QKeyEvent *keyEvent);

signals:
    void keyPressed(QString key);

public slots:
    void addVideoActionMenu(QString file = NULL);
    void addEmptyActionMenu();
    void addPlayAction();
    void addStopAction();
    void addOpacityAction();
    void addWaitAction();
    void removeSMAction(SMAction *action);

    void go(void);
    void goNext(SMAction* a);
    void loadNext(SMAction* a);
    void stopAll(void);
    void stopSelected(void);
    void keySetted(SMAction* a);

protected slots:
    void slotKeyTimeout(void);
};

#endif // SMACTIONMANAGER_H
