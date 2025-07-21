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

RowLayout {
    id: root

    enabled: HanaSettings.showVideoInfo

    Label {
        text: i18nc("@label", "Text color")
    }

    Rectangle {
        id: control

        Layout.preferredWidth: Math.ceil(Kirigami.Units.gridUnit * 1.4)
        Layout.preferredHeight: Math.ceil(Kirigami.Units.gridUnit * 1.4)

        color: HanaSettings.videoInfoTextColor
        border.width: 1
        border.color: ma.containsMouse
                      ? Kirigami.Theme.highlightColor
                      : Kirigami.Theme.textColor

        MouseArea {
            id: ma

            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                colorPicker.open()
            }
            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
        }

        ToolTip {
            text: i18nc("@info:tooltip", "Select a color")
            visible: ma.containsMouse
        }

        ColorDialog {
            id: colorPicker

            title: i18nc("@title:window", "Select Color")
            onAccepted: {
                HanaSettings.videoInfoTextColor = colorPicker.selectedColor
                HanaSettings.save()
            }
            selectedColor: HanaSettings.videoInfoTextColor
        }
    }
}
