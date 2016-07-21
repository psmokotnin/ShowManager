#ifndef SMACTIONPLAY_H
#define SMACTIONPLAY_H

#include "Action/smaction.h"

class SMActionPlay : public SMAction
{
    Q_OBJECT

    int _actionId;

public:
    SMActionPlay(QWidget *parent = 0);
    virtual QString type(void) {return "Play";};
    virtual void readSettingsFromNode(QDomNode node);

    void setActionId(int id);
    int actionId(){return _actionId;}

    QDomElement createDomElement(QDomDocument& document);

public slots:
    virtual void go();
    virtual void slotSetActionId(void);
    virtual void load(int time = 0) {SMAction::load(time); _loadStatus = STATUS_LOADED;}
};

#endif // SMACTIONPLAY_H
