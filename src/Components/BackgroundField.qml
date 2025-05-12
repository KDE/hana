import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import org.kde.kirigami as Kirigami

RowLayout {
    id: root

    property alias color: control.color

    Label {
        text: qsTr("Background color")
    }

    Rectangle {
        id: control

        // using onColorChanged causes a binding loop
        signal colorChosen(string color)

        Layout.preferredWidth: Math.ceil(Kirigami.Units.gridUnit * 1.7)
        Layout.preferredHeight: Math.ceil(Kirigami.Units.gridUnit * 1.7)

        color: "#333"
        border.width: 2
        border.color: ma.containsMouse
                      ? Kirigami.Theme.highlightColor
                      : color

        onColorChosen: {
            root.color = colorPicker.selectedColor
        }

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
            text: qsTr("@info:tooltip", "Select a color")
            visible: ma.containsMouse
        }

        ColorDialog {
            id: colorPicker

            title: qsTr("@title:window", "Select Color")
            onAccepted: control.colorChosen(colorPicker.selectedColor)
            selectedColor: root.color
        }
    }
}
