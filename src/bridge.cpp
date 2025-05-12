#include "bridge.h"


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
    qDebug() << url;
}
