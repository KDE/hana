import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina

ColumnLayout {
    id: root

    property alias columns: control.value

    Label {
        text: qsTr("Columns")
    }

    SpinBox {
        id: control

        from: 0
        to: 9000
        value: RinaSettings.thumbnailsColumns

        onValueModified: {
            RinaSettings.thumbnailsColumns = value
            RinaSettings.save()
        }
    }
}
