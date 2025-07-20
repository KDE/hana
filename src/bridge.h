#ifndef BRIDGE_H
#define BRIDGE_H

#include <QImage>
#include <QObject>
#include <qqmlintegration.h>

class Worker;

class Bridge : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit Bridge(QObject *parent = nullptr);

public Q_SLOTS:
    QString urlToFilename(QUrl url);
    QString urlToLocalFile(QUrl url);
    QUrl localFileToUrl(const QString &localFile);
    QUrl parentUrl(QUrl url);
    QString parentPath(QString path);
    void highlightInFileManager(const QUrl &fileUrl);

Q_SIGNALS:
    void filesSelected(QList<QUrl> urls);
    void clearFiles();
    void run();
    void processFile(uint index, QUrl url);
    void thumbGenerated(uint index, const QString &filePath, const QString &thumbPath);
    void thumbnailProgress(uint index, const QString &filePath, uint progress);

private:
    Worker* m_worker;
};

#endif // BRIDGE_H
