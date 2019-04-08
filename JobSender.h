/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef JOBSENDER_H
#define JOBSENDER_H

#include <QObject>

class Spielfeld;

class JobSender : public QObject
{
    Q_OBJECT

public:
    JobSender(QObject *parent = 0);
    virtual ~JobSender();

    void fuehreBerechnungAus(Spielfeld*, Spielfeld*);

public slots:
    void fertigBerechnet(Spielfeld*, int);

signals:
    void berechne(Spielfeld*, Spielfeld*);
    void resultat(Spielfeld*, int);

private:

private slots:

};

#endif
