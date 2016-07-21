#ifndef SMACTIONAUDIO_H
#define SMACTIONAUDIO_H
#include "Action/smaction.h"

class SMActionAudio : public SMAction
{
public:
    SMActionAudio(QWidget *parent = 0);
    virtual QString type(void) {return "Audio";};

public slots:
    virtual void load(int time = 0) {SMAction::load(time); _loadStatus = STATUS_LOADED;}
};

#endif // SMACTIONAUDIO_H
