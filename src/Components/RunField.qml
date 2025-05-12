import QtQuick
import QtQuick.Controls

import com.georgefb.rina

ToolButton {
    id: root

    text: qsTr("Run")

    onClicked: Bridge.run()
}
