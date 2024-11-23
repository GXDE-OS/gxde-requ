#ifndef REQU_H
#define REQU_H

#include <QWidget>
#include <QEvent>
#include <QTimer>
#include <QProcess>
#include <QCoreApplication>
#define WIDGET_WIDTH 30

class requ : public QWidget
{
    Q_OBJECT
public:
    enum Place {
        TopLeft,
        TopRight,
        LowerLeft,
        LowerRight
    };
    explicit requ(Place whereIsShow, QWidget *parent = nullptr);
    void setShell(QString t);

    void resizeWindow(Place where);

private:
    QTimer *Timer = new QTimer;
    QString shell;
    Place showPlace;
    bool mouseOnHotPlace = false;

    void paintEvent(QPaintEvent* paint);
    /*void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void transMouseEvents(QMouseEvent *event);*/

signals:

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void runShell();
};

#endif // REQU_H
