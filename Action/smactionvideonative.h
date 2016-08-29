#ifndef SMActionVideoNative_H
#define SMActionVideoNative_H
#include <QGraphicsVideoItem>

/* *
 * SMActionVideoNative
 * class for playing video files with native Qt mediaplayer
 * */


#include "Action/smaction.h"

class SMActionVideoNative : public SMAction
{
    Q_OBJECT

public:
    SMActionVideoNative(QWidget *parent = 0);
    ~SMActionVideoNative();
    virtual QString type(void) {return "Video";}

    virtual void setView(SMView* v);
    virtual void setMedia(QString fileName);
    void setOpacity(qreal opacity);

    virtual void readSettingsFromNode(QDomNode node);
    QDomElement createDomElement(QDomDocument& document);

    QVideoWidget* videowidget;

protected:
    int _repeat, _repeated;
    int repeat(){return _repeat;}
    void setRepeat(int r){_repeat = r;}

    qreal _volume;
    qreal volume(){return _volume;}
    void setVolume(qreal v);

private:
    QGraphicsVideoItem* videoItem;
    QMediaPlayer* mediaPlayer;
    qint64 mediaDuration;
    void connectPlayer();
    void disconnectPlayer();

public slots:
    virtual void go();
    virtual void stop();
    virtual void load(int time = 0);

    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void slotSetRepeat(void);
    void slotSetVolume(void);

private slots:
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
};

#endif // SMActionVideoNative_H
