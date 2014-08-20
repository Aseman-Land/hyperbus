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

    QWidget main;
    QVBoxLayout *layout = new QVBoxLayout(&main);
    layout->addWidget(line);
    layout->addWidget(sldr);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(1);
    main.show();

    QObject::connect( line, SIGNAL(textChanged(QString)), myobj, SLOT(setText(QString)) );
    QObject::connect( sldr, SIGNAL(valueChanged(int)), myobj, SLOT(setValue(int)) );

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

