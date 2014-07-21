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

#include "HyperBusTools.h"

#include <QHash>

#ifdef Q_OS_WIN32
#include <QLibrary>
#include <QCoreApplication>
#include <windows.h>

typedef BOOL (WINAPI *QueryFullProcessImageName)(HANDLE hProcess,
                                                    DWORD dwFlags,
                                                    LPTSTR lpExeName,
                                                    PDWORD lpdwSize);
static QueryFullProcessImageName pQueryFullProcessImageName= 0;
#endif

QHash<quint64,QFileInfo> pid_binaries_cache;

QFileInfo HyperBusTools::getPidBinaryPath(quint64 pid)
{
    QFileInfo res;

#ifndef Q_OS_WIN
    res = QFileInfo( "/proc/" + QString::number(pid) + "/exe" );
#else
    if( !pQueryFullProcessImageName )
        pQueryFullProcessImageName = (QueryFullProcessImageName) QLibrary::resolve("kernel32", "QueryFullProcessImageNameW");

    quint64 pid = pid;
    DWORD PID = pid; // something here
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PID);

    DWORD value = MAX_PATH;
    WCHAR buffer[MAX_PATH];
    pQueryFullProcessImageName(hProcess, 0,  (LPWSTR)&buffer, &value);
    QString processPath = QString::fromUtf16((ushort*)buffer, value);

    res = QFileInfo( processPath.replace("\\","/") );
#endif

    return res;
}
