/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef GAMEOFLIFERANDOM_H
#define GAMEOFLIFERANDOM_H

#include <QObject>
#include "Spielfeld.h"

class GameOfLifeRandom : public QObject
{
    Q_OBJECT

public:
    GameOfLifeRandom(QObject *parent = 0);
    virtual ~GameOfLifeRandom();
    
    void erzeugeSpielfeld(int breite, int hoehe, int anzahlLebendeZellen, int endlos);

signals:
    void neuesSpielfeld(Spielfeld);

private:
    int threads;
};

#endif // GAMEOFLIFERANDOM_H
