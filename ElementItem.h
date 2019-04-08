/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef ELEMENTITEM_H
#define ELEMENTITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QSizeF>

class Scene;

class ElementItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    ElementItem(bool lebt_, const QSizeF& abmessung_, Scene *oparent = 0, QGraphicsItem *gparent = 0);
    virtual ~ElementItem();

    Scene* meinescene() const;
    void setzeAbmessung(QSizeF abmessung_);
    void setLeben(bool zustand);
    bool istLebendig() const;

public slots:

signals:
    void angeklickt(ElementItem*);

private slots:

private:
    Scene *Meinescene;
    bool lebt;
    QSizeF abmessung;

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
};

#endif
