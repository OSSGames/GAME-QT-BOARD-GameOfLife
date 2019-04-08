/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef ZELLE_H
#define ZELLE_H

#include <QObject>
#include "Position.h"

class Spielfeld;

class Zelle : public QObject
{
    Q_OBJECT

public:
    Zelle(Spielfeld *parent, const Position& position_);
    virtual ~Zelle();

    bool istLebendig();
    bool istTot();
    void beleben();
    void toeten();
    char toChar();
    void fromChar(char wert);
    const Position& getPosition() const;
    void setPosition(const Position&);
    QList<Zelle*> lebendeNachbarn() const;
    QList<Zelle*> toteNachbarn() const;
    int anzahlLebendeNachbarn() const;
    int anzahlToteNachbarn() const;

private:
    bool lebt;
    Position position;
    Spielfeld *spielfeld;
};

#endif
