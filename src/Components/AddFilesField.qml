import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import com.georgefb.rina

ToolButton {
    id: root

    text: qsTr("Add files")

    onClicked: filesDialog.open()

    FileDialog {
        id: filesDialog

        fileMode: FileDialog.OpenFiles
        currentFolder: StandardPaths.writableLocation(StandardPaths.MoviesLocation)
        title: qsTr("@title:window", "Select files")

        onAccepted: {
            Bridge.filesSelected(selectedFiles)
        }
    }
}
