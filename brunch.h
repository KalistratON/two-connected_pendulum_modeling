#ifndef BRUNCH_H
#define BRUNCH_H

#include <QGraphicsItem>

class Brunch : public QGraphicsItem
{
    qreal top, left, width, height;
    qreal angel = 0;

public:
    Brunch(qreal, qreal, qreal, qreal, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,
               QWidget *) override;
    void advance(int phase) override;

    void setAngle(qreal);
};



#endif // BRUNCH_H
