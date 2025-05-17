import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import org.kde.kirigami as Kirigami

import com.georgefb.rina

ColumnLayout {
    id: root

    spacing: 0

    RowLayout {
        Label {
            Layout.maximumWidth: 250

            text: qsTr("Save location")
            elide: Text.ElideRight
        }

        ComboBox {
            id: control

            textRole: "text"
            valueRole: "value"

            model: ListModel {
                id: model
                ListElement { text: qsTr("Same folder as video"); value: "SameAsVideo" }
                ListElement { text: qsTr("Folder next to video"); value: "NextToVideo" }
                ListElement { text: qsTr("Custom folder"); value: "Custom" }
            }

            Component.onCompleted: {
                const index = indexOfValue(RinaSettings.saveLocation)
                currentIndex = index >= 0 ? index : 0
            }

            onActivated: {
                RinaSettings.saveLocation = currentValue
                RinaSettings.save()
            }
        }
    }

    TextField {
        id: foldername

        Layout.fillWidth: true

        text: RinaSettings.saveLocationFolderName
        visible: control.currentIndex === control.indexOfValue("NextToVideo")

        onTextEdited: {
            RinaSettings.saveLocationFolderName = text
            RinaSettings.save()
        }

        ToolTip {
            text: qsTr("name of the folder")
        }
    }

    Kirigami.ActionTextField {
        id: customFolderPath

        Layout.fillWidth: true

        text: Bridge.urlToLocalFile(RinaSettings.saveLocationFolderUrl)
        visible: control.currentIndex === control.indexOfValue("Custom")
        readOnly: true
        rightActions: [
            Kirigami.Action {
                icon.name: "folder"
                onTriggered: folderDialog.open()
            }
        ]
        onTextEdited: {
            RinaSettings.saveLocationFolderUrl = Bridge.localFileToUrl(text)
            RinaSettings.save()
        }
        ToolTip {
            text: qsTr("path to the folder")
        }
    }

    FolderDialog {
        id: folderDialog

        currentFolder: RinaSettings.saveLocationFolderUrl
        title: qsTr("@title:window", "Select folder")

        onAccepted: {
            Bridge.filesSelected(folder)
            RinaSettings.saveLocationFolderUrl = Bridge.parentUrl(folder)
            RinaSettings.save()
        }
    }
}
