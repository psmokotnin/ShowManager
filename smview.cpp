#include "smview.h"
#include <QGLWidget>

SMView::SMView(QWidget *parent) : QGraphicsView(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    setFocusPolicy(Qt::NoFocus);

    //fo native or qtav, not for VLC
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setScene(new QGraphicsScene(this));

    setBackgroundBrush(Qt::black);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void SMView::setScreen(int screen)
{
    QDesktopWidget* d = QApplication::desktop();
    if (screen > d->screenCount() - 1)
        screen = -1;

    screenId = screen;
    if (screen != d->primaryScreen() && screen != -1)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    QRect g = d->screenGeometry(screen);
    if (!g.isEmpty())
        setGeometry(g);
}
void SMView::setScreenPosition(int pos)
{
    QDesktopWidget* d = QApplication::desktop();
    QRect g = d->screenGeometry(screenId), vg = g;

    if (pos == POSITION_TOP_RIGHT || pos == POSITION_BOTTOM_RIGHT)
        vg.setLeft(g.left() + g.width() / 2);
    if (pos == POSITION_BOTTOM_LEFT || pos == POSITION_BOTTOM_RIGHT)
        vg.setTop(g.top() + g.height() / 2);

    if (pos != POSITION_FULL)
    {
        vg.setWidth(g.width() / 2);
        vg.setHeight(g.height() / 2);
    }

    switch (pos)
    {
        case 0:setBackgroundBrush(Qt::black);break;
        case 1:setBackgroundBrush(Qt::red);break;
        case 2:setBackgroundBrush(Qt::green);break;
        case 3:setBackgroundBrush(Qt::yellow);break;
        case 4:setBackgroundBrush(Qt::gray);break;
    }

    setGeometry(vg);
}

void SMView::keyPressEvent(QKeyEvent *keyEvent)
{
    emit keyPressed(keyEvent);
    QGraphicsView::keyPressEvent(keyEvent);
}
