/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QList>
#include "Spielfeld.h"
#include "Position.h"

class Viewer;
class ElementItem;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(Viewer *parent);
    virtual ~Scene();

    Spielfeld getSpielfeld();

public slots:
    void setSpielfeld(Spielfeld feld);
    void verteileElemente();
    void zeigeVorbereitungsSpielfeld(int breite_, int hoehe_, bool endlos_);
    void setzeEndlos(bool);

signals:
    void zelleAngeklickt(ElementItem*);
    void anzahlLebendeElemente(int);
    void zell_status(int, int);

private slots:
    void reaktionAufzelleAngeklickt(ElementItem*);
    void sendeZellstatus(int, int);

private:
    int breite, hoehe;
    Spielfeld spielfeld;
    QMap<Position, ElementItem*> elemente;
    QList<ElementItem*> lebendeElemente;
    Viewer *meinViewer;
    int threads;
    bool endlos;

    void erzeugeElemente();
};

#endif
