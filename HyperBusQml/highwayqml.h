#ifndef HIGHWAYQML_H
#define HIGHWAYQML_H

#include <QObject>
#include <QVariant>
#include <QStringList>

class HighWayQmlPrivate;
class HighWayQml : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY keyChanged)

    Q_PROPERTY(QStringList keys READ keys NOTIFY changed)
    Q_PROPERTY(int count READ count NOTIFY changed)

public:
    HighWayQml(QObject *parent = 0);
    ~HighWayQml();

    void setSource(const QString &source);
    QString source() const;

    QStringList keys() const;
    int count() const;

public slots:
    bool contains( const QString & key );
    bool connectToSignal( const QString & key , QObject *reciver , const QString &member );
    QVariant property( const QString & key );
    QVariant call( const QString & key, QVariant val0 = QVariant(),
                                        QVariant val1 = QVariant(),
                                        QVariant val2 = QVariant(),
                                        QVariant val3 = QVariant(),
                                        QVariant val4 = QVariant(),
                                        QVariant val5 = QVariant(),
                                        QVariant val6 = QVariant(),
                                        QVariant val7 = QVariant(),
                                        QVariant val8 = QVariant(),
                                        QVariant val9 = QVariant() );

signals:
    void keyChanged();

    void added( const QString & kay );
    void removed( const QString & kay );

    void changed();

private:
    HighWayQmlPrivate *p;
};

#endif // HIGHWAYQML_H
