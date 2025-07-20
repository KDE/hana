import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.hana

ColumnLayout {
    id: root

    property alias rows: control.value

    Label {
        Layout.maximumWidth: 250

        text: i18nc("@label", "Rows")
        elide: Text.ElideRight
    }

    SpinBox {
        id: control

        from: 1
        to: 9000
        value: HanaSettings.thumbnailsRows

        onValueModified: {
            HanaSettings.thumbnailsRows = value
            HanaSettings.save()
        }
    }
}
