/*

 Name: GameOfLife
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef EINSTELLUNGSDIALOG_H
#define EINSTELLUNGSDIALOG_H

#include <QDialog>
#include "ui_EinstellungsDialog.h"

class EinstellungsDialog : public QDialog, private Ui::EinstellungsDialog
{
    Q_OBJECT

public:
    EinstellungsDialog(QWidget *parent);
    virtual ~EinstellungsDialog();

    int getBreite() const;
    int getHoehe() const;
    int getPlayzeit() const;
    bool getEndlos() const;

public slots:
    void setzeBreite(int);
    void setzeHoehe(int);
    void setzeEndlos(bool);
    void setzePlayzeit(int);
    void sperreSpielfeldEinstellungen(bool);
    void sperrePlayzeitEinstellungen(bool);

private slots:
    void reaktionAufAccept();
    void reaktionAufCancel();

signals:
    void anpassungenFuerSpielfeld(int, int, bool);
    void anpassungenFuerPlayzeit(int);

private:
    int backupBreite, backupHoehe, backupPlayzeit;
    bool backupEndlos;
};

#endif
