#ifndef SMACTIONVIDEOQTAV_H
#define SMACTIONVIDEOQTAV_H
#include <QGraphicsVideoItem>

/**
 * USE http://www.qtav.org/
 */
#include <QtAV>
#include <QtAVWidgets>
#include <QtAV/AVPlayer.h>
#include <QtAVWidgets/GraphicsItemRenderer.h>


#include "Action/smaction.h"

class SMActionVideoQtAV : public SMAction
{
    Q_OBJECT

public:
    SMActionVideoQtAV(QWidget *parent = 0);
    ~SMActionVideoQtAV();
    virtual QString type(void) {return "Video";}

    virtual void setView(SMView* v);
    virtual void setMedia(QString fileName);
    void setOpacity(qreal opacity);

    virtual void readSettingsFromNode(QDomNode node);
    QDomElement createDomElement(QDomDocument& document);

protected:
    int _repeat;
    int repeat(){return _repeat;}
    void setRepeat(int r){_repeat = r;}

    qreal _volume;
    qreal volume(){return _volume;}
    void setVolume(qreal v);

private:
    qint64 mediaDuration;
    bool _goOnLoad;
    bool _goOnStopped;
    bool _emitEndOnStopped;
    bool _stopOnStarted;
    void connectPlayer();
    void disconnectPlayer();

    bool _goDisabled;
    void setDisableGo(bool disable){_goDisabled = disable;}
    bool isGoDisabled() {return _goDisabled;}

    bool _stopDisabled;
    void setDisableStop(bool disable){_stopDisabled = disable;}
    bool isStopDisabled() {return _stopDisabled;}

public slots:
    virtual void go();
    virtual void stop();
    virtual void load(int time = 0);

    void playerStop();
    void playerStarted();

    void mediaStatusChanged(QtAV::MediaStatus status);
    void slotSetRepeat(void);
    void slotSetVolume(void);

private slots:
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);

protected slots:
    virtual void onEnd();

/*QtAV*/
private:
    QtAV::AVPlayer AVmediaPlayer;
    QtAV::GraphicsItemRenderer *AVideoItem;
};

#endif // SMACTIONVIDEOQTAV_H
