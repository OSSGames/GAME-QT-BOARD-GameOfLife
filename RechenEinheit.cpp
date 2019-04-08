#include "RechenEinheit.h"
#include "Spielfeld.h"

using namespace std;

// Diese Klasse repraesentiert eine Recheneinheit.
// Konstruktor
RechenEinheit::RechenEinheit(int nummer_) : QObject(0), nummer(nummer_)
{
}


// Destruktor
RechenEinheit::~RechenEinheit()
{
}


// Diese Methode dient der durchfuehrung einer berechnung.
void RechenEinheit::berechne(Spielfeld* vorgeneration, Spielfeld* aktuelleGeneration)
{
    // Berechnung durchfuehren.
    aktuelleGeneration->aktualisiereZellstatus(vorgeneration, nummer);

    // Ergebnis senden.
    emit fertigBerechnen(aktuelleGeneration, nummer);
}
