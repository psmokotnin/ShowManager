#include "smapplication.h"
#include "mainwindow.h"


SMApplication::SMApplication(QObject *parent) : QObject(parent)
{
    w        = new MainWindow();
    settings = new QSettings("PProcherk", "Show manager");
    w->getListWidget()->setApplication(this);

    connect(w, SIGNAL(save(QString)), SLOT(saveProject(QString)));
    connect(w, SIGNAL(open(QString)), SLOT(openProject(QString)));
    connect(w, SIGNAL(clear()), SLOT(clearProject()));
    connect(w, SIGNAL(destroyed(QObject*)), SLOT(exit()));

    //scenes
    QDesktopWidget* d = QApplication::desktop();
    //QRect g = d->screenGeometry(0);

    for (int i = 0; i < d->screenCount(); i++)
    {
        SMView* view = new SMView();
        view->setScreen(i);
        connect(view, SIGNAL(keyPressed(QKeyEvent *)), SLOT(slotKeyPressed(QKeyEvent*)));
        //view->scene()->addLine(QLineF(-0.5*g.width(),-0.5*g.height(),0.5*g.width(),0.5*g.height()), QPen(Qt::blue, 2));
        //view->setScreenPosition(i+1);
        view->show();

        viewList << view;
    }

    QVariant lastFile = settings->value("lastProjectFile");
    if (!lastFile.isNull())
        openProject(lastFile.toString());
}
void SMApplication::showWindow(void)
{
    w->show();
}
int SMApplication::getViewCount()
{
    return viewList.count();
}
SMView* SMApplication::getView(int i)
{
    if (!viewList.isEmpty())
    {
        if (i < viewList.size())
            return viewList.at(i);
    }
    return 0;
}
SMApplication::~SMApplication()
{
    //save settings
    viewList.clear();
    delete w;
    delete settings;
}
void SMApplication::exit(void)
{
    //qDebug() << "close";
    viewList.clear();
    //qApp->quit();

}
void SMApplication::saveProject(QString str)
{
    if (str.isEmpty()) //Save Cmd+S
    {
        if (projectFile.isEmpty()) //new file
        {
            str = w->saveDialog();
            if (str.isEmpty()) //cancel
                return;
        }
        else
            str = projectFile;
    }
    QFile file(str);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0, "Save file", "Can't save file", QMessageBox::Cancel);
        return;
    }
    setProjectFile(str);

    QDomDocument document("show");

    QDomElement showElement = document.createElement("show");
    document.appendChild(showElement);

    QDomElement title = document.createElement("title");
    title.appendChild(document.createTextNode("My Show"));
    showElement.appendChild(title);

    QDomElement actions = document.createElement("actions");
    for (int i = 0; i < w->getListWidget()->count(); i++)
    {
        SMAction* action =  w->getListWidget()->getActionItem(i);
        actions.appendChild(action->createDomElement(document));
    }
    showElement.appendChild(actions);

    QTextStream output(&file);
    output.setCodec(QTextCodec::codecForName("UTF-8"));
    document.save(output, 4, QDomNode::EncodingFromTextStream );
    file.close();
}
void SMApplication::openProject(QString str)
{
    QFile file(str);
    QDomDocument document("show");
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, "Open file", "Can't read file", QMessageBox::Cancel);
        return;
    }

    QString error = "";
    int errorLine = 0, errorColumn = 0;
    if (!document.setContent(&file, false, &error, &errorLine, &errorColumn))
    {
        file.close();
        qDebug() << error << " at line: " << errorLine << " and column: " << errorColumn;
        QMessageBox::critical(0, "Open file", "Can't read file: " + error, QMessageBox::Cancel);
        return;
    }
    file.close();

    QDomNode node = document.firstChild();//<?xml ... ?> node
    node = node.nextSibling();//show node
    QDomElement element = node.toElement();
    QString name = element.tagName();

    if (name.toLower() != "show")
    {
        QMessageBox::critical(0, "Open file", "It is not show file.", QMessageBox::Cancel);
        return;
    }
    setProjectFile(str);
    clearProject();

    node = node.firstChild();
    while(!node.isNull())
    {
        if(!node.isElement())
        {
            node = node.nextSibling();
            continue;
        }
        element = node.toElement();
        name = element.tagName();

        if (name.toLower() == "actions")
        {
            readActionsNode(node);
        }
        node = node.nextSibling();
    }
}
void SMApplication::readActionsNode(QDomNode node)
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
        if (name.toLower() != "action")
        {
            node = node.nextSibling();
            continue;
        }
        readActionNode(node);
        node = node.nextSibling();
    }
}
void SMApplication::readActionNode(QDomNode node)
{
    SMAction* action;
    QDomElement element = node.toElement();

    QString actionType = element.attribute("type").toLower();
    if (actionType == "empty")
        action = new SMActionEmpty(w->getListWidget());
    else if (actionType == "video")
        action = new SMActionVideo(w->getListWidget());
    else if (actionType == "audio")
        action = new SMActionAudio(w->getListWidget());
    else if (actionType == "play")
        action = new SMActionPlay(w->getListWidget());
    else if (actionType == "stop")
        action = new SMActionStop(w->getListWidget());
    else if (actionType == "opacity")
        action = new SMActionOpacity(w->getListWidget());
    else if (actionType == "wait")
        action = new SMActionWait(w->getListWidget());
    else
        return; //unknow action

    action->readSettingsFromNode(node);

    w->getListWidget()->addAction(action);
}
void SMApplication::clearProject()
{
    w->getListWidget()->removeActions();
}
void SMApplication::setProjectFile(QString name)
{
    projectFile = name;
    settings->setValue("lastProjectFile", projectFile);
    emit projectFileChanged(projectFile);
}
void SMApplication::slotKeyPressed(QKeyEvent *keyEvent)
{
    w->getListWidget()->keyPressEvent(keyEvent);
}
