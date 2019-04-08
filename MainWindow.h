/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "0.20"

#include <QMainWindow>
#include "ui_MainWindow.h"

class QSettings;
class GameOfLife;
class QLabel;
class GameOfLifeRandom;
class EinstellungsDialog;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void about();
    void einstellungen_laden();
    void einstellungen_speichern();
    void starteSpiel();
    void vorwaerts();
    void zurueck();
    void verarbeiteFuellstand_vorwaerts_cache(int);
    void verarbeiteFuellstand_rueckwaerts_cache(int);
    void spielGestartet();
    void abspielenGestartet();
    void abspielenGestoppt();
    void reset();
    void play();
    void stop();
    void steuereVerfuegbarkeitStartSpiel(int);
    void generierePerZufall();
    void reaktionAufReset();
    void annahmeAnpassungenFuerSpielfeld(int, int, bool);
    void annahmeZellstatus(int, int);

signals:

private:
    QSettings *einstellungen;
    GameOfLife *gameOfLife;
    QLabel *vorwaertsCacheLabel, *rueckwaertsCacheLabel, *lebendeZellenLabel, *toteZellenLabel;
    GameOfLifeRandom *random;
    int breite, hoehe;
    bool endlos;
    EinstellungsDialog *einstellungsDialog;
};

#endif
