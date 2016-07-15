#ifndef SMVIEW_H
#define SMVIEW_H

#include <QWidget>
#include <QtWidgets>



class SMView : public QGraphicsView
{
    Q_OBJECT

public:
    const static int POSITION_FULL         = 0;
    const static int POSITION_TOP_LEFT     = 1;
    const static int POSITION_TOP_RIGHT    = 2;
    const static int POSITION_BOTTOM_LEFT  = 3;
    const static int POSITION_BOTTOM_RIGHT = 4;

    SMView(QWidget * parent = 0);
    void setScreen(int screen = -1);
    void setScreenPosition(int pos);

protected:
    int screenId;
    void keyPressEvent(QKeyEvent *keyEvent);

signals:
    void keyPressed(QKeyEvent *keyEvent);
};

#endif // SMVIEW_H
