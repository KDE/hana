import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import org.kde.kirigami as Kirigami

import com.georgefb.rina

RowLayout {
    id: root

    enabled: RinaSettings.showVideoInfo

    Label {
        text: qsTr("Video info text color")
    }

    Rectangle {
        id: control

        Layout.preferredWidth: Math.ceil(Kirigami.Units.gridUnit * 1.7)
        Layout.preferredHeight: Math.ceil(Kirigami.Units.gridUnit * 1.7)

        color: RinaSettings.videoInfoTextColor
        border.width: 2
        border.color: ma.containsMouse
                      ? Kirigami.Theme.highlightColor
                      : color

        MouseArea {
            id: ma

            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                colorPicker.open()
            }
            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
        }

        ToolTip {
            text: qsTr("Select a color")
            visible: ma.containsMouse
        }

        ColorDialog {
            id: colorPicker

            title: qsTr("Select Color")
            onAccepted: {
                RinaSettings.videoInfoTextColor = colorPicker.selectedColor
                RinaSettings.save()
            }
            selectedColor: RinaSettings.videoInfoTextColor
        }
    }
}
