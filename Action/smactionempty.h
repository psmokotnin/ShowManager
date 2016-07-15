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
    virtual void load(int time = 0) {SMAction::load(time); _loaded = true;}
};

#endif // SMACTIONEMPTY_H
