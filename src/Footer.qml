/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.hana.components

ToolBar {
    RowLayout {
        anchors.fill: parent

        AddFilesField {
            Layout.alignment: Qt.AlignLeft
        }

        ClearFilesField {}

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
