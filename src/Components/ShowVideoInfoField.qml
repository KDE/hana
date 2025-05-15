import QtQuick
import QtQuick.Controls

import com.georgefb.rina

CheckBox {
    id: root

    text: qsTr("Show video info")
    checked: RinaSettings.showVideoInfo

    onClicked: {
        RinaSettings.showVideoInfo = root.checked
        RinaSettings.save()
    }
}
