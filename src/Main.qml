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

            GridLayout {
                id: scrollLayout

                columns: 2

                Label {
                    text: qsTr("Thumbnail settings")
                    font.pointSize: 14
                    Layout.columnSpan: 2
                }
                ColumnsField {}
                RowsField {}
                SpacingField {}
                ThumbWidthField {}
                BackgroundField {
                    Layout.columnSpan: 2
                }

                Label {
                    text: qsTr("Video info settings")
                    font.pointSize: 14
                    Layout.columnSpan: 2
                }
                VideoInfoBackgroundColorField {
                    Layout.columnSpan: 2
                }
                VideoInfoTextColorField {
                    Layout.columnSpan: 2
                }
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
