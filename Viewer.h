/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef VIEWER_H
#define VIEWER_H

#include <QGraphicsView>
#include "Spielfeld.h"

class Scene;

class Viewer : public QGraphicsView
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = 0);
    
    Spielfeld getSpielfeld();

signals:
    void spielfeldAufgebaut(int);
    void zelleAngeklickt(Zelle*);
    void anzahlLebendeElemente(int);
    void zell_status(int, int);

public slots:
    void zeigeVorbereitungsSpielfeld(int breite_, int hoehe_, bool endlos_);
    void setSpielfeld(Spielfeld);
    void setzeEndlos(bool);

private:
    Scene *scene;

    virtual void resizeEvent(QResizeEvent*);
};

#endif // VIEWER_H
