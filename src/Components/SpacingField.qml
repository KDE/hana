import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.hana

ColumnLayout {
    id: root

    property alias spacing: control.value

    Label {
        Layout.maximumWidth: 250

        text: qsTr("Spacing")
        elide: Text.ElideRight
    }

    SpinBox {
        id: control

        from: 0
        to: 9000
        value: HanaSettings.thumbnailsSpacing
        stepSize: 5

        onValueModified: {
            HanaSettings.thumbnailsSpacing = value
            HanaSettings.save()
        }
    }
}
