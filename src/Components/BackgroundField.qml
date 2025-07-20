import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import org.kde.kirigami as Kirigami

import com.georgefb.hana

RowLayout {
    id: root

    Label {
        text: i18nc("@label", "Background color")
    }

    Rectangle {
        id: control

        Layout.preferredWidth: Math.ceil(Kirigami.Units.gridUnit * 1.4)
        Layout.preferredHeight: Math.ceil(Kirigami.Units.gridUnit * 1.4)

        color: HanaSettings.backgroundColor
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
            text: i18nc("@info:tooltip", "Select a color")
            visible: ma.containsMouse
        }

        ColorDialog {
            id: colorPicker

            title: i18nc("@title:window", "Select Color")
            onAccepted: {
                HanaSettings.backgroundColor = colorPicker.selectedColor
                HanaSettings.save()
            }
            selectedColor: HanaSettings.backgroundColor
        }
    }
}
