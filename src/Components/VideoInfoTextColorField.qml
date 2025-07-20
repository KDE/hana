import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import org.kde.kirigami as Kirigami

import com.georgefb.hana

RowLayout {
    id: root

    enabled: HanaSettings.showVideoInfo

    Label {
        text: qsTr("Text color")
    }

    Rectangle {
        id: control

        Layout.preferredWidth: Math.ceil(Kirigami.Units.gridUnit * 1.4)
        Layout.preferredHeight: Math.ceil(Kirigami.Units.gridUnit * 1.4)

        color: HanaSettings.videoInfoTextColor
        border.width: 1
        border.color: ma.containsMouse
                      ? Kirigami.Theme.highlightColor
                      : Kirigami.Theme.textColor

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
                HanaSettings.videoInfoTextColor = colorPicker.selectedColor
                HanaSettings.save()
            }
            selectedColor: HanaSettings.videoInfoTextColor
        }
    }
}
