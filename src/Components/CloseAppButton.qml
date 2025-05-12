import QtQuick
import QtQuick.Controls

ToolButton {
    id: root
    text: qsTr("Close")

    onClicked: Qt.quit()
}
