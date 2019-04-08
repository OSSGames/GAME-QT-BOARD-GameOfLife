#include "Position.h"
#include <QByteArray>
#include <QStringList>

using namespace std;

Position::Position(int x_, int y_) : X(x_), Y(y_)
{
}


Position::~Position()
{
}


int Position::x() const
{
    return X;
}


int Position::y() const
{
    return Y;
}


bool Position::operator==(const Position& anderer) const
{
    bool erg = false;

    if (x() == anderer.x() && y() == anderer.y()) erg = true;

    return erg;
}


bool Position::operator!=(const Position& anderer) const
{
    bool erg = false;

    if (x() != anderer.x() || y() != anderer.y()) erg = true;

    return erg;
}


const Position& Position::operator=(const Position& anderer)
{
    if (&anderer != this)
    {
        X = anderer.x();
        Y = anderer.y();
    }

    return *this;
}


bool Position::operator<(const Position& anderer) const
{
    return toByteArray() < anderer.toByteArray();
}


Position::Position(const Position& anderer) : X(anderer.x()), Y(anderer.y())
{
}


QString Position::toString() const
{
    QString erg;

    erg += QString::number(X);
    erg += ", ";
    erg += QString::number(Y);

    return erg;
}


QByteArray Position::toByteArray() const
{
    QByteArray erg;

    erg.append(X);
    erg.append(Y);

    return erg;
}


Position::Position(const QByteArray& daten) : X(0), Y(0)
{
    if (daten.size() == 2)
    {
        X = (int) daten.at(0);
        Y = (int) daten.at(1);
    }
}


Position::Position(const QString& daten) : X(0), Y(0)
{
    QStringList gesplittet(QString(daten).remove(' ').split(',', QString::SkipEmptyParts));

    if (gesplittet.size() == 2)
    {
        bool ok = false;

        int tmp_x = gesplittet.first().toInt(&ok), tmp_y = 0;

        if (ok == true) tmp_y = gesplittet.last().toInt(&ok);

        if (ok == true)
        {
            X = tmp_x;
            Y = tmp_y;
        }
    }
}


void Position::setX(int wert)
{
    X = wert;
}


void Position::setY(int wert)
{
    Y = wert;
}
