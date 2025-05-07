// graphviewitems.hpp (continued)
#include <QGraphicsLineItem>
#include <QPen>

class EdgeLine : public QGraphicsLineItem {
public:
    QString fromCity, toCity;

    EdgeLine(const QLineF& line, const QString& from, const QString& to)
        : QGraphicsLineItem(line), fromCity(from), toCity(to) {
        setPen(QPen(Qt::black, 2));
        setAcceptHoverEvents(true);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        qDebug() << "Edge clicked from:" << fromCity << "to:" << toCity;
        QGraphicsLineItem::mousePressEvent(event);
    }
};
