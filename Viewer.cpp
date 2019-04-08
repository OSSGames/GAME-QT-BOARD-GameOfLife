#include "Viewer.h"
#include "Scene.h"
#include <QResizeEvent>
#include <QDateTime>

using namespace std;

// Konstruktor
Viewer::Viewer(QWidget *parent) : QGraphicsView(parent)
{
    scene = new Scene(this);
    setScene(scene);

    // Signal - Slot Verbindungen
    // Anzahl der angeklickten lebenden Zellen weitergeben
    connect(scene, SIGNAL(anzahlLebendeElemente(int)), this, SIGNAL(anzahlLebendeElemente(int)));

    // Zellstatus des angezeigten Spielfeldes weitergeben
    connect(scene, SIGNAL(zell_status(int, int)), this, SIGNAL(zell_status(int, int)));
}


// Diese Methode dient der Uebergabe des anzuzeigenden Spielfeldes.
void Viewer::setSpielfeld(Spielfeld feld)
{
    // Beginn der Uebernahme erfassen.
    QDateTime beginn = QDateTime::currentDateTime();

    // Die Scene soll nicht auf Mausklicks reagieren, der Nutzer soll nicht in den Ablauf eingreifen.
    setInteractive(false);

    // Das Spielfeld uebernehmen.
    scene->setSpielfeld(feld);

    // Das Ende der Uebernahme ermitteln.
    QDateTime ende = QDateTime::currentDateTime();

    // Darueber informieren das das Spielfeld in der GUI fertig aufgebaut wurde. Die zum Aufbau benoetigte Zeit mitgeben, in Millisekunden. Diese wird beim abspielen benoetigt um, sofern noetig, naeher an die gewuenschte Abspielgeschwindigkeit zu gelangen un um den Abspielvorgang moeglichst zu glaetten.
    emit spielfeldAufgebaut(beginn.msecsTo(ende));
}


// Diese Methode wird aufgerufen wenn sich die Groesse des Viewers aendert.
void Viewer::resizeEvent(QResizeEvent* event)
{
    if (event->size().width() > 0 && event->size().height() > 0)
    {
        QGraphicsView::resizeEvent(event);

        this->setSceneRect(0, 0, event->size().width(), event->size().height());
        scene->setSceneRect(0, 0, event->size().width(), event->size().height());
        scene->verteileElemente();
    }
}


// Diese Methode dient dem anzeigen des Vorbereitungs Spielfeldes. Der Viewer soll Interactive sein, der Nutzer soll den Lebens Zustand von Zellen bearbeiten koennen.
void Viewer::zeigeVorbereitungsSpielfeld(int breite_, int hoehe_, bool endlos_)
{
    // Den Aufbau des Vorberitungsspielfeldes an die Scene weiterdelegieren.
    scene->zeigeVorbereitungsSpielfeld(breite_, hoehe_, endlos_);

    // Den Viewer interaktive machen.
    setInteractive(true);
}


// Diese Methode gibt den Zustand des Spielfeldes in der GUI als Spielfeld zurueck.
Spielfeld Viewer::getSpielfeld()
{
    // Aufgabe an Scene weiterdelegieren.
    return scene->getSpielfeld();
}


void Viewer::setzeEndlos(bool zustand)
{
    scene->setzeEndlos(zustand);
}
