#ifndef SMACTIONOPACITY_H
#define SMACTIONOPACITY_H

#include "Action/smaction.h"

class SMActionOpacity : public SMAction
{

    Q_OBJECT

    int _actionId;
    qreal _opacity;

public:
    SMActionOpacity(QWidget *parent = 0);
    virtual QString type(void) {return "Opacity";};
    virtual void readSettingsFromNode(QDomNode node);

    void setActionId(int id);
    int actionId(){return _actionId;}

    void setTitle(QString title = QString());

    void setOpacity(qreal o);
    qreal opacity(){return _opacity;}

    QDomElement createDomElement(QDomDocument& document);

public slots:
    virtual void go();
    virtual void slotSetActionId(void);
    virtual void slotSetOpacity(void);
    virtual void load(int time = 0) {SMAction::load(time); _loadStatus = STATUS_LOADED;}
};

#endif // SMACTIONOPACITY_H
