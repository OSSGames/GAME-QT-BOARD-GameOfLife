#include "Zelle.h"
#include "Spielfeld.h"

using namespace std;

// Diese Klasse repraesentiert eine Zelle auf Berechnungsebene.
// Konstruktor
Zelle::Zelle(Spielfeld *parent, const Position& position_) : QObject(parent), lebt(false), position(position_), spielfeld(parent)
{
}


// Destruktor
Zelle::~Zelle()
{
}


// Mittels dieser Methode kann mann nachprüfen ob die Zelle lebt.
bool Zelle::istLebendig()
{
    return lebt;
}


// Mittels dieser Methode kann mann nachprüfen ob die Zelle tot ist.
bool Zelle::istTot()
{
    return !lebt;
}


// Mittels dieser Methode kann mann die Zelle beleben.
void Zelle::beleben()
{
    // Wenn die Zelle tot ist ...
    if (istTot())
    {
        // ... die Zelle beleben.
        lebt = true;
    }
}


// Mittels dieser Methode kann man die Zelle töten.
void Zelle::toeten()
{
    // Wenn die Zelle lebendig ist ...
    if (istLebendig())
    {
        // ... die Zelle töten.
        lebt = false;
    }
}


// Mit dieser Methode kann man den Zustand der Zelle in ein char umsetzen.
char Zelle::toChar()
{
    char erg;

    // Wenn die Zelle lebt ...
    if (lebt)
    {
        // ... den Zustand der Zelle in ein "l" umsetzen.
        erg = 'l';
    }

    // Ansonsten ist die Zelle tot, den Zustand der Zelle in ...
    else
    {
        // ... ein "t" umsetzen.
        erg = 't';
    }

    return erg;
}


// Mittels dieser Methode kann man den gesicherten zustand einer Zelle in den Zustand der Zelle zurückverwandeln.
void Zelle::fromChar(char wert)
{
    switch (wert)
    {
    // Wenn ein "l" übergeben wurde ...
    case 'l':

        // ... lebt die Zelle.
        lebt = true;

        break;

        // Wenn ein "t" übergeben wurde ...
    case 't':

        // ... ist die Zelle tot.
        lebt = false;

        break;

        // Wenn irgendetwas übergeben wurde ...
    default:

        break;
    }
}


// Diese Methoden dienen dem Setzen und Abfragen der Position der Zelle.
const Position& Zelle::getPosition() const
{
    return position;
}


// Diese Methode dient dem setzen der Position dieser Zelle.
void Zelle::setPosition(const Position& position_)
{
    position = position_;
}


// Diese Methode ermittelt die lebenden Nachbarn dieser Zelle. Aufgabe wird an das Spielfeld dieser Zelle delegiert.
QList<Zelle*> Zelle::lebendeNachbarn() const
{
    return spielfeld->lebendeNachbarn(getPosition());
}


// Diese Methode ermittelt die toten Nachbarn dieser Zelle. Aufgabe wird an das Spielfeld dieser Zelle delegiert.
QList<Zelle*> Zelle::toteNachbarn() const
{
    return spielfeld->toteNachbarn(getPosition());
}


// Diese Methode ermittelt die Anzahl der lebenden Nachbarn dieser Zelle. Aufgabe wird an das Spielfeld dieser Zelle delegiert.
int Zelle::anzahlLebendeNachbarn() const
{
    return spielfeld->anzahlLebendeNachbarn(getPosition());
}


// Diese Methode ermittelt die Anzahl der toten Nachbarn dieser Zelle. Aufgabe wird an das Spielfeld dieser Zelle delegiert.
int Zelle::anzahlToteNachbarn() const
{
    return spielfeld->anzahlToteNachbarn(getPosition());
}
