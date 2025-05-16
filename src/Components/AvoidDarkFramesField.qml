import QtQuick
import QtQuick.Controls

import com.georgefb.rina

CheckBox {
    id: root

    text: qsTr("Avoid dark frames")
    checked: RinaSettings.avoidDarkFrames

    onClicked: {
        RinaSettings.avoidDarkFrames = root.checked
        RinaSettings.save()
    }

    ToolTip {
        text: qsTr("Checked: tries to avoid very dark frames by changing the time the thumbnail is taken<br>"+
                   "<b>Can be very slow</b><br><br>"+
                   "Unchecked: thumbnail is taken taken regardless of it's content")
    }
}
