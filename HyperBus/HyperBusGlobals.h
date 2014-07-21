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

#ifndef HYPERBUSGLOBALS_H
#define HYPERBUSGLOBALS_H

#include <QString>

class HyperBusGlobals
{
public:
    enum PermissionUserType{
        Owner,
        CurrentUser,
        LocalUsers,
        PublicUsers
    };

    enum FireWallMode{
        BlackList,
        WhiteList,
        Disable
    };

    static void nsleep( quint64 ns );
};

#endif // HYPERBUSGLOBALS_H