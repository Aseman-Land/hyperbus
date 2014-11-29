/*
    Copyright (C) 2014 Aseman
    http://aseman.co

    HyperBus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HyperBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <hyperbus.h>

class MyObjectPrivate;
class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject(HyperBus *hbus, QString appendTxt, QObject *parent = 0);
    ~MyObject();

public slots:
    void setText( const QString & txt );
    void setValue( int v );

private:
    MyObjectPrivate *p;
};

#endif // MYOBJECT_H
