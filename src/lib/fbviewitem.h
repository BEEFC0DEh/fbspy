#ifndef FBVIEWITEM_H
#define FBVIEWITEM_H

#include <QQuickPaintedItem>
#include <QImage>

#include "fbview.h"

class FbSpy;

class FbViewItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(FbSpy *spy READ spy WRITE setSpy NOTIFY spyChanged)

public:
    explicit FbViewItem(QQuickItem *parent = nullptr);

    // QQuickPaintedItem interface
    virtual void paint(QPainter *painter) override;

    FbSpy *spy() const;

public slots:
    void setSpy(FbSpy *spy);

signals:
    void spyChanged(FbSpy *spy);

private slots:
    void updateFbData();
    void updatePartialFbData(uint id, int position, int size);
    void updateVirtualSize(const QSize&virtualSize);

private:
    void copyFbData(int start, int size);

    QImage m_fbImage;
    uint   m_currentFbDataId;
    FbView *m_fbView;
};

#endif // FBVIEWITEM_H
