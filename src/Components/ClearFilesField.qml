import QtQuick
import QtQuick.Controls

import com.georgefb.hana

ToolButton {
    id: root

    text: i18nc("@action:button", "Clear files")

    onClicked: Bridge.clearFiles()
}
