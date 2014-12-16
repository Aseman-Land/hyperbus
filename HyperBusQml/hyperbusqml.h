#ifndef HYPERBUSQML_H
#define HYPERBUSQML_H

#include <QObject>
#include <HyperBus>

class HyperBusQmlPrivate;
class HyperBusQml : public QObject
{
    Q_OBJECT
    Q_ENUMS(SessionType)

    Q_PROPERTY(QString source  READ source  WRITE setSource  NOTIFY sourceChanged )
    Q_PROPERTY(int     session READ session WRITE setSession NOTIFY sessionChanged)

public:
    enum SessionType{
        Normal = HyperBus::Normal,
        Global = HyperBus::Global,
        Share  = HyperBus::Share,
        Local  = HyperBus::Local
    };

    HyperBusQml(QObject *parent = 0);
    ~HyperBusQml();

    void setSource(const QString &source);
    QString source() const;

    void setSession(int s);
    int session() const;

public slots:
    QVariant call( const QString & key , const QVariant & val0 = QVariant(),
                                         const QVariant & val1 = QVariant(),
                                         const QVariant & val2 = QVariant(),
                                         const QVariant & val3 = QVariant(),
                                         const QVariant & val4 = QVariant(),
                                         const QVariant & val5 = QVariant(),
                                         const QVariant & val6 = QVariant(),
                                         const QVariant & val7 = QVariant(),
                                         const QVariant & val8 = QVariant(),
                                         const QVariant & val9 = QVariant() );

signals:
    void sourceChanged();
    void sessionChanged();

private:
    HyperBusQmlPrivate *p;
};

#endif // HYPERBUSQML_H

