#ifndef SMACTIONVIDEOVLC_H
#define SMACTIONVIDEOVLC_H

#include <QTimer>
#include <vlc/vlc.h>
#include "Action/smaction.h"

class SMActionVideoVLC : public SMAction
{
    Q_OBJECT

public:
    SMActionVideoVLC(QWidget *parent = 0);
    ~SMActionVideoVLC();
    virtual QString type(void) {return "Video";}

    //virtual void setView(SMView* v);
    virtual void setMedia(QString fileName);
    void setOpacity(qreal opacity);

    virtual void readSettingsFromNode(QDomNode node);
    QDomElement createDomElement(QDomDocument& document);

protected:
    int _repeat;
    int repeat(){return _repeat;}
    void setRepeat(int r);

    qreal _volume;
    qreal volume(){return _volume;}
    void setVolume(qreal v);

private:
    //QGraphicsVideoItem* videoItem;
    //QMediaPlayer* mediaPlayer;
    //qint64 mediaDuration;
    //void connectPlayer();
    //void disconnectPlayer();

public slots:
    virtual void go();
    virtual void stop();
    virtual void load(int time = 0);

    //void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void slotSetRepeat(void);
    void slotSetVolume(void);

private:
    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;

protected:
    // update interface data from timer (position)
    QTimer *poller;
    QFrame* _videoWidget;

protected slots:
    void slotTimeout(void);
};

#endif // SMACTIONVIDEOVLC_H
