#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QRunnable>
#include <QUrl>

#include "bridge.h"
#include "framedecoder.h"

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(Bridge *bridge);
    ~Worker() = default;

public Q_SLOTS:
    void process(uint index, QUrl url);

Q_SIGNALS:
    void finished();
    void error(QString err);
    void thumbGenerated(uint index, const QString &filePath, const QString &thumbPath);
    void thumbnailProgress(uint index, const QString &filePath, uint progress);

private:
    Bridge *m_bridge;
};


class ThumbnailerRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ThumbnailerRunnable(uint index, QUrl url, const QString &saveFolder);
    void run() override;

Q_SIGNALS:
    void done(uint index, const QString &filePath, const QString &thumbPath);
    void thumbnailProgress(uint index, const QString &filePath, uint progress);

private:
    QImage videoFileInfoImage(uint width);
    uint m_index;
    QUrl m_url;
    QString m_saveFolder;
    FrameDecoder m_frameDecoder;
};

#endif // WORKER_H
