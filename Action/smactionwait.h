#ifndef SMACTIONWAIT_H
#define SMACTIONWAIT_H

#include "Action/smaction.h"

class SMActionWait : public SMAction
{
    Q_OBJECT

    int _waitTime;
    int _currentTime;
    int _timeStep;
    QTimer* _timer;

public:
    SMActionWait(QWidget *parent = 0);
    ~SMActionWait();
    virtual QString type(void) {return "Wait";};
    virtual void readSettingsFromNode(QDomNode node);

    void setWaitTime(int time);
    int waitTime(){return _waitTime;}

    QDomElement createDomElement(QDomDocument& document);

protected slots:
    void slotTimeout(void);

public slots:
    virtual void go();
    virtual void stop();
    virtual void slotSetTime(void);
    virtual void load(int time = 0) {SMAction::load(time); _loaded = true;}
};

#endif // SMACTIONWAIT_H
