
qt_add_library(components STATIC)
qt_add_qml_module(components
    URI com.georgefb.rina.components
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/com/georgefb/rina/components
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
)
