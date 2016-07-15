#include "smactionempty.h"

SMActionEmpty::SMActionEmpty(QWidget *parent):
    SMAction(parent)
{

}
void SMActionEmpty::go(void)
{
    SMAction::go();
    setStatus(STATUS_STOP);
    emit end(this);
}
