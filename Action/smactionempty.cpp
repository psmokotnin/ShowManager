#include "smactionempty.h"

SMActionEmpty::SMActionEmpty(QWidget *parent):
    SMAction(parent)
{

}
void SMActionEmpty::go(void)
{
    SMAction::go();
    emit end(this);
}
