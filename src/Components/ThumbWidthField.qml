import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina

ColumnLayout {
    id: root

    property alias thumbWidth: control.value

    Label {
        text: qsTr("Thumbnail width")
    }

    SpinBox {
        id: control

        from: 0
        to: 9000
        value: RinaSettings.thumbnailsWidth
        stepSize: 10
    }
}
