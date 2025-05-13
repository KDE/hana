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
    QString urlToFilename(QUrl url);
    void processFile(QUrl url);
    QString urlToLocalFile(QUrl url);
    QString parentPath(QString path);

Q_SIGNALS:
    void filesSelected(QList<QUrl> urls);
    void clearFiles();
    void run();
    void thumbGenerated(const QString &thumbPath);

private:
    QString thumbSaveLocation();
};

#endif // BRIDGE_H
