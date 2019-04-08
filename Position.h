/*

 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef POSITION_H
#define POSITION_H

#include <QString>

class Position
{
public:
    Position(int = 0, int = 0);
    Position(const Position&);
    Position(const QByteArray&);
    Position(const QString&);
    ~Position();

    int x() const;
    int y() const;

    void setX(int);
    void setY(int);

    bool operator==(const Position&) const;
    bool operator!=(const Position&) const;
    bool operator<(const Position&) const;
    const Position& operator=(const Position&);

    QString toString() const;
    QByteArray toByteArray() const;

private:
    int X, Y;
};

#endif
