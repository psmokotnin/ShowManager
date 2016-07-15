#include "smactionplay.h"
#include "smactionlist.h"

SMActionPlay::SMActionPlay(QWidget *parent):
    SMAction(parent)
{
    getContextMenu()->addSeparator();
    QAction* selectActionAction = getContextMenu()->addAction("set action id");
    connect(selectActionAction, SIGNAL(triggered(bool)), SLOT(slotSetActionId()));
}

void SMActionPlay::setActionId(int id)
{
    _actionId = id;
    //if (title().isEmpty())
        setTitle("Play action #" + QString::number(id));
}
void SMActionPlay::go()
{
    SMAction::go();
    if (actionId())
    {
        SMAction* action = getActionList()->getActionById(actionId());
        if (action)
        {
            action->go();
        }
    }
    emit end(this);
}
void SMActionPlay::slotSetActionId(void)
{
    bool ok;
    int id = QInputDialog::getInt(this, tr("Select Action"), tr("action id:"),
                                        actionId(), 0, getActionList()->getMaxId(), 1, &ok);
    if (ok && id)
        setActionId(id);
}
QDomElement SMActionPlay::createDomElement(QDomDocument& document)
{
    QDomElement element = SMAction::createDomElement(document);

    //actionId
    QDomElement actionIdElement = document.createElement("actionId");
    actionIdElement.appendChild(document.createTextNode(QString::number(actionId())));
    element.appendChild(actionIdElement);

    return element;
}
void SMActionPlay::readSettingsFromNode(QDomNode node)
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
