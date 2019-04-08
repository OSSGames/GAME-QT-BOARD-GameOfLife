/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef SPIELFELD_H
#define SPIELFELD_H

#include <QObject>
#include <QMap>
#include <QList>
#include "Position.h"
#include "Zelle.h"

class Spielfeld : public QObject
{
    Q_OBJECT
public:
    Spielfeld(int breite_, int hoehe_,int threads_, bool endlos_);
    Spielfeld(const Spielfeld& anderes);
    Spielfeld();

    ~Spielfeld();
    int Breite() const;
    int Hoehe() const;
    bool Endlos() const;
    QList<Zelle*> nachbarn(const Position&) const;
    QList<Zelle*> nachbarn(Zelle*) const;
    Zelle* zelleAnPosition(const Position&) const;
    QList<Zelle*> lebendeNachbarn(const Position&) const;
    QList<Zelle*> toteNachbarn(const Position&) const;
    int anzahlLebendeNachbarn(const Position&) const;
    int anzahlToteNachbarn(const Position&) const;
    QList<Zelle*> lebendeNachbarn(Zelle*) const;
    QList<Zelle*> toteNachbarn(Zelle*) const;
    int anzahlLebendeNachbarn(Zelle*) const;
    int anzahlToteNachbarn(Zelle*) const;
    bool operator==(const Spielfeld&) const;
    bool operator!=(const Spielfeld&) const;
    const Spielfeld& operator=(const Spielfeld&);
    void aktualisiereZellstatus(Spielfeld* vorgeneration, int threadnr);
    bool istFertigBerechnet() const;
    QList<Zelle*> lebendeZellen() const;
    QList<Zelle*> toteZellen() const;
    QList<Zelle*> alleZellen() const;
    int anzahlLebendeZellen() const;
    int anzahlZellen() const;
    int anzahlToteZellen() const;

signals:
    void loescheZellen();

private:
    int breite, hoehe, threads;
    QMap<Position, Zelle*> zellen;
    QList<QList<Zelle*> > zellenAufgeteil;
    QList<bool> fertigBerechnet;
    bool endlos;

    void positionsanpassung_bei_endlos(Position& position) const;
    void aufbauen();
    void aufbauen(const Spielfeld&);
};

#endif
