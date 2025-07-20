qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apphana
    main.cpp
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

add_subdirectory(Components)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apphana PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apphana
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

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
        KF6::KIOWidgets
        KF6::Kirigami

        ${AVUTIL_LIBRARIES}
        ${AVFILTER_LIBRARIES}
        ${AVFORMAT_LIBRARIES}
        ${AVCODEC_LIBRARIES}
        ${SWSCALE_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS apphana
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
