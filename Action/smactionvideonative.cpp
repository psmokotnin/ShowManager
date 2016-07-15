#include "smactionvideonative.h"

SMActionVideoNative::SMActionVideoNative(QWidget *parent):
    SMAction(parent)
{
    mediaDuration = 0;

    mediaPlayer = new QMediaPlayer(0, QMediaPlayer::VideoSurface);
    videoItem = new QGraphicsVideoItem;
    mediaPlayer->setVideoOutput(videoItem);
    mediaPlayer->setNotifyInterval(250); //interval 250 ms
}
SMActionVideoNative::~SMActionVideoNative()
{
    delete mediaPlayer;
    delete videoItem;
}
void SMActionVideoNative::setView(SMView* v)
{
    if (view() && videoItem && videoItem->scene())
        view()->scene()->removeItem(videoItem);

    SMAction::setView(v);
    videoItem->setSize(view()->size());
    videoItem->setPos(QPointF(-1 * view()->size().width() / 2, -1 * view()->size().height() / 2));
}
void SMActionVideoNative::load(int time)
{
    if (!isLoaded())
    {
        SMAction::load(time);
        mediaPlayer->setMedia(mediaUrl());
        _loaded = true;
    }
}
void SMActionVideoNative::go(void)
{
    if (getStatus() != STATUS_PLAY)
    {
        SMAction::go();
        if (!view() || !view()->scene() || !mediaPlayer->isAvailable())
        {
            setStatus(STATUS_ERROR);
            return;
        }
        if (!mediaPlayer->isVideoAvailable())
        {
            setStatus(STATUS_ERROR);
            connect(mediaPlayer, SIGNAL(videoAvailableChanged(bool)), SLOT(go()));
            return;
        }

        connectPlayer();
        view()->scene()->addItem(videoItem);
        mediaPlayer->play();
    }
}
void SMActionVideoNative::stop(void)
{
    if (getStatus() == STATUS_PLAY)
    {
        SMAction::stop();
        disconnectPlayer();
        if (mediaPlayer->state() == QMediaPlayer::PlayingState)
            mediaPlayer->stop();

        if (view() && videoItem->scene())
            view()->scene()->removeItem(videoItem);
    }
}
void SMActionVideoNative::setMedia(QString name)
{
    SMAction::setMedia(name);
}
void SMActionVideoNative::connectPlayer()
{

    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(mediaPlayer,
            SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
}
void SMActionVideoNative::disconnectPlayer()
{
    mediaPlayer->disconnect(this);
}

void SMActionVideoNative::positionChanged(qint64 position)
{
    qint64 percent = (100 * position) / mediaDuration;
    getProgressBar()->setValue((int)percent);
    if (position >= mediaDuration)
        emit end(this);
}
void SMActionVideoNative::durationChanged(qint64 duration)
{
    mediaDuration = duration;
}
void SMActionVideoNative::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    Q_UNUSED(status);
    qDebug() << "Status: " << status << " " << title();
    //if (status == QMediaPlayer::EndOfMedia)
        //view()->scene()->removeItem(videoItem);
}
void SMActionVideoNative::setOpacity(qreal opacity)
{
    videoItem->setOpacity(opacity);
}
