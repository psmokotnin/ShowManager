#include "Action/smaction.h"
#include "ui_smaction.h"
#include "smapplication.h"
#include "mainwindow.h"
#include "smactionlist.h"

SMAction::SMAction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SMAction)
{
    ui->setupUi(this);

    actionList = (SMActionList*) parent;
    setApplication(actionList->getApplication());

    setupList();

    _loadStatus = STATUS_NOTLOADED;
    setId(0);
    setView(0);
    setViewId(0);
    setGoType(GOTYPE_NONE);
    setStatus(STATUS_STOP);

    contextMenu = new QMenu(this);
    QAction* editTitleAction = contextMenu->addAction("edit title");
    QAction* selectView = contextMenu->addAction("set view");
    QAction* shortKey = contextMenu->addAction("set key");
    contextMenu->addSeparator();
    QAction* deleteAction = contextMenu->addAction("delete");
    contextMenu->addSeparator();
    QAction* goType0Action = contextMenu->addAction("Go type: none");
    QAction* goType1Action = contextMenu->addAction("Go type: next on go");
    QAction* goType2Action = contextMenu->addAction("Go type: next on end");


    connect(editTitleAction, SIGNAL(triggered(bool)), SLOT(slotTitleDialog()));
    connect(selectView, SIGNAL(triggered(bool)), SLOT(slotSelectView()));
    connect(shortKey, SIGNAL(triggered(bool)), SLOT(slotSetKey()));
    connect(deleteAction, SIGNAL(triggered(bool)), SLOT(slotDelete()));
    connect(goType0Action, SIGNAL(triggered(bool)), SLOT(slotGoTypeNone()));
    connect(goType1Action, SIGNAL(triggered(bool)), SLOT(slotGoTypeNextOnGo()));
    connect(goType2Action, SIGNAL(triggered(bool)), SLOT(slotGoTypeNextOnEnd()));
    connect(this, SIGNAL(end(SMAction*)), SLOT(onEnd()));

    setTitle("");
}
void SMAction::setupList()
{
    listItem = new QListWidgetItem();
    listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);

    QSize s = sizeHint(); //QSize(-1, -1)
    s.setHeight(minimumHeight());
    listItem->setSizeHint(s);
}
void SMAction::setTitle(QString title)
{
    ui->title->setText(title);
}
void SMAction::setShortKey(QString key)
{
    _shortKey = key;
    emit keySetted(this);
}
QString SMAction::title(void)
{
    return ui->title->text();
}
void SMAction::setMedia(QString name)
{
    _loadStatus = STATUS_NOTLOADED;
    this->fileName = name;
    if (ui->title->text().length() == 0)
    {
        QFileInfo qfi(name);
        setTitle(qfi.fileName());
    }
    stop();
}
void SMAction::setView(SMView* view)
{
    _view = view;
}
SMView* SMAction::view(void)
{
    return _view;
}
void SMAction::setGoType(int type)
{
    _goType = type;
    ui->gotype->setText(QString::number(getGoType()));
}
QListWidgetItem* SMAction::getListItem()
{
    return listItem;
}
QProgressBar* SMAction::getProgressBar(void)
{
    return ui->progressBar;
}
void SMAction::setStatus(int status)
{qInfo() << "setStatus " << status << " " << getId();
    //if (_status == status)
    //    return ;

    _status = status;
    //ui->status->setText(QString::number(getStatus()));
    QPixmap p; // load pixmap
    int w = ui->status->width();
    int h = ui->status->height();
    switch (getStatus())
    {
    case STATUS_PLAY:
        p.load("://Images/playAction.png");
        ui->status->setPixmap(p.scaled(w / 2, h / 2, Qt::KeepAspectRatio));
        break;
    case STATUS_STOP:
        qInfo() << "stopped " << getId() << " " << title();
        emit stopped(this);
        ui->status->setText(QString::number(getStatus()));
        //p.load("://Images/stopAction.png");
        //ui->status->setPixmap(p.scaled(w / 2, h / 2, Qt::KeepAspectRatio));
        break;
    default:
        ui->status->setText(QString::number(getStatus()));
    }
}
SMAction::~SMAction()
{
    delete contextMenu;
    delete listItem;
    delete ui;
}
bool SMAction::eventFilter(QObject* pObject, QEvent* pEvent)
{
    qDebug() << "event: " << pEvent->type();
    if (pEvent->type() == QEvent::MouseButtonPress)
    {
        qDebug() << "click";
        return true;
    }
    return QObject::eventFilter(pObject, pEvent);
}

void SMAction::slotDelete()
{
    emit removeItem(this);
}
void SMAction::slotTitleDialog()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Edit action"),
                                         tr("Title:"), QLineEdit::Normal,
                                         title(), &ok);
    if (!ok || text.isEmpty())
        return;

    setTitle(text);
}
void SMAction::slotSelectView(void)
{
    bool ok;
    QStringList items;
    for (int i =0; i < application->getViewCount() ; i++)
    {
        items << QString::number(i);
    }
    QString text = QInputDialog::getItem(this, tr("Select view"),
                                         tr("View window:"), items, getViewId(), false, &ok);
    if (ok)
    {
        setViewId(text.toInt());
    }
}
void SMAction::setViewId(int id)
{
    SMView* view = application->getView(id);
    if (view)
    {
        _viewId = id;
        setView(view);
        ui->viewid->setText(QString::number(id));
    }
}
void SMAction::setId(int id)
{
    _id = id;
    ui->id->setText(QString::number(id));
}
/**
 * @brief SMAction::readSettingsFromNode
 * read all settings from xml node and configure action
 * @param node - node from opened xml-file
 */
void SMAction::readSettingsFromNode(QDomNode node)
{
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
        if (name.toLower() == "id")
            setId(element.text().toInt());
        if (name.toLower() == "shortkey")
            setShortKey(element.text());
        if (name.toLower() == "title")
            setTitle(element.text());
        if (name.toLower() == "file")
            setMedia(element.text());
        if (name.toLower() == "gotype")
            setGoType(element.text().toInt());
        if (name.toLower() == "view")
        {
            QDomNodeList list = element.elementsByTagName("id");
            QDomNode viewIdNode = list.at(0);
            if (!viewIdNode.isNull() && viewIdNode.isElement())
            {
                setViewId(viewIdNode.toElement().text().toInt());
            }
        }

        node = node.nextSibling();
    }
}
/**
 * @brief SMAction::createDomElement
 * Create dom element that containing all data about action for save
 * @param document
 * @return
 */
QDomElement SMAction::createDomElement(QDomDocument& document)
{
    QDomElement element = document.createElement("action");
    element.setAttribute("type", type());

    //id
    QDomElement idElement = document.createElement("id");
    idElement.appendChild(document.createTextNode(QString::number(getId())));
    element.appendChild(idElement);

    //shortKey
    QDomElement shortKeyElement = document.createElement("shortkey");
    shortKeyElement.appendChild(document.createTextNode(shortKey()));
    element.appendChild(shortKeyElement);

    //title
    QDomElement titleElement = document.createElement("title");
    titleElement.appendChild(document.createTextNode(title()));
    element.appendChild(titleElement);

    //media
    if (!fileName.isEmpty())
    {
        //file
        QDomElement fileElement = document.createElement("file");
        fileElement.appendChild(document.createTextNode(fileName));
        element.appendChild(fileElement);
    }

    //gotype
    QDomElement gotypeElement = document.createElement("gotype");
    gotypeElement.appendChild(document.createTextNode(QString::number(getGoType())));
    element.appendChild(gotypeElement);

    //view
    QDomElement viewElement = document.createElement("view");
        QDomElement viewIdElement = document.createElement("id");
        viewIdElement.appendChild(document.createTextNode(QString::number(getViewId())));
    viewElement.appendChild(viewIdElement);
    element.appendChild(viewElement);

    return element;
}

/**
  * Actions events
  */
/**
 * signal nextongo
 * pre wait timer
 */
void SMAction::preGo()
{
    if (getGoType() == GOTYPE_NEXTONGO)
        emit goNext(this);
}
void SMAction::go(void)
{
    setStatus(STATUS_PLAY);
    qInfo() << "GO " << getId() << " " << title();
    if (!isLoaded())
        load();

    preGo();
}
void SMAction::load(int time)
{
    Q_UNUSED(time);
    if (getGoType() == GOTYPE_NEXTONGO ||
            getGoType() == GOTYPE_NEXTONEND)
        emit loadNext(this);
}
void SMAction::stop(void)
{
    qInfo() << "STOP " << getId() << " " << title();
    setStatus(STATUS_STOP);
}
void SMAction::onEnd()
{qInfo() << "onEnd " << getId() << " " << title();
    //qInfo() << "onEnd " << title();
    /*
     * don't go next if already stopped
     */
    if (getStatus() != STATUS_STOP)
    {
        setStatus(STATUS_STOP);
        if (getGoType() == GOTYPE_NEXTONEND)
            emit goNext(this);
    }
}
bool SMAction::keyPressed(QString key)
{
    if (!key.isEmpty() && key == shortKey())
    {
        go();
        return true;
    }
    return false;
}
void SMAction::slotSetKey()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Short key"), tr("key"), QLineEdit::Normal, shortKey(), &ok);
    if (ok)
        setShortKey(text);
}
