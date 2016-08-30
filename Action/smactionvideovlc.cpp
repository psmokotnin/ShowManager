/**
  * Action video on libvlc
  *
  * ADD: end event, volume, opacity
  *
  */
#include "smactionvideovlc.h"
#include "smactionlist.h"

SMActionVideoVLC::SMActionVideoVLC(QWidget *parent):
    SMAction(parent)
{
    poller = new QTimer(0);
    connect(poller, SIGNAL(timeout()), SLOT(slotTimeout()));

    //preparation of the vlc command
    const char * const vlc_args[] = {
              "--verbose=2", //be much more verbose then normal for debugging purpose
    };
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);
    _m = NULL;

    setRepeat(0);
    setVolume(1.0);

    getContextMenu()->addSeparator();
    QAction* setRepeatAction = getContextMenu()->addAction("set repeat");
    connect(setRepeatAction, SIGNAL(triggered(bool)), SLOT(slotSetRepeat()));

    QAction* setVolumeAction = getContextMenu()->addAction("set volume");
    connect(setVolumeAction, SIGNAL(triggered(bool)), SLOT(slotSetVolume()));
}
SMActionVideoVLC::~SMActionVideoVLC()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp);

    /* Free the media_player */
    libvlc_media_player_release (_mp);

    libvlc_release (_vlcinstance);

    poller->deleteLater();
}
void SMActionVideoVLC::setMedia(QString name)
{
    SMAction::setMedia(name);
}
void SMActionVideoVLC::load(int time)
{
    if (!isLoaded())
    {
        SMAction::load(time);qInfo() << mediaVLC();
        _m = libvlc_media_new_path(_vlcinstance, mediaVLC());//"C:\\EmotionsVideo\\1.mp4");//mediaUTF8());qInfo() << _m;
        if (!_m)
        {
            setStatus(STATUS_ERROR);
            setLoadStatus(STATUS_ERROR);
            return;
        }
        libvlc_media_player_set_media (_mp, _m);

        QString options = "input-repeat=";
        options += QString::number(repeat());
        libvlc_media_add_option(_m, options.toLatin1());

        setLoadStatus(STATUS_LOADED);
    }
}
void SMActionVideoVLC::go()
{
    if (getStatus() != STATUS_PLAY && getStatus() != STATUS_ERROR)
    {
        SMAction::go();
        /* Get our media instance to use our window */
#if defined(Q_OS_WIN)
        libvlc_media_player_set_hwnd(_mp, (void*)getView()->winId());
        //libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(getView()->winId()));// _videoWidget->winId()));
#elif defined(Q_OS_MAC)
//        libvlc_media_player_set_drawable(_mp, getView()->winId()));//_videoWidget->winId());
#else //Linux
        /* again note X11 handle on Linux is needed
        winID() returns X11 handle when QX11EmbedContainer us used */
//        int windid = getView()->winId();//_videoWidget->winId();
//        libvlc_media_player_set_xwindow (_mp, windid );

#endif
        /* Play */
        libvlc_media_player_play (_mp);

        poller->start(100); //start timer to trigger every 100 ms the updateInterface slot
    }
}
void SMActionVideoVLC::stop()
{
    libvlc_media_player_stop(_mp);
    SMAction::stop();
}

void SMActionVideoVLC::slotTimeout(void)
{
    if (getStatus() != STATUS_PLAY)
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;

    float pos=libvlc_media_player_get_position (_mp);

    qint64 percent = 100 * pos;
    getProgressBar()->setValue((int)percent);
    if (pos >= 1.0)
    {

    }
}

void SMActionVideoVLC::slotSetRepeat(void)
{
    bool ok;
    int r = QInputDialog::getInt(this, tr("Repeat Action"), tr("repeat count:"),
                                        repeat(), -1, 1000, 1, &ok);
    if (ok)
        setRepeat(r);
}
void SMActionVideoVLC::slotSetVolume(void)
{
    bool ok;
    qreal v = (qreal) QInputDialog::getDouble(this, tr("Set volume"), tr("Valume:"),
                                        volume(), 0.0, 1.0, 1, &ok);
    if (ok)
        setVolume(v);
}
QDomElement SMActionVideoVLC::createDomElement(QDomDocument& document)
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
void SMActionVideoVLC::readSettingsFromNode(QDomNode node)
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
void SMActionVideoVLC::setRepeat(int r)
{
    _repeat = r;
    if (_m)
    {
        QString options = "input-repeat=";
        options += QString::number(repeat());
        libvlc_media_add_option(_m, options.toLatin1());
    }
}
void SMActionVideoVLC::setOpacity(qreal opacity)
{
    qWarning() << opacity << "TODO";
}
void SMActionVideoVLC::setVolume(qreal v)
{
    if (v > 1.0) v = 1.0;
    if (v < 0.0) v = 0.0;
    _volume = v;
    libvlc_audio_set_volume(_mp, _volume*100);
}
