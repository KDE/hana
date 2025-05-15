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
        currentFolder: RinaSettings.selectVideosUrl
        title: qsTr("@title:window", "Select files")

        onAccepted: {
            Bridge.filesSelected(selectedFiles)
            RinaSettings.selectVideosUrl = Bridge.parentUrl(selectedFiles[0])
            RinaSettings.save()
        }
    }
}
