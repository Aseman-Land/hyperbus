#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>
#include <QImage>

class TestObject : public QObject
{
    Q_OBJECT
public:
    TestObject(QObject *parent = 0);

public slots:
    QString ping();
    QImage getImage();

    void void_call();
    void in_int_call( int a );
    int out_int_call();
    int inout_int_call( int a );
    void in_string_call( const QString & a );
    QString out_string_call();
    QString inout_string_call( const QString & a );

    
signals:
    
public slots:
    
};

#endif // TESTOBJECT_H
