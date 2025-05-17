import QtQuick
import QtQuick.Controls

import com.georgefb.rina

CheckBox {
    id: root

    text: qsTr("Always show hours")
    checked: RinaSettings.videoInfoAlwaysShowHours
    enabled: RinaSettings.showVideoInfo

    onClicked: {
        RinaSettings.videoInfoAlwaysShowHours = root.checked
        RinaSettings.save()
    }

    ToolTip {
        text: qsTr("Checked: shows hours as “00“ (two zeros) when video length is under an hour\n\n"+
                   "Unchecked: only minutes and seconds are shown")
    }
}
