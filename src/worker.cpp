/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "worker.h"

#include <QPainter>
#include <QTemporaryDir>
#include <QTextDocument>
#include <QThreadPool>
#include <QElapsedTimer>

#include "settings.h"

using namespace Qt::StringLiterals;

QString formatBytes(qint64 bytes) {
    QStringList sizes = { u"B"_s, u"KB"_s, u"MB"_s, u"GB"_s, u"TB"_s, u"PB"_s };
    double len = static_cast<double>(bytes);
    uint order = 0;

    while (len >= 1024.0 && order < sizes.count()) {
        order++;
        len = len / 1024.0;
    }

    return QString(u"%1 %2"_s).arg(QString::number(len, 'f', 1), sizes[order]);
}

QString formatDuration(const double time)
{
    int totalNumberOfSeconds = static_cast<int>(time);
    int seconds = totalNumberOfSeconds % 60;
    int minutes = (totalNumberOfSeconds / 60) % 60;
    int hours = (totalNumberOfSeconds / 60 / 60);

    QString hoursString = u"%1"_s.arg(hours, 2, 10, QLatin1Char('0'));
    QString minutesString = u"%1"_s.arg(minutes, 2, 10, QLatin1Char('0'));
    QString secondsString = u"%1"_s.arg(seconds, 2, 10, QLatin1Char('0'));
    QString timeString = u"%1:%2:%3"_s.arg(hoursString, minutesString, secondsString);
    if (hours == 0 && !HanaSettings::videoInfoAlwaysShowHours()) {
        timeString = u"%1:%2"_s.arg(minutesString, secondsString);
    }

    return timeString;
}

double calculateBrightness(const QImage &image)
{
    if (image.isNull()) {
        qDebug() << "Failed to load image.";
        return -1;
    }

    // Convert to format that allows pixel access (grayscale optional)
    QImage converted = image.convertToFormat(QImage::Format_RGB32);

    qint64 totalBrightness {0};
    int width = converted.width();
    int height = converted.height();

    for (int y = 0; y < height; ++y) {
        const QRgb* line = reinterpret_cast<const QRgb*>(converted.scanLine(y));
        for (int x = 0; x < width; ++x) {
            QRgb pixel = line[x];

            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            // Use luminance formula
            int brightness = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
            totalBrightness += brightness;
        }
    }
    return static_cast<double>(totalBrightness) / (width * height);
}

Worker::Worker(Bridge *bridge)
    : m_bridge(bridge)
{

}

void Worker::process(uint index, QUrl url)
{
    auto runnable = new ThumbnailerRunnable(index, url);
    connect(runnable, &ThumbnailerRunnable::done, m_bridge, &Bridge::thumbGenerated, Qt::QueuedConnection);
    connect(runnable, &ThumbnailerRunnable::thumbnailProgress, m_bridge, &Bridge::thumbnailProgress, Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(runnable);
}


ThumbnailerRunnable::ThumbnailerRunnable(uint index, QUrl url)
    : m_index(index)
    , m_url(url)
    , m_frameDecoder(m_url.toLocalFile())
{
}

void ThumbnailerRunnable::run()
{
    QElapsedTimer timer;
    timer.start();

    if (!m_frameDecoder.isInitialized()) {
        return;
    }
    // before seeking, a frame has to be decoded
    if (!m_frameDecoder.decodeVideoFrame()) {
        return;
    }

    uint rows {static_cast<uint>(HanaSettings::self()->thumbnailsRows())};
    uint columns {static_cast<uint>(HanaSettings::self()->thumbnailsColumns())};
    uint thumbWidth {static_cast<uint>(HanaSettings::self()->thumbnailsWidth())};
    uint spacing {static_cast<uint>(HanaSettings::self()->thumbnailsSpacing())};
    uint fileDuration {m_frameDecoder.getDuration()};

    auto size = m_frameDecoder.calculateDimensions(thumbWidth, true);
    thumbWidth = size.width();
    int thumbHeight {size.height()};

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

        m_frameDecoder.seek(seekPosition);
        m_frameDecoder.getScaledVideoFrame(thumbWidth, true, image);

        if (HanaSettings::avoidDarkFrames()) {
            double brightness = calculateBrightness(image);
            uint newSeekPosition {seekPosition};
            uint brightestPosition {seekPosition+1};
            double maxBrightness {brightness};
            while (brightness < 30 && newSeekPosition < seekPosition + (startTime/2)) {
                m_frameDecoder.seek(newSeekPosition);
                m_frameDecoder.getScaledVideoFrame(thumbWidth, true, image);
                brightness = calculateBrightness(image);
                brightestPosition = maxBrightness >= brightness ? brightestPosition : newSeekPosition;
                maxBrightness = std::max(brightness, maxBrightness);
                ++newSeekPosition;
            }
            if (brightness < 30) {
                m_frameDecoder.seek(brightestPosition);
                m_frameDecoder.getScaledVideoFrame(thumbWidth, true, image);
            }
        }

        auto thumbPath {u"%1/img-%2.png"_s.arg(tmpDir.path()).arg(i)};
        files.append(thumbPath);
        image.save(thumbPath);

        prevSeekPosition = seekPosition;
        Q_EMIT thumbnailProgress(m_index, m_url.isLocalFile() ? m_url.toLocalFile() : QString{}, i * 100 / (totalThumbs + 1));
    }

    uint x {0};
    QImage thumbsImage ({static_cast<int>(w), static_cast<int>(h)}, QImage::Format_RGB32);
    QPainter p(&thumbsImage);
    QImage thumbImg;
    thumbsImage.fill(QColor::fromString(HanaSettings::self()->backgroundColor()));
    for (uint i {0}; i < rows; ++i) {
        for (uint j {0}; j < columns; ++j) {
            uint left {(j * thumbWidth) + ((j + 1) * spacing)};
            uint top {(i * thumbHeight) + ((i + 1) * spacing)};
            thumbImg.load(files.at(x));

            p.drawImage(QRect{static_cast<int>(left), static_cast<int>(top), thumbImg.width(), thumbImg.height()},
                        thumbImg);

            QFile::moveToTrash(files.at(x));

            x++;
        }
    }

    const auto savePath = thumbnailsImageSaveLocation();
    QDir dir;
    dir.mkpath(savePath);

    auto thumbsImagePath {u"%1/%2.thumbs.png"_s.arg(savePath, m_url.fileName())};
    if (HanaSettings::showVideoInfo()) {
        QImage textImage {videoFileInfoImage(w)};
        QImage thumbsImageWithText(thumbsImage.width(), thumbsImage.height() + textImage.height(), QImage::Format_RGB32);
        thumbsImageWithText.fill(QColor::fromString(HanaSettings::self()->backgroundColor()));
        QPainter thumbsImageWithTextPainter(&thumbsImageWithText);
        thumbsImageWithTextPainter.drawImage(QRect{0, 0, textImage.width(), textImage.height()}, textImage);
        thumbsImageWithTextPainter.drawImage(QRect{0, textImage.height(), thumbsImage.width(), thumbsImage.height()}, thumbsImage);
        thumbsImageWithText.save(thumbsImagePath);
    } else {
        thumbsImage.save(thumbsImagePath);
    }

    Q_EMIT done(m_index, m_url.isLocalFile() ? m_url.toLocalFile() : QString{}, thumbsImagePath);
    Q_EMIT thumbnailProgress(m_index, m_url.isLocalFile() ? m_url.toLocalFile() : QString{}, 100);
    qDebug() << "Finished" << thumbsImagePath << "in" << timer.elapsed() << "miliseconds";
}

QImage ThumbnailerRunnable::videoFileInfoImage(uint width)
{
    QFileInfo fi(m_url.toLocalFile());
    QFont font;
    font.setPointSize(20);

    QString html;
    html.append(u"<div style='color: %1'>"_s.arg(HanaSettings::videoInfoTextColor()));
    html.append(u"<div><strong>%1</strong></div>"_s.arg(m_url.fileName()));
    html.append(u"<div><strong>Size</strong>  %1</div>"_s.arg(formatBytes(fi.size())));
    html.append(u"<div><strong>Resolution</strong> %1x%2</div>"_s.arg(m_frameDecoder.getWidth()).arg(m_frameDecoder.getHeight()));
    html.append(u"<div><strong>Codec</strong> %1</div>"_s.arg(m_frameDecoder.getCodec()));
    html.append(u"<div><strong>Length</strong> %1</div>"_s.arg(formatDuration(m_frameDecoder.getDuration())));
    html.append(u"</div>"_s);

    uint docPadding{20};
    QTextDocument td;
    td.setDefaultFont(font);
    td.setHtml(html);
    td.setTextWidth(width - docPadding * 2);
    td.setDocumentMargin(docPadding);

    QImage textImage(QSize{static_cast<int>(width), static_cast<int>(td.size().height())}, QImage::Format_RGB32);
    textImage.fill(HanaSettings::videoInfoBackgroundColor());
    QPainter textPainter(&textImage);
    td.drawContents(&textPainter, QRectF{0, 0, td.textWidth(), td.size().height()});

    return textImage;
}

QString ThumbnailerRunnable::thumbnailsImageSaveLocation()
{
    if (HanaSettings::saveLocation() == u"SameAsVideo"_s) {
        QFileInfo fi{m_url.toLocalFile()};
        auto parentFolder = fi.absolutePath();
        return parentFolder;

    } else if (HanaSettings::saveLocation() == u"NextToVideo"_s) {
        QFileInfo fi{m_url.toLocalFile()};
        auto parentFolder = fi.absolutePath();
        parentFolder.append(u"/%1"_s.arg(HanaSettings::saveLocationFolderName()));
        return parentFolder;

    } else if (HanaSettings::saveLocation() == u"Custom"_s) {
        return QUrl(HanaSettings::saveLocationFolderUrl()).toLocalFile();

    } else {
        return {};
    }

    return {};
}
