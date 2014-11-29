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

#include "myobject.h"

class MyObjectPrivate
{
public:
    HyperBus *hbus;
    QString appendTxt;
};

MyObject::MyObject(HyperBus *hbus, QString appendTxt, QObject *parent) :
    QObject(parent)
{
    p = new MyObjectPrivate;
    p->hbus = hbus;
    p->appendTxt = appendTxt;
}

void MyObject::setText(const QString &txt)
{
    p->hbus->call("/setText" + p->appendTxt, txt);
}

void MyObject::setValue(int v)
{
    p->hbus->call("/setValue" + p->appendTxt, v);
}

MyObject::~MyObject()
{
    delete p;
}
