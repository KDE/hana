import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
        value: 20
        stepSize: 10
    }
}
