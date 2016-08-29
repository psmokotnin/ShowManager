#include "smactionvideonative.h"

SMActionVideoNative::SMActionVideoNative(QWidget *parent):
    SMAction(parent)
{
    mediaDuration = 0;
    _repeated     = 0;

    mediaPlayer = new QMediaPlayer(0, QMediaPlayer::VideoSurface);
    videoItem = new QGraphicsVideoItem;
    mediaPlayer->setVideoOutput(videoItem);
    mediaPlayer->setNotifyInterval(250); //interval 250 ms

    setRepeat(0);
    setVolume(1.0);

    getContextMenu()->addSeparator();
    QAction* setRepeatAction = getContextMenu()->addAction("set repeat");
    connect(setRepeatAction, SIGNAL(triggered(bool)), SLOT(slotSetRepeat()));

    QAction* setVolumeAction = getContextMenu()->addAction("set volume");
    connect(setVolumeAction, SIGNAL(triggered(bool)), SLOT(slotSetVolume()));
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
        setLoadStatus(STATUS_LOADED);
    }
}
void SMActionVideoNative::go(void)
{
    if (getStatus() != STATUS_PLAY)
    {
        _repeated = 0;
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
        disconnectPlayer();
        if (mediaPlayer->state() == QMediaPlayer::PlayingState)
            mediaPlayer->stop();

        if (view() && videoItem->scene())
            view()->scene()->removeItem(videoItem);

        SMAction::stop();
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
    {
//        qInfo() << "END " << repeat() << " " << _repeat;
//        if (repeat() > 0 && _repeated < repeat())
//        {
//            _repeated ++;
//            mediaPlayer->setPosition(0);
//            mediaPlayer->play();
//            qInfo() << "repeat";
//        }
//        else
            emit end(this);
    }
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
void SMActionVideoNative::setVolume(qreal v)
{
    if (v > 1.0) v = 1.0;
    if (v < 0.0) v = 0.0;
    _volume = v;
    mediaPlayer->setVolume(volume() * 100);
}
void SMActionVideoNative::slotSetRepeat(void)
{
    bool ok;
    int r = QInputDialog::getInt(this, tr("Repeat Action"), tr("repeat count:"),
                                        repeat(), -1, 1000, 1, &ok);
    if (ok)
        setRepeat(r);
}
void SMActionVideoNative::slotSetVolume(void)
{
    bool ok;
    qreal v = (qreal) QInputDialog::getDouble(this, tr("Set volume"), tr("Valume:"),
                                        volume(), 0.0, 1.0, 1, &ok);
    if (ok)
        setVolume(v);
}
QDomElement SMActionVideoNative::createDomElement(QDomDocument& document)
{
    QDomElement element = SMAction::createDomElement(document);

    //repeat
    QDomElement repeatElement = document.createElement("repeat");
    repeatElement.appendChild(document.createTextNode(QString::number(repeat())));
    element.appendChild(repeatElement);

    //volume
    QDomElement volumeElement = document.createElement("volume");
    volumeElement.appendChild(document.createTextNode(QString::number(volume())));
    element.appendChild(volumeElement);

    return element;
}
void SMActionVideoNative::readSettingsFromNode(QDomNode node)
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
        if (name.toLower() == "repeat")
            setRepeat(element.text().toInt());
        if (name.toLower() == "volume")
            setVolume(element.text().toDouble());

        node = node.nextSibling();
    }
}
