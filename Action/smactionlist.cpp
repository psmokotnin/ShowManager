
#include <QFileDialog>
#include "Action/smactionlist.h"

SMActionList::SMActionList(QWidget *parent) : QListWidget(parent)
{
    _keyTimer = new QTimer(this);
    connect(_keyTimer, SIGNAL(timeout()), SLOT(slotKeyTimeout()));
}
SMActionList::~SMActionList()
{
    delete _keyTimer;
}

void SMActionList::addVideoActionMenu(QString file)
{
    if (file == NULL)
        file = QFileDialog::getOpenFileName(0, tr("Open video file"), "","");
    if (file.isEmpty())
        return;
    SMAction* action;
    action = new SMActionVideo(this);
    action->setMedia(file);

    addAction(action);
}
void SMActionList::addEmptyActionMenu()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add action"),
                                         tr("Title:"), QLineEdit::Normal,
                                         "new action", &ok);
    if (!ok || text.isEmpty())
        return;

    SMAction* action;
    action = new SMActionEmpty(this);
    action->setTitle(text);
    addAction(action);
}
void SMActionList::addPlayAction()
{
    SMActionPlay* action;
    action = new SMActionPlay(this);
    action->setActionId(1);

    addAction(action);
}
void SMActionList::addStopAction()
{
    SMActionStop* action;
    action = new SMActionStop(this);
    action->setActionId(1);

    addAction(action);
}
void SMActionList::addOpacityAction()
{
    SMActionOpacity* action;
    action = new SMActionOpacity(this);

    addAction(action);
}
void SMActionList::addWaitAction()
{
    SMActionWait* action;
    action = new SMActionWait(this);

    addAction(action);
}
void SMActionList::addAction(SMAction* action)
{
    addItem(action->getListItem());
    setItemWidget(action->getListItem(), action);
    if (!action->getId())
    {
        action->setId(calcNewId());
    }

    connect(action, SIGNAL(removeItem(SMAction*)), SLOT(removeSMAction(SMAction*)));
    connect(action, SIGNAL(goNext(SMAction*)), SLOT(goNext(SMAction*)));
    connect(action, SIGNAL(loadNext(SMAction*)), SLOT(loadNext(SMAction*)));
}
SMAction* SMActionList::getActionById(int id)
{
    for (int i = 0; i < count(); i++)
    {
        SMAction* a = getActionItem(i);
        if (a && a->getId() == id)
            return a;
    }
    return 0;
}
int SMActionList::calcNewId(void)
{
    return getMaxId() + 1;
}
int SMActionList::getMaxId(void)
{
    int maxId = 0;
    for (int i = 0; i < count(); i++)
    {
        SMAction* a = getActionItem(i);
        if (a->getId() > maxId)
            maxId = a->getId();
    }
    return maxId;
}
/**
 * @brief SMActionList::removeActions
 * remove all actions from list
 */
void SMActionList::removeActions()
{
    while (count() > 0)
    {
        SMAction* action = getActionItem(0);
        if (action)
            action->stop();
        takeItem(0);
    }
}
void SMActionList::removeSMAction(SMAction *action)
{
    action->stop();
    takeItem(row(action->getListItem()));
}
void SMActionList::go(void)
{
    QList<QListWidgetItem *> selected = selectedItems();
    foreach (QListWidgetItem* item, selected)
    {
        SMAction* a = (SMAction*) itemWidget(item);
        a->go();
    }
}
void SMActionList::goNext(SMAction* a)
{
    QModelIndex mIndex = indexFromItem(a->getListItem());
    SMAction* action = getActionItem(mIndex.row() + 1);
    if (action)
        action->go();
}
void SMActionList::loadNext(SMAction* a)
{
    QModelIndex mIndex = indexFromItem(a->getListItem());
    SMAction* action = getActionItem(mIndex.row() + 1);
    if (action)
        action->load();
}
void SMActionList::stopAll(void)
{
    for (int i = 0; i < count(); i++)
    {
        SMAction* a = getActionItem(i);
        if (a)
            a->stop();
    }
}
void SMActionList::stopSelected(void)
{
    QList<QListWidgetItem *> selected = selectedItems();
    foreach (QListWidgetItem* item, selected)
    {
        SMAction* a = (SMAction*) itemWidget(item);
        a->stop();
    }
}
SMAction* SMActionList::getActionItem(int i)
{
    if (item(i))
        return (SMAction*) itemWidget(item(i));
    return 0;
}
/**
 * @brief SMActionList::keyPressEvent
 * @param keyEvent
 *
 * TODO: rewrite to actionKeySlots and keyboard signals
 */
void SMActionList::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->modifiers() == Qt::NoModifier)
    {
        for (int i = 0; i < count(); i++)
        {
            SMAction* a = getActionItem(i);
            if (a && !_keyTimer->isActive())
                if (a->keyPressed(keyEvent->text()))
                {
                    _keyTimer->start(2000);//disable events for 2 sec
                }
        }
    }
    QListWidget::keyPressEvent(keyEvent);
}
void SMActionList::slotKeyTimeout()
{qInfo() << "timer key";
    _keyTimer->stop();
}
