#pragma once
#ifndef SMACTION_H
#define SMACTION_H

#include <QDebug>
#include <QWidget>
#include <QtWidgets>
#include <QtXml>
#include <QMediaPlayer>
#include <QMovie>

#include "smview.h"


namespace Ui {
class SMAction;
}

class SMApplication;
class SMActionList;

class SMAction : public QWidget
{

    Q_OBJECT

    SMApplication* application;
    SMActionList* actionList;

public:
    const static int GOTYPE_NONE      = 0;
    const static int GOTYPE_NEXTONGO  = 1;
    const static int GOTYPE_NEXTONEND = 2;

    const static int STATUS_STOP          = 0;
    const static int STATUS_LOADING       = 1;
    const static int STATUS_LOADED        = 2;
    const static int STATUS_PLAY          = 3;
    const static int STATUS_PAUSE         = 4;
    const static int STATUS_ERROR         = 5;

public:
    explicit SMAction(QWidget *parent = 0);
    virtual void readSettingsFromNode(QDomNode node);
    virtual QDomElement createDomElement(QDomDocument& document);
    ~SMAction();

    virtual void setTitle(QString title);
    QString title(void);

    void setShortKey(QString key) {_shortKey = key;}
    QString shortKey(void) {return _shortKey;}

    virtual void setMedia(QString fileName);
    QString media() {return fileName;}
    QUrl mediaUrl() {return QUrl::fromLocalFile(media());}

    SMView* view(void);

    QListWidgetItem* getListItem();

    virtual QString type(void) {return "Abstract";};

    bool isLoaded(void) {return _loaded;}
    int getGoType() {return _goType;}
    void setGoType(int type);

    int getStatus() {return _status;}

    int getId(void) {return _id;}
    void setId(int id);

    int getViewId(void) {return _viewId;}
    void setViewId(int id);

protected:
    QProgressBar *getProgressBar(void);
    virtual void contextMenuEvent(QContextMenuEvent *pe)
    {
        contextMenu->exec(pe->globalPos());
    }
    void setStatus(int status);
    virtual void setView(SMView* view);
    void preGo();

    SMActionList* getActionList() {return actionList;}
    QMenu* getContextMenu() {return contextMenu;}

    bool _loaded;
    int _goType;
    int _status;
    int _viewId;
    int _id;
    QString _shortKey;

private:
    Ui::SMAction *ui;
    QMenu* contextMenu;
    bool eventFilter(QObject* pObject, QEvent* pEvent);
    QListWidgetItem* listItem;
    QString fileName;
    SMView* _view;

    void setupList();
    void setApplication(SMApplication* app) {application = app;}

public slots:
    void slotDelete();
    void slotSelectView();

    virtual void start() {}
    virtual void go();
    virtual void load(int time = 0);
    virtual void pause() {}
    virtual void stop();


    void slotTitleDialog();
    void slotSetKey();
    void slotGoTypeNone() {setGoType(GOTYPE_NONE);}
    void slotGoTypeNextOnGo() {setGoType(GOTYPE_NEXTONGO);}
    void slotGoTypeNextOnEnd() {setGoType(GOTYPE_NEXTONEND);}

    bool keyPressed(QString key);

protected slots:
    virtual void onEnd();

signals:
    void removeItem(SMAction* a);
    void goNext(SMAction* a);
    void end(SMAction* a);
    void loadNext(SMAction* a);
};

#endif // SMACTION_H
