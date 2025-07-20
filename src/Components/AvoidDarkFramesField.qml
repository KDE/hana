import QtQuick
import QtQuick.Controls

import com.georgefb.hana

CheckBox {
    id: root

    text: qsTr("Avoid dark frames")
    checked: HanaSettings.avoidDarkFrames

    onClicked: {
        HanaSettings.avoidDarkFrames = root.checked
        HanaSettings.save()
    }

    ToolTip {
        text: qsTr("Checked: tries to avoid very dark frames by changing the time the thumbnail is taken<br>"+
                   "<b>Can be very slow</b><br><br>"+
                   "Unchecked: thumbnail is taken taken regardless of it's content")
    }
}
