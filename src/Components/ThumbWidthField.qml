import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.hana

ColumnLayout {
    id: root

    property alias thumbWidth: control.value

    Label {
        Layout.maximumWidth: 250

        text: i18nc("@label", "Thumbnail width")
        elide: Text.ElideRight
    }

    SpinBox {
        id: control

        from: 100
        to: 9000
        value: HanaSettings.thumbnailsWidth
        stepSize: 10

        onValueModified: {
            HanaSettings.thumbnailsWidth = value
            HanaSettings.save()
        }
    }
}
