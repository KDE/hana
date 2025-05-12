import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import com.georgefb.rina.components

ToolBar {
    RowLayout {
        anchors.fill: parent

        AddFilesField {
            Layout.alignment: Qt.AlignLeft
        }

        RunField {
            Layout.alignment: Qt.AlignLeft
        }

        Item {
            Layout.fillWidth: true
        }

        CloseAppButton {
            Layout.alignment: Qt.AlignRight
        }
    }
}
