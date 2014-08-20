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

    TestObject *test = new TestObject();

    QProgressBar *pr = new QProgressBar();
    pr->show();

    QLabel *label = new QLabel();
    label->show();

    QWidget main;
    QVBoxLayout *layout = new QVBoxLayout(&main);
    layout->addWidget(label);
    layout->addWidget(pr);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(1);
    main.show();

    HyperBus reciever;
    reciever.registerService( "/setValue" + append_txt, pr, SLOT(setValue(int)), "test", HyperBusReciever::Global );
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
