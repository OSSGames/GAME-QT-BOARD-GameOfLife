#include "JobSender.h"
#include "Spielfeld.h"

using namespace std;

// Diese Klasse stellt die Verbindungen zu den Recheneinheiten bereit.
JobSender::JobSender(QObject *parent) : QObject(parent)
{	
}

JobSender::~JobSender()
{	
}

// Diese Methode nimmt das Resultat "ihrer" Recheneinheit an.
void JobSender::fertigBerechnet(Spielfeld* feld, int threadnr)
{
    emit resultat(feld, threadnr);
}

// Diese Methode gibt den Arbeitsauftrag an "Ihre" Recheneinheit weiter.
void JobSender::fuehreBerechnungAus(Spielfeld* vorgeneration, Spielfeld* aktuelleGeneration)
{
    emit berechne(vorgeneration, aktuelleGeneration);
}
