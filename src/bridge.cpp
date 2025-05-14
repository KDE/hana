#include "bridge.h"

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "framedecoder.h"
#include "settings.h"

using namespace Qt::StringLiterals;

Bridge::Bridge(QObject *parent)
    : QObject{parent}
{

}

QString Bridge::urlToFilename(QUrl url)
{
    return url.fileName();
}

void Bridge::processFile(QUrl url)
{
    auto runnable = new ThumbnailerRunnable(url, thumbSaveLocation());
    connect(runnable, &ThumbnailerRunnable::done, this, &Bridge::thumbGenerated);
    m_pool.start(runnable);
}

QString Bridge::urlToLocalFile(QUrl url)
{

    return url.isLocalFile() ? url.toLocalFile() : QString{};
}

QString Bridge::parentPath(QString path)
{
    QFileInfo fi(path);
    return fi.exists() ? fi.absolutePath() : QString{};
}

QString Bridge::thumbSaveLocation()
{
    auto picturesFolder {QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)};
    auto saveFolder {u"%1/rina"_s.arg(picturesFolder)};
    QDir dir;
    dir.mkpath(saveFolder);
    return saveFolder;
}

ThumbnailerRunnable::ThumbnailerRunnable(QUrl url, const QString &saveFolder)
    : m_url(url)
    , m_saveFolder{saveFolder}
{
}

void ThumbnailerRunnable::run()
{
    QElapsedTimer timer;
    timer.start();

    FrameDecoder frameDecoder(m_url.toLocalFile());
    if (!frameDecoder.isInitialized()) {
        return;
    }
    // before seeking, a frame has to be decoded
    if (!frameDecoder.decodeVideoFrame()) {
        return;
    }

    uint rows {static_cast<uint>(RinaSettings::self()->thumbnailsRows())};
    uint columns {static_cast<uint>(RinaSettings::self()->thumbnailsColumns())};
    uint thumbWidth {static_cast<uint>(RinaSettings::self()->thumbnailsWidth())};
    uint spacing {static_cast<uint>(RinaSettings::self()->thumbnailsSpacing())};
    uint fileDuration {frameDecoder.getDuration()};

    float aspectRatio {static_cast<float>(frameDecoder.getWidth())/frameDecoder.getHeight()};
    uint thumbHeight {static_cast<uint>(thumbWidth/aspectRatio)};

    uint w {(columns * thumbWidth) + (spacing * columns + spacing)};
    uint h {(rows * thumbHeight) + (spacing * rows + spacing)};
    uint totalThumbs {rows * columns};
    float startTime {static_cast<float>(fileDuration)/totalThumbs};

    QStringList files;
    QImage image;
    uint seekPosition {0};

    QTemporaryDir tmpDir;
    if (!tmpDir.isValid()) {
        return;
    }

    uint prevSeekPosition {0};
    for (uint i {0}; i < totalThumbs; ++i) {
        seekPosition = static_cast<uint>(startTime * i);
        if (i == 0) {
            seekPosition = static_cast<uint>(startTime / 2);
        }
        if (seekPosition > fileDuration) {
            // get thumbnail between prevSeekPosition and fileDuration
            seekPosition = prevSeekPosition + ((fileDuration - prevSeekPosition) / 2);
        }

        frameDecoder.seek(seekPosition);
        frameDecoder.getScaledVideoFrame(thumbWidth, true, image);

        auto thumbPath {u"%1/img-%2.png"_s.arg(tmpDir.path()).arg(i)};
        files.append(thumbPath);
        image.save(thumbPath);

        prevSeekPosition = seekPosition;
    }

    uint x {0};
    QImage thumbsImage ({static_cast<int>(w), static_cast<int>(h)}, QImage::Format_RGB32);
    QImage thumbImg;
    thumbsImage.fill(QColor::fromString(RinaSettings::self()->backgroundColor()));
    for (uint i {0}; i < rows; ++i) {
        for (uint j {0}; j < columns; ++j) {
            uint left {(j * thumbWidth) + ((j + 1) * spacing)};
            uint top {(i * thumbHeight) + ((i + 1) * spacing)};
            thumbImg.load(files.at(x));

            QPainter p(&thumbsImage);
            p.drawImage(QRect{static_cast<int>(left), static_cast<int>(top), thumbImg.width(), thumbImg.height()},
                        thumbImg);

            QFile::moveToTrash(files.at(x));

            x++;
        }
    }
    auto thumbsImagePath {u"%1/%2.thumbs.png"_s.arg(m_saveFolder).arg(m_url.fileName())};
    thumbsImage.save(thumbsImagePath);
    Q_EMIT done(thumbsImagePath);
    qDebug() << "Finished" << thumbsImagePath << "in" << timer.elapsed() << "miliseconds";
}
