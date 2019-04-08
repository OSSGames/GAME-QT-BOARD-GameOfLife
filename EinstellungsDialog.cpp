#include "EinstellungsDialog.h"

using namespace std;

// Diese Klasse repraesentiert den Einstellungsdialog.
// Konstruktor
EinstellungsDialog::EinstellungsDialog(QWidget *parent) : QDialog(parent), backupBreite(30), backupHoehe(30), backupPlayzeit(300), backupEndlos(true)
{
    setupUi(this);

    // Signal - Slot Verbindungen
    // Reaktion auf Accept ermoeglichen
    connect(this, SIGNAL(accepted()), this, SLOT(reaktionAufAccept()));

    // Reaktion auf Cancel ermoeglichen
    connect(this, SIGNAL(rejected()), this, SLOT(reaktionAufCancel()));
}


// Destruktor
EinstellungsDialog::~EinstellungsDialog()
{
}


void EinstellungsDialog::setzeBreite(int wert)
{
    breite_box->setValue(wert);

    backupBreite = breite_box->value();
}


void EinstellungsDialog::setzeHoehe(int wert)
{
    hoehe_box->setValue(wert);

    backupHoehe = hoehe_box->value();
}


void EinstellungsDialog::setzeEndlos(bool zustand)
{
    backupEndlos = zustand;

    endlos_checkbox->setChecked(zustand);
}


void EinstellungsDialog::setzePlayzeit(int wert)
{
    playspeed_sbox->setValue(wert);

    backupPlayzeit = playspeed_sbox->value();
}


int EinstellungsDialog::getBreite() const
{
    return backupBreite;
}


int EinstellungsDialog::getHoehe() const
{
    return backupHoehe;
}


int EinstellungsDialog::getPlayzeit() const
{
    return backupPlayzeit;
}


bool EinstellungsDialog::getEndlos() const
{
    return backupEndlos;
}


void EinstellungsDialog::reaktionAufAccept()
{
    if (breite_box->isEnabled() && (breite_box->value() != backupBreite || hoehe_box->value() != backupHoehe || endlos_checkbox->isChecked() != backupEndlos))
    {
        backupBreite = breite_box->value();
        backupHoehe = hoehe_box->value();
        backupEndlos = endlos_checkbox->isChecked();

        emit anpassungenFuerSpielfeld(backupBreite, backupHoehe, backupEndlos);
    }

    if (playspeed_sbox->isEnabled() && playspeed_sbox->value() != backupPlayzeit)
    {
        backupPlayzeit = playspeed_sbox->value();

        emit anpassungenFuerPlayzeit(backupPlayzeit);
    }
}


void EinstellungsDialog::reaktionAufCancel()
{
    if (breite_box->isEnabled())
    {
        breite_box->setValue(backupBreite);
        hoehe_box->setValue(backupHoehe);
        endlos_checkbox->setChecked(backupEndlos);
    }

    if (playspeed_sbox->isEnabled())
    {
        playspeed_sbox->setValue(backupPlayzeit);
    }
}


void EinstellungsDialog::sperreSpielfeldEinstellungen(bool zustand)
{
    breite_box->setEnabled(!zustand);
    hoehe_box->setEnabled(!zustand);
    endlos_checkbox->setEnabled(!zustand);
}


void EinstellungsDialog::sperrePlayzeitEinstellungen(bool zustand)
{
    playspeed_sbox->setEnabled(!zustand);
}
