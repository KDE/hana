#include "bridge.h"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QStandardPaths>
#include <qelapsedtimer.h>

#include "framedecoder.h"
#include "settings.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

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
    QElapsedTimer timer;
    timer.start();

    FrameDecoder frameDecoder(url.toLocalFile());
    if (!frameDecoder.isInitialized()) {
        return;
    }

    auto rows {RinaSettings::self()->thumbnailsRows()};
    auto columns {RinaSettings::self()->thumbnailsColumns()};
    auto thumbWidth {RinaSettings::self()->thumbnailsWidth()};
    auto spacing {RinaSettings::self()->thumbnailsSpacing()};
    auto fileDuration {frameDecoder.getDuration()};

    auto aspectRatio {static_cast<float>(frameDecoder.getWidth())/frameDecoder.getHeight()};
    auto thumbHeight {thumbWidth/aspectRatio};

    auto w {(columns * thumbWidth) + (spacing * columns + spacing)};
    auto h {(rows * thumbHeight) + (spacing * rows + spacing)};
    auto totalThumbs {rows * columns};
    auto startTime {static_cast<float>(fileDuration)/totalThumbs};

    QStringList files;
    QImage image;
    uint seekPosition{0};
    for (int i = 0; i < totalThumbs; ++i) {
        seekPosition = static_cast<uint>(startTime * i);
        if (i == 0) {
            seekPosition = static_cast<uint>(startTime/4);
        }
        if (seekPosition > fileDuration) {
            seekPosition = fileDuration - 100;
        }

        frameDecoder.seek(seekPosition);
        if (!frameDecoder.decodeVideoFrame()) {
            qDebug() << "Failed to decode frame:" << url.toLocalFile() << " thumbnail:" << i;
            return;
        }
        frameDecoder.getScaledVideoFrame(thumbWidth, true, image);

        auto thumbPath {u"%1/img-%2.png"_s.arg(thumbSaveLocation()).arg(i)};
        files.append(thumbPath);
        image.save(thumbPath);
    }

    uint x{0};
    QImage thumbsImage ({w, static_cast<int>(h)}, QImage::Format_RGB32);
    QImage thumbImg;
    thumbsImage.fill(QColor::fromString(RinaSettings::self()->backgroundColor()));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            auto left {(j * thumbWidth) + ((j + 1) * spacing)};
            auto top {(i * thumbHeight) + ((i + 1) * spacing)};
            thumbImg.load(files.at(x));

            QPainter p(&thumbsImage);
            p.drawImage(QRect{left, static_cast<int>(top), thumbImg.width(), thumbImg.height()}, thumbImg);

            QFile::moveToTrash(files.at(x));

            x++;
        }
    }
    auto thumbsImagePath {u"%1/%2.thumbs.png"_s.arg(thumbSaveLocation()).arg(url.fileName())};
    thumbsImage.save(thumbsImagePath);
    Q_EMIT thumbGenerated(thumbsImagePath);
    qDebug() << timer.elapsed() << "ms elapsed;" << timer.nsecsElapsed() << "ns elapsed:";
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
    auto picturesFolder{QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)};
    auto saveFolder{u"%1/rina"_s.arg(picturesFolder)};
    QDir dir;
    dir.mkpath(saveFolder);
    return saveFolder;
}
