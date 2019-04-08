/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef RECHENEINHEIT_H
#define RECHENEINHEIT_H

#include <QObject>

class Spielfeld;

class RechenEinheit : public QObject
{
    Q_OBJECT

public:
    RechenEinheit(int nummer_);
    virtual ~RechenEinheit();

public slots:
    void berechne(Spielfeld*, Spielfeld*);

signals:
    void fertigBerechnen(Spielfeld*, int);

private slots:

private:
    int nummer;
};

#endif
