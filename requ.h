#ifndef REQU_H
#define REQU_H

#include <QWidget>
#include <QEvent>
#include <QTimer>
#include <QProcess>
#include <QCoreApplication>

#define WIDGET_WIDTH 30
#define WIDGET_SEARCH_WIDTH 2

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
    void setTransportFlat();

private:
    QTimer *Timer = new QTimer;
    QString shell;
    Place showPlace;
    bool mouseOnHotPlace = false;
    bool m_supportTransport = true;

    void paintEvent(QPaintEvent* paint);

signals:

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void runShell();
};

#endif // REQU_H
