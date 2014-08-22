/*
    Copyright (C) 2014 Sialan Labs
    http://labs.sialan.org

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

#include "HPidTools.h"

#include <QProcess>
#include <QStringList>
#include <QHash>
#include <QDebug>

QHash<quint64,QString> h_pid_tools_users_cache;

QString HPidTools::userOf(quint64 pid)
{
    if( h_pid_tools_users_cache.contains(pid) )
        return h_pid_tools_users_cache.value(pid);

    QProcess prc;
    prc.start("ps", QStringList()<< "-o" << "user" << QString::number(pid) );
    prc.waitForStarted();
    prc.waitForReadyRead();
    prc.waitForFinished();

    QStringList res = QString(prc.readAll()).split("\n",QString::SkipEmptyParts);
    if( res.isEmpty() )
        return QString();

    h_pid_tools_users_cache[pid] = res.last();
    return h_pid_tools_users_cache.value(pid);
}
