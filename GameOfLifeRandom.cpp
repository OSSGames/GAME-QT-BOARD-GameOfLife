#include "GameOfLifeRandom.h"
#include "Random.h"
#include <QThread>
#include <QList>
#include <QDebug>

using namespace std;

// Diese Klasse generiert Zufallsgenerierte Spielfelder.
// Konstruktor
GameOfLifeRandom::GameOfLifeRandom(QObject *parent) : QObject(parent), threads(QThread::idealThreadCount())
{
    // Den Zufallsgenerator initialisieren.
    Random::initialisiere();
}


// Destruktor
GameOfLifeRandom::~GameOfLifeRandom()
{
}


// Mit Hilfe dieser Methode wird ein neues Zufallsgeneriertes Spielfeld erzeugt.
void GameOfLifeRandom::erzeugeSpielfeld(int breite, int hoehe, int anzahlLebendeZellen, int endlos)
{
    // Ein neues Spielfeld erzeugen.
    Spielfeld erg(breite, hoehe, threads, endlos);

    // Alle Zellen aus dem Spielfeld abfragen. Momentan sind noch alle Zellen tot.
    QList<Zelle*> toteZellen = erg.alleZellen();

    // Wenn die gewuenschte Anzahl der lebenden Zellen geringer als die Anzahl der toten Zellen ist.
    if (anzahlLebendeZellen < toteZellen.size())
    {
        // Solange, bis die gewuenschte Anzahl an lebenden Zellen generiert wurde.
        for (register int idx = 0; idx < anzahlLebendeZellen; idx++)
        {
            // Das Ziel auslosen. Der Wert muss einer gueltigen Index Position innerhalb der Liste der toten Zellen sein.
            int los = Random::random(0, toteZellen.size() - 1);

            // Eine Zelle aus der Liste der toten Zellen entnehmen und gleich dabei aus der Liste entfernen.
            toteZellen.takeAt(los)->beleben();
        }

        // Das neue Spielfeld senden.
        emit neuesSpielfeld(erg);
    }

    // Wenn das Spielfeld weniger Zellen enthaelt als die Anzahl der zu belebenden Zellen.
    else
    {
        qDebug() << tr("Impossible to create %1 living Cells in a Field with %2 Cells!").arg(anzahlLebendeZellen).arg(toteZellen.size());
    }
}
