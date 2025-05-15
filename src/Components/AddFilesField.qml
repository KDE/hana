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
        nameFilters: ["Video files (*.mkv *.webm *.mp4 *.mov *.ogg *.avi *.avi "+
                      "*.wmv *.flv *.vob *.m2ts *.mts *.ts *.rm *.rmvb *.mpg *.3gp)",
                      "All files (*)"]

        onAccepted: {
            Bridge.filesSelected(selectedFiles)
            RinaSettings.selectVideosUrl = Bridge.parentUrl(selectedFiles[0])
            RinaSettings.save()
        }
    }
}
