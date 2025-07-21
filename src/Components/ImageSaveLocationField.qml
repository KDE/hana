/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import org.kde.kirigami as Kirigami

import org.kde.hana

ColumnLayout {
    id: root

    spacing: 0

    RowLayout {
        Label {
            Layout.maximumWidth: 250

            text: i18nc("@label", "Save location")
            elide: Text.ElideRight
        }

        ComboBox {
            id: control

            property string value: modelValues[currentIndex]
            property list<string> modelValues: ["SameAsVideo", "NextToVideo", "Custom"]

            model: [
                i18nc("@label:listbox left mouse button", "Same folder as video"),
                i18nc("@label:listbox middle mouse button", "Folder next to video"),
                i18nc("@label:listbox right mouse button", "Custom folder"),
            ]

            Component.onCompleted: {
                const index = modelValues.indexOf(HanaSettings.saveLocation)
                currentIndex = index >= 0 ? index : 0
            }

            onActivated: {
                HanaSettings.saveLocation = value
                HanaSettings.save()
            }
        }
    }

    TextField {
        id: foldername

        Layout.fillWidth: true

        text: HanaSettings.saveLocationFolderName
        visible: control.modelValues[control.currentIndex] === "NextToVideo"

        onTextEdited: {
            HanaSettings.saveLocationFolderName = text
            HanaSettings.save()
        }

        ToolTip {
            text: i18nc("@info:tooltip", "name of the folder")
        }
    }

    Kirigami.ActionTextField {
        id: customFolderPath

        Layout.fillWidth: true

        text: Bridge.urlToLocalFile(HanaSettings.saveLocationFolderUrl)
        visible: control.modelValues[control.currentIndex] === "Custom"
        readOnly: true
        rightActions: [
            Kirigami.Action {
                icon.name: "folder"
                onTriggered: folderDialog.open()
            }
        ]
        onTextEdited: {
            HanaSettings.saveLocationFolderUrl = Bridge.localFileToUrl(text)
            HanaSettings.save()
        }
        ToolTip {
            text: i18nc("@info:tooltip", "path to the folder")
        }
    }

    FolderDialog {
        id: folderDialog

        currentFolder: HanaSettings.saveLocationFolderUrl
        title: i18nc("@title:window", "Select folder")

        onAccepted: {
            Bridge.filesSelected(selectedFolder)
            HanaSettings.saveLocationFolderUrl = Bridge.parentUrl(selectedFolder)
            HanaSettings.save()
        }
    }
}
