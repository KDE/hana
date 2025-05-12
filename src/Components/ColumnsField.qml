import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
        value: 4
    }
}
