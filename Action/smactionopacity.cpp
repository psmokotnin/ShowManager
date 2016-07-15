#include "smactionopacity.h"
#include "smactionlist.h"

SMActionOpacity::SMActionOpacity(QWidget *parent):
    SMAction(parent)
{
    setOpacity(1.0);
    setActionId(0);

    getContextMenu()->addSeparator();
    QAction* selectActionAction = getContextMenu()->addAction("set action id");
    connect(selectActionAction, SIGNAL(triggered(bool)), SLOT(slotSetActionId()));

    QAction* setOpacityAction = getContextMenu()->addAction("set opacity");
    connect(setOpacityAction, SIGNAL(triggered(bool)), SLOT(slotSetOpacity()));
}
void SMActionOpacity::setActionId(int id)
{
    _actionId = id;
    setTitle();
}
void SMActionOpacity::setOpacity(qreal o)
{
    _opacity = o;
    setTitle();
}
void SMActionOpacity::setTitle(QString title)
{
    if (title.isEmpty())
    {
        if (actionId() > 0)
            setTitle("Opacity " + QString::number(opacity()) + " #" + QString::number(actionId()));
        else if (actionId() == -1)
            setTitle("Opacity " + QString::number(opacity()) + " all");
        else
            setTitle("Opacity " + QString::number(opacity()) + " none");
    }
    else
        SMAction::setTitle(title);
}

void SMActionOpacity::go()
{
    SMAction::go();
    if (actionId() > 0)
    {
        qInfo() << "set opacity " << opacity() << " #" << getId();
        SMActionVideo* action = (SMActionVideo*) getActionList()->getActionById(actionId());
        if (action && action->type() == "Video")
        {
            action->setOpacity(opacity());
            qInfo() << "seted opacity " << opacity() << " #" << getId();
        }
    }
    else if (actionId() == -1)
    {
        //all
        for (int i = 0; i < getActionList()->count(); i++)
        {
            SMActionVideo* action = (SMActionVideo*)getActionList()->getActionItem(i);
            if (action && action->type() == "Video")
            {
                action->setOpacity(opacity());
            }
        }
    }
    emit end(this);
}
void SMActionOpacity::slotSetActionId(void)
{
    bool ok;
    int id = QInputDialog::getInt(this, tr("Select video Action"), tr("action id:"),
                                        actionId(), -1, getActionList()->getMaxId(), 1, &ok);
    if (ok && id)
        setActionId(id);
}
void SMActionOpacity::slotSetOpacity(void)
{
    bool ok;
    qreal op = (qreal) QInputDialog::getDouble(this, tr("Set opacity"), tr("opacity value:"),
                                        opacity(), 0.0, 1.0, 1, &ok);
    if (ok)
        setOpacity(op);
}
QDomElement SMActionOpacity::createDomElement(QDomDocument& document)
{
    QDomElement element = SMAction::createDomElement(document);

    //actionId
    QDomElement actionIdElement = document.createElement("actionId");
    actionIdElement.appendChild(document.createTextNode(QString::number(actionId())));
    element.appendChild(actionIdElement);

    //opacity
    QDomElement opacityElement = document.createElement("opacity");
    opacityElement.appendChild(document.createTextNode(QString::number(opacity())));
    element.appendChild(opacityElement);

    return element;
}

void SMActionOpacity::readSettingsFromNode(QDomNode node)
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
        if (name.toLower() == "opacity")
            setOpacity(element.text().toDouble());

        node = node.nextSibling();
    }
}
