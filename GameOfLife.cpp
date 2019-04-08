#include "GameOfLife.h"
#include "RechenEinheit.h"
#include "JobSender.h"
#include <QThread>
#include <QTimer>

using namespace std;

#define VORWAERTS_CACHE 1000
#define RUECKWAERTS_CACHE 1000

// Diese Klasse repraesentiert das Spiel Gane Of Life, hier werden die noetigen Berechnungen ausgefuehrt.
// Konstruktor
GameOfLife::GameOfLife(QObject *parent) : QObject(parent), aktuelles(0), threads(QThread::idealThreadCount()), vorwaertsCache(VORWAERTS_CACHE), rueckwaertsCache(RUECKWAERTS_CACHE), intervall(100), sendenAusstehend(false), abbruch(false), spielAb(false), rechnet(false)
{
    // Die nötigen Recheneinheiten und Threads erstellen.
    for (register int idx = 0; idx < threads; idx++)
    {
        // Eine Recheneinheit und zugehoerige Unterstuetzungsobjekte erstellen.
        QThread *thread = new QThread(this);
        RechenEinheit *rechenEinheit = new RechenEinheit(idx);
        JobSender *sender = new JobSender(this);

        // Die Recheneinheit zum zugehörigen Thread transferieren.
        rechenEinheit->moveToThread(thread);

        // Referenzen auf Objekte in Listen sichern.
        threadObjekte.append(thread);
        recheneinheiten.append(rechenEinheit);
        jobsender.append(sender);

        // Signal - Slot Verbindungen für die Recheneinheit
        // Vergabe von Rechenjobs ermoeglichen
        connect(sender, SIGNAL(berechne(Spielfeld*, Spielfeld*)), rechenEinheit, SLOT(berechne(Spielfeld*, Spielfeld*)));

        // Annahme von Rechenergebnissen ermoeglichen
        connect(rechenEinheit, SIGNAL(fertigBerechnen(Spielfeld*, int)), sender, SLOT(fertigBerechnet(Spielfeld*, int)));
        connect(sender, SIGNAL(resultat(Spielfeld*, int)), this, SLOT(nehmeVerarbeiteteDatenAn(Spielfeld*, int)));

        // Den Thread starten.
        thread->start(QThread::HighestPriority);
    }

    // Den Timer fur den Abspielvorgang erzeugen.
    timer = new QTimer(this);
    timer->setInterval(intervall);
    timer->setSingleShot(true);

    // reaktionen auf den timer ermoeglichen
    connect(timer, SIGNAL(timeout()), this, SLOT(anforderungSpielfeld()));
}


// Destruktor
GameOfLife::~GameOfLife()
{
    // Die Threads beenden.
    for (register int idx = 0; idx < threadObjekte.size(); idx++)
    {
        threadObjekte[idx]->quit();
    }

    // Auf beenden der Threads warten.
    for (register int idx = 0; idx < threadObjekte.size(); idx++)
    {
        threadObjekte[idx]->wait();
    }

    // Löschung der Recheneinheiten veranlassen.
    for (register int idx = 0; idx < recheneinheiten.size(); idx++)
    {
        recheneinheiten[idx]->deleteLater();
    }

    // Die Spielfelder loeschen.
    loescheSpielfelder();
}


// Diese Methode dient dem starten eines neuen Spiels.
void GameOfLife::starteSpiel(Spielfeld* feld)
{
    // Ueber den Start des neuen Spiels veranlassen.
    emit spielGestartet();

    // Das Feld zum aktuellen Feld erklaeren.
    aktuelles = feld;

    // Darueber informieren das Berechnungen gestartet wurden.
    rechnet = true;
    emit berechnungenGestartet();

    // Ein neues Spielfeld erstellen.
    Spielfeld* spielfeld = new Spielfeld(aktuelles->Breite(), aktuelles->Hoehe(), threads, aktuelles->Endlos());

    // Das aktuelle und das neue Spielfeld an die Jobsender uebergeben.
    for (register int idx = 0; idx < jobsender.size(); idx++)
    {
        jobsender[idx]->fuehreBerechnungAus(aktuelles, spielfeld);
    }

    // Das aktuelle Spielfeld senden.
    emit neuesSpielfed(*aktuelles);
}


// Diese Methode dient der loeschung der Spielfelder.
void GameOfLife::loescheSpielfelder()
{
    abbruch = true;

    // Ueber den neuen Cache Fuellstand informieren.
    emit fuellstand_vorwaerts_cache(0);
    emit fuellstand_rueckwaerts_cache(0);

    sendenAusstehend = false;

    // Vergangene Felder loeschen.
    for (register int idx = 0; idx < vergangenheit.size(); idx++)
    {
        vergangenheit[idx]->deleteLater();
    }

    vergangenheit.clear();

    // Zukuenftige Felder loeschen.
    for (register int idx = 0; idx < zukunft.size(); idx++)
    {
        zukunft[idx]->deleteLater();
    }

    zukunft.clear();

    // Wenn aktuelles Element vorhanden, dieses loeschen.
    if (aktuelles != 0)
    {
        aktuelles->deleteLater();

        aktuelles = 0;
    }

    emit spielResettet();
}


int GameOfLife::Threads() const
{
    return threads;
}


// Diese Methode dient der Annahme der verarbeiteten Daten.
void GameOfLife::nehmeVerarbeiteteDatenAn(Spielfeld* ergebnis, int)
{
    if (!abbruch)
    {
        // Wenn das uebergebene Spielfeld bereits fertig berechnet ist.
        if (ergebnis->istFertigBerechnet())
        {
            // Wenn Zukunft leer ist oder das empfangene Spielfeld noch nicht enthaelt.
            if ((!zukunft.isEmpty() && zukunft.last() != ergebnis) || zukunft.isEmpty())
            {
                zukunft.append(ergebnis);

                if (sendenAusstehend)
                {
                    sendenAusstehend = false;

                    anforderungSpielfeld();
                }

                // Wenn es im Spielfeld noch lebende Zellen gibt und der Vorwaerts Cache sein Limit noch nicht erreicht hat.
                if (ergebnis->anzahlLebendeZellen() > 0 && zukunft.size() < vorwaertsCache)
                {
                    // Ueber den Start der Berechnung informieren.
                    rechnet = true;
                    emit berechnungenGestartet();

                    // Ein neues Spielfeld erzeugen.
                    Spielfeld* spielfeld = new Spielfeld(ergebnis->Breite(), ergebnis->Hoehe(), threads, ergebnis->Endlos());

                    // Das letzte Ergebnis SPielfeld und das neue Spielfeld auf die Jobsender verteilen.
                    for (register int idx = 0; idx < jobsender.size(); idx++)
                    {
                        jobsender[idx]->fuehreBerechnungAus(ergebnis, spielfeld);
                    }
                }

                // Wenn der Vorwaerts Cache sein Limit erreicht hat.
                else
                {
                    // Darueber informieren das die Berechnungen vorerst abgeschlossen sind.
                    rechnet = false;
                    emit berechnungenFertiggestellt();
                }

                // Ueber den Cache Fuellstand informieren.
                emit fuellstand_vorwaerts_cache(zukunft.size());
                emit fuellstand_rueckwaerts_cache(vergangenheit.size());
            }
        }
    }

    else
    {
        rechnet = false;
        emit berechnungenFertiggestellt();
    }
}


// Diese Methode gibt die Groesse der Vorwaerts Caches zurueck.
int GameOfLife::VorwaertsCache() const
{
    return vorwaertsCache;
}


// Diese Methode gibt die Groesse der Rueckwaerts Caches zurueck.
int GameOfLife::RueckwaertsCache() const
{
    return rueckwaertsCache;
}


// Ueber diese Methode werden Spielfelder angefordert.
void GameOfLife::anforderungSpielfeld()
{
    // Wenn ein aktuelles Spielfeld vorhanden ist, es jedoch keine lebenden Zellen mehr enthält.
    if (aktuelles != 0 && aktuelles->anzahlLebendeZellen() == 0)
    {
    }

    // Wenn das aktuelle Feld ungleich null ist und Daten im Vorwaerts Cache vorhanden sind.
    else if (aktuelles != 0 && !zukunft.isEmpty())
    {
        sendenAusstehend = false;

        if (aktuelles->anzahlLebendeZellen() > 0)
        {
            // Das aktuelle Element in die Vergangenheit transferieren.
            vergangenheit.append(aktuelles);

            // Das naechste Spielfeld aus der Zukunft zum aktuellen Spielfeld erklaeren.
            aktuelles = zukunft.takeFirst();

            // Das neue Spielfeld senden.
            emit neuesSpielfed(*aktuelles);
        }

        else if (spielAb)
        {
            stop();
        }

        // Ueber den Fuellstand des Caches informieren.
        emit fuellstand_vorwaerts_cache(zukunft.size());
        emit fuellstand_rueckwaerts_cache(vergangenheit.size());

        stutzeVergangenheitCache();
    }

    else
    {
        sendenAusstehend = true;
    }

    if (!rechnet && zukunft.size() < VorwaertsCache() && (aktuelles == 0 || (aktuelles != 0 && aktuelles->anzahlLebendeZellen() > 0)))
    {
        Spielfeld *letztes = 0;

        if (!zukunft.isEmpty())
        {
            letztes = zukunft.last();
        }

        else if (aktuelles != 0)
        {
            letztes = aktuelles;
        }

        if (letztes != 0)
        {
            // Ueber den Start der Berechnung informieren.
            rechnet = true;
            emit berechnungenGestartet();

            // Ein neues Spielfeld erzeugen.
            Spielfeld* spielfeld = new Spielfeld(letztes->Breite(), letztes->Hoehe(), threads, letztes->Endlos());

            // Das letzte Ergebnis SPielfeld und das neue Spielfeld auf die Jobsender verteilen.
            for (register int idx = 0; idx < jobsender.size(); idx++)
            {
                jobsender[idx]->fuehreBerechnungAus(letztes, spielfeld);
            }
        }
    }
}


// Diese Methode dient dem Starten eines neuen Spiels.
void GameOfLife::starteSpiel(Spielfeld anderes)
{
    if (!rechnet)
    {
        abbruch = false;

        Spielfeld *feld = new Spielfeld(anderes);

        starteSpiel(feld);
    }
}


// Mit Hilfe dieser Methode ist es moeglich einen Schritt zurueck anzufordern.
void GameOfLife::anforderungSchrittZurueck()
{
    // Wenn das aktuelle Spielfeld ungleich null und der Vergangenheits Cache nicht leer ist.
    if (aktuelles != 0 && !vergangenheit.isEmpty())
    {
        // Das aktuelle Spielfeld wieder in die Zukunft transferieren.
        zukunft.prepend(aktuelles);

        // Das letzte Element aus dem Vergangenheitscache wieder zum aktuellen Element machen.
        aktuelles = vergangenheit.takeLast();

        // Das aktuelle Spielfeld senden.
        emit neuesSpielfed(*aktuelles);

        // Ueber den Fuellstand der Caches informieren.
        emit fuellstand_vorwaerts_cache(zukunft.size());
        emit fuellstand_rueckwaerts_cache(vergangenheit.size());
    }
}


// Diese Methode reagiert darauf wenn der Aufbau der GUI fertiggestellt ist. Sie wird fuer die Play Funktion benoetigt.
void GameOfLife::reaktionGuiFertig(int dauerAufbauMs)
{
    // Wenn Abspielvorgang laueft.
    if (spielAb)
    {
        // Der Aufbau hat sicher Zeit benoetigt. Diese Zeit soll, sofern dies moeglich ist, keinen Einfluss auf die Abspielgeschwindigkeit haben.
        int aktuellerIntervall = intervall - dauerAufbauMs;

        // Wenn der Aufbau laenger als der geplante Intervall gedauert hat laesst sich daran leider nichts aendern. Den aktuellen Intervall auf 0 setzen.
        if (aktuellerIntervall < 0) aktuellerIntervall = 0;

        // Den berechneten Intervall dem Timer uebergeben und den Timer starten.
        timer->setInterval(aktuellerIntervall);
        timer->start();
    }
}


// Diese Methode dient dem Start des Abspielvorgangs.
void GameOfLife::play()
{
    if (aktuelles != 0 && aktuelles->anzahlLebendeZellen() > 0)
    {
        if (!spielAb)
        {
            spielAb = true;

            emit abspielenGestartet();
        }

        anforderungSpielfeld();
    }
}


// Diese Methode dient der Ermittlung des Play Status.
bool GameOfLife::getPlayStatus() const
{
    return spielAb;
}


// Diese Methode dient dem stoppen des Abspielvorgangs.
void GameOfLife::stop()
{
    // Merken das der Abspielvorgang unterbrochen wurde.
    spielAb = false;

    // Den Timer stoppen.
    timer->stop();

    emit abspielenGestoppt();
}


// Diese Methode dient dem zurueckstutzen des Vergangenheits Caches.
void GameOfLife::stutzeVergangenheitCache()
{
    while (vergangenheit.size() > rueckwaertsCache)
    {
        vergangenheit.takeFirst()->deleteLater();
    }
}


// Diese Methode dient dem anpassen des Abspielintervalls.
void GameOfLife::setzeIntervall(int wert)
{
    intervall = wert;
}
