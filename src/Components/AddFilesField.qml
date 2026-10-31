/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import org.kde.ki18n

import org.kde.hana

ToolButton {
    id: root

    text: KI18n.i18nc("@action:button", "Add files")

    onClicked: filesDialog.open()

    FileDialog {
        id: filesDialog

        fileMode: FileDialog.OpenFiles
        currentFolder: HanaSettings.selectVideosUrl
        title: KI18n.i18nc("@title:window", "Select files")
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
