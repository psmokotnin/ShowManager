#ifndef SMACTIONEMPTY_H
#define SMACTIONEMPTY_H
#include "Action/smaction.h"

class SMActionEmpty : public SMAction
{
public:
    SMActionEmpty(QWidget *parent = 0);
    virtual QString type(void) {return "Empty";};

public slots:
    virtual void go();
    virtual void load(int time = 0) {SMAction::load(time); _loadStatus = STATUS_LOADED;}
};

#endif // SMACTIONEMPTY_H
