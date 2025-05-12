import QtQuick
import QtQuick.Controls

import org.kde.kirigami as Kirigami

import com.georgefb.rina

ListView {
    id: root

    property ListModel filesModel: ListModel {
        id: filesModel
    }

    model: filesModel

    delegate: ItemDelegate {
        id: delegate

        required property string filename

        width: ListView.view.width
        hoverEnabled: true
        contentItem: Label {
            text: delegate.filename
            elide: Text.ElideRight

            ToolTip.text: text
            ToolTip.visible: truncated && delegate.hovered
            ToolTip.delay: Kirigami.Units.shortDuration
        }
    }

    Connections {
        target: Bridge
        function onFilesSelected(urls) {
            const filenames = Bridge.urlsToFilenames(urls)
            console.log(filenames)
            filenames.forEach((file) => filesModel.append({filename: file}))
        }
        function onClearFiles() {
            filesModel.clear()
        }
    }
}
