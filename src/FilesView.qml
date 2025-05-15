import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import com.georgefb.rina
import com.georgefb.rina.components

Rectangle {
    id: root

    property ListModel filesModel: ListModel {
        id: filesModel
    }
    Layout.fillWidth: true
    Layout.fillHeight: true

    color: Kirigami.Theme.alternateBackgroundColor
    radius: Kirigami.Units.cornerRadius
    border.width: 1
    border.color: Qt.darker(color, 1.5)

    ScrollView {
        anchors.fill: parent

        ListView {
            id: view

            clip: true
            model: filesModel
            delegate: ItemDelegate {
                id: delegate

                required property url fileUrl
                required property string filename
                property string thumbPath: ""

                width: ListView.view.width
                hoverEnabled: true
                contentItem: ColumnLayout {
                    spacing: 0

                    RowLayout {
                        Label {
                            Layout.fillWidth: true

                            text: delegate.filename
                            elide: Text.ElideRight

                            ToolTip.text: text
                            ToolTip.visible: truncated && delegate.hovered
                            ToolTip.delay: Kirigami.Units.shortDuration
                        }

                        ToolButton {
                            text: qsTr("Open folder")
                            onClicked: openFolder()

                            function openFolder() {
                                const filePath = Bridge.urlToLocalFile(delegate.fileUrl)
                                const fileParentPath = Bridge.parentPath(filePath)
                                Qt.openUrlExternally(`file://${fileParentPath}`)
                            }
                        }
                    }

                    RowLayout {
                        ProgressBar {
                            id: thumbnailProgress

                            Layout.fillWidth: true

                            from: 0
                            to: 100

                            Connections {
                                target: Bridge
                                function onThumbnailProgress(filePath, progress) {
                                    if (filePath === Bridge.urlToLocalFile(delegate.fileUrl)) {
                                        thumbnailProgress.value = progress
                                    }
                                }
                                function onThumbGenerated(filePath, thumbPath) {
                                    if (filePath === Bridge.urlToLocalFile(delegate.fileUrl)) {
                                        delegate.thumbPath = thumbPath
                                    }
                                }
                            }
                        }

                        ToolButton {
                            text: qsTr("Open thumbnail")
                            enabled: delegate.thumbPath !== ""
                            onClicked: openThumbnail()

                            function openThumbnail() {
                                Qt.openUrlExternally(`file://${delegate.thumbPath}`)
                            }
                        }
                    }
                }
            }

            AddFilesField {
                anchors.centerIn: parent
                visible: view.count === 0
            }

            Connections {
                target: Bridge
                function onFilesSelected(urls) {
                    urls.forEach((u) => filesModel.append({filename: Bridge.urlToFilename(u), fileUrl: u}))
                }
                function onClearFiles() {
                    filesModel.clear()
                }
                function onRun() {
                    for (let i = 0; i < filesModel.count; i++) {
                        Bridge.processFile(filesModel.get(i).fileUrl)
                    }
                }
            }
        }
    }
}
