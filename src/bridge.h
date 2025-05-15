#ifndef BRIDGE_H
#define BRIDGE_H

#include "framedecoder.h"
#include <QImage>
#include <QObject>
#include <QQmlEngine>
#include <QRunnable>
#include <QThreadPool>

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
    QUrl parentUrl(QUrl url);
    QString parentPath(QString path);

Q_SIGNALS:
    void filesSelected(QList<QUrl> urls);
    void clearFiles();
    void run();
    void thumbGenerated(const QString &filePath, const QString &thumbPath);
    void thumbnailProgress(const QString &filePath, uint progress);

private:
    QString thumbSaveLocation();
    QThreadPool m_pool;
};

class ThumbnailerRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ThumbnailerRunnable(QUrl url, const QString &saveFolder);
    void run() override;

Q_SIGNALS:
    void done(const QString &filePath, const QString &thumbPath);
    void thumbnailProgress(const QString &filePath, uint progress);

private:
    QImage videoFileInfoImage(uint width);
    QUrl m_url;
    QString m_saveFolder;
    FrameDecoder m_frameDecoder;
};
#endif // BRIDGE_H
