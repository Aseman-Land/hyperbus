/*
    Copyright (C) 2014 Sialan Labs
    http://labs.sialan.org

    HyperBus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HyperBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QCoreApplication>
#include "myserver.h"

#include <HyperBusServer.h>
#include <HTcpServer.h>

#include <QUuid>
#include <QTime>
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString ip_txt = "127.0.0.1:25480";
    if( app.arguments().count() > 1 )
        ip_txt = app.arguments().at(1);

    QStringList splits = ip_txt.split(":");
    MyServer server(splits.at(0),splits.at(1).toInt());
    return app.exec();
}
