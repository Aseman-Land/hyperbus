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

#include <QApplication>
#include "myobject.h"

#include <HyperBus.h>
#include <HMsgTransporter.h>
#include <HyperBusRecord.h>
#include <HyperBusGlobals.h>

#include <QUuid>
#include <QTime>
#include <QDebug>
#include <QTest>
#include <QThread>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString append_txt;
    if( app.arguments().count() > 1 )
        append_txt = app.arguments().at(1);

    HyperBus reciever;
    reciever.setSession( HyperBusReciever::Global );

    MyObject *myobj = new MyObject(&reciever,append_txt);

    QLineEdit *line = new QLineEdit();

    QSlider *sldr = new QSlider();
    sldr->setOrientation(Qt::Horizontal);
    sldr->setMinimum(0);
    sldr->setMaximum(100);

    QProgressBar *pbar = new QProgressBar();
    pbar->setValue(100);

    QWidget main;
    QVBoxLayout *layout = new QVBoxLayout(&main);
    layout->addWidget(line);
    layout->addWidget(sldr);
    layout->addWidget(pbar);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(1);
    main.show();

    QObject::connect( line, SIGNAL(textChanged(QString)), myobj, SLOT(setText(QString)) );
    QObject::connect( sldr, SIGNAL(valueChanged(int)), myobj, SLOT(setValue(int)) );

    reciever.registerService( "/setValue2" + append_txt, pbar, SLOT(setValue(int)), "test", HyperBusReciever::Global );

    return app.exec();

    QTime t1 = QTime::currentTime();
    for( int i=0 ; i<10000; i++ )
    {
        reciever.call("/setValue" + append_txt, i%100);
        QThread::msleep(10);
    }
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        qDebug() << reciever.call("/ping" + append_txt);
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test0" + append_txt);
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test1" + append_txt, i );
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test2" + append_txt);
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test3" + append_txt, i );
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test4" + append_txt, QString::number(i) );
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test5" + append_txt);
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();

    for( int i=0 ; i<10000; i++ )
        reciever.call("/test6" + append_txt, QString::number(i));
    qDebug() << t1.msecsTo(QTime::currentTime());
    t1 = QTime::currentTime();


    return app.exec();
}

