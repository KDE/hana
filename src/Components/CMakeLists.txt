#
# SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#

qt_add_library(components STATIC)
qt_add_qml_module(components
    URI org.kde.hana.components
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/com/georgefb/hana/components
    QML_FILES
        AddFilesField.qml
        BackgroundField.qml
        ClearFilesField.qml
        CloseAppButton.qml
        ColumnsField.qml
        RowsField.qml
        RunField.qml
        SpacingField.qml
        ThumbWidthField.qml
        VideoInfoBackgroundColorField.qml
        VideoInfoTextColorField.qml
        ShowVideoInfoField.qml
        VideoInfoAlwaysShowHoursField.qml
        AvoidDarkFramesField.qml
        ImageSaveLocationField.qml
)
