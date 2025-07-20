import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import com.georgefb.hana

ToolButton {
    id: root

    text: qsTr("Add files")

    onClicked: filesDialog.open()

    FileDialog {
        id: filesDialog

        fileMode: FileDialog.OpenFiles
        currentFolder: HanaSettings.selectVideosUrl
        title: qsTr("@title:window", "Select files")
        nameFilters: ["Video files (*.mkv *.webm *.mp4 *.mov *.ogg *.avi *.avi "+
                      "*.wmv *.flv *.vob *.m2ts *.mts *.ts *.rm *.rmvb *.mpg *.3gp)",
                      "All files (*)"]

        onAccepted: {
            Bridge.filesSelected(selectedFiles)
            HanaSettings.selectVideosUrl = Bridge.parentUrl(selectedFiles[0])
            HanaSettings.save()
        }
    }
}
