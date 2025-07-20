import QtQuick
import QtQuick.Controls

import com.georgefb.hana

CheckBox {
    id: root

    text: qsTr("Always show hours")
    checked: HanaSettings.videoInfoAlwaysShowHours
    enabled: HanaSettings.showVideoInfo

    onClicked: {
        HanaSettings.videoInfoAlwaysShowHours = root.checked
        HanaSettings.save()
    }

    ToolTip {
        text: qsTr("Checked: shows hours as “00“ (two zeros) when video length is under an hour\n\n"+
                   "Unchecked: only minutes and seconds are shown")
    }
}
