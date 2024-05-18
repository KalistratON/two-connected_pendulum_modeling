#include "brunch.h"
#include <QPainter>
#include <QRandomGenerator>

Brunch::Brunch(qreal _top,
               qreal _left,
               qreal _width,
               qreal _height,
               QGraphicsItem* parent)  :
    QGraphicsItem(parent)
    , top(_top)
    , left(_left)
    , width(_width)
    , height(_height)
{
}

QRectF Brunch::boundingRect() const
{
    return QRectF(top, left, width, height);
}

void Brunch::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    auto rectangle = boundingRect();

    painter->fillRect(rectangle,  QBrush(Qt::darkYellow));
    painter->drawRect(rectangle);
}

void Brunch::advance(int phase)
{
    if(!phase)
        setRotation(angel);
}

void Brunch::setAngle(qreal _ang)
{
    angel = _ang;
}
