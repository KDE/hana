import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina

ColumnLayout {
    id: root

    property alias spacing: control.value

    Label {
        text: qsTr("Spacing")
    }

    SpinBox {
        id: control

        from: 0
        to: 9000
        value: RinaSettings.thumbnailsSpacing
        stepSize: 5
    }
}
