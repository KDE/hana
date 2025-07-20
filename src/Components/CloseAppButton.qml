import QtQuick
import QtQuick.Controls

ToolButton {
    id: root

    text: i18nc("@action:button", "Close")

    onClicked: Qt.quit()
}
