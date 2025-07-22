/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "bridge.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QPainter>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTextDocument>
#include <QThread>

#include <KIO/OpenFileManagerWindowJob>
#include <KIO/OpenUrlJob>

#include "worker.h"

using namespace Qt::StringLiterals;

Bridge::Bridge(QObject *parent)
    : QObject{parent}
{
    QThread* thread = new QThread();
    m_worker = new Worker(this);
    m_worker->moveToThread(thread);
    connect(m_worker, &Worker::finished, thread, &QThread::quit);
    connect(m_worker, &Worker::finished, m_worker, &Worker::deleteLater);
    connect(m_worker, &Worker::thumbGenerated, this, &Bridge::thumbGenerated, Qt::QueuedConnection);
    connect(m_worker, &Worker::thumbnailProgress, this, &Bridge::thumbnailProgress, Qt::QueuedConnection);
    connect(this, &Bridge::processFile, m_worker, &Worker::process, Qt::QueuedConnection);
    thread->start();
}

QString Bridge::urlToFilename(QUrl url)
{
    return url.fileName();
}

QString Bridge::urlToLocalFile(QUrl url)
{

    return url.isLocalFile() ? url.toLocalFile() : QString{};
}

QUrl Bridge::localFileToUrl(const QString &localFile)
{
    return QUrl::fromLocalFile(localFile);
}

QUrl Bridge::parentUrl(QUrl url)
{
    const auto localFile = urlToLocalFile(url);
    const auto parent = parentPath(localFile);
    return QUrl::fromLocalFile(parent);
}

QString Bridge::parentPath(QString path)
{
    QFileInfo fi(path);
    return fi.exists() ? fi.absolutePath() : QString{};
}

void Bridge::highlightInFileManager(const QUrl &fileUrl)
{
    KIO::highlightInFileManager({fileUrl});
}

void Bridge::openUrl(const QUrl &fileUrl)
{
    auto *job = new KIO::OpenUrlJob({fileUrl});
    job->start();
}

QList<QUrl> Bridge::urlsFromCmdLineArgs()
{
    QCommandLineParser clParser;
    clParser.addHelpOption();
    clParser.addVersionOption();
    clParser.process(*qApp);

    QFileInfo fi;
    QList<QUrl> urls;
    const auto posArgs{clParser.positionalArguments()};

    for (const auto &posArg : posArgs) {
        fi.setFile(posArg);
        if (fi.exists()) {
            urls.append(QUrl::fromLocalFile(fi.absoluteFilePath()));
        } else {
            fi.setFile(QDir::current(), posArg);
            if (fi.exists()) {
                urls.append(QUrl::fromLocalFile(fi.absoluteFilePath()));
            }
        }
    }

    return urls;
}
