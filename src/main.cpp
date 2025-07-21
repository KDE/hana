/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlExtensionPlugin>

#include <KLocalizedQmlContext>
#include <KLocalizedString>

int main(int argc, char *argv[])
{
    Q_IMPORT_QML_PLUGIN(com_georgefb_hana_componentsPlugin);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    KLocalizedString::setApplicationDomain("hana");
    KLocalization::setupLocalizedContext(&engine);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("org.kde.hana", "Main");

    return app.exec();
}
