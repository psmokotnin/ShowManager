#include "smactionstop.h"
#include "smactionlist.h"

SMActionStop::SMActionStop(QWidget *parent):
    SMAction(parent)
{
    getContextMenu()->addSeparator();
    QAction* selectActionAction = getContextMenu()->addAction("set action id");
    connect(selectActionAction, SIGNAL(triggered(bool)), SLOT(slotSetActionId()));
}
SMActionStop::~SMActionStop()
{

}

void SMActionStop::setActionId(int id)
{
    _actionId = id;
    disconnect(SIGNAL(stopped(SMAction*)), this);
    if (id > 0)
    {
        setTitle("Stop action #" + QString::number(id));
        SMAction* action = getActionList()->getActionById(actionId());
        connect(action, SIGNAL(stopped(SMAction*)), this, SLOT(actionStopped(SMAction*)));
    }
    else if (id == -1)
    {
        setTitle("Stop all actions");
        for (int i = 0; i < getActionList()->count(); i++)
        {
            SMAction* action = getActionList()->getActionItem(i);
            if (action && action != this)
            {
                connect(action, SIGNAL(stopped(SMAction*)), this, SLOT(actionStopped(SMAction*)));
            }
        }
    }
}
void SMActionStop::go()
{
    bool endImmediately = true;
    SMAction::go();
    getProgressBar()->setValue(0);

    if (actionId() > 0)
    {
        SMAction* action = getActionList()->getActionById(actionId());
        if (action && action != this && action->getStatus() != STATUS_STOP)
        {
            endImmediately = false;
            action->stop();
        }
    }
    else if (actionId() == -1)
    {qInfo() << "STOP ALLL GO " << getId();
        getProgressBar()->setValue(50);
        //all
        for (int i = 0; i < getActionList()->count(); i++)
        {
            SMAction* action = getActionList()->getActionItem(i);
            if (action && action != this && action->getStatus() != STATUS_STOP)
            {
                endImmediately = false;
                qInfo() << "    wait for stop " << action->getId() << " " << action->title() << " status: " << action->getStatus();
                action->stop();
            }
            if (getStatus() == STATUS_STOP)
                break;
        }
    }
    if (endImmediately)
    {
        emit end(this);
    }
}
void SMActionStop::stop()
{qInfo() << "        STOP " << getId();
    SMAction::stop();
}
void SMActionStop::onEnd()
{
    getProgressBar()->setValue(100);
    SMAction::onEnd();
}

/**
 * @brief SMActionStop::actionStopped
 * @param stoppedAction
 *
 * send end signal when all actions is stopped
 */
void SMActionStop::actionStopped(SMAction *stoppedAction)
{qInfo() << "        slot actionStopped " << getId() << " " << stoppedAction->getId();
    if (getStatus() != STATUS_PLAY)
        return;

    qInfo() << "        slot stopped " << stoppedAction->getId() << " " << stoppedAction->title();
    if (actionId() > 0 && actionId() == stoppedAction->getId() && stoppedAction->getStatus() == STATUS_STOP)
    {
        emit end(this);
    }
    else if (actionId() == -1)
    {
        //all
        for (int i = 0; i < getActionList()->count(); i++)
        {
            SMAction* action = getActionList()->getActionItem(i);
            if (action && (action != this) && (action->getStatus() != STATUS_STOP))
            {qInfo() << "           still wait for " << action->getId() << " " << action->title();
                action->stop();
                return;
            }
        }
        emit end(this);
    }
}

void SMActionStop::slotSetActionId(void)
{
    bool ok;
    int id = QInputDialog::getInt(this, tr("Select Action"), tr("action id:"),
                                        actionId(), -1, getActionList()->getMaxId(), 1, &ok);
    if (ok)
        setActionId(id);
}
QDomElement SMActionStop::createDomElement(QDomDocument& document)
{
    QDomElement element = SMAction::createDomElement(document);

    //actionId
    QDomElement actionIdElement = document.createElement("actionId");
    actionIdElement.appendChild(document.createTextNode(QString::number(actionId())));
    element.appendChild(actionIdElement);

    return element;
}
void SMActionStop::readSettingsFromNode(QDomNode node)
{
    SMAction::readSettingsFromNode(node);
    node = node.firstChild();
    while(!node.isNull())
    {
        if(!node.isElement())
        {
            node = node.nextSibling();
            continue;
        }
        QDomElement element = node.toElement();
        QString name = element.tagName();
        if (name.toLower() == "actionid")
            setActionId(element.text().toInt());

        node = node.nextSibling();
    }
}
