#include "smactionwait.h"
#include "smactionlist.h"

SMActionWait::SMActionWait(QWidget *parent):
    SMAction(parent)
{
    _currentTime = 0;
    _timeStep    = 100;
    _waitTime    = 0;
    _timer       = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), SLOT(slotTimeout()));

    getContextMenu()->addSeparator();
    QAction* setTimeAction = getContextMenu()->addAction("set time");
    connect(setTimeAction, SIGNAL(triggered(bool)), SLOT(slotSetTime()));
}
SMActionWait::~SMActionWait(){
    delete _timer;
}

void SMActionWait::setWaitTime(int time)
{
    _waitTime = time;
    setTitle("Wait action " + QString::number(time/1000) + " sec");
}
void SMActionWait::go()
{
    if (getStatus() != STATUS_PLAY)
    {
        SMAction::go();
        _currentTime = 0;
        if (waitTime() > 0)
            _timer->start(_timeStep);
        else
            emit end(this);
    }
}
void SMActionWait::stop()
{
    SMAction::stop();
    _timer->stop();
}

void SMActionWait::slotSetTime(void)
{
    bool ok;
    int time = QInputDialog::getInt(this, tr("Action wait"), tr("time (ms):"),
                                        waitTime(), 0, 3600000, 1, &ok);
    if (ok)
        setWaitTime(time);
}
void SMActionWait::slotTimeout()
{
    _currentTime += _timeStep;
    getProgressBar()->setValue(100 * _currentTime / _waitTime);
    if (_currentTime > _waitTime)
    {
        _timer->stop();
        emit end(this);
    }
}

QDomElement SMActionWait::createDomElement(QDomDocument& document)
{
    QDomElement element = SMAction::createDomElement(document);

    //wait
    QDomElement waitElement = document.createElement("wait");
    waitElement.appendChild(document.createTextNode(QString::number(waitTime())));
    element.appendChild(waitElement);

    return element;
}
void SMActionWait::readSettingsFromNode(QDomNode node)
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
        if (name.toLower() == "wait")
            setWaitTime(element.text().toInt());

        node = node.nextSibling();
    }
}
