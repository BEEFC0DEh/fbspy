#ifndef FBVIEW_H
#define FBVIEW_H

#include <QObject>
#include <QPointer>

#include "fbspy.h"

class FbView : public QObject
{
    Q_OBJECT

public:
    explicit FbView(QObject *parent);

    FbSpy *spy() const;
    void setSpy(FbSpy *spy);

    bool running() const;
    bool fbCaptured() const;
    QSize virtualSize() const;
    QString status() const;
    const QByteArray &data() const;

signals:
    void spyChanged(FbSpy *spy);

private:
    QPointer<FbSpy> m_fbSpy;
    QObject *m_parent;
};

#endif // FBVIEW_H
