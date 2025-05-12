#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QQmlEngine>

class Bridge : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit Bridge(QObject *parent = nullptr);

public Q_SLOTS:
    QStringList urlsToFilenames(QList<QUrl> urls);

Q_SIGNALS:
    void filesSelected(QList<QUrl> urls);
    void clearFiles();
};

#endif // BRIDGE_H
