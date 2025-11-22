/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlExtensionPlugin>

#include <KAboutData>
#include <KLocalizedQmlContext>
#include <KLocalizedString>

#include "hana-version.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon::fromTheme(u"hana"_s));

    QQmlApplicationEngine engine;
    KLocalizedString::setApplicationDomain("hana");
    KLocalization::setupLocalizedContext(&engine);

    KAboutData aboutData;
    aboutData.setComponentName(u"hana"_s);
    aboutData.setDisplayName(u"Hana"_s);
    aboutData.setVersion(HANA_VERSION_STRING);
    aboutData.setShortDescription(i18nc("@title", "Video thumbnails generator"));
    aboutData.setLicense(KAboutLicense::GPL_V3);
    aboutData.setCopyrightStatement(i18nc("copyright statement, use copyright symbol and en dash for the year range", "© 2025"));
    aboutData.setHomepage(u"https://invent.kde.org/multimedia/hana"_s);
    aboutData.setBugAddress("https://bugs.kde.org/enter_bug.cgi?product=Hana");
    aboutData.setDesktopFileName(u"org.kde.hana"_s);

    aboutData.addAuthor(i18nc("@info:credit", "George Florea Bănuș"),
                          i18nc("@info:credit", "Developer"),
                          u"georgefb899@gmail.com"_s,
                          u"https://georgefb.com"_s);

    KAboutData::setApplicationData(aboutData);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("org.kde.hana", "Main");

    return app.exec();
}
