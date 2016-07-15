#include "smactionstop.h"
#include "smactionlist.h"

SMActionStop::SMActionStop(QWidget *parent):
    SMAction(parent)
{
    getContextMenu()->addSeparator();
    QAction* selectActionAction = getContextMenu()->addAction("set action id");
    connect(selectActionAction, SIGNAL(triggered(bool)), SLOT(slotSetActionId()));
}

void SMActionStop::setActionId(int id)
{
    _actionId = id;
    if (id > 0)
        setTitle("Stop action #" + QString::number(id));
    else if (id == -1)
        setTitle("Stop all actions");
}
void SMActionStop::go()
{
    SMAction::go();
    if (actionId() > 0)
    {
        if (actionId() > 0)
        {
            SMAction* action = getActionList()->getActionById(actionId());
            if (action)
            {
                action->stop();
            }
        }
    }
    else if (actionId() == -1)
    {
        //all
        for (int i = 0; i < getActionList()->count(); i++)
        {
            SMAction* action = getActionList()->getActionItem(i);
            if (action)
            {
                action->stop();
            }
        }
    }
    getProgressBar()->setValue(100);
    emit end(this);
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
