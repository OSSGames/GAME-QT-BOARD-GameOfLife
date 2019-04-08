#include "Scene.h"
#include "Viewer.h"
#include "ElementItem.h"
#include <QThread>

using namespace std;

// Diese Klasse repraesentiert die Scene zur Darstellung des Spiels.
// Konstruktor
Scene::Scene(Viewer *parent) : QGraphicsScene(parent), breite(0), hoehe(0), meinViewer(parent), threads(QThread::idealThreadCount()), endlos(true)
{
    // Reaktion auf angeklickte Zellen ermoeglichen
    connect(this, SIGNAL(zelleAngeklickt(ElementItem*)), this, SLOT(reaktionAufzelleAngeklickt(ElementItem*)));
}


// Destruktor
Scene::~Scene()
{
}


// Diese Methode dient der uebernahme eines Spielfeldes.
void Scene::setSpielfeld(Spielfeld feld)
{
    // Die Liste der lebenden Elemente loeschen. Diese Liste ist nur fuer das Vorbereitungsspielfeld relevant. Lebende Elemente werden durch das anklicken von Elementen erfasst, was nur moeglich ist wenn der Viewer im interaktive Modus ist.
    lebendeElemente.clear();
    emit anzahlLebendeElemente(0);

    // Das Spielfeld uebernehmen.
    spielfeld = feld;

    // Die Elemente erzeugen.
    erzeugeElemente();

    // Den aktuellen Zellstatus melden
    sendeZellstatus(spielfeld.anzahlZellen(), spielfeld.anzahlLebendeZellen());
}


// Diese Methode dient der Erzeugung der Elemente zur Darstellung des Spielfeldes. Es wird versucht die bestehenden Elemente wieder zu verwenden.
void Scene::erzeugeElemente()
{
    // Wenn sich die Abmessung des Spielfeldes geaendert haben sollte.
    if (breite != spielfeld.Breite() || hoehe != spielfeld.Hoehe())
    {
        // Die Scene ...
        clear();

        // Und die Liste der Elemente leeren.
        elemente.clear();

        // Breite und Hoehe des SPielfeldes uebernehmen.
        breite = spielfeld.Breite();
        hoehe = spielfeld.Hoehe();

        // Fur alle Elemente im Spielfeld.
        for (register int idx_y = 0; idx_y < hoehe; idx_y++)
        {
            for (register int idx_x = 0; idx_x < breite; idx_x++)
            {
                // Ein neues Element erzeugen. Dabei den Lebens Status aus dem SPielfeld uebernehmen.
                ElementItem *item = new ElementItem(spielfeld.zelleAnPosition(Position(idx_x, idx_y))->istLebendig(), QSizeF((float) width() / (float) breite, (float) height() / (float) hoehe), this);

                // Das Element der Liste von Elemente hinzufuegen und in die Scene einfuegen.
                elemente.insert(Position(idx_x, idx_y), item);
                addItem(item);

                // Signal - Slot Verbindungen
                // Darauf reagieren wenn ein GUI Element angeklickt wird.
                connect(item, SIGNAL(angeklickt(ElementItem*)), this, SIGNAL(zelleAngeklickt(ElementItem*)));
            }
        }

        // Die Elemente verteilen.
        verteileElemente();
    }

    // Wenn die Elemente wiederverwendet werden koennen.
    else
    {
        // Fuer alle Elemente des Spielfeldes.
        for (register int idx_y = 0; idx_y < hoehe; idx_y++)
        {
            for (register int idx_x = 0; idx_x < breite; idx_x++)
            {
                // Das GUI Element ermitteln das das gerade betrachtete Element des Spielfeldes repraesentiert.
                ElementItem *item = elemente.value(Position(idx_x, idx_y));

                // Lebenszustand der GUI Elementes anpassen.
                item->setLeben(spielfeld.zelleAnPosition(Position(idx_x, idx_y))->istLebendig());
            }
        }
    }
}


// Dieses Methode dient der Verteilung der GUI Elemente innerhalb der Scene.
void Scene::verteileElemente()
{
    // Die einem GUI Element bereitstehende Breite und Hoehe berechnen.
    float elementbreite = (float) width() / (float) breite;
    float elementhoehe = (float) height() / (float) hoehe;

    // Fuer alle GUI Elemente ausfuehren.
    for (register int idx_y = 0; idx_y < hoehe; idx_y++)
    {
        for (register int idx_x = 0; idx_x < breite; idx_x++)
        {
            // Ein ELment ermitteln.
            ElementItem *item = elemente.value(Position(idx_x, idx_y));

            // Abmessung und Position der Zelle anpassen.
            item->setzeAbmessung(QSizeF(elementbreite, elementhoehe));
            item->setPos(elementbreite * idx_x, elementhoehe * idx_y);
        }
    }
}


// Diese Methode dient der Anzeige des Vorbereitungsspielfeldes.
void Scene::zeigeVorbereitungsSpielfeld(int breite_, int hoehe_, bool endlos_)
{
    // Das Spielfeld erzeugen.
    Spielfeld spielfeld(breite_, hoehe_, threads, endlos_);

    // Das spielfeld uebernehmen.
    setSpielfeld(spielfeld);
}


// Diese Methode gibt den Zustand des Spielfeldes in der GUI als Spielfeld zurueck.
Spielfeld Scene::getSpielfeld()
{
    // Dpielfeld erzeugen das der Abmessung des in der GUI angezeigten Spielfeldes entspricht.
    Spielfeld erg(breite, hoehe, QThread::idealThreadCount(), endlos);

    // Fuer alle Zellen.
    for (register int idx_y = 0; idx_y < hoehe; idx_y++)
    {
        for (register int idx_x = 0; idx_x < breite; idx_x++)
        {
            // Das gerade betrachtete GUI Element ermitteln.
            ElementItem *item = elemente.value(Position(idx_x, idx_y));

            // Wenn das gerade betrachtete GUI Element lebt das entsprechende Element im Spielfeld beleben.
            if (item->istLebendig()) erg.zelleAnPosition(Position(idx_x, idx_y))->beleben();
        }
    }

    return erg;
}


// Diese Methode reagiert auf Klicks auf Elemente.
void Scene::reaktionAufzelleAngeklickt(ElementItem* element)
{
    // Wenn das angeklickte Element lebendig ist ...
    if (element->istLebendig())
    {
        // ... und nicht in der Liste der lebenden Zellen vorhanden ist ...
        if (!lebendeElemente.contains(element))
        {
            // das Element in die Liste der lebenden Elemente einfuegen.
            lebendeElemente.append(element);

            // Anzahl der lebenden Elemente senden.
            emit anzahlLebendeElemente(lebendeElemente.size());
            sendeZellstatus(spielfeld.anzahlZellen(), lebendeElemente.size());
        }
    }

    // Wenn das angeklickte Element nicht lebendig ist ...
    else
    {
        // ... aber in der Liste der lebenden Elemente vorhanden ist ...
        if (lebendeElemente.contains(element))
        {
            // das Element aus der Liste der lebenden Elemente entfernen.
            lebendeElemente.removeAll(element);

            // Anzahl der lebenden Elemente senden.
            emit anzahlLebendeElemente(lebendeElemente.size());
            sendeZellstatus(spielfeld.anzahlZellen(), lebendeElemente.size());
        }
    }
}


void Scene::setzeEndlos(bool zustand)
{
    endlos = zustand;
}


void Scene::sendeZellstatus(int anzahlZellen, int anzahlLebendeZellen)
{
    emit zell_status(anzahlLebendeZellen, anzahlZellen - anzahlLebendeZellen);
}
