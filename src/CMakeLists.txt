qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apphana
    main.cpp
)

qt_policy(SET QTP0001 NEW)
qt_add_qml_module(apphana
    URI com.georgefb.hana
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES Footer.qml
        QML_FILES Components/ColumnsField.qml
        QML_FILES FilesView.qml
    SOURCES
        bridge.h bridge.cpp
        framedecoder.h framedecoder.cpp
        worker.h worker.cpp
)

kconfig_target_kcfg_file(apphana
    FILE settings.kcfg
    CLASS_NAME HanaSettings

    MUTATORS
    DEFAULT_VALUE_GETTERS
    GENERATE_PROPERTIES
    GENERATE_MOC
    PARENT_IN_CONSTRUCTOR
    SINGLETON
    QML_REGISTRATION
)

add_subdirectory(Components)

target_include_directories(apphana
    PRIVATE
        ${FFMPEG_INCLUDE_DIRS}
)
target_link_libraries(apphana
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Widgets

        KF6::ConfigCore
        KF6::ConfigGui
        KF6::I18n
        KF6::I18nQml
        KF6::KIOWidgets
        KF6::Kirigami

        ${AVUTIL_LIBRARIES}
        ${AVFILTER_LIBRARIES}
        ${AVFORMAT_LIBRARIES}
        ${AVCODEC_LIBRARIES}
        ${SWSCALE_LIBRARIES}
)

install(TARGETS apphana ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
