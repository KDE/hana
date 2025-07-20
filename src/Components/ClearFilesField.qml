import QtQuick
import QtQuick.Controls

import com.georgefb.hana

ToolButton {
    id: root

    text: qsTr("Clear files")

    onClicked: Bridge.clearFiles()
}
