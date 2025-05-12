import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina.components

ApplicationWindow {
    width: 640
    minimumWidth: 400
    height: 480
    minimumHeight: 400
    visible: true
    title: qsTr("Hello World")

    footer: Footer {}

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.margins: 20

            ColumnsField {}
            RowsField {}
            SpacingField {}
            ThumbWidthField {}
        }

        BackgroundField {
            Layout.alignment: Qt.AlignCenter
        }

        FilesView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20

            model: 100
            delegate: ItemDelegate {
                required property string modelData

                width: ListView.view.width
                text: modelData
            }
        }
    }
}
