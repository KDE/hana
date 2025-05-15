import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina

ColumnLayout {
    id: root

    property alias columns: control.value

    Label {
        Layout.maximumWidth: 250

        text: qsTr("Columns")
        elide: Text.ElideRight
    }

    SpinBox {
        id: control

        from: 1
        to: 9000
        value: RinaSettings.thumbnailsColumns

        onValueModified: {
            RinaSettings.thumbnailsColumns = value
            RinaSettings.save()
        }
    }
}
