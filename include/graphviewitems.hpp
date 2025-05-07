// graphviewitems.hpp
#ifndef GRAPHVIEWITEMS_HPP
#define GRAPHVIEWITEMS_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QDebug>
// we should use this instead of the addEllipse , adn addline this will make the map interactive
// as you can click any where you like or
class CityNode : public QGraphicsEllipseItem {
public:
    QString cityName;

    CityNode(const QRectF& rect, const QString& name)
        : QGraphicsEllipseItem(rect), cityName(name) {
        setBrush(Qt::cyan);
        setPen(QPen(Qt::black));
        setAcceptHoverEvents(true);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        qDebug() << "City clicked:" << cityName;
        QGraphicsEllipseItem::mousePressEvent(event);
    }
};


class EdgeLine : public QGraphicsLineItem {
public:
    QString fromCity;
    QString toCity;

    EdgeLine(const QLineF& line, const QString& from, const QString& to)
        : QGraphicsLineItem(line), fromCity(from), toCity(to) {
        setPen(QPen(Qt::red, 5));
        setAcceptHoverEvents(true);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        qDebug() << "Edge clicked from" << fromCity << "to" << toCity;
        QGraphicsLineItem::mousePressEvent(event);
    }
};

#endif
