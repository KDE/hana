import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    property alias rows: control.value

    Label {
        text: qsTr("Rows")
    }

    SpinBox {
        id: control

        from: 0
        to: 9000
        value: 4
    }
}
