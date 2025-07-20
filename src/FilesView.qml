pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import com.georgefb.hana
import com.georgefb.hana.components

Rectangle {
    id: root

    property alias filesModel: filesModel

    Layout.fillWidth: true
    Layout.fillHeight: true

    color: Kirigami.Theme.alternateBackgroundColor
    radius: Kirigami.Units.cornerRadius
    border.width: 1
    border.color: Qt.darker(color, 1.5)

    ListModel {
        id: filesModel
    }

    ScrollView {
        anchors.fill: parent

        ListView {
            id: view

            clip: true
            model: filesModel
            delegate: ItemDelegate {
                id: delegate

                required property int index
                required property url fileUrl
                required property string filename
                required property string thumbPath
                required property int progress

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
                            icon.name: "overflow-menu"
                            onClicked: openMenu()

                            function openMenu() {
                                if (menu.opened) {
                                    menu.close()
                                } else {
                                    menu.popup(0, height)
                                }
                            }

                            Menu {
                                id: menu

                                y: parent.height
                                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                                MenuItem {
                                    text: i18nc("@action:inmenu", "Open video folder")
                                    onClicked: openVideoFolder()

                                    function openVideoFolder() {
                                        const videoPath = Bridge.urlToLocalFile(delegate.fileUrl)
                                        const videoParentPath = Bridge.parentPath(videoPath)
                                        Qt.openUrlExternally(`file://${videoParentPath}`)
                                    }
                                }

                                MenuItem {
                                    text: i18nc("@action:inmenu", "Remove")
                                    onClicked: filesModel.remove(delegate.index)
                                }
                            }
                        }
                    }

                    RowLayout {
                        ProgressBar {
                            id: thumbnailProgress

                            Layout.fillWidth: true

                            from: 0
                            to: 100
                            value: delegate.progress
                        }

                        ToolButton {
                            text: i18nc("@action:inmenu", "Open thumbnail")
                            enabled: delegate.thumbPath !== ""
                            onClicked: openThumbnail()

                            function openThumbnail() {
                                Bridge.highlightInFileManager(`file://${delegate.thumbPath}`)
                            }
                        }
                    }
                }
            }

            AddFilesField {
                anchors.fill: parent
                visible: view.count === 0
            }

            Connections {
                target: Bridge
                function onFilesSelected(urls) {
                    urls.forEach(function(u) {
                        for (let i = 0; i < filesModel.count; i++) {
                            const fileUrl = filesModel.get(i).fileUrl
                            if (fileUrl === u) {
                                // file already in the list
                                return
                            }
                        }
                        const modelItem = {
                            filename: Bridge.urlToFilename(u),
                            fileUrl: u,
                            thumbPath: "",
                            progress: 0,
                        }

                        filesModel.append(modelItem)
                    })
                }
                function onClearFiles() {
                    filesModel.clear()
                }
                function onRun() {
                    for (let i = 0; i < filesModel.count; i++) {
                        Bridge.processFile(i, filesModel.get(i).fileUrl)
                    }
                }
                function onThumbGenerated(index, filePath, thumbPath) {
                    let modelItem = filesModel.get(index)
                    if (filePath === Bridge.urlToLocalFile(modelItem.fileUrl)) {
                        modelItem.thumbPath = thumbPath
                    }
                }
                function onThumbnailProgress(index, filePath, progress) {
                    let modelItem = filesModel.get(index)
                    if (filePath === Bridge.urlToLocalFile(modelItem.fileUrl)) {
                        modelItem.progress = progress
                    }
                }
            }
        }
    }
}
