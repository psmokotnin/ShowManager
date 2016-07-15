#include "smactionvideo.h"

using namespace QtAV;

SMActionVideo::SMActionVideo(QWidget *parent):
    SMAction(parent)
{
    mediaDuration     = 0;
    _goOnLoad         = false;
    _goDisabled       = false;
    _goOnStopped      = false;
    _emitEndOnStopped = false;
    _stopOnStarted    = false;
    setRepeat(0);
    setVolume(1.0);

    QtAV::Widgets::registerRenderers();
    AVideoItem = new GraphicsItemRenderer(0);
    AVideoItem->resizeRenderer(640, 360);
    AVideoItem->setOutAspectRatioMode(VideoRenderer::VideoAspectRatio);
    AVideoItem->setOpenGL(true);
    AVmediaPlayer.setRenderer(AVideoItem);
    //AVmediaPlayer.setNotifyInterval(100);

    getContextMenu()->addSeparator();
    QAction* setRepeatAction = getContextMenu()->addAction("set repeat");
    connect(setRepeatAction, SIGNAL(triggered(bool)), SLOT(slotSetRepeat()));

    QAction* setVolumeAction = getContextMenu()->addAction("set volume");
    connect(setVolumeAction, SIGNAL(triggered(bool)), SLOT(slotSetVolume()));

    connectPlayer();
}
SMActionVideo::~SMActionVideo()
{
    //if (AVmediaPlayer.isPlaying())
//        AVmediaPlayer.stop();
    delete AVideoItem;
}
void SMActionVideo::setView(SMView* v)
{
    if (view() && AVideoItem && AVideoItem->scene())
        view()->scene()->removeItem(AVideoItem);

    SMAction::setView(v);

    AVideoItem->resizeRenderer(view()->size());
    view()->scene()->addItem(AVideoItem);
}
void SMActionVideo::load(int time)
{
    SMAction::load(time);

    if (!isLoaded())
    {
        if (AVmediaPlayer.file().isEmpty())
            AVmediaPlayer.setFile(media());

        AVmediaPlayer.load();
    }
    AVmediaPlayer.setPosition(0);
}
void SMActionVideo::go(void)
{
    if (!isLoaded())
    {
        _goOnLoad = true;
        load();
        return;
    }
    preGo();
    _goOnLoad = false;
    if (getStatus() == STATUS_ERROR)
        return;

    //wait for stopping process end
    if (isGoDisabled())
    {
        _goOnStopped = true;
        return;
    }
    if (!AVideoItem->scene())
        view()->scene()->addItem(AVideoItem);

    if (!AVmediaPlayer.isPlaying())
    {
        AVmediaPlayer.setRepeat(repeat());
        setDisableStop(true);
        AVmediaPlayer.play();
    }
}
void SMActionVideo::stop(void)
{
    _goOnLoad = false;

    if (isStopDisabled())
    {
        _stopOnStarted = true;
        return;
    }

    if (AVmediaPlayer.isPlaying())
    {
        AVmediaPlayer.setRepeat(0);//disable repeat for stoping
        AVmediaPlayer.stop();
        //now wait for signal and return to else
    }
    else
    {
        SMAction::stop();
        if (view() && AVideoItem->scene())
            view()->scene()->removeItem(AVideoItem);
    }
}
void SMActionVideo::setMedia(QString name)
{
    SMAction::setMedia(name);
}
void SMActionVideo::connectPlayer()
{
    connect(&AVmediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(&AVmediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(&AVmediaPlayer, SIGNAL(stopped()), SLOT(playerStop()));
    connect(&AVmediaPlayer, SIGNAL(started()), SLOT(playerStarted()));
    connect(&AVmediaPlayer,
            SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
            SLOT(mediaStatusChanged(QtAV::MediaStatus)));
}
void SMActionVideo::disconnectPlayer()
{
    AVmediaPlayer.disconnect(this);
}
void SMActionVideo::playerStop()
{
    setDisableGo(false);

    stop();

    if (_emitEndOnStopped)
    {
        _emitEndOnStopped = false;
        emit end(this);
    }

    if (_goOnStopped)
    {
        _goOnStopped = false;
        go();
    }
}
void SMActionVideo::playerStarted()
{
    setDisableStop(false);
    if (_stopOnStarted)
    {
        _stopOnStarted = false;
        stop();
    }
}
void SMActionVideo::positionChanged(qint64 position)
{
    qint64 percent = (100 * position) / mediaDuration;
    getProgressBar()->setValue((int)percent);
    if (position >= mediaDuration)
        emit end(this);
}
void SMActionVideo::durationChanged(qint64 duration)
{
    mediaDuration = duration;
}
void SMActionVideo::mediaStatusChanged(QtAV::MediaStatus status)
{
    switch ((int)status)
    {
    case QtAV::MediaStatus::LoadingMedia:
        //_loaded = false;
        setStatus(STATUS_LOADING);
        break;

    case QtAV::MediaStatus::LoadedMedia:
        _loaded = true;
        setStatus(STATUS_LOADED);
        if (_goOnLoad)
            go();
        break;

    case QtAV::MediaStatus::BufferingMedia:
    case QtAV::MediaStatus::BufferedMedia:
        setStatus(STATUS_PLAY);
        break;

    case QtAV::MediaStatus::NoMedia:
    case QtAV::MediaStatus::InvalidMedia:
        _loaded = false;
        setStatus(STATUS_ERROR);
        qInfo() << "ERROR!!!! " << title();
        break;

    case QtAV::MediaStatus::EndOfMedia:
        if (AVmediaPlayer.duration() == 0)
            break; //is not end of media, it's stop media
        if (getStatus() == STATUS_PLAY)
        {
            setDisableGo(true);
            _emitEndOnStopped = true;
        }
        else
            emit end(this);
        break;
    }

    //qDebug() << "Status AV: " << status << " " << title();
}
void SMActionVideo::setOpacity(qreal opacity)
{
    AVideoItem->setOpacity(opacity);
}
void SMActionVideo::setVolume(qreal v)
{
    if (v > 1.0) v = 1.0;
    if (v < 0.0) v = 0.0;
    _volume = v;
    if (AVmediaPlayer.audio())
        AVmediaPlayer.audio()->setVolume(volume());
}
void SMActionVideo::onEnd()
{
    getProgressBar()->setValue(getProgressBar()->maximum());

    //if (view() && AVideoItem->scene())
    //    view()->scene()->removeItem(AVideoItem);

    SMAction::onEnd();
}
void SMActionVideo::slotSetRepeat(void)
{
    bool ok;
    int r = QInputDialog::getInt(this, tr("Repeat Action"), tr("repeat count:"),
                                        repeat(), -1, 1000, 1, &ok);
    if (ok)
        setRepeat(r);
}
void SMActionVideo::slotSetVolume(void)
{
    bool ok;
    qreal v = (qreal) QInputDialog::getDouble(this, tr("Set volume"), tr("Valume:"),
                                        volume(), 0.0, 1.0, 1, &ok);
    if (ok)
        setVolume(v);
}
QDomElement SMActionVideo::createDomElement(QDomDocument& document)
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
void SMActionVideo::readSettingsFromNode(QDomNode node)
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
