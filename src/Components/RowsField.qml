import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina

ColumnLayout {
    id: root

    property alias rows: control.value

    Label {
        text: qsTr("Rows")
    }

    SpinBox {
        id: control

        from: 1
        to: 9000
        value: RinaSettings.thumbnailsRows

        onValueModified: {
            RinaSettings.thumbnailsRows = value
            RinaSettings.save()
        }
    }
}
