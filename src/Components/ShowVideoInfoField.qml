import QtQuick
import QtQuick.Controls

import com.georgefb.hana

CheckBox {
    id: root

    text: i18nc("@option:check", "Show video info")
    checked: HanaSettings.showVideoInfo

    onClicked: {
        HanaSettings.showVideoInfo = root.checked
        HanaSettings.save()
    }
}
