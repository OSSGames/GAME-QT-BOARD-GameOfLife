#include "MainWindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>
#include "Spielfeld.h"
#include "GameOfLife.h"
#include "Zelle.h"
#include "Position.h"
#include "GameOfLifeRandom.h"
#include "EinstellungsDialog.h"

using namespace std;

#define SPIELFELD_DEFAULT_BREITE 15
#define SPIELFELD_DEFAULT_HOEHE 15
#define SPIELFELD_DEFAULT_ENDLOS true
#define SPIELFELD_DEFAULT_SPIELZEIT 300

// Diese Klasse repraesentiert das Hauptfenster der Anwendung.
// Konstruktor.
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), einstellungen(0), breite(SPIELFELD_DEFAULT_BREITE), hoehe(SPIELFELD_DEFAULT_HOEHE), endlos(SPIELFELD_DEFAULT_ENDLOS)
{
    // die benutzeroberflaeche aufbauen
    setupUi(this);

    // osx spezifische einstellungen vornehmen
#ifdef Q_WS_MAC

    setUnifiedTitleAndToolBarOnMac(true);

#endif

    // unter symbian os die statusleiste verbergen
#ifdef Q_OS_SYMBIAN

    statusBar()->hide();

#endif

    // unter macos soll menu_datei samt inhalt nicht zu sehen sein wenn nur action_schliessen enthalten ist
#ifdef Q_OS_MAC

    action_schliessen->setVisible(false);
    if (menu_datei->actions().size() == 1 && menu_datei->actions().first() == action_schliessen) menu_datei->menuAction()->setVisible(false);

#endif

    // Metatypen registrieren
    qRegisterMetaType<Spielfeld>("Spielfeld");

    // GameOfLife Objekt erzeugen.
    gameOfLife = new GameOfLife(this);

    // einstellungen erstellen
    einstellungen = new QSettings("konarski-wuppertal", qApp->applicationName(), this);

    // Info Labels erstellen und der Statusleiste als permanente Widgets hinzufuegen.
    vorwaertsCacheLabel = new QLabel(this);
    rueckwaertsCacheLabel = new QLabel(this);
    lebendeZellenLabel = new QLabel(this);
    toteZellenLabel = new QLabel(this);

    statusBar()->addPermanentWidget(toteZellenLabel);
    statusBar()->addPermanentWidget(lebendeZellenLabel);
    statusBar()->addPermanentWidget(rueckwaertsCacheLabel);
    statusBar()->addPermanentWidget(vorwaertsCacheLabel);

    vorwaertsCacheLabel->setText("0");
    rueckwaertsCacheLabel->setText("0");

    vorwaertsCacheLabel->setToolTip(tr("Forward Cache"));
    rueckwaertsCacheLabel->setToolTip(tr("Backward Cache"));
    lebendeZellenLabel->setToolTip(tr("Number of living cells"));
    toteZellenLabel->setToolTip(tr("Number of dead cells"));

    random = new GameOfLifeRandom(this);

    // Den Einstellungsdialog erzeugen
    einstellungsDialog = new EinstellungsDialog(this);

    // Die Einstellungen laden.
    einstellungen_laden();

    gameOfLife->setzeIntervall(einstellungsDialog->getPlayzeit());

    viewer->setzeEndlos(endlos);

    // signal - slot verbindungen
    // action_schliessen ermoeglichen
    connect(action_schliessen, SIGNAL(triggered(bool)), this, SLOT(close()));

    // action_about ermoeglichen
    connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));

    // action_about_qt ermoeglichen
    connect(action_about_qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    // Das annehmen von Spielfeldern ermoeglichen
    connect(gameOfLife, SIGNAL(neuesSpielfed(Spielfeld)), viewer, SLOT(setSpielfeld(Spielfeld)));

    // Vorwaerts und Rurckwaerts ermoeglichen
    connect(action_vorwaerts, SIGNAL(triggered(bool)), this, SLOT(vorwaerts()));
    connect(action_zurueck, SIGNAL(triggered(bool)), this, SLOT(zurueck()));

    // Starten eines Spiels ermoeglichen
    connect(action_start, SIGNAL(triggered(bool)), this, SLOT(starteSpiel()));

    // Verarbeiten des Cache fuellstaende ermoeglichen
    connect(gameOfLife, SIGNAL(fuellstand_vorwaerts_cache(int)), this, SLOT(verarbeiteFuellstand_vorwaerts_cache(int)));
    connect(gameOfLife, SIGNAL(fuellstand_rueckwaerts_cache(int)), this, SLOT(verarbeiteFuellstand_rueckwaerts_cache(int)));

    // Reaktion auf Start eines neuen Spiels ermoeglichen.
    connect(gameOfLife, SIGNAL(spielGestartet()), this, SLOT(spielGestartet()));

    // Das resetten des Spiels ermoeglichen
    connect(action_reset, SIGNAL(triggered(bool)), this, SLOT(reset()));

    // Game Of Life muss beim abspielen darauf reagieren koennen wenn die GUI komplett aufgebaut wurde.
    connect(viewer, SIGNAL(spielfeldAufgebaut(int)), gameOfLife, SLOT(reaktionGuiFertig(int)));

    // Play ermoeglichen
    connect(action_play, SIGNAL(triggered(bool)), this, SLOT(play()));

    // Stop ermoeglichen
    connect(action_stop, SIGNAL(triggered(bool)), this, SLOT(stop()));

    // Reaktionen auf Start und Stop eines Spiels ermoeglichen.
    connect(gameOfLife, SIGNAL(abspielenGestoppt()), this, SLOT(abspielenGestoppt()));
    connect(gameOfLife, SIGNAL(abspielenGestartet()), this, SLOT(abspielenGestartet()));

    // Die Verfuegbarkeit von StartSpiel steuern.
    connect(viewer, SIGNAL(anzahlLebendeElemente(int)), this, SLOT(steuereVerfuegbarkeitStartSpiel(int)));

    // Erstellen von zufallsgenerierten Spielen ermoeglichen
    connect(action_zufall, SIGNAL(triggered(bool)), this, SLOT(generierePerZufall()));
    connect(random, SIGNAL(neuesSpielfeld(Spielfeld)), gameOfLife, SLOT(starteSpiel(Spielfeld)));

    // Reaktionen auf Reset ermoeglichen
    connect(gameOfLife, SIGNAL(spielResettet()), this, SLOT(reaktionAufReset()));

    // Das annehmen von einstellungen aus dem Einstellungsdialog ermoeglichen
    connect(einstellungsDialog, SIGNAL(anpassungenFuerSpielfeld(int,int,bool)), this, SLOT(annahmeAnpassungenFuerSpielfeld(int,int,bool)));
    connect(einstellungsDialog, SIGNAL(anpassungenFuerPlayzeit(int)), gameOfLife, SLOT(setzeIntervall(int)));

    // Oeffnen des Einstellungsdialogs ermoeglichen
    connect(action_einstellungen, SIGNAL(triggered(bool)), einstellungsDialog, SLOT(show()));

    // Annahme des Zellstatus des gerade angezeigten Spielfeldes ermoeglichen
    connect(viewer, SIGNAL(zell_status(int,int)), this, SLOT(annahmeZellstatus(int,int)));

    // Das Vorbereitungsspielfeld zeigen.
    viewer->zeigeVorbereitungsSpielfeld(breite, hoehe, endlos);
}


// Informationen zur Anwendung ausgeben.
void MainWindow::about()
{
    QMessageBox::about(this, tr("About GameOfLife"), tr("GameOfLife version %1 \n\nAuthor:\tAndreas Konarski\nLicence:\tgpl v3 or later\n\nContact:\n\nprogrammieren@konarski-wuppertal.de\nwww.konarski-wuppertal.de").arg(VERSION));
}


// Destruktor.
MainWindow::~MainWindow()
{
    einstellungen_speichern();
}


// Diese Methode dient dem Laden der Einstellungen.
void MainWindow::einstellungen_laden()
{
    // Die Fenstergeometrie laden.
    restoreGeometry(einstellungen->value("MainWindow/Geometrie").toByteArray());

    // Zustand des Spielfeldes laden.
    einstellungsDialog->setzeBreite(einstellungen->value("GameOfLife/Breite", SPIELFELD_DEFAULT_BREITE).toInt());
    einstellungsDialog->setzeHoehe(einstellungen->value("GameOfLife/Hoehe", SPIELFELD_DEFAULT_HOEHE).toInt());
    einstellungsDialog->setzeEndlos(einstellungen->value("GameOfLife/Endlos", SPIELFELD_DEFAULT_ENDLOS).toBool());
    einstellungsDialog->setzePlayzeit(einstellungen->value("GameOfLife/Playzeit", SPIELFELD_DEFAULT_SPIELZEIT).toInt());

    breite = einstellungsDialog->getBreite();
    hoehe = einstellungsDialog->getHoehe();
    endlos = einstellungsDialog->getEndlos();
}


// Diese MEthode dient dem speichern der Einstellungen.
void MainWindow::einstellungen_speichern()
{
    // Die Fenstergeometrie speichern
    einstellungen->setValue("MainWindow/Geometrie", saveGeometry());

    // Zustand des Spielfeldes speichern.
    einstellungen->setValue("GameOfLife/Breite", breite);
    einstellungen->setValue("GameOfLife/Hoehe", hoehe);
    einstellungen->setValue("GameOfLife/Endlos", endlos);
    einstellungen->setValue("GameOfLife/Playzeit", einstellungsDialog->getPlayzeit());
}


// Diese Methode dient dem Start eines Spiels.
void MainWindow::starteSpiel()
{
    gameOfLife->starteSpiel(viewer->getSpielfeld());
}


// Mit Hilfe dieser Methode wird eine Generation weiter geschaltet.
void MainWindow::vorwaerts()
{
    gameOfLife->anforderungSpielfeld();
}


// Mit Hilfe dieser Methode wird eine Generation zurueck geschaltet.
void MainWindow::zurueck()
{
    gameOfLife->anforderungSchrittZurueck();
}


// Diese Methode dient der Verarbeitung des Fuellstandes der Vorwaerts Caches.
void MainWindow::verarbeiteFuellstand_vorwaerts_cache(int wert)
{
    action_vorwaerts->setEnabled(wert > 0 && !gameOfLife->getPlayStatus());
    action_play->setEnabled(wert > 0 && !gameOfLife->getPlayStatus());

    vorwaertsCacheLabel->setText(QString::number(wert));
}


// Diese Methode dient der Verarbeitung des Fuellstandes der Rueckwaerts Caches.
void MainWindow::verarbeiteFuellstand_rueckwaerts_cache(int wert)
{
    action_zurueck->setEnabled(wert > 0 && !gameOfLife->getPlayStatus());

    rueckwaertsCacheLabel->setText(QString::number(wert));
}


// Diese Methode wird ausgefuehrt wenn ein neues Spiel gestartet wurde. Nicht zu verwechseln mit Play!
void MainWindow::spielGestartet()
{
    action_start->setEnabled(false);
    action_zufall->setEnabled(false);
    action_reset->setEnabled(true);
    einstellungsDialog->sperreSpielfeldEinstellungen(true);
}


// Diese Methode setzt das Spiel zurueck damit ein neues Spiel begonnen werden kann.
void MainWindow::reset()
{
    // Die reset Action sperren.
    action_reset->setEnabled(false);

    // Ein neues Vorberitungsspielfeld im Viewer zeigen. Dies schaltet auch das anklicken von Zellen frei.
    viewer->zeigeVorbereitungsSpielfeld(breite, hoehe, endlos);

    // Die in Game of Life vorhandenen Spielfelder loeschen.
    gameOfLife->loescheSpielfelder();
}


// Diese Methode startet den Abspielvorgang.
void MainWindow::play()
{
    // Abspielvorgang starten.
    gameOfLife->play();
}


// Diese Methode dient dem Stop des Abspielvorgangs.
void MainWindow::stop()
{
    // Abspielvorgang stoppen.
    gameOfLife->stop();
}


// Diese Methode wird aufgerufen wenn in gameOfLife ein Abspielvorgang gestoppt wurde.
void MainWindow::abspielenGestoppt()
{
    action_reset->setEnabled(true);
    action_stop->setEnabled(false);
    action_reset->setEnabled(true);
    action_vorwaerts->setEnabled(gameOfLife->VorwaertsCache() > 0);
    action_zurueck->setEnabled(gameOfLife->RueckwaertsCache() > 0);
    action_play->setEnabled(gameOfLife->VorwaertsCache() > 0);
}


// Diese Methode wird aufgerufen wenn in gameOfLife ein Abspielvorgang gestartet wurde.
void MainWindow::abspielenGestartet()
{
    action_stop->setEnabled(true);
    action_reset->setEnabled(false);
    action_vorwaerts->setEnabled(false);
    action_zurueck->setEnabled(false);
}


// Diese Methode steuert die Verfuegbarkeit der Aktion zum Starten eines Spiels. Nicht zu verwechseln mit Play.
void MainWindow::steuereVerfuegbarkeitStartSpiel(int anzahlLebendeElemente)
{
    action_start->setEnabled(anzahlLebendeElemente > 0);
}


// Mit Hilfe dieser Methode wird ein Zufallsgeneriertes Spiel erzeugt.
void MainWindow::generierePerZufall()
{
    bool ok;

    // Die Anzahl der zu belebenden Zellen abfragen.
    int wert = QInputDialog::getInt(this, tr("GameOfLife"), tr("Number of living cells:"), breite * hoehe / 2, 1, breite * hoehe, 1, &ok);

    // Wenn der Dialog per OK geschlossen wurde.
    if (ok)
    {
        // Das Zufallsgenerierte Spielfeld erzeugen.
        random->erzeugeSpielfeld(breite, hoehe, wert, endlos);

        // Action Zufall sperren.
        action_zufall->setEnabled(false);
    }
}


// Diese Methode wird aufgerufen wenn das Spiel resettet wurde.
void MainWindow::reaktionAufReset()
{
    action_zufall->setEnabled(true);
    einstellungsDialog->sperreSpielfeldEinstellungen(false);
}


// Diese Methode nimmt aus dem Einstellungsdialog stammende anpassungen fuer das Spielfeld an.
void MainWindow::annahmeAnpassungenFuerSpielfeld(int breite, int hoehe, bool endlos)
{
    // Neue Werte uebernehmen.
    this->breite = breite;
    this->hoehe = hoehe;
    this->endlos = endlos;

    // Endlos Einstellung des Viewers fuer das Vorbereitungsspielfeld anpassen.
    viewer->setzeEndlos(endlos);

    // Das Vorbereitungsspielfeld zeigen.
    viewer->zeigeVorbereitungsSpielfeld(breite, hoehe, endlos);
}


void MainWindow::annahmeZellstatus(int anzahlLebendeZellen, int anzahlToteZellen)
{
    lebendeZellenLabel->setText(QString::number(anzahlLebendeZellen));
    toteZellenLabel->setText(QString::number(anzahlToteZellen));
}
