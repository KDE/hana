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

            model: filesModel
            delegate: ItemDelegate {
                id: delegate

                required property url fileUrl
                required property string filename

                width: ListView.view.width
                hoverEnabled: true
                contentItem: Label {
                    text: delegate.filename
                    elide: Text.ElideRight

                    ToolTip.text: text
                    ToolTip.visible: truncated && delegate.hovered
                    ToolTip.delay: Kirigami.Units.shortDuration
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
