#include "ElementItem.h"
#include "Scene.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

using namespace std;

#define AUSSENRAND_DICKE 0.5

// Diese Klasse repräsentiert die grafische darstellung einer Zelle.
// Konstruktor
ElementItem::ElementItem(bool lebt_, const QSizeF& abmessung_, Scene *oparent, QGraphicsItem *gparent) : QObject(oparent), QGraphicsItem(gparent), Meinescene(oparent), lebt(lebt_), abmessung(abmessung_)
{
}


// Destruktor
ElementItem::~ElementItem()
{
}


Scene* ElementItem::meinescene() const
{
    return Meinescene;
}


// Aussengrenze des Elements.
QRectF ElementItem::boundingRect() const
{
    return QRectF(QPointF(0, 0), abmessung);
}


// Diese Methode bestimmt das aussehen des Elementes, hier wird gezeichnet. Niemals ausserhalb von Bounding Rect malen!
void ElementItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // Die Staerke des Aussenrandes festlegen.
    QPen stift = painter->pen();
    stift.setWidthF(AUSSENRAND_DICKE);

    // Farbe der Zelle bestimmen. Wenn die Zelle lebt Rot, sonst Gruen.
    if (lebt) painter->setBrush(Qt::red);
    else painter->setBrush(Qt::green);

    painter->drawRect(boundingRect().adjusted(AUSSENRAND_DICKE / 2.0, AUSSENRAND_DICKE / 2.0, -(AUSSENRAND_DICKE / 2.0), -(AUSSENRAND_DICKE / 2.0)));
}


// Mit hilfe dieser Methode kann die Groesse angepasst werden.
void ElementItem::setzeAbmessung(QSizeF abmessung_)
{
    // Nur wenn sich die Abmessung aendert.
    if (abmessung != abmessung_)
    {
        // Die geplante Groessenaenderung der Scene melden.
        prepareGeometryChange();

        // Die Abmessung anpassen.
        abmessung = abmessung_;

        // GUI aktualisieren.
        update();
    }
}


// Den Lebens Zustand der Zelle anpassen.
void ElementItem::setLeben(bool zustand)
{
    // Nur, wenn sich der Zustand der Zelle aendern wuerde.
    if (lebt != zustand)
    {
        // Neuen Zustand uebernehmen.
        lebt = zustand;

        // GUI aktualisieren.
        update();
    }
}


// Diese Methode dient der Verarbeitung von Mausklicks.
void ElementItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // Implementierung der Basisklasse aufrufen.
    QGraphicsItem::mousePressEvent(event);

    // Wenn mit der linken Maustaste geklickt wurde.
    if (event->button() == Qt::LeftButton)
    {
        // Lebenszustand der Zelle umkehren.
        setLeben(!lebt);

        emit angeklickt(this);
    }
}


// Mit Hilfe dieser Methode wird erfasst ob die Zelle lebt.
bool ElementItem::istLebendig() const
{
    return lebt;
}
