#include "Spielfeld.h"
#include <QThread>

using namespace std;

// Diese Klasse repraesentiert eine Generation von Zellen.
// Konstruktoren.
Spielfeld::Spielfeld(int breite_, int hoehe_, int threads_, bool endlos_) : QObject(0), breite(breite_), hoehe(hoehe_), threads(threads_), endlos(endlos_)
{
    aufbauen();
}


Spielfeld::Spielfeld(const Spielfeld& anderes) : QObject(0), breite(anderes.breite), hoehe(anderes.hoehe), threads(anderes.threads), endlos(anderes.endlos)
{	
    aufbauen();

    aufbauen(anderes);
}


Spielfeld::Spielfeld() : QObject(0), breite(0), hoehe(0), threads(QThread::idealThreadCount()), endlos(true)
{
}


// Destruktor
Spielfeld::~Spielfeld()
{
}


// Diese Methode baut das Spielfeld auf.
void Spielfeld::aufbauen()
{
    // Die Zellen aufbauen
    for (register int idx_y = 0; idx_y < hoehe; idx_y++)
    {
        for (register int idx_x = 0; idx_x < breite; idx_x++)
        {
            Position position = Position(idx_x, idx_y);

            Zelle *neueZelle = new Zelle(this, position);

            zellen.insert(position, neueZelle);

            connect(this, SIGNAL(loescheZellen()), neueZelle, SLOT(deleteLater()));
        }
    }

    // Listen für Threads aufbauen
    for (register int idx = 0; idx < threads; idx++)
    {
        zellenAufgeteil.append(QList<Zelle*>());
    }

    // Die Zellen auf die Threads verteilen
    QList<Zelle*> alleZellen(zellen.values());

    for (register int idx = 0; idx < alleZellen.size(); idx++)
    {
        zellenAufgeteil[idx % threads].append(alleZellen[idx]);
    }

    // Berechnungs Status Liste aufbauen
    for (register int idx = 0; idx < threads; idx++)
    {
        fertigBerechnet.append(false);
    }
}


// Diese Methode dient der Uebernahme des Zustandes der Zellen eines anderen Spielfeldes.
void Spielfeld::aufbauen(const Spielfeld& anderes)
{
    for (register int idx_y = 0; idx_y < hoehe; idx_y++)
    {
        for (register int idx_x = 0; idx_x < breite; idx_x++)
        {
            Position position = Position(idx_x, idx_y);

            if (anderes.zellen.value(position)->istLebendig()) zellen.value(position)->beleben();
            else zellen.value(position)->toeten();
        }
    }

    // Berechnungs Status Liste aufbauen
    for (register int idx = 0; idx < threads; idx++)
    {
        fertigBerechnet[idx] = anderes.fertigBerechnet[idx];
    }
}


// Diese Methode gibt die Breite des Spielfeldes zurueck.
int Spielfeld::Breite() const
{
    return breite;
}


// Diese Methode gibt die Hoehe des Spielfeldes zurueck.
int Spielfeld::Hoehe() const
{
    return hoehe;
}


// Diese Methode ermittelt die Nachbarn der Zelle mit der uebergebenen Position.
QList<Zelle*> Spielfeld::nachbarn(const Position& position) const
{	
    QList<Zelle*> erg;
    Position tmp_position(position);

    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() - 1);
    tmp_position.setX(position.x() - 1);

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() - 1);
    tmp_position.setX(position.x());

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() - 1);
    tmp_position.setX(position.x() + 1);

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y());
    tmp_position.setX(position.x() - 1);

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() );
    tmp_position.setX(position.x() + 1);

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() + 1);
    tmp_position.setX(position.x() - 1);

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() + 1);
    tmp_position.setX(position.x());

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }


    // Zu einem Nachbarn wechseln.
    tmp_position.setY(position.y() + 1);
    tmp_position.setX(position.x() + 1);

    // Positionsanpassung bei endlosem Spielfeld durchfuehren.
    if (endlos) positionsanpassung_bei_endlos(tmp_position);

    // Wenn die ermittelte Zelle ungleich null ist ...
    if (zelleAnPosition(tmp_position) != 0)
    {
        // ... die Zelle in die Liste der NAchbarn uebernehmen.
        erg.append(zelleAnPosition(tmp_position));
    }

    return erg;
}


// Diese Methode dient der Ermittlung der Zelle an einer angegebenen Position. Wenn die Zelle nicht existiert wird null zurueck gegeben.
Zelle* Spielfeld::zelleAnPosition(const Position& position) const
{
    // Ergebnis mit null vorinitialisieren.
    Zelle* erg = 0;

    // Nur wenn die Zelle vorhanden ist.
    if (zellen.contains(position))
    {
        // ... die Zelle als Ergebnis uebernehmen.
        erg = zellen.value(position);
    }

    return erg;
}


// Diese Methode dient der Anpassung der Position bei einem endlosen Spielfeld "ueber den Rand hinaus".
void Spielfeld::positionsanpassung_bei_endlos(Position& position) const
{
    // Nur wenn das Spielfeld endlos ist.
    if (endlos)
    {
        // Linker Rand zum rechten Rand
        if (position.x() < 0)
        {
            position.setX(breite - 1);
        }

        // Rechter Rand zum linken Rand.
        else if (position.x() >= breite)
        {
            position.setX(0);
        }

        // Unterer Rand zum oberen Rand.
        if (position.y() < 0)
        {
            position.setY(hoehe - 1);
        }

        // Oberer Rabd zum unteren Rand.
        else if (position.y() >= hoehe)
        {
            position.setY(0);
        }
    }
}


// Diese Methode ermittelt die Nachbarn einer angegebenen Zelle. Wird an die Methode weiter delegiert die dies mit Positionen erledigt.
QList<Zelle*> Spielfeld::nachbarn(Zelle* zelle) const
{
    return nachbarn(zelle->getPosition());
}


// Diese Methode ermittelt die lebenden Nachbarn der Zelle an der angegebenen Position.
QList<Zelle*> Spielfeld::lebendeNachbarn(const Position& position) const
{
    QList<Zelle*> erg;
    QList<Zelle*> nachbarn(this->nachbarn(position));

    for (register int idx = 0; idx < nachbarn.size(); idx++)
    {
        if (nachbarn.at(idx)->istLebendig()) erg.append(nachbarn.at(idx));
    }

    return erg;
}


// Diese Methode ermittelt die toten Nachbarn der Zelle an der angegebenen Position.
QList<Zelle*> Spielfeld::toteNachbarn(const Position& position) const
{
    QList<Zelle*> erg;
    QList<Zelle*> nachbarn(this->nachbarn(position));

    for (register int idx = 0; idx < nachbarn.size(); idx++)
    {
        if (nachbarn.at(idx)->istTot()) erg.append(nachbarn.at(idx));
    }
    return erg;
}


// Diese Methode ermittelt die Anzahl der lebenden Nachbarn der Zelle an der Angegebenen Position.
int Spielfeld::anzahlLebendeNachbarn(const Position& position) const
{
    return lebendeNachbarn(position).size();
}


// Diese Methode ermittelt die Anzahl der toten Nachbarn der Zelle an der Angegebenen Position.
int Spielfeld::anzahlToteNachbarn(const Position& position) const
{
    return toteNachbarn(position).size();
}


// Diese Methode ermittelt die lebenden Nachbarn der angegebenen Zelle.
QList<Zelle*> Spielfeld::lebendeNachbarn(Zelle* zelle) const
{
    return lebendeNachbarn(zelle->getPosition());
}


// Diese Methode ermittelt die  toten Nachbarn der angegebenen Zelle.
QList<Zelle*> Spielfeld::toteNachbarn(Zelle* zelle) const
{
    return toteNachbarn(zelle->getPosition());
}


// Diese Methode ermittelt die Anzahl der lebenden Nachbarn der angegebenen Zelle.
int Spielfeld::anzahlLebendeNachbarn(Zelle* zelle) const
{
    return anzahlLebendeNachbarn(zelle->getPosition());
}


// Diese Methode ermittelt die Anzahl der toten Nachbarn der angegebenen Zelle.
int Spielfeld::anzahlToteNachbarn(Zelle* zelle) const
{
    return anzahlToteNachbarn(zelle->getPosition());
}


// Vergleichoperator zum Vergleichen von Spielfeldern.
bool Spielfeld::operator==(const Spielfeld& anderes) const
{
    bool erg = false;

    QList<Zelle*> alleZellen1(zellen.values());
    QList<Zelle*> alleZellen2(anderes.zellen.values());


    if (alleZellen1.size() == alleZellen2.size() && endlos == anderes.endlos && Breite() == anderes.Breite() && Hoehe() == anderes.Hoehe() && anzahlLebendeZellen() == anderes.anzahlLebendeZellen())
    {
        erg = true;

        for (register int idx = 0; erg && idx < alleZellen1.size(); idx++)
        {
            erg = alleZellen1.at(idx)->istLebendig() == alleZellen2.at(idx)->istLebendig();
        }
    }

    return erg;
}


// Vergleichoperator zum Vergleichen von Spielfeldern.
bool Spielfeld::operator!=(const Spielfeld& anderes) const
{
    return !(*this == anderes);
}


// Zuweisungsoperator fuer Spielfelder.
const Spielfeld& Spielfeld::operator=(const Spielfeld& anderes)
{	
    // Selbstzuweisungen verhindern.
    if (&anderes != this)
    {
        // Anzahl der Threads uebernehmen.
        threads = anderes.threads;

        // Nur wenn die momentan im Spielfeld vorhandenen Zeööen nicht wiederverwendet werden koennen.
        if (anderes.Breite() != breite || anderes.Hoehe() != hoehe || anderes.Endlos() != endlos)
        {
            // Breite, Hoehe und Endlosigkeit uebernehmen.
            breite = anderes.Breite();
            hoehe = anderes.Hoehe();
            endlos = anderes.endlos;

            // Bisher vorhandene Zellen loeschen.
            emit loescheZellen();

            // Listen leeren.
            zellen.clear();
            zellenAufgeteil.clear();

            // Zellen neu aufbauen.
            aufbauen();
        }

        // Zustaende der Zellen des anderen Spielfeldes uebernehmen.
        aufbauen(anderes);
    }

    return *this;
}


// Diese Methode berechnen die naechste Generation. Nur den Anteil dieses Threads berechnen.
void Spielfeld::aktualisiereZellstatus(Spielfeld* vorgeneration, int threadnr)
{
    for (register int idx = 0; idx < vorgeneration->zellenAufgeteil[threadnr].size(); idx++)
    {
        int lebendeNachbarn = vorgeneration->zellenAufgeteil[threadnr][idx]->anzahlLebendeNachbarn();
        bool zelleInVorgenerationLebt = vorgeneration->zellenAufgeteil[threadnr][idx]->istLebendig();

        // Tote Zellen mit 3 lebenden Nachbarn werden in der folgegeneration belebt.
        if (!zelleInVorgenerationLebt && lebendeNachbarn == 3)
        {
            zelleAnPosition(vorgeneration->zellenAufgeteil[threadnr][idx]->getPosition())->beleben();
        }

        // Lebende Zellen mit weniger als 2 lebenden Nachbarn sterben in der folgenden Generation.
        else if (zelleInVorgenerationLebt && lebendeNachbarn < 2)
        {
            zelleAnPosition(vorgeneration->zellenAufgeteil[threadnr][idx]->getPosition())->toeten();
        }

        // Lebende Zellen mit 2 oder 3 Nachbarn bleiben in der folgenden Generation am leben.
        else if (zelleInVorgenerationLebt && (lebendeNachbarn == 2 || lebendeNachbarn == 3))
        {
            zelleAnPosition(vorgeneration->zellenAufgeteil[threadnr][idx]->getPosition())->beleben();
        }

        // Lebende Zellen mit mehr als 3 lebenden Nachbarnsterben in der folgenden Generation.
        else if (zelleInVorgenerationLebt && lebendeNachbarn > 3)
        {
            zelleAnPosition(vorgeneration->zellenAufgeteil[threadnr][idx]->getPosition())->toeten();
        }
    }

    // speichern, das fertig berechnet wurde.
    fertigBerechnet[threadnr] = true;
}


// Diese Methode ermittelt ob das Spielfeld fertig berechnet wurde.
bool Spielfeld::istFertigBerechnet() const
{
    bool erg = true;

    // Sobald ein Teil nicht fertig berechnet ist gilt das Spielfeld insgesamt als nicht fertig berechnet.
    for (register int idx = 0; erg && idx < fertigBerechnet.size(); idx++)
    {
        erg = fertigBerechnet.at(idx);
    }

    return erg;
}


// Mit Hilfe dieser Methode kann festgestellt werden ob das Spielfeld endlos ist.
bool Spielfeld::Endlos() const
{
    return endlos;
}


// Diese Methode dient der ermittlung der lebenden Zellen.
QList<Zelle*> Spielfeld::lebendeZellen() const
{
    // Speicher fur das Ergebnis, die lebenden Zellen.
    QList<Zelle*> erg;

    // Zunaechst alle Zellen ermitteln.
    QList<Zelle*> alleZellen(zellen.values());

    for (register int idx = 0; idx < alleZellen.size(); idx++)
    {
        if (alleZellen.at(idx)->istLebendig()) erg.append(alleZellen[idx]);
    }

    return erg;
}


// Diese Methode dient der Ermittlung der lebenden Zellen.
int Spielfeld::anzahlLebendeZellen() const
{
    return lebendeZellen().size();
}


// Diese Methode dient der Ermittlung der toten Zellen.
QList<Zelle*> Spielfeld::toteZellen() const
{
    // Speicher fur das Ergebnis, die lebenden Zellen.
    QList<Zelle*> erg;

    // Zunaechst alle Zellen ermitteln.
    QList<Zelle*> alleZellen(zellen.values());

    for (register int idx = 0; idx < alleZellen.size(); idx++)
    {
        if (!alleZellen.at(idx)->istLebendig()) erg.append(alleZellen[idx]);
    }

    return erg;
}


// Diese Methode dient der ermittlung aller Zellen.
QList<Zelle*> Spielfeld::alleZellen() const
{
    return zellen.values();
}


int Spielfeld::anzahlZellen() const
{
    return zellen.size();
}


int Spielfeld::anzahlToteZellen() const
{
    return toteZellen().size();
}
