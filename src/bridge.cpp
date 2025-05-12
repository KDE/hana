#include "bridge.h"


Bridge::Bridge(QObject *parent)
    : QObject{parent}
{

}

QStringList Bridge::urlsToFilenames(QList<QUrl> urls)
{
    QStringList filenames;
    for (const auto &url : urls) {
        filenames.append(url.fileName());
    }

    return filenames;
}
