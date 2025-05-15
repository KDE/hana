import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina.components

ApplicationWindow {
    width: 800
    minimumWidth: 600
    height: 450
    minimumHeight: 300
    visible: true
    title: qsTr("Hello World")

    footer: Footer {}

    RowLayout {
        anchors.fill: parent

        ScrollView {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            Layout.margins: 20

            ColumnLayout {
                id: scrollLayout

                spacing: 15

                Label {
                    text: qsTr("Thumbnail settings")
                    font.pointSize: 14
                }

                RowLayout {
                    ColumnsField {}
                    RowsField {}
                }
                RowLayout {
                    SpacingField {}
                    ThumbWidthField {}
                }
                BackgroundField {}
            }

        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20

            FilesView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
