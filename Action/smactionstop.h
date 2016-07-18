#ifndef SMACTIONSTOP_H
#define SMACTIONSTOP_H

#include "Action/smaction.h"

class SMActionStop : public SMAction
{
    Q_OBJECT

    int _actionId;

public:
    SMActionStop(QWidget *parent = 0);
    virtual QString type(void) {return "Stop";}
    QDomElement createDomElement(QDomDocument& document);
    virtual void readSettingsFromNode(QDomNode node);

    void setActionId(int id);
    int actionId(){return _actionId;}

public slots:
    virtual void go();
    virtual void stop();
    void slotSetActionId(void);
    virtual void load(int time = 0) {SMAction::load(time); _loaded = true;}
    void actionStopped(SMAction *stoppedAction);
};

#endif // SMACTIONSTOP_H
