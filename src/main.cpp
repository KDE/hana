#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>

int main(int argc, char *argv[])
{
    Q_IMPORT_QML_PLUGIN(com_georgefb_rina_componentsPlugin);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("rina", "Main");

    return app.exec();
}
