/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QObject>
#include <QList>
#include "Spielfeld.h"

class QThread;
class RechenEinheit;
class JobSender;
class QTimer;

class GameOfLife : public QObject
{
    Q_OBJECT

public:
    GameOfLife(QObject *parent = 0);
    virtual ~GameOfLife();

    int Threads() const;
    int VorwaertsCache() const;
    int RueckwaertsCache() const;
    bool getPlayStatus() const;

public slots:
    void starteSpiel(Spielfeld);
    void nehmeVerarbeiteteDatenAn(Spielfeld*, int);
    void anforderungSpielfeld();
    void anforderungSchrittZurueck();
    void loescheSpielfelder();
    void reaktionGuiFertig(int);
    void play();
    void stop();
    void setzeIntervall(int);

signals:
    void neuesSpielfed(Spielfeld);
    void fuellstand_vorwaerts_cache(int);
    void fuellstand_rueckwaerts_cache(int);
    void spielGestartet();
    void abspielenGestartet();
    void abspielenGestoppt();
    void berechnungenGestartet();
    void berechnungenFertiggestellt();
    void spielResettet();

private slots:

private:
    QList<Spielfeld*> vergangenheit, zukunft;
    Spielfeld* aktuelles;
    int threads, vorwaertsCache, rueckwaertsCache, intervall;
    QList<RechenEinheit*> recheneinheiten;
    QList<QThread*> threadObjekte;
    QList<JobSender*> jobsender;
    bool sendenAusstehend, abbruch, spielAb, rechnet;
    QTimer *timer;

    void starteSpiel(Spielfeld*);
    void stutzeVergangenheitCache();
};

#endif
