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

#include "testobject.h"
#include <QApplication>

#include <HyperBus.h>
#include <HMsgTransporter.h>
#include <HyperBusRecord.h>

#include <QUuid>
#include <QTime>
#include <QDebug>
#include <QTest>
#include <QThread>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString append_txt;
    if( app.arguments().count() > 1 )
        append_txt = app.arguments().at(1);

    QProgressBar *pr = new QProgressBar();
    QLabel *label = new QLabel();

    QWidget main;
    QVBoxLayout *layout = new QVBoxLayout(&main);
    layout->addWidget(label);
    layout->addWidget(pr);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(1);
    main.show();

    HyperBus reciever;
    reciever.setSession( HyperBusReciever::Global );

    TestObject *test = new TestObject();
    test->pbar = pr;
    test->hbus = &reciever;
    test->append_txt = append_txt;

    reciever.registerService( "/setValue" + append_txt, test, SLOT(setProgress(int)), "test", HyperBusReciever::Global );
    reciever.registerService( "/getImage" + append_txt, test, SLOT(getImage()), "test", HyperBusReciever::Global );
    reciever.registerService( "/ping" + append_txt, test, SLOT(ping()), "test", HyperBusReciever::Global );
    reciever.registerService( "/setText" + append_txt, label, SLOT(setText(QString)), "test", HyperBusReciever::Global );

    reciever.registerService( "/test0" + append_txt, test, SLOT(void_call()), "test", HyperBusReciever::Global );
    reciever.registerService( "/test1" + append_txt, test, SLOT(in_int_call(int)), "test", HyperBusReciever::Global );
    reciever.registerService( "/test2" + append_txt, test, SLOT(out_int_call()), "test", HyperBusReciever::Global );
    reciever.registerService( "/test3" + append_txt, test, SLOT(inout_int_call(int)), "test", HyperBusReciever::Global );
    reciever.registerService( "/test4" + append_txt, test, SLOT(in_string_call(QString)), "test", HyperBusReciever::Global );
    reciever.registerService( "/test5" + append_txt, test, SLOT(out_string_call()), "test", HyperBusReciever::Global );
    reciever.registerService( "/test6" + append_txt, test, SLOT(inout_string_call(QString)), "test", HyperBusReciever::Global );

    return app.exec();
}
